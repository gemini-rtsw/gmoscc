#!/bin/bash
# Upgrade test for the gmoscc-config sub-repo switch, in a clean rockylinux:9
# WITHOUT git, the way the boot server installs.
#
# The old RPM owns data/*.lut and data/*.pv; the new one does not. rpm erases
# an old package's files that the new package no longer owns, after %post and
# before %posttrans, so this upgrade would delete the live tuned files unless
# %pretrans stashes them and %posttrans restores them. This test fails unless
# every file that was in data/ before the upgrade is unchanged afterwards
# (content, mode, owner), including operations edits made after the old
# install. Then: git as a non-root user, reinstall is a no-op, and rollback.
#
# Dependencies (gem86-epics-runtime, gmos-deplibs, the vxWorks kernel) are
# skipped with --nodeps: only data/ handling is under test here.
#
#   tools/linux-build/rpm/test-config-upgrade.sh <old gmoscc RPM> <new gmoscc RPM>
set -euo pipefail

abspath() { echo "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"; }
[ $# = 2 ] && [ -f "$1" ] && [ -f "$2" ] || { echo "usage: $0 <old gmoscc rpm> <new gmoscc rpm>" >&2; exit 1; }
OLD=$(abspath "$1"); NEW=$(abspath "$2")
echo "old: $(basename "$OLD")"
echo "new: $(basename "$NEW")"

docker run --rm -i -v "$OLD:/rpms/old.rpm:ro" -v "$NEW:/rpms/new.rpm:ro" rockylinux:9 bash -s <<'EOF'
set -euo pipefail
fail() { echo "FAIL: $*"; exit 1; }
D=/gemini/GEM8.6/gmos/gmos
# rpm directly, --nodeps (deps are not under test); --oldpackage because
# git-hash releases do not sort by date.
to() { nvr=$(rpm -qp --qf '%{NVRA}' "$1" 2>/dev/null)
       rpm -U --nodeps --oldpackage --replacepkgs "$1"
       [ "$(rpm -q gmoscc)" = "$nvr" ] || fail "could not switch to $nvr"; echo "  now $nvr"; }
snap() { (cd $D/data && find . -path ./.git -prune -o -type f -printf '%p %m %u:%g ' -exec md5sum {} \; | awk '{print $1,$2,$3,$4}' | sort); }

dnf -y -q install diffutils tar findutils >/dev/null
rpm -q git >/dev/null && fail "git is installed; this test needs a host without it"

echo "== fresh install of the new RPM: tuned files seeded from data.dist"
rpm -i --nodeps /rpms/new.rpm
for f in $D/data.dist/*; do cmp -s "$f" $D/data/$(basename "$f") || fail "fresh install: $(basename "$f") not seeded"; done
[ -d $D/data/.git ] || fail "fresh install: data/.git missing"
echo "  $(ls $D/data.dist | wc -l) tuned files seeded, data/.git present"
rpm -e --nodeps gmoscc
rm -rf $D

echo "== old RPM, then operations edits like the boot server's"
rpm -i --nodeps /rpms/old.rpm
rpm -q gmoscc | sed 's/^/  /'
[ -f $D/data/gr.lut ] || fail "old RPM did not install data/gr.lut"
echo "# retuned in operations" >> $D/data/gr.lut
echo "# retuned in operations" >> $D/data/gmOiwfs.pv
chmod 0664 $D/data/flt.lut                                   # a mode change must survive too
cp -p $D/data/gr.lut $D/data/gr.lut.2026.11.13-local         # a local backup
clash=aux_30000004gr.lut.20231005                           # a dated backup in gmoscc-config
echo "local copy, not the repo's" > $D/data/$clash            # name clash with a repo backup
snap > /tmp/before

echo "== upgrade to the new RPM (no git on the host)"
to /rpms/new.rpm
snap > /tmp/after
missing=$(comm -23 <(cut -d' ' -f1 /tmp/before) <(cut -d' ' -f1 /tmp/after))
[ -z "$missing" ] || { echo "$missing"; fail "files deleted from data/"; }
changed=$(join -j1 <(sort /tmp/before) <(sort /tmp/after) | awk '$2!=$5||$3!=$6||$4!=$7{print $1}')
[ -z "$changed" ] || { echo "$changed"; fail "files already in data/ changed"; }
echo "  every file already in data/ present and unchanged ($(wc -l < /tmp/before) files)"
added=$(comm -13 <(cut -d' ' -f1 /tmp/before) <(cut -d' ' -f1 /tmp/after) | sed 's|^\./||')
echo "  added: $(echo "$added" | grep -c .) files"
grep -q 'retuned in operations' $D/data/gr.lut || fail "tuned gr.lut lost its edit"
grep -q 'retuned in operations' $D/data/gmOiwfs.pv || fail "tuned gmOiwfs.pv lost its edit"
[ "$(stat -c %a $D/data/flt.lut)" = 664 ] || fail "flt.lut mode not kept"
grep -q "local copy" $D/data/$clash || fail "local backup overwritten by the repo's"
[ -d $D/data/.git ] || fail "data/.git missing"
[ -e $D/.tuned-stash ] && fail "stash left behind"
ls -d $D/.config-repo.* 2>/dev/null && fail "temp dir left behind"
rpm -qf $D/data/gr.lut >/dev/null 2>&1 && fail "gr.lut still owned by a package"
[ -f $D/data/gmosTop.dl ] || fail "static .dl not installed"
rpm -qf $D/data/gmosTop.dl >/dev/null || fail "static .dl not owned by the package"
echo "  tuned edits, mode, local backups kept; tuned files unowned; static files owned"

echo "== git as a non-root user"
dnf -y -q install git >/dev/null
rpm -U --nodeps --replacepkgs /rpms/new.rpm    # %post sets safe.directory now git exists
git config --system --get-all safe.directory | grep -qx $D/data || fail "safe.directory not set"
useradd -m sci
su - sci -c "cd $D/data && git status --short | head -20 | sed 's/^/  /'"
su - sci -c "cd $D/data && git -c user.name=sci -c user.email=sci@example.invalid commit -qam 'ops tuning' && git log --oneline | sed 's/^/  /'"
head=$(git -C $D/data rev-parse HEAD)

echo "== reinstall: no-op for data/"
snap > /tmp/before
rpm -U --nodeps --replacepkgs /rpms/new.rpm
snap > /tmp/after
cmp -s /tmp/before /tmp/after || { diff /tmp/before /tmp/after; fail "reinstall changed data/"; }
[ "$(git -C $D/data rev-parse HEAD)" = "$head" ] || fail "reinstall changed data/.git"
echo "  data/ and its git history unchanged"

echo "== rollback to the old RPM (it owns the tuned files again)"
to /rpms/old.rpm
[ -d $D/data/.git ] || fail "rollback removed data/.git"
n=$(git -C $D/data status --short | grep -c '^ M' || true)
echo "  old RPM overwrote $n tracked files with its own copies (expected: it owns them)"
git -C $D/data checkout -- .
grep -q 'retuned in operations' $D/data/gr.lut || fail "committed tuning not recoverable after rollback"
echo "  git checkout restored committed tuning"
echo "PASS"
EOF

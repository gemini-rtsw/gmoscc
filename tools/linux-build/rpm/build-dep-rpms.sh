#!/bin/bash
# Build the gmoscc dependency RPMs from staged trees (one-time bootstrap; they
# change essentially never). Output: rpm/out/.
# Publish them to rpm-repo with gemini-rtsw-repo's upload-rpm.sh.
#
# Each spec needs its own subtree staged under $GMOSCC_BUILDENV. A spec whose
# tree is absent is skipped with a message rather than failing the run, because
# the trees come from different hosts and are rarely all present at once:
#
#   usr/software/...                      gem-tornado22-linux, gem-epics3139gem86
#   gemini/GEM8.6/...                     gem86-deplibs
#   gemini/external/vxWorks/tornado2.2    gem-vxworks-tornado22
#   gemini/external/GEM8.6/...            gem86-epics-runtime
#   gemini/GEM8.6/{ast,sla,time}lib       gmos-deplibs (GMOS-private copy)
#
# Stage the last one from the boot server, e.g.
#   mkdir -p "$GMOSCC_BUILDENV/gemini/external/vxWorks"
#   rsync -a /net/mkotcsbootv2-lv1/export/gemini/external/vxWorks/tornado2.2 \
#         "$GMOSCC_BUILDENV/gemini/external/vxWorks/"
set -e

HERE="$(cd "$(dirname "$0")" && pwd)"
POLARIS="${GMOSCC_BUILDENV:-$HOME/work/gmoscc-buildenv/polaris}"
[ -d "$POLARIS" ] || POLARIS="$HERE/../polaris"
OUT="$HERE/out"
mkdir -p "$OUT"

# spec:required-subtree
CANDIDATES="
gem-tornado22-linux:usr/software
gem-epics3139gem86:usr/software
gem86-deplibs:gemini/GEM8.6
gem-vxworks-tornado22:gemini/external/vxWorks/tornado2.2
gem86-epics-runtime:gemini/external/GEM8.6
gmos-deplibs:gemini/GEM8.6
"
SPECS=""
for entry in $CANDIDATES; do
    spec="${entry%%:*}"; need="${entry##*:}"
    if [ -e "$POLARIS/$need" ]; then
        SPECS="$SPECS $spec"
    else
        echo "skipping $spec — $POLARIS/$need not staged"
    fi
done
[ -n "$SPECS" ] || { echo "ERROR: nothing staged under $POLARIS; unpack the trees first" >&2; exit 1; }
echo "building:$SPECS"

docker run --rm \
    -v "$POLARIS:/trees:ro" \
    -v "$HERE:/specs:ro" \
    -v "$OUT:/out" \
    -e SPECS="$SPECS" \
    rockylinux:9 bash -c '
        set -e
        dnf install -y -q rpm-build
        for s in $SPECS; do
            echo "==== building $s"
            rpmbuild -bb --define "trees /trees" --define "_rpmdir /out" /specs/$s.spec
        done
        ls -l /out/*/
    '
echo "RPMs in $OUT/"

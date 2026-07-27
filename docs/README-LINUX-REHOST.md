# Porting a Gemini EPICS/vxWorks IOC build from Solaris to Linux

How we moved the gmoscc build off the Solaris SPARC host `polaris` and into a
Linux container / CI pipeline (REL-4693), written as a procedure for doing the
same to the other IOCs.

**Read this first if you are porting a different IOC.** Each IOC pins its own
GEM software-tree version (GEM8.4, GEM8.6, ...), and each GEM version means a
different EPICS tree, different host tools and a **separate set of dependency
RPMs**. The *procedure* below is reusable; the *artifacts* are not. Budget a
from-scratch pass per GEM version — roughly a day for the first IOC on a new
GEM version, then hours for further IOCs sharing that version.

---

## 1. Why this works at all

The Gemini UAE build system is almost entirely host-portable. Three facts make
the rehost possible:

1. **The cross-compiler exists for Linux.** Wind River's Tornado GNU tools are
   GPL'd, so ANL publishes Linux x86 rebuilds from Wind River's own sources:
   <https://epics.anl.gov/base/tornado-linux.php> (`gnu-tools.tor2_2-ppc-rhel5.tgz`
   for Tornado 2.2 / vxWorks 5.5 / ppc40x-60x-860). As a vxWorks licensee
   Gemini is entitled to those sources.
2. **EPICS 3.13 already supports `HOST_ARCH=Linux`.** The GEM tree ships
   `CONFIG.Host.Linux`, `CONFIG_HOST_ARCH.Linux` (which already points at
   `$WIND_BASE/host/x86-linux`) and `include/os/Linux` headers. Gemini even
   started this work years ago — commented-out `HOST_ARCH=Linux` lines and a
   `copyLinux` script are in the tree.
3. **The host tools rebuild cleanly on a modern distro.** `dbExpand`, `snc`,
   `macTest`, `antelope`, `e_flex`, `e2db` all compiled under gcc 12 with one
   trivial patch (see §5).

What is *not* portable: the commercial Capfast schematic tools and the
`adl2dl`/`edd` display converters. See §7 — both are already dead on Solaris
too, so neither is a regression.

---

## 2. Inventory the Solaris host first

Everything downstream depends on these answers. On the Solaris build host,
after sourcing the GEM environment (e.g. `GEM8.6`):

```csh
which ccppc ; ccppc -v ; file `which ccppc`
echo $WIND_BASE ; ls $WIND_BASE/host
echo $EPICS ; ls $EPICS/base/bin $EPICS/extensions/bin
which applSetup.pl snc dbExpand e2db sch2edif adl2dl
alias GEM8.6                  # -> which setup file to copy
du -sk $EPICS $WIND_BASE/target /gemini/<GEMVER>
```

Record: Tornado version, gcc version banner, EPICS version + path, the
`HOST_ARCH` name used (`solaris`), the target arch (`ppc604_long`), and which
tools are missing/aliased-to-nothing. **Check the compiler banner against the
ANL tools** — ours matched at the `gcc-2.96 / VxWorks 5.5` level but with a
different Wind River patch level (see §8).

---

## 3. Copy the trees off the Solaris host

Solaris `tar` has no `z` flag; pipe through gzip. Write to `/var/tmp` (on
Solaris 8, `/tmp` is swap-backed and a large tarball can exhaust swap).

```csh
tar cf - $HOME/.gem<VER> <builddir>/config | gzip -c > /var/tmp/gem-config.tar.gz
tar cf - $WIND_BASE/target/h $WIND_BASE/target/config | gzip -c > /var/tmp/wind-target.tar.gz
tar cf - $EPICS | gzip -c > /var/tmp/epics.tar.gz
tar cf - /gemini/<GEMVER>/astlib /gemini/<GEMVER>/slalib /gemini/<GEMVER>/timelib \
    | gzip -c > /var/tmp/gem-libs.tar.gz
```

Then `scp` them to a working machine and unpack them **outside the repo**
(they are proprietary and large). We used `~/work/<ioc>-buildenv/polaris`,
selectable via a `GMOSCC_BUILDENV`-style env var.

Note: these tarballs are the **only** copies of some of this software. Keep
them; they are the escrow for anything the RPMs later omit.

---

## 4. Assemble the Linux build environment

Layout the trees at their **original absolute paths** — the generated
`config/CONFIG.Defs` and the EPICS rules refer to them, so keeping the paths
identical avoids editing anything:

```
/usr/software/dev/packages/epics/epics<VER>GEM<VER>/   (EPICS base+extensions)
/usr/software/dev/packages/vxworks/tornado2.2/ppc/     (WIND_BASE)
/gemini/GEM<VER>/{astlib,slalib,timelib}
```

Add the ANL cross-toolchain into `$WIND_BASE`:

```sh
curl -fsSLO https://epics.anl.gov/base/gnu-tools.tor2_2-ppc-rhel5.tgz
tar xzf gnu-tools.tor2_2-ppc-rhel5.tgz -C <...>/tornado2.2/ppc   # adds host/x86-linux
```

Container needs (see `tools/linux-build/Dockerfile.el9`):

- `glibc.i686` — **the cross-compiler binaries are 32-bit i386 ELF**
- `make` (symlinked as `gmake`), `perl`, `tcsh`, `gcc`, `gcc-c++`
- `flex`, `libfl-dev`/`byacc`, `bison` — for rebuilding the host tools
- `/export/software` → `/usr/software` symlink if the Solaris `CONFIG.Defs`
  used the automounter path
- an `/etc/profile.d/gem<VER>.sh` that exports `EPICS`, `EPICS_BASE`,
  `HOST_ARCH=Linux`, `WIND_BASE`, `WIND_HOST_TYPE=x86-linux` and prepends the
  tool directories to `PATH` — the Linux equivalent of the Solaris
  `~/.gem<VER>` + `base/config/epics.csh`

---

## 5. Rebuild the EPICS host tools for `HOST_ARCH=Linux`

Two preparatory steps, then the builds.

**a. Seed `bin/Linux` with the script tools.** Many "tools" are perl/sh and
host-independent, but the build needs them present before it can bootstrap:

```sh
cd $EPICS
for d in base extensions; do
    mkdir -p $d/bin/Linux
    for f in $d/bin/solaris/*; do
        file -b "$f" | grep -q ELF || cp -p "$f" $d/bin/Linux/
    done
done
```

**b. Fix the perl shebang.** `applSetup.pl` hardcodes the Solaris perl path;
symlink it: `/usr/software/dev/solaris/bin/perl -> /usr/bin/perl`.

**c. Build, in this order** (later dirs need earlier outputs):

```sh
for d in tools include libCom toolsComm dbStatic sequencer uae; do
    (cd $EPICS_BASE/src/$d && gmake CROSS_COMPILER_TARGET_ARCHS= SHRLIB_VERSION=)
done
(cd $EPICS/extensions/src/edif && gmake CROSS_COMPILER_TARGET_ARCHS= SHRLIB_VERSION=)
```

The two overrides matter: `CROSS_COMPILER_TARGET_ARCHS=` stops base from also
cross-building for the target, and `SHRLIB_VERSION=` works around an install
rule that otherwise self-links `.a` files ("Too many levels of symbolic
links").

**Patches needed** (both in `tools/linux-build/patches/`):

1. `extensions/src/edif/e2cDefParse.y` — declare `yylineno`/`erroryy` in the
   yacc prologue; modern bison/gcc reject the implicit declarations.
2. `base/config/CONFIG_SITE.Host.Linux` — it hardcodes `GCC = /usr/local/bin/gcc`;
   use `gcc` from `PATH` (or symlink, as our container does for belt-and-braces).

Expect these host tools afterwards: `dbExpand`, `snc`, `macTest`, `antelope`,
`e_flex`, `e2db`, `e2sr`, `dbToRecordtypeH`, `dbToMenuH`.

---

## 6. Per-checkout bootstrap and build

The only steps that cannot be baked into an image, because `applSetup.pl`
stamps the checkout's **absolute path** into the generated config:

```sh
echo "APPLIC_TOP = $(pwd)" > .applTop
applSetup.pl -T <target_arch> -I <dirs...> -d <deplibs...> -S <site>
gmake
```

Take the exact arguments from the IOC's own `*Install` script (for gmoscc,
`gmosInstall`). See `tools/linux-build/setup.sh` for a worked version that also
scrubs stale state first.

**Critical gotcha:** `applSetup.pl` *preserves* `APPLIC_INSTALL` from an
existing `config/CONFIG.Defs`. A checkout bootstrapped in one container and
re-bootstrapped in another (different mount path) will keep the **stale** path
— failing loudly if the path is absent, or silently baking a wrong path into
generated startup scripts if it exists. Always delete `config/` (and the other
generated dirs) before re-bootstrapping.

Generated, never committed: `config/ bin/ lib/ include/ dbd/ data/ O.*/
Distfile .applTop`. Add a `.gitignore` for them — these repos predate git and
only have `.cvsignore`.

---

## 7. Dead tools: Capfast and the display screens

**Capfast is no longer supported.** The commercial Phase Three Logic tools
(`sch2edif`, `schplot`, `schps`) convert `capfast/*.sch` schematics to EDIF,
which `e2db` then turns into EPICS `.db` databases. As of July 2026:

- `sch2edif` **fails on polaris** — it exits 255 with the vendor's phone number
  after a FlexLM licence failure (`CAPFAST_LMHOST=@aguila.cl.gemini.edu`, a
  Cerro Pachón licence server). Capfast is a dead product; the licence is not
  renewable.
- Therefore **schematics can no longer be edited or rebuilt on any host** —
  this is not a Linux limitation.
- `e2db` itself is an open EPICS *extension* and rebuilds fine for Linux, so
  only the `.sch → .edf` step is lost.

**Mitigation, and the pattern to repeat:** commit the generated `.db` files to
git (we put them in `capfast/db/`) and seed them into the build directory so
make's chained-intermediate logic treats the chain as satisfied:

```sh
mkdir -p capfast/O.<HOST_ARCH>
cp capfast/db/*.db capfast/O.<HOST_ARCH>/
touch capfast/O.<HOST_ARCH>/*.db      # must be NEWER than the .sch files
```

The `touch` is required: make regenerates anything older than its prerequisite.
The `.edf` intermediates are deleted by make, which is why `.db` — not `.edf` —
is the right thing to commit. **Any future database change must be made by
editing the `.db` files directly**, with the `.sch` schematics kept only as
historical documentation of intent. Note this loses the schematic as the source
of truth; flag it to whoever owns the IOC's databases.

**The MEDM/EDD display screens are no longer built.** `adl2dl` converts
`adl/*.adl` sources into the `.dl` screens engineers open. On polaris the
`~/.gem<VER>` file aliases `adl2dl` → `adl2dl2.4` and `edd` → `eddadl2.4`, and
**neither binary exists** — the aliases have been broken for years. So:

- The `adl` directory cannot build on Solaris *or* Linux. Remove it from
  `Makefile.Dirs` for the build (`setup.sh` does this).
- The `.dl` files currently in production `data/` are **stale artifacts** that
  rdist never deleted; they are not reproducible from source by any build.
- Consequently a CI-built payload **does not contain display screens**. If the
  screens matter, either commit the existing `.dl` files as versioned data, or
  investigate rebuilding `adl2dl`/`edd` from the EPICS extensions tree for
  Linux (the same approach that worked for `e2db` — untried).

---

## 8. Validate the port: compare objects against a Solaris build

Do not compare `.a` archives or md5 whole directories: archives embed
timestamps and uid/gid, so they never match even when the code is identical.
Compare **object files**, and build **the same source** on both hosts.

```sh
# on the Solaris host, from a build of the same commit:
tar cf - src/*/O.<target> db/O.<target> | gzip -c > /var/tmp/objs.tar.gz
```

Then, per object, strip the things that legitimately differ:

```sh
objcopyppc --strip-debug -R .comment <solaris>.o /tmp/a.o
objcopyppc --strip-debug -R .comment <linux>.o   /tmp/b.o
cmp /tmp/a.o /tmp/b.o
```

`-g` debug sections embed build paths; `.comment` holds the compiler version
banner. Both differ by construction.

**For objects that still differ, compare per function, never by address.** A
different compiler can change code size, which shifts every later address —
so address-keyed comparison silently compares unrelated instructions and
manufactures false findings (this cost us two wrong conclusions). Extract each
function by name from `objdumpppc -d` output and compare its instruction
sequence. `tools/linux-build/` history has a worked script; note objdump
separates mnemonic and operands with a **tab**.

### What we found for gmoscc (V7-17, same source both hosts)

40 objects: **16 byte-identical**, 24 differing. Across **384 functions
containing comparisons: 347 identical, 37 differing only in compare mnemonic,
0 operand mismatches.** Exactly two systematic differences, both benign:

1. **`-mlongcall` applied more consistently.** The ANL compiler routes every
   call through a register (`lis/addi/mtlr/blrl`); polaris's emitted some direct
   `bl` despite `-mlongcall` being in `ARCH_DEP_CFLAGS`. Direct `bl` has a
   ±32 MB reach, so the newer behaviour is *stricter and safer*. This accounts
   for the size growth and all the address shifts.
2. **`cmplwi` vs `cmpwi`** (unsigned vs signed immediate compare) in 37 places,
   with identical register, constant and following branch. In every case the
   operand is provably non-negative — loaded with `lhz` (zero-extending) and
   masked with `clrlwi ...,16`, so it holds 0–65535 and the two instructions
   give identical results. It is a free optimizer tie-break, not a semantic
   change. (Where the source really is signed, both compilers emit `extsh`
   first.)

**Conclusion:** the toolchains are not bit-identical, so "byte-identical
therefore safe" is unavailable. But there are no differing constants, branch
conditions, call targets or operands anywhere in the build. Normal IOC release
testing is the appropriate remaining validation. If bit-exactness is ever
required, Wind River's original GCC sources (available to licensees) can be
built for Linux with ANL's build scripts.

Also verify the non-compiled outputs, which should match **exactly**: files
copied from the EPICS tree (e.g. `gemini.Support`, `mv167st`) prove the EPICS
tree is intact, and the hand-written `start*` scripts prove the data-install
path works. Generated `*Startup*` scripts and `local` will differ — they embed
`APPLIC_INSTALL` (see §10).

---

## 9. CI integration (gemini-rtsw-ci)

The pipeline builds a `.spec` inside `rockylinux:<el>` with dependencies from
`ghcr.io/gemini-rtsw/rpm-repo`, publishes the RPM back, and uploads it as an
Actions artifact. To join an IOC:

**a. Package the build environment as dependency RPMs**, one set per GEM
version. For gmoscc (see `tools/linux-build/rpm/`):

| RPM | Contents | Installs at |
|---|---|---|
| `gem-tornado22-linux` | ANL cross-tools + `target/{h,config}` | `/usr/software/dev/packages/vxworks/tornado2.2/ppc` |
| `gem-epics3139gem86` | EPICS config/rules/headers/dbd/templates, Linux host tools, target artifacts, `/etc/profile.d/gem86.sh` | `/usr/software/dev/packages/epics/epics3.13.9GEM8.6` |
| `gem86-deplibs` | astlib/slalib/timelib | `/gemini/GEM8.6` |

These specs have no `%build` — they only `cp -a` the prepared trees into the
buildroot, trimming sources and dead arches. `rpm/build-dep-rpms.sh` builds
them in a container so the process is reproducible.

> **Naming convention — important for multiple GEM versions.** Gemini runs
> several GEM trees in parallel, so anything tied to one carries the version in
> its **name** (`gem86-deplibs`, `gem-epics3139gem86`; a GEM8.4 set would be
> `gem84-deplibs`, `gem-epics<ver>gem84`) **and** installs under
> version-specific paths. Different generations are then distinct,
> co-installable packages, and an IOC's `BuildRequires` names its generation
> unambiguously. Tornado is generation-neutral (`gem-tornado22-linux`) and is
> shared by every IOC on the same Tornado version.

Publish with `gemini-rtsw-repo/upload-rpm.sh --tag-only <rpms>` (pushes each
RPM as a `FROM scratch` image tagged `rpm-<NVRA>`), then run the
**`rebuild-latest`** workflow in `gemini-rtsw-repo` on GitHub to fold the new
tags into the served `rpm-repo:latest`. Do the rebuild on a runner, not a
laptop: it pulls ~900 tags, and `sync_repo.sh` uses GNU-only `find -printf`
plus `timeout`, so it does not run as-is on macOS.

**b. Add the IOC's `.spec`.** `BuildRequires` the dependency RPMs; `%build`
sources the profile script and runs the per-checkout bootstrap + `gmake`;
`%install` mirrors the historical rdist payload. A `%package devel` section is
**mandatory** — `build_rpm.sh` hard-fails without one, because the dev image
installs the `-devel` RPM to pull in the pinned deps.

**c. Add the submodule and workflow:**

```sh
git submodule add -b main https://github.com/gemini-rtsw/gemini-rtsw-ci.git gemini-rtsw-ci
```

`.github/workflows/ci.yml` calls the reusable `ci.yml` + `publish.yml`. Note
`el_version` **defaults to 8** everywhere — an EL9-only IOC must pass `el: ['9']`
in the matrix, and the local scripts need `--el 9` explicitly.

**d. Grant the IOC repo Write on the `rpm-repo` package** (org → Packages →
rpm-repo → settings → Manage Actions access), or the build fails pulling or
publishing.

**e. `custom-repo-setup.sh`** runs in the build container before
`dnf builddep` — use it for anything the spec cannot express. We need
`dnf -y upgrade glibc`, because `glibc.i686` (for the 32-bit cross-tools) must
exactly match the x86_64 glibc, and the base image can lag the mirrors enough
to make the transaction unresolvable.

---

## 10. Deploy: the open question

The historical deploy is `gmake rdist` → `Distfile` → rdist to the IOC's NFS
tree (from a Linux host — Solaris and Linux rdist protocols are incompatible),
then a symlink flip to activate. Our RPM payload deliberately mirrors that same
tree so the deploy step can stay unchanged.

**But a CI-built RPM is not yet IOC-bootable.** `APPLIC_INSTALL` is stamped
into the generated `bin/<target>/local` and startup scripts, and the IOC reads
that path when it NFS-mounts and boots. A CI build bakes the rpmbuild path.
Options, undecided: install the RPM on the deploy host so the path is real;
re-home the tree at install time; or teach the deploy script to consume a CI
artifact. Until then, treat CI output as **verification-grade** and keep
deploying from a build made where the IOC can see it.

---

## 11. Checklist for the next IOC

- [ ] Inventory the Solaris host (§2) — Tornado/EPICS/GEM versions, missing tools
- [ ] Copy trees off (§3), keep the tarballs as escrow
- [ ] Assemble the environment at original paths + ANL toolchain (§4)
- [ ] Rebuild host tools for `HOST_ARCH=Linux`, apply the two patches (§5)
- [ ] Bootstrap + build; add a `.gitignore` for generated dirs (§6)
- [ ] Commit the Capfast `.db` outputs; drop `adl` from `Makefile.Dirs` (§7)
- [ ] Build the same commit on Solaris and compare objects per function (§8)
- [ ] Package per-GEM-version dependency RPMs with versioned names (§9a)
- [ ] Add spec (with `%package devel`), submodule, `ci.yml`, package access (§9b–d)
- [ ] Decide the deploy story before shipping to a real IOC (§10)

## References

- ANL Linux cross-tools: <https://epics.anl.gov/base/tornado-linux.php>
- gmoscc worked example: `tools/linux-build/` (this repo), branch `REL-4693`
- Pipeline docs: `gemini-rtsw-ci/README.md`, `gemini-rtsw-ci/WORKFLOW.md`

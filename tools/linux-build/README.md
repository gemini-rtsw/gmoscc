# Building gmoscc on Linux (REL-4693)

Builds gmoscc (EPICS 3.13.9 GEM8.6, VxWorks 5.5 target `ppc604_long`) on a
Linux host in Docker, replacing the Solaris SPARC build host `polaris`.

The short version: the Gemini UAE build system is almost entirely
host-portable. The cross-compiler (`ccppc`, gcc 2.96 for Tornado 2.2) exists
as Linux x86 binaries [rebuilt by ANL from Wind River's GPL'd
sources](https://epics.anl.gov/base/tornado-linux.php); the EPICS host tools
(`dbExpand`, `snc`, `macTest`, `e2db`, ...) compile fine on a modern Linux;
and EPICS 3.13.9 already ships `HOST_ARCH=Linux` support that Gemini started
wiring up years ago. The only tools with no Linux path are the commercial
Capfast `sch2edif` (schematic → EDIF) and `adl2dl` — both handled below.

## Quick start

Prerequisites: Docker; the build-environment trees (one-time setup below).

```sh
./tools/linux-build/build.sh              # applSetup (if needed) + gmake
./tools/linux-build/build.sh --clean      # wipe generated files, full rebuild
./tools/linux-build/build.sh --shell      # interactive shell in the build env
```

Run from the repo top (or anywhere — the script finds the top). First run
builds the Docker image automatically. A full build takes a few minutes.

### One-time setup of the build environment

The build needs ~450 MB of trees copied from polaris (proprietary — not in
git). The tarballs currently live in `~/work/gmoscc-polaris/`; unpack them:

```sh
cd tools/linux-build && mkdir -p polaris && cd polaris
for f in epics3139gem86 wind-target gem-libs gem-config gemini-external; do
    tar xzf ~/work/gmoscc-polaris/$f.tar.gz
done
cd .. && ./fetch-gnu-tools.sh
tar xzf gnu-tools.tor2_2-ppc-rhel5.tgz \
    -C polaris/usr/software/dev/packages/vxworks/tornado2.2/ppc
```

Then apply the local modifications listed under
[Modifications to the polaris copy](#modifications-to-the-polaris-copy),
and build the EPICS host tools once (they self-install into the tree):

```sh
./build.sh --shell
# inside the container:
for d in tools include libCom toolsComm dbStatic sequencer uae; do
    (cd $EPICS_BASE/src/$d && gmake CROSS_COMPILER_TARGET_ARCHS= SHRLIB_VERSION=)
done
(cd $EPICS/extensions/src/edif && gmake CROSS_COMPILER_TARGET_ARCHS= SHRLIB_VERSION=)
```

## What the build produces, and where

The UAE build installs into the checkout itself (`APPLIC_INSTALL` =
`APPLIC_TOP` = the repo top), exactly as it does on polaris:

| Location             | Contents                                                                 |
|----------------------|--------------------------------------------------------------------------|
| `bin/ppc604_long/`   | The IOC payload: loadable objects (`gmosLib.a`, `deviceControlLib.a`, `gemini.Support`, snc CARs), generated startup scripts (`ccStartup*`, `gmStartup*`), and `local` — the boot script the IOC runs after NFS-mounting this tree |
| `bin/Linux/`         | Host-side utilities (`gmosLutLib`, `gmOiwfsCalibrate`, `libCalc`, ...)   |
| `dbd/`               | `gemini.dbd` (dbExpand output) and the application `.dbd` files          |
| `data/`              | LUTs, `.pv` files, alarm-handler configs, and the top-level `.db` databases |
| `config/`            | Generated build configuration (`CONFIG.Defs` — from `applSetup.pl`)      |

All of these are git-ignored (see `.gitignore`); `--clean` removes them.

Deployment is unchanged for now: the IOC boots over NFS from a tree on the
`/gemini` filesystem, deployed with `gmake rdist` + `tools/deploy.sh` from
pisces (see the top-level README). A container build on a laptop produces a
correct tree, but its baked-in `APPLIC_INSTALL` path (`/build/gmoscc`) is only
meaningful in the container — deployable builds need to be made on a host
that shares the `/gemini` namespace, or the deploy flow needs to re-home the
tree (future work, see CI section).

## How it works

- `Dockerfile.el9` (Rocky Linux 9, canonical — same OS family as the
  gemini-rtsw-ci pipeline) and `Dockerfile` (Debian, the original pilot
  image) provide: 32-bit glibc for the cross-tools, gcc/g++ for host tools,
  `gmake`/`perl`/`tcsh`, and two compat shims (GEM8.6's
  `CONFIG_SITE.Host.Linux` hardcodes `/usr/local/bin/gcc`; `/export/software`
  is polaris's automounter alias for `/usr/software`).
- The polaris trees are bind-mounted at their real paths:
  `/usr/software/dev/packages/...` (EPICS + Tornado) and `/gemini/GEM8.6`
  (astlib/slalib/timelib). Nothing is baked into the image, so tree updates
  don't require image rebuilds.
- `gem-env.sh` replicates polaris's `~/.gem8.6` + `epics.csh` environment
  with `HOST_ARCH=Linux` / `WIND_HOST_TYPE=x86-linux`. The EPICS build
  system does the rest: `CONFIG_HOST_ARCH.Linux` already points at
  `$WIND_BASE/host/x86-linux`, which is where the ANL cross-toolchain lives.
- `build.sh` writes `.applTop`, runs `applSetup.pl` (same arguments as
  `gmosInstall`) if `config/` is missing, and runs `gmake`.
- EPICS host-tool builds need `CROSS_COMPILER_TARGET_ARCHS= SHRLIB_VERSION=`
  overrides: the first stops base from also cross-building, the second works
  around a versioned-symlink install rule that self-links `.a` files.

### The two Solaris-only tools

- **Capfast `sch2edif`** (commercial, FlexLM-licensed): compiles the 170
  `capfast/*.sch` schematics. Its end products are 9 top-level `.db` files;
  the `.edf` intermediates are deleted by make, and the open-source half of
  the chain (`e2db`) is rebuilt for Linux. Until the `.db` files are
  committed to the repo (planned — the UAE `DATA` install rule handles
  committed files natively), `build.sh` seeds them from the polaris V7-16
  reference build. Editing a `.sch` requires Capfast and therefore polaris
  (or the QEMU-Solaris lifeboat) regardless of where the build runs.
- **`adl2dl`**: missing on polaris too — the `.dl` display generation has
  been failing (ignored) there for years. The `adl` directory is the one
  remaining build failure on Linux; decision pending (commit the existing
  `.dl` files, or drop the directory from the build).

## Modifications to the polaris copy

Everything under `polaris/` is a verbatim copy of the Solaris trees except:

1. ANL gnu-tools extracted into `$WIND_BASE/host/x86-linux` (see Quick start).
2. `base/bin/Linux` + `extensions/bin/Linux` seeded with the non-ELF (script)
   tools from the corresponding `bin/solaris` — they're host-independent
   perl/sh:
   ```sh
   cd polaris/usr/software/dev/packages/epics/epics3.13.9GEM8.6
   for d in base extensions; do
       mkdir -p $d/bin/Linux
       for f in $d/bin/solaris/*; do
           file -b "$f" | grep -q ELF || cp -p "$f" $d/bin/Linux/
       done
   done
   ```
3. `patches/01-edif-e2cDefParse-yylineno.patch` applied
   (`patch -p1 -d polaris < patches/...`): declares `yylineno`/`erroryy`
   in the e2db yacc grammar prologue.
4. `dev/solaris/bin/perl` symlink → `/usr/bin/perl` (applSetup.pl's shebang):
   ```sh
   mkdir -p polaris/usr/software/dev/solaris/bin
   ln -sf /usr/bin/perl polaris/usr/software/dev/solaris/bin/perl
   ```
5. Host-tool builds add `O.Linux`, `bin/Linux`, `lib/Linux`,
   `include/os/Linux` — additive only; the Solaris artifacts are untouched.

## Validation

Byte-comparison against the polaris **V7-16** build (note: V7-16 predates
the REL-4969 commit, so `deviceControl`/`assemblyControl` objects differ by
source). Of 40 `.o` files, after stripping debug info and `.comment`:

- **16 byte-identical.**
- **2 identical except local-symbol spelling** — the two compiler builds
  name function-local statics differently (`mode.3` vs `mode$3`); invisible
  to the VxWorks loader.
- **10 differ as expected** from REL-4969's `MVCounter` struct field
  (all of `src/deviceControl` + `src/assemblyControl`).
- **12 differ only by alignment padding**: the ANL toolchain was built from
  Wind River's 2012 cumulative-patch sources (`ccppc -v`: `2.96+ MW/LM`),
  polaris runs GA 2002 (`2.96+`). The patched compiler inserts a `nop`
  after some unconditional branches; the instruction streams are otherwise
  identical.

A same-source comparison against a polaris **V7-17** build is in progress to
confirm that alignment nops and symbol spelling are the *only* compiler-level
deltas. If bit-exactness is ever required, Wind River's GA-2002 GCC sources
(available to vxWorks licensees) can be rebuilt for Linux with ANL's build
scripts.

## CI integration (gemini-rtsw-ci) — plan

The gemini-rtsw-ci pipeline builds a `.spec` inside `rockylinux:<el>` with
dependencies pulled from the `ghcr.io/gemini-rtsw/rpm-repo` image, publishes
the RPM back to rpm-repo, and uploads it as an Actions artifact. gmoscc fits
this natively once the build environment is packaged as RPMs; the EL9 image
in this directory proves the build works on the pipeline's OS.

Plan (in order):

1. **Commit the 9 capfast `.db` files** to the repo (after the V7-17
   comparison confirms them) so CI needs no polaris seed.
2. **Package the build environment as three dependency RPMs** (specs can
   live in this directory; built once from the polaris tarballs, published
   to rpm-repo via `upload-rpm.sh` — they essentially never change):
   - `gem-tornado22-linux` (~90 MB): ANL cross-tools + `$WIND_BASE/target/{h,config}`
     → `/usr/software/dev/packages/vxworks/tornado2.2/ppc`
   - `gem-epics3.13.9GEM8.6` (~300 MB): the EPICS tree with `bin/Linux` host
     tools prebuilt → `/usr/software/dev/packages/epics/epics3.13.9GEM8.6`
   - `gem86-deplibs` (~100 MB): astlib/slalib/timelib → `/gemini/GEM8.6`
3. **Add `gmoscc.spec`**: `BuildRequires` the three; `%build` = `.applTop` +
   `applSetup.pl` + `gmake`; `%install` packages the IOC tree
   (`bin/ppc604_long`, `dbd`, `data`, startup) under a versioned prefix.
   Then the standard submodule + `ci.yml` (matrix `el: ['9']`) works
   unchanged, and every push produces a downloadable build.
4. **Deploy story** (later): today's deploy is rdist-over-NFS from pisces.
   Either install the RPM on pisces and keep `setgmos` symlinks, or teach
   `deploy.sh` to take a CI artifact. Needs the `APPLIC_INSTALL` re-homing
   question answered (the IOC reads that path at boot).

The proprietary trees can't be rebuilt from public sources, so the dep RPMs
in rpm-repo (org-private GHCR) become their canonical home; the original
tarballs and the QEMU-Solaris image are the escrow copies.

## Files

- `build.sh` — build entry point (see Quick start)
- `Dockerfile.el9` / `Dockerfile` — build images (EL9 canonical, Debian alt)
- `gem-env.sh` — build environment variables (sourced in the container)
- `fetch-gnu-tools.sh` — downloads the ANL cross-toolchain (checksum-pinned)
- `patches/` — patches applied to the polaris tree copy
- `polaris/` (git-ignored) — the copied Solaris trees
- `polaris-v716/`, `work/` (git-ignored) — comparison reference / scratch

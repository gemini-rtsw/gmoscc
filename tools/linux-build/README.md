# Building gmoscc on Linux (REL-4693)

Builds gmoscc (EPICS 3.13.9 GEM8.6, VxWorks 5.5 target `ppc604_long`) on a
Linux host in Docker, replacing the Solaris SPARC build host `polaris`.

> **Porting another IOC?** Read
> [`docs/README-LINUX-REHOST.md`](../../docs/README-LINUX-REHOST.md) — the
> generalised procedure. Note that each IOC pins its own GEM tree version
> (GEM8.4, GEM8.6, ...), and each GEM version needs its **own** dependency
> RPMs built from scratch; only the method transfers, not these artifacts.

The short version: the Gemini UAE build system is almost entirely
host-portable. The cross-compiler (`ccppc`, gcc 2.96 for Tornado 2.2) exists
as Linux x86 binaries [rebuilt by ANL from Wind River's GPL'd
sources](https://epics.anl.gov/base/tornado-linux.php); the EPICS host tools
(`dbExpand`, `snc`, `macTest`, `e2db`, ...) compile fine on a modern Linux;
and EPICS 3.13.9 already ships `HOST_ARCH=Linux` support that Gemini started
wiring up years ago. The only tools with no Linux path are the commercial
Capfast `sch2edif` (schematic → EDIF) and `adl2dl` — both handled below.

## Quick start (dev container — recommended)

Everyone with GHCR access can build without any polaris trees:

```sh
./gemini-rtsw-ci/dev_environment.sh --el 9    # NB: --el 9, default is 8
# inside the container:
git clone git@github.com:gemini-rtsw/gmoscc.git && cd gmoscc
./tools/linux-build/setup.sh    # once per checkout (.applTop + applSetup)
make                            # and just `make` from then on
```

The dev image ships `gmoscc-devel`, which pulls the three build-environment
packages; `/etc/profile.d/gem86.sh` (from gem-epics3139gem86 ≥ 3.13.9-2)
puts the toolchain on PATH in every shell. `setup.sh` does the per-checkout
UAE bootstrap — the only part that can't be baked into the image, because
applSetup bakes the checkout's absolute path into the generated config.

## Quick start (mounted trees — build-env maintenance)

For working on the build environment itself (or without rpm-repo access),
with the polaris trees on disk (one-time setup below):

```sh
./tools/linux-build/build.sh              # applSetup (if needed) + gmake
./tools/linux-build/build.sh --clean      # wipe generated files, full rebuild
./tools/linux-build/build.sh --shell      # interactive shell in the build env
```

Run from the repo top (or anywhere — the script finds the top). First run
builds the Docker image automatically. A full build takes a few minutes.

### One-time setup of the build environment

The build needs the EPICS/Tornado/gem-libs trees copied from polaris
(proprietary — not in git). The tarballs live in `~/work/gmoscc-polaris/`;
unpack them **outside the repo** (default location
`~/work/gmoscc-buildenv/polaris`, override with `GMOSCC_BUILDENV`):

```sh
mkdir -p ~/work/gmoscc-buildenv/polaris && cd ~/work/gmoscc-buildenv/polaris
for f in epics3139gem86 wind-target gem-libs gem-config; do
    tar xzf ~/work/gmoscc-polaris/$f.tar.gz
done
cd <repo>/tools/linux-build && ./fetch-gnu-tools.sh
tar xzf gnu-tools.tor2_2-ppc-rhel5.tgz \
    -C ~/work/gmoscc-buildenv/polaris/usr/software/dev/packages/vxworks/tornado2.2/ppc
```

(Alternatively skip all of this and install the two rpm-repo packages —
see the CI section.)

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

### Two dead tools — Capfast and the display screens

Neither of these works **on Solaris either**. They are not Linux limitations,
and the rehost is not what broke them.

**Capfast is no longer supported.** The commercial Phase Three Logic tools
(`sch2edif`, `schplot`, `schps`) turned `capfast/*.sch` schematics into EDIF
for `e2db` to compile into `.db` databases. On polaris `sch2edif` now **fails
with a FlexLM licence error** (exit 255, prints the vendor's phone number);
`CAPFAST_LMHOST=@aguila.cl.gemini.edu` is a Cerro Pachón licence server and
Capfast is a dead product. So:

- **The 170 `.sch` schematics can no longer be compiled anywhere.** The 9
  generated `.db` files are committed in `capfast/db/` and are the databases
  the build actually uses; `setup.sh` seeds them into `capfast/O.Linux` and
  `touch`es them so make treats the `.sch → .edf → .db` chain as satisfied.
  (`.db` is the right artifact to commit — make deletes the `.edf`
  intermediates.)
- **Future database changes must edit the `.db` files directly.** The `.sch`
  schematics remain only as historical documentation of design intent; they
  are no longer the source of truth. Anyone maintaining these databases needs
  to know that.
- Only the licensed half of the chain is lost: `e2db` is an open EPICS
  extension and is rebuilt for Linux here.

**The MEDM/EDD display screens are no longer built.** `adl2dl` converted
`adl/*.adl` into the `.dl` screens. On polaris `~/.gem8.6` aliases
`adl2dl` → `adl2dl2.4` and `edd` → `eddadl2.4` — and **neither binary
exists**; the aliases have been dead for years. Therefore:

- The `adl` directory cannot build on Solaris or Linux. `setup.sh` (and the
  spec's `%build`) drop `DIRS += adl` from `Makefile.Dirs`.
- The `.dl` files in production `data/` are **stale artifacts** that rdist
  never deleted — not reproducible from source by any build.
- **A CI-built payload therefore contains no display screens.** If they
  matter, either commit the existing `.dl` files as versioned data, or try
  rebuilding `adl2dl`/`edd` from the EPICS extensions tree for Linux (the
  approach that worked for `e2db`; untried).

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

## Validation (V7-17, same source on both hosts) — COMPLETE

Objects from a polaris V7-17 build compared against the container build of
the same commit, after `objcopyppc --strip-debug -R .comment`:

- **40 objects: 16 byte-identical**, 24 differing.
- Per-function analysis of the 24 (functions matched **by name**, never by
  address — see the warning below): **384 functions containing comparisons →
  347 identical, 37 differing only in compare mnemonic, 0 operand
  mismatches.** No differing constants, branch conditions, call targets or
  operands anywhere in the build.

Exactly two systematic compiler differences, both benign:

1. **`-mlongcall` applied more consistently.** The ANL compiler routes all
   calls through a register (`lis/addi/mtlr/blrl`); polaris's emitted some
   direct `bl` despite `-mlongcall` in `ARCH_DEP_CFLAGS`. Direct `bl` reaches
   only ±32 MB, so the ANL behaviour is stricter/safer. This accounts for the
   code-size growth and all address shifts.
2. **`cmplwi` → `cmpwi`** (unsigned → signed immediate compare) in 37 places,
   same register/constant/branch. The operand is provably non-negative in
   every case — loaded with `lhz` (zero-extending) then masked by
   `clrlwi ...,16`, so 0–65535, where signed and unsigned compares are
   identical. A free optimizer tie-break, not a semantic change.

**Verdict:** the toolchains are not bit-identical (`ccppc -v`: ANL
`2.96+ MW/LM` from Wind River's 2012 cumulative-patch sources vs polaris
`2.96+` GA 2002), so byte-equality is not available as a safety net — but
there is no semantic divergence. Normal IOC release testing is the
appropriate remaining validation. If bit-exactness is ever required, Wind
River's original GCC sources (available to licensees) can be built for Linux
with ANL's scripts.

Non-compiled outputs match exactly, as expected: `gemini.Support` and
`mv167st` (copied from the EPICS tree) and all hand-written `start*` scripts
are byte-identical to both polaris and production. Generated `*Startup*`
scripts and `local` differ because they embed `APPLIC_INSTALL` — that is the
deploy re-homing question, not a build difference.

> **Method warning:** do not compare `.a` archives or whole-directory md5s
> (archives embed timestamps and uid/gid), and do not compare objects **by
> address** — a size change shifts every later address, so address-keyed
> comparison silently compares unrelated instructions. Two wrong conclusions
> in this project came from exactly that mistake. Compare per function, by
> name. Also note `objdumpppc` separates mnemonic and operands with a tab.

## CI integration (gemini-rtsw-ci)

gmoscc is wired into the standard pipeline: the `gemini-rtsw-ci` submodule +
`.github/workflows/ci.yml` (matrix `el: ['9']`) build `gmoscc.spec` inside
`rockylinux:9` on every push, publish the RPM to rpm-repo, and upload it as
an Actions artifact.

How the pieces fit:

- **Dependency RPMs** (in rpm-repo, built once by `rpm/build-dep-rpms.sh`
  from the polaris trees — they essentially never change, and any sibling
  GEM8.6 system can BuildRequire them):
  - `gem-tornado22-linux`: ANL cross-tools + `$WIND_BASE/target/{h,config}`
    → `/usr/software/dev/packages/vxworks/tornado2.2/ppc`
  - `gem-epics3139gem86`: the EPICS tree (config/rules, headers, dbd,
    templates, ppc604_long artifacts, Linux host tools; sources trimmed —
    they stay in the escrow tarballs)
    → `/usr/software/dev/packages/epics/epics3.13.9GEM8.6`
  - `gem86-deplibs`: astlib/slalib/timelib → `/gemini/GEM8.6`

  **Versioning convention**: Gemini runs several GEM software-tree
  generations in parallel. Packages tied to one carry it in their NAME
  (`gem86-*`, `...gem86`; a future GEM8.4 set would be `gem84-*`) and
  install under generation-specific paths (`/gemini/GEM8.6`,
  `epics3.13.9GEM8.6`), so different generations are distinct packages
  that co-install without conflicts. Tornado is generation-neutral and
  stays `gem-tornado22-linux`.
- **Committed in this repo**: the 9 capfast `.db` files (`capfast/db/`),
  so CI needs nothing from polaris.
- **`gmoscc.spec`** (repo root): `%build` = `.applTop` + `applSetup.pl`
  (without `adl`) + `gmake`; `%install` mirrors the classic rdist payload
  (`bin/ppc604_long` minus Distfile, `bin/Linux`, `include`, `dbd`, `data`,
  `RELEASE.NOTES`, `test`) under `/gemini/GEM8.6/gmos/gmos` — a fixed path, not
  a versioned one. rpm already provides what the `V7-xx` + `setgmos` symlink
  flip provided (`rpm -q` names what is installed, `dnf downgrade` rolls back),
  and a fixed path is what lets `APPLIC_INSTALL` and the crate's boot
  parameters name a location that does not change between releases.

Still open:

- **GHCR package access**: the gmoscc repo needs Write on the `rpm-repo`
  package (github.com/orgs/gemini-rtsw/packages/container/rpm-repo/settings
  → Manage Actions access) or the pipeline fails pulling/pushing.
- **Deploy story**: install the RPM on pisces and keep `setgmos`, or teach
  `deploy.sh` to take a CI artifact. Needs the `APPLIC_INSTALL` re-homing
  question answered (the IOC reads that path at boot; a CI build bakes the
  rpmbuild path, so a deployed CI RPM is not yet bootable — verification
  builds only for now).

The proprietary trees can't be rebuilt from public sources, so the dep RPMs
in rpm-repo (org-private GHCR) are their canonical home; the original
tarballs and the QEMU-Solaris image are the escrow copies.

## Files

- `build.sh` — build entry point (see Quick start)
- `Dockerfile.el9` / `Dockerfile` — build images (EL9 canonical, Debian alt)
- `gem-env.sh` — build environment variables (sourced in the container)
- `fetch-gnu-tools.sh` — downloads the ANL cross-toolchain (checksum-pinned)
- `rpm/` — specs + builder for the three dependency RPMs
- `patches/` — patches applied to the polaris tree copy
- `../../gmoscc.spec`, `../../.github/workflows/ci.yml` — pipeline wiring
- `~/work/gmoscc-buildenv/` (outside the repo) — the copied Solaris trees,
  V7-16 comparison objects, and build scratch

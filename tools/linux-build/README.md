# Linux cross-build environment (REL-4693 pilot)

Goal: build gmoscc (EPICS 3.13 / GEM8.6, target `ppc604_long` VxWorks) on a
Linux host instead of the Solaris SPARC machine (`polaris`).

## Approach

The Wind River Tornado cross-compilers (`ccppc` etc.) exist as Linux x86
binaries, rebuilt from Wind River's GPL'd GCC sources by ANL and published at
<https://epics.anl.gov/base/tornado-linux.php>. ANL reports the generated code
is **identical** to the Solaris-hosted compilers. Everything else in the build
(applSetup.pl, gmake, snc, dbExpand, shell glue) is portable, except Capfast
(see below).

Validation plan: build the same source tree on polaris and here, and compare
the produced objects/libraries byte-for-byte.

## Contents

- `fetch-gnu-tools.sh` — downloads the ANL cross-toolchain tarball
  (gcc 2.96 for Tornado 2.2 / vxWorks 5.5, 32-bit i386 Linux binaries)
  and verifies its checksum.
- `Dockerfile` — Debian-based build environment: i386 compat libs for the
  toolchain, `gmake`, `tcsh`, `perl`.
- `polaris/` — (not in git) files copied from the Solaris host: the GEM8.6
  tree pieces, VxWorks target headers, generated `config/`, etc.

## polaris inventory (July 2026)

- Tornado 2.2 at `WIND_BASE=/usr/software/dev/packages/vxworks/tornado2.2/ppc`,
  host arches `sun4-solaris2` and `x86-win32`.
- `ccppc -v`: `gcc version gcc-2.96 (2.96+) 19990621 AltiVec VxWorks 5.5`.
  Same toolchain generation as the ANL Linux rebuild, but the ANL build
  carries Wind River's cumulative patches (Jan 2012 sources, banner
  `2.96+ MW/LM` + patch list) while polaris appears to be unpatched GA.
  The byte-compare below decides whether that matters.
- EPICS base 3.13.9 GEM8.6 at
  `/usr/software/dev/packages/epics/epics3.13.9GEM8.6` (`HOST_ARCH=solaris`);
  `applSetup.pl`, `snc`, `dbExpand` live in `base/bin/solaris`.
- `e2db` is an EPICS **extension** (`extensions/bin/solaris/e2db`) —
  rebuildable from source. The only commercial Solaris-only tool is
  Capfast's `sch2edif` (`/home/p3/wcs/bin/sch2edif`).
- `adl2dl2.4` is not installed on polaris — already out of the build.
- `GEM8.6` is `alias GEM8.6 source ~/.gem8.6`.
- `/gemini/external/GEM8.6` (slalib/timelib/astlib) is only ~100 MB.

## Status / open items

- [x] ANL Linux `ccppc` toolchain obtained; `PPC604gnu/libgcc.a` present.
- [x] Toolchain smoke-tested in the container.
- [x] Confirm Tornado version on polaris — Tornado 2.2 / gcc 2.96 (see
  patch-level caveat above).
- [ ] Copy from polaris: `~/.gem8.6`, generated `config/` from a build dir,
  `$WIND_BASE/target/h` (+ `target/config`), the epics3.13.9GEM8.6 tree
  (base + extensions), `/gemini/external/GEM8.6`.
- [ ] Rebuild EPICS host tools (`snc`, `dbExpand`, `e2db`, ...) for a Linux
  HOST_ARCH — needs the EPICS base/extensions sources from polaris.
- [ ] Capfast `sch2edif` (commercial, Solaris-only): generate the `.edif`
  (or `.db`) from the 170 `capfast/*.sch` once on polaris, commit them, and
  make the Linux build treat them as sources.
- [ ] Byte-compare a polaris build vs a container build of the same tag.

## Usage (so far)

```sh
cd tools/linux-build
./fetch-gnu-tools.sh
docker build -t gmoscc-build .
docker run --rm -it -v "$(git rev-parse --show-toplevel):/build/gmoscc" gmoscc-build
```

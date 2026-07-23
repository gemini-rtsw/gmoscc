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

## Status / open items

- [x] ANL Linux `ccppc` toolchain obtained; `PPC604gnu/libgcc.a` present.
- [x] Toolchain smoke-tested in the container.
- [ ] **Confirm Tornado version on polaris.** The ANL binaries are
  Tornado 2.2 / gcc 2.96. If polaris runs an older Tornado (gcc 2.7.2),
  codegen will differ and we must rebuild that exact gcc from WRS sources
  instead.
- [ ] Copy VxWorks target headers (`$WIND_BASE/target/h`) from polaris —
  required for compiling anything that includes vxWorks headers.
- [ ] Copy / port the GEM8.6 tree (EPICS base + config templates +
  `applSetup.pl` + gemini dbd files + slalib/timelib/astlib).
- [ ] Rebuild EPICS host tools (`snc`, `dbExpand`, `dbst`, ...) for a Linux
  HOST_ARCH — needs EPICS base sources from the GEM8.6 tree.
- [ ] Capfast (`e2db`/`sch2edif`): commercial Solaris binary, compiles the
  170 `capfast/*.sch` schematics to `.db` at build time. Plan: generate the
  `.db` files once on polaris, commit them, and make the Linux build treat
  them as sources.
- [ ] `adl2dl2.4`: same idea (generate-and-commit) unless a Linux binary
  exists.
- [ ] Byte-compare a polaris build vs a container build of the same tag.

## Usage (so far)

```sh
cd tools/linux-build
./fetch-gnu-tools.sh
docker build -t gmoscc-build .
docker run --rm -it -v "$(git rev-parse --show-toplevel):/build/gmoscc" gmoscc-build
```

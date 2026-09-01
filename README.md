# GMOS Control Computer (GMOSCC)

EPICS-based control software for the Gemini Multi-Object Spectrograph (GMOS) control computer.

---

> **Building on Linux (REL-4693).** gmoscc now builds without Solaris, in a
> container or in CI on every push:
>
> ```sh
> ./gemini-rtsw-ci/dev_environment.sh --el 9   # NB: --el 9, the default is 8
> ./tools/linux-build/setup.sh                 # once per checkout
> make
> ```
>
> See [tools/linux-build/README.md](tools/linux-build/README.md) for details
> and the object-level validation against a polaris build, and
> [docs/README-LINUX-REHOST.md](docs/README-LINUX-REHOST.md) for the
> generalised procedure for other IOCs. The Solaris/polaris flow below still
> works and remains the deploy path until the `APPLIC_INSTALL` re-homing
> question is settled (CI output is verification-grade, not IOC-bootable).
>
> **Two build steps are permanently gone — on Solaris as well as Linux:**
> - **Capfast is unsupported.** `sch2edif` fails a FlexLM licence check on
>   polaris; the `capfast/*.sch` schematics can no longer be compiled
>   anywhere. The generated databases are committed in `capfast/db/` and
>   **future database changes must edit those `.db` files directly** — the
>   schematics are now only historical documentation.
> - **The MEDM/EDD display screens are no longer built.** `adl2dl`/`edd` do
>   not exist on polaris (the `~/.gem8.6` aliases have been dead for years),
>   so the `adl` directory is excluded from the build and the `.dl` files in
>   production `data/` are stale artifacts, not build products.

## Build & Deploy Instructions

### Prerequisites

- SSH access to `polaris` (build host, Solaris) and `mkotcsbootv2-lv1` (deploy host, Linux)
- Access to the Gemini software tree at `/gemini/external/GEM8.6/`

---

### 1. Connect to the build host

```csh
ssh gemvx@polaris
cd <your working dir>
```

### 2. Clone the repo

```csh
git clone git@github.com:gemini-rtsw/gmoscc.git
cd gmoscc
```

### 3. Set up the GEM8.6 environment

```csh
GEM8.6
```

This adds the required tools (including `applSetup.pl`) to your PATH and sets up the EPICS build environment.

### 4. Create `.applTop`

> **Note:** `.applTop` is not stored in the repo — it contains a local path and must be created manually each time you set up a new working directory.

`APPLIC_TOP` must point at **your build directory**, never at the production tree.

```csh
echo "APPLIC_TOP = `pwd`" > .applTop
```

> **Why this matters:** Every Makefile does `include $(APPLIC_TOP)/config/CONFIG`, which then pulls in `$(APPLIC_TOP)/config/CONFIG.Defs`. If `.applTop` points at `/gemini/GEM8.6/gmos/gmos`, the build reads **production's** `CONFIG.Defs`, and whatever stale `APPLIC_INSTALL` is there gets baked into the generated `bin/ppc604_long/local`, `startup`, etc. The deploy will then either ship the wrong paths or — worse — clobber prod if the stale `APPLIC_IOCPATH` points there.

### 5. Run the install script and build

```csh
./gmosInstall "" build.log
gmake
```

`gmosInstall` runs `applSetup.pl`, which writes `config/CONFIG.Defs` based on `pwd`. After it runs, your `config/CONFIG.Defs` should contain:

```
APPLIC_INSTALL = /home/gemvx/<you>/<your build dir>
APPLIC_IOCPATH =
```

### 6. Deploy

Builds ship as RPMs. `rdist`, `Distfile`, `deploy.sh` and `setgmos` are all
retired: the startup scripts name `/gemini/GEM8.6/gmos/gmos` literally, and the
RPM installs exactly there. `APPLIC_IOCPATH` is no longer part of deployment --
leave it as `applSetup.pl` wrote it.

Build the package (see `tools/linux-build/README.md`), publish it to rpm-repo
with `gemini-rtsw-repo/upload-rpm.sh`, then on the boot server:

```sh
sudo dnf install gmoscc
```

That pulls `gem86-epics-runtime`, `gem86-deplibs` and `gem-vxworks-tornado22`
as dependencies -- every `/gemini` tree the crate loads at boot.

### 7. Verify

```sh
rpm -q gmoscc
ls /gemini/GEM8.6/gmos/gmos/bin/ppc604_long/
```

### 8. Reboot the crate

There is one fixed directory, so there are no symlinks to flip. Rollback is
`sudo dnf downgrade gmoscc`.

After the boot, confirm the crate is running the build you think it is: the
package version is stamped into `gm:sad:name` and `gm:sad:cc:name`, and the
startup log should show `cd "/gemini/GEM8.6/gmos/gmos"` followed by loads from
that same path.

---

## Notes

- `config/`, `Distfile`, `bin/`, `lib/`, and `include/` are all generated — they are not stored in the repo.
- `.applTop` is excluded from the repo (see `.cvsignore`) because it contains a machine-local path.
- `APPLIC_IOCPATH` is vestigial: the startup scripts hardcode the deploy path, so nothing reads it any more.

---

## Common mistakes (read before building)

These have all caused real incidents. The build system is quiet about them — nothing fails loudly, you just end up with bad paths in `bin/ppc604_long/local` or, worse, prod overwritten.

1. **Pointing `.applTop` at the production tree.**
   `APPLIC_TOP` controls where the build reads `config/CONFIG` and `config/CONFIG.Defs` from. If it points at `/gemini/GEM8.6/gmos/gmos`, your build inherits whatever `APPLIC_INSTALL`/`APPLIC_IOCPATH` happen to be in prod's `CONFIG.Defs` (often left over from a previous person's test build). `.applTop` must always point at **your local build directory**.

2. **Editing `APPLIC_INSTALL` in `config/CONFIG.Defs`.**
   `APPLIC_INSTALL` is the path the IOC will `cd` into at boot — it gets stamped into the generated `bin/ppc604_long/local` and `startup` files. It must match your **build directory** (where the object files actually live on the NFS-mounted filesystem the IOC sees), not the deploy destination. `applSetup.pl` sets this correctly from `pwd`; leave it alone.

3. **Trusting prod's `CONFIG.Defs`.**
   Anyone can `rdist` into prod and replace its `config/CONFIG.Defs`. Never read that file expecting it to reflect what prod was *built* with. `rpm -q gmoscc` names what is installed.

### Quick sanity check before packaging

```sh
grep -n '^cd ' bin/ppc604_long/*Startup* bin/ppc604_long/startup
grep -c '@VERSION@' bin/ppc604_long/*Startup*
```

- Every `cd` should name `/gemini/GEM8.6/gmos/gmos`. The scripts hardcode it, so
  a different value means a stale generated file.
- `@VERSION@` should be gone from the RPM's copies -- `%build` substitutes it and
  fails the build if any remain.

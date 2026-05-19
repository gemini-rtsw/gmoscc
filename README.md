# GMOS Control Computer (GMOSCC)

EPICS-based control software for the Gemini Multi-Object Spectrograph (GMOS) control computer.

---

## Build & Deploy Instructions

### Prerequisites

- SSH access to `polaris` (build host, Solaris) and `pisces` (deploy host, Linux)
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

### 6. Set the deploy path

> **Important:** Only edit `APPLIC_IOCPATH`. **Do not** change `APPLIC_INSTALL` — it must stay pointed at your build directory, otherwise the generated `local`/`startup` files will reference a path that doesn't exist on the IOC at boot time.
>
> **Also:** `APPLIC_IOCPATH` must **not** point at the production directory. Use a versioned subdir like `V7-15-test1` so a misfired `rdist` cannot overwrite prod.

```csh
vi config/CONFIG.Defs
```

Leave `APPLIC_INSTALL` alone. Set only:

```
APPLIC_IOCPATH = pisces:/gemini/GEM8.6/gmos/<your-version-dir>
```

The version name you use here (e.g. `V7-15-test1`) is what `deploy.sh` will ask you to confirm before deploying.

### 7. Generate the Distfile

```csh
rm -f Distfile
gmake rdist
cat Distfile
```

Review the `cat` output and verify the source and destination paths look correct before proceeding.

### 8. Deploy from pisces

> **Important:** The deploy script must be run from `pisces`, not `polaris`. The two hosts have incompatible `rdist` protocol versions.

```csh
ssh gemvx@pisces
cd <your working dir>/gmoscc
./tools/deploy.sh
```

`deploy.sh` will show you the source and destination, then ask you to confirm by typing the version name. If the destination already exists it will warn you and require an exact match before proceeding.

### 9. Verify the deployment

```csh
cd /gemini/GEM8.6/gmos
```

Diff your test directory against the production directory and copy over any config files as needed.

### 10. Activate and reboot

```csh
setgmos
```

Set the symlinks to point to the new build, then reboot the GMOS CC.

---

## Notes

- `config/`, `Distfile`, `bin/`, `lib/`, and `include/` are all generated — they are not stored in the repo.
- `.applTop` is excluded from the repo (see `.cvsignore`) because it contains a machine-local path.
- Always double-check `APPLIC_IOCPATH` in `config/CONFIG.Defs` before deploying. A stale value pointing at the wrong directory can overwrite production binaries.

---

## Common mistakes (read before building)

These have all caused real incidents. The build system is quiet about them — nothing fails loudly, you just end up with bad paths in `bin/ppc604_long/local` or, worse, prod overwritten.

1. **Pointing `.applTop` at the production tree.**
   `APPLIC_TOP` controls where the build reads `config/CONFIG` and `config/CONFIG.Defs` from. If it points at `/gemini/GEM8.6/gmos/gmos`, your build inherits whatever `APPLIC_INSTALL`/`APPLIC_IOCPATH` happen to be in prod's `CONFIG.Defs` (often left over from a previous person's test build). `.applTop` must always point at **your local build directory**.

2. **Editing `APPLIC_INSTALL` in `config/CONFIG.Defs`.**
   `APPLIC_INSTALL` is the path the IOC will `cd` into at boot — it gets stamped into the generated `bin/ppc604_long/local` and `startup` files. It must match your **build directory** (where the object files actually live on the NFS-mounted filesystem the IOC sees), not the deploy destination. `applSetup.pl` sets this correctly from `pwd`; leave it alone.

3. **Setting `APPLIC_IOCPATH` to the production directory (e.g. `/gemini/GEM8.6/gmos/gmos`).**
   `APPLIC_IOCPATH` is the rdist destination. Setting it to the prod symlink target means `gmake rdist` produces a Distfile that points at prod, and the next deploy mirrors your tree over production — overwriting binaries and deleting any unversioned files (config edits, dated backups). Always use a versioned subdir like `V7-16` or `V7-15-test1`.

4. **Trusting prod's `CONFIG.Defs`.**
   Anyone can `rdist` into prod and replace its `config/CONFIG.Defs`. Never read that file expecting it to reflect what prod was *built* with. If you need to know the last good install path, check the `gmos` symlink target with `ls -la /gemini/GEM8.6/gmos/`.

5. **Running `deploy.sh` from polaris.**
   The Solaris and Linux `rdist` implementations have an incompatible protocol version. `deploy.sh` detects polaris and refuses; if you bypass it with raw `rdist`, the deploy will fail or hang. Always deploy from pisces.

### Quick sanity check before deploy

```csh
grep gemini bin/ppc604_long/local bin/ppc604_long/startup
cat Distfile
```

- `local` and `startup` should reference your **build directory** (where the IOC will NFS-mount and read object files from).
- `Distfile` `install -R` lines should reference your **versioned deploy directory**, never `/gemini/GEM8.6/gmos/gmos`.

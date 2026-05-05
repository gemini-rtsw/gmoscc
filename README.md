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

```csh
echo "APPLIC_TOP = `pwd`" > .applTop
```

### 5. Run the install script and build

```csh
./gmosInstall "" build.log
gmake
```

### 6. Set the deploy path

> **Important:** This must **not** point to the production directory.

```csh
vi config/CONFIG.Defs
```

Set the following line to your test directory:

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

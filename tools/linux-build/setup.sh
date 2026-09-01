#!/bin/bash
# Per-checkout bootstrap for building gmoscc on Linux (dev container or any
# host with the gem-* RPMs installed, or with the trees at the same paths).
# Run once after cloning; then plain `make`/`gmake` builds forever after.
#
#   git clone .../gmoscc.git && cd gmoscc
#   ./tools/linux-build/setup.sh
#   make
set -e

TOP="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$TOP"

# Environment: prefer the RPM-installed profile script, fall back to the
# repo copy (identical content). Interactive shells normally have this
# already via /etc/profile.d/gem86.sh.
if [ -z "${WIND_BASE:-}" ]; then
    if [ -f /etc/profile.d/gem86.sh ]; then . /etc/profile.d/gem86.sh
    else . "$TOP/tools/linux-build/gem-env.sh"; fi
fi

# Scrub generated state. applSetup PRESERVES APPLIC_INSTALL from an existing
# config/CONFIG.Defs, so a checkout previously bootstrapped in a different
# container (different mount path) would keep the stale path and fail —
# and already-generated startup files would keep it silently. A checkout
# being (re)bootstrapped gets a clean slate.
rm -rf config bin lib include dbd data Distfile .applTop
find . -type d -name 'O.*' -prune -exec rm -rf {} +

echo "APPLIC_TOP = $TOP" > .applTop

# The capfast .db files are generated on Solaris by the licensed Capfast
# sch2edif and committed in capfast/db; seed them so make treats the
# .sch -> .edf -> .db chain as up to date.
mkdir -p capfast/O.Linux
cp capfast/db/*.db capfast/O.Linux/
touch capfast/O.Linux/*.db

# Same arguments as gmosInstall, minus adl (adl2dl has no Linux port)
applSetup.pl -T ppc604_long -I alh -I capfast -I src -I startup -I db \
    -d /gemini/GEM8.6/gmos/support/astlib/V1-6 -d /gemini/GEM8.6/gmos/support/slalib/V1-9-4 \
    -d /gemini/GEM8.6/gmos/support/timelib/V1-8-6 -S MK

# The committed Makefile.Dirs still lists adl; drop it for Linux builds
sed -i '/^DIRS += adl$/d' Makefile.Dirs

echo
echo "Setup complete — run 'make' (or gmake) to build."

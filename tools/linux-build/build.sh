#!/bin/bash
# Build gmoscc on Linux inside the Docker build environment.
#
#   ./tools/linux-build/build.sh              # applSetup (if needed) + gmake
#   ./tools/linux-build/build.sh --clean      # wipe generated files first
#   ./tools/linux-build/build.sh --shell      # interactive shell in the env
#   ./tools/linux-build/build.sh --image debian   # use the Debian image
#
# Requires the polaris trees under tools/linux-build/polaris/ (see README.md).
set -e

HERE="$(cd "$(dirname "$0")" && pwd)"
TOP="$(cd "$HERE/../.." && pwd)"
POLARIS="${GMOSCC_BUILDENV:-$HERE/polaris}"

IMAGE_FLAVOR=el9
MODE=build
CLEAN=no
while [ "$#" -gt 0 ]; do
    case "$1" in
        --image) IMAGE_FLAVOR="$2"; shift 2 ;;
        --clean) CLEAN=yes; shift ;;
        --shell) MODE=shell; shift ;;
        *) echo "unknown arg: $1" >&2; exit 1 ;;
    esac
done

case "$IMAGE_FLAVOR" in
    el9)    IMAGE=gmoscc-build:el9;    DOCKERFILE=Dockerfile.el9 ;;
    debian) IMAGE=gmoscc-build:latest; DOCKERFILE=Dockerfile ;;
    *) echo "unknown --image '$IMAGE_FLAVOR' (el9 or debian)" >&2; exit 1 ;;
esac

for d in "$POLARIS/usr/software" "$POLARIS/gemini/GEM8.6/slalib"; do
    if [ ! -d "$d" ]; then
        echo "ERROR: build environment tree missing: $d" >&2
        echo "Unpack the polaris tarballs first — see tools/linux-build/README.md" >&2
        exit 1
    fi
done

if ! docker image inspect "$IMAGE" > /dev/null 2>&1; then
    echo "Building Docker image $IMAGE..."
    docker build -f "$HERE/$DOCKERFILE" -t "$IMAGE" "$HERE"
fi

if [ "$CLEAN" = yes ]; then
    echo "Cleaning generated files..."
    (cd "$TOP" && rm -rf config bin lib include dbd data Distfile .applTop \
        && find . -type d -name 'O.*' -prune -exec rm -rf {} +)
fi

# Until the capfast .db outputs are committed to the repo, seed them from the
# polaris reference build (sch2edif is Solaris-only; see README.md).
if [ ! -f "$TOP/capfast/O.Linux/gmosCcTop.db" ]; then
    SEED="$POLARIS/home/gemvx/hstecher/gmoscc-V7-16/capfast/O.solaris"
    if [ -d "$SEED" ]; then
        echo "Seeding capfast/O.Linux with polaris-built .db files..."
        mkdir -p "$TOP/capfast/O.Linux"
        cp "$SEED"/*.db "$TOP/capfast/O.Linux/" && touch "$TOP/capfast/O.Linux"/*.db
    else
        echo "WARNING: no capfast .db seed found; the capfast dir will fail" >&2
    fi
fi

TTY=()
[ -t 0 ] && TTY=(-it)
RUN=(docker run --rm "${TTY[@]}"
    -v "$POLARIS/usr/software:/usr/software"
    -v "$POLARIS/gemini:/gemini"
    -v "$TOP:/build/gmoscc"
    -v "$HERE/gem-env.sh:/etc/gem-env.sh:ro"
    "$IMAGE")

if [ "$MODE" = shell ]; then
    exec "${RUN[@]}" bash -c '. /etc/gem-env.sh && cd /build/gmoscc && exec bash'
fi

"${RUN[@]}" bash -c '
    . /etc/gem-env.sh
    cd /build/gmoscc
    echo "APPLIC_TOP = /build/gmoscc" > .applTop
    if [ ! -f config/CONFIG.Defs ]; then
        echo "Running applSetup.pl..."
        applSetup.pl -T ppc604_long -I adl -I alh -I capfast -I src -I startup -I db \
            -d /gemini/GEM8.6/astlib/astlib -d /gemini/GEM8.6/slalib/slalib \
            -d /gemini/GEM8.6/timelib/timelib -S MK > setup.log 2>&1
    fi
    gmake -k 2>&1 | tee build.log | grep -E "Entering directory .(/build/gmoscc/[a-z]+).$|Error" | grep -v ignored
    echo "--- done; full log in build.log"
'
echo
echo "Artifacts (in the checkout): bin/ppc604_long (IOC objects, startup"
echo "scripts, local), bin/Linux (host tools), dbd/gemini.dbd, data/ (luts,"
echo ".db files, alarm configs)."

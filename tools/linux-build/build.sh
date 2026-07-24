#!/bin/bash
# Build gmoscc against MOUNTED polaris trees (build-environment maintenance).
#
# For everything else use the flows that share the pipeline's inputs exactly:
#   pipeline-identical RPM build:  ./gemini-rtsw-ci/build_rpm.sh --el 9
#   interactive development:       ./gemini-rtsw-ci/dev_environment.sh --el 9
#                                  then: ./tools/linux-build/setup.sh && make
#
# This script differs from those ONLY in where the build environment comes
# from (tarball trees mounted at the same paths, instead of the rpm-repo
# packages) — the checkout is mounted at /repo and the build itself is the
# same setup.sh + gmake the spec runs.
#
#   ./tools/linux-build/build.sh              # setup.sh (if needed) + gmake
#   ./tools/linux-build/build.sh --clean      # force re-bootstrap first
#   ./tools/linux-build/build.sh --shell      # interactive shell in the env
#   ./tools/linux-build/build.sh --image debian   # use the Debian image
set -e

HERE="$(cd "$(dirname "$0")" && pwd)"
TOP="$(cd "$HERE/../.." && pwd)"
# The polaris trees live OUTSIDE the repo by default so local runs of the CI
# scripts (which tarball the whole checkout) stay small. GMOSCC_BUILDENV
# overrides; a repo-local polaris/ (gitignored) also works.
POLARIS="${GMOSCC_BUILDENV:-$HOME/work/gmoscc-buildenv/polaris}"
[ -d "$POLARIS" ] || POLARIS="$HERE/polaris"

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

if [ ! -d "$POLARIS/usr/software" ]; then
    echo "ERROR: build environment tree missing: $POLARIS/usr/software" >&2
    echo "Unpack the polaris tarballs first — see tools/linux-build/README.md" >&2
    exit 1
fi

if ! docker image inspect "$IMAGE" > /dev/null 2>&1; then
    echo "Building Docker image $IMAGE..."
    docker build -f "$HERE/$DOCKERFILE" -t "$IMAGE" "$HERE"
fi

TTY=()
[ -t 0 ] && TTY=(-it)
RUN=(docker run --rm "${TTY[@]}"
    -v "$POLARIS/usr/software:/usr/software"
    -v "$POLARIS/gemini:/gemini"
    -v "$TOP:/repo"
    "$IMAGE")

if [ "$MODE" = shell ]; then
    exec "${RUN[@]}" bash -c '. /repo/tools/linux-build/gem-env.sh && cd /repo && exec bash'
fi

"${RUN[@]}" bash -c "
    set -e
    cd /repo
    . tools/linux-build/gem-env.sh
    if [ '$CLEAN' = yes ] || [ ! -f config/CONFIG.Defs ]; then
        ./tools/linux-build/setup.sh
    fi
    gmake 2>&1 | tee build.log | grep -E 'Entering directory .(/repo/[a-z]+).\$|Error' | grep -v ignored || true
    echo '--- done; full log in build.log'
"
echo
echo "Artifacts (in the checkout): bin/ppc604_long (IOC objects, startup"
echo "scripts, local), bin/Linux (host tools), dbd/gemini.dbd, data/ (luts,"
echo ".db files, alarm configs)."

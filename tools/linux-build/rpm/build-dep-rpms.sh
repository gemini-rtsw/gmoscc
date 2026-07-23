#!/bin/bash
# Build the three gmoscc build-environment dependency RPMs from the polaris
# trees (one-time bootstrap; they change essentially never). Output: rpm/out/.
# Publish them to rpm-repo with gemini-rtsw-repo's upload-rpm.sh.
set -e

HERE="$(cd "$(dirname "$0")" && pwd)"
POLARIS="${GMOSCC_BUILDENV:-$HOME/work/gmoscc-buildenv/polaris}"
[ -d "$POLARIS" ] || POLARIS="$HERE/../polaris"
OUT="$HERE/out"
mkdir -p "$OUT"

[ -d "$POLARIS/usr/software" ] || { echo "ERROR: missing $POLARIS/usr/software — unpack the polaris tarballs first" >&2; exit 1; }

docker run --rm \
    -v "$POLARIS:/trees:ro" \
    -v "$HERE:/specs:ro" \
    -v "$OUT:/out" \
    rockylinux:9 bash -c '
        set -e
        dnf install -y -q rpm-build
        for s in gem-tornado22-linux gem-epics3139gem86 gem86-deplibs; do
            echo "==== building $s"
            rpmbuild -bb --define "trees /trees" --define "_rpmdir /out" /specs/$s.spec
        done
        ls -l /out/x86_64/
    '
echo "RPMs in $OUT/x86_64/"

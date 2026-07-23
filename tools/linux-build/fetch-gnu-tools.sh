#!/bin/sh
# Download the ANL Linux rebuild of the Wind River Tornado 2.2 PowerPC
# cross-toolchain (ccppc/gcc 2.96). See tools/linux-build/README.md.
set -e

cd "$(dirname "$0")"

TARBALL=gnu-tools.tor2_2-ppc-rhel5.tgz
URL=https://epics.anl.gov/base/$TARBALL
SHA256=b9881437f7f0c1cdd1acdda5a2e0b02515050b9805b5604b001fb21a4df30a2e

if [ ! -f "$TARBALL" ]; then
    echo "Downloading $URL"
    curl -fsSL -o "$TARBALL" "$URL"
fi

echo "$SHA256  $TARBALL" | shasum -a 256 -c - || {
    echo "Checksum mismatch — deleting $TARBALL" >&2
    rm -f "$TARBALL"
    exit 1
}
echo "OK: $TARBALL"

#!/bin/sh
#
# Safe wrapper around rdist for deploying the CC build to /gemini/GEM8.6/...
#
# Rationale: plain rdist will happily mirror a build tree into whatever
# destination the tracked Distfile points at. A stale Distfile once caused
# a V1-24 build to be deployed over V1-21 on pisces, overwriting binaries
# and deleting ~220 dated data backups before it was caught. This script
# forces a human to confirm the source/destination and to re-confirm when
# the destination already has contents.
#
# Usage: ./tools/deploy.sh   (run from the CC repo root)
#

set -e

DISTFILE=./Distfile

if [ ! -f "$DISTFILE" ]; then
    echo "ERROR: no Distfile in $(pwd). Run 'make Distfile' first." >&2
    exit 1
fi

# Pull the first source and destination out of the Distfile. rdist supports
# many src/dst pairs; we only report the first as a sanity check for the
# operator -- they're all under the same version dir in our builds.
SRC=`sed -n '/^FILES1/{s|.*= *( *||;s| *).*||;p;q;}' "$DISTFILE"`
DST=`sed -n '/install -R/{s|.*install -R *||;s|/bin/mv167;.*||;s|/bin/mv167.*||;p;q;}' "$DISTFILE"`

if [ -z "$SRC" ] || [ -z "$DST" ]; then
    echo "ERROR: could not parse Distfile (SRC='$SRC' DST='$DST')." >&2
    exit 1
fi

# On pisces, /gemini is a local path. On polaris it's NFS-mounted the same
# way. Either way we can stat the destination directly.
echo ""
echo "Deploy plan from $DISTFILE:"
echo "  src: $SRC"
echo "  dst: $DST"
echo ""

if [ -d "$DST" ]; then
    FILE_COUNT=`find "$DST" -type f 2>/dev/null | wc -l | tr -d ' '`
    EXPECTED=`basename "$DST"`
    echo "============================================================"
    echo "   WARNING: DESTINATION ALREADY EXISTS"
    echo "============================================================"
    echo ""
    echo "   $DST"
    echo "   contains $FILE_COUNT existing file(s)."
    echo ""
    echo "   rdist will OVERWRITE any files that differ and DELETE"
    echo "   any files present at the destination but not in the"
    echo "   source tree. Dated backups, hand-edits, and any other"
    echo "   unversioned content under this path will be lost."
    echo ""
    echo "   To proceed, type the version name '$EXPECTED' exactly."
    echo "   Anything else (including 'yes' or 'y') will abort."
    echo ""
    echo "============================================================"
    echo ""
    printf "Confirm by typing '%s': " "$EXPECTED"
    read CONFIRM
    if [ "$CONFIRM" != "$EXPECTED" ]; then
        echo "" >&2
        echo "Got '$CONFIRM', expected '$EXPECTED'. Aborting." >&2
        exit 1
    fi
else
    echo "Destination does not exist. It will be created by rdist."
    echo ""
    printf "Deploy now? [yes/no]: "
    read CONFIRM
    if [ "$CONFIRM" != "yes" ]; then
        echo "Aborted." >&2
        exit 1
    fi
fi

# rdist's protocol mismatches between Solaris (polaris) and Linux (pisces)
# break deploys when run from polaris. Detect that case up front and tell
# the operator to run from pisces instead.
HOST=`hostname 2>/dev/null | sed 's|\..*||'`
if [ "$HOST" = "polaris" ]; then
    echo ""
    echo "============================================================"
    echo "   ERROR: deploy.sh must be run from pisces, not polaris."
    echo ""
    echo "   rdist on polaris (Solaris) cannot talk to pisces (Linux)"
    echo "   due to a protocol version mismatch. Run this script on"
    echo "   pisces instead -- /home/gemvx is NFS-mounted there."
    echo "============================================================"
    exit 1
fi

echo ""
echo "Running rdist..."
echo ""
exec /usr/bin/rdist -f "$DISTFILE"

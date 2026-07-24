#!/bin/sh
# Runs inside the CI build container BEFORE `dnf builddep` (hook provided by
# gemini-rtsw-ci/build_rpm.sh).
#
# The 32-bit Tornado cross-tools need glibc.i686, which must exactly match
# the installed x86_64 glibc version. The rockylinux base image can lag the
# mirrors, making the builddep transaction unresolvable (cannot install both
# glibc-...-N.i686 and glibc-...-M.x86_64). Upgrading glibc first keeps the
# two arches in lockstep.
set -e
dnf -y upgrade glibc

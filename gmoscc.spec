# gmoscc — GMOS Control Computer IOC software.
# Cross-compiles for vxWorks 5.5 / ppc604_long using the GEM8.6 EPICS tree
# and Tornado 2.2 Linux toolchain from rpm-repo (see tools/linux-build/).
# The RPM payload installs to a FIXED path, /gemini/GEM8.6/gmos/gmos. The old
# versioned V7-xx directory plus a `setgmos` symlink flip existed to let two
# builds sit side by side and switch atomically; rpm provides that already --
# `rpm -q` names what is installed and `dnf downgrade` is the rollback -- so
# the extra indirection only adds a step that can be forgotten.

%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}
# Fixed deploy-directory name. Deliberately NOT the version: the version lives
# in the RPM, and a fixed path is what lets APPLIC_INSTALL and the crate's boot
# parameters name a location that does not change from release to release.
#
# It must stay "gmos". That was the old setgmos symlink name, and 144 places in
# this repo hardcode /gemini/GEM8.6/gmos/gmos/... rather than deriving it --
# TDIR and dirLUT settings in the pv files, the MKSS simulation startups, the
# legacy mv167 scripts. Those are not rewritten by the re-homing below, which
# only substitutes the build path, so renaming this directory silently breaks
# them. The crates' boot parameters name this path too.
%global gmosver gmos

# $GIT_HASH first: build_rpm.sh computes the hash on the HOST and passes it
# into the build container as an environment variable. Shelling out to git
# alone resolves to "nogit" whenever git is missing from the builder or trips
# dubious-ownership on the mounted checkout.
%define git_hash %(if [ -n "$GIT_HASH" ]; then echo "$GIT_HASH"; else git rev-parse --short HEAD 2>/dev/null || echo nogit; fi)

Name:           gmoscc
Version:        7.17
Release:        1.git%{git_hash}%{?dist}
Summary:        Gemini GMOS Control Computer IOC software (vxWorks ppc604)
License:        Gemini Observatory (org-internal)
Source0:        %{name}-%{version}.tar.gz
AutoReqProv:    no

BuildRequires:  gem-tornado22-linux
BuildRequires:  gem-epics3139gem86
BuildRequires:  gem86-deplibs
BuildRequires:  glibc(x86-32)
BuildRequires:  make, gcc, gcc-c++, perl, tcsh

%description
EPICS 3.13 (GEM8.6) control software for the GMOS control computer,
cross-compiled for the ppc604_long vxWorks target. The package installs the
deployable IOC tree under /gemini/GEM8.6/gmos/%{gmosver}: loadable objects,
generated startup scripts, databases, LUTs and alarm configs.

%package devel
Summary:        Build environment for gmoscc development images
Requires:       gem-tornado22-linux, gem-epics3139gem86, gem86-deplibs
Requires:       glibc(x86-32), make, gcc, gcc-c++, perl, tcsh

%description devel
Pulls the pinned gmoscc build dependencies into a dev container.

%prep
%setup -q

%build
# Environment from the gem-epics3139gem86 package (>= 3.13.9-2); repo copy
# as fallback so a -1 dependency still builds. Bootstrap (applSetup, capfast
# .db seed, adl removal) is shared with interactive use via setup.sh.
if [ -f /etc/profile.d/gem86.sh ]; then . /etc/profile.d/gem86.sh
else . tools/linux-build/gem-env.sh; fi
./tools/linux-build/setup.sh

# Build with APPLIC_INSTALL left exactly as applSetup.pl set it -- the build
# directory. It must stay that way for gmake: the UAE build system does not
# merely stamp APPLIC_INSTALL into generated files, it INSTALLS INTO IT
# ("Installing $APPLIC_INSTALL/./data/..."), so pointing it at the deploy path
# makes gmake try to write to /gemini on the build host and fail. That is what
# the top-level README's "do not change APPLIC_INSTALL" is protecting.
gmake

# Now re-home: rewrite the build path to the deploy path in the generated
# files, after everything has been built and installed into the build tree.
#
# APPLIC_INSTALL is the path the IOC cd's into at boot, and it is baked into
# generated files such as bin/ppc604_long/{startup,local}. Under rpmbuild that
# is the BUILD directory, which exists on no crate -- so without this the RPM
# installs cleanly and then boots the crate into nothing. Historically the
# build ran on a host that shared the /gemini namespace, so the build path was
# reachable and no re-homing was needed; building in a container removes that.
DEPLOY=/gemini/GEM8.6/gmos/%{gmosver}
BUILD_PATH=$PWD

# -I (text files only) is deliberate, not an oversight. The built archives and
# executables DO contain the build path -- ar and ld embed object-directory and
# debug paths, e.g. .../src/gmos/O.ppc604_long inside gmosLib.a. Those are
# inert: VxWorks `ld <` loads the file it is given and never consults them, and
# every compiled artifact on every platform carries the same thing. Running sed
# over a binary to "fix" them would corrupt it. Only the generated scripts
# matter, because those name paths the IOC actually reads at boot.
for f in $(grep -rlI "$BUILD_PATH" bin data dbd include 2>/dev/null); do
    echo "re-homing $f"
    sed -i "s|$BUILD_PATH|$DEPLOY|g" "$f"
done

# The failure this guards against is silent at runtime: the crate boots, cd's
# to a path that is not there, and stops. Cheaper to fail the build.
if grep -rlI "$BUILD_PATH" bin data dbd include 2>/dev/null | grep -q .; then
    echo "ERROR: build directory still present in files that will be shipped:" >&2
    grep -rlI "$BUILD_PATH" bin data dbd include >&2
    exit 1
fi

# And it must actually name the deploy path, or the rewrite silently did
# nothing and the previous check passed for the wrong reason.
grep -q "$DEPLOY" bin/ppc604_long/startup || {
    echo "ERROR: bin/ppc604_long/startup does not reference $DEPLOY" >&2
    exit 1
}

%install
# Mirror the classic rdist payload (UAE.dist): bin/<archs>, include, dbd,
# data, RELEASE.NOTES, test — rooted at the fixed deploy directory.
D=%{buildroot}/gemini/GEM8.6/gmos/%{gmosver}
mkdir -p $D/bin
cp -a bin/ppc604_long $D/bin/
rm -f $D/bin/ppc604_long/Distfile
cp -a bin/Linux $D/bin/
cp -a include dbd data test RELEASE.NOTES $D/

%files
/gemini/GEM8.6/gmos/%{gmosver}

%files devel

%changelog
* Thu Jul 23 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 7.17-1
- Initial RPM packaging via the Linux cross-build (REL-4693)

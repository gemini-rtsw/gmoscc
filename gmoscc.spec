# gmoscc — GMOS Control Computer IOC software.
# Cross-compiles for vxWorks 5.5 / ppc604_long using the GEM8.6 EPICS tree
# and Tornado 2.2 Linux toolchain from rpm-repo (see tools/linux-build/).
# The RPM payload mirrors the classic rdist deploy tree
# (/gemini/GEM8.6/gmos/<version>); activation stays a symlink flip (setgmos).

%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}
# Deploy-directory name, matching the historical V7-xx convention
%global gmosver V7-17

Name:           gmoscc
Version:        7.17
Release:        1%{?dist}
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
gmake

%install
# Mirror the classic rdist payload (UAE.dist): bin/<archs>, include, dbd,
# data, RELEASE.NOTES, test — rooted at the versioned deploy directory.
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

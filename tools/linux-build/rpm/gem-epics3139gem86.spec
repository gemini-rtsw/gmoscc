# EPICS 3.13.9 GEM8.6 (Gemini flavor) build tree for Linux hosts.
# Binary package: config tree, installed headers/dbd/templates, prebuilt
# ppc604_long target artifacts (from the Solaris build — host-independent),
# and host tools rebuilt for HOST_ARCH=Linux. Sources are NOT included; they
# live in the escrow tarballs (see gmoscc/tools/linux-build/README.md).
# Org-internal only. Built by build-dep-rpms.sh.

%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}
%global epicsdir /usr/software/dev/packages/epics/epics3.13.9GEM8.6

Name:           gem-epics3139gem86
Version:        3.13.9
Release:        1%{?dist}
Summary:        EPICS 3.13.9 GEM8.6 build tree (Linux host tools + ppc604_long target)
License:        EPICS Open License / Proprietary Gemini additions (org-internal)
AutoReqProv:    no
Requires:       gem-tornado22-linux, perl, make, gcc, gcc-c++

%description
The GEM8.6 EPICS base+extensions tree as used on polaris, rehosted for
Linux: UAE config/rules, applSetup.pl, host tools (dbExpand, snc, macTest,
antelope, e_flex, e2db) built for HOST_ARCH=Linux, and the cross-built
ppc604_long artifacts. Installs at the polaris path so generated
configuration stays compatible.

%install
mkdir -p %{buildroot}%{epicsdir}/base %{buildroot}%{epicsdir}/extensions
cd %{trees}%{epicsdir}
for d in config include dbd templates tools startup bin/Linux bin/ppc604_long lib/Linux; do
    [ -e "base/$d" ] || continue
    mkdir -p "%{buildroot}%{epicsdir}/base/$(dirname $d)"
    cp -a "base/$d" "%{buildroot}%{epicsdir}/base/$d"
done
for d in include config bin/Linux bin/ppc604_long lib/Linux; do
    [ -e "extensions/$d" ] || continue
    mkdir -p "%{buildroot}%{epicsdir}/extensions/$(dirname $d)"
    cp -a "extensions/$d" "%{buildroot}%{epicsdir}/extensions/$d"
done
# strip Solaris-era build litter
find %{buildroot}%{epicsdir} -type d -name 'O.*' -prune -exec rm -rf {} +
# applSetup.pl's shebang expects perl here
mkdir -p %{buildroot}/usr/software/dev/solaris/bin
ln -sf /usr/bin/perl %{buildroot}/usr/software/dev/solaris/bin/perl

%files
%{epicsdir}
/usr/software/dev/solaris

%changelog
* Thu Jul 23 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 3.13.9-1
- Initial packaging (REL-4693 Linux rehost)

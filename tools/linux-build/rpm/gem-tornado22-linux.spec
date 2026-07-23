# Wind River Tornado 2.2 PowerPC cross-toolchain for Linux x86 hosts.
# Host tools are the ANL rebuild of WRS's GPL'd GCC (gcc 2.96, VxWorks 5.5,
# https://epics.anl.gov/base/tornado-linux.php); target headers/config are
# copied from the Gemini Tornado 2.2 installation (org-internal only).
# Built from the trees in gmoscc/tools/linux-build/ — see build-dep-rpms.sh.

%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}

Name:           gem-tornado22-linux
Version:        2.2
Release:        1%{?dist}
Summary:        Tornado 2.2 ppc cross-toolchain (x86-linux host) + vxWorks target headers
License:        GPL (host tools) / Proprietary (target headers, org-internal)
AutoReqProv:    no
# 32-bit host binaries (ccppc and friends)
Requires:       glibc(x86-32)

%description
ccppc/ldppc/etc. (gcc 2.96 for PowerPC vxWorks 5.5) as 32-bit Linux binaries,
plus $WIND_BASE/target/h and target/config from the Gemini Tornado 2.2
install. Installs under /usr/software/dev/packages/vxworks/tornado2.2/ppc,
the WIND_BASE path the GEM8.6 build system expects.

%install
mkdir -p %{buildroot}/usr/software/dev/packages/vxworks/tornado2.2/ppc
cp -a %{trees}/usr/software/dev/packages/vxworks/tornado2.2/ppc/host \
      %{trees}/usr/software/dev/packages/vxworks/tornado2.2/ppc/target \
      %{buildroot}/usr/software/dev/packages/vxworks/tornado2.2/ppc/
# only the x86-linux host tools belong in this package
rm -rf %{buildroot}/usr/software/dev/packages/vxworks/tornado2.2/ppc/host/sun4-solaris2 \
       %{buildroot}/usr/software/dev/packages/vxworks/tornado2.2/ppc/host/x86-win32

%files
/usr/software/dev/packages/vxworks/tornado2.2

%changelog
* Thu Jul 23 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 2.2-1
- Initial packaging (REL-4693 Linux rehost)

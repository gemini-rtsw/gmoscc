# EPICS 3.13.9 GEM8.6 *target* binaries at the path the crates load them from.
#
# Not to be confused with gem-epics3139gem86, which installs the BUILD tree
# under /usr/software/dev/packages/epics/... — host tools, UAE config, rules.
# That one is what gmake needs. This one is what the IOC needs: at boot the
# crate does
#
#   ld < /gemini/external/GEM8.6/base/bin/ppc604_long/iocCore
#   ld < /gemini/external/GEM8.6/base/bin/ppc604_long/seq
#   ld < /gemini/external/GEM8.6/extensions/bin/ppc604_long/pvload
#
# and nothing packaged those. They existed on the file server only because it
# was a copy of the old pisces export, so a clean install of gmoscc + the other
# deps produced a crate that mounted /gemini, cd'd correctly, and then died on
# the first ld. This package closes that gap and makes the boot's dependency
# set expressible as RPM requires.
#
# VERSIONING CONVENTION: as with gem86-deplibs -- packages tied to a GEM
# software-tree version carry the gemNN prefix and install under
# version-specific paths, so GEM8.4 and GEM8.6 co-install without conflicts.

# rpmbuild refuses "arch dependent binaries in a noarch package". That check is
# right for ordinary software; here the payload is ppc604_long objects the host
# never runs, only serves. Same waiver as gem-vxworks-tornado22.
%global _binaries_in_noarch_packages_terminate_build 0
%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}
# ppc604_long only. The host-side bin/Linux and lib/Linux belong to the build
# tree (gem-epics3139gem86); shipping them here would put two owners on the
# same content and give the file server a second, diverging copy.
%global tarch ppc604_long

Name:           gem86-epics-runtime
Version:        3.13.9
Release:        1%{?dist}
Summary:        GEM8.6 EPICS target binaries served to vxWorks crates
License:        EPICS Open License / Proprietary Gemini additions (org-internal)
AutoReqProv:    no
# noarch, as in gem-vxworks-tornado22: the payload is ppc604_long objects that
# the HOST never executes -- it only serves them over NFS to the crate. Without
# this, rpmbuild stamps the builder's arch (aarch64 on an Apple Silicon laptop,
# x86_64 on CI) and the package refuses to install on the boot server.
BuildArch:      noarch

%description
The EPICS 3.13.9 GEM8.6 base and extensions ppc604_long binaries (iocCore,
seq, pvload and companions) at /gemini/external/GEM8.6, the path vxWorks IOCs
load them from over NFS. Host-side build tooling is in gem-epics3139gem86.

%install
mkdir -p %{buildroot}/gemini/external/GEM8.6/base/bin \
         %{buildroot}/gemini/external/GEM8.6/extensions/bin
cp -a %{trees}/gemini/external/GEM8.6/base/bin/%{tarch} \
      %{buildroot}/gemini/external/GEM8.6/base/bin/
cp -a %{trees}/gemini/external/GEM8.6/extensions/bin/%{tarch} \
      %{buildroot}/gemini/external/GEM8.6/extensions/bin/

# The staging tree is usually an NFS copy read through root_squash, so the
# files arrive owned by nobody. Package them as root-owned; they are served
# read-only to crates and nothing needs write access. (Same as
# gem-vxworks-tornado22.)
chown -R root:root %{buildroot}/gemini/external/GEM8.6

# Fail the build rather than ship a package that boots a crate into nothing:
# these three are named literally by the startup scripts.
for f in base/bin/%{tarch}/iocCore base/bin/%{tarch}/seq \
         extensions/bin/%{tarch}/pvload; do
    [ -f "%{buildroot}/gemini/external/GEM8.6/$f" ] || {
        echo "ERROR: missing $f -- crates load this at boot" >&2; exit 1; }
done

%files
%defattr(-,root,root,-)
/gemini/external/GEM8.6

%changelog
* Mon Aug 31 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 3.13.9-1
- Initial packaging: EPICS target binaries were never packaged, only present
  as a copy of the pisces export (REL-4693)

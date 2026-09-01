# Gemini GEM8.6 dependency libraries (astlib, slalib, timelib) as deployed
# on /gemini/GEM8.6 — headers plus prebuilt ppc604_long/mv167 target
# artifacts. Org-internal only. Built by build-dep-rpms.sh.
#
# VERSIONING CONVENTION: Gemini runs several GEM software-tree versions in
# parallel (GEM8.4, GEM8.6, ...). Packages tied to a GEM version carry the
# gemNN prefix in their NAME (gem86-*) and install under version-specific
# paths (/gemini/GEM8.6, epics3.13.9GEM8.6), so different GEM generations
# are distinct packages that co-install without conflicts. Version: tracks
# the content release within that family.

# rpmbuild refuses "arch dependent binaries in a noarch package". That check is
# right for ordinary software; here the payload is ppc604_long/mv167 target
# objects the host never runs, only serves over NFS to the crates.
%global _binaries_in_noarch_packages_terminate_build 0
%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}

Name:           gem86-deplibs
Version:        8.6
Release:        1%{?dist}
Summary:        GEM8.6 astlib/slalib/timelib dependency trees
License:        Proprietary / GPL (slalib) — org-internal
AutoReqProv:    no
# noarch for the same reason as gem-vxworks-tornado22 and gem86-epics-runtime:
# nothing here executes on the host. Without it rpmbuild stamps the BUILDER's
# arch (aarch64 on an Apple Silicon laptop) and the package will not install on
# the x86_64 boot server.
BuildArch:      noarch

%description
The /gemini/GEM8.6 astlib, slalib and timelib application trees that gmoscc
(and sibling GEM8.6 systems) build against, at their production paths.

%install
mkdir -p %{buildroot}/gemini/GEM8.6
cp -a %{trees}/gemini/GEM8.6/astlib %{trees}/gemini/GEM8.6/slalib \
      %{trees}/gemini/GEM8.6/timelib %{buildroot}/gemini/GEM8.6/

# The crate loads these through the unversioned symlinks (astlib/astlib ->
# V1-6 and friends), so a broken or missing link is a failed boot, not a
# missing file. cp -a preserves them; check that it did.
for l in astlib/astlib slalib/slalib timelib/timelib; do
    lib="${l##*/}"
    [ -e "%{buildroot}/gemini/GEM8.6/$l/bin/ppc604_long/$lib" ] || {
        echo "ERROR: /gemini/GEM8.6/$l/bin/ppc604_long/$lib does not resolve -- crates ld this at boot" >&2
        exit 1; }
done
[ -e "%{buildroot}/gemini/GEM8.6/timelib/timelib/bin/ppc604_long/timeSeq" ] || {
    echo "ERROR: timeSeq does not resolve" >&2; exit 1; }

%files
/gemini/GEM8.6

%changelog
* Thu Jul 23 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 8.6-1
- Initial packaging (REL-4693 Linux rehost)

# astlib/slalib/timelib for GMOS, installed under the GMOS tree rather than
# shared at /gemini/GEM8.6/<lib>/.
#
# WHY THIS IS GMOS-SPECIFIC. VxWorks has no shared libraries: gmStartupMKRR
# does `ld < .../slalib` at boot, so the support libraries are running code
# selected at boot time, not build inputs baked into a binary. The RTEMS IOCs
# pin their support versions in BuildRequires and link them in, so they have
# no runtime exposure at all; GMOS does, and that is the difference this
# package exists to address.
#
# Previously the version was chosen by an unversioned symlink on the file
# server -- /gemini/GEM8.6/slalib/slalib -> V1-9-4, last moved in December
# 2016 -- which nothing in gmoscc recorded or could query. Build time resolved
# it for headers and boot time resolved it for code, separately, on different
# machines. Under the RPM the version becomes packaged content that travels
# with the release: `rpm -q` names it, `dnf downgrade` rolls it back, and
# gmoscc pins it with an exact Requires.
#
# The versions below are the ones the symlinks pointed at, i.e. what GMOS has
# actually been running. The install step fails if the staged tree disagrees,
# so this list cannot silently drift from the payload.
%global astlibver  V1-6
%global slalibver  V1-9-4
%global timelibver V1-8-6

%global gmosdir /gemini/GEM8.6/gmos
%global supdir  %{gmosdir}/support

# rpmbuild refuses "arch dependent binaries in a noarch package". The payload
# is ppc604_long objects the host serves over NFS and never executes. Same
# waiver as gem-vxworks-tornado22 and gem86-epics-runtime.
%global _binaries_in_noarch_packages_terminate_build 0
%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}

Name:           gmos-deplibs
Version:        8.6
Release:        1%{?dist}
Summary:        astlib/slalib/timelib for the GMOS CC, at GMOS-private paths
License:        Proprietary / GPL (slalib) -- org-internal
AutoReqProv:    no
BuildArch:      noarch

# So gmoscc (and any other consumer) can pin a library version rather than a
# bundle: Requires: gmos-slalib = 1.9.4
Provides:       gmos-astlib  = 1.6
Provides:       gmos-slalib  = 1.9.4
Provides:       gmos-timelib = 1.8.6

%description
The astlib, slalib and timelib PowerPC trees the GMOS control computer loads
at boot, installed under %{supdir} rather than the shared GEM8.6 location.
One version of each, named explicitly, so the version GMOS runs is recorded
in the package instead of in a symlink on the file server.

Does not conflict with gem86-deplibs: different paths, different owners.

%install
mkdir -p %{buildroot}%{supdir}

for spec in astlib:%{astlibver} slalib:%{slalibver} timelib:%{timelibver}; do
    lib="${spec%%:*}"; ver="${spec##*:}"
    src="%{trees}/gemini/GEM8.6/$lib"

    # The staged tree must actually be running the version named above. The
    # symlink is the historical selector; if it points somewhere else, the
    # staging is from a different era than this spec and the pin would be a
    # lie.
    actual="$(readlink "$src/$lib" || true)"
    if [ "$actual" != "$ver" ]; then
        echo "ERROR: $lib/$lib -> '$actual', but this spec pins '$ver'." >&2
        echo "       Restage, or update the %%global in this spec." >&2
        exit 1
    fi

    mkdir -p "%{buildroot}%{supdir}/$lib"
    cp -a "$src/$ver" "%{buildroot}%{supdir}/$lib/$ver"
done

# Each library ships its own standalone-IOC startup script carrying an
# absolute `cd "/gemini/GEM8.6/<lib>/<ver>"`. gmoscc never sources them -- it
# ld's the objects directly -- but they would be wrong at the new location, so
# ship nothing rather than something misleading.
find %{buildroot}%{supdir} -type f -name startup -path '*/bin/*' -delete

# astlib carries `epics -> ../../../external/GEM8.6`, a relative link tuned to
# the old depth. At the new depth it would dangle silently. Absolute, so this
# tree can be installed at any depth from here on.
find %{buildroot}%{supdir} -type l -name epics -exec \
    ln -sfn /gemini/external/GEM8.6 {} \;

# The four objects gmStartupMKRR loads at boot. A missing one is a crate that
# stops mid-startup, so fail the build instead.
for f in astlib/%{astlibver}/bin/ppc604_long/astlib \
         slalib/%{slalibver}/bin/ppc604_long/slalib \
         timelib/%{timelibver}/bin/ppc604_long/timelib \
         timelib/%{timelibver}/bin/ppc604_long/timeSeq; do
    [ -f "%{buildroot}%{supdir}/$f" ] || {
        echo "ERROR: missing $f -- the crate ld's this at boot" >&2; exit 1; }
done

# Headers, for the -d flags in tools/linux-build/setup.sh.
for f in astlib/%{astlibver}/include slalib/%{slalibver}/include \
         timelib/%{timelibver}/include; do
    [ -d "%{buildroot}%{supdir}/$f" ] || {
        echo "ERROR: missing $f -- the build compiles against these" >&2; exit 1; }
done

%files
%defattr(-,root,root,-)
%{supdir}

%changelog
* Tue Sep 01 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 8.6-1
- Initial packaging. Moves the GMOS support libraries off the shared GEM8.6
  symlinks onto GMOS-private versioned paths, so the version GMOS boots is
  recorded in the RPM rather than selected by a symlink (REL-4693)

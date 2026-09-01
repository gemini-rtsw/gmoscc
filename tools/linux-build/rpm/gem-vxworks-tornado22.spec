# Prebuilt VxWorks 5.5 kernel images for Gemini VME crates, at their runtime
# path under /gemini/external. Org-internal only. Built by build-dep-rpms.sh.
#
# NOT a build dependency, unlike its siblings here. gem-tornado22-linux ships
# the cross-toolchain to $WIND_BASE so gmoscc can be compiled; this ships the
# kernel the crate loads over the wire at boot, which is a different file at a
# different path for a different purpose. It belongs on the NFS/rsh boot
# server, not on a build host.
#
# The images are the Gemini build from Jan 2010 (provenance: mkMv2700.tar in
# the /gemini tree), carried forward verbatim. They are binaries with no
# internal version marker, which is the reason for packaging them rather than
# copying: after this, `rpm -q` identifies which kernel a crate loaded.

# The payload is a PowerPC ELF, which rpm's brp check flags as an arch
# dependent binary in a noarch package. That check is right for ordinary
# packages and wrong here: the binary is never executed on the host, it is
# served over the network to a VME crate. noarch is what makes one build
# installable on any boot server regardless of what it was built on.
%global _binaries_in_noarch_packages_terminate_build 0

%global _build_id_links none
%global __os_install_post %{nil}
%global debug_package %{nil}

Name:           gem-vxworks-tornado22
Version:        2.2
Release:        2%{?dist}
Summary:        Prebuilt VxWorks 5.5 kernel images for Tornado 2.2 VME targets
License:        Proprietary (Wind River / Gemini) — org-internal
AutoReqProv:    no
# noarch, unlike its siblings here: this package contains one PowerPC VxWorks
# image and no host binaries at all, so tagging it to the build host's arch is
# wrong and actively harmful -- a build on an arm64 laptop produces an
# aarch64 RPM that will not install on the x86_64 boot server.
BuildArch:      noarch

%description
VxWorks 5.5 boot images built under Tornado 2.2, installed at
/gemini/external/vxWorks/tornado2.2 — the path Gemini VME boot parameters
name directly, e.g.

    file name (f): /gemini/external/vxWorks/tornado2.2/mv2700/vxWorks

Currently carries the mv2700 (MVME2700) BSP, plus tornado2.0/vxUsers -- the
shell login definitions the GMOS startup script reads. That file lives under a
DIFFERENT Tornado version directory than the kernel, which is why it is called
out here: packaging only tornado2.2 leaves a boot that loads the kernel fine
and then fails on

    can't open input '/gemini/external/vxWorks/tornado2.0/vxUsers'

Additional BSPs and versions are added as new releases of this package rather
than as separate packages, so one package answers "what does this boot server
offer the crates".

Install on the boot server that exports /gemini. The crate fetches the image
over rsh or TFTP depending on its boot flags; neither needs this package on
the crate itself.

%install
mkdir -p %{buildroot}/gemini/external/vxWorks
cp -a %{trees}/gemini/external/vxWorks/tornado2.2 \
      %{trees}/gemini/external/vxWorks/tornado2.0 \
      %{buildroot}/gemini/external/vxWorks/
# The staging tree is usually an NFS copy read through root_squash, so the
# files arrive owned by nobody. Package them as root-owned; they are served
# read-only to crates and nothing needs write access.
chown -R root:root %{buildroot}/gemini/external/vxWorks

%files
%defattr(-,root,root,-)
/gemini/external/vxWorks/tornado2.2
/gemini/external/vxWorks/tornado2.0

%changelog
* Wed Aug 26 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 2.2-2
- Add tornado2.0/vxUsers. Found during the REL-4693 TR: the crate loaded its
  kernel and ran the startup script, which then failed on
  "can't open input '/gemini/external/vxWorks/tornado2.0/vxUsers'". It is a
  different Tornado version directory from the kernel, so packaging
  tornado2.2 alone missed it. Without it no shell login users are defined and
  telnet/rlogin to the crate is refused; the IOC itself is unaffected.
- Note: vxUsers contains a VxWorks-hashed shell password. It has been
  world-readable on the NFS export since 2006, so packaging does not change
  its exposure much, but the hash is a weak additive checksum and should not
  be treated as protecting anything.
  vxUsers 85 bytes, dated 2006-08-26,
  sha256 dab0eeca7dbb7a6323b4ce5c83e1620383413049b57310f96f4427d9072b7041

* Mon Aug 24 2026 Hawi Stecher <hawi.stecher@noirlab.edu> - 2.2-1
- Initial packaging of the mv2700 VxWorks 5.5 image, dated 2010-01-14, taken
  from mkotcsbootv2-lv1:/export/gemini/external/vxWorks/tornado2.2/mv2700. Packaged so
  the boot kernel a crate loads is identifiable by rpm -q rather than being an
  undated binary copied between hosts.
- Both files are ELF 32-bit MSB PowerPC, statically linked, not stripped;
  the kernel reports WIND version 2.6 (VxWorks 5.5) and supports
  MVME2300/2600/2700/3600. Recorded here because the binaries carry no
  internal version marker, so these hashes are the only way to tell one build
  from another:
    vxWorks      1732058  sha256 5b075f18b42025adfb6f03be1df2870ebc6064b0bf493819353720f49879af91
    vxWorks.sym   253878  sha256 649f0c9eb33e485dbb646a9eaa4ce7616a20c0c8c8dc9c5dfcd959e73f8bb248
- The bootloader loads only vxWorks; vxWorks.sym is the symbol table the
  Tornado target server reads. Shipped together so a crate and a debugging
  session can never disagree about which kernel they are looking at.

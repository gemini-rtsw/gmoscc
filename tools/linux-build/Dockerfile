FROM debian:bookworm-slim

# The Tornado cross-compilers are 32-bit i386 Linux binaries.
RUN dpkg --add-architecture i386 && apt-get update && apt-get install -y --no-install-recommends \
    libc6:i386 \
    make \
    perl \
    tcsh \
    csh \
    file \
    rsync \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/* \
    && ln -s /usr/bin/make /usr/local/bin/gmake

# Tornado layout: the ANL tarball extracts to host/x86-linux under WIND_BASE.
# target/ (vxWorks headers/libs) must be copied in from the real Tornado
# install on polaris — see README.md.
ENV WIND_BASE=/opt/tornado \
    WIND_HOST_TYPE=x86-linux

COPY gnu-tools.tor2_2-ppc-rhel5.tgz /tmp/gnu-tools.tgz
RUN mkdir -p "$WIND_BASE" \
    && tar xzf /tmp/gnu-tools.tgz -C "$WIND_BASE" \
    && rm /tmp/gnu-tools.tgz

ENV PATH=$WIND_BASE/host/x86-linux/bin:$PATH

WORKDIR /build
CMD ["/bin/bash"]

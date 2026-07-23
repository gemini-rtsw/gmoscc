# Linux equivalent of polaris ~/.gem8.6 + base/config/epics.csh
# (sourced inside the build container; bash/sh syntax)

export EPICS=/usr/software/dev/packages/epics/epics3.13.9GEM8.6
export HOST_ARCH=Linux
export WIND_BASE=/usr/software/dev/packages/vxworks/tornado2.2/ppc
export WIND_HOST_TYPE=x86-linux
export EPICS_BASE=$EPICS/base

export PATH=$EPICS/base/bin/$HOST_ARCH:$EPICS/base/tools:$EPICS/extensions/bin/$HOST_ARCH:$WIND_BASE/host/$WIND_HOST_TYPE/bin:$PATH
export LD_LIBRARY_PATH=$EPICS/base/lib/$HOST_ARCH:$EPICS/extensions/lib/$HOST_ARCH${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}

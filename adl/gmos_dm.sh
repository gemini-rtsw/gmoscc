#!/bin/csh -f
setenv PATH ${PATH}:/gemini/GEM8.6/gmos/gmos/bin/solaris
setenv EPICS_DISPLAY_PATH .:/gemini/epics3.13.4/gmosdc/gmosdc/data
dm2-4 gmosTop.dl "T=gm:, S=gm:, C1=cc:, C2=cc:, CC=cc:, DC=dc:" &

#!/bin/csh -f
setenv EPICS_DISPLAY_PATH .:/gemini/gmosdc/gmosdc/data
setenv PATH ${PATH}:/home/ajf/gemini/gmos/bin/solaris
dm2-4 gmosTop.dl "T=gm:, S=gm:, C1=cc:, C2=cc:, CC=cc:, DC=dc:" &

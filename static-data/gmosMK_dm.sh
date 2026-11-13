#!/bin/csh -f

export EPICS_CA_AUTO_ADDR_LIST=NO
export EPICS_CA_ADDR_LIST=10.2.2.255

#dm2-4  gmosTop.dl cmapfile=colorsMK.dl &
dm2-4 gmosTop.dl "T=gm:, S=gm:, C1=cc:, C2=cc:, CC=cc:, DC=dc:" &

#!/bin/bash

export EPICS_CA_AUTO_ADDR_LIST=NO
export EPICS_CA_ADDR_LIST=10.2.2.255

dm2-4 template.dl cmapfile=colorsMK.dl &

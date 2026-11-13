#!/bin/bash

export EPICS_CA_AUTO_ADDR_LIST=NO
export EPICS_CA_ADDR_LIST=172.17.2.255

dm2-4 template.dl cmapfile=colorsCP.dl &

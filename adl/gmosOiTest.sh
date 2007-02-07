#!/bin/csh -f
#
# gmOiTest
#
# This script is used to move the OIWFS probe arm in a square trajectory
# at a constant distance from the X & Y limits. The distance from the limits
# can be entered by the user. The program prompts for all the input needed
# as well as confirmation.
#
# This is a script intended for testing the OIWFS probe arm to check for
# any type of errors that produce a lost of index. It should not be used
# while observing or using the instrument for other activities.
# 
# 07/Feb/2007	Created (P Gigoux)
#

unalias *

# OIWFS limits
#set xmin = -7.1
#set xmax = 125
#set ymin = -21.7
#set ymax = 133

# Channel acces enable flag (1=enable, 0=disable)
set ca = 1

# Channels used to send the X,Y demands
set xdem_chan = "gm:wfs:probeAssembly.A"
set ydem_chan = "gm:wfs:probeAssembly.B"

# Channels used to read the X,Y positions
set xpos_chan = "gm:wfs:probeCalcPosition.VALA"
set ypos_chan = "gm:wfs:probeCalcPosition.VALB"

# Channels used to get the X,Y limits
set xmin_chan = "gm:wfs:probeAssembly.AALL"
set xmax_chan = "gm:wfs:probeAssembly.AAHL"
set ymin_chan = "gm:wfs:probeAssembly.ABLL"
set ymax_chan = "gm:wfs:probeAssembly.ABHL"

# Mode channel and value to MOVE
set mode = "gm:wfs:probeAssembly.MODE"
set move = 1

# Assembly record DIR channel and values
set dir   = "gm:wfs:probeAssembly.DIR"
set mark  = 0
set start = 3

# Status channel
set valc  = "gm:wfs:probeC.VAL"

# Delay to wait after sending a demand
set d_delay = 2

# Delay to wait between status checks
set w_delay = 2

# Set default distance from the limits (tolerance)
set tol  = 10

# Get OIWFS X,Y limits from the EPICS database
set xmin = `caget  $xmin_chan | awk '{print $2}'`
set xmax = `caget  $xmax_chan | awk '{print $2}'`
set ymin = `caget  $ymin_chan | awk '{print $2}'`
set ymax = `caget  $ymax_chan | awk '{print $2}'`
if ("$xmin" == "" || "$xmax" == "" || "$ymin" == "" || "$ymax" == "") then
    echo "Cannot get X,Y limits from the OIWFS"
    exit
endif

# List of demands
set xlist = (0 $xmin $xmin $xmax $xmax $xmin 0)
set ylist = (0 $ymin $ymax $ymax $ymin $ymin 0)
set nmax  = $#xlist

# Prompt user for confirmation
echo ""
echo "*** WARNING *** WARNING *** WARNING ***"
echo "This program will move the OIWFS probe"
echo ""
echo -n "Are you sure you want to proceed (yes/no): "
set ans = $<
if ("$ans" != "yes") then
    echo "Aborted"
    exit
endif
echo ""

# Get distance from limits (tolerance) from user
if ($#argv == 0) then
    echo ""
    echo -n "Distance from the limits ($tol): "
    set ans = $<
    if ($ans > 0) set tol = $ans
else
    set tol = $argv[1]
    if ($tol < 0) set tol = 10
endif

echo ""
echo "Software limits:"
echo "X min = $xmin"
echo "X max = $xmax"
echo "Y min = $ymin"
echo "Y max = $ymax"
echo ""

# Calculate min and max values based on the tolerance
set xmin = `echo $xmin $tol | awk -e '{print $1 + $2}'`
set xmax = `echo $xmax $tol | awk -e '{print $1 - $2}'`
set ymin = `echo $ymin $tol | awk -e '{print $1 + $2}'`
set ymax = `echo $ymax $tol | awk -e '{print $1 - $2}'`

echo "Motion limits for this test:"
echo "X min = $xmin"
echo "X max = $xmax"
echo "Y min = $ymin"
echo "Y max = $ymax"
echo ""

# Loop over all demands
set n = 1
while ($n <= $nmax)

    set x = $xlist[$n]
    set y = $ylist[$n]
    #echo $n, $x, $y
    @ n = $n + 1

    echo ""
    echo "----- Moving to $x, $y ----"

    if ($ca) then
	caput $xdem_chan $x
	caput $ydem_chan $y
	caput $mode $move
	caput $dir $mark
	caput $dir $start
    endif
    sleep $d_delay

    if ($ca) then
	while (1)
	    set stat = `caget $valc | awk -e '{print $2}'`
	    if ($stat == "IDLE") break
	    if ($stat == "ERROR") goto abort
	    set x = `caget $xpos_chan | awk -e '{print $2}'`
	    set y = `caget $ypos_chan | awk -e '{print $2}'`
	    echo -n "$x $y" | \
		awk -e '{printf ("Moving... %g %g\r", $1, $2)}'
	end
	sleep 2
    endif
    echo ""
end

echo ""
echo "Done"
echo ""
exit

abort:
echo ""
echo "Error detected"
echo ""

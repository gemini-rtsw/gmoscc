[schematic2]
uniq 9
[tools]
[detail]
w 2262 1627 100 0 c#7 ecalcouts.MonChange.FLNK 2064 1616 2496 1616 2496 1408 2688 1408 outhier.FLNK.p
w 2346 1563 100 0 RDOUT ecalcouts.MonChange.VAL 2064 1552 2688 1552 2688 1696 outhier.NEWVAL.p
s 3184 2528 150 0 gmSeqWaitChange.sch
s 1744 2128 150 0 Schematic to monitor changes in
s 1744 2080 150 0 a records value
[cell use]
use ecalcouts 1744 1431 100 0 MonChange
xform 0 1904 1552
p 1816 1464 100 0 -1 CALC:A
p 2576 1982 100 0 0 DOPT:Use CALC
p 2576 2014 100 0 0 OOPT:On Change
p 1832 1664 100 0 1 SCAN:Passive
p 2160 1808 60 0 0 def(INPA): $(monitor)
p 1808 1432 100 0 -1 name:$(prefix)$(I)
p 1696 1624 75 0 -1 pproc(INPA):CP
use outhier 2720 1408 130 0 FLNK
xform 0 2672 1408
use outhier 2720 1680 130 0 NEWVAL
xform 0 2672 1696
use oslBorderC 320 87 100 0 oslBorderC#1
xform 0 2000 1392
p 3260 336 120 256 -1 Title:GMOS IS - monitor subsystem channel
[comments]

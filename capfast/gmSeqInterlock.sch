[schematic2]
uniq 17
[tools]
[detail]
w 1652 1563 100 2 c#7 lboat.c#7.p 1648 1568 1648 1568 ecalcouts.CcInterlockMonChange.INPA
w 2112 1579 100 0 n#12 ecalcouts.CcInterlockMonChange.FLNK 1968 1568 2304 1568 2304 1520 2480 1520 ebos.interlockVal.SLNK
w 2200 1515 100 0 n#11 ecalcouts.CcInterlockMonChange.VAL 1968 1504 2480 1504 2480 1552 ebos.interlockVal.DOL
w 2846 1643 100 0 c#8 ebos.interlockVal.FLNK 2736 1552 2736 1632 2992 1632 outhier.FLNK.p
w 2846 1523 100 0 c#4 ebos.interlockVal.VAL 2736 1520 2992 1520 outhier.ILOCK.p
s 3392 2512 140 0 gmSeqInterlock.sch
[cell use]
use ecalcouts 1648 1383 100 0 CcInterlockMonChange
xform 0 1808 1504
p 1720 1416 100 0 -1 CALC:A
p 2480 1934 100 0 0 DOPT:Use CALC
p 2480 1966 100 0 0 OOPT:On Change
p 1736 1616 100 0 1 SCAN:I/O Intr
p 2064 1760 60 0 0 def(INPA):$(cc)ilock.VAL
p 1600 1576 75 0 -1 pproc(INPA):NPP
use ebos 2480 1431 100 0 interlockVal
xform 0 2608 1520
p 2544 1392 100 0 1 OMSL:closed_loop
p 2512 1664 100 0 1 ONAM:INTERLOCK
p 2512 1616 100 0 1 ZNAM:OK
use outhier 2960 1591 100 0 FLNK
xform 0 2976 1632
use outhier 2960 1479 100 0 ILOCK
xform 0 2976 1520
use lboat 1488 1527 100 0 c#7
xform 0 1568 1568
use oslBorderC 560 71 100 0 oslBorderC#1
xform 0 2240 1376
p 3500 320 120 256 -1 Title:GMOS IS - monitor for subsystem interlock
[comments]

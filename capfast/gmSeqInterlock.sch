[schematic2]
uniq 13
[tools]
[detail]
w 2176 1275 100 0 n#12 ewait.CcInterlockMonChange.FLNK 2096 1264 2304 1264 2304 1520 2480 1520 ebos.interlockVal.SLNK
w 2264 1563 100 0 n#11 ewait.CcInterlockMonChange.VAL 2096 1552 2480 1552 ebos.interlockVal.DOL
w 2846 1643 100 0 c#8 ebos.interlockVal.FLNK 2736 1552 2736 1632 2992 1632 outhier.FLNK.p
w 1396 1643 100 2 c#7 lboat.c#7.p 1392 1648 1392 1648 ewait.CcInterlockMonChange.INAN
w 2846 1523 100 0 c#4 ebos.interlockVal.VAL 2736 1520 2992 1520 outhier.ILOCK.p
s 3392 2512 140 0 gmSeqInterlock.sch
[cell use]
use ebos 2480 1431 100 0 interlockVal
xform 0 2608 1520
p 2544 1392 100 0 1 OMSL:closed_loop
p 2512 1664 100 0 1 ONAM:INTERLOCK
p 2512 1616 100 0 1 ZNAM:OK
use outhier 2960 1591 100 0 FLNK
xform 0 2976 1632
use outhier 2960 1479 100 0 ILOCK
xform 0 2976 1520
use lboat 1232 1607 100 0 c#7
xform 0 1312 1648
use ewait 1392 1175 100 0 CcInterlockMonChange
xform 0 1744 1504
p 1715 1752 100 0 1 CALC:A
p 1776 1552 100 0 1 INAP:Yes
p 1520 1328 100 0 1 OOPT:On Change
p 1520 1646 100 0 1 SCAN:I/O Intr
p 1120 1664 100 0 -1 def(INAN): $(cc)ilock.VAL
use oslBorderC 560 71 100 0 oslBorderC#1
xform 0 2240 1376
p 3500 320 120 256 -1 Title:GMOS IS - monitor for subsystem interlock
[comments]

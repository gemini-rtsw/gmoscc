[schematic2]
uniq 8
[tools]
[detail]
w 2510 1419 100 0 c#7 ewait.MonChange.FLNK 2368 1408 2688 1408 outhier.FLNK.p
w 2510 1707 100 0 RDOUT ewait.MonChange.VAL 2368 1696 2688 1696 outhier.NEWVAL.p
s 1744 2080 150 0 a records value
s 1744 2128 150 0 Schematic to monitor changes in
s 3184 2528 150 0 gmSeqWaitChange.sch
[cell use]
use outhier 2720 1680 130 0 NEWVAL
xform 0 2672 1696
use outhier 2720 1408 130 0 FLNK
xform 0 2672 1408
use ewait 1664 1319 100 0 MonChange
xform 0 2016 1648
p 1987 1896 100 0 1 CALC:A
p 2048 1696 100 0 1 INAP:Yes
p 2048 1664 100 0 0 INBP:No
p 2048 1632 100 0 0 INCP:No
p 1792 1472 100 0 1 OOPT:On Change
p 1792 1790 100 0 1 SCAN:I/O Intr
p 1504 1776 100 0 -1 def(INAN): $(monitor)
p 2144 1312 100 1024 1 name:$(prefix)$(I)
use oslBorderC 320 87 100 0 oslBorderC#1
xform 0 2000 1392
p 3260 336 120 256 -1 Title:GMOS IS - monitor subsystem channel
[comments]

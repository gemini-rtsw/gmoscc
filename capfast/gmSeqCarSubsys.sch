[schematic2]
uniq 33
[tools]
[detail]
w 1056 1675 100 0 n#15 gmSeqWaitChange.gmSeqWaitChange#31.FLNK 992 1664 1168 1664 1168 1136 junction
w 1464 1003 100 0 n#15 estringouts.Name.FLNK 1104 1136 1280 1136 1280 992 1696 992 egenSub.combine.SLNK
w 1264 1707 100 0 n#32 gmSeqWaitChange.gmSeqWaitChange#31.NEWVAL 992 1696 1584 1696 1584 1664 1696 1664 egenSub.combine.INPA
w 928 1355 100 0 n#30 gmSeqCarMonitor.gmSeqCarMonitor#1.SUBAPPLYC 640 1344 1264 1344 1264 1472 1696 1472 egenSub.combine.INPD
w 2608 1611 100 0 n#27 ecars.applyC.VAL 2512 1600 2752 1600 outhier.CAR.p
w 2608 1547 100 0 n#26 ecars.applyC.OMSS 2512 1536 2752 1536 outhier.MESS.p
w 2608 1387 100 0 n#25 ecars.applyC.FLNK 2512 1376 2752 1376 outhier.FLNK.p
w 2016 1675 100 0 n#24 egenSub.combine.OUTA 1984 1664 2096 1664 2096 1536 2192 1536 ecars.applyC.IMSS
w 2064 1611 100 0 n#23 egenSub.combine.OUTB 1984 1600 2192 1600 ecars.applyC.IVAL
w 928 875 100 0 c#29 lboat.c#29.p 560 864 1344 864 1344 1344 1696 1344 egenSub.combine.INPF
w 1144 1419 100 0 n#20 gmSeqCarMonitor.gmSeqCarMonitor#1.CSOMSS 640 1408 1696 1408 egenSub.combine.INPE
w 904 1483 100 0 n#18 gmSeqCarMonitor.gmSeqCarMonitor#1.CSVAL 640 1472 1216 1472 1216 1536 1696 1536 egenSub.combine.INPC
w 1480 1643 100 0 n#16 estringouts.Name.OUT 1104 1104 1312 1104 1312 1632 1696 1632 egenSub.combine.B
w 720 1131 100 0 n#13 gmSeqCarMonitor.gmSeqCarMonitor#1.FLNK 640 1120 848 1120 estringouts.Name.SLNK
s 448 816 100 0 Subsystem Apply error message
s 1152 2080 100 0 fields are blamk.
s 1152 2112 100 0 message if the overall CAR state is ERR but the input message
s 1152 2144 100 0 for a particular subsystem. The gensub will provide a default error
s 1152 2176 100 0 This schematic combines the applyC and commSentC CAR records
s 2736 2416 150 0 gmSeqCarSubsys.sch
s 688 1488 100 0 CommSentC value
s 688 1424 100 0 CommSentC message
s 656 1360 100 0 Subsystem applyC value
s 464 1904 160 0 Check for subsystem disconnection
[cell use]
use gmSeqWaitChange 688 1591 100 0 gmSeqWaitChange#31
xform 0 840 1704
p 528 1856 100 0 1 seta:monitor $(gm)$(subsys)Disabled.VAL
p 528 1824 100 0 1 setb:prefix $(gm)$(subsys)CheckDiscon
use lboat 400 823 100 0 c#29
xform 0 480 864
use oslBorderC -128 -25 100 0 oslBorderC#28
xform 0 1552 1280
p 2812 224 120 256 -1 Title:GMOS IS - generate subsystem applyC status
use outhier 2720 1335 100 0 FLNK
xform 0 2736 1376
use outhier 2720 1495 100 0 MESS
xform 0 2736 1536
use outhier 2720 1559 100 0 CAR
xform 0 2736 1600
use estringouts 848 1047 100 0 Name
xform 0 976 1120
p 864 1216 100 0 -1 DESC:Name of subsystem
p 912 992 100 0 1 OMSL:supervisory
p 960 1040 100 0 1 VAL:$(instru)
use ecars 2192 1319 100 0 applyC
xform 0 2352 1488
p 2192 1248 100 0 -1 DESC:Combined CAR status record for a subsystem
use egenSub 1696 903 100 0 combine
xform 0 1840 1328
p 1680 1776 100 0 -1 DESC:Combine GMOS subsystem CAR values
p 1473 677 100 0 0 FTA:LONG
p 1473 677 100 0 0 FTB:STRING
p 1473 645 100 0 0 FTC:LONG
p 1473 613 100 0 0 FTD:LONG
p 1473 581 100 0 0 FTE:STRING
p 1473 517 100 0 0 FTF:STRING
p 1473 677 100 0 0 FTVA:STRING
p 1473 677 100 0 0 FTVB:LONG
p 1760 832 100 0 1 SNAM:gmSeqSubsysCar
p 768 880 100 0 -1 def(INPF):$(gm)$(subsys):applyC.OMSS
p 1984 1610 75 0 -1 pproc(OUTB):PP
use gmSeqCarMonitor 320 1063 100 0 gmSeqCarMonitor#1
xform 0 480 1312
[comments]

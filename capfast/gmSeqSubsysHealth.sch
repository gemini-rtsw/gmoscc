[schematic2]
uniq 78
[tools]
[detail]
w 1828 1659 100 2 c#77 lboat.c#77.p 1824 1664 1824 1664 estringouts.health.DOL
w 1828 1947 100 2 c#76 lboat.c#76.p 1824 1952 1824 1952 estringouts.healthMess.DOL
w 2224 1003 100 0 FLNK efanouts.fan.FLNK 1584 1440 1728 1440 1728 992 2768 992 outhier.FLNK.p
w 624 1179 100 0 c#72 inhier.SLNK.P 496 1168 800 1168 ecalcs.inc.SLNK
w 2296 1083 100 0 n#68 estringouts.badHealth.OUT 2080 1072 2560 1072 2560 1520 junction
w 2296 1627 100 0 n#68 estringouts.health.OUT 2080 1616 2560 1616 2560 1520 2816 1520 outhier.HEALTH.p
w 1976 1211 100 0 n#70 estringouts.badMess.FLNK 2080 1328 2240 1328 2240 1200 1760 1200 1760 1088 1824 1088 estringouts.badHealth.SLNK
w 2264 1307 100 0 n#67 estringouts.badMess.OUT 2080 1296 2496 1296 2496 1472 junction
w 2264 1915 100 0 n#67 estringouts.healthMess.OUT 2080 1904 2496 1904 2496 1472 2784 1472 outhier.MESS.p
w 1736 1323 100 0 n#69 efanouts.fan.LNK2 1584 1376 1696 1376 1696 1312 1824 1312 estringouts.badMess.SLNK
w 1992 1755 100 0 n#66 estringouts.healthMess.FLNK 2080 1936 2272 1936 2272 1744 1760 1744 1760 1632 1824 1632 estringouts.health.SLNK
w 1736 1931 100 0 n#65 efanouts.fan.LNK1 1584 1408 1696 1408 1696 1920 1824 1920 estringouts.healthMess.SLNK
w 1208 1339 100 0 n#63 ecalcs.inc.FLNK 1088 1392 1120 1392 1120 1328 1344 1328 efanouts.fan.SLNK
w 1264 1410 100 0 n#22 ecalcs.inc.VAL 1088 1360 1184 1360 1184 1408 1392 1408 efanouts.fan.SELL
w 630 1554 100 0 n#6 inhier.DISV.P 496 1552 800 1552 ecalcs.inc.INPA
s 2752 1584 100 0 and then the health value
s 2752 1616 100 0 N.B. The message is written first
s 1216 736 100 0 health value from the subsystem is used.
s 1216 768 100 0 then the health value generated is BAD, otherwise the
s 1216 800 100 0 If a subsystem is not present or failed
s 1216 704 100 0 If a subsystem has a health message then this
s 1216 672 100 0 is copied through.
s 1392 2192 150 0 This schematic fetches the health value and 
s 1392 2144 150 0 message from a GMOS subsystem.
s 400 1472 100 0 DISV = 1 if absent/failed
s 1232 1472 100 0 OK = 1
s 1232 1440 100 0 Bad = 2
s 2864 2560 150 0 gmSeqSubsysHealth
[cell use]
use lboat 1664 1623 100 0 c#77
xform 0 1744 1664
use lboat 1664 1911 100 0 c#76
xform 0 1744 1952
use outhier 2736 951 100 0 FLNK
xform 0 2752 992
use outhier 2808 1480 100 0 HEALTH
xform 0 2800 1520
use outhier 2776 1432 100 0 MESS
xform 0 2768 1472
use inhier 480 1127 100 0 SLNK
xform 0 496 1168
use inhier 504 1512 100 0 DISV
xform 0 496 1552
use oslBorderC 0 119 100 0 oslBorderC#71
xform 0 1680 1424
use estringouts 1848 1240 100 0 badMess
xform 0 1952 1312
p 1872 1390 100 0 -1 DESC:Empty message
p 1760 1118 100 0 0 OMSL:closed_loop
p 1760 1278 100 0 0 VAL:
p 2080 1296 75 768 -1 pproc(OUT):NPP
use estringouts 1848 1848 100 0 healthMess
xform 0 1952 1920
p 1856 1998 100 0 -1 DESC:Subsys health message
p 1856 1806 100 0 1 IVOA:Set output to IVOV
p 1856 1774 100 0 1 IVOV:Invalid health from $(instru)
p 1760 1726 100 0 0 OMSL:closed_loop
p 1760 1886 100 0 0 VAL:
p 1488 1982 100 0 -1 def(DOL):$(gm)sad:$(subsys):health.OMSS
p 2080 1904 75 768 -1 pproc(OUT):NPP
use estringouts 1848 1016 100 0 badHealth
xform 0 1952 1088
p 1872 1166 100 0 -1 DESC:Bad health
p 1760 894 100 0 0 OMSL:closed_loop
p 1920 1056 100 0 -1 VAL:BAD
p 2080 1072 75 768 -1 pproc(OUT):NPP
use estringouts 1848 1560 100 0 health
xform 0 1952 1632
p 1856 1710 100 0 -1 DESC:Current subsys health
p 1856 1518 100 0 1 IVOA:Set output to IVOV
p 1856 1486 100 0 1 IVOV:BAD
p 1760 1438 100 0 0 OMSL:closed_loop
p 1760 1598 100 0 0 VAL:
p 1376 1680 100 0 -1 def(DOL):$(gm)sad:$(subsys):health.VAL
p 2080 1616 75 768 -1 pproc(OUT):NPP
use efanouts 1368 1192 100 0 fan
xform 0 1464 1344
p 1200 1310 100 0 0 SELM:Specified
use ecalcs 824 1080 100 0 inc
xform 0 944 1344
p 896 1294 100 0 1 CALC:A+1
p 768 1614 100 0 -1 DESC:Increment absent/failed flag
p 512 1230 100 0 0 EGU:none
p 880 1440 100 0 0 SCAN:Passive
[comments]

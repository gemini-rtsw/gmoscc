[schematic2]
uniq 121
[tools]
[detail]
w 1898 1227 100 0 n#120 gmSeqSubsysHealth.gmSeqSubsysHealth#68.MESS 1288 1472 1680 1472 1680 1216 2176 1216 egenSub.combineHealth.D
w 1922 1291 100 0 n#119 gmSeqSubsysHealth.gmSeqSubsysHealth#68.HEALTH 1288 1536 1728 1536 1728 1280 2176 1280 egenSub.combineHealth.C
w 1502 1867 100 0 n#118 gmSeqSubsysHealth.gmSeqSubsysHealth#62.MESS 1288 1856 1776 1856 1776 1344 2176 1344 egenSub.combineHealth.B
w 1526 1931 100 0 n#116 gmSeqSubsysHealth.gmSeqSubsysHealth#62.HEALTH 1288 1920 1824 1920 1824 1408 2176 1408 egenSub.combineHealth.A
w 398 1547 100 0 DC inhier.DC.P 64 1536 792 1536 gmSeqSubsysHealth.gmSeqSubsysHealth#68.DISV
w 398 1931 100 0 CC inhier.CC.P 64 1920 792 1920 gmSeqSubsysHealth.gmSeqSubsysHealth#62.DISV
w 1746 715 100 0 n#110 gmSeqSubsysHealth.gmSeqSubsysHealth#68.FLNK 1288 1392 1376 1392 1376 704 2176 704 egenSub.combineHealth.SLNK
w 1000 1643 100 0 n#72 gmSeqSubsysHealth.gmSeqSubsysHealth#62.FLNK 1288 1776 1408 1776 1408 1632 640 1632 640 1408 800 1408 gmSeqSubsysHealth.gmSeqSubsysHealth#68.SLNK
w 536 2235 100 0 n#71 efanouts.healthScan.FLNK 480 2224 640 2224 640 1792 800 1792 gmSeqSubsysHealth.gmSeqSubsysHealth#62.SLNK
w 2632 1346 100 0 c#114 egenSub.combineHealth.VALB 2464 1344 2848 1344 rboat.c#114.p
w 2632 1410 100 0 c#113 egenSub.combineHealth.VALA 2464 1408 2848 1408 rboat.c#113.p
s 2688 2448 150 0 gmSeqHealth.sch
s 1152 2272 100 0 This schematic combines the health values (and messages)
s 1152 2240 100 0 of the Components and Detector Controllers to form an
s 1152 2208 100 0 overall health value for GMOS.
[cell use]
use rboat 2848 1303 100 0 c#114
xform 0 2928 1344
use rboat 2848 1367 100 0 c#113
xform 0 2928 1408
use oslBorderC -176 7 100 0 oslBorderC#107
xform 0 1504 1312
use inhier 72 1496 100 0 DC
xform 0 64 1536
use inhier 72 1880 100 0 CC
xform 0 64 1920
use gmSeqSubsysHealth 800 1735 100 0 gmSeqSubsysHealth#62
xform 0 1040 1856
p 916 1708 100 0 1 seta:subsys cc
p 912 1680 100 0 1 setb:top $(top)$(subsys)
p 912 1648 100 0 1 setc:instru CC
use gmSeqSubsysHealth 800 1351 100 0 gmSeqSubsysHealth#68
xform 0 1040 1472
p 896 1312 100 0 1 seta:subsys dc
p 896 1280 100 0 1 setb:top $(top)$(subsys)
p 896 1248 100 0 1 setc:instru DC
use efanouts 240 1975 100 0 healthScan
xform 0 360 2128
p 272 2288 100 0 -1 DESC:Trigger health calculation
p 160 2048 100 0 1 SCAN:1 second
use egenSub 2200 616 100 0 combineHealth
xform 0 2320 1040
p 2224 1470 100 0 -1 DESC:Generate GMOS health
p 1953 389 100 0 0 FTA:STRING
p 1953 389 100 0 0 FTB:STRING
p 1953 357 100 0 0 FTC:STRING
p 1953 325 100 0 0 FTD:STRING
p 1953 293 100 0 0 FTE:STRING
p 1953 229 100 0 0 FTF:STRING
p 1953 229 100 0 0 FTG:STRING
p 1953 197 100 0 0 FTH:STRING
p 1953 165 100 0 0 FTI:STRING
p 1953 133 100 0 0 FTJ:STRING
p 2256 1408 100 0 1 FTVA:STRING
p 2256 1328 100 0 1 FTVB:STRING
p 2288 1264 100 0 1 FTVC:LONG
p 2240 542 100 0 1 SNAM:gmosHealthCombineString
p 1968 1374 100 0 0 def(INPA):0.0
p 1968 1294 100 0 0 def(INPB):0.0
p 2544 1374 100 0 -1 def(OUTA):$(sad)health.VAL
p 2544 1296 100 0 -1 def(OUTB):$(sad)health.OMSS
p 2464 1386 75 0 -1 pproc(OUTA):PP
p 2464 1322 75 0 -1 pproc(OUTB):NPP
[comments]

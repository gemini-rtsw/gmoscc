[schematic2]
uniq 55
[tools]
[detail]
w 2104 1419 100 0 n#54 elongouts.elongouts#50.FLNK 2224 1568 2384 1568 2384 1408 1872 1408 1872 1312 1984 1312 elongouts.elongouts#14.SLNK
w 1672 1211 100 0 n#53 ewait.MonChange.FLNK 1568 1200 1824 1200 1824 1536 1968 1536 elongouts.elongouts#50.SLNK
w 2392 1515 100 0 c#43 elongouts.elongouts#50.OUT 2224 1504 2608 1504 2608 1264 2752 1264 outhier.CVAL.p
w 1648 1499 100 0 n#51 ewait.MonChange.VAL 1568 1488 1776 1488 1776 1568 1968 1568 elongouts.elongouts#50.DOL
w 2368 971 100 0 c#46 estringouts.estringouts#41.FLNK 2256 960 2528 960 2528 896 2672 896 2672 928 2752 928 outhier.CFLK.p
w 2416 939 100 0 c#44 estringouts.estringouts#41.OUT 2256 928 2624 928 2624 1168 2752 1168 outhier.CMESS.p
w 2368 1291 100 0 c#45 elongouts.elongouts#14.OUT 2240 1280 2544 1280 2544 1056 2752 1056 outhier.CERR.p
w 2136 1099 100 0 n#42 elongouts.elongouts#14.FLNK 2240 1344 2464 1344 2464 1088 1856 1088 1856 944 2000 944 estringouts.estringouts#41.SLNK
s 3200 2528 140 0 gmSeqObserveCar
[cell use]
use elongouts 1968 1447 100 0 elongouts#50
xform 0 2096 1536
p 1808 1454 100 0 0 OMSL:closed_loop
p 2112 1440 100 1024 1 name:$(gm)observeDcCarVal
p 2224 1504 75 768 -1 pproc(OUT):PP
use elongouts 1984 1223 100 0 elongouts#14
xform 0 2112 1312
p 1824 1230 100 0 0 OMSL:closed_loop
p 1728 1376 100 0 -1 def(DOL):$(dc)observeC.OERR
p 2144 1200 100 1024 1 name:$(gm)observeDcCarOerr
use outhier 2784 1248 100 0 CVAL
xform 0 2736 1264
use outhier 2784 1168 100 0 CMESS
xform 0 2736 1168
use outhier 2784 1040 100 0 CERR
xform 0 2736 1056
use outhier 2784 928 100 0 CFLK
xform 0 2736 928
use estringouts 2000 871 100 0 estringouts#41
xform 0 2128 944
p 1936 750 100 0 0 OMSL:supervisory
p 1792 1008 100 0 -1 def(DOL):$(dc)observeC.OMSS
p 2112 864 100 1024 1 name:$(gm)observeDcCarOmss
use ewait 864 1111 -100 0 MonChange
xform 0 1216 1440
p 1187 1688 100 0 1 CALC:A
p 1248 1488 100 0 1 INAP:Yes
p 1248 1456 100 0 0 INBP:No
p 1248 1424 100 0 0 INCP:No
p 992 1264 100 0 1 OOPT:On Change
p 992 1582 100 0 1 SCAN:I/O Intr
p 560 1568 100 0 -1 def(INAN): $(dc)observeC.VAL
p 1152 1072 100 1024 1 name:$(gm)observeDcWaitCar
use oslBorderC 320 87 100 0 oslBorderC#1
xform 0 2000 1392
p 3260 336 120 256 -1 Title:GMOS IS - monitor observeC CAR record
[comments]

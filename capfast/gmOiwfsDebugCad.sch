[schematic2]
uniq 274
[tools]
[detail]
w 1122 1675 100 0 n#273 ecad2.debug.MESS 1056 1472 1088 1472 1088 1664 1216 1664 estringouts.debugMess.DOL
w 1066 1163 100 0 n#272 ecad2.debug.FLNK 1056 1152 1136 1152 1136 1632 1216 1632 estringouts.debugMess.SLNK
w 1842 1355 100 0 n#270 eseqs.debugSeq.LNK1 1824 1344 1920 1344 junction
w 1970 1739 100 0 n#270 eseqs.debugSeq.LNK3 1824 1280 1920 1280 1920 1728 2080 1728 ecars.debugC.IVAL
w 2098 1323 100 0 DEBUG eseqs.debugSeq.LNK2 1824 1312 2432 1312 outhier.DEBUG.p
w 1250 1035 100 0 n#267 ecad2.debug.STLK 1056 1024 1504 1024 eseqs.debugSeq.SLNK
w 1250 1323 100 0 n#266 ecad2.debug.VALA 1056 1312 1504 1312 eseqs.debugSeq.DOL2
w 1508 1339 100 2 n#265 hwin.hwin#260.in 1504 1344 1504 1344 eseqs.debugSeq.DOL1
w 1508 1275 100 2 n#264 hwin.hwin#262.in 1504 1280 1504 1280 eseqs.debugSeq.DOL3
w 656 1328 100 0 debug embbos.debugMode.OUT 544 1312 736 1312 ecad2.debug.A
[cell use]
use estringouts 1376 1712 100 0 debugMess
xform 0 1344 1632
p 1248 1536 100 0 1 OMSL:closed_loop
p 1376 1712 100 512 -1 PV:$(top)
p 1440 1584 100 0 -1 def(OUT):$(top)rejectMess.IMSS
use ecars 2272 1776 100 0 debugC
xform 0 2240 1616
p 2272 1776 100 512 -1 PV:$(top)
use outhier 2400 1271 100 0 DEBUG
xform 0 2416 1312
use hwin 1312 1303 100 0 hwin#260
xform 0 1408 1344
p 1315 1336 100 0 -1 val(in):2
use hwin 1312 1239 100 0 hwin#262
xform 0 1408 1280
p 1315 1272 100 0 -1 val(in):0
use eseqs 1664 1424 100 0 debugSeq
xform 0 1664 1184
p 1632 976 100 0 1 DLY1:0.0
p 1632 944 100 0 1 DLY2:0.0
p 1632 912 100 0 1 DLY3:1.0
p 1664 1424 100 512 -1 PV:$(top)
p 1840 1344 75 1024 -1 pproc(LNK1):PP
p 1840 1312 75 1024 -1 pproc(LNK2):PP
p 1840 1280 75 1024 -1 pproc(LNK3):PP
use embbos 432 1392 100 0 debugMode
xform 0 416 1312
p 448 1056 100 0 1 FRST:MAX
p 256 1056 100 0 1 FRVL:4
p 416 1264 100 1792 1 NOBT:3
p 256 1216 100 0 1 OMSL:supervisory
p 448 1152 100 0 1 ONST:NONE
p 256 1152 100 0 1 ONVL:1
p 432 1392 100 512 -1 PV:$(top)
p 448 1088 100 0 1 THST:FULL
p 256 1088 100 0 1 THVL:3
p 448 1120 100 0 1 TWST:MIN
p 256 1120 100 0 1 TWVL:2
p 448 1184 100 0 1 ZRST:QUIET
p 256 1184 100 0 1 ZRVL:0
use ecad2 896 1552 100 0 debug
xform 0 896 1248
p 960 1312 70 512 -1 FTVA:LONG
p 880 1552 100 512 -1 PV:$(top)
p 816 976 100 0 -1 SNAM:oiwfsDebugCad
use gmosBorderC -416 -153 100 0 gmosBorderC#91
xform 0 1264 1152
p 2836 -24 100 512 1 File:gmOiwfsDebugCad.sch
p 2532 160 120 256 -1 Project:Gemini Multi-object Optical Spectrometer
p 2244 20 150 0 1 Rev:
p 2524 96 120 256 -1 Title:OIWFS TCS Debug Command
p 2564 32 100 1024 -1 author:B.Wooff
p 2564 0 100 1024 -1 date:October 3, 2000
[comments]

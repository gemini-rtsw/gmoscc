[schematic2]
uniq 322
[tools]
[detail]
w 786 811 100 0 n#321 elongouts.LocTarget.FLNK 768 800 864 800 864 1024 junction
w 898 1035 100 0 n#321 elongouts.LocIn.FLNK 768 1024 1088 1024 elongouts.MoveMode.SLNK
w 418 1003 100 0 n#320 elongouts.MaskTarget.FLNK 384 992 512 992 elongouts.LocIn.SLNK
w 442 1035 100 0 n#319 hwin.hwin#318.in 432 1152 432 1024 512 1024 elongouts.LocIn.DOL
w 1604 1371 100 2 n#316 hwin.hwin#299.in 1600 1376 1600 1376 eseqs.ValDirSeq.DOL1
w 1282 1515 100 0 n#313 elongouts.ParkMode.FLNK 1120 1504 1504 1504 1504 1056 1600 1056 eseqs.ValDirSeq.SLNK
w 1394 1067 100 0 n#313 elongouts.MoveMode.FLNK 1344 1056 1504 1056 junction
w 1378 1739 100 0 n#313 elongouts.ModeTarget.FLNK 1312 1728 1504 1728 1504 1504 junction
w 706 1483 100 0 n#314 elongouts.ParkTarget.FLNK 608 1472 864 1472 elongouts.ParkMode.SLNK
w 868 1499 100 2 n#312 hwin.hwin#311.in 864 1504 864 1504 elongouts.ParkMode.DOL
w 1092 1051 100 2 n#310 hwin.hwin#300.in 1088 1056 1088 1056 elongouts.MoveMode.DOL
[cell use]
use hwin 240 1111 100 0 hwin#318
xform 0 336 1152
p 243 1144 100 0 -1 val(in):0
use hwin 1408 1335 100 0 hwin#299
xform 0 1504 1376
p 1411 1368 100 0 -1 val(in):$(START)
use hwin 896 1015 100 0 hwin#300
xform 0 992 1056
p 899 1048 100 0 -1 val(in):$(MOVE)
use hwin 672 1463 100 0 hwin#311
xform 0 768 1504
p 675 1496 100 0 -1 val(in):$(PARK)
use elongouts 672 1072 100 0 LocIn
xform 0 640 992
p 608 896 100 0 1 OMSL:supervisory
p 672 1072 100 512 -1 PV:$(top)$(dev)
p 704 928 70 0 1 def(OUT):$(top)$(dev)Assembly.B
use elongouts 1216 1776 100 0 ModeTarget
xform 0 1184 1696
p 1120 1600 100 0 1 OMSL:supervisory
p 1216 1776 100 512 -1 PV:$(top)$(dev)
p 1248 1632 70 0 1 def(OUT):$(top)$(dev)Assembly.MODE
use elongouts 512 1520 100 0 ParkTarget
xform 0 480 1440
p 384 1328 100 0 1 OMSL:supervisory
p 496 1520 100 512 -1 PV:$(top)$(dev)
p 544 1376 70 0 1 def(OUT):$(top)$(dev)Assembly.C
use elongouts 288 1040 100 0 MaskTarget
xform 0 256 960
p 160 864 100 0 1 OMSL:supervisory
p 272 1040 100 512 -1 PV:$(top)$(dev)
p 320 896 70 0 1 def(OUT):$(top)$(dev)Assembly.A
use elongouts 672 848 100 0 LocTarget
xform 0 640 768
p 560 672 100 0 1 OMSL:supervisory
p 656 848 100 512 -1 PV:$(top)$(dev)
p 704 704 70 0 1 def(OUT):$(top)$(dev)Assembly.B
use elongouts 1248 1104 100 0 MoveMode
xform 0 1216 1024
p 1120 928 100 0 1 OMSL:supervisory
p 1248 1104 100 512 -1 PV:$(top)$(dev)
p 1280 960 70 0 1 def(OUT):$(top)$(dev)Assembly.MODE
use elongouts 1024 1552 100 0 ParkMode
xform 0 992 1472
p 896 1376 100 0 1 OMSL:supervisory
p 1024 1552 100 512 -1 PV:$(top)$(dev)
p 1072 1408 70 0 1 def(OUT):$(top)$(dev)Assembly.MODE
use eseqs 1792 1456 100 0 ValDirSeq
xform 0 1760 1216
p 1712 976 100 0 1 DLY1:0.5
p 1712 944 100 0 0 DLY2:0.0
p 1776 1456 100 512 -1 PV:$(top)$(dev)
p 2016 1376 70 0 1 def(LNK1):$(top)$(dev)Assembly.DIR
p 2016 1344 70 0 0 def(LNK2):0.0
p 1936 1376 75 1024 -1 pproc(LNK1):PP
p 1936 1344 75 1024 -1 pproc(LNK2):NPP
use gmosBorderC -416 -153 100 0 gmosBorderC#91
xform 0 1264 1152
p 2840 -24 100 512 1 File:/data/nimpkish/1/daoinsw/dev/gmos/current/develop/gmMisc/capfast/gmAssemblyDriver.sch
p 1216 1088 100 0 0 IO:
p 2532 160 120 256 -1 Project:Gemini Multi-object Optical Spectrometer
p 2244 20 150 0 1 Rev:
p 2524 96 120 256 -1 Title:Engineering Driver for assemblyControl
p 2564 32 100 1024 -1 author:B.Wooff
p 2564 0 100 1024 -1 date:January 31, 2000
p 1152 1056 100 0 0 model:
p 1152 1024 100 0 0 revision:
[comments]

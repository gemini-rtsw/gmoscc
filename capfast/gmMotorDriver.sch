[schematic2]
uniq 318
[tools]
[detail]
w 418 1099 100 0 n#316 eaos.ValIndx.FLNK 320 1088 576 1088 elongouts.Indx0.SLNK
w 898 1131 100 0 n#315 elongouts.Indx0.FLNK 832 1120 1024 1120 elongouts.IndexMode.SLNK
w 1426 1451 100 0 n#307 elongouts.MoveMode.FLNK 1280 1440 1632 1440 eseqs.DirSeq.SLNK
w 1170 1867 100 0 n#307 elongouts.ModeTarget.FLNK 1024 1856 1376 1856 1376 1440 junction
w 1298 1163 100 0 n#307 elongouts.IndexMode.FLNK 1280 1152 1376 1152 1376 1440 junction
w 580 1115 100 2 n#314 hwin.hwin#313.in 576 1120 576 1120 elongouts.Indx0.DOL
w 1028 1147 100 2 n#312 hwin.hwin#311.in 1024 1152 1024 1152 elongouts.IndexMode.DOL
w 658 1611 100 0 n#302 estringouts.ValsTarget.FLNK 640 1600 736 1600 736 1408 junction
w 802 1419 100 0 n#302 eaos.ValTarget.FLNK 640 1408 1024 1408 elongouts.MoveMode.SLNK
w 1636 1755 100 2 n#306 hwin.hwin#299.in 1632 1760 1632 1760 eseqs.DirSeq.DOL1
w 1028 1435 100 2 n#305 hwin.hwin#300.in 1024 1440 1024 1440 elongouts.MoveMode.DOL
[cell use]
use ecalcs 1760 960 100 0 CalcEncErr
xform 0 1792 720
p 1776 912 100 0 1 CALC:B - ( A * D / C )
p 1760 960 100 512 -1 PV:$(top)$(dev)
p 1808 528 100 0 1 SCAN:1 second
p 1536 928 100 512 1 def(INPA):$(top)$(dev)Device.RRBV
p 1536 896 100 512 1 def(INPB):$(top)$(dev)Device.RENC
p 1536 864 100 512 1 def(INPC):$(top)$(dev)Device.MRES
p 1536 832 100 512 1 def(INPD):$(top)$(dev)Device.ERES
use hwin 1440 1719 100 0 hwin#299
xform 0 1536 1760
p 1443 1752 100 0 -1 val(in):$(GO)
use hwin 832 1399 100 0 hwin#300
xform 0 928 1440
p 835 1432 100 0 -1 val(in):$(MOVE)
use hwin 832 1111 100 0 hwin#311
xform 0 928 1152
p 835 1144 100 0 -1 val(in):$(INDEX)
use hwin 384 1079 100 0 hwin#313
xform 0 480 1120
p 387 1112 100 0 -1 val(in):0
use elongouts 928 1904 100 0 ModeTarget
xform 0 896 1824
p 832 1728 100 0 1 OMSL:supervisory
p 928 1904 100 512 -1 PV:$(top)$(dev)
p 960 1760 70 0 1 def(OUT):$(top)$(dev)Device.MODE
use elongouts 1184 1488 100 0 MoveMode
xform 0 1152 1408
p 1088 1312 100 0 1 OMSL:supervisory
p 1184 1488 100 512 -1 PV:$(top)$(dev)
p 1216 1344 70 0 1 def(OUT):$(top)$(dev)Device.MODE
use elongouts 736 1168 100 0 Indx0
xform 0 704 1088
p 736 1168 100 512 -1 PV:$(top)$(dev)
p 768 1024 70 0 1 def(OUT):$(top)$(dev)Device.IALG
use elongouts 1184 1200 100 0 IndexMode
xform 0 1152 1120
p 1184 1200 100 512 -1 PV:$(top)$(dev)
p 1216 1056 70 0 1 def(OUT):$(top)$(dev)Device.MODE
use eaos 544 1456 100 0 ValTarget
xform 0 512 1376
p 448 1280 100 0 1 OMSL:supervisory
p 512 1328 100 256 1 PREC:4
p 544 1456 100 512 -1 PV:$(top)$(dev)
p 576 1312 70 0 1 def(OUT):$(top)$(dev)Device.VAL
use eaos 224 1136 100 0 ValIndx
xform 0 192 1056
p 224 1136 100 512 -1 PV:$(top)$(dev)
p 256 992 70 0 1 def(OUT):$(top)$(dev)Device.VAL
use estringouts 544 1648 100 0 ValsTarget
xform 0 512 1584
p 416 1504 100 0 1 OMSL:supervisory
p 544 1648 100 512 -1 PV:$(top)$(dev)
p 592 1536 70 0 1 def(OUT):$(top)$(dev)Device.VALS
use eseqs 1824 1840 100 0 DirSeq
xform 0 1792 1600
p 1744 1376 100 0 0 DLY1:0.5
p 1744 1328 100 0 0 DLY2:0.0
p 1808 1840 100 512 -1 PV:$(top)$(dev)
p 2048 1760 70 0 1 def(LNK1):$(top)$(dev)Device.DIR
p 2048 1728 70 0 0 def(LNK2):0.0
p 1968 1760 75 1024 -1 pproc(LNK1):PP
p 1968 1728 75 1024 -1 pproc(LNK2):NPP
use gmosBorderC -416 -153 100 0 gmosBorderC#91
xform 0 1264 1152
p 2840 -24 100 512 1 File:/sw3/gmos/development/gmosHIA/rel/capfast/gmMotorDriver.sch
p 1216 1088 100 0 0 IO:
p 2532 160 120 256 -1 Project:Gemini Multi-object Optical Spectrometer
p 2244 20 150 0 1 Rev:
p 2524 96 120 256 -1 Title:Engineering Driver for deviceControl
p 2564 32 100 1024 -1 author:B.Wooff
p 2564 0 100 1024 -1 date:July 12, 2000
p 1152 1056 100 0 0 model:
p 1152 1024 100 0 0 revision:
[comments]

[schematic2]
uniq 339
[tools]
[detail]
w 738 1419 100 0 n#327 efanouts.Move1.LNK3 512 1408 1024 1408 1024 1344 1248 1344 elongouts.MoveMode.SLNK
w 738 1035 100 0 n#327 efanouts.Move2.LNK3 512 1024 1024 1024 1024 1344 junction
w 738 651 100 0 n#327 efanouts.Move3.LNK3 512 640 1024 640 1024 1024 junction
w 522 715 100 0 n#335 efanouts.Move3.LNK1 512 704 592 704 592 736 672 736 eaos.MoveX3Target.SLNK
w 530 683 100 0 n#334 efanouts.Move3.LNK2 512 672 608 672 608 544 672 544 eaos.MoveY3Target.SLNK
w 522 1099 100 0 n#330 efanouts.Move2.LNK1 512 1088 592 1088 592 1120 672 1120 eaos.MoveX2Target.SLNK
w 530 1067 100 0 n#329 efanouts.Move2.LNK2 512 1056 608 1056 608 928 672 928 eaos.MoveY2Target.SLNK
w 530 1451 100 0 n#326 efanouts.Move1.LNK2 512 1440 608 1440 608 1312 672 1312 eaos.MoveY1Target.SLNK
w 522 1483 100 0 n#325 efanouts.Move1.LNK1 512 1472 592 1472 592 1504 672 1504 eaos.MoveX1Target.SLNK
w 1764 1691 100 2 n#316 hwin.hwin#299.in 1760 1696 1760 1696 eseqs.ValDirSeq.DOL1
w 1554 1387 100 0 n#313 elongouts.MoveMode.FLNK 1504 1376 1760 1376 eseqs.ValDirSeq.SLNK
w 1474 2059 100 0 n#313 elongouts.ModeTarget.FLNK 1472 2048 1536 2048 1536 1376 junction
w 1252 1371 100 2 n#310 hwin.hwin#300.in 1248 1376 1248 1376 elongouts.MoveMode.DOL
[cell use]
use eaos 832 1584 100 0 MoveX1Target
xform 0 800 1504
p 800 1456 50 256 1 PREC:4
p 832 1584 100 512 -1 PV:$(top)$(dev)
p 864 1440 50 0 1 def(OUT):$(top)$(dev)Assembly.A
use eaos 832 1392 100 0 MoveY1Target
xform 0 800 1312
p 800 1264 50 256 1 PREC:4
p 832 1392 100 512 -1 PV:$(top)$(dev)
p 864 1248 50 0 1 def(OUT):$(top)$(dev)Assembly.B
use eaos 832 1200 100 0 MoveX2Target
xform 0 800 1120
p 800 1072 50 256 1 PREC:4
p 832 1200 100 512 -1 PV:$(top)$(dev)
p 864 1056 50 0 1 def(OUT):$(top)$(dev)Assembly.A
use eaos 832 1008 100 0 MoveY2Target
xform 0 800 928
p 800 880 50 256 1 PREC:4
p 832 1008 100 512 -1 PV:$(top)$(dev)
p 864 864 50 0 1 def(OUT):$(top)$(dev)Assembly.B
use eaos 832 816 100 0 MoveX3Target
xform 0 800 736
p 800 688 50 256 1 PREC:4
p 832 816 100 512 -1 PV:$(top)$(dev)
p 864 672 50 0 1 def(OUT):$(top)$(dev)Assembly.A
use eaos 832 624 100 0 MoveY3Target
xform 0 800 544
p 800 496 50 256 1 PREC:4
p 816 624 100 512 -1 PV:$(top)$(dev)
p 864 480 50 0 1 def(OUT):$(top)$(dev)Assembly.B
use efanouts 448 1552 100 0 Move1
xform 0 392 1408
p 432 1552 100 512 -1 PV:$(top)$(dev)
use efanouts 448 1168 100 0 Move2
xform 0 392 1024
p 432 1168 100 512 -1 PV:$(top)$(dev)
use efanouts 448 784 100 0 Move3
xform 0 392 640
p 432 784 100 512 -1 PV:$(top)$(dev)
use hwin 1568 1655 100 0 hwin#299
xform 0 1664 1696
p 1571 1688 100 0 -1 val(in):$(START)
use hwin 1056 1335 100 0 hwin#300
xform 0 1152 1376
p 1059 1368 100 0 -1 val(in):$(MOVE)
use elongouts 1376 2096 100 0 ModeTarget
xform 0 1344 2016
p 1280 1920 100 0 1 OMSL:supervisory
p 1376 2096 100 512 -1 PV:$(top)$(dev)
p 1408 1952 70 0 1 def(OUT):$(top)$(dev)Assembly.MODE
use elongouts 1408 1424 100 0 MoveMode
xform 0 1376 1344
p 1280 1248 100 0 1 OMSL:supervisory
p 1408 1424 100 512 -1 PV:$(top)$(dev)
p 1440 1280 70 0 1 def(OUT):$(top)$(dev)Assembly.MODE
use eseqs 1952 1776 100 0 ValDirSeq
xform 0 1920 1536
p 1872 1296 100 0 1 DLY1:0.5
p 1872 1264 100 0 0 DLY2:0.0
p 1936 1776 100 512 -1 PV:$(top)$(dev)
p 2176 1696 70 0 1 def(LNK1):$(top)$(dev)Assembly.DIR
p 2176 1664 70 0 0 def(LNK2):0.0
p 2096 1696 75 1024 -1 pproc(LNK1):PP
p 2096 1664 75 1024 -1 pproc(LNK2):NPP
use gmosBorderC -416 -153 100 0 gmosBorderC#91
xform 0 1264 1152
p 2840 -24 100 512 1 File:/data/nimpkish/1/daoinsw/dev/gmos/current/develop/gmMisc/capfast/gmOiwfsAssembly.sch
p 1216 1088 100 0 0 IO:
p 2532 160 120 256 -1 Project:Gemini Multi-object Optical Spectrometer
p 2244 20 150 0 1 Rev:
p 2524 96 120 256 -1 Title:Engineering Driver for OIWFS Assembly
p 2564 32 100 1024 -1 author:B.Wooff
p 2564 0 100 1024 -1 date:January 31, 2000
p 1152 1056 100 0 0 model:
p 1152 1024 100 0 0 revision:
[comments]

[schematic2]
uniq 59
[tools]
[detail]
w 1604 939 100 2 n#55 hwin.hwin#54.in 1600 944 1600 944 ebis.MasterDisabled.INP
w 504 395 100 0 n#56 eseqs.WdgEnableHoldoff.LNK6 288 1008 384 1008 384 384 672 384 ebos.sensorEnable.SLNK
w -120 1019 100 0 n#37 junction -160 1040 -160 1008 -32 1008 eseqs.WdgEnableHoldoff.DOL6
w -120 1051 100 0 n#37 junction -160 1072 -160 1040 -32 1040 eseqs.WdgEnableHoldoff.DOL5
w -120 1115 100 0 n#37 hwin.hwin#21.in -160 1104 -32 1104 eseqs.WdgEnableHoldoff.DOL3
w -120 1083 100 0 n#37 eseqs.WdgEnableHoldoff.DOL4 -32 1072 -160 1072 -160 1104 junction
w 528 683 100 0 n#57 eseqs.WdgEnableHoldoff.LNK5 288 1040 432 1040 432 672 672 672 ebos.optsensorEnable.SLNK
w 932 347 100 2 n#50 hwout.hwout#49.outp 928 352 928 352 ebos.sensorEnable.OUT
w 932 635 100 2 n#48 hwout.hwout#47.outp 928 640 928 640 ebos.optsensorEnable.OUT
w -120 1179 100 0 n#43 hwin.hwin#19.in -160 1168 -32 1168 eseqs.WdgEnableHoldoff.DOL1
w -120 1147 100 0 n#43 eseqs.WdgEnableHoldoff.DOL2 -32 1136 -160 1136 -160 1168 junction
w 360 1083 100 0 n#58 eseqs.WdgEnableHoldoff.LNK4 288 1072 480 1072 480 928 672 928 ebos.masterEnable.SLNK
w 360 1147 100 0 n#58 junction 480 1072 480 1136 288 1136 eseqs.WdgEnableHoldoff.LNK2
w 376 1115 100 0 n#41 eseqs.WdgEnableHoldoff.LNK3 288 1104 512 1104 512 1168 288 1168 eseqs.WdgEnableHoldoff.LNK1
w 568 1147 100 0 n#41 junction 512 1136 672 1136 ebis.WdgBuffer.SLNK
w 904 907 100 0 n#31 ebos.masterEnable.OUT 928 896 928 896 hwout.hwout#33.outp
w 1480 1387 100 0 n#30 ebos.WdgWrite.SLNK 1600 1376 1408 1376 ecalcs.WdgGenerate.FLNK
w 1208 1643 100 0 n#29 ecalcs.WdgGenerate.VAL 1408 1344 1472 1344 1472 1632 992 1632 992 1536 1120 1536 ecalcs.WdgGenerate.INPA
w 1512 1419 100 0 n#29 ebos.WdgWrite.DOL 1600 1408 1472 1408 junction
w 1000 1131 100 0 n#18 ebis.WdgBuffer.VAL 928 1120 1120 1120 ecalcs.WdgGenerate.SDIS
w 1832 1355 100 0 n#4 ebos.WdgWrite.OUT 1856 1344 1856 1344 hwout.hwout#7.outp
s 256 -112 100 1024 A
s 288 -112 100 768 Changed port to reflect wiring changes.
s 784 -112 100 1024 BMW
s 896 -112 100 1024 20-MAY-99
[cell use]
use hwin 1408 903 100 0 hwin#54
xform 0 1504 944
p 1520 896 100 512 -1 val(in):#<$(allOff)>
use hwin -352 1127 100 0 hwin#19
xform 0 -256 1168
p -349 1160 100 0 -1 val(in):0
use hwin -352 1063 100 0 hwin#21
xform 0 -256 1104
p -349 1096 100 0 -1 val(in):1
use ebis 1776 992 100 0 MasterDisabled
xform 0 1728 912
p 1600 832 100 0 1 DTYP:XYCOM-240
p 1600 736 100 0 1 ONAM:Disabled
p 1760 992 100 512 1 PV:$(top)$(dev)
p 1600 800 100 0 1 SCAN:I/O Intr
p 1600 768 100 0 1 ZNAM:Enabled
use ebis 848 1216 100 0 WdgBuffer
xform 0 800 1136
p 672 974 100 0 0 DISV:1
p 832 1216 100 512 1 PV:$(top)$(dev)
use hwout 928 311 100 0 hwout#49
xform 0 1024 352
p 1008 304 100 0 -1 val(outp):#<$(sensorEn)>
use hwout 928 599 100 0 hwout#47
xform 0 1024 640
p 1024 592 100 0 -1 val(outp):#<$(encoderEn)>
use hwout 928 855 100 0 hwout#33
xform 0 1024 896
p 1008 848 100 0 -1 val(outp):#<$(mstrEn)>
use hwout 1856 1303 100 0 hwout#7
xform 0 1952 1344
p 1936 1296 100 0 -1 val(outp):#<$(wDog)>
use ebos 880 464 100 0 sensorEnable
xform 0 800 384
p 672 256 100 0 1 ONAM:ON
p 688 464 100 0 1 PV:$(top)$(dev)
p 672 288 100 0 1 ZNAM:OFF
use ebos 880 752 100 0 optsensorEnable
xform 0 800 672
p 672 544 100 0 1 ONAM:ON
p 688 752 100 0 1 PV:$(top)$(dev)
p 672 576 100 0 1 ZNAM:OFF
use ebos 880 1008 100 0 masterEnable
xform 0 800 928
p 511 1133 100 0 0 DTYP:XYCOM-240
p 832 864 100 0 0 OMSL:supervisory
p 672 800 100 0 1 ONAM:ON
p 688 1008 100 0 1 PV:$(top)$(dev)
p 352 1006 100 0 0 SCAN:Passive
p 672 832 100 0 1 ZNAM:OFF
use ebos 1808 1472 100 0 WdgWrite
xform 0 1728 1376
p 1663 1247 100 0 1 DTYP:XYCOM-240
p 1280 1326 100 0 0 OMSL:closed_loop
p 1664 1152 100 0 1 ONAM:ON
p 1792 1472 100 512 1 PV:$(top)$(dev)
p 1664 1216 100 0 1 SCAN:Passive
p 1664 1184 100 0 1 ZNAM:OFF
use gmosBorderC -768 -281 100 0 gmosBorderC#44
xform 0 912 1024
p 2484 -152 100 512 1 File:/data/nimpkish/1/daoinsw/dev/gmos/current/develop/gmMisc/capfast/gmMiscWdg.sch
p 1892 -108 150 0 1 Rev:A
p 2172 -32 120 256 -1 Title:Watchdog Timer
p 2212 -96 100 1024 -1 author:B.Wooff
p 2212 -128 100 1024 -1 date:September 15, 1999
use eseqs 240 1248 100 0 WdgEnableHoldoff
xform 0 128 1008
p 64 1072 100 0 1 DLY1:0.0e+00
p 64 1040 100 0 1 DLY2:0.0e+00
p 64 960 100 0 1 DLY3:5.0e+00
p 64 928 100 0 1 DLY4:2.0e+00
p -416 1166 100 0 0 PINI:YES
p 224 1248 100 512 1 PV:$(top)$(dev)
p -64 1168 75 1280 -1 pproc(DOL1):NPP
p -64 1136 75 1280 -1 pproc(DOL2):NPP
p -64 1104 75 1280 -1 pproc(DOL3):NPP
p -64 1072 75 1280 -1 pproc(DOL4):NPP
p -64 880 75 1280 -1 pproc(DOLA):NPP
p 304 1168 75 1024 -1 pproc(LNK1):PP
p 304 1136 75 1024 -1 pproc(LNK2):PP
p 304 1104 75 1024 -1 pproc(LNK3):PP
p 304 1072 75 1024 -1 pproc(LNK4):PP
p 304 1040 75 1024 -1 pproc(LNK5):PP
p 304 1008 75 1024 -1 pproc(LNK6):PP
use ecalcs 1296 1568 100 0 WdgGenerate
xform 0 1264 1328
p 1344 1152 100 0 1 CALC:!A
p 832 1310 100 0 0 DISV:0
p 1280 1568 100 512 1 PV:$(top)$(dev)
p 1344 1216 100 0 1 SCAN:.1 second
[comments]

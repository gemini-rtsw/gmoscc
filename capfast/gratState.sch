[schematic2]
uniq 76
[tools]
[detail]
w 1288 1259 100 0 n#75 egenSub.SwitchStates.OUTB 1248 1248 1376 1248 hwout.hwout#74.outp
w 56 667 100 0 n#72 ebis.TurretPosC.FLNK 224 832 416 832 416 656 -256 656 -256 560 -32 560 ebis.TurretPosD.SLNK
w 56 923 100 0 n#71 ebis.TurretPosB.FLNK 224 1088 416 1088 416 912 -256 912 -256 816 -32 816 ebis.TurretPosC.SLNK
w 56 1179 100 0 n#70 ebis.TurretPosA.FLNK 224 1344 416 1344 416 1168 -256 1168 -256 1072 -32 1072 ebis.TurretPosB.SLNK
w 488 579 100 0 n#61 ebis.TurretPosD.FLNK 224 576 800 576 800 640 960 640 egenSub.SwitchStates.SLNK
w 548 827 100 0 n#59 ebis.TurretPosD.VAL 224 544 544 544 544 1120 960 1120 egenSub.SwitchStates.INPD
w 712 1195 100 0 n#58 ebis.TurretPosC.VAL 224 800 512 800 512 1184 960 1184 egenSub.SwitchStates.INPC
w -56 859 100 0 n#53 ebis.TurretPosC.INP -32 848 -32 848 hwin.hwin#56.in
w -56 603 100 0 n#52 ebis.TurretPosD.INP -32 592 -32 592 hwin.hwin#57.in
w 696 1259 100 0 n#51 ebis.TurretPosB.VAL 224 1056 480 1056 480 1248 960 1248 egenSub.SwitchStates.INPB
w 568 1323 100 0 n#50 ebis.TurretPosA.VAL 224 1312 960 1312 egenSub.SwitchStates.INPA
w 1554 1315 100 0 state outhier.state.p 1920 1312 1248 1312 egenSub.SwitchStates.OUTA
w -56 1371 100 0 n#46 hwin.hwin#13.in -32 1360 -32 1360 ebis.TurretPosA.INP
w -56 1115 100 0 n#45 hwin.hwin#16.in -32 1104 -32 1104 ebis.TurretPosB.INP
s -128 1472 100 0 Grating in position microswitches
s 752 1472 100 0 This gensub combines the switch states into a single 0101 string.
s -352 1952 300 0 Monitor the state fo the system microswitches
[cell use]
use hwout 1376 1207 100 0 hwout#74
xform 0 1472 1248
p 1472 1239 100 0 -1 val(outp):$(sadtop)$(dev)InBeam PP NMS
use notes -160 1511 100 0 notes#73
xform 0 96 1696
p -132 1822 100 0 -1 COMMENT1:All of the binary records with
p -132 1790 100 0 -1 COMMENT2:DTYP=XYCOM-240 below will process
p -132 1760 100 0 -1 COMMENT3:when any one of the bits in the XYCOM
p -132 1728 100 0 -1 COMMENT4:changes. To prevent a whole cascade of
p -132 1696 100 0 -1 COMMENT5:processing whenever a single bit changes,
p -132 1664 100 0 -1 COMMENT6:only one of the records is triggered by
p -132 1632 100 0 -1 COMMENT7:an I/O interrupt, and the others are
p -132 1600 100 0 -1 COMMENT8:forward linked
use hwin -224 551 100 0 hwin#57
xform 0 -128 592
p -221 584 100 0 -1 val(in):#<$(grtAtD)>
use hwin -224 807 100 0 hwin#56
xform 0 -128 848
p -221 840 100 0 -1 val(in):#<$(grtAtC)>
use hwin -224 1063 100 0 hwin#16
xform 0 -128 1104
p -221 1096 100 0 -1 val(in):#<$(grtAtB)>
use hwin -224 1319 100 0 hwin#13
xform 0 -128 1360
p -221 1352 100 0 -1 val(in):#<$(grtAtA)>
use ebis 224 624 100 0 TurretPosD
xform 0 96 560
p 32 416 100 0 1 DTYP:$(xycom)
p 32 480 100 0 1 ONAM:NOTINPOSITION
p 32 624 100 0 1 PV:$(top)$(dev)
p -160 480 100 0 1 SCAN:Passive
p 32 448 100 0 1 ZNAM:INPOSITION
use ebis 224 880 100 0 TurretPosC
xform 0 96 816
p 32 672 100 0 1 DTYP:$(xycom)
p 32 736 100 0 1 ONAM:NOTINPOSITION
p 32 880 100 0 1 PV:$(top)$(dev)
p -160 736 100 0 1 SCAN:Passive
p 32 704 100 0 1 ZNAM:INPOSITION
use ebis 224 1136 100 0 TurretPosB
xform 0 96 1072
p 32 928 100 0 1 DTYP:$(xycom)
p 32 992 100 0 1 ONAM:NOTINPOSITION
p 32 1136 100 0 1 PV:$(top)$(dev)
p -160 992 100 0 1 SCAN:Passive
p 32 960 100 0 1 ZNAM:INPOSITION
use ebis 224 1392 100 0 TurretPosA
xform 0 96 1328
p 32 1184 100 0 1 DTYP:$(xycom)
p 32 1248 100 0 1 ONAM:NOTINPOSITION
p 32 1392 100 0 1 PV:$(top)$(dev)
p -160 1248 100 0 1 SCAN:$(iointr)
p 32 1216 100 0 1 ZNAM:INPOSITION
use outhier 1888 1271 100 0 state
xform 0 1904 1312
use egenSub 1216 1408 100 0 SwitchStates
xform 0 1104 976
p 803 1291 100 0 0 DESC:compile string of switch status
p 1040 1344 100 0 -1 FTA:LONG
p 1040 1264 100 0 -1 FTB:LONG
p 1040 1184 100 0 -1 FTC:LONG
p 1040 1120 100 0 -1 FTD:LONG
p 1120 1344 100 0 -1 FTVA:STRING
p 1120 1280 100 0 -1 FTVB:LONG
p 1120 1216 100 0 -1 FTVC:LONG
p 1024 512 100 0 1 INAM:swStateInit
p 1024 1408 100 0 1 PV:$(top)$(dev)
p 1024 480 100 0 1 SNAM:swState
p 1248 1322 75 0 -1 pproc(OUTA):NPP
p 1248 1258 75 0 -1 pproc(OUTB):PP
p 1248 1194 75 0 -1 pproc(OUTC):NPP
p 1248 1130 75 0 -1 pproc(OUTD):NPP
use ukatcBorderC -576 -441 100 0 ukatcBorderC#0
xform 0 1104 864
p 2364 -192 120 256 -1 Title:GMOS turret in position microswitches
p 2080 -256 100 768 -1 author:C.Tierney, S.Beard & S.Prior
[comments]

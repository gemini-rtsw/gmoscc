[schematic2]
uniq 137
[tools]
[detail]
w -150 1035 100 0 n#136 hwin.hwin#135.in -192 1024 -48 1024 ecad8.dtaPos.INPD
w 1794 907 100 0 n#134 elongouts.dtaPosTrackXYOff.OUT 1344 896 2304 896 outhier.TRACKXY.p
w 1786 1163 100 0 n#133 elongouts.dtaPosTrackZOff.OUT 1344 1152 2288 1152 outhier.TRACKZ.p
w 706 -213 100 0 n#132 ecad8.dtaPos.PLNK 272 608 384 608 384 -224 1088 -224 elongouts.dtaPosAssemblyMode.SLNK
w 754 107 100 0 n#131 ecad8.dtaPos.VALC 272 1120 480 1120 480 96 1088 96 eaos.dtaPosZDemand.DOL
w 770 395 100 0 n#130 ecad8.dtaPos.VALB 272 1184 512 1184 512 384 1088 384 eaos.dtaPosYDemand.DOL
w 786 683 100 0 n#129 ecad8.dtaPos.VALA 272 1248 544 1248 544 672 1088 672 eaos.dtaPosXDemand.DOL
w 1650 107 100 0 n#128 eaos.dtaPosZDemand.FLNK 1344 96 2016 96 eevents.dtaPosCadStEvent.SLNK
w 1218 219 100 0 n#127 eaos.dtaPosYDemand.FLNK 1344 384 1504 384 1504 208 992 208 992 64 1088 64 eaos.dtaPosZDemand.SLNK
w 1218 507 100 0 n#126 eaos.dtaPosXDemand.FLNK 1344 672 1504 672 1504 496 992 496 992 352 1088 352 eaos.dtaPosYDemand.SLNK
w 1218 795 100 0 n#125 elongouts.dtaPosTrackXYOff.FLNK 1344 960 1504 960 1504 784 992 784 992 640 1088 640 eaos.dtaPosXDemand.SLNK
w 1218 1067 100 0 n#124 elongouts.dtaPosTrackZOff.FLNK 1344 1216 1504 1216 1504 1056 992 1056 992 928 1088 928 elongouts.dtaPosTrackXYOff.SLNK
w 850 1195 100 0 n#123 ecad8.dtaPos.STLK 272 576 672 576 672 1184 1088 1184 elongouts.dtaPosTrackZOff.SLNK
w 994 971 100 0 n#120 hwin.hwin#119.in 960 960 1088 960 elongouts.dtaPosTrackXYOff.DOL
w 994 1227 100 0 n#118 hwin.hwin#117.in 960 1216 1088 1216 elongouts.dtaPosTrackZOff.DOL
w 1944 139 100 0 n#81 hwin.hwin#80.in 1856 176 1920 176 1920 128 2016 128 eevents.dtaPosCadStEvent.INP
w 1016 -190 100 0 n#53 hwin.hwin#51.in 992 -192 1088 -192 elongouts.dtaPosAssemblyMode.DOL
w 1256 1442 100 0 n#46 ecad8.dtaPos.VAL 272 1440 2288 1440 outhier.VAL.p
w 1526 1348 100 0 n#9 ecad8.dtaPos.MESS 272 1408 800 1408 800 1344 2288 1344 outhier.MESS.p
w -226 1412 100 0 n#8 inhier.ICID.P -448 1344 -368 1344 -368 1408 -48 1408 ecad8.dtaPos.ICID
w -266 1442 100 0 n#7 inhier.DIR.P -448 1440 -48 1440 ecad8.dtaPos.DIR
s 2160 1696 140 0 gmSeqDtaPos
s -320 1296 100 0 Attributes are :
s -352 1248 100 0 A: X-axis position
s -352 1184 100 0 B: Y-axis position
s -352 1120 100 0 C: Z-axis position
s 1920 1024 100 0 N.B. Tracking flags are turned off before
s 1920 992 100 0 sending positions to assembly
[cell use]
use hwin 768 919 100 0 hwin#119
xform 0 864 960
p 771 952 100 0 -1 val(in):0
use hwin 768 1175 100 0 hwin#117
xform 0 864 1216
p 771 1208 100 0 -1 val(in):0
use hwin 1664 135 100 0 hwin#80
xform 0 1760 176
p 1667 168 100 0 -1 val(in):$(event)
use hwin 824 -232 100 0 hwin#51
xform 0 896 -192
p 803 -200 100 0 -1 val(in):$(MODE_MOVE)
use hwin -384 983 100 0 hwin#135
xform 0 -288 1024
p -381 1016 100 0 -1 val(in):$(cc)dtaAssembly.INDX
use outhier 2280 1304 100 0 MESS
xform 0 2272 1344
use outhier 2280 1400 100 0 VAL
xform 0 2272 1440
use outhier 2256 1111 100 0 TRACKZ
xform 0 2272 1152
use outhier 2272 855 100 0 TRACKXY
xform 0 2288 896
use eaos 1088 551 100 0 dtaPosXDemand
xform 0 1216 640
p 1120 720 100 0 -1 DESC:Demand X position
p 1360 512 100 0 1 EGU:microns
p 1152 528 100 0 1 OMSL:closed_loop
p 1360 480 100 0 1 PREC:2
p 1456 608 100 0 -1 def(OUT):$(cc)dtaAssembly.A
use eaos 1088 263 100 0 dtaPosYDemand
xform 0 1216 352
p 1120 432 100 0 -1 DESC:Demand Y position
p 1376 224 100 0 1 EGU:microns
p 1152 240 100 0 1 OMSL:closed_loop
p 1376 192 100 0 1 PREC:2
p 1456 320 100 0 -1 def(OUT):$(cc)dtaAssembly.B
use eaos 1088 -25 100 0 dtaPosZDemand
xform 0 1216 64
p 1120 144 100 0 -1 DESC:Demand Z position
p 1376 -48 100 0 1 EGU:microns
p 1152 -48 100 0 1 OMSL:closed_loop
p 1376 -80 100 0 1 PREC:2
p 1456 32 100 0 -1 def(OUT):$(cc)dtaAssembly.C
use elongouts 1088 839 100 0 dtaPosTrackXYOff
xform 0 1216 928
p 1072 1024 100 0 -1 DESC:Disable XY track flag
p 1408 816 100 0 1 EGU:0/1
p 1152 816 100 0 1 OMSL:closed_loop
p 1344 896 75 768 -1 pproc(OUT):PP
use elongouts 1088 1095 100 0 dtaPosTrackZOff
xform 0 1216 1184
p 1072 1280 100 0 -1 DESC:Disable focus track flag
p 1408 1072 100 0 1 EGU:0/1
p 1152 1072 100 0 1 OMSL:closed_loop
p 1344 1152 75 768 -1 pproc(OUT):PP
use elongouts 1112 -312 100 0 dtaPosAssemblyMode
xform 0 1216 -224
p 1136 -130 100 0 -1 DESC:Set assembly record mode MOVE
p 1152 -336 100 0 1 OMSL:closed_loop
p 1424 -258 100 0 -1 def(OUT):$(cc)dtaAssembly.MODE
p 1312 -320 100 1024 0 name:$(top)$(I)
p 1344 -256 75 768 -1 pproc(OUT):NPP
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
p 2220 -496 120 256 -1 Title:GMOS IS - dtaPos CAD record
use eevents 2016 7 100 0 dtaPosCadStEvent
xform 0 2160 96
p 2080 -32 100 0 0 EVNT:0
p 2128 0 100 1024 0 name:$(top)$(I)
use ecad8 -24 488 100 0 dtaPos
xform 0 112 992
p -96 1534 100 0 -1 DESC:Triggers a subsystem CAD on START only
p 64 1238 100 0 1 FTVA:DOUBLE
p 64 1182 100 0 1 FTVB:DOUBLE
p 64 1120 100 0 1 FTVC:DOUBLE
p 48 1088 100 0 0 FTVD:DOUBLE
p 48 1056 100 0 0 FTVE:DOUBLE
p 48 1024 100 0 0 FTVF:DOUBLE
p 48 992 100 0 0 FTVG:DOUBLE
p 144 782 100 0 0 FTVH:STRING
p 16 448 100 0 1 INAM:gmSeqNullInit
p 16 320 100 0 1 PREC:2
p 16 384 100 0 1 SNAM:gmSeqCadDtaPos
p 320 1234 100 0 0 def(OUTA):0.0
p 320 1170 100 0 0 def(OUTB):0.0
p 288 1104 100 0 0 def(OUTC):0.0
p 336 1022 100 0 0 def(OUTD):0.0
p 336 958 100 0 0 def(OUTE):0.0
p 336 894 100 0 0 def(OUTF):0.0
p 336 830 100 0 0 def(OUTG):0.0
p 352 766 100 0 0 def(OUTH):0.0
p 16 430 100 0 0 name:$(top)$(I)
p 272 1216 75 768 -1 pproc(OUTA):NPP
p 272 1152 75 768 -1 pproc(OUTB):NPP
p 272 768 75 768 -1 pproc(OUTH):NPP
use inhier -440 1304 100 0 ICID
xform 0 -448 1344
use inhier -440 1400 100 0 DIR
xform 0 -448 1440
[comments]

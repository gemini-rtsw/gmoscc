[schematic2]
uniq 139
[tools]
[detail]
w 474 -245 100 0 fltWaitForIdle ecalcouts.ecalcouts#136.FLNK 160 -112 192 -112 192 -256 816 -256 eevents.fltCadStartEvent.SLNK
w -270 -101 100 0 n#137 hwin.hwin#106.in -352 0 -320 0 -320 -112 -160 -112 ecalcouts.ecalcouts#136.INPA
w 794 555 100 0 n#135 ecad8.fltPos.VALF 128 864 416 864 416 544 1232 544 1232 768 1328 768 eaos.lambdaOffset2.DOL
w 594 843 100 0 n#134 ecad8.fltPos.VALE 128 928 448 928 448 832 800 832 800 768 880 768 eaos.lambdaOffset1.DOL
w 1154 875 100 0 n#133 eaos.filtOffset2.FLNK 1568 1152 1632 1152 1632 864 736 864 736 736 880 736 eaos.lambdaOffset1.SLNK
w 1218 747 100 0 n#130 eaos.lambdaOffset1.FLNK 1136 768 1168 768 1168 736 1328 736 eaos.lambdaOffset2.SLNK
w 146 1099 100 0 n#129 ecad8.fltPos.OUTB 128 1088 224 1088 224 1152 256 1152 hwout.hwout#127.outp
w 196 1195 100 0 n#128 ecad8.fltPos.OUTA 128 1152 192 1152 192 1248 256 1248 hwout.hwout#126.outp
w 1810 411 100 0 n#125 elongouts.fltAssemblyMode.FLNK 1536 432 1664 432 1664 400 2016 400 elongouts.fltMarkDta.SLNK
w 1944 434 100 0 n#121 hwin.hwin#123.in 1920 432 2016 432 elongouts.fltMarkDta.DOL
w 1266 515 100 0 n#120 eaos.lambdaOffset2.FLNK 1584 768 1664 768 1664 512 928 512 928 112 1472 112 estringouts.fltSirUnknown1.SLNK
w 1202 1131 100 0 n#119 eaos.filtOffset1.FLNK 1120 1152 1152 1152 1152 1120 1312 1120 eaos.filtOffset2.SLNK
w 668 811 100 0 n#118 ecad8.fltPos.STLK 128 512 672 512 672 1120 864 1120 eaos.filtOffset1.SLNK
w 1898 123 100 0 n#117 estringouts.fltSirUnknown1.FLNK 1728 128 1824 128 1824 112 2032 112 estringouts.fltSirUnknown2.SLNK
w 786 403 100 0 n#102 ecad8.fltPos.PLNK 128 544 352 544 352 400 1280 400 elongouts.fltAssemblyMode.SLNK
w 634 995 100 0 n#101 ecad8.fltPos.VALD 128 992 1200 992 1200 1152 1312 1152 eaos.filtOffset2.DOL
w 354 1059 100 0 n#100 ecad8.fltPos.VALC 128 1056 640 1056 640 1152 864 1152 eaos.filtOffset1.DOL
w 1304 899 100 0 n#97 ecad8.fltPos.VALH 128 736 544 736 544 896 2112 896 2112 1152 2192 1152 estringouts.filtSad2.DOL
w 1080 931 100 0 n#96 ecad8.fltPos.VALG 128 800 512 800 512 928 1696 928 1696 1152 1776 1152 estringouts.filtSad1.DOL
w 744 -213 100 0 n#81 hwin.hwin#80.in 656 -176 720 -176 720 -224 816 -224 eevents.fltCadStartEvent.INP
w 1208 434 100 0 n#53 hwin.hwin#51.in 1184 432 1280 432 elongouts.fltAssemblyMode.DOL
w 1216 1379 100 0 n#46 ecad8.fltPos.VAL 128 1376 2352 1376 outhier.VAL.p
w 1590 1283 100 0 n#9 ecad8.fltPos.MESS 128 1344 864 1344 864 1280 2352 1280 outhier.MESS.p
w -370 1348 100 0 n#8 inhier.ICID.P -592 1280 -512 1280 -512 1344 -192 1344 ecad8.fltPos.ICID
w -410 1378 100 0 n#7 inhier.DIR.P -592 1376 -192 1376 ecad8.fltPos.DIR
s 1696 -48 120 0 Set SIRs 'unknown' on START
s 384 -64 150 0 Trigger SIR output when CAR goes IDLE
s 624 1488 100 0 As well as driving the CAD it also updates the specified SAD items.
s 624 1456 100 0 Note that event scanning is used between the CAD and the SAD to
s 624 1424 100 0 split any lock sets.
s -544 1200 100 0 Input attributes:
s -544 1152 100 0 A: Name of first filter
s -544 1104 100 0 B: Name of second filter
s 304 1280 100 0 Target Filter 1 Barcode ID
s 304 1184 100 0 Target Filter 2 Barcode ID
s 2128 1712 140 0 gmSeqFilter.sch
s 944 1312 100 0 Offset records now SCAN Passive rather than Event - SMB
[cell use]
use ecalcouts -160 -297 100 0 ecalcouts#136
xform 0 0 -176
p -88 -264 100 0 -1 CALC:A
p 672 254 100 0 0 DOPT:Use CALC
p 672 286 100 0 0 OOPT:Transition To Zero
p -72 -64 100 0 1 SCAN:I/O Intr
use eaos 1328 647 100 0 lambdaOffset2
xform 0 1456 736
p 1392 576 100 0 1 EGU:microns
p 1248 832 100 0 1 EVNT:$(event)
p 1392 608 100 0 1 OMSL:closed_loop
p 1392 544 100 0 1 PREC:2
p 1440 832 100 0 1 SCAN:Passive
use eaos 880 647 100 0 lambdaOffset1
xform 0 1008 736
p 944 592 100 0 1 EGU:microns
p 816 832 100 0 1 EVNT:$(event)
p 944 624 100 0 1 OMSL:closed_loop
p 944 560 100 0 1 PREC:2
p 1024 832 100 0 1 SCAN:Passive
p 848 768 75 1280 -1 pproc(DOL):NPP
use eaos 1312 1031 100 0 filtOffset2
xform 0 1440 1120
p 1376 960 100 0 1 EGU:microns
p 1232 1216 100 0 1 EVNT:$(event)
p 1376 992 100 0 1 OMSL:closed_loop
p 1376 928 100 0 1 PREC:2
p 1424 1216 100 0 1 SCAN:Passive
use eaos 864 1031 100 0 filtOffset1
xform 0 992 1120
p 928 976 100 0 1 EGU:microns
p 800 1216 100 0 1 EVNT:$(event)
p 928 1008 100 0 1 OMSL:closed_loop
p 928 944 100 0 1 PREC:2
p 1008 1216 100 0 1 SCAN:Passive
p 832 1152 75 1280 -1 pproc(DOL):NPP
use hwout 256 1111 100 0 hwout#127
xform 0 352 1152
p 352 1143 100 0 -1 val(outp):$(cc)fltAssembly.B
use hwout 256 1207 100 0 hwout#126
xform 0 352 1248
p 352 1239 100 0 -1 val(outp):$(cc)fltAssembly.A
use hwin 1752 392 100 0 hwin#123
xform 0 1824 432
p 1731 424 100 0 -1 val(in):$(CAD_MARK)
use hwin 464 -217 100 0 hwin#80
xform 0 560 -176
p 467 -184 100 0 -1 val(in):$(event)
use hwin 1016 392 100 0 hwin#51
xform 0 1088 432
p 995 424 100 0 -1 val(in):$(MODE_MOVE)
use hwin -544 -41 100 0 hwin#106
xform 0 -448 0
p -541 -8 100 0 -1 val(in):$(cc)fltC.VAL
use elongouts 2040 312 100 0 fltMarkDta
xform 0 2144 400
p 2064 494 100 0 -1 DESC:Mark dtaTrack CAD
p 1856 542 100 0 0 EGU:CAD directive
p 2080 272 100 0 1 OMSL:closed_loop
p 2352 366 100 0 -1 def(OUT):$(gm)dtaTrack.DIR
p 2240 304 100 1024 0 name:$(top)$(I)
p 2272 368 75 768 -1 pproc(OUT):PP
use elongouts 1304 312 100 0 fltAssemblyMode
xform 0 1408 400
p 1328 494 100 0 -1 DESC:Set assembly record mode MOVE
p 1120 542 100 0 0 EGU:Assembly mode
p 1344 272 100 0 1 OMSL:closed_loop
p 1616 366 100 0 -1 def(OUT):$(cc)fltAssembly.MODE
p 1504 304 100 1024 0 name:$(top)$(I)
p 1536 368 75 768 -1 pproc(OUT):NPP
use estringouts 2032 23 100 0 fltSirUnknown2
xform 0 2160 112
p 1812 298 100 0 0 DESC:Output of CAD
p 2000 -162 100 0 0 EGU:degs
p 2128 192 100 0 0 EVNT:0
p 2096 0 100 0 1 OMSL:closed_loop
p 2080 176 100 0 1 SCAN:Passive
p 2080 208 100 0 1 VAL:unknown
p 2240 48 100 0 -1 def(OUT):$(sad)filtName2
p 2144 16 100 1024 0 name:$(top)$(I)
p 2288 96 75 768 -1 pproc(OUT):PP
use estringouts 1472 23 100 0 fltSirUnknown1
xform 0 1600 112
p 1252 298 100 0 0 DESC:Output of CAD
p 1440 -162 100 0 0 EGU:degs
p 1568 192 100 0 0 EVNT:0
p 1536 0 100 0 1 OMSL:closed_loop
p 1552 128 100 0 1 PINI:YES
p 1520 176 100 0 1 SCAN:Passive
p 1520 208 100 0 1 VAL:unknown
p 1680 48 100 0 -1 def(OUT):$(sad)filtName1
p 1584 16 100 1024 0 name:$(top)$(I)
p 1728 96 75 768 -1 pproc(OUT):PP
use estringouts 2192 1031 100 0 filtSad2
xform 0 2320 1120
p 1972 1306 100 0 0 DESC:Output of CAD
p 2160 846 100 0 0 EGU:degs
p 2448 1216 100 0 1 EVNT:$(event)
p 2256 992 100 0 1 OMSL:closed_loop
p 2256 1216 100 0 1 SCAN:Event
p 2416 1056 100 0 -1 def(OUT):$(sad)filtName2
p 2304 1024 100 1024 0 name:$(top)$(I)
p 2448 1104 75 768 -1 pproc(OUT):PP
use estringouts 1776 1031 100 0 filtSad1
xform 0 1904 1120
p 1556 1306 100 0 0 DESC:Output of CAD
p 1744 846 100 0 0 EGU:degs
p 1872 1200 100 0 1 EVNT:$(event)
p 1840 992 100 0 1 OMSL:closed_loop
p 1680 1200 100 0 1 SCAN:Event
p 1984 1056 100 0 -1 def(OUT):$(sad)filtName1
p 1888 1024 100 1024 0 name:$(top)$(I)
p 2032 1104 75 768 -1 pproc(OUT):PP
use gmSeqFiltNames -528 1415 100 0 gmSeqFiltNames#104
xform 0 -408 1560
use eevents 816 -345 100 0 fltCadStartEvent
xform 0 960 -256
p 896 -384 100 0 1 EVNT:0
p 928 -352 100 1024 0 name:$(top)$(I)
use outhier 2344 1240 100 0 MESS
xform 0 2336 1280
use outhier 2344 1336 100 0 VAL
xform 0 2336 1376
use ecad8 -168 424 100 0 fltPos
xform 0 -32 928
p -240 1470 100 0 -1 DESC:Triggers a subsystem CAD on START only
p -80 1168 100 0 1 FTVA:STRING
p -80 1104 100 0 1 FTVB:STRING
p -80 1040 100 0 1 FTVC:DOUBLE
p -80 976 100 0 1 FTVD:DOUBLE
p -80 912 100 0 1 FTVE:DOUBLE
p -80 848 100 0 1 FTVF:DOUBLE
p -80 784 100 0 1 FTVG:STRING
p -80 720 100 0 1 FTVH:STRING
p -128 384 100 0 1 INAM:gmSeqCadInitFilter
p -96 768 100 0 0 PREC:4
p -128 320 100 0 1 SNAM:gmSeqCadFilter
p 192 1022 100 0 0 def(OUTC):0.0
p 192 958 100 0 0 def(OUTD):0.0
p 192 894 100 0 0 def(OUTE):0.0
p 192 830 100 0 0 def(OUTF):0.0
p 192 766 100 0 0 def(OUTG):0.0
p 208 702 100 0 0 def(OUTH):0.0
p -128 366 100 0 0 name:$(top)$(I)
p 128 1152 75 768 -1 pproc(OUTA):NPP
p 128 1088 75 768 -1 pproc(OUTB):NPP
p 128 1024 75 768 -1 pproc(OUTC):NPP
p 128 960 75 768 -1 pproc(OUTD):NPP
p 128 704 75 768 -1 pproc(OUTH):NPP
use inhier -584 1240 100 0 ICID
xform 0 -592 1280
use inhier -584 1336 100 0 DIR
xform 0 -592 1376
[comments]

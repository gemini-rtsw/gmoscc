[schematic2]
uniq 114
[tools]
[detail]
w 530 299 100 0 n#113 ecad8.atmPos.PLNK 256 288 864 288 864 320 992 320 elongouts.atmPosAssemblyMode.SLNK
w 712 51 100 0 n#96 ecad8.atmPos.STLK 256 256 480 256 480 48 992 48 eevents.atmPosCadStEvent.SLNK
w 920 91 100 0 n#81 hwin.hwin#80.in 832 128 896 128 896 80 992 80 eevents.atmPosCadStEvent.INP
w 920 354 100 0 n#53 hwin.hwin#51.in 896 352 992 352 elongouts.atmPosAssemblyMode.DOL
w 1240 1122 100 0 n#46 ecad8.atmPos.VAL 256 1120 2272 1120 outhier.VAL.p
w 1510 1028 100 0 n#9 ecad8.atmPos.MESS 256 1088 784 1088 784 1024 2272 1024 outhier.MESS.p
w -242 1092 100 0 n#8 inhier.ICID.P -464 1024 -384 1024 -384 1088 -64 1088 ecad8.atmPos.ICID
w -282 1122 100 0 n#7 inhier.DIR.P -464 1120 -64 1120 ecad8.atmPos.DIR
s 288 944 100 0 A: Entrance prism angle
s 288 880 100 0 B: Exit prism angle
s 2160 1696 140 0 gmSeqAtmPos.sch
s -336 976 100 0 Attributes are :
s -368 928 100 0 A: Entrance prism angle
s -368 864 100 0 B: Exit prism angle
[cell use]
use elongouts 1016 232 100 0 atmPosAssemblyMode
xform 0 1120 320
p 1040 414 100 0 -1 DESC:Set assembly record mode MOVE
p 832 462 100 0 0 EGU:Assembly mode
p 1056 192 100 0 1 OMSL:closed_loop
p 1328 286 100 0 -1 def(OUT):$(cc)atmAssembly.MODE
p 1216 224 100 1024 0 name:$(top)$(I)
p 1248 288 75 768 -1 pproc(OUT):NPP
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
p 2220 -496 120 256 -1 Title:GMOS IS - atmPos command
use hwin 640 87 100 0 hwin#80
xform 0 736 128
p 643 120 100 0 -1 val(in):$(event)
use hwin 728 312 100 0 hwin#51
xform 0 800 352
p 707 344 100 0 -1 val(in):$(MODE_MOVE)
use eevents 992 -41 100 0 atmPosCadStEvent
xform 0 1136 48
p 1072 -80 100 0 0 EVNT:0
p 1104 -48 100 1024 0 name:$(top)$(I)
use outhier 2264 984 100 0 MESS
xform 0 2256 1024
use outhier 2264 1080 100 0 VAL
xform 0 2256 1120
use ecad8 -40 168 100 0 atmPos
xform 0 96 672
p -112 1214 100 0 -1 DESC:Triggers a subsystem CAD on START only
p 48 920 100 0 1 FTVA:DOUBLE
p 48 864 100 0 1 FTVB:DOUBLE
p 32 800 100 0 0 FTVC:DOUBLE
p 32 768 100 0 0 FTVD:DOUBLE
p 32 736 100 0 0 FTVE:DOUBLE
p 32 704 100 0 0 FTVF:DOUBLE
p 32 672 100 0 0 FTVG:DOUBLE
p 128 462 100 0 0 FTVH:STRING
p 0 128 100 0 1 INAM:gmSeqNullInit
p 0 16 100 0 1 PREC:2
p 0 64 100 0 1 SNAM:gmSeqCadAtmPos
p 288 928 100 0 -1 def(OUTA):$(cc)atmAssembly.A
p 288 864 100 0 -1 def(OUTB):$(cc)atmAssembly.B
p 320 766 100 0 0 def(OUTC):0.0
p 320 702 100 0 0 def(OUTD):0.0
p 320 638 100 0 0 def(OUTE):0.0
p 320 574 100 0 0 def(OUTF):0.0
p 320 510 100 0 0 def(OUTG):0.0
p 336 446 100 0 0 def(OUTH):0.0
p 0 110 100 0 0 name:$(top)$(I)
p 256 896 75 768 -1 pproc(OUTA):NPP
p 256 832 75 768 -1 pproc(OUTB):NPP
p 256 448 75 768 -1 pproc(OUTH):NPP
use inhier -456 984 100 0 ICID
xform 0 -464 1024
use inhier -456 1080 100 0 DIR
xform 0 -464 1120
[comments]

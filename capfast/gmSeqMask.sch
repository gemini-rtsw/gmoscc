[schematic2]
uniq 106
[tools]
[detail]
w 1700 475 100 2 n#105 hwin.hwin#104.in 1696 480 1696 480 elongouts.mskMarkData.DOL
w 1458 459 100 0 n#102 elongouts.mskAssemblyMode.FLNK 1248 480 1280 480 1280 448 1696 448 elongouts.mskMarkData.SLNK
w 260 763 100 2 c#100 rboat.c#100.p 256 768 256 768 ecad8.mskPos.OUTC
w 736 459 100 0 n#97 ecad8.mskPos.PLNK 256 288 528 288 528 448 992 448 elongouts.mskAssemblyMode.SLNK
w 260 827 100 2 c#89 rboat.c#89.p 256 832 256 832 ecad8.mskPos.OUTB
w 260 891 100 2 c#88 rboat.c#88.p 256 896 256 896 ecad8.mskPos.OUTA
w 584 267 100 0 n#82 ecad8.mskPos.STLK 256 256 960 256 eevents.maskCadStartEvent.SLNK
w 888 299 100 0 n#81 hwin.hwin#80.in 800 336 864 336 864 288 960 288 eevents.maskCadStartEvent.INP
w 920 482 100 0 n#53 hwin.hwin#51.in 896 480 992 480 elongouts.mskAssemblyMode.DOL
w 1240 1122 100 0 n#46 ecad8.mskPos.VAL 256 1120 2272 1120 outhier.VAL.p
w 1510 1028 100 0 n#9 ecad8.mskPos.MESS 256 1088 784 1088 784 1024 2272 1024 outhier.MESS.p
w -242 1092 100 0 n#8 inhier.ICID.P -464 1024 -384 1024 -384 1088 -64 1088 ecad8.mskPos.ICID
w -282 1122 100 0 n#7 inhier.DIR.P -464 1120 -64 1120 ecad8.mskPos.DIR
s -480 944 100 0 Input attributes:
s -480 896 100 0 A: Mask or IFU ID
s -480 848 100 0 B: Mask/IFU location
s 304 928 100 0 Target Mask Barcode ID
s 304 864 100 0 Target Mask location
s 2144 1696 140 0 gmSeqMask.sch
s -448 816 100 0 [IN-BEAM | OUT-OF-BEAM]
[cell use]
use hwin 1528 440 100 0 hwin#104
xform 0 1600 480
p 1507 472 100 0 -1 val(in):$(CAD_MARK)
use elongouts 1696 359 100 0 mskMarkData
xform 0 1824 448
p 1581 793 100 0 0 DESC:Mark dtaTrack CAD
p 1536 590 100 0 0 EGU:CAD directive
p 1536 366 100 0 0 OMSL:closed_loop
p 1536 92 100 0 -1 def(OUT):$(gm)dtaTrack.DIR
p 1952 416 75 768 -1 pproc(OUT):PP
use rboat 256 855 100 0 c#88
xform 0 336 896
p 256 768 100 0 0 revision:2.2
use rboat 256 791 100 0 c#89
xform 0 336 832
use rboat 256 727 100 0 c#100
xform 0 336 768
use gmSeqMaskNames -416 1335 100 0 gmSeqMaskNames#98
xform 0 -296 1480
use elongouts 1016 360 100 0 mskAssemblyMode
xform 0 1120 448
p 1040 542 100 0 -1 DESC:Set assembly record mode MOVE
p 832 590 100 0 0 EGU:Assembly mode
p 832 366 100 0 0 OMSL:closed_loop
p 1328 414 100 0 -1 def(OUT):$(cc)mskAssembly.MODE
p 1216 352 100 1024 0 name:$(top)$(I)
p 1248 416 75 768 -1 pproc(OUT):NPP
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
use hwin 608 295 100 0 hwin#80
xform 0 704 336
p 611 328 100 0 -1 val(in):$(event)
use hwin 728 440 100 0 hwin#51
xform 0 800 480
p 707 472 100 0 -1 val(in):$(MODE_MOVE)
use eevents 960 167 100 0 maskCadStartEvent
xform 0 1104 256
p 1072 160 100 1024 0 name:$(top)$(I)
use outhier 2264 984 100 0 MESS
xform 0 2256 1024
use outhier 2264 1080 100 0 VAL
xform 0 2256 1120
use ecad8 -40 168 100 0 mskPos
xform 0 96 672
p -112 1214 100 0 -1 DESC:Triggers a subsystem CAD on START only
p 48 888 100 0 1 FTVA:LONG
p 48 832 100 0 1 FTVB:LONG
p 48 784 100 0 0 FTVC:LONG
p 48 736 100 0 0 FTVD:LONG
p 32 736 100 0 0 FTVE:DOUBLE
p 32 704 100 0 0 FTVF:DOUBLE
p 128 462 100 0 0 FTVH:STRING
p 0 128 100 0 1 INAM:gmSeqNullInit
p 32 512 100 0 0 PREC:4
p 0 64 100 0 1 SNAM:gmSeqCadMask
p -64 32 100 0 0 def(INPB):0.0
p -64 0 100 0 0 def(INPC):0.0
p 352 896 100 0 -1 def(OUTA):$(cc)mskAssembly.A
p 352 832 100 0 -1 def(OUTB):$(cc)mskAssembly.B
p 320 766 100 0 -1 def(OUTC):$(gm)offsetSum.D
p 320 702 100 0 0 def(OUTD):0.0
p 320 638 100 0 0 def(OUTE):0.0
p 320 574 100 0 0 def(OUTF):0.0
p 320 510 100 0 0 def(OUTG):0.0
p 336 446 100 0 0 def(OUTH):0.0
p 0 110 100 0 0 name:$(top)$(I)
p 256 896 75 768 -1 pproc(OUTA):NPP
p 256 832 75 768 -1 pproc(OUTB):NPP
p 256 768 75 768 -1 pproc(OUTC):PP
p 256 448 75 768 -1 pproc(OUTH):NPP
use inhier -456 984 100 0 ICID
xform 0 -464 1024
use inhier -456 1080 100 0 DIR
xform 0 -464 1120
[comments]

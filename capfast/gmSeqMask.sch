[schematic2]
uniq 111
[tools]
[detail]
w 680 699 100 0 maskName ecad8.mskPos.VALD 256 736 336 736 336 688 1120 688 estringouts.mskNameToSad.DOL
w 1458 459 100 0 n#106 elongouts.mskAssemblyMode.VAL 1248 448 1728 448 elongouts.mskMarkDta.SLNK
w 1656 482 100 0 n#103 hwin.hwin#104.in 1632 480 1728 480 elongouts.mskMarkDta.DOL
w 1218 811 100 0 n#102 eaos.mskOffset.FLNK 992 800 1504 800 eevents.maskCadStartEvent.SLNK
w 452 507 100 0 n#101 ecad8.mskPos.STLK 256 256 448 256 448 768 736 768 eaos.mskOffset.SLNK
w 466 811 100 0 n#100 ecad8.mskPos.VALC 256 800 736 800 eaos.mskOffset.DOL
w 736 459 100 0 n#97 ecad8.mskPos.PLNK 256 288 528 288 528 448 992 448 elongouts.mskAssemblyMode.SLNK
w 260 827 100 2 c#89 rboat.c#89.p 256 832 256 832 ecad8.mskPos.OUTB
w 260 891 100 2 c#88 rboat.c#88.p 256 896 256 896 ecad8.mskPos.OUTA
w 1432 843 100 0 n#81 hwin.hwin#80.in 1344 880 1408 880 1408 832 1504 832 eevents.maskCadStartEvent.INP
w 920 482 100 0 n#53 hwin.hwin#51.in 896 480 992 480 elongouts.mskAssemblyMode.DOL
w 1240 1122 100 0 n#46 ecad8.mskPos.VAL 256 1120 2272 1120 outhier.VAL.p
w 1510 1028 100 0 n#9 ecad8.mskPos.MESS 256 1088 784 1088 784 1024 2272 1024 outhier.MESS.p
w -242 1092 100 0 n#8 inhier.ICID.P -464 1024 -384 1024 -384 1088 -64 1088 ecad8.mskPos.ICID
w -282 1122 100 0 n#7 inhier.DIR.P -464 1120 -64 1120 ecad8.mskPos.DIR
s 1504 896 100 0 Left here "just in case"
s 1504 944 100 0 This event is probably not used - SMB
s -480 944 100 0 Input attributes:
s -480 896 100 0 A: Mask or IFU ID
s -480 848 100 0 B: Mask/IFU location
s 304 928 100 0 Target Mask Barcode ID
s 304 864 100 0 Target Mask location
s 2144 1696 140 0 gmSeqMask.sch
s -448 816 100 0 [IN-BEAM | OUT-OF-BEAM]
[cell use]
use estringouts 1120 583 100 0 mskNameToSad
xform 0 1248 656
p 1200 656 100 0 0 DESC:mask name output of cad
p 1056 462 100 0 0 OMSL:closed_loop
p 1056 590 100 0 0 SCAN:Event
p 1088 90 100 0 0 def(OUT):$(sad)mskName
p 1376 640 75 768 -1 pproc(OUT):PP
use elongouts 1016 360 100 0 mskAssemblyMode
xform 0 1120 448
p 1040 542 100 0 -1 DESC:Set assembly record mode MOVE
p 832 590 100 0 0 EGU:Assembly mode
p 832 366 100 0 0 OMSL:closed_loop
p 1328 414 100 0 -1 def(OUT):$(cc)mskAssembly.MODE
p 1216 352 100 1024 0 name:$(top)$(I)
p 1248 416 75 768 -1 pproc(OUT):NPP
use elongouts 1752 360 100 0 mskMarkDta
xform 0 1856 448
p 1776 542 100 0 -1 DESC:Mark dtaTrack CAD
p 1568 590 100 0 0 EGU:CAD directive
p 1792 320 100 0 1 OMSL:closed_loop
p 2064 414 100 0 -1 def(OUT):$(gm)dtaTrack.DIR
p 1952 352 100 1024 0 name:$(top)$(I)
p 1984 416 75 768 -1 pproc(OUT):PP
use hwin 1152 839 100 0 hwin#80
xform 0 1248 880
p 1155 872 100 0 -1 val(in):$(event)
use hwin 728 440 100 0 hwin#51
xform 0 800 480
p 707 472 100 0 -1 val(in):$(MODE_MOVE)
use hwin 1464 440 100 0 hwin#104
xform 0 1536 480
p 1443 472 100 0 -1 val(in):$(CAD_MARK)
use eaos 736 679 100 0 mskOffset
xform 0 864 768
p 800 624 100 0 1 EGU:microns
p 672 864 100 0 1 EVNT:$(event)
p 800 656 100 0 1 OMSL:closed_loop
p 800 592 100 0 1 PREC:2
p 880 864 100 0 1 SCAN:Passive
p 704 800 75 1280 -1 pproc(DOL):NPP
use gmSeqMaskNames -416 1335 100 0 gmSeqMaskNames#98
xform 0 -296 1480
use rboat 256 855 100 0 c#88
xform 0 336 896
use rboat 256 791 100 0 c#89
xform 0 336 832
use oslBorderC -720 -745 100 0 oslBorderC#87
xform 0 960 560
p 2220 -496 120 256 -1 Title:GMOS IS - mask commands
use eevents 1504 711 100 0 maskCadStartEvent
xform 0 1648 800
p 1616 704 100 1024 0 name:$(top)$(I)
use outhier 2264 984 100 0 MESS
xform 0 2256 1024
use outhier 2264 1080 100 0 VAL
xform 0 2256 1120
use ecad8 -40 168 100 0 mskPos
xform 0 96 672
p -112 1214 100 0 -1 DESC:Triggers a subsystem CAD on START only
p 48 888 100 0 1 FTVA:LONG
p 48 832 100 0 1 FTVB:LONG
p 48 784 100 0 1 FTVC:DOUBLE
p 48 736 100 0 1 FTVD:STRING
p 32 736 100 0 0 FTVE:DOUBLE
p 32 704 100 0 0 FTVF:DOUBLE
p 128 462 100 0 0 FTVH:STRING
p 0 128 100 0 1 INAM:gmSeqNullInit
p 32 512 100 0 0 PREC:4
p 0 64 100 0 1 SNAM:gmSeqCadMask
p 352 896 100 0 -1 def(OUTA):$(cc)mskAssembly.A
p 352 832 100 0 -1 def(OUTB):$(cc)mskAssembly.B
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

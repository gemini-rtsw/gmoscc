[schematic2]
uniq 129
[tools]
[detail]
w 498 267 100 0 n#128 ecad4.update.PLNK 224 256 832 256 832 544 1024 544 elongouts.updatedcCadMark.SLNK
w 1426 -245 100 0 n#126 elongouts.updateOerrIn.OUT 1376 -256 1536 -256 1536 -448 1648 -448 ecars.updateC.IERR
w 2010 -437 100 0 c#122 ecars.updateC.OERR 1968 -448 2112 -448 2112 -496 2240 -496 outhier.OERR.p
w 2074 -405 100 0 c#121 ecars.updateC.OMSS 1968 -416 2240 -416 outhier.OMSS.p
w 2074 -341 100 0 c#120 ecars.updateC.VAL 1968 -352 2240 -352 outhier.CAR.p
w 1242 -293 100 0 n#119 elongouts.updateOerrIn.FLNK 1376 -192 1456 -192 1456 -304 1088 -304 1088 -592 1104 -592 estringouts.updateMessIn.SLNK
w 1124 -197 100 2 c#113 elongouts.updateOerrIn.DOL 1120 -192 1120 -192 lboat.c#113.p
w 986 -213 100 0 n#116 ewait.ewait#104.FLNK 880 -768 912 -768 912 -224 1120 -224 elongouts.updateOerrIn.SLNK
w 1442 -565 100 0 n#114 estringouts.updateMessIn.FLNK 1360 -576 1584 -576 1584 -544 1648 -544 ecars.updateC.SLNK
w 1108 -565 100 2 c#110 lboat.c#110.p 1104 -560 1104 -560 estringouts.updateMessIn.DOL
w 180 -389 100 2 c#109 lboat.c#109.p 176 -384 176 -384 ewait.ewait#104.INAN
w 1490 -405 100 0 n#108 estringouts.updateMessIn.OUT 1360 -608 1392 -608 1392 -416 1648 -416 ecars.updateC.IMSS
w 1074 -469 100 0 n#106 ewait.ewait#104.VAL 880 -480 1328 -480 1328 -352 1648 -352 ecars.updateC.IVAL
w 1284 507 100 2 c#101 rboat.c#101.p 1280 512 1280 512 elongouts.updatedcCadMark.OUT
w 1472 587 100 0 c#91 elongouts.updatedcCadMark.FLNK 1280 576 1712 576 outhier.FLNK.p
w 936 635 100 0 n#90 hwin.hwin#88.in 928 624 992 624 992 576 1024 576 elongouts.updatedcCadMark.DOL
w 288 802 100 0 n#30 ecad4.update.MESS 224 800 400 800 400 928 528 928 outhier.MESS.p
w 370 944 100 0 n#29 ecad4.update.VAL 224 832 368 832 368 1008 528 1008 outhier.VAL.p
w -296 770 100 0 n#28 inhier.ICID.P -336 768 -208 768 -208 800 -96 800 ecad4.update.ICID
w -224 834 100 0 n#27 inhier.DIR.P -336 848 -304 848 -304 832 -96 832 ecad4.update.DIR
s 2320 1264 150 0 gmSeqUpdate
s 816 1024 180 0 Update Sequence Command
s 720 928 150 0 MARK command to Components Controller only
[cell use]
use outhier 520 968 100 0 VAL
xform 0 512 1008
use outhier 520 888 100 0 MESS
xform 0 512 928
use outhier 1680 535 100 0 FLNK
xform 0 1696 576
use outhier 2272 -352 100 0 CAR
xform 0 2224 -352
use outhier 2272 -432 100 0 OMSS
xform 0 2224 -416
use outhier 2272 -512 100 0 OERR
xform 0 2224 -496
use elongouts 1168 -144 100 0 updateOerrIn
xform 0 1248 -224
p 960 -82 100 0 0 EGU:CAR value
p 1184 -336 100 0 1 OMSL:closed_loop
p 896 -160 100 0 -1 def(DOL):$(cc)updateC.OERR
p 1376 -256 75 768 -1 pproc(OUT):PP
use elongouts 1024 455 100 0 updatedcCadMark
xform 0 1152 544
p 864 686 100 0 0 EGU:CAD directive
p 1104 400 100 0 1 OMSL:closed_loop
p 1328 464 100 0 -1 def(OUT):$(cc)update.DIR
p 1280 512 75 768 -1 pproc(OUT):PP
use lboat 960 -233 100 0 c#113
xform 0 1040 -192
use lboat 944 -601 100 0 c#110
xform 0 1024 -560
use lboat 16 -425 100 0 c#109
xform 0 96 -384
use estringouts 1104 -665 100 0 updateMessIn
xform 0 1232 -592
p 1184 -704 100 0 1 OMSL:closed_loop
p 832 -592 100 0 -1 def(DOL):$(cc)updateC.OMSS
p 1360 -608 75 768 -1 pproc(OUT):PP
use ewait 176 -857 100 0 ewait#104
xform 0 528 -528
p 499 -280 100 0 -1 CALC:A
p 560 -480 100 0 1 INAP:Yes
p 304 -386 100 0 1 SCAN:I/O Intr
p -64 -416 100 0 -1 def(INAN):$(cc)updateC.VAL 
use rboat 1280 471 100 0 c#101
xform 0 1360 512
use ecars 1648 -633 100 0 updateC
xform 0 1808 -464
use hwin 736 583 100 0 hwin#88
xform 0 832 624
p 739 616 100 0 -1 val(in):$(CAD_MARK)
use oslBorderC -608 -1177 100 0 oslBorderC#48
xform 0 1072 128
p 2332 -928 120 256 -1 Title:GMOS IS - update command
use ecad4 -96 135 100 0 update
xform 0 64 512
p -80 352 100 0 1 SNAM:gmSeqCadSetup
p 128 -96 100 0 0 def(OUTA):0.0
p 128 -128 100 0 0 def(OUTB):0.0
p 128 -160 100 0 0 def(OUTC):0.0
p 128 -192 100 0 0 def(OUTD):0.0
use inhier -328 808 100 0 DIR
xform 0 -336 848
use inhier -328 728 100 0 ICID
xform 0 -336 768
[comments]

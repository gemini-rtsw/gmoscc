[schematic2]
uniq 90
[tools]
[detail]
w -40 75 100 0 n#89 efanouts.debugFan.LNK5 560 176 608 176 608 64 -640 64 -640 320 -512 320 elongouts.debugResetB.SLNK
w -536 363 100 0 n#87 hwin.hwin#86.in -512 352 -512 352 elongouts.debugResetB.DOL
w 1032 971 100 0 n#84 elongouts.debugCarBusy.DOL 1056 960 1056 960 hwin.hwin#22.in
w 1176 1323 100 0 n#82 ecad2.debug.PLNK 160 256 256 256 256 416 656 416 656 1312 1744 1312 estringouts.ccDebugOut.SLNK
w -40 843 100 0 n#81 estringouts.debugString.OUT 160 960 208 960 208 832 -240 832 -240 512 -160 512 ecad2.debug.A
w -160 1019 100 0 n#80 embbis.debugMenu.VAL -208 1024 -176 1024 -176 1008 -96 1008 estringouts.debugString.DOL
w -140 1011 100 0 n#79 embbis.debugMenu.FLNK -208 1056 -144 1056 -144 976 -96 976 estringouts.debugString.SLNK
w 1840 1211 100 0 n#76 estringouts.ccDebugOut.FLNK 2000 1328 2096 1328 2096 1200 1632 1200 1632 1104 1744 1104 estringouts.dcDebugOut.SLNK
w 1160 1139 100 0 n#74 ecad2.debug.VALB 160 448 624 448 624 1136 1744 1136 estringouts.dcDebugOut.DOL
w 1144 1355 100 0 n#73 ecad2.debug.VALA 160 512 592 512 592 1344 1744 1344 estringouts.ccDebugOut.DOL
w 480 27 100 0 n#64 efanouts.debugFan.LNK4 560 208 640 208 640 16 368 16 368 -128 624 -128 eevents.debugStartEvent.SLNK
w 628 -101 100 2 n#63 hwin.hwin#62.in 624 -96 624 -96 eevents.debugStartEvent.INP
w 2240 219 100 0 CFLK ecars.debugC.FLNK 2176 208 2352 208 outhier.CFLK.p
w 2280 331 100 0 c#56 ecars.debugC.OERR 2176 336 2256 336 2256 320 2352 320 outhier.OERR.p
w 2200 379 100 0 OMSS ecars.debugC.OMSS 2176 368 2272 368 2272 384 2352 384 outhier.OMSS.p
w 2024 539 100 0 CAR junction 1824 432 1824 528 2272 528 2272 448 2352 448 outhier.CAR.p
w 1496 459 100 0 CAR eseqs.debugCarIdle.LNK1 1376 448 1664 448 1664 432 1856 432 ecars.debugC.IVAL
w 1408 907 100 0 CAR elongouts.debugCarBusy.OUT 1312 896 1552 896 1552 448 junction
w 872 131 100 0 n#49 efanouts.debugFan.LNK3 560 240 736 240 736 128 1056 128 eseqs.debugCarIdle.SLNK
w 1208 739 100 0 n#48 efanouts.debugFan.LNK2 560 272 736 272 736 736 1728 736 outhier.FLNK.p
w 700 611 100 0 n#46 efanouts.debugFan.LNK1 560 304 704 304 704 928 1056 928 elongouts.debugCarBusy.SLNK
w 216 226 100 0 n#45 ecad2.debug.STLK 160 224 320 224 efanouts.debugFan.SLNK
w 1032 450 100 0 n#41 hwin.hwin#24.in 1056 448 1056 448 eseqs.debugCarIdle.DOL1
w 224 674 100 0 n#30 ecad2.debug.MESS 160 672 336 672 336 800 464 800 outhier.MESS.p
w 306 816 100 0 n#29 ecad2.debug.VAL 160 704 304 704 304 880 464 880 outhier.VAL.p
w -360 642 100 0 n#28 inhier.ICID.P -400 640 -272 640 -272 672 -160 672 ecad2.debug.ICID
w -288 706 100 0 n#27 inhier.DIR.P -400 720 -368 720 -368 704 -160 704 ecad2.debug.DIR
s -512 96 100 0 marked after every operation
s -512 128 100 0 because CAD ended up
s -512 160 100 0 DISCONNECTED from B
s 2112 1632 150 0 gmSeqDebug.sch
s 400 1504 300 0 GMOS Debug command
s 832 480 100 0 IDLE
s 880 1008 100 0 BUSY
s 272 528 100 0 Send Debug level to both
s 240 480 100 0 CC and DC Debug CAD field A
s -592 464 100 0 Reset isOnly flag after
s -592 432 100 0 each operation
[cell use]
use hwin 888 408 100 0 hwin#24
xform 0 960 448
p 878 376 100 0 0 typ(in):val
p 867 440 100 0 -1 val(in):$(CAR_IDLE)
use hwin 888 920 100 0 hwin#22
xform 0 960 960
p 867 952 100 0 -1 val(in):$(CAR_BUSY)
use hwin 432 -137 100 0 hwin#62
xform 0 528 -96
p 435 -104 100 0 -1 val(in):$(event)
use hwin -704 311 100 0 hwin#86
xform 0 -608 352
p -701 344 100 0 -1 val(in):0
use elongouts 1056 839 100 0 debugCarBusy
xform 0 1184 928
p 941 1273 100 0 0 DESC:Writes BUSY to CAR record
p 896 1070 100 0 0 EGU:CAR value
p 1120 816 100 0 1 OMSL:closed_loop
p 1312 896 75 768 -1 pproc(OUT):PP
use elongouts -512 231 100 0 debugResetB
xform 0 -384 320
p -627 665 100 0 0 DESC:Writes BUSY to CAR record
p -672 462 100 0 0 EGU:CAR value
p -448 208 100 0 1 OMSL:closed_loop
use estringouts -96 903 100 0 debugString
xform 0 32 976
p -16 864 100 0 1 OMSL:closed_loop
use estringouts 1744 1239 100 0 ccDebugOut
xform 0 1872 1312
p 1808 1216 100 0 1 OMSL:closed_loop
p 2048 1296 100 0 -1 def(OUT):$(cc)debug.A
use estringouts 1744 1031 100 0 dcDebugOut
xform 0 1872 1104
p 1808 992 100 0 1 OMSL:closed_loop
p 2016 1072 100 0 -1 def(OUT):$(dc)debug.A
use embbis -464 967 100 0 debugMenu
xform 0 -336 1040
p -432 816 100 0 1 FRST:MAX
p -432 912 100 0 1 ONST:NONE
p -432 848 100 0 1 THST:FULL
p -432 880 100 0 1 TWST:MIN
p -432 944 100 0 1 ZRST:QUIET
use eevents 624 -217 100 0 debugStartEvent
xform 0 768 -128
p 688 -256 100 0 1 EVNT:0
use outhier 2320 167 100 0 CFLK
xform 0 2336 208
use outhier 2320 279 100 0 OERR
xform 0 2336 320
use outhier 2320 343 100 0 OMSS
xform 0 2336 384
use outhier 2320 407 100 0 CAR
xform 0 2336 448
use outhier 1720 696 100 0 FLNK
xform 0 1712 736
use outhier 456 840 100 0 VAL
xform 0 448 880
use outhier 456 760 100 0 MESS
xform 0 448 800
use ecars 1856 151 100 0 debugC
xform 0 2016 320
use oslBorderC -816 -809 100 0 oslBorderC#50
xform 0 864 496
p 2124 -560 120 256 -1 Title:GMOS IS - debug command
use efanouts 344 88 100 0 debugFan
xform 0 440 240
p 352 352 100 0 1 SELM:All
use inhier -392 680 100 0 DIR
xform 0 -400 720
use inhier -392 600 100 0 ICID
xform 0 -400 640
use eseqs 1080 40 100 0 debugCarIdle
xform 0 1216 288
p 1136 334 100 0 1 DLY1:0.5
p 1136 334 100 0 0 DLY2:0.0
p 1456 480 100 0 0 def(LNK1):0.0
p 1488 430 100 0 0 def(LNK2):0.0
p 1056 480 75 1280 -1 palrm(SELL):NMS
p 1024 448 75 1280 -1 pproc(DOL1):NPP
p 1024 416 75 1280 -1 pproc(DOL2):NPP
p 1376 458 75 0 -1 pproc(LNK1):PP
p 1376 426 75 0 -1 pproc(LNK2):PP
use ecad2 -136 136 100 0 debug
xform 0 0 448
p 0 512 100 0 -1 FTVA:STRING
p 0 480 100 0 -1 FTVB:STRING
p -80 432 100 0 -1 SNAM:gmSeqCadDebug
p -256 510 100 0 0 def(INPA):0.0
p 192 480 100 0 0 def(OUTA):0.0
p 192 416 100 0 0 def(OUTB):0.0
p 160 480 75 768 -1 pproc(OUTA):NPP
p -96 -184 100 0 0 typ(INPA):path
[comments]

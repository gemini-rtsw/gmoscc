[schematic2]
uniq 63
[tools]
[detail]
w 1032 971 100 0 n#62 elongouts.Busy.DOL 1056 960 1056 960 hwin.hwin#22.in
w 2240 219 100 0 CFLK ecars.ecars#51.FLNK 2176 208 2352 208 outhier.CFLK.p
w 2280 331 100 0 c#56 ecars.ecars#51.OERR 2176 336 2256 336 2256 320 2352 320 outhier.OERR.p
w 2200 379 100 0 OMSS ecars.ecars#51.OMSS 2176 368 2272 368 2272 384 2352 384 outhier.OMSS.p
w 1408 907 100 0 CAR elongouts.Busy.OUT 1312 896 1552 896 1552 448 junction
w 1496 459 100 0 CAR eseqs.CarVals.LNK1 1376 448 1664 448 1664 432 1856 432 ecars.ecars#51.IVAL
w 2024 539 100 0 CAR junction 1824 432 1824 528 2272 528 2272 448 2352 448 outhier.CAR.p
w 856 130 100 0 n#49 efanouts.fan.LNK3 560 240 704 240 704 128 1056 128 eseqs.CarVals.SLNK
w 1192 738 100 0 n#48 efanouts.fan.LNK2 560 272 704 272 704 736 1728 736 outhier.FLNK.p
w 674 640 100 0 n#46 efanouts.fan.LNK1 560 304 672 304 672 928 1056 928 elongouts.Busy.SLNK
w 216 226 100 0 n#45 ecad2.endObserve.STLK 160 224 320 224 efanouts.fan.SLNK
w 1032 450 100 0 n#41 hwin.hwin#24.in 1056 448 1056 448 eseqs.CarVals.DOL1
w 224 674 100 0 n#30 ecad2.endObserve.MESS 160 672 336 672 336 800 464 800 outhier.MESS.p
w 306 816 100 0 n#29 ecad2.endObserve.VAL 160 704 304 704 304 880 464 880 outhier.VAL.p
w -360 642 100 0 n#28 inhier.ICID.P -400 640 -272 640 -272 672 -160 672 ecad2.endObserve.ICID
w -288 706 100 0 n#27 inhier.DIR.P -400 720 -368 720 -368 704 -160 704 ecad2.endObserve.DIR
s 528 1232 150 0 - toggles CAR record only
s 528 1280 150 0 Generic CAD/CAR for trivial sequence command
s 880 1008 100 0 BUSY
s 832 480 100 0 IDLE
s 640 1376 180 0 EndObserve Sequence Command
s 2016 1632 160 0 gmSeqEndObserve
[cell use]
use elongouts 1056 839 100 0 Busy
xform 0 1184 928
p 941 1273 100 0 0 DESC:Writes BUSY to CAR record
p 896 1070 100 0 0 EGU:CAR value
p 1120 816 100 0 1 OMSL:closed_loop
p 1216 1008 100 1024 1 name:$(gm)$(seqcommand)$(I)
p 1312 896 75 768 -1 pproc(OUT):PP
use outhier 456 760 100 0 MESS
xform 0 448 800
use outhier 456 840 100 0 VAL
xform 0 448 880
use outhier 1720 696 100 0 FLNK
xform 0 1712 736
use outhier 2320 407 100 0 CAR
xform 0 2336 448
use outhier 2320 343 100 0 OMSS
xform 0 2336 384
use outhier 2320 279 100 0 OERR
xform 0 2336 320
use outhier 2320 167 100 0 CFLK
xform 0 2336 208
use ecars 1856 151 100 0 ecars#51
xform 0 2016 320
p 2016 144 100 1024 1 name:$(gm)$(seqcommand)C
use oslBorderC -816 -809 100 0 oslBorderC#50
xform 0 864 496
p 2124 -560 120 256 -1 Title:GMOS IS - endObserve command
use efanouts 344 88 100 0 fan
xform 0 440 240
p 336 352 100 0 1 SELM:All
use inhier -392 600 100 0 ICID
xform 0 -400 640
use inhier -392 680 100 0 DIR
xform 0 -400 720
use hwin 888 920 100 0 hwin#22
xform 0 960 960
p 867 952 100 0 -1 val(in):$(CAR_BUSY)
use hwin 888 408 100 0 hwin#24
xform 0 960 448
p 878 376 100 0 0 typ(in):val
p 867 440 100 0 -1 val(in):$(CAR_IDLE)
use eseqs 1080 40 100 0 CarVals
xform 0 1216 288
p 1136 334 100 0 1 DLY1:$(delay)
p 1136 334 100 0 0 DLY2:0.0
p 1456 480 100 0 0 def(LNK1):0.0
p 1488 430 100 0 0 def(LNK2):0.0
p 1216 -16 100 1024 1 name:$(gm)$(seqcommand)$(I)
p 1056 480 75 1280 -1 palrm(SELL):NMS
p 1024 448 75 1280 -1 pproc(DOL1):NPP
p 1024 416 75 1280 -1 pproc(DOL2):NPP
p 1376 458 75 0 -1 pproc(LNK1):PP
p 1376 426 75 0 -1 pproc(LNK2):PP
use ecad2 -136 136 100 0 endObserve
xform 0 0 448
p -96 936 100 0 0 FTVA:STRING
p -144 80 100 0 1 SNAM:gmSeqCadEndObserve
p -256 510 100 0 0 def(INPA):0.0
p 240 494 100 0 0 def(OUTA):0.0
p -144 78 100 0 0 name:$(gm)$(I)
p 160 480 75 768 -1 pproc(OUTA):NPP
p -96 -184 100 0 0 typ(INPA):path
[comments]

[schematic2]
uniq 95
[tools]
[detail]
w 208 75 100 0 n#94 hwin.hwin#93.in 128 64 336 64 ecad4.ecad4#44.INPD
w 1904 267 100 0 c#91 elongouts.dcCadMark.FLNK 1712 256 2144 256 outhier.FLNK.p
w 1368 315 100 0 n#90 hwin.hwin#88.in 1360 304 1424 304 1424 256 1456 256 elongouts.dcCadMark.DOL
w 920 -85 100 0 n#87 ecad4.ecad4#44.PLNK 656 -96 1232 -96 1232 224 1456 224 elongouts.dcCadMark.SLNK
w 720 450 100 0 n#30 ecad4.ecad4#44.MESS 656 448 832 448 832 576 960 576 outhier.MESS.p
w 802 592 100 0 n#29 ecad4.ecad4#44.VAL 656 480 800 480 800 656 960 656 outhier.VAL.p
w 136 418 100 0 n#28 inhier.ICID.P 96 416 224 416 224 448 336 448 ecad4.ecad4#44.ICID
w 208 482 100 0 n#27 inhier.DIR.P 96 496 128 496 128 480 336 480 ecad4.ecad4#44.DIR
s 2320 1264 150 0 gmSeqCadCarC
s 816 1024 180 0 Sequence Command Type C
s 720 928 150 0 MARK command to Detector Controller only
[cell use]
use hwin -64 23 100 0 hwin#93
xform 0 32 64
p -61 56 100 0 -1 val(in):$(gm)dcDisabled.VAL
use hwin 1168 263 100 0 hwin#88
xform 0 1264 304
p 1171 296 100 0 -1 val(in):$(CAD_MARK)
use outhier 952 616 100 0 VAL
xform 0 944 656
use outhier 952 536 100 0 MESS
xform 0 944 576
use outhier 2112 215 100 0 FLNK
xform 0 2128 256
use elongouts 1456 135 100 0 dcCadMark
xform 0 1584 224
p 1296 366 100 0 0 EGU:CAD directive
p 1520 48 100 0 1 OMSL:closed_loop
p 1808 192 100 0 -1 def(OUT):$(dc)$(seqcommand).DIR
p 1568 80 100 1024 1 name:$(gm)$(seqcommand)$(I)
p 1712 192 75 768 -1 pproc(OUT):PP
use oslBorderC -608 -1177 100 0 oslBorderC#48
xform 0 1072 128
p 2332 -928 120 256 -1 Title:GMOS IS - detector controller command (type C)
use ecad4 336 -217 100 0 ecad4#44
xform 0 496 160
p 272 -512 100 0 0 FTVD:LONG
p 432 16 100 0 -1 SNAM:$(snam)
p 560 -448 100 0 0 def(OUTA):0.0
p 560 -480 100 0 0 def(OUTB):0.0
p 560 -512 100 0 0 def(OUTC):0.0
p 560 -544 100 0 0 def(OUTD):0.0
p 448 -224 100 1024 1 name:$(gm)$(seqcommand)
use inhier 104 456 100 0 DIR
xform 0 96 496
use inhier 104 376 100 0 ICID
xform 0 96 416
[comments]

[schematic2]
uniq 102
[tools]
[detail]
w 202 67 100 0 n#101 hwin.hwin#100.in 128 64 336 64 ecad4.ecad4#44.INPD
w 1472 427 100 0 FLNK estringouts.datasetID.FLNK 1376 416 1616 416 outhier.FLNK.p
w 848 -85 100 0 n#98 ecad4.ecad4#44.PLNK 656 -96 1088 -96 1088 400 1120 400 estringouts.datasetID.SLNK
w 832 299 100 0 n#94 ecad4.ecad4#44.VALA 656 288 1056 288 1056 432 1120 432 estringouts.datasetID.DOL
w 720 450 100 0 n#30 ecad4.ecad4#44.MESS 656 448 832 448 832 576 960 576 outhier.MESS.p
w 802 592 100 0 n#29 ecad4.ecad4#44.VAL 656 480 800 480 800 656 960 656 outhier.VAL.p
w 136 418 100 0 n#28 inhier.ICID.P 96 416 224 416 224 448 336 448 ecad4.ecad4#44.ICID
w 208 482 100 0 n#27 inhier.DIR.P 96 496 128 496 128 480 336 480 ecad4.ecad4#44.DIR
s 720 928 150 0 MARK command to Detector Controller only
s 816 1024 180 0 Observe Sequence Command
s 2320 1264 150 0 gmSeqCadObserve
[cell use]
use hwin -64 23 100 0 hwin#100
xform 0 32 64
p -61 56 100 0 -1 val(in):$(gm)dcDisabled.VAL
use estringouts 1120 327 -100 0 datasetID
xform 0 1248 400
p 1200 224 100 0 1 OMSL:closed_loop
p 1376 352 100 0 -1 def(OUT):$(dc)observe.A
p 1232 288 100 1024 1 name:$(gm)datasetID
use outhier 1584 375 100 0 FLNK
xform 0 1600 416
use outhier 952 536 100 0 MESS
xform 0 944 576
use outhier 952 616 100 0 VAL
xform 0 944 656
use oslBorderC -608 -1177 100 0 oslBorderC#48
xform 0 1072 128
p 2332 -928 120 256 -1 Title:GMOS IS - observe command
use ecad4 336 -217 100 0 ecad4#44
xform 0 496 160
p 448 288 100 0 1 FTVA:STRING
p 272 -512 100 0 0 FTVD:LONG
p 432 16 100 0 -1 SNAM:$(snam)
p 560 -448 100 0 0 def(OUTA):0.0
p 560 -480 100 0 0 def(OUTB):0.0
p 560 -512 100 0 0 def(OUTC):0.0
p 560 -544 100 0 0 def(OUTD):0.0
p 448 -224 100 1024 1 name:$(gm)$(seqcommand)
use inhier 104 376 100 0 ICID
xform 0 96 416
use inhier 104 456 100 0 DIR
xform 0 96 496
[comments]

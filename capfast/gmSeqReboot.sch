[schematic2]
uniq 114
[tools]
[detail]
w 874 299 100 0 n#113 ecad4.reboot.PLNK 448 0 560 0 560 288 1248 288 elongouts.rebootdcCadMark.SLNK
w 2170 75 100 0 c#112 ecars.rebootC.OERR 2144 64 2256 64 2256 0 2336 0 outhier.OERR.p
w 2210 107 100 0 OMSS ecars.rebootC.OMSS 2144 96 2336 96 outhier.OMSS.p
w 2210 171 100 0 c#110 ecars.rebootC.VAL 2144 160 2336 160 outhier.CAR.p
w 1508 251 100 2 c#101 rboat.c#101.p 1504 256 1504 256 elongouts.rebootdcCadMark.OUT
w 1570 171 100 0 n#100 eseqs.rebootSeq.LNK1 1136 32 1376 32 1376 160 1824 160 ecars.rebootC.IVAL
w 820 27 100 2 n#97 hwin.hwin#96.in 816 32 816 32 eseqs.rebootSeq.DOL1
w 528 -21 100 0 n#94 ecad4.reboot.STLK 448 -32 656 -32 656 -288 816 -288 eseqs.rebootSeq.SLNK
w 1696 331 100 0 c#91 elongouts.rebootdcCadMark.FLNK 1504 320 1936 320 outhier.FLNK.p
w 1160 379 100 0 n#90 hwin.hwin#88.in 1152 368 1216 368 1216 320 1248 320 elongouts.rebootdcCadMark.DOL
w 512 546 100 0 n#30 ecad4.reboot.MESS 448 544 624 544 624 672 752 672 outhier.MESS.p
w 594 688 100 0 n#29 ecad4.reboot.VAL 448 576 592 576 592 752 752 752 outhier.VAL.p
w -72 514 100 0 n#28 inhier.ICID.P -112 512 16 512 16 544 128 544 ecad4.reboot.ICID
w 0 578 100 0 n#27 inhier.DIR.P -112 592 -80 592 -80 576 128 576 ecad4.reboot.DIR
s 720 928 150 0 MARK command to Detector Controller only
s 816 1024 180 0 Reboot Sequence Command
s 2320 1264 150 0 gmSeqReboot
[cell use]
use outhier 1904 279 100 0 FLNK
xform 0 1920 320
use outhier 744 632 100 0 MESS
xform 0 736 672
use outhier 744 712 100 0 VAL
xform 0 736 752
use outhier 2368 144 100 0 CAR
xform 0 2320 160
use outhier 2384 80 100 0 OMSS
xform 0 2320 96
use outhier 2384 0 100 0 OERR
xform 0 2320 0
use esubs 1536 -569 100 0 subReboot
xform 0 1680 -304
p 1248 -450 100 0 0 INAM:gmSeqNullInit
p 1616 -432 100 0 1 SNAM:gmSeqReboot
use rboat 1504 215 100 0 c#101
xform 0 1584 256
use ecars 1824 -121 100 0 rebootC
xform 0 1984 48
use hwin 960 327 100 0 hwin#88
xform 0 1056 368
p 963 360 100 0 -1 val(in):$(CAD_MARK)
use hwin 624 -9 100 0 hwin#96
xform 0 720 32
p 627 24 100 0 -1 val(in):$(CAR_BUSY)
use eseqs 816 -377 100 0 rebootSeq
xform 0 976 -128
p 928 -80 100 0 1 DLY1:0.0
p 928 -112 100 0 1 DLY2:3.0
p 1168 -16 100 0 -1 def(LNK2):$(gm)subReboot.VAL
p 1152 32 75 1024 -1 pproc(LNK1):PP
p 1152 0 75 1024 -1 pproc(LNK2):PP
use elongouts 1248 199 100 0 rebootdcCadMark
xform 0 1376 288
p 1088 430 100 0 0 EGU:CAD directive
p 1312 384 100 0 1 OMSL:closed_loop
p 1600 256 100 0 -1 def(OUT):$(dc)reboot.DIR
p 1504 256 75 768 -1 pproc(OUT):PP
use oslBorderC -608 -1177 100 0 oslBorderC#48
xform 0 1072 128
p 2332 -928 120 256 -1 Title:GMOS IS - reboot sequence command
use ecad4 128 -121 100 0 reboot
xform 0 288 256
p 208 208 100 0 -1 SNAM:gmSeqCadReboot
p 352 -352 100 0 0 def(OUTA):0.0
p 352 -384 100 0 0 def(OUTB):0.0
p 352 -416 100 0 0 def(OUTC):0.0
p 352 -448 100 0 0 def(OUTD):0.0
use inhier -104 472 100 0 ICID
xform 0 -112 512
use inhier -104 552 100 0 DIR
xform 0 -112 592
[comments]

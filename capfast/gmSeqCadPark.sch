[schematic2]
uniq 143
[tools]
[detail]
w 802 579 100 0 n#131 elongouts.dtaStopXY.FLNK 992 512 1056 512 1056 576 608 576 608 768 736 768 elongouts.dtaStopZ.SLNK
w 1202 1091 100 0 n#133 elongouts.atmStop.FLNK 992 1088 1472 1088 1472 976 1344 976 1344 784 1456 784 efanouts.PresetFan.SLNK
w 802 867 100 0 n#132 elongouts.dtaStopZ.FLNK 992 800 1056 800 1056 864 608 864 608 1056 736 1056 elongouts.atmStop.SLNK
w 740 1083 100 2 n#128 elongouts.atmStop.DOL 736 1088 736 1088 hwin.hwin#130.in
w 962 1035 100 0 n#126 hwout.hwout#127.outp 992 1024 992 1024 elongouts.atmStop.OUT
w 740 795 100 2 n#123 elongouts.dtaStopZ.DOL 736 800 736 800 hwin.hwin#125.in
w 962 747 100 0 n#121 hwout.hwout#122.outp 992 736 992 736 elongouts.dtaStopZ.OUT
w 740 507 100 2 n#118 elongouts.dtaStopXY.DOL 736 512 736 512 hwin.hwin#120.in
w 514 483 100 0 n#114 ecad4.ecad4#44.PLNK 160 144 352 144 352 480 736 480 elongouts.dtaStopXY.SLNK
w 962 459 100 0 n#113 elongouts.dtaStopXY.OUT 992 448 992 448 hwout.hwout#111.outp
w 1084 -437 100 0 n#105 efanouts.StartFan.LNK2 720 -144 1088 -144 1088 -720 1344 -720 eseqs.CarSeqVal.SLNK
w 844 19 100 0 n#104 efanouts.StartFan.LNK1 720 -112 848 -112 848 160 944 160 elongouts.CarBusy.SLNK
w 1882 659 100 0 n#103 efanouts.PresetFan.LNK2 1696 832 1776 832 1776 656 2048 656 elongouts.ccCadMark.SLNK
w 1762 867 100 0 n#102 efanouts.PresetFan.LNK1 1696 864 1888 864 1888 896 2048 896 elongouts.dcCadMark.SLNK
w 226 123 100 0 n#101 ecad4.ecad4#44.STLK 160 112 352 112 352 -192 480 -192 efanouts.StartFan.SLNK
w 1348 -405 100 2 n#95 hwin.hwin#94.in 1344 -400 1344 -400 eseqs.CarSeqVal.DOL1
w 2052 683 100 2 n#92 hwin.hwin#91.in 2048 688 2048 688 elongouts.ccCadMark.DOL
w 2052 923 100 2 n#90 hwin.hwin#89.in 2048 928 2048 928 elongouts.dcCadMark.DOL
w 2560 -581 100 0 CFLK ecars.ecars#76.FLNK 2432 -624 2528 -624 2528 -592 2640 -592 outhier.CFLK.p
w 2464 -485 100 0 OERR ecars.ecars#76.OERR 2432 -496 2544 -496 2544 -512 2640 -512 outhier.OERR.p
w 2464 -453 100 0 OMSS ecars.ecars#76.OMSS 2432 -464 2544 -464 2544 -432 2640 -432 outhier.OMSS.p
w 2266 -301 100 0 c#80 junction 2080 -400 2080 -304 2512 -304 2512 -352 2640 -352 outhier.CAR.p
w 1864 -397 100 0 c#80 eseqs.CarSeqVal.LNK1 1664 -400 2112 -400 ecars.ecars#76.IVAL
w 2032 -112 100 0 c#80 eseqs.CarSeqBusy.LNK1 1824 240 2080 240 2080 -304 junction
w 1340 51 100 0 n#62 elongouts.CarBusy.FLNK 1200 192 1344 192 1344 -80 1504 -80 eseqs.CarSeqBusy.SLNK
w 1264 163 100 0 n#61 elongouts.CarBusy.VAL 1200 160 1376 160 1376 240 1504 240 eseqs.CarSeqBusy.DOL1
w 948 187 100 2 n#52 elongouts.CarBusy.DOL 944 192 944 192 hwin.hwin#24.in
w 224 690 100 0 n#30 ecad4.ecad4#44.MESS 160 688 336 688 336 816 464 816 outhier.MESS.p
w 306 832 100 0 n#29 ecad4.ecad4#44.VAL 160 720 304 720 304 896 464 896 outhier.VAL.p
w -360 643 100 0 n#28 inhier.ICID.P -400 640 -272 640 -272 688 -160 688 ecad4.ecad4#44.ICID
w -304 723 100 0 n#27 inhier.DIR.P -400 720 -160 720 ecad4.ecad4#44.DIR
s 784 224 100 0 BUSY
s -416 1120 180 0 PARK Sequencer Command
s -432 1024 150 0 Mark command in Component and Detector Controllers
s 2272 1216 150 0 gmSeqCadPark
s 1920 1056 140 0 Write MARK directive to
s 1856 1008 140 0 the subsystem sequence command CADs
s -416 960 150 0 Also stop all atmDC and flexure following.
s 688 1168 100 0 Stop the AtmDc and dta following
[cell use]
use hwin 776 152 100 0 hwin#24
xform 0 848 192
p 766 120 100 0 0 typ(in):val
p 755 184 100 0 -1 val(in):$(CAR_BUSY)
use hwin 1856 887 100 0 hwin#89
xform 0 1952 928
p 1859 920 100 0 -1 val(in):$(CAD_MARK)
use hwin 1856 647 100 0 hwin#91
xform 0 1952 688
p 1859 680 100 0 -1 val(in):$(CAD_MARK)
use hwin 1152 -441 100 0 hwin#94
xform 0 1248 -400
p 1155 -408 100 0 -1 val(in):$(CAR_IDLE)
use hwin 568 472 100 0 hwin#120
xform 0 640 512
p 558 440 100 0 0 typ(in):val
p 547 504 100 0 -1 val(in):0
use hwin 568 760 100 0 hwin#125
xform 0 640 800
p 558 728 100 0 0 typ(in):val
p 547 792 100 0 -1 val(in):0
use hwin 568 1048 100 0 hwin#130
xform 0 640 1088
p 558 1016 100 0 0 typ(in):val
p 547 1080 100 0 -1 val(in):0
use hwout 992 407 100 0 hwout#111
xform 0 1088 448
p 1088 439 100 0 -1 val(outp):$(gm)dtaCadtrackingXY.VAL PP NMS
use hwout 992 695 100 0 hwout#122
xform 0 1088 736
p 1088 727 100 0 -1 val(outp):$(gm)dtaCadtrackingZ.VAL PP NMS
use hwout 992 983 100 0 hwout#127
xform 0 1088 1024
p 1088 1015 100 0 -1 val(outp):$(gm)atmCadtracking.VAL PP NMS
use elongouts 944 71 100 0 CarBusy
xform 0 1072 160
p 784 302 100 0 0 EGU:CAR value
p 1008 0 100 0 1 OMSL:closed_loop
p 1120 32 100 1024 1 name:$(gm)$(seqcommand)$(I)
use elongouts 2048 807 100 0 dcCadMark
xform 0 2176 896
p 1888 1038 100 0 0 EGU:CAD directive
p 2112 752 100 0 1 OMSL:closed_loop
p 2384 864 100 0 -1 def(OUT):$(dc)$(seqcommand).DIR
p 2112 768 100 1024 1 name:$(gm)$(seqcommand)$(I)
p 2304 864 75 768 -1 pproc(OUT):PP
use elongouts 2048 567 100 0 ccCadMark
xform 0 2176 656
p 1888 798 100 0 0 EGU:CAD directive
p 2112 512 100 0 1 OMSL:closed_loop
p 2384 624 100 0 -1 def(OUT):$(cc)$(seqcommand).DIR
p 2144 528 100 1024 1 name:$(gm)$(seqcommand)$(I)
p 2304 624 75 768 -1 pproc(OUT):PP
use elongouts 736 391 100 0 dtaStopXY
xform 0 864 480
p 576 622 100 0 0 EGU:0/1
p 800 320 100 0 1 OMSL:closed_loop
p 912 352 100 1024 1 name:$(gm)$(seqcommand)$(I)
p 992 448 75 768 -1 pproc(OUT):PP
use elongouts 736 679 100 0 dtaStopZ
xform 0 864 768
p 576 910 100 0 0 EGU:0/1
p 800 608 100 0 1 OMSL:closed_loop
p 912 640 100 1024 1 name:$(gm)$(seqcommand)$(I)
p 992 736 75 768 -1 pproc(OUT):PP
use elongouts 736 967 100 0 atmStop
xform 0 864 1056
p 576 1198 100 0 0 EGU:0/1
p 800 896 100 0 1 OMSL:closed_loop
p 912 928 100 1024 1 name:$(gm)$(seqcommand)$(I)
p 992 1024 75 768 -1 pproc(OUT):PP
use ukatcBorderC -576 -1241 100 0 ukatcBorderC#106
xform 0 1104 64
p 2364 -992 120 256 -1 Title:GMOS IS - park sequence command
p 2080 -1056 100 768 -1 author:$Author$
use efanouts 480 -329 100 0 StartFan
xform 0 600 -176
p 512 -64 100 0 1 SELM:All
p 608 -368 100 1024 -1 name:$(gm)$(seqcommand)$(I)
use efanouts 1488 672 100 0 PresetFan
xform 0 1576 800
p 1488 928 100 0 1 SELM:All
p 1600 624 100 1024 -1 name:$(gm)$(seqcommand)$(I)
use outhier 2608 -633 100 0 CFLK
xform 0 2624 -592
use outhier 2608 -553 100 0 OERR
xform 0 2624 -512
use outhier 2608 -473 100 0 OMSS
xform 0 2624 -432
use outhier 2608 -393 100 0 CAR
xform 0 2624 -352
use outhier 456 856 100 0 VAL
xform 0 448 896
use outhier 456 776 100 0 MESS
xform 0 448 816
use ecars 2112 -681 100 0 ecars#76
xform 0 2272 -512
p 2288 -688 100 1024 1 name:$(gm)is$(seqcommand)C
use eseqs 1368 -808 100 0 CarSeqVal
xform 0 1504 -560
p 1456 -480 100 0 1 DLY1:0.1
p 1456 -512 100 0 0 DLY2:0.0
p 1456 -544 100 0 0 DLY3:0.0
p 1696 -368 100 0 0 def(LNK1):0.0
p 1488 -848 100 1024 1 name:$(gm)$(seqcommand)$(I)
p 1680 -400 75 1024 -1 pproc(LNK1):PP
p 1680 -432 75 1024 -1 pproc(LNK2):PP
p 1680 -464 75 1024 -1 pproc(LNK3):PP
use eseqs 1504 -176 100 0 CarSeqBusy
xform 0 1664 80
p 1584 126 100 0 1 DLY1:0.0
p 1584 126 100 0 0 DLY2:0.0
p 1872 272 100 0 0 def(LNK1):0.0
p 1664 -208 100 1024 1 name:$(gm)$(seqcommand)$(I)
p 1504 272 75 1280 -1 palrm(SELL):NMS
p 1472 240 75 1280 -1 pproc(DOL1):NPP
p 1472 208 75 1280 -1 pproc(DOL2):NPP
p 1824 250 75 0 -1 pproc(LNK1):PP
p 1824 218 75 0 -1 pproc(LNK2):NPP
use ecad4 -160 23 100 0 ecad4#44
xform 0 0 400
p -64 240 100 0 1 SNAM:$(snam)
p 64 -208 100 0 0 def(OUTA):0.0
p 64 -240 100 0 0 def(OUTB):0.0
p 64 -272 100 0 0 def(OUTC):0.0
p 64 -304 100 0 0 def(OUTD):0.0
p -48 16 100 1024 1 name:$(gm)$(seqcommand)
use inhier -392 680 100 0 DIR
xform 0 -400 720
use inhier -392 600 100 0 ICID
xform 0 -400 640
[comments]

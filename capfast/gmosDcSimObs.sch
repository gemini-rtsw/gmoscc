[schematic2]
uniq 63
[tools]
[detail]
w 1088 219 100 0 CFLK ecad2.ecad2#8.STLK 1008 208 1216 208 outhier.CFLK.p
w 1072 658 100 0 n#30 ecad2.ecad2#8.MESS 1008 656 1184 656 1184 784 1312 784 outhier.MESS.p
w 1154 800 100 0 n#29 ecad2.ecad2#8.VAL 1008 688 1152 688 1152 864 1312 864 outhier.VAL.p
w 488 626 100 0 n#28 inhier.ICID.P 448 624 576 624 576 656 688 656 ecad2.ecad2#8.ICID
w 560 690 100 0 n#27 inhier.DIR.P 448 704 480 704 480 688 688 688 ecad2.ecad2#8.DIR
s 704 1392 140 0 observing CAD
s 624 1440 140 0 Detector Controller Simulated
s 2112 1632 150 0 gmosDcSimObs.sch
[cell use]
use outhier 1304 824 100 0 VAL
xform 0 1296 864
use outhier 1304 744 100 0 MESS
xform 0 1296 784
use outhier 1184 167 100 0 CFLK
xform 0 1200 208
use oslBorderC -816 -809 100 0 oslBorderC#50
xform 0 864 496
use inhier 456 664 100 0 DIR
xform 0 448 704
use inhier 456 584 100 0 ICID
xform 0 448 624
use ecad2 712 120 100 0 ecad2#8
xform 0 848 432
p 752 920 100 0 0 FTVA:STRING
p 720 350 100 0 -1 SNAM:gmSeqCadTrivial
p 592 494 100 0 0 def(INPA):0.0
p 1088 478 100 0 0 def(OUTA):0.0
p 704 62 100 0 1 name:$(gm)$(seqcommand)
p 1008 464 75 768 -1 pproc(OUTA):NPP
p 752 -200 100 0 0 typ(INPA):path
[comments]

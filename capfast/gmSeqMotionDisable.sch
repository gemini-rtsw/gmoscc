[schematic2]
uniq 13
[tools]
[detail]
w 1520 1203 100 0 n#12 ecalcs.calcMotionDisable.FLNK 1408 1200 1680 1200 1680 1136 1856 1136 ebos.motionDisable.SLNK
w 864 987 100 0 c#10 inhier.SLNK.P 656 976 1120 976 ecalcs.calcMotionDisable.SLNK
w 1614 1171 100 0 n#8 ecalcs.calcMotionDisable.VAL 1408 1168 1856 1168 ebos.motionDisable.DOL
w 830 1307 100 0 c#5 inhier.RDOUT.P 672 1296 1024 1296 1024 1328 1120 1328 ecalcs.calcMotionDisable.INPB
w 878 1371 100 0 c#4 inhier.ILOCK.P 672 1360 1120 1360 ecalcs.calcMotionDisable.INPA
s 2976 2272 140 0 gmSeqMotionDisable.sch
[cell use]
use inhier 592 1360 100 0 ILOCK
xform 0 672 1360
use inhier 592 1280 100 0 RDOUT
xform 0 672 1296
use inhier 576 976 100 0 SLNK
xform 0 656 976
use ebos 1856 1047 100 0 motionDisable
xform 0 1984 1136
p 1936 976 100 0 1 OMSL:closed_loop
p 1936 912 100 0 1 ONAM:disabled
p 1936 944 100 0 1 ZNAM:enabled
use oslBorderC 160 -169 100 0 oslBorderC#2
xform 0 1840 1136
p 3100 80 120 256 -1 Title:GMOS IS - generate motion disabled flag
use ecalcs 1120 887 100 0 calcMotionDisable
xform 0 1264 1152
p 1216 1072 100 0 1 CALC:A||B
p 832 1038 100 0 0 EGU:0/1
[comments]

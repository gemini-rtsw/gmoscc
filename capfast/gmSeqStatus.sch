[schematic2]
uniq 28
[tools]
[detail]
w 2120 795 100 0 APPLYC gmSeqApplycCombine.gmSeqApplycCombine#2.APPLYC 2064 784 2224 784 outhier.APPLYC.p
w 1160 1403 100 0 ISFLNK inhier.ISFLNK.P 944 1392 1472 1392 1472 752 1616 752 gmSeqApplycCombine.gmSeqApplycCombine#2.ISFLNK
w 1176 1531 100 0 ISMESS inhier.ISMESS.P 944 1520 1504 1520 1504 784 1616 784 gmSeqApplycCombine.gmSeqApplycCombine#2.ISMESS
w 1192 1643 100 0 ISCAR inhier.ISCAR.P 944 1632 1536 1632 1536 816 1616 816 gmSeqApplycCombine.gmSeqApplycCombine#2.ISCAR
w 1200 1739 100 0 CONFC inhier.CONFC.P 944 1728 1568 1728 1568 848 1616 848 gmSeqApplycCombine.gmSeqApplycCombine#2.CONFC
w 622 731 100 0 CC inhier.CC.P -336 720 1616 720 gmSeqApplycCombine.gmSeqApplycCombine#2.CC
w 1004 587 100 0 CC junction 1008 720 1008 464 1184 464 gmSeqHeartBeat.gmSeqHeartBeat#4.CC
w 622 667 100 0 DC inhier.DC.P -336 656 1472 656 1472 688 1616 688 gmSeqApplycCombine.gmSeqApplycCombine#2.DC
w 1068 515 100 0 DC junction 1072 656 1072 384 1184 384 gmSeqHeartBeat.gmSeqHeartBeat#4.DC
s 400 1568 100 0 the sequencer local CAR records
s 432 1600 100 0 These input values are from
s 1968 1888 150 0 gmSeqStatus.sch
[cell use]
use outhier 2192 743 100 0 APPLYC
xform 0 2208 784
use gmSeqLocalCars -272 1127 100 0 gmSeqLocalCars#25
xform 0 -72 1296
use inhier 928 1351 100 0 ISFLNK
xform 0 944 1392
use inhier 928 1479 100 0 ISMESS
xform 0 944 1520
use inhier 928 1591 100 0 ISCAR
xform 0 944 1632
use inhier 928 1687 100 0 CONFC
xform 0 944 1728
use inhier -384 656 100 0 DC
xform 0 -336 656
use inhier -384 704 100 0 CC
xform 0 -336 720
use gmSeqHeartBeat 1232 287 100 0 gmSeqHeartBeat#4
xform 0 1408 424
use gmSeqApplycCombine 1664 647 100 0 gmSeqApplycCombine#2
xform 0 1840 784
use oslBorderC -896 -553 100 0 oslBorderC#1
xform 0 784 752
p 2044 -304 120 256 -1 Title:GMOS Instrument Sequencer Status
[comments]

[schematic2]
uniq 73
[tools]
[detail]
w 632 747 100 0 n#70 gmSeqWaitChange.gmSeqWaitChange#67.NEWVAL -368 1152 -224 1152 -224 736 1536 736 gmSeqApply.gmSeqApply#6.MENB
w 380 347 100 0 n#51 gmSeqMotionDisable.gmSeqMotionDisable#38.RDOUT 560 -96 384 -96 384 800 junction
w 664 803 100 0 n#51 gmSeqWaitChange.gmSeqWaitChange#64.NEWVAL -368 1456 -160 1456 -160 800 1536 800 gmSeqApply.gmSeqApply#6.RDOUT
w 476 411 100 0 ILOCK gmSeqMotionDisable.gmSeqMotionDisable#38.ILOCK 560 -32 480 -32 480 864 junction
w 714 835 100 0 ILOCK gmSeqInterlock.gmSeqInterlock#0.ILOCK -368 1744 -48 1744 -48 864 1536 864 gmSeqApply.gmSeqApply#6.ILOCK
w -196 1547 100 0 n#65 gmSeqInterlock.gmSeqInterlock#0.FLNK -368 1680 -192 1680 -192 1424 junction
w -196 643 100 0 n#65 gmSeqWaitChange.gmSeqWaitChange#64.FLNK -368 1424 -192 1424 -192 -128 560 -128 gmSeqMotionDisable.gmSeqMotionDisable#38.SLNK
w 1208 1635 100 0 n#63 gmSeqApply.gmSeqApply#6.CONFC 2016 928 2208 928 2208 1632 256 1632 256 1440 400 1440 gmSeqStatus.gmSeqStatus#5.CONFC
w 1208 1667 100 0 n#62 gmSeqApply.gmSeqApply#6.ISCAR 2016 832 2272 832 2272 1664 192 1664 192 1408 400 1408 gmSeqStatus.gmSeqStatus#5.ISCAR
w 1208 1699 100 0 n#61 gmSeqApply.gmSeqApply#6.ISMESS 2016 720 2336 720 2336 1696 128 1696 128 1376 400 1376 gmSeqStatus.gmSeqStatus#5.ISMESS
w 1208 1731 100 0 n#60 gmSeqApply.gmSeqApply#6.ISFLNK 2016 624 2400 624 2400 1728 64 1728 64 1344 400 1344 gmSeqStatus.gmSeqStatus#5.ISFLNK
w 156 923 100 0 n#55 gmSeqStatus.gmSeqStatus#5.CC 400 1312 160 1312 160 544 junction
w 600 547 100 0 n#55 gmSeqSubsysPresent.gmSeqSubsysPresent#9.DISV -368 128 -288 128 -288 544 1536 544 gmSeqApply.gmSeqApply#6.CC
w 220 875 100 0 n#57 gmSeqStatus.gmSeqStatus#5.DC 400 1280 224 1280 224 480 junction
w 616 483 100 0 n#57 gmSeqSubsysPresent.gmSeqSubsysPresent#10.DISV -368 -288 -256 -288 -256 480 1536 480 gmSeqApply.gmSeqApply#6.DC
w 1224 931 100 0 n#58 gmSeqStatus.gmSeqStatus#5.APPLYC 848 1376 960 1376 960 928 1536 928 gmSeqApply.gmSeqApply#6.APPLYC
s 0 496 100 0 DC
s 0 560 100 0 CC
s 0 816 100 0 RDOUT
s 0 880 100 0 ILOCK
s -720 272 100 0 Check Components Controller present
s -720 -144 100 0 Check Detector Controller present
s 2128 1872 150 0 gmSeq.sch
s 0 752 100 0 Master Enable
[cell use]
use gmSeqWaitChange -672 1047 100 0 gmSeqWaitChange#67
xform 0 -520 1160
p -688 1040 100 0 -1 seta:monitor $(cc)masterEnable.VAL
p -672 1008 100 0 -1 setb:prefix $(gm)CcMEnable
use gmSeqWaitChange -672 1351 100 0 gmSeqWaitChange#64
xform 0 -520 1464
p -688 1344 100 0 -1 seta:monitor $(sad)dc:rdout.VAL
p -672 1312 100 0 -1 setb:prefix $(gm)DcReadout
use gmSeqMotionDisable 560 -185 100 0 gmSeqMotionDisable#38
xform 0 696 -72
use gmSeqSubsysPresent -680 7 100 0 gmSeqSubsysPresent#9
xform 0 -520 120
p -664 0 100 0 1 seta:subsys cc
p -680 -48 100 0 1 setb:top $(top)$(subsys)
use gmSeqSubsysPresent -680 -409 100 0 gmSeqSubsysPresent#10
xform 0 -520 -296
p -680 -416 100 0 1 seta:subsys dc
p -680 -480 100 0 1 setb:top $(top)$(subsys)
use gmSeqApply 1632 439 100 0 gmSeqApply#6
xform 0 1776 712
use gmSeqStatus 480 1103 100 0 gmSeqStatus#5
xform 0 624 1376
use oslBorderC -832 -569 100 0 oslBorderC#1
xform 0 848 736
p 2108 -320 120 256 -1 Title:GMOS Instrument Sequencer - under top level
use gmSeqInterlock -672 1623 100 0 gmSeqInterlock#0
xform 0 -520 1736
[comments]

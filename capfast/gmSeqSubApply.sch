[schematic2]
uniq 61
[tools]
[detail]
w 3616 1499 100 0 c#58 inhier.DC.P 3584 1488 3696 1488 gmSeqDriveSubApply.gmSeqDriveSubApply#10.SDIS
w 3616 1883 100 0 c#57 inhier.CC.P 3584 1872 3696 1872 gmSeqDriveSubApply.gmSeqDriveSubApply#9.SDIS
w 3544 1547 100 0 n#56 efanouts.SubApplyStartFan.LNK2 3280 1760 3440 1760 3440 1536 3696 1536 gmSeqDriveSubApply.gmSeqDriveSubApply#10.SLNK
w 3544 1931 100 0 n#55 efanouts.SubApplyStartFan.LNK1 3280 1792 3440 1792 3440 1920 3696 1920 gmSeqDriveSubApply.gmSeqDriveSubApply#9.SLNK
w 2872 1931 100 0 n#54 efanouts.SubApplyFan.LNK3 2832 1920 2960 1920 2960 1712 3040 1712 efanouts.SubApplyStartFan.SLNK
w 2810 2411 100 0 INVAL junction 2512 1984 2512 2400 3168 2400 elongouts.ccDirectiveOut.DOL
w 2472 1995 100 0 INVAL inhier.DIRVAL.P 2208 1712 2352 1712 2352 1984 2640 1984 efanouts.SubApplyFan.SELL
w 3496 2427 100 0 n#52 elongouts.ccDirectiveOut.VAL 3424 2368 3456 2368 3456 2416 3584 2416 3584 2400 3664 2400 elongouts.dcDirectiveOut.DOL
w 3560 2379 100 0 n#49 elongouts.ccDirectiveOut.FLNK 3424 2400 3504 2400 3504 2368 3664 2368 elongouts.dcDirectiveOut.SLNK
w 2856 1963 100 0 n#47 efanouts.SubApplyFan.LNK2 2832 1952 2928 1952 2928 1984 junction
w 3032 2379 100 0 n#47 efanouts.SubApplyFan.LNK1 2832 1984 2944 1984 2944 2368 3168 2368 elongouts.ccDirectiveOut.SLNK
w 3700 1419 100 2 n#45 hwin.hwin#44.in 3696 1424 3696 1424 gmSeqDriveSubApply.gmSeqDriveSubApply#10.DIR
w 3700 1803 100 2 n#43 hwin.hwin#42.in 3696 1808 3696 1808 gmSeqDriveSubApply.gmSeqDriveSubApply#9.DIR
w 4296 1547 100 0 c#32 gmSeqDriveSubApply.gmSeqDriveSubApply#10.SUBVAL 4240 1536 4400 1536 outhier.DCSUBVAL.p
w 4296 1931 100 0 c#30 gmSeqDriveSubApply.gmSeqDriveSubApply#9.SUBVAL 4240 1920 4400 1920 outhier.CCSUBVAL.p
w 2312 1963 100 0 STLK inhier.SLNK.P 2208 1952 2464 1952 2464 1904 2592 1904 efanouts.SubApplyFan.SLNK
s 4096 2912 150 0 gmSeqSubApply.sch
[cell use]
use inhier 3536 1488 100 0 DC
xform 0 3584 1488
use inhier 3536 1872 100 0 CC
xform 0 3584 1872
use inhier 2112 1936 150 0 SLNK
xform 0 2208 1952
use inhier 2064 1696 150 0 DIRVAL
xform 0 2208 1712
use efanouts 2592 1767 100 0 SubApplyFan
xform 0 2712 1920
p 2608 2048 100 0 1 SELM:Specified
use efanouts 3040 1575 100 0 SubApplyStartFan
xform 0 3160 1728
p 3056 1840 100 0 1 SELM:All
use elongouts 3664 2279 100 0 dcDirectiveOut
xform 0 3792 2368
p 3696 2224 100 0 1 OMSL:closed_loop
p 3872 2304 100 0 -1 def(OUT):$(dc)apply.DIR
p 3920 2336 75 768 -1 pproc(OUT):PP
use elongouts 3168 2279 100 0 ccDirectiveOut
xform 0 3296 2368
p 3200 2224 100 0 1 OMSL:closed_loop
p 3360 2304 100 0 -1 def(OUT):$(cc)apply.DIR
p 3424 2336 75 768 -1 pproc(OUT):PP
use hwin 3504 1383 100 0 hwin#44
xform 0 3600 1424
p 3507 1416 100 0 -1 val(in):$(CAD_START)
use hwin 3504 1767 100 0 hwin#42
xform 0 3600 1808
p 3507 1800 100 0 -1 val(in):$(CAD_START)
use outhier 4368 1879 100 0 CCSUBVAL
xform 0 4384 1920
use outhier 4368 1495 100 0 DCSUBVAL
xform 0 4384 1536
use gmSeqDriveSubApply 3728 1303 100 0 gmSeqDriveSubApply#10
xform 0 3968 1448
p 3892 1196 100 0 0 scsetd:top $(gm)dcApply
p 3808 1280 100 0 1 seta:subsys dc
p 3828 1260 100 0 1 setb:prefix $(dc)
p 3848 1240 100 0 1 setc:instru DC
p 3872 1216 100 0 1 setd:top $(gm)dcApply
use gmSeqDriveSubApply 3728 1687 100 0 gmSeqDriveSubApply#9
xform 0 3968 1832
p 3792 1680 100 0 1 seta:subsys cc
p 3812 1660 100 0 1 setb:prefix $(cc)
p 3832 1640 100 0 1 setc:instru CC
p 3856 1616 100 0 1 setd:top $(gm)ccApply
use oslBorderC 1632 455 100 0 oslBorderC#1
xform 0 3312 1760
[comments]

[schematic2]
uniq 54
[tools]
[detail]
w 2296 1643 100 0 n#19 eaos.ActIdle.OUT 2368 736 2592 736 2592 1632 2048 1632 2048 2032 junction
w 1970 1872 100 0 n#19 eaos.ActTimeoutErr.OUT 1808 1664 1968 1664 1968 2032 2224 2032 ecars.CommSentC.IVAL
w 2264 1611 100 0 n#18 estringouts.ActNullMess.OUT 2368 1040 2560 1040 2560 1600 2016 1600 2016 1968 junction
w 1992 1970 100 0 n#18 estringouts.ActTimeoutMess.OUT 1808 1968 2224 1968 ecars.CommSentC.IMSS
w 2084 1371 -100 0 ACTVAL ecalcouts.applyWait.VAL 800 992 1312 992 1312 1360 2928 1360 outhier.SUBAPPLYC.p
w 1032 1067 100 0 n#49 ecalcouts.applyWait.FLNK 800 1056 1312 1056 1312 1008 1504 1008 efanouts.IssueStop.SLNK
w 1904 1067 100 0 n#48 efanouts.IssueStop.LNK2 1744 1056 2112 1056 estringouts.ActNullMess.SLNK
w 2248 914 100 0 n#44 estringouts.ActNullMess.FLNK 2368 1072 2496 1072 2496 912 2048 912 2048 768 2112 768 eaos.ActIdle.SLNK
w 2040 802 100 0 n#43 hwin.hwin#47.in 2016 800 2112 800 eaos.ActIdle.DOL
w 1214 1483 100 0 n#8 efanouts.IssueStop.LNK1 1744 1088 1872 1088 1872 1472 592 1472 592 1728 junction
w 526 1730 100 0 n#8 eaos.StopapplyCTimer.FLNK 432 1728 656 1728 gmSeqTimeOut.gmSeqTimeOut#4.STOP
w 2712 1819 -100 0 FLNK ecars.CommSentC.FLNK 2544 1808 2928 1808 outhier.FLNK.p
w 2712 1970 100 0 n#25 ecars.CommSentC.OMSS 2544 1968 2928 1968 outhier.CSOMSS.p
w 2712 2034 100 0 n#24 ecars.CommSentC.VAL 2544 2032 2928 2032 outhier.CSVAL.p
w 1688 1842 100 0 n#17 estringouts.ActTimeoutMess.FLNK 1808 2000 1936 2000 1936 1840 1488 1840 1488 1696 1552 1696 eaos.ActTimeoutErr.SLNK
w 1368 1986 100 0 n#16 gmSeqTimeOut.gmSeqTimeOut#4.EXPIRED 1104 1920 1232 1920 1232 1984 1552 1984 estringouts.ActTimeoutMess.SLNK
w 1480 1730 100 0 n#15 hwin.hwin#14.in 1456 1728 1552 1728 eaos.ActTimeoutErr.DOL
w 494 1986 100 0 n#7 eaos.StartapplyCTimer.FLNK 432 1984 592 1984 592 1920 656 1920 gmSeqTimeOut.gmSeqTimeOut#4.START
s 1200 720 130 0 responded to the command
s -32 2064 100 0 from elsewhere via their PROC fields
s -32 2096 100 0 the timer to be triggered
s -32 2128 100 0 These dummy records allow
s 2160 2224 130 0 This CAR is set BUSY when the
s 1280 592 130 0 and stop the timeout counter
s 1280 624 130 0 So set CommSentC to IDLE.
s 1200 752 130 0 indicates that the subsystem has
s 1200 784 130 0 Any change in the subsystem applyC
s 816 2352 160 0 This schematic gets the subsystem applyC value
s 816 2304 160 0 and sets the CommSentC value, indicating whether
s 816 2256 160 0 the subsystem has responded to the command
s 2672 2432 150 0 gmSeqCarMonitor.sch
s 2160 2176 130 0 subsystem APPLY is triggered
n 1824 2144 2048 2408 100
See subsystem 
apply triggering 
and timeout 
handling in 
schematic 
gmSeqDriveSubApply
_
[cell use]
use ecalcouts 480 871 100 0 applyWait
xform 0 640 992
p 552 904 100 0 -1 CALC:A
p 1080 1692 100 0 0 DESC:Monitor applyC
p 1312 1422 100 0 0 DOPT:Use CALC
p 1312 1454 100 0 0 OOPT:On Change
p 568 1104 100 0 1 SCAN:I/O Intr
p 896 1248 60 0 0 def(INPA):$(gm)$(subsys):applyC.VAL
use oslBorderC -192 7 100 0 oslBorderC#52
xform 0 1488 1312
p 2748 256 120 256 -1 Title:GMOS IS - monitor subsystem applyC status
use hwin 1848 760 100 0 hwin#47
xform 0 1920 800
p 1827 792 100 0 -1 val(in):$(CAR_IDLE)
use hwin 1288 1688 100 0 hwin#14
xform 0 1360 1728
p 1267 1720 100 0 -1 val(in):$(CAR_ERROR)
use eaos 2136 680 100 0 ActIdle
xform 0 2240 768
p 2144 864 100 0 -1 DESC:Output idle status
p 2176 640 100 0 1 OMSL:closed_loop
p 2368 736 75 768 -1 pproc(OUT):PP
use eaos 200 1864 100 0 StartapplyCTimer
xform 0 304 1952
p 160 2030 100 0 -1 DESC:Dummy record to trigger Start
p 256 1824 100 0 1 OMSL:supervisory
use eaos 200 1608 100 0 StopapplyCTimer
xform 0 304 1696
p 160 1774 100 0 -1 DESC:Dummy record to trigger Stop
p 256 1568 100 0 1 OMSL:supervisory
use eaos 1576 1608 100 0 ActTimeoutErr
xform 0 1680 1696
p 1568 1790 100 0 -1 DESC:Output error status
p 1616 1568 100 0 1 OMSL:closed_loop
p 1808 1664 75 768 -1 pproc(OUT):PP
use estringouts 2136 984 100 0 ActNullMess
xform 0 2240 1056
p 2128 1136 100 0 -1 DESC:Output null string
p 2176 944 100 0 1 OMSL:supervisory
p 2048 1022 100 0 0 VAL:string
use estringouts 1576 1912 100 0 ActTimeoutMess
xform 0 1680 1984
p 1568 2078 100 0 -1 DESC:Output time out string
p 1616 1872 100 0 1 OMSL:supervisory
p 1488 1950 100 0 0 VAL:$(subsys) timed out
use outhier 2920 1768 100 0 FLNK
xform 0 2912 1808
use outhier 2920 1992 100 0 CSVAL
xform 0 2912 2032
use outhier 2920 1928 100 0 CSOMSS
xform 0 2912 1968
use outhier 2920 1320 100 0 SUBAPPLYC
xform 0 2912 1360
use gmSeqTimeOut 680 1672 100 0 gmSeqTimeOut#4
xform 0 880 1824
p 720 1662 100 0 1 seta:timeout 6.0
use efanouts 1528 872 100 0 IssueStop
xform 0 1624 1024
p 1488 928 100 0 1 SELM:All
use ecars 2248 1752 100 0 CommSentC
xform 0 2384 1920
p 2288 2094 100 0 -1 DESC:Command sent CAR
p 2332 1744 100 1024 0 name:$(top)$(I)
[comments]

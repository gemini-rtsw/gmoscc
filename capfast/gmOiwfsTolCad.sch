[schematic2]
uniq 376
[tools]
[detail]
w 2002 2091 100 0 n#375 eseqs.tolSeq.LNK3 1792 1664 1888 1664 1888 2080 2176 2080 ecars.toleranceC.IVAL
w 1810 1739 100 0 n#375 eseqs.tolSeq.LNK1 1792 1728 1888 1728 junction
w 1314 1707 100 0 n#373 ecad4.tolerance.VALA 1088 1824 1216 1824 1216 1696 1472 1696 eseqs.tolSeq.DOL2
w 1476 1659 100 2 n#372 hwin.hwin#371.in 1472 1664 1472 1664 eseqs.tolSeq.DOL3
w 1476 1723 100 2 n#370 hwin.hwin#369.in 1472 1728 1472 1728 eseqs.tolSeq.DOL1
w 2090 1707 100 0 tolerance eseqs.tolSeq.LNK2 1792 1696 2496 1696 outhier.TOLERANCE.p
w 1090 1547 100 0 n#318 ecad4.tolerance.FLNK 1088 1536 1152 1536 1152 1952 1168 1952 estringouts.toleranceMess.SLNK
w 1098 1995 100 0 n#317 ecad4.tolerance.MESS 1088 1984 1168 1984 estringouts.toleranceMess.DOL
w 1428 1931 100 2 n#316 hwout.hwout#315.outp 1424 1936 1424 1936 estringouts.toleranceMess.OUT
w 1250 1419 100 0 n#300 ecad4.tolerance.STLK 1088 1408 1472 1408 eseqs.tolSeq.SLNK
s 704 1616 100 512 Max Tolerance
s 704 1680 100 512 Min Tolerance
s 704 1808 100 512 Target X & Y Tolerance
s 704 1744 100 512 Target Z Tolerance
s 1232 1664 100 0 IDLE
s 1232 1728 100 0 BUSY
[cell use]
use ecars 2368 2128 100 0 toleranceC
xform 0 2336 1968
p 2368 2128 100 512 -1 PV:$(top)
use hwin 1280 1687 100 0 hwin#369
xform 0 1376 1728
p 1283 1720 100 0 -1 val(in):2
use hwin 1280 1623 100 0 hwin#371
xform 0 1376 1664
p 1283 1656 100 0 -1 val(in):0
use eseqs 1632 1808 100 0 tolSeq
xform 0 1632 1568
p 1584 1360 100 0 1 DLY1:0.0
p 1584 1328 100 0 1 DLY2:0.0
p 1584 1296 100 0 1 DLY3:1.0
p 1632 1808 100 512 -1 PV:$(top)
p 1808 1728 75 1024 -1 pproc(LNK1):PP
p 1808 1696 75 1024 -1 pproc(LNK2):NPP
p 1808 1664 75 1024 -1 pproc(LNK3):PP
use ecad4 976 2064 100 0 tolerance
xform 0 928 1696
p 1008 1824 70 512 -1 FTVA:DOUBLE
p 704 896 100 0 0 PREC:4
p 960 2064 100 512 -1 PV:$(top)
p 816 1280 100 0 1 SNAM:oiwfsToleranceCad
p 1088 1450 75 0 -1 pproc(PLNK):NPP
p 1088 1418 75 0 -1 pproc(STLK):PP
use outhier 2464 1655 100 0 TOLERANCE
xform 0 2480 1696
use hwout 1424 1895 100 0 hwout#315
xform 0 1520 1936
p 1456 1968 100 0 -1 val(outp):$(top)rejectMess.IMSS NPP NMS
use estringouts 1312 2016 100 0 toleranceMess
xform 0 1296 1952
p 1232 1872 100 0 1 OMSL:closed_loop
p 1152 2016 100 0 -1 PV:$(top)$(dev)
p 1424 1936 75 768 -1 pproc(OUT):NPP
use gmosBorderC -144 343 100 0 gmosBorderC#11
xform 0 1536 1648
p 3108 472 100 512 1 File:gmOiwfsTolCad.sch
p 2516 516 150 0 1 Rev:1.0
p 2796 592 120 256 -1 Title:Receive TCS Tolerance Command
p 2836 528 100 1024 -1 author:Bob Wooff
p 2836 496 100 1024 -1 date:October 3, 2000
[comments]

[schematic2]
uniq 397
[tools]
[detail]
w 338 1355 100 0 n#395 hwin.hwin#396.in 224 1344 512 1344 ecad8.follow.INPE
w 338 1419 100 0 n#393 hwin.hwin#392.in 224 1408 512 1408 ecad8.follow.INPD
w 338 1483 100 0 n#390 hwin.hwin#391.in 224 1472 512 1472 ecad8.follow.INPC
w 338 1547 100 0 n#389 hwin.hwin#388.in 224 1536 512 1536 ecad8.follow.INPB
w 338 1611 100 0 n#387 hwin.hwin#386.in 224 1600 512 1600 ecad8.follow.INPA
w 956 1643 100 0 n#384 ecad8.follow.FLNK 832 1088 960 1088 960 2208 1120 2208 estringouts.followMess.SLNK
w 892 2011 100 0 n#383 ecad8.follow.MESS 832 1792 896 1792 896 2240 1120 2240 estringouts.followMess.DOL
w 1764 1947 100 2 n#381 hwin.hwin#337.in 1760 1952 1760 1952 eseqs.followSeq.DOL1
w 1764 1915 100 2 n#380 eseqs.followSeq.DOL2 1760 1920 1760 1920 hwin.hwin#371.in
w 2146 1451 100 0 FollowState elongouts.followLong.OUT 1376 1568 1568 1568 1568 1440 2784 1440 outhier.FollowState.p
w 2226 2219 100 0 n#377 eseqs.followSeq.LNK2 2080 1920 2176 1920 2176 2208 2336 2208 ecars.followC.IVAL
w 2098 1963 100 0 n#377 eseqs.followSeq.LNK1 2080 1952 2176 1952 junction
w 1538 1643 100 0 n#341 elongouts.followLong.FLNK 1376 1632 1760 1632 eseqs.followSeq.SLNK
w 1020 1275 100 0 n#149 ecad8.follow.STLK 832 960 1024 960 1024 1600 1120 1600 elongouts.followLong.SLNK
w 946 1635 100 0 n#139 ecad8.follow.VALA 832 1632 1120 1632 elongouts.followLong.DOL
s 1552 1920 100 2048 IDLE
s 1552 1952 100 512 BUSY
[cell use]
use hwin 32 1303 100 0 hwin#396
xform 0 128 1344
p 35 1336 100 0 -1 val(in):$(gm)cc:masterEnable.VAL
use ecad8 688 1888 100 0 follow
xform 0 672 1376
p 688 1600 100 0 -1 FTVA:LONG
p 608 1536 100 0 0 FTVB:STRING
p 608 1504 100 0 0 FTVC:STRING
p 608 1472 100 0 0 FTVD:STRING
p 576 848 100 0 1 INAM:
p 672 1888 100 512 -1 PV:$(top)
p 576 816 100 0 1 SNAM:oiwfsFollowCad
p 832 970 75 0 -1 pproc(STLK):PP
use hwin 32 1367 100 0 hwin#392
xform 0 128 1408
p 35 1400 100 0 -1 val(in):$(top)$(dev)Assembly.ASTA
use hwin 32 1431 100 0 hwin#391
xform 0 128 1472
p 35 1464 100 0 -1 val(in):$(top)$(dev)Assembly.ILCK
use hwin 32 1495 100 0 hwin#388
xform 0 128 1536
p 35 1528 100 0 -1 val(in):$(top)$(dev)Assembly.INDX
use hwin 32 1559 100 0 hwin#386
xform 0 128 1600
p 35 1592 100 0 -1 val(in):$(top)$(dev)Assembly.INIT
use hwin 1568 1879 100 0 hwin#371
xform 0 1664 1920
p 1571 1912 100 0 -1 val(in):0
use hwin 1568 1911 100 0 hwin#337
xform 0 1664 1952
p 1571 1944 100 0 -1 val(in):2
use estringouts 1280 2272 100 0 followMess
xform 0 1248 2208
p 1152 2112 100 0 1 OMSL:closed_loop
p 1280 2272 100 512 -1 PV:$(top)
p 1456 2192 100 0 -1 def(OUT):$(top)rejectMess.IMSS
use elongouts 1232 1680 100 0 followLong
xform 0 1248 1600
p 1152 1488 100 0 1 OMSL:closed_loop
p 1232 1680 100 512 -1 PV:$(top)
p 1376 1568 75 768 -1 pproc(OUT):PP
use eseqs 1936 2032 100 0 followSeq
xform 0 1920 1792
p 1872 1584 100 0 1 DLY1:0.0
p 1872 1552 100 0 1 DLY2:0.1
p 1872 1520 100 0 0 DLY3:0.0
p 1872 1488 100 0 0 DLY4:0.0
p 1920 2032 100 512 -1 PV:$(top)
p 1728 1952 75 1280 -1 pproc(DOL1):NPP
p 1728 1920 75 1280 -1 pproc(DOL2):NPP
p 2096 1952 75 1024 -1 pproc(LNK1):PP
p 2096 1920 75 1024 -1 pproc(LNK2):PP
use outhier 2752 1399 100 0 FollowState
xform 0 2768 1440
use ecars 2496 2256 100 0 followC
xform 0 2496 2096
p 2480 2256 100 512 -1 PV:$(top)
use gmosBorderC -144 343 100 0 gmosBorderC#11
xform 0 1536 1648
p 3108 472 100 512 1 File:gmOiwfsFollowCad.sch
p 2516 516 150 0 1 Rev:1.0
p 2796 592 120 256 -1 Title:Receive TCS Follow Command
p 2836 528 100 1024 -1 author:Bob Wooff
p 2836 496 100 1024 -1 date:October 3, 2000
[comments]

[schematic2]
uniq 386
[tools]
[detail]
w 2102 1963 100 0 StopDir junction 2208 1920 2208 1952 2080 1952 eseqs.stopSeq.LNK1
w 2390 1931 100 0 StopDir eseqs.stopSeq.LNK2 2080 1920 2784 1920 outhier.StopDir.p
w 930 2123 100 0 n#385 ecad2.stop.MESS 832 1792 896 1792 896 2112 1024 2112 estringouts.stopMess.DOL
w 850 1483 100 0 n#384 ecad2.stop.FLNK 832 1472 928 1472 928 2080 1024 2080 estringouts.stopMess.SLNK
w 1764 1947 100 2 n#382 hwin.hwin#381.in 1760 1952 1760 1952 eseqs.stopSeq.DOL1
w 2146 1451 100 0 StopFollowState elongouts.stopFollow.OUT 1376 1568 1568 1568 1568 1440 2784 1440 outhier.StopFollowState.p
w 1764 1915 100 2 n#376 hwin.hwin#339.in 1760 1920 1760 1920 eseqs.stopSeq.DOL2
w 1538 1643 100 0 n#341 elongouts.stopFollow.FLNK 1376 1632 1760 1632 eseqs.stopSeq.SLNK
w 898 1355 100 0 n#149 ecad2.stop.STLK 832 1344 1024 1344 1024 1600 1120 1600 elongouts.stopFollow.SLNK
w 946 1643 100 0 n#139 ecad2.stop.VALA 832 1632 1120 1632 elongouts.stopFollow.DOL
s 1552 1952 100 2048 MARK
s 1552 1920 100 2048 STOP
[cell use]
use estringouts 1200 2144 100 0 stopMess
xform 0 1152 2080
p 1056 2000 100 0 1 OMSL:closed_loop
p 1184 2144 100 512 -1 PV:$(top)
p 1360 2064 100 0 -1 def(OUT):$(top)rejectMess.IMSS
use hwin 1568 1911 100 0 hwin#381
xform 0 1664 1952
p 1571 1944 100 0 -1 val(in):0
use hwin 1568 1879 100 0 hwin#339
xform 0 1664 1920
p 1571 1912 100 0 -1 val(in):4
use elongouts 1232 1680 100 0 stopFollow
xform 0 1248 1600
p 1152 1488 100 0 1 OMSL:closed_loop
p 1216 1680 100 512 -1 PV:$(top)
p 1376 1568 75 768 -1 pproc(OUT):PP
use ecad2 720 1872 100 0 stop
xform 0 672 1568
p 752 1632 70 512 -1 FTVA:LONG
p 704 1872 100 512 -1 PV:$(top)
p 560 1216 100 0 1 SNAM:oiwfsStopCad
p 848 1600 75 1024 -1 pproc(OUTA):NPP
p 848 1344 75 1024 -1 pproc(STLK):PP
use eseqs 1936 2032 100 0 stopSeq
xform 0 1920 1792
p 1872 1584 100 0 1 DLY1:0.0
p 1872 1552 100 0 1 DLY2:0.5
p 1872 1520 100 0 0 DLY3:0.0
p 1872 1488 100 0 0 DLY4:0.0
p 1920 2032 100 512 -1 PV:$(top)
p 1728 1952 75 1280 -1 pproc(DOL1):NPP
p 1728 1920 75 1280 -1 pproc(DOL2):NPP
p 2096 1952 75 1024 -1 pproc(LNK1):PP
p 2096 1920 75 1024 -1 pproc(LNK2):PP
p 2096 1888 75 1024 -1 pproc(LNK3):NPP
p 2096 1856 75 1024 -1 pproc(LNK4):NPP
use outhier 2752 1879 100 0 StopDir
xform 0 2768 1920
use outhier 2752 1399 100 0 StopFollowState
xform 0 2768 1440
use gmosBorderC -144 343 100 0 gmosBorderC#11
xform 0 1536 1648
p 3108 472 100 512 1 File:gmOiwfsStopCad.sch
p 2516 516 150 0 1 Rev:1.0
p 2796 592 120 256 -1 Title:Receive TCS Stop Command
p 2836 528 100 1024 -1 author:Bob Wooff
p 2836 496 100 1024 -1 date:October 3, 2000
[comments]

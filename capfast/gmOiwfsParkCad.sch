[schematic2]
uniq 407
[tools]
[detail]
w 530 1483 100 0 n#405 hwin.hwin#406.in 416 1472 704 1472 ecad8.park.INPE
w 530 1547 100 0 n#403 hwin.hwin#402.in 416 1536 704 1536 ecad8.park.INPD
w 530 1611 100 0 n#400 hwin.hwin#401.in 416 1600 704 1600 ecad8.park.INPC
w 530 1675 100 0 n#399 hwin.hwin#398.in 416 1664 704 1664 ecad8.park.INPB
w 530 1739 100 0 n#397 hwin.hwin#396.in 416 1728 704 1728 ecad8.park.INPA
w 1266 2043 100 0 n#395 ecad8.park.MESS 1024 1920 1120 1920 1120 2032 1472 2032 estringouts.parkMess.DOL
w 1180 1603 100 0 n#394 ecad8.park.FLNK 1024 1216 1184 1216 1184 2000 1472 2000 estringouts.parkMess.SLNK
w 1874 1195 100 0 ParkFollowState elongouts.parkFollow.OUT 1728 1184 2080 1184 outhier.ParkFollowState.p
w 1874 1451 100 0 ParkDir elongouts.parkDir.OUT 1728 1440 2080 1440 outhier.ParkDIR.p
w 1874 1707 100 0 ParkMode elongouts.parkMode.OUT 1728 1696 2080 1696 outhier.ParkMODE.p
w 1570 1355 100 0 n#389 elongouts.parkDir.FLNK 1728 1504 1792 1504 1792 1344 1408 1344 1408 1216 1472 1216 elongouts.parkFollow.SLNK
w 1570 1611 100 0 n#388 elongouts.parkMode.FLNK 1728 1760 1792 1760 1792 1600 1408 1600 1408 1472 1472 1472 elongouts.parkDir.SLNK
w 1244 1403 100 0 n#387 ecad8.park.STLK 1024 1088 1248 1088 1248 1728 1472 1728 elongouts.parkMode.SLNK
w 1138 1643 100 0 n#386 ecad8.park.VALC 1024 1632 1312 1632 1312 1248 1472 1248 elongouts.parkFollow.DOL
w 1170 1707 100 0 n#385 ecad8.park.VALB 1024 1696 1376 1696 1376 1504 1472 1504 elongouts.parkDir.DOL
w 1218 1771 100 0 n#384 ecad8.park.VALA 1024 1760 1472 1760 elongouts.parkMode.DOL
[cell use]
use hwin 224 1431 100 0 hwin#406
xform 0 320 1472
p 227 1464 100 0 -1 val(in):$(gm)cc:masterEnable.VAL
use ecad8 896 2016 100 0 park
xform 0 864 1504
p 880 1760 100 0 -1 FTVA:LONG
p 880 1696 100 0 -1 FTVB:LONG
p 880 1632 100 0 -1 FTVC:LONG
p 768 976 100 0 1 INAM:
p 880 2016 100 512 -1 PV:$(top)
p 768 944 100 0 1 SNAM:oiwfsParkCad
p 1024 1098 75 0 -1 pproc(STLK):PP
use hwin 224 1495 100 0 hwin#402
xform 0 320 1536
p 227 1528 100 0 -1 val(in):$(top)$(dev)Assembly.ASTA
use hwin 224 1559 100 0 hwin#401
xform 0 320 1600
p 227 1592 100 0 -1 val(in):$(top)$(dev)Assembly.ILCK
use hwin 224 1623 100 0 hwin#398
xform 0 320 1664
p 227 1656 100 0 -1 val(in):$(top)$(dev)Assembly.INDX
use hwin 224 1687 100 0 hwin#396
xform 0 320 1728
p 227 1720 100 0 -1 val(in):$(top)$(dev)Assembly.INIT
use estringouts 1632 2064 100 0 parkMess
xform 0 1600 2000
p 1520 1920 100 0 1 OMSL:closed_loop
p 1616 2064 100 512 -1 PV:$(top)
p 1840 1984 100 0 -1 def(OUT):$(top)rejectMess.IMSS
use outhier 2048 1143 100 0 ParkFollowState
xform 0 2064 1184
use outhier 2048 1655 100 0 ParkMODE
xform 0 2064 1696
use outhier 2048 1399 100 0 ParkDIR
xform 0 2064 1440
use elongouts 1648 1288 100 0 parkFollow
xform 0 1600 1216
p 1520 1120 100 0 1 OMSL:closed_loop
p 1632 1296 100 512 -1 PV:$(top)
p 1728 1184 75 768 -1 pproc(OUT):PP
use elongouts 1648 1808 100 0 parkMode
xform 0 1600 1728
p 1520 1632 100 0 1 OMSL:closed_loop
p 1632 1808 100 512 -1 PV:$(top)
use elongouts 1648 1552 100 0 parkDir
xform 0 1600 1472
p 1520 1376 100 0 1 OMSL:closed_loop
p 1616 1552 100 512 -1 PV:$(top)
p 1728 1440 75 768 -1 pproc(OUT):PP
use gmosBorderC -144 343 100 0 gmosBorderC#11
xform 0 1536 1648
p 3108 472 100 512 1 File:gmOiwfsParkCad.sch
p 2516 516 150 0 1 Rev:1.0
p 2796 592 120 256 -1 Title:Receive TCS Park Command
p 2836 528 100 1024 -1 author:Bob Wooff
p 2836 496 100 1024 -1 date:October 3, 2000
[comments]

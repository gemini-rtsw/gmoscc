[schematic2]
uniq 398
[tools]
[detail]
w 1360 2112 100 0 FOLLOW elongouts.followState.VAL 896 2144 2208 2144 2208 2272 2304 2272 gmOiwfsCmd.gmOiwfsCmd#135.FollowState
w 1037 2172 100 1 FOLLOW gmOiwfsCmd.gmOiwfsCmd#133.FollowState 1120 2272 1024 2272 1024 2144 junction
w 1645 2172 100 1 FOLLOW gmOiwfsCmd.gmOiwfsCmd#134.FollowState 1728 2272 1632 2272 1632 2144 junction
w 450 2155 100 0 n#374 gmOiwfsFollowCad.gmOiwfsFollowCad#369.FollowState 448 2144 512 2144 junction
w 450 1099 100 0 n#374 gmOiwfsMoveCad.gmOiwfsMoveCad#370.MoveFollowState 448 1088 512 1088 512 2144 junction
w 450 1771 100 0 n#374 gmOiwfsStopCad.gmOiwfsStopCad#366.StopFollowState 448 1760 512 1760 junction
w 450 1419 100 0 n#374 gmOiwfsParkCad.gmOiwfsParkCad#367.ParkFollowState 448 1408 512 1408 junction
w 546 2147 100 0 n#374 junction 512 2144 640 2144 elongouts.followState.SLNK
w 602 619 100 0 CUR_POS inhier.CUR_POS.P 240 608 1024 608 1024 1248 1216 1248 gmOiwfsFollowA.gmOiwfsFollowA#385.cur_pos
w 2162 1259 100 0 Y_M gmOiwfsFollowA.gmOiwfsFollowA#385.y_m 1536 1248 2848 1248 outhier.Y_M.p
w 2018 1291 100 0 X_M gmOiwfsFollowA.gmOiwfsFollowA#385.x_m 1536 1280 2560 1280 2560 1408 2848 1408 outhier.X_M.p
w 1656 1355 100 0 MODE gmOiwfsFollowA.gmOiwfsFollowA#385.mode 1536 1344 1824 1344 1824 1536 junction
w 1624 1547 100 0 MODE gmOiwfsParkCad.gmOiwfsParkCad#367.ParkMODE 448 1536 2848 1536 outhier.MODE.p
w 1456 2288 100 0 MODE gmOiwfsCmd.gmOiwfsCmd#133.MODE 1440 2272 1472 2272 1472 1536 junction
w 2064 2288 100 0 MODE gmOiwfsCmd.gmOiwfsCmd#134.MODE 2048 2272 2080 2272 2080 1536 junction
w 2640 2288 100 0 MODE gmOiwfsCmd.gmOiwfsCmd#135.MODE 2624 2272 2656 2272 2656 1536 junction
w 1630 1387 100 0 DIR gmOiwfsFollowA.gmOiwfsFollowA#385.dir 1536 1376 1760 1376 1760 1824 junction
w 1618 1835 100 0 DIR gmOiwfsStopCad.gmOiwfsStopCad#366.StopDir 448 1824 2848 1824 outhier.DIR.p
w 558 1483 100 0 DIR gmOiwfsParkCad.gmOiwfsParkCad#367.ParkDIR 448 1472 704 1472 704 1824 junction
w 1504 2352 100 0 DIR gmOiwfsCmd.gmOiwfsCmd#133.DIR 1440 2336 1536 2336 1536 1824 junction
w 2112 2352 100 0 DIR gmOiwfsCmd.gmOiwfsCmd#134.DIR 2048 2336 2144 2336 2144 1824 junction
w 2704 2352 100 0 DIR gmOiwfsCmd.gmOiwfsCmd#135.DIR 2624 2336 2720 2336 2720 1824 junction
w 1084 1739 100 0 n#394 elongouts.followState.OUT 896 2112 1088 2112 1088 1376 1216 1376 gmOiwfsFollowA.gmOiwfsFollowA#385.follow
w 2626 1003 100 0 DEBUG gmOiwfsDebugCad.gmOiwfsDebugCad#382.DEBUG 2464 992 2848 992 outhier.DEBUG.p
w 554 843 100 0 TOLERANCE gmOiwfsFollowA.gmOiwfsFollowA#385.tolerance 1216 1280 928 1280 928 832 448 832 gmOiwfsTolCad.gmOiwfsTolCad#368.TOLERANCE
w 990 1323 100 0 Y_T gmOiwfsMoveCad.gmOiwfsMoveCad#370.Y_T 448 1152 800 1152 800 1312 1216 1312 gmOiwfsFollowA.gmOiwfsFollowA#385.y_t
w 974 1355 100 0 X_T gmOiwfsMoveCad.gmOiwfsMoveCad#370.X_T 448 1184 768 1184 768 1344 1216 1344 gmOiwfsFollowA.gmOiwfsFollowA#385.x_t
w 1410 2571 100 0 n#377 ecars.activeC.VAL 672 2560 2208 2560 2208 2336 2304 2336 gmOiwfsCmd.gmOiwfsCmd#135.ActiveState
w 1650 2347 100 0 n#377 junction 1632 2560 1632 2336 1728 2336 gmOiwfsCmd.gmOiwfsCmd#134.ActiveState
w 1042 2347 100 0 n#377 junction 1024 2560 1024 2336 1120 2336 gmOiwfsCmd.gmOiwfsCmd#133.ActiveState
s 240 2672 100 0 activeC monitors the assembly and commands
s 1872 2736 100 0 put reject messages here for testing
s 336 2640 100 0 through SNL in commandCar.stpp
[cell use]
use elongouts 800 2224 100 0 followState
xform 0 768 2144
p 480 2286 100 0 0 EGU:0/1/2
p 704 2048 100 0 1 OMSL:supervisory
p 784 2224 100 512 -1 PV:$(top)
use inhier 224 567 100 0 CUR_POS
xform 0 240 608
use outhier 2816 951 100 0 DEBUG
xform 0 2832 992
use outhier 2816 1783 100 0 DIR
xform 0 2832 1824
use outhier 2816 1495 100 0 MODE
xform 0 2832 1536
use outhier 2816 1367 100 0 X_M
xform 0 2832 1408
use outhier 2816 1207 100 0 Y_M
xform 0 2832 1248
use gmOiwfsFollowA 1216 1159 100 0 gmOiwfsFollowA#385
xform 0 1376 1312
use gmOiwfsDebugCad 2208 839 100 0 gmOiwfsDebugCad#382
xform 0 2336 976
use gmOiwfsMoveCad 192 999 100 0 gmOiwfsMoveCad#370
xform 0 320 1136
use gmOiwfsFollowCad 192 1975 100 0 gmOiwfsFollowCad#369
xform 0 320 2120
use gmOiwfsTolCad 224 679 100 0 gmOiwfsTolCad#368
xform 0 320 816
use gmOiwfsParkCad 224 1335 100 0 gmOiwfsParkCad#367
xform 0 320 1472
use gmOiwfsStopCad 224 1671 100 0 gmOiwfsStopCad#366
xform 0 320 1808
use esirs 2512 2736 100 0 rejectMess
xform 0 2512 2592
p 2240 2336 100 0 0 DESC:Wavefront Sensor reject message
p 2368 2432 100 0 1 FDSC:Wavefront Sensor reject message
p 2512 2736 100 512 -1 PV:$(top)
use gmOiwfsCmd 1144 2143 100 0 gmOiwfsCmd#133
xform 0 1288 2304
p 1280 2288 150 256 -1 seta:dev datum
use gmOiwfsCmd 1752 2143 100 0 gmOiwfsCmd#134
xform 0 1896 2304
p 1888 2288 150 256 -1 seta:dev init
use gmOiwfsCmd 2328 2143 100 0 gmOiwfsCmd#135
xform 0 2472 2304
p 2464 2304 150 256 -1 seta:dev test
use ecars 512 2608 100 0 activeC
xform 0 512 2448
p 512 2608 100 512 -1 PV:$(top)
use gmosBorderC -144 343 100 0 gmosBorderC#11
xform 0 1536 1648
p 3108 472 100 512 1 File:gmOiwfsCommands.sch
p 2516 516 150 0 1 Rev:1.0
p 2796 592 120 256 -1 Title:Receive TCS Commands
p 2836 528 100 1024 -1 author:A.Ebbers & B.Wooff
p 2836 496 100 1024 -1 date:November 2, 2000
[comments]

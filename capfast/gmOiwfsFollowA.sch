[schematic2]
uniq 302
[tools]
[detail]
w 2026 2075 100 0 n#301 elongouts.followMove.FLNK 2208 1984 2240 1984 2240 2064 1872 2064 1872 2176 1952 2176 elongouts.followStart.SLNK
w 2026 1851 100 0 n#300 eaos.followX.FLNK 2208 1760 2240 1760 2240 1840 1872 1840 1872 1952 1952 1952 elongouts.followMove.SLNK
w 2026 1627 100 0 n#299 eaos.followY.FLNK 2208 1536 2240 1536 2240 1616 1872 1616 1872 1728 1952 1728 eaos.followX.SLNK
w 2026 1403 100 0 n#298 ebos.following.FLNK 2208 1312 2240 1312 2240 1392 1872 1392 1872 1504 1952 1504 eaos.followY.SLNK
w 1890 459 100 0 n#297 egenSub.Offset.OUTJ 1568 448 2272 448 hwout.hwout#296.outp
w 2210 1259 100 0 n#293 ebos.following.OUT 2208 1248 2272 1248 hwout.hwout#292.outp
w 2128 1040 100 0 stream_valid egenSub.followA.OUTH 992 1536 1728 1536 1728 1024 2272 1024 hwout.hwout#295.outp
w 2192 1104 100 0 in_position egenSub.followA.OUTG 992 1600 1760 1600 1760 1088 2272 1088 hwout.hwout#294.outp
w 1792 1328 100 0 follow_s egenSub.followA.VALB 992 1952 1248 1952 1248 1312 1952 1312 ebos.following.DOL
w 496 1568 100 0 follow inhier.follow.P -128 1152 448 1152 448 1568 704 1568 egenSub.followA.H
w 1442 1291 100 0 n#270 egenSub.followA.FLNK 992 1280 1952 1280 ebos.following.SLNK
w 448 1904 100 0 cur_pos inhier.cur_pos.P -128 1888 704 1888 egenSub.followA.C
w 1074 1835 100 0 reject_offset egenSub.followA.VALD 992 1824 1216 1824 1216 320 1280 320 egenSub.Offset.SDIS
w 1058 1419 100 0 offset_stream egenSub.followA.OUTJ 992 1408 1184 1408 1184 1056 1280 1056 egenSub.Offset.A
w 416 288 100 0 simulated_demand_stream egenSub.tcsSimulator.OUTJ 352 320 576 320 576 1440 704 1440 egenSub.followA.J
w 514 1611 100 0 n#248 egenSub.followA.INPG 704 1600 384 1600 384 1248 224 1248 eais.PatrolYLoLim.VAL
w 512 1504 100 0 tolerance inhier.tolerance.P -128 1088 512 1088 512 1504 704 1504 egenSub.followA.I
w 2306 1483 100 0 y_m eaos.followY.OUT 2208 1472 2464 1472 outhier.y_m.p
w 2306 1707 100 0 x_m eaos.followX.OUT 2208 1696 2464 1696 outhier.x_m.p
w 498 1675 100 0 n#240 eais.PatrolYHiLim.VAL 224 1408 352 1408 352 1664 704 1664 egenSub.followA.INPF
w 482 1739 100 0 n#239 eais.PatrolXLoLim.VAL 224 1568 320 1568 320 1728 704 1728 egenSub.followA.INPE
w 466 1803 100 0 n#268 eais.PatrolXHiLim.VAL 224 1728 288 1728 288 1792 704 1792 egenSub.followA.INPD
w 1956 2203 -100 2 mask_dir hwin.hwin#279.in 1952 2208 1952 2208 elongouts.followStart.DOL
w 1808 1552 100 0 mask_y egenSub.followA.VALF 992 1696 1792 1696 1792 1536 1952 1536 eaos.followY.DOL
w 1776 1776 100 0 mask_x egenSub.followA.VALE 992 1760 1952 1760 eaos.followX.DOL
w 1956 1979 -100 2 mask_mode hwin.hwin#278.in 1952 1984 1952 1984 elongouts.followMove.DOL
w 1792 1488 100 0 reject egenSub.followA.VALA 992 2016 1280 2016 1280 1472 1952 1472 eaos.followY.SDIS
w 2306 1931 100 0 mode elongouts.followMove.OUT 2208 1920 2464 1920 outhier.mode.p
w 2306 2155 100 0 dir elongouts.followStart.OUT 2208 2144 2464 2144 outhier.dir.p
w 448 1968 100 0 y_t inhier.y_t.P -128 1952 704 1952 egenSub.followA.B
w 448 2032 100 0 x_t inhier.x_t.P -128 2016 704 2016 egenSub.followA.A
s 528 1616 100 512 y_lo_limit
s 528 1680 100 512 y_hi_limit
s 528 1744 100 512 x_lo_limit
s 528 1808 100 512 x_hi_limit
s 1088 1552 100 0 stream_valid
s 1072 1616 100 0 in_position
s 1088 1712 100 0 mask_y
s 1072 1776 100 0 mask_x
s 1072 1968 100 0 follow_s
s 1088 2032 100 0 reject
s 0 720 100 512 radius
s 0 784 100 512 period
s 0 848 100 512 Y_origin
s 0 912 100 512 X_origin
s 432 864 100 0 Y_target
s 432 928 100 0 X_target
s 1600 800 100 0 track_ID
s 1600 864 100 0 R_offset
s 1600 928 100 0 Z_offset
s 1600 992 100 0 Y_offset
s 1600 1056 100 0 X_offset
s 1056 320 100 0 reject_offset
s 1024 1040 100 0 offset_stream
s 0 656 100 512 track_id
s 512 800 100 512 track_id
s 1072 1888 100 0 new_track
s 1744 1984 100 512 MOVE
s 1744 2192 100 512 START
s 2608 1280 100 2 To Status Alarm Database
[cell use]
use hwout 2272 407 100 0 hwout#296
xform 0 2368 448
p 2272 400 100 0 -1 val(outp):$(agProbeOffset) PP NMS
use hwout 2272 983 100 0 hwout#295
xform 0 2368 1024
p 2272 976 100 0 -1 val(outp):$(top)arrayS.VAL PP NMS
use hwout 2272 1047 100 0 hwout#294
xform 0 2368 1088
p 2272 1056 100 0 -1 val(outp):$(top)inPosition.VAL PP NMS
use hwout 2272 1207 100 0 hwout#292
xform 0 2368 1248
p 2272 1200 100 0 -1 val(outp):$(top)followS.VAL PP MS
use ebos 2064 1360 100 0 following
xform 0 2080 1280
p 2000 1184 100 0 1 OMSL:closed_loop
p 2000 1120 100 0 1 ONAM:FOLLOWING
p 2064 1360 100 512 1 PV:$(top)
p 2000 1152 100 0 1 ZNAM:NOT_FOLLOWING
p 2240 1248 75 768 -1 palrm(OUT):MS
p 2208 1248 75 768 -1 pproc(OUT):PP
use hwin 1760 2167 100 0 hwin#279
xform 0 1856 2208
p 1763 2200 100 0 -1 val(in):3
use hwin 1760 1943 100 0 hwin#278
xform 0 1856 1984
p 1763 1976 100 0 -1 val(in):1
use inhier -160 2016 100 512 x_t
xform 0 -128 2016
use inhier -160 1952 100 512 y_t
xform 0 -128 1952
use inhier -144 1111 100 0 follow
xform 0 -128 1152
use inhier -144 1047 100 0 tolerance
xform 0 -128 1088
use inhier -240 1888 100 0 cur_pos
xform 0 -128 1888
use eaos 2112 1584 100 0 followY
xform 0 2080 1504
p 1952 1456 70 512 1 DISV:-1
p 1920 1230 100 0 0 EGU:millimetres
p 1984 1408 100 0 1 OMSL:closed_loop
p 2112 1584 100 512 1 PV:$(top)
use eaos 2112 1808 100 0 followX
xform 0 2080 1728
p 1952 1680 70 512 1 DISV:-1
p 1920 1454 100 0 0 EGU:millimetres
p 1984 1632 100 0 1 OMSL:closed_loop
p 2112 1808 100 512 1 PV:$(top)
use egenSub 208 976 100 0 tcsSimulator
xform 0 208 560
p -64 192 100 0 1 DISV:1
p 144 928 70 0 -1 FTA:DOUBLE
p 144 864 70 0 -1 FTB:DOUBLE
p 144 800 70 0 -1 FTC:DOUBLE
p 144 736 70 0 -1 FTD:DOUBLE
p 144 656 70 0 -1 FTE:DOUBLE
p 224 928 70 0 -1 FTVA:DOUBLE
p 224 864 70 0 -1 FTVB:DOUBLE
p 272 816 70 512 -1 FTVC:DOUBLE
p 224 320 70 0 -1 FTVJ:DOUBLE
p 272 288 70 512 1 NOVJ:6
p 64 64 100 0 1 PREC:1
p 192 976 100 512 -1 PV:$(top)
p 64 96 100 0 1 SCAN:.05 second
p 64 128 100 0 1 SNAM:oiTargetSimulator
p 352 330 75 0 -1 pproc(OUTJ):PP
use egenSub 848 2080 100 0 followA
xform 0 848 1648
p 784 2016 70 0 -1 FTA:DOUBLE
p 784 1952 70 0 -1 FTB:DOUBLE
p 784 1888 70 0 -1 FTC:DOUBLE
p 784 1824 70 0 -1 FTD:DOUBLE
p 784 1760 70 0 -1 FTE:DOUBLE
p 784 1696 70 0 -1 FTF:DOUBLE
p 784 1632 70 0 -1 FTG:DOUBLE
p 784 1568 100 0 -1 FTH:LONG
p 784 1504 100 0 -1 FTI:DOUBLE
p 784 1440 80 0 -1 FTJ:DOUBLE
p 912 2016 70 512 -1 FTVA:LONG
p 912 1952 70 512 -1 FTVB:LONG
p 912 1888 70 512 -1 FTVC:LONG
p 912 1824 70 512 -1 FTVD:LONG
p 912 1760 70 512 -1 FTVE:DOUBLE
p 912 1696 70 512 -1 FTVF:DOUBLE
p 912 1632 70 512 -1 FTVG:LONG
p 864 1584 80 0 -1 FTVH:LONG
p 912 1504 70 512 -1 FTVI:LONG
p 912 1440 70 512 -1 FTVJ:DOUBLE
p 800 1856 100 0 -1 NOC:4
p 800 1408 100 0 -1 NOJ:6
p 896 1408 100 0 -1 NOVJ:9
p 736 1168 100 0 1 PREC:2
p 832 2080 100 512 -1 PV:$(top)
p 848 1264 70 256 1 SCAN:Passive
p 816 1200 100 256 1 SNAM:oiFollowA
p 992 1930 75 0 -1 pproc(OUTB):NPP
p 992 1738 75 0 -1 pproc(OUTE):NPP
p 992 1674 75 0 -1 pproc(OUTF):NPP
p 992 1610 75 0 -1 pproc(OUTG):PP
p 992 1546 75 0 -1 pproc(OUTH):PP
p 992 1482 75 0 -1 pproc(OUTI):NPP
p 992 1418 75 0 -1 pproc(OUTJ):PP
use egenSub 1456 1120 100 0 Offset
xform 0 1424 688
p 1360 320 100 0 1 DISV:-1
p 1360 1024 70 0 -1 FTA:DOUBLE
p 1488 1056 70 512 -1 FTVA:DOUBLE
p 1488 992 70 512 -1 FTVB:DOUBLE
p 1488 928 70 512 -1 FTVC:DOUBLE
p 1488 864 70 512 -1 FTVD:DOUBLE
p 1488 800 70 512 -1 FTVE:DOUBLE
p 1488 480 70 512 -1 FTVJ:DOUBLE
p 1360 1056 70 0 1 NOA:9
p 1488 448 70 512 1 NOVJ:9
p 992 814 100 0 0 PREC:2
p 1440 1120 100 512 -1 PV:$(top)$(dev)
p 1312 240 100 0 1 SNAM:oiProbeOffset
p 1648 464 100 0 0 def(OUTJ):0.0
p 1568 458 75 0 -1 pproc(OUTJ):PP
use eais 128 1328 100 0 PatrolYLoLim
xform 0 96 1264
p -288 1006 100 0 0 EGU:millimetres
p 96 1232 100 256 1 PREC:3
p 128 1328 100 512 1 PV:$(top)$(dev)
use eais 128 1488 100 0 PatrolYHiLim
xform 0 96 1424
p -288 1166 100 0 0 EGU:millimetres
p 96 1392 100 256 1 PREC:3
p 128 1488 100 512 1 PV:$(top)$(dev)
use eais 128 1648 100 0 PatrolXLoLim
xform 0 96 1584
p -288 1326 100 0 0 EGU:millimetres
p 96 1552 100 256 1 PREC:3
p 128 1648 100 512 1 PV:$(top)$(dev)
use eais 128 1808 100 0 PatrolXHiLim
xform 0 96 1744
p -288 1486 100 0 0 EGU:millimetres
p 96 1712 100 256 1 PREC:3
p 128 1808 100 512 1 PV:$(top)$(dev)
use elongouts 2112 2032 100 0 followMove
xform 0 2080 1952
p 1952 1904 70 512 1 DISV:-1
p 1792 2094 100 0 0 EGU:mode
p 1984 1856 100 0 1 OMSL:closed_loop
p 2112 2032 100 512 1 PV:$(top)
use elongouts 2112 2256 100 0 followStart
xform 0 2080 2176
p 1952 2128 70 512 1 DISV:-1
p 1792 2318 100 0 0 EGU:directive
p 1984 2080 100 0 1 OMSL:closed_loop
p 2112 2256 100 512 1 PV:$(top)
p 2208 2144 75 768 -1 pproc(OUT):PP
use outhier 2496 1920 100 0 mode
xform 0 2448 1920
use outhier 2496 1696 100 0 x_m
xform 0 2448 1696
use outhier 2496 1472 100 0 y_m
xform 0 2448 1472
use outhier 2496 2144 100 0 dir
xform 0 2448 2144
use gmosBorderC -416 -153 100 0 gmosBorderC#91
xform 0 1264 1152
p 2840 -24 100 512 1 File:gmOiwfsFollowA.sch
p 1216 1088 100 0 0 IO:
p 2532 160 120 256 -1 Project:Gemini Multi-object Optical Spectrometer
p 2244 20 150 0 1 Rev:
p 2524 96 120 256 -1 Title:OIWFS Calculate Angles
p 2564 32 100 1024 -1 author:B.Wooff, A. Ebbers
p 2564 0 100 1024 -1 date:November 3, 2000
p 1152 1056 100 0 0 model:
p 1152 1024 100 0 0 revision:
[comments]

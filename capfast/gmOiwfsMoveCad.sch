[schematic2]
uniq 388
[tools]
[detail]
w 1682 2043 100 0 n#377 eaos.moveYout.FLNK 1856 2208 1920 2208 1920 2032 1504 2032 1504 1920 1600 1920 elongouts.moveFollow.SLNK
w 1682 2283 100 0 n#376 eaos.moveXout.FLNK 1856 2464 1920 2464 1920 2272 1504 2272 1504 2176 1600 2176 eaos.moveYout.SLNK
w 1388 1627 100 0 n#375 ecad20.move.STLK 1072 832 1392 832 1392 2432 1600 2432 eaos.moveXout.SLNK
w 1990 1899 100 0 MoveFollowState elongouts.moveFollow.OUT 1856 1888 2304 1888 outhier.MoveFollowState.p
w 1196 1803 100 0 n#329 ecad20.move.FLNK 1072 960 1200 960 1200 2656 1600 2656 estringouts.moveMess.SLNK
w 1338 2691 100 0 n#327 estringouts.moveMess.DOL 1600 2688 1136 2688 1136 2432 1072 2432 ecad20.move.MESS
w 1860 2635 100 2 n#324 hwout.hwout#323.outp 1856 2640 1856 2640 estringouts.moveMess.OUT
w 2062 2411 100 0 x_t outhier.X_T.p 2304 2400 1856 2400 eaos.moveXout.OUT
w 2062 2155 100 0 y_t outhier.Y_T.p 2304 2144 1856 2144 eaos.moveYout.OUT
w 1434 1955 100 0 n#248 ecad20.move.VALC 1072 2144 1328 2144 1328 1952 1600 1952 elongouts.moveFollow.DOL
w 1306 2211 100 0 n#372 ecad20.move.VALB 1072 2208 1600 2208 eaos.moveYout.DOL
w 1394 2467 100 0 n#374 ecad20.move.VALA 1072 2272 1248 2272 1248 2464 1600 2464 eaos.moveXout.DOL
s 640 2192 100 512 Y Target
s 640 2240 100 512 X Target
[cell use]
use ecad20 944 2528 100 0 move
xform 0 912 1632
p 928 2272 100 0 -1 FTVA:DOUBLE
p 928 2208 100 0 -1 FTVB:DOUBLE
p 944 2144 100 0 -1 FTVC:LONG
p 944 2080 100 0 -1 FTVD:LONG
p 816 720 100 0 1 INAM:
p 912 2528 100 512 -1 PV:$(top)
p 816 688 100 0 1 SNAM:oiwfsMoveCad
p 240 2112 100 0 1 def(INPC):$(top)$(dev)PatrolXLoLim
p 240 2048 100 0 1 def(INPD):$(top)$(dev)PatrolXHiLim
p 224 1984 100 0 1 def(INPE):$(top)$(dev)PatrolYLoLim
p 240 1920 100 0 1 def(INPF):$(top)$(dev)PatrolYHiLim
p 224 1856 100 0 1 def(INPG):$(top)$(dev)Assembly.INIT
p 224 1792 100 0 1 def(INPH):$(top)$(dev)Assembly.INDX
p 224 1728 100 0 1 def(INPI):$(top)$(dev)Assembly.ILCK
p 224 1664 100 0 1 def(INPJ):$(top)$(dev)Assembly.ASTA
p 224 1600 100 0 1 def(INPK):$(gm)cc:masterEnable.VAL
p 368 1536 100 0 0 def(INPL):0.0
p 352 1472 100 0 0 def(INPM):0.0
p 352 1472 100 0 0 def(INPN):0.0
p 352 1344 100 0 0 def(INPO):0.0
p 352 1280 100 0 0 def(INPP):0.0
p 320 1248 100 0 0 def(INPQ):0.0
p 336 1152 100 0 0 def(INPR):0.0
p 304 1104 100 0 0 def(INPS):0.0
p 352 1056 100 0 0 def(INPT):0.0
p 1072 842 75 0 -1 pproc(STLK):PP
use elongouts 1720 2000 100 0 moveFollow
xform 0 1728 1920
p 1632 1824 100 0 1 OMSL:closed_loop
p 1712 2000 80 512 -1 PV:$(top)
p 1856 1888 75 768 -1 pproc(OUT):PP
use outhier 2272 1847 100 0 MoveFollowState
xform 0 2288 1888
use outhier 2272 2359 100 0 X_T
xform 0 2288 2400
use outhier 2272 2103 100 0 Y_T
xform 0 2288 2144
use hwout 1856 2599 100 0 hwout#323
xform 0 1952 2640
p 1936 2592 100 0 -1 val(outp):$(top)rejectMess.IMSS NPP NMS
use estringouts 1760 2720 100 0 moveMess
xform 0 1728 2656
p 1632 2576 100 0 1 OMSL:closed_loop
p 1744 2720 100 512 -1 PV:$(top)
p 1856 2640 75 768 -1 pproc(OUT):NPP
use eaos 1728 2512 100 0 moveXout
xform 0 1728 2432
p 1632 2336 100 0 1 OMSL:closed_loop
p 1728 2512 80 512 -1 PV:$(top)
use eaos 1732 2248 100 0 moveYout
xform 0 1728 2176
p 1632 2080 100 0 1 OMSL:closed_loop
p 1728 2240 80 512 -1 PV:$(top)
p 1856 2144 75 768 -1 pproc(OUT):PP
use gmosBorderC -144 343 100 0 gmosBorderC#11
xform 0 1536 1648
p 3108 472 100 512 1 File:gmOiwfsMoveCad.sch
p 2516 516 150 0 1 Rev:1.0
p 2796 592 120 256 -1 Title:Receive TCS Move Command
p 2836 528 100 1024 -1 author:Bob Wooff & Steven Beard
p 2836 496 100 1024 -1 date:October 3, 2000
[comments]

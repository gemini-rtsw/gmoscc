[schematic2]
uniq 213
[tools]
[detail]
w 562 -1381 100 0 n#211 hwin.hwin#212.in 480 -1392 704 -1392 esirs.MotorSteps.INP
w -590 -405 100 0 n#205 hwin.hwin#206.in -672 -416 -448 -416 esirs.Debug.INP
w -1486 -885 100 0 n#201 hwin.hwin#202.in -1568 -896 -1344 -896 esirs.Update.INP
w 1650 -885 100 0 n#195 hwin.hwin#196.in 1568 -896 1792 -896 esirs.Rotation.INP
w 530 -885 100 0 n#194 hwin.hwin#193.in 448 -896 672 -896 esirs.TiltD.INP
w 530 -405 100 0 n#191 hwin.hwin#192.in 448 -416 672 -416 esirs.TiltC.INP
w 530 75 100 0 n#190 hwin.hwin#189.in 448 64 672 64 esirs.TiltB.INP
w 530 555 100 0 n#187 hwin.hwin#188.in 448 544 672 544 esirs.TiltA.INP
w -590 -885 100 0 n#181 hwin.hwin#179.in -672 -896 -448 -896 esirs.Simulate.INP
w -1486 555 100 0 n#156 hwin.hwin#158.in -1568 544 -1344 544 esirs.Init.INP
w -1486 75 100 0 n#155 hwin.hwin#159.in -1568 64 -1344 64 esirs.Index.INP
w -1486 -405 100 0 n#154 hwin.hwin#160.in -1568 -416 -1344 -416 esirs.Park.INP
w -590 75 100 0 n#153 hwin.hwin#161.in -672 64 -448 64 esirs.Health.INP
w -590 555 100 0 n#152 hwin.hwin#162.in -672 544 -448 544 esirs.State.INP
s 1312 64 100 0 Written by assembly record --->
s 1312 544 100 0 Written by assembly record --->
s 1152 -416 100 0 Written by grating switch genSub record --->
[cell use]
use hwin -864 -457 100 0 hwin#206
xform 0 -768 -416
p -861 -424 100 0 -1 val(in):$(top)$(dev)Assembly.DBUG
use hwin -1760 503 100 0 hwin#158
xform 0 -1664 544
p -1757 536 100 0 -1 val(in):$(top)$(dev)Assembly.INIT
use hwin -1760 23 100 0 hwin#159
xform 0 -1664 64
p -1757 56 100 0 -1 val(in):$(top)$(dev)Assembly.INDX
use hwin -1760 -457 100 0 hwin#160
xform 0 -1664 -416
p -1757 -424 100 0 -1 val(in):$(top)$(dev)Assembly.PARK
use hwin -864 23 100 0 hwin#161
xform 0 -768 64
p -861 56 100 0 -1 val(in):$(top)$(dev)Assembly.HLTH
use hwin -864 503 100 0 hwin#162
xform 0 -768 544
p -861 536 100 0 -1 val(in):$(top)$(dev)Assembly.ASTA
use hwin -864 -937 100 0 hwin#179
xform 0 -768 -896
p -861 -904 100 0 -1 val(in):$(top)$(dev)Assembly.SIMM
use hwin 256 503 100 0 hwin#188
xform 0 352 544
p 259 536 100 0 -1 val(in):$(top)$(dev)ADevice.MPOS
use hwin 256 23 100 0 hwin#189
xform 0 352 64
p 259 56 100 0 -1 val(in):$(top)$(dev)BDevice.MPOS
use hwin 256 -457 100 0 hwin#192
xform 0 352 -416
p 259 -424 100 0 -1 val(in):$(top)$(dev)CDevice.MPOS
use hwin 256 -937 100 0 hwin#193
xform 0 352 -896
p 259 -904 100 0 -1 val(in):$(top)$(dev)DDevice.MPOS
use hwin 1376 -937 100 0 hwin#196
xform 0 1472 -896
p 1379 -904 100 0 -1 val(in):$(top)$(dev)tDevice.MPOS
use hwin -1760 -937 100 0 hwin#202
xform 0 -1664 -896
p -1757 -904 100 0 -1 val(in):$(top)$(dev)Assembly.INIT
use hwin 288 -1433 100 0 hwin#212
xform 0 384 -1392
p 291 -1400 100 0 -1 val(in):$(top)$(dev)Assembly.VALC
use esirs 1792 -1145 100 0 Rotation
xform 0 2000 -992
p 1856 -1216 100 0 1 DESC:Rotation of turret
p 2048 -1248 100 0 1 EGU:indexer teeth
p 1856 -1216 100 0 0 FDSC:Rotation of turret
p 1856 -1248 100 0 1 FTVL:DOUBLE
p 2272 -1248 100 0 1 PREC:3
p 1856 -1280 100 0 1 PV:$(sadtop)$(dev)
p 1856 -1184 100 0 1 SCAN:1 second
use esirs 1792 295 100 0 Tilt
xform 0 2000 448
p 1856 224 100 0 1 DESC:Tilt angle of grating in beam
p 2048 192 100 0 1 EGU:degrees
p 1856 224 100 0 0 FDSC:Tilt angle of grating in beam
p 1856 192 100 0 1 FTVL:DOUBLE
p 2208 192 100 0 1 PREC:4
p 1856 160 100 0 1 PV:$(sadtop)$(dev)
p 1856 256 100 0 1 SCAN:Passive
use esirs 1792 -185 100 0 ID
xform 0 2000 -32
p 1856 -256 100 0 1 DESC:ID of grating in beam
p 2048 -288 100 0 1 EGU:barcode ID
p 1856 -256 100 0 0 FDSC:ID of grating in beam
p 1856 -288 100 0 1 FTVL:LONG
p 1856 -320 100 0 1 PV:$(sadtop)$(dev)
p 1856 -224 100 0 1 SCAN:Passive
use esirs 672 295 100 0 TiltA
xform 0 880 448
p 736 224 100 0 1 DESC:Grating A tilt position
p 928 192 100 0 1 EGU:motor steps
p 736 224 100 0 0 FDSC:Grating A tilt position
p 736 192 100 0 1 FTVL:DOUBLE
p 1120 192 100 0 1 PREC:1
p 736 160 100 0 1 PV:$(sadtop)$(dev)
p 736 256 100 0 1 SCAN:1 second
use esirs 672 -185 100 0 TiltB
xform 0 880 -32
p 736 -256 100 0 1 DESC:Grating B tilt position
p 928 -288 100 0 1 EGU:motor steps
p 736 -256 100 0 0 FDSC:Grating B tilt position
p 736 -288 100 0 1 FTVL:DOUBLE
p 1120 -288 100 0 1 PREC:1
p 736 -320 100 0 1 PV:$(sadtop)$(dev)
p 736 -208 100 0 1 SCAN:1 second
use esirs 672 -665 100 0 TiltC
xform 0 880 -512
p 736 -736 100 0 1 DESC:Grating C tilt position
p 928 -768 100 0 1 EGU:motor steps
p 736 -736 100 0 0 FDSC:Grating C tilt position
p 736 -768 100 0 1 FTVL:DOUBLE
p 1120 -768 100 0 1 PREC:1
p 736 -800 100 0 1 PV:$(sadtop)$(dev)
p 736 -704 100 0 1 SCAN:1 second
use esirs 672 -1145 100 0 TiltD
xform 0 880 -992
p 736 -1216 100 0 1 DESC:Grating D tilt position
p 928 -1248 100 0 1 EGU:motor steps
p 736 -1216 100 0 0 FDSC:Grating D tilt position
p 736 -1248 100 0 1 FTVL:DOUBLE
p 1120 -1248 100 0 1 PREC:1
p 736 -1280 100 0 1 PV:$(sadtop)$(dev)
p 736 -1168 100 0 1 SCAN:1 second
use esirs -1344 -1145 100 0 Update
xform 0 -1136 -992
p -1280 -1216 100 0 1 DESC:Grating characterized?
p -1088 -1248 100 0 1 EGU:0/1
p -1280 -1216 100 0 0 FDSC:Grating characterized?
p -1280 -1248 100 0 1 FTVL:LONG
p -1280 -1280 100 0 1 PV:$(sadtop)$(dev)
p -1280 -1184 100 0 1 SCAN:1 second
use esirs -448 295 100 0 State
xform 0 -240 448
p -384 224 100 0 1 DESC:Grating state
p -384 224 100 0 0 FDSC:Grating state
p -384 192 100 0 1 FTVL:STRING
p -384 160 100 0 1 PV:$(sadtop)$(dev)
p -384 256 100 0 1 SCAN:1 second
use esirs -1344 295 100 0 Init
xform 0 -1136 448
p -1280 224 100 0 1 DESC:Grating initialized?
p -1088 192 100 0 1 EGU:0/1
p -1280 224 100 0 0 FDSC:Grating initialized?
p -1280 192 100 0 1 FTVL:LONG
p -1280 160 100 0 1 PV:$(sadtop)$(dev)
p -1280 256 100 0 1 SCAN:1 second
use esirs -1344 -185 100 0 Index
xform 0 -1136 -32
p -1280 -256 100 0 1 DESC:Grating indexed?
p -1088 -288 100 0 1 EGU:0/1
p -1280 -256 100 0 0 FDSC:Grating indexed?
p -1280 -288 100 0 1 FTVL:LONG
p -1280 -320 100 0 1 PV:$(sadtop)$(dev)
p -1280 -224 100 0 1 SCAN:1 second
use esirs -1344 -665 100 0 Park
xform 0 -1136 -512
p -1280 -736 100 0 1 DESC:Grating parked?
p -1088 -768 100 0 1 EGU:0/1
p -1280 -736 100 0 0 FDSC:Grating parked?
p -1280 -768 100 0 1 FTVL:LONG
p -1280 -800 100 0 1 PV:$(sadtop)$(dev)
p -1280 -704 100 0 1 SCAN:1 second
use esirs -448 -185 100 0 Health
xform 0 -240 -32
p -384 -256 100 0 1 DESC:Grating health
p -384 -256 100 0 0 FDSC:Grating health
p -384 -288 100 0 1 FTVL:STRING
p -384 -320 100 0 1 PV:$(sadtop)$(dev)
p -384 -224 100 0 1 SCAN:1 second
use esirs -448 -665 100 0 Debug
xform 0 -240 -512
p -384 -736 100 0 1 DESC:Grating debug mode
p -384 -736 100 0 0 FDSC:Grating debug mode
p -384 -768 100 0 1 FTVL:STRING
p -384 -800 100 0 1 PV:$(sadtop)$(dev)
p -384 -704 100 0 1 SCAN:1 second
use esirs -448 -1145 100 0 Simulate
xform 0 -240 -992
p -384 -1216 100 0 1 DESC:Grating simulation mode
p -384 -1216 100 0 0 FDSC:Grating simulation mode
p -384 -1248 100 0 1 FTVL:STRING
p -384 -1280 100 0 1 PV:$(sadtop)$(dev)
p -384 -1184 100 0 1 SCAN:1 second
use esirs 1792 -665 100 0 InBeam
xform 0 2000 -512
p 1856 -736 100 0 1 DESC:Grating in beam?
p 2048 -768 100 0 1 EGU:0/1
p 1856 -736 100 0 0 FDSC:Grating in beam?
p 1856 -768 100 0 1 FTVL:LONG
p 1856 -800 100 0 1 PV:$(sadtop)$(dev)
p 1856 -704 100 0 1 SCAN:Passive
use esirs 704 -1641 100 0 MotorSteps
xform 0 912 -1488
p 768 -1712 100 0 1 DESC:Motorsteps before forwardlash
p 960 -1744 100 0 1 EGU:motor steps
p 768 -1712 100 0 0 FDSC:Motorsteps before forwardlash
p 768 -1744 100 0 1 FTVL:DOUBLE
p 1152 -1744 100 0 1 PREC:4
p 768 -1776 100 0 1 PV:$(sadtop)$(dev)
p 768 -1664 100 0 1 SCAN:1 second
use ukatcBorderD -2192 -2265 100 0 ukatcBorderD#177
xform 0 448 -560
p 1520 -2128 100 0 -1 ID:$Id$
p 2676 -1952 120 256 -1 Project:Gemini Multi-object Optical Spectrograph
p 768 -2128 150 0 -1 Rev:$Revision$
p 2668 -2016 120 256 -1 Title:Grating assembly SAD
p 2384 -2080 100 768 -1 author:S M Beard
p 2384 -2112 100 768 -1 date:$Date$
[comments]

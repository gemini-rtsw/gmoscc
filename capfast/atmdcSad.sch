[schematic2]
uniq 161
[tools]
[detail]
w 914 -117 100 0 n#160 hwin.hwin#159.in 832 -128 1056 -128 esirs.ExpAngle.INP
w 914 363 100 0 n#157 hwin.hwin#158.in 832 352 1056 352 esirs.EnpAngle.INP
w -878 363 100 0 n#155 hwin.hwin#156.in -960 352 -736 352 esirs.Present.INP
w 18 -1045 100 0 n#153 hwin.hwin#152.in -64 -1056 160 -1056 esirs.Simulate.INP
w 18 -597 100 0 n#150 hwin.hwin#151.in -64 -608 160 -608 esirs.Debug.INP
w 18 363 100 0 n#143 hwin.hwin#145.in -64 352 160 352 esirs.State.INP
w 18 -117 100 0 n#142 hwin.hwin#144.in -64 -128 160 -128 esirs.Health.INP
w -878 -1077 100 0 n#141 hwin.hwin#148.in -960 -1088 -736 -1088 esirs.Park.INP
w -878 -597 100 0 n#140 hwin.hwin#147.in -960 -608 -736 -608 esirs.Index.INP
w -878 -117 100 0 n#139 hwin.hwin#146.in -960 -128 -736 -128 esirs.Init.INP
[cell use]
use hwin -1152 311 100 0 hwin#156
xform 0 -1056 352
p -1152 384 100 0 -1 val(in):$(top)$(dev)Present
use hwin -256 -1097 100 0 hwin#152
xform 0 -160 -1056
p -256 -1024 100 0 -1 val(in):$(top)$(dev)Assembly.SIMM
use hwin -256 -649 100 0 hwin#151
xform 0 -160 -608
p -256 -576 100 0 -1 val(in):$(top)$(dev)Assembly.DBUG
use hwin -256 -169 100 0 hwin#144
xform 0 -160 -128
p -256 -96 100 0 -1 val(in):$(top)$(dev)Assembly.HLTH
use hwin -256 311 100 0 hwin#145
xform 0 -160 352
p -256 384 100 0 -1 val(in):$(top)$(dev)Assembly.ASTA
use hwin -1152 -169 100 0 hwin#146
xform 0 -1056 -128
p -1152 -96 100 0 -1 val(in):$(top)$(dev)Assembly.INIT
use hwin -1152 -649 100 0 hwin#147
xform 0 -1056 -608
p -1152 -576 100 0 -1 val(in):$(top)$(dev)Assembly.INDX
use hwin -1152 -1129 100 0 hwin#148
xform 0 -1056 -1088
p -1152 -1056 100 0 -1 val(in):$(top)$(dev)Assembly.PARK
use hwin 640 311 100 0 hwin#158
xform 0 736 352
p 640 384 100 0 -1 val(in):$(top)$(dev)EnpDevice.MPOS
use hwin 640 -169 100 0 hwin#159
xform 0 736 -128
p 640 -96 100 0 -1 val(in):$(top)$(dev)ExpDevice.MPOS
use esirs 416 400 100 0 State
xform 0 368 256
p 224 32 100 0 1 DESC:AtmDC state
p 224 32 100 0 0 FDSC:AtmDC state
p 224 0 100 0 1 FTVL:STRING
p 416 400 100 512 -1 PV:$(sadtop)$(dev)
p 224 64 100 0 1 SCAN:1 second
use esirs -480 -80 100 0 Init
xform 0 -528 -224
p -672 -448 100 0 1 DESC:AtmDC initialized?
p -480 -480 100 0 1 EGU:0/1
p -672 -448 100 0 0 FDSC:AtmDC initialized?
p -672 -480 100 0 1 FTVL:LONG
p -480 -80 100 512 -1 PV:$(sadtop)$(dev)
p -672 -416 100 0 1 SCAN:1 second
use esirs -480 -560 100 0 Index
xform 0 -528 -704
p -672 -928 100 0 1 DESC:AtmDC indexed?
p -480 -960 100 0 1 EGU:0/1
p -672 -928 100 0 0 FDSC:AtmDC indexed?
p -672 -960 100 0 1 FTVL:LONG
p -480 -560 100 512 -1 PV:$(sadtop)$(dev)
p -672 -896 100 0 1 SCAN:1 second
use esirs -480 -1040 100 0 Park
xform 0 -528 -1184
p -672 -1408 100 0 1 DESC:AtmDC parked?
p -480 -1440 100 0 1 EGU:0/1
p -672 -1408 100 0 0 FDSC:AtmDC parked?
p -672 -1440 100 0 1 FTVL:LONG
p -480 -1040 100 512 -1 PV:$(sadtop)$(dev)
p -672 -1376 100 0 1 SCAN:1 second
use esirs 416 -80 100 0 Health
xform 0 368 -224
p 224 -448 100 0 1 DESC:AtmDC health
p 224 -448 100 0 0 FDSC:AtmDC health
p 224 -480 100 0 1 FTVL:STRING
p 416 -80 100 512 -1 PV:$(sadtop)$(dev)
p 224 -416 100 0 1 SCAN:1 second
use esirs 1312 400 100 0 EnpAngle
xform 0 1264 256
p 1120 64 100 0 1 DESC:AtmDC entrance prism angle
p 1312 32 100 0 1 EGU:degrees
p 1120 32 100 0 0 FDSC:AtmDC entrance prism angle
p 1120 32 100 0 1 FTVL:DOUBLE
p 1120 0 100 0 1 PREC:3
p 1312 400 100 512 -1 PV:$(sadtop)$(dev)
p 1120 96 100 0 1 SCAN:1 second
use esirs 1312 -80 100 0 ExpAngle
xform 0 1264 -224
p 1120 -416 100 0 1 DESC:AtmDC exit prism angle
p 1312 -448 100 0 1 EGU:degrees
p 1120 -448 100 0 0 FDSC:AtmDC exit prism angle
p 1120 -448 100 0 1 FTVL:DOUBLE
p 1120 -480 100 0 1 PREC:3
p 1312 -80 100 512 -1 PV:$(sadtop)$(dev)
p 1120 -384 100 0 1 SCAN:1 second
use esirs 416 -560 100 0 Debug
xform 0 368 -704
p 224 -928 100 0 1 DESC:AtmDC debug mode
p 224 -928 100 0 0 FDSC:AtmDC debug mode
p 224 -960 100 0 1 FTVL:STRING
p 416 -560 100 512 -1 PV:$(sadtop)$(dev)
p 224 -896 100 0 1 SCAN:1 second
use esirs 416 -1008 100 0 Simulate
xform 0 368 -1152
p 224 -1376 100 0 1 DESC:AtmDC simulation mode
p 224 -1376 100 0 0 FDSC:AtmDC simulation mode
p 224 -1408 100 0 1 FTVL:STRING
p 416 -1008 100 512 -1 PV:$(sadtop)$(dev)
p 224 -1344 100 0 1 SCAN:1 second
use esirs -480 400 100 0 Present
xform 0 -528 256
p -672 32 100 0 1 DESC:AtmDC installed?
p -480 0 100 0 1 EGU:0/1
p -672 32 100 0 0 FDSC:AtmDC installed?
p -672 0 100 0 1 FTVL:LONG
p -480 400 100 512 -1 PV:$(sadtop)$(dev)
p -672 64 100 0 1 SCAN:1 second
use ukatcBorderC -1424 -1833 100 0 ukatcBorderC#89
xform 0 256 -528
p 1516 -1584 120 256 -1 Title:AtmDC assembly SAD
p 1556 -1648 100 1024 -1 author:S M Beard
p 1556 -1680 100 1024 -1 date:$Date$
[comments]

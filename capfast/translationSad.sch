[schematic2]
uniq 154
[tools]
[detail]
w -142 -853 100 0 n#153 hwin.hwin#152.in -224 -864 0 -864 esirs.Simulate.INP
w -142 -405 100 0 n#150 hwin.hwin#151.in -224 -416 0 -416 esirs.Debug.INP
w -142 555 100 0 n#143 hwin.hwin#145.in -224 544 0 544 esirs.State.INP
w -142 75 100 0 n#142 hwin.hwin#144.in -224 64 0 64 esirs.Health.INP
w -1038 -405 100 0 n#141 hwin.hwin#148.in -1120 -416 -896 -416 esirs.Park.INP
w -1038 75 100 0 n#140 hwin.hwin#147.in -1120 64 -896 64 esirs.Index.INP
w -1038 555 100 0 n#139 hwin.hwin#146.in -1120 544 -896 544 esirs.Init.INP
w 754 -405 100 0 n#135 hwin.hwin#136.in 672 -416 896 -416 esirs.Z.INP
w 754 75 100 0 n#134 hwin.hwin#133.in 672 64 896 64 esirs.Y.INP
w 754 555 100 0 n#132 hwin.hwin#129.in 672 544 896 544 esirs.X.INP
[cell use]
use hwin -1312 -457 100 0 hwin#148
xform 0 -1216 -416
p -1309 -424 100 0 -1 val(in):$(top)$(dev)Assembly.PARK
use hwin -1312 23 100 0 hwin#147
xform 0 -1216 64
p -1309 56 100 0 -1 val(in):$(top)$(dev)Assembly.INDX
use hwin -1312 503 100 0 hwin#146
xform 0 -1216 544
p -1309 536 100 0 -1 val(in):$(top)$(dev)Assembly.INIT
use hwin -416 503 100 0 hwin#145
xform 0 -320 544
p -413 536 100 0 -1 val(in):$(top)$(dev)Assembly.ASTA
use hwin -416 23 100 0 hwin#144
xform 0 -320 64
p -413 56 100 0 -1 val(in):$(top)$(dev)Assembly.HLTH
use hwin 480 -457 100 0 hwin#136
xform 0 576 -416
p 483 -424 100 0 -1 val(in):$(top)$(dev)3Device.MPOS
use hwin 480 23 100 0 hwin#133
xform 0 576 64
p 483 56 100 0 -1 val(in):$(top)$(dev)2Device.MPOS
use hwin 480 503 100 0 hwin#129
xform 0 576 544
p 483 536 100 0 -1 val(in):$(top)$(dev)1Device.MPOS
use hwin -416 -457 100 0 hwin#151
xform 0 -320 -416
p -413 -424 100 0 -1 val(in):$(top)$(dev)Assembly.DBUG
use hwin -416 -905 100 0 hwin#152
xform 0 -320 -864
p -413 -872 100 0 -1 val(in):$(top)$(dev)Assembly.SIMM
use esirs 0 -1113 100 0 Simulate
xform 0 208 -960
p 64 -1184 100 0 1 DESC:Detector translation simulation mode
p 64 -1184 100 0 0 FDSC:Detector translation simulation mode
p 64 -1216 100 0 1 FTVL:STRING
p 64 -1248 100 0 1 PV:$(sadtop)$(dev)
p 64 -1152 100 0 1 SCAN:1 second
use esirs 0 -665 100 0 Debug
xform 0 208 -512
p 64 -736 100 0 1 DESC:Detector translation debug mode
p 64 -736 100 0 0 FDSC:Detector translation debug mode
p 64 -768 100 0 1 FTVL:STRING
p 64 -800 100 0 1 PV:$(sadtop)$(dev)
p 64 -704 100 0 1 SCAN:1 second
use esirs 896 -665 100 0 Z
xform 0 1104 -512
p 960 -736 100 0 1 DESC:Detector translation Z position
p 1152 -768 100 0 1 EGU:microns
p 960 -736 100 0 0 FDSC:Detector translation Z position
p 960 -768 100 0 1 FTVL:DOUBLE
p 1424 -720 100 0 1 PREC:3
p 960 -800 100 0 1 PV:$(sadtop)$(dev)
p 960 -704 100 0 1 SCAN:1 second
use esirs 896 -185 100 0 Y
xform 0 1104 -32
p 960 -256 100 0 1 DESC:Detector translation Y position
p 1152 -288 100 0 1 EGU:microns
p 960 -256 100 0 0 FDSC:Detector translation Y position
p 960 -288 100 0 1 FTVL:DOUBLE
p 1424 -256 100 0 1 PREC:3
p 960 -320 100 0 1 PV:$(sadtop)$(dev)
p 960 -208 100 0 1 SCAN:1 second
use esirs 896 295 100 0 X
xform 0 1104 448
p 960 224 100 0 1 DESC:Detector translation X position
p 1152 192 100 0 1 EGU:microns
p 960 224 100 0 0 FDSC:Detector translation X position
p 960 192 100 0 1 FTVL:DOUBLE
p 1424 224 100 0 1 PREC:3
p 960 160 100 0 1 PV:$(sadtop)$(dev)
p 960 256 100 0 1 SCAN:1 second
use esirs 0 -185 100 0 Health
xform 0 208 -32
p 64 -256 100 0 1 DESC:Detector translation health
p 64 -256 100 0 0 FDSC:Detector translation health
p 64 -288 100 0 1 FTVL:STRING
p 64 -320 100 0 1 PV:$(sadtop)$(dev)
p 64 -224 100 0 1 SCAN:1 second
use esirs -896 -665 100 0 Park
xform 0 -688 -512
p -832 -736 100 0 1 DESC:Detector translation parked?
p -640 -768 100 0 1 EGU:0/1
p -832 -736 100 0 0 FDSC:Detector translation parked?
p -832 -768 100 0 1 FTVL:LONG
p -832 -800 100 0 1 PV:$(sadtop)$(dev)
p -832 -704 100 0 1 SCAN:1 second
use esirs -896 -185 100 0 Index
xform 0 -688 -32
p -832 -256 100 0 1 DESC:Detector translation indexed?
p -640 -288 100 0 1 EGU:0/1
p -832 -256 100 0 0 FDSC:Detector translation indexed?
p -832 -288 100 0 1 FTVL:LONG
p -832 -320 100 0 1 PV:$(sadtop)$(dev)
p -832 -224 100 0 1 SCAN:1 second
use esirs -896 295 100 0 Init
xform 0 -688 448
p -832 224 100 0 1 DESC:Detector translation initialized?
p -640 192 100 0 1 EGU:0/1
p -832 224 100 0 0 FDSC:Detector translation initialized?
p -832 192 100 0 1 FTVL:LONG
p -832 160 100 0 1 PV:$(sadtop)$(dev)
p -832 256 100 0 1 SCAN:1 second
use esirs 0 295 100 0 State
xform 0 208 448
p 64 224 100 0 1 DESC:Detector translation state
p 64 224 100 0 0 FDSC:Detector translation state
p 64 192 100 0 1 FTVL:STRING
p 64 160 100 0 1 PV:$(sadtop)$(dev)
p 64 256 100 0 1 SCAN:1 second
use ukatcBorderC -1424 -1833 100 0 ukatcBorderC#89
xform 0 256 -528
p 1516 -1584 120 256 -1 Title:Translation assembly SAD
p 1556 -1648 100 1024 -1 author:S M Beard
p 1556 -1680 100 1024 -1 date:$Date$
[comments]

[schematic2]
uniq 162
[tools]
[detail]
w -78 -469 100 0 n#160 hwin.hwin#161.in -160 -480 64 -480 esirs.Debug.INP
w -974 -949 100 0 n#158 hwin.hwin#159.in -1056 -960 -832 -960 esirs.Update.INP
w -78 -949 100 0 n#157 hwin.hwin#156.in -160 -960 64 -960 esirs.Simulate.INP
w -974 491 100 0 n#152 hwin.hwin#151.in -1056 480 -832 480 esirs.Init.INP
w -974 11 100 0 n#149 hwin.hwin#150.in -1056 0 -832 0 esirs.Index.INP
w -974 -469 100 0 n#148 hwin.hwin#147.in -1056 -480 -832 -480 esirs.Park.INP
w -78 11 100 0 n#144 hwin.hwin#143.in -160 0 64 0 esirs.Health.INP
w -78 491 100 0 n#141 hwin.hwin#142.in -160 480 64 480 esirs.State.INP
w 978 11 100 0 n#134 hwin.hwin#133.in 896 0 1120 0 esirs.Pos2.INP
w 978 491 100 0 n#132 hwin.hwin#129.in 896 480 1120 480 esirs.Pos1.INP
s 656 -496 100 0 Written by assembly record --->
s 656 -960 100 0 Written by assembly record --->
[cell use]
use hwin -352 -521 100 0 hwin#161
xform 0 -256 -480
p -349 -488 100 0 -1 val(in):$(top)$(dev)Assembly.DBUG
use hwin -1248 -1001 100 0 hwin#159
xform 0 -1152 -960
p -1245 -968 100 0 -1 val(in):$(top)$(dev)Assembly.INIT
use hwin -352 -1001 100 0 hwin#156
xform 0 -256 -960
p -349 -968 100 0 -1 val(in):$(top)$(dev)Assembly.SIMM
use hwin -1248 439 100 0 hwin#151
xform 0 -1152 480
p -1245 472 100 0 -1 val(in):$(top)$(dev)Assembly.INIT
use hwin -1248 -41 100 0 hwin#150
xform 0 -1152 0
p -1245 -8 100 0 -1 val(in):$(top)$(dev)Assembly.INDX
use hwin -1248 -521 100 0 hwin#147
xform 0 -1152 -480
p -1245 -488 100 0 -1 val(in):$(top)$(dev)Assembly.PARK
use hwin -352 -41 100 0 hwin#143
xform 0 -256 0
p -349 -8 100 0 -1 val(in):$(top)$(dev)Assembly.HLTH
use hwin -352 439 100 0 hwin#142
xform 0 -256 480
p -349 472 100 0 -1 val(in):$(top)$(dev)Assembly.ASTA
use hwin 704 439 100 0 hwin#129
xform 0 800 480
p 707 472 100 0 -1 val(in):$(top)$(dev)1Device.MPOS
use hwin 704 -41 100 0 hwin#133
xform 0 800 0
p 707 -8 100 0 -1 val(in):$(top)$(dev)2Device.MPOS
use esirs -832 -1209 100 0 Update
xform 0 -624 -1056
p -768 -1280 100 0 1 DESC:Filter characterized?
p -576 -1312 100 0 1 EGU:0/1
p -768 -1280 100 0 0 FDSC:Filter characterized?
p -768 -1312 100 0 1 FTVL:LONG
p -768 -1344 100 0 1 PV:$(sadtop)$(dev)
p -768 -1248 100 0 1 SCAN:1 second
use esirs 1120 -1209 100 0 ID2
xform 0 1328 -1056
p 1184 -1280 100 0 1 DESC:Filter wheel 2 ID code
p 1376 -1312 100 0 1 EGU:barcode ID
p 1184 -1280 100 0 0 FDSC:Filter wheel 2 ID code
p 1184 -1312 100 0 1 FTVL:LONG
p 1184 -1344 100 0 1 PV:$(sadtop)$(dev)
p 1184 -1248 100 0 1 SCAN:Passive
use esirs 1120 -729 100 0 ID1
xform 0 1328 -576
p 1184 -800 100 0 1 DESC:Filter wheel 1 ID code
p 1376 -832 100 0 1 EGU:barcode ID
p 1184 -800 100 0 0 FDSC:Filter wheel 1 ID code
p 1184 -832 100 0 1 FTVL:LONG
p 1184 -864 100 0 1 PV:$(sadtop)$(dev)
p 1184 -768 100 0 1 SCAN:Passive
use esirs 64 231 100 0 State
xform 0 272 384
p 128 160 100 0 1 DESC:Filter state
p 128 160 100 0 0 FDSC:Filter state
p 128 128 100 0 1 FTVL:STRING
p 128 96 100 0 1 PV:$(sadtop)$(dev)
p 128 192 100 0 1 SCAN:1 second
use esirs -832 231 100 0 Init
xform 0 -624 384
p -768 160 100 0 1 DESC:Filter initialized?
p -576 128 100 0 1 EGU:0/1
p -768 160 100 0 0 FDSC:Filter initialized?
p -768 128 100 0 1 FTVL:LONG
p -768 96 100 0 1 PV:$(sadtop)$(dev)
p -768 192 100 0 1 SCAN:1 second
use esirs -832 -249 100 0 Index
xform 0 -624 -96
p -768 -320 100 0 1 DESC:Filter indexed?
p -576 -352 100 0 1 EGU:0/1
p -768 -320 100 0 0 FDSC:Filter indexed?
p -768 -352 100 0 1 FTVL:LONG
p -768 -384 100 0 1 PV:$(sadtop)$(dev)
p -768 -288 100 0 1 SCAN:1 second
use esirs -832 -729 100 0 Park
xform 0 -624 -576
p -768 -800 100 0 1 DESC:Filter parked?
p -576 -832 100 0 1 EGU:0/1
p -768 -800 100 0 0 FDSC:Filter parked?
p -768 -832 100 0 1 FTVL:LONG
p -768 -864 100 0 1 PV:$(sadtop)$(dev)
p -768 -768 100 0 1 SCAN:1 second
use esirs 64 -249 100 0 Health
xform 0 272 -96
p 128 -320 100 0 1 DESC:Filter health
p 128 -320 100 0 0 FDSC:Filter health
p 128 -352 100 0 1 FTVL:STRING
p 128 -384 100 0 1 PV:$(sadtop)$(dev)
p 128 -288 100 0 1 SCAN:1 second
use esirs 1120 231 100 0 Pos1
xform 0 1328 384
p 1184 160 100 0 1 DESC:Filter wheel 1 position
p 1376 128 100 0 1 EGU:eng units
p 1184 160 100 0 0 FDSC:Filter wheel 1 position
p 1184 128 100 0 1 FTVL:DOUBLE
p 1568 128 100 0 1 PREC:3
p 1184 96 100 0 1 PV:$(sadtop)$(dev)
p 1184 192 100 0 1 SCAN:1 second
use esirs 1120 -249 100 0 Pos2
xform 0 1328 -96
p 1184 -320 100 0 1 DESC:Filter wheel 2 position
p 1376 -352 100 0 1 EGU:eng units
p 1184 -320 100 0 0 FDSC:Filter wheel 2 position
p 1184 -352 100 0 1 FTVL:DOUBLE
p 1568 -352 100 0 1 PREC:3
p 1184 -384 100 0 1 PV:$(sadtop)$(dev)
p 1184 -272 100 0 1 SCAN:1 second
use esirs 64 -729 100 0 Debug
xform 0 272 -576
p 128 -800 100 0 1 DESC:Filter debug mode
p 128 -800 100 0 0 FDSC:Filter debug mode
p 128 -832 100 0 1 FTVL:STRING
p 128 -864 100 0 1 PV:$(sadtop)$(dev)
p 128 -768 100 0 1 SCAN:1 second
use esirs 64 -1209 100 0 Simulate
xform 0 272 -1056
p 128 -1280 100 0 1 DESC:Filter simulation mode
p 128 -1280 100 0 0 FDSC:Filter simulation mode
p 128 -1312 100 0 1 FTVL:STRING
p 128 -1344 100 0 1 PV:$(sadtop)$(dev)
p 128 -1248 100 0 1 SCAN:1 second
use ukatcBorderC -1424 -1833 100 0 ukatcBorderC#89
xform 0 256 -528
p 1516 -1584 120 256 -1 Title:Filter assembly SAD
p 1556 -1648 100 1024 -1 author:S M Beard
p 1556 -1680 100 1024 -1 date:$Date$
[comments]

[schematic2]
uniq 50
[tools]
[detail]
w 1732 1435 100 2 n#47 esirs.Loc.INP 1728 1440 1728 1440 hwin.hwin#49.in
w 1732 1851 100 2 n#44 esirs.ID.INP 1728 1856 1728 1856 hwin.hwin#46.in
w 772 1851 100 2 n#41 esirs.State.INP 768 1856 768 1856 hwin.hwin#43.in
w -220 1851 100 2 n#38 esirs.Simulate.INP -224 1856 -224 1856 hwin.hwin#40.in
w 772 187 100 2 n#35 esirs.Park.INP 768 192 768 192 hwin.hwin#37.in
w 772 603 100 2 n#32 esirs.Init.INP 768 608 768 608 hwin.hwin#34.in
w 772 1019 100 2 n#29 esirs.Index.INP 768 1024 768 1024 hwin.hwin#31.in
w 772 1435 100 2 n#26 esirs.Health.INP 768 1440 768 1440 hwin.hwin#28.in
w -220 187 100 2 n#23 esirs.Release.INP -224 192 -224 192 hwin.hwin#25.in
w -220 603 100 2 n#20 esirs.Gripper.INP -224 608 -224 608 hwin.hwin#22.in
w -220 1019 100 2 n#6 esirs.FP.INP -224 1024 -224 1024 hwin.hwin#5.in
w -220 1435 100 2 n#3 hwin.hwin#2.in -224 1440 -224 1440 esirs.Debug.INP
[cell use]
use hwin -416 983 100 0 hwin#5
xform 0 -320 1024
p -320 976 100 512 -1 val(in):$(top)$(dev)SwitchWord.VALD
use hwin -416 1399 100 0 hwin#2
xform 0 -320 1440
p -320 1392 100 512 -1 val(in):$(top)$(dev)Assembly.DBUG
use hwin -416 567 100 0 hwin#22
xform 0 -320 608
p -320 560 100 512 -1 val(in):$(top)$(dev)SwitchWord.VALC
use hwin -416 151 100 0 hwin#25
xform 0 -320 192
p -320 144 100 512 -1 val(in):$(top)$(dev)SwitchWord.VALB
use hwin 576 1399 100 0 hwin#28
xform 0 672 1440
p 672 1392 100 512 -1 val(in):$(top)$(dev)Assembly.HLTH
use hwin 576 983 100 0 hwin#31
xform 0 672 1024
p 672 976 100 512 -1 val(in):$(top)$(dev)Assembly.INDX
use hwin 576 567 100 0 hwin#34
xform 0 672 608
p 688 560 100 512 -1 val(in):$(top)$(dev)Assembly.INIT
use hwin 576 151 100 0 hwin#37
xform 0 672 192
p 688 144 100 512 -1 val(in):$(top)$(dev)Assembly.PARK
use hwin -416 1815 100 0 hwin#40
xform 0 -320 1856
p -304 1808 100 512 -1 val(in):$(top)$(dev)Assembly.SIMM
use hwin 576 1815 100 0 hwin#43
xform 0 672 1856
p 688 1808 100 512 -1 val(in):$(top)$(dev)Assembly.ASTA
use hwin 1536 1815 100 0 hwin#46
xform 0 1632 1856
p 1632 1808 100 512 -1 val(in):$(top)$(dev)Assembly.VALA
use hwin 1536 1399 100 0 hwin#49
xform 0 1632 1440
p 1632 1392 100 512 -1 val(in):$(top)$(dev)Assembly.VALB
use esirs 16 1072 100 0 FP
xform 0 -16 928
p -288 672 100 0 0 DESC:Mask/IFU in focal plane?
p -160 752 100 0 1 FDSC:Mask/IFU in focal plane?
p 0 816 100 256 -1 FTVL:LONG
p 16 1072 100 512 1 PV:$(sadtop)$(dev)
p -16 848 100 256 1 SCAN:1 second
use esirs 16 1488 100 0 Debug
xform 0 -16 1344
p -288 1088 100 0 0 DESC:Mask assembly debug mode
p -160 1168 100 0 1 FDSC:Mask assembly debug mode
p -16 1232 100 256 -1 FTVL:STRING
p 16 1488 100 512 1 PV:$(sadtop)$(dev)
p -16 1264 100 256 1 SCAN:1 second
use esirs 16 656 100 0 Gripper
xform 0 -16 512
p -288 256 100 0 0 DESC:Gripper state
p -288 160 100 0 0 EGU:0/1
p -160 336 100 0 1 FDSC:Gripper state
p -16 400 100 256 -1 FTVL:LONG
p 16 656 100 512 1 PV:$(sadtop)$(dev)
p -16 432 100 256 1 SCAN:1 second
use esirs 16 240 100 0 Release
xform 0 -16 96
p -288 -160 100 0 0 DESC:Release state
p -288 -256 100 0 0 EGU:0/1
p -160 -80 100 0 1 FDSC:Release state
p -16 -16 100 256 -1 FTVL:LONG
p 16 240 100 512 1 PV:$(sadtop)$(dev)
p -16 16 100 256 1 SCAN:1 second
use esirs 1008 1488 100 0 Health
xform 0 976 1344
p 704 1088 100 0 0 DESC:Mask assembly health
p 832 1168 100 0 1 FDSC:Mask assembly health
p 992 1232 100 256 -1 FTVL:STRING
p 1008 1488 100 512 1 PV:$(sadtop)$(dev)
p 976 1264 100 256 1 SCAN:1 second
use esirs 1008 1072 100 0 Index
xform 0 976 928
p 704 672 100 0 0 DESC:Mask assembly indexed?
p 704 576 100 0 0 EGU:0/1
p 832 752 100 0 1 FDSC:Mask assembly indexed?
p 976 816 100 256 -1 FTVL:LONG
p 1008 1072 100 512 1 PV:$(sadtop)$(dev)
p 992 848 100 256 1 SCAN:1 second
use esirs 1008 656 100 0 Init
xform 0 976 512
p 704 256 100 0 0 DESC:Mask assembly initialized?
p 704 160 100 0 0 EGU:0/1
p 832 336 100 0 1 FDSC:Mask assembly initialized?
p 976 400 100 256 -1 FTVL:LONG
p 1008 656 100 512 1 PV:$(sadtop)$(dev)
p 976 432 100 256 1 SCAN:1 second
use esirs 1008 240 100 0 Park
xform 0 976 96
p 704 -160 100 0 0 DESC:Mask assembly parked?
p 704 -256 100 0 0 EGU:0/1
p 832 -80 100 0 1 FDSC:Mask assembly parked?
p 992 -16 100 256 -1 FTVL:LONG
p 1008 240 100 512 1 PV:$(sadtop)$(dev)
p 992 16 100 256 1 SCAN:1 second
use esirs 16 1904 100 0 Simulate
xform 0 -16 1760
p -288 1504 100 0 0 DESC:Mask assembly simulation mode
p -160 1584 100 0 1 FDSC:Mask assembly simulation mode
p -16 1648 100 256 -1 FTVL:STRING
p 16 1904 100 512 1 PV:$(sadtop)$(dev)
p 0 1680 100 256 1 SCAN:1 second
use esirs 1008 1904 100 0 State
xform 0 976 1760
p 704 1504 100 0 0 DESC:Mask assembly state
p 832 1584 100 0 1 FDSC:Mask assembly state
p 976 1648 100 256 -1 FTVL:STRING
p 1008 1904 100 512 1 PV:$(sadtop)$(dev)
p 976 1680 100 256 1 SCAN:1 second
use esirs 1968 1904 100 0 ID
xform 0 1936 1760
p 1664 1504 100 0 0 DESC:ID of current mask/IFU
p 1664 1408 100 0 0 EGU:Barcode ID
p 1792 1584 100 0 1 FDSC:ID of current mask/IFU
p 1936 1648 100 256 -1 FTVL:LONG
p 1968 1904 100 512 1 PV:$(sadtop)$(dev)
p 1936 1680 100 256 1 SCAN:1 second
use esirs 1968 1488 100 0 Loc
xform 0 1936 1344
p 1664 1088 100 0 0 DESC:Location of current mask/IFU
p 1664 992 100 0 0 EGU:location code
p 1792 1168 100 0 1 FDSC:Location of current mask/IFU
p 1936 1232 100 256 -1 FTVL:LONG
p 1968 1488 100 512 1 PV:$(sadtop)$(dev)
p 1936 1264 100 256 1 SCAN:1 second
use gmosBorderC -768 -441 100 0 gmosBorderC#0
xform 0 912 864
p 2484 -312 100 512 1 File:/data/nimpkish/1/daoinsw/dev/gmos/current/develop/gmMask/capfast/gmMaskSad.sch
p 1892 -268 150 0 1 Rev:
p 2172 -192 120 256 -1 Title:SAD for GMOS Mask Assembly
p 2212 -256 100 1024 -1 author:B. Wooff
p 2212 -288 100 1024 -1 date:October 27, 1999
[comments]

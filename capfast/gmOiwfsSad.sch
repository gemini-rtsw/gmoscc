[schematic2]
uniq 136
[tools]
[detail]
w -1436 571 100 0 n#135 ecalcs.stateCalc.FLNK -1472 752 -1440 752 -1440 400 -1344 400 embbi.stateMbbi.SLNK
w -1438 731 100 0 n#134 ecalcs.stateCalc.VAL -1472 720 -1344 720 embbi.stateMbbi.INP
w -1902 915 100 0 n#132 hwin.hwin#133.in -1984 912 -1760 912 ecalcs.stateCalc.INPA
w -270 563 100 0 n#113 embbi.stateMbbi.VAL -320 560 -160 560 esirs.state.INP
w -292 571 100 0 n#112 embbi.stateMbbi.FLNK -320 752 -288 752 -288 400 -160 400 esirs.state.SLNK
w -1902 883 100 0 n#103 hwin.hwin#123.in -1984 880 -1760 880 ecalcs.stateCalc.INPB
w 1864 603 100 0 n#97 esirs.Debug.INP 1888 592 1888 592 hwin.hwin#2.in
w 1864 1035 100 0 n#96 hwin.hwin#40.in 1888 1024 1888 1024 esirs.Simulate.INP
w 968 -277 100 0 n#95 hwin.hwin#94.in 992 -288 992 -288 esirs.Index.INP
w -156 1435 100 2 n#75 esirs.health.INP -160 1440 -160 1440 hwin.hwin#90.in
w 1883 172 100 2 n#70 esirs.Park.INP 1888 128 1888 128 hwin.hwin#87.in
w 987 172 100 2 n#69 esirs.Init.INP 992 128 992 128 hwin.hwin#88.in
w 968 579 100 0 n#64 hwin.hwin#63.in 992 576 992 576 esirs.present.INP
w 1883 1484 100 2 n#60 hwin.hwin#59.in 1888 1440 1888 1440 esirs.Y.INP
w 1883 1932 100 2 n#52 hwin.hwin#51.in 1888 1888 1888 1888 esirs.X.INP
s 640 1056 100 0 Set from $(top)followA.VALG
s 640 1472 100 0 Set from $(top)followA.VALH
s 656 1920 100 0 Set from $(top)followA.VALB
[cell use]
use hwin -2176 871 100 0 hwin#133
xform 0 -2080 912
p -2173 904 100 0 -1 val(in):$(gm)cc:masterEnable.VAL
use hwin -2176 839 100 0 hwin#123
xform 0 -2080 880
p -2173 872 100 0 -1 val(in):$(top)wfs:probeAssembly.ASTA
use hwin 1696 983 100 0 hwin#40
xform 0 1792 1024
p 1808 976 100 512 -1 val(in):$(wfstop)$(dev)Assembly.SIMM
use hwin 1696 551 100 0 hwin#2
xform 0 1792 592
p 1840 544 100 512 -1 val(in):$(wfstop)$(dev)Assembly.DBUG
use hwin -352 1399 100 0 hwin#90
xform 0 -256 1440
p -256 1392 100 512 -1 val(in):$(wfstop)$(dev)Assembly.HLTH
use hwin 800 87 100 0 hwin#88
xform 0 896 128
p 912 80 100 512 -1 val(in):$(wfstop)$(dev)Assembly.INIT
use hwin 1696 87 100 0 hwin#87
xform 0 1792 128
p 1808 80 100 512 -1 val(in):$(wfstop)$(dev)Assembly.PARK
use hwin 800 535 100 0 hwin#63
xform 0 896 576
p 672 528 100 0 -1 val(in):$(wfstop)heartbeat.VAL
use hwin 1696 1399 100 0 hwin#59
xform 0 1792 1440
p 1456 1392 100 0 -1 val(in):$(wfstop)$(dev)CalcPosition.VALB
use hwin 1696 1847 100 0 hwin#51
xform 0 1792 1888
p 1456 1824 100 0 -1 val(in):$(wfstop)$(dev)CalcPosition.VALA
use hwin 800 -329 100 0 hwin#94
xform 0 896 -288
p 912 -336 100 512 -1 val(in):$(wfstop)$(dev)Assembly.INDX
use ecalcs -1552 976 100 0 stateCalc
xform 0 -1616 704
p -1696 384 100 0 1 CALC:A?(B+1):0
p -1568 976 100 512 1 PV:$(sadtop)
p -1696 416 100 0 1 SCAN:.5 second
use gmosBorderD -2688 -1241 100 0 gmosBorderD#130
xform 0 -48 464
p 2172 -992 120 256 -1 Title:Status Records for GMOS Oiwfs Assembly
p 1888 -1056 100 768 -1 author:B. Wooff, A. Ebbers
p 1888 -1088 100 768 -1 date:November 1, 2000
use esirs 112 624 100 0 state
xform 0 48 464
p -224 208 100 0 0 DESC:Wavefront Sensor state
p 144 240 100 0 0 EGU:units
p -96 272 100 0 1 FDSC:Wavefront Sensor state
p -96 240 100 0 1 FTVL:STRING
p 48 240 100 0 0 PREC:0
p 96 624 100 512 1 PV:$(sadtop)
p -96 208 100 0 1 SCAN:Passive
p -160 560 75 1280 -1 palrm(INP):MS
use esirs 2128 1072 100 0 Simulate
xform 0 2096 928
p 1824 672 100 0 0 DESC:Wavefront Sensor simulation mode
p 1952 752 100 0 1 FDSC:Wavefront Sensor simulation mode
p 2096 816 100 256 -1 FTVL:STRING
p 2128 1072 100 512 1 PV:$(sadtop)$(dev)
p 2112 848 100 256 1 SCAN:1 second
use esirs 2128 640 100 0 Debug
xform 0 2096 496
p 1824 240 100 0 0 DESC:Wavefront Sensor debugging mode
p 1952 320 100 0 1 FDSC:Wavefront Sensor debugging mode
p 2096 384 100 256 -1 FTVL:STRING
p 2128 640 100 512 1 PV:$(sadtop)$(dev)
p 2096 416 100 256 1 SCAN:1 second
use esirs 80 1488 100 0 health
xform 0 48 1344
p -224 1088 100 0 0 DESC:Wavefront Sensor Health
p -96 1152 100 0 1 FDSC:Wavefront Sensor Health
p 64 1232 100 256 -1 FTVL:STRING
p 80 1488 100 512 1 PV:$(sadtop)
p 48 1264 100 256 1 SCAN:1 second
use esirs 80 1968 100 0 name
xform 0 48 1824
p 48 1712 100 256 -1 FTVL:STRING
p 80 1968 100 512 1 PV:$(sadtop)
p 48 1744 100 256 1 SCAN:Passive
use esirs 2128 176 100 0 Park
xform 0 2096 32
p 1824 -224 100 0 0 DESC:Wavefront Sensor Parked?
p 1824 -320 100 0 0 EGU:0/1
p 1952 -160 100 0 1 FDSC:Wavefront Sensor Parked?
p 2112 -80 100 256 -1 FTVL:LONG
p 2128 176 100 512 1 PV:$(sadtop)$(dev)
p 2112 -48 100 256 1 SCAN:1 second
use esirs 1232 176 100 0 Init
xform 0 1200 32
p 928 -224 100 0 0 DESC:Wavefront Sensor initialized?
p 928 -320 100 0 0 EGU:0/1
p 1056 -144 100 0 1 FDSC:Wavefront Sensor Initialized?
p 1200 -80 100 256 -1 FTVL:LONG
p 1232 176 100 512 1 PV:$(sadtop)$(dev)
p 1200 -48 100 256 1 SCAN:1 second
use esirs 2160 1488 100 0 Y
xform 0 2096 1344
p 1824 1088 100 0 0 DESC:Wavefront Sensor Y coordinate
p 1824 992 100 0 0 EGU:mm
p 1952 1168 100 0 1 FDSC:Wavefront Sensor Y coordinate
p 2096 1232 100 0 -1 FTVL:DOUBLE
p 1968 1232 100 0 1 PREC:2
p 2128 1488 100 512 1 PV:$(sadtop)$(dev)
p 2016 1264 100 0 1 SCAN:1 second
use esirs 2160 1952 100 0 X
xform 0 2096 1792
p 1824 1536 100 0 0 DESC:Wavefront Sensor X coordinate
p 1824 1440 100 0 0 EGU:mm
p 1952 1616 100 0 1 FDSC:Wavefront Sensor X coordinate
p 2080 1680 100 0 -1 FTVL:DOUBLE
p 1968 1680 100 0 1 PREC:2
p 2128 1952 100 512 1 PV:$(sadtop)$(dev)
p 2016 1728 100 0 1 SCAN:1 second
use esirs 1232 1936 100 0 followS
xform 0 1200 1792
p 1216 1680 100 256 -1 FTVL:LONG
p 1232 1936 100 512 1 PV:$(sadtop)
p 1152 1712 100 0 1 SCAN:Passive
p 1072 1632 100 0 -1 SNAM:
use esirs 1232 1488 100 0 arrayS
xform 0 1200 1344
p 928 1088 100 0 0 DESC:TCS demand acceptable?
p 1056 1152 100 0 1 FDSC:TCS demand acceptable?
p 1200 1232 100 256 -1 FTVL:LONG
p 1232 1488 100 512 1 PV:$(sadtop)
p 1200 1264 100 256 1 SCAN:Passive
use esirs 1232 1072 100 0 inPosition
xform 0 1200 928
p 928 672 100 0 0 DESC:Wavefront Sensor in position?
p 928 576 100 0 0 EGU:0/1
p 1056 736 100 0 1 FDSC:Wavefront Sensor in position?
p 1200 816 100 256 -1 FTVL:LONG
p 1232 1072 100 512 1 PV:$(sadtop)
p 1200 848 100 256 1 SCAN:Passive
use esirs 1232 624 100 0 present
xform 0 1200 480
p 928 224 100 0 0 DESC:Continuously changing
p 1056 304 100 0 1 FDSC:Continuously changing
p 1200 368 100 256 -1 FTVL:LONG
p 1216 624 100 512 1 PV:$(sadtop)
p 1200 400 100 256 1 SCAN:1 second
use esirs 1232 -240 100 0 Index
xform 0 1200 -384
p 928 -640 100 0 0 DESC:Wavefront Sensor indexed?
p 928 -736 100 0 0 EGU:0/1
p 1056 -576 100 0 1 FDSC:Wavefront Sensor indexed?
p 1216 -496 100 256 -1 FTVL:LONG
p 1232 -240 100 512 1 PV:$(sadtop)$(dev)
p 1216 -464 100 256 1 SCAN:1 second
use embbi -816 976 100 0 stateMbbi
xform 0 -832 544
p -800 462 100 0 1 EIST:CONFIGURING
p -800 366 100 0 1 ELST:BOOTING
p -608 366 100 0 1 ELSV:INVALID
p -800 238 100 0 1 FFST:BOOTING
p -608 238 100 0 1 FFSV:INVALID
p -800 590 100 0 1 FRST:CONFIGURING
p -800 270 100 0 1 FTST:BOOTING
p -608 270 100 0 1 FTSV:INVALID
p -800 558 100 0 1 FVST:CONFIGURING
p -800 430 100 0 1 NIST:RUNNING
p -800 686 100 0 1 ONST:RUNNING
p -832 976 100 512 1 PV:$(sadtop)
p -800 494 100 0 1 SVST:CONFIGURING
p -800 526 100 0 1 SXST:CONFIGURING
p -800 398 100 0 1 TEST:BOOTING
p -608 398 100 0 1 TESV:INVALID
p -800 622 100 0 1 THST:CONFIGURING
p -800 302 100 0 1 TTST:BOOTING
p -608 302 100 0 1 TTSV:INVALID
p -800 334 100 0 1 TVST:BOOTING
p -608 334 100 0 1 TVSV:INVALID
p -800 654 100 0 1 TWST:INITIALIZING
p -800 718 100 0 1 ZRST:BOOTING
p -1344 720 75 1280 -1 palrm(INP):MS
[comments]

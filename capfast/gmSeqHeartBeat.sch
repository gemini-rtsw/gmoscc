[schematic2]
uniq 75
[tools]
[detail]
w 3088 771 100 0 n#74 elongouts.heartBeatOut.FLNK 3120 992 3456 992 3456 768 2768 768 2768 320 2944 320 ecalcs.binHeartBeat.SLNK
w 3096 819 100 0 n#73 elongouts.heartBeatOut.VAL 3120 960 3424 960 3424 816 2816 816 2816 704 2944 704 ecalcs.binHeartBeat.INPA
w 3152 939 100 0 c#70 elongouts.heartBeatOut.OUT 3120 928 3232 928 rboat.c#70.p
w 2768 971 100 0 n#69 ecalcs.heartBeatCalc.FLNK 2672 1024 2720 1024 2720 960 2864 960 elongouts.heartBeatOut.SLNK
w 2478 1314 100 0 n#1 ecalcs.heartBeatCalc.VAL 2672 992 2768 992 2768 1312 2224 1312 2224 1184 2384 1184 ecalcs.heartBeatCalc.INPA
w 2798 1003 100 0 n#1 junction 2768 992 2864 992 elongouts.heartBeatOut.DOL
w 1624 1147 100 0 c#66 inhier.CC.P 1280 1344 1504 1344 1504 1136 1792 1136 esels.scanHeartBeat.INPB
w 1624 1115 100 0 c#67 inhier.DC.P 1280 896 1504 896 1504 1104 1792 1104 esels.scanHeartBeat.INPC
w 2144 1003 100 0 n#55 esels.scanHeartBeat.VAL 2080 992 2256 992 2256 768 2384 768 ecalcs.heartBeatCalc.SDIS
w 2280 811 100 0 n#54 esels.scanHeartBeat.FLNK 2080 1024 2224 1024 2224 800 2384 800 ecalcs.heartBeatCalc.SLNK
s 1056 1104 150 0 subsystem absent
s 1040 1152 150 0 Input value = 1 if
s 3104 2096 150 0 gmSeqHeartBeat.sch
[cell use]
use ecalcs 2408 712 100 0 heartBeatCalc
xform 0 2528 976
p 2576 814 100 0 1 CALC:(A+1)%1000
p 2368 1246 100 0 -1 DESC:Heartbeat record for GMOS
p 2576 768 100 0 1 DISV:1
p 2096 862 100 0 0 EGU:counter
p 2576 782 100 0 0 SCAN:Passive
p 2432 688 100 1024 0 name:$(top)$(I)
use ecalcs 2944 231 100 0 binHeartBeat
xform 0 3088 496
p 3040 544 100 0 1 CALC:A%2
p 2656 382 100 0 0 EGU:counter
use rboat 3232 887 100 0 c#70
xform 0 3312 928
use elongouts 2864 871 100 0 heartBeatOut
xform 0 2992 960
p 2704 1102 100 0 0 EGU:counter
p 2928 848 100 0 1 OMSL:closed_loop
p 3120 880 100 0 -1 def(OUT):$(sad)heartBeat
p 3120 928 75 768 -1 pproc(OUT):PP
use inhier 1216 880 140 0 DC
xform 0 1280 896
use inhier 1216 1328 140 0 CC
xform 0 1280 1344
use oslBorderC 512 -345 100 0 oslBorderC#63
xform 0 2192 960
p 3452 -96 120 256 -1 Title:GMOS IS - generate heartbeat
use esels 1792 711 100 0 scanHeartBeat
xform 0 1936 976
p 1728 1264 100 0 -1 DESC:Disable/enable heartbeat record
p 1984 768 100 0 1 SCAN:1 second
p 1984 816 100 0 1 SELM:High Signal
[comments]

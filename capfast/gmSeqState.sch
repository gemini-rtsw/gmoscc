[schematic2]
uniq 66
[tools]
[detail]
w 1784 1243 -100 0 c#65 egenSub.combineState.OUTC 1600 1232 2016 1232 rboat.c#65.p
w 1120 1307 100 0 n#52 inhier.DC.P 976 1296 1312 1296 egenSub.combineState.INPB
w 1120 1371 100 0 n#51 inhier.CC.P 976 1360 1312 1360 egenSub.combineState.INPA
w 1120 1211 100 0 c#58 lboat.c#58.p 976 1200 1312 1200 egenSub.combineState.D
w 1102 1259 100 0 c#57 lboat.c#57.p 976 1248 1312 1248 1312 1264 egenSub.combineState.C
w 1784 1307 -100 0 c#60 egenSub.combineState.OUTB 1600 1296 2016 1296 rboat.c#60.p
w 1784 1371 -100 0 c#59 egenSub.combineState.OUTA 1600 1360 2016 1360 rboat.c#59.p
s 2720 2208 150 0 gmSeqState.sch
s 848 1392 100 0 Value = 1 if disabled
s 1152 1888 140 0 This schematic combines the state values
s 1152 1840 140 0 of the Components and Detector Controllers to form
s 1152 1808 140 0 an overall state for GMOS
s 1696 1200 100 0 Connection state change message
s 2080 1392 100 0 Combined GMOS system state
[cell use]
use rboat 2016 1191 100 0 c#65
xform 0 2096 1232
use rboat 2016 1319 100 0 c#59
xform 0 2096 1360
use rboat 2016 1255 100 0 c#60
xform 0 2096 1296
use lboat 816 1159 100 0 c#58
xform 0 896 1200
use lboat 816 1207 100 0 c#57
xform 0 896 1248
use oslBorderC -144 -233 100 0 oslBorderC#56
xform 0 1536 1072
use inhier 928 1296 100 0 DC
xform 0 976 1296
use inhier 928 1360 100 0 CC
xform 0 976 1360
use egenSub 1336 600 100 0 combineState
xform 0 1456 1024
p 1263 1463 100 0 -1 DESC:Calculate value of GMOS state record
p 1024 1134 100 0 0 DISS:NO_ALARM
p 1376 1376 100 0 -1 FTA:LONG
p 1376 1344 100 0 -1 FTB:LONG
p 1392 1232 100 0 -1 FTC:STRING
p 1392 1200 100 0 -1 FTD:STRING
p 1089 277 100 0 0 FTE:STRING
p 1089 213 100 0 0 FTF:STRING
p 1089 213 100 0 0 FTG:STRING
p 1089 181 100 0 0 FTH:STRING
p 1089 149 100 0 0 FTI:STRING
p 1456 1360 100 0 -1 FTVA:STRING
p 1488 1296 100 0 -1 FTVB:LONG
p 1472 1248 100 0 -1 FTVC:STRING
p 1472 1184 100 0 0 FTVD:STRING
p 1344 464 100 0 1 INAM:gmSeqStateInit
p 1344 558 100 0 1 SCAN:1 second
p 1344 526 100 0 1 SNAM:gmSeqStateCombine
p 816 1248 100 0 -1 def(INPC):$(top)cc:state
p 800 1200 100 0 -1 def(INPD):$(top)dc:state
p 1072 1120 100 0 0 def(INPE):0.0
p 1072 1056 100 0 0 def(INPF):0.0
p 1712 1376 100 0 -1 def(OUTA):$(sad)state.VAL
p 1680 1312 100 0 0 def(OUTB):0.0
p 1712 1248 100 0 -1 def(OUTC):$(sad)state.IMSS
p 1424 592 100 1024 0 name:$(top)$(I)
p 1632 1242 75 0 -1 palrm(OUTC):MS
p 1632 1178 75 0 -1 palrm(OUTD):NMS
p 1600 1370 75 0 -1 pproc(OUTA):PP
p 1600 1306 75 0 -1 pproc(OUTB):PP
[comments]

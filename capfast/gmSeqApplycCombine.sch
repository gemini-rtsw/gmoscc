[schematic2]
uniq 121
[tools]
[detail]
w 2658 1547 100 0 n#120 hwin.hwin#119.in 2752 1616 2848 1616 2848 1536 2528 1536 2528 1344 2608 1344 ecars.applyC.ICID
w 218 1387 100 0 DC inhier.DC.P 176 1376 320 1376 gmSeqCarSubsys.gmSeqCarSubsys#58.LOADED
w 218 1803 100 0 CC inhier.CC.P 176 1792 320 1792 gmSeqCarSubsys.gmSeqCarSubsys#57.LOADED
w 2506 1195 100 0 n#115 egenSubD.combineApplyC.FLNK 2384 736 2464 736 2464 1184 2608 1184 ecars.applyC.SLNK
w 2410 1387 100 0 n#114 egenSubD.combineApplyC.OUTB 2384 1376 2496 1376 2496 1312 2608 1312 ecars.applyC.IMSS
w 2434 1451 100 0 n#113 egenSubD.combineApplyC.OUTA 2384 1440 2544 1440 2544 1376 2608 1376 ecars.applyC.IVAL
w 1562 1163 100 0 ISMESS inhier.ISMESS.P 480 880 1088 880 1088 1152 2096 1152 egenSubD.combineApplyC.INPK
w 1522 1259 100 0 n#111 gmSeqCarSubsys.gmSeqCarSubsys#58.MESS 832 1312 1008 1312 1008 1248 2096 1248 egenSubD.combineApplyC.INPH
w 1562 1347 100 0 n#110 gmSeqCarSubsys.gmSeqCarSubsys#57.MESS 832 1728 1088 1728 1088 1344 2096 1344 egenSubD.combineApplyC.INPE
w 1538 1195 100 0 ISCAR inhier.ISCAR.P 480 960 1040 960 1040 1184 2096 1184 egenSubD.combineApplyC.INPJ
w 1538 1291 100 0 n#108 gmSeqCarSubsys.gmSeqCarSubsys#58.CAR 832 1376 1040 1376 1040 1280 2096 1280 egenSubD.combineApplyC.INPG
w 1578 1387 100 0 n#107 gmSeqCarSubsys.gmSeqCarSubsys#57.CAR 832 1792 1120 1792 1120 1376 2096 1376 egenSubD.combineApplyC.INPD
w 1658 1515 100 0 CONFC inhier.CONFC.P 1408 1504 1968 1504 1968 1472 2096 1472 egenSubD.combineApplyC.INPA
w 2984 1387 100 0 APPLYC ecars.applyC.VAL 2928 1376 3088 1376 outhier.APPLYC.p
w 1440 779 100 0 c#103 inhier.ISFLNK.P 480 768 2096 768 egenSubD.combineApplyC.SLNK
w 848 1195 100 0 c#103 gmSeqCarSubsys.gmSeqCarSubsys#58.FLNK 832 1184 912 1184 912 768 junction
w 824 1611 100 0 c#103 gmSeqCarSubsys.gmSeqCarSubsys#57.FLNK 832 1600 864 1600 864 768 junction
s 2112 1936 100 0 If any of the applyC records are processed then this causes 
s 2112 1904 100 0 the genSub combine record to process. It combines the applyC
s 2112 1872 100 0 values and messages to generate a single overall
s 2112 1840 100 0 applyC status for GMOS.
s 2736 2256 150 0 gmSeqApplycCombine.sch
s 16 896 140 0 Local Instrument Sequencer
s 16 848 140 0 overall CAR status
s 1264 1536 100 0 Configuration active CAR status
[cell use]
use hwin 2560 1575 100 0 hwin#119
xform 0 2656 1616
p 2560 1648 100 0 -1 val(in):$(gm)apply.CLID
use inhier 184 1752 100 0 CC
xform 0 176 1792
use inhier 184 1336 100 0 DC
xform 0 176 1376
use inhier 1392 1463 100 0 CONFC
xform 0 1408 1504
use inhier 464 919 100 0 ISCAR
xform 0 480 960
use inhier 464 839 100 0 ISMESS
xform 0 480 880
use inhier 464 727 100 0 ISFLNK
xform 0 480 768
use outhier 3056 1335 100 0 APPLYC
xform 0 3072 1376
use oslBorderC -96 -185 100 0 oslBorderC#91
xform 0 1584 1120
p 2844 64 120 256 -1 Title:GMOS IS - generate applyC CAR status
use ecars 2632 1096 100 0 applyC
xform 0 2768 1264
p 2672 1438 100 0 -1 DESC:GMOS main applyC record
p 2720 1088 100 1024 0 name:$(gm)$(I)
use gmSeqCarSubsys 440 1032 100 0 gmSeqCarSubsys#58
xform 0 576 1280
p 384 1120 100 0 1 seta:subsys dc
p 384 1088 100 0 1 setb:top $(top)$(subsys)
p 384 1056 100 0 1 setc:instru DC
use gmSeqCarSubsys 440 1448 100 0 gmSeqCarSubsys#57
xform 0 576 1696
p 384 1536 100 0 1 seta:subsys cc
p 384 1504 100 0 1 setb:top $(top)$(subsys)
p 384 1472 100 0 1 setc:instru CC
use egenSubD 2096 679 100 0 combineApplyC
xform 0 2240 1104
p 2128 1536 100 0 -1 DESC:Combine all applyC values
p 1873 453 100 0 0 FTA:LONG
p 1873 453 100 0 0 FTB:STRING
p 1873 421 100 0 0 FTC:LONG
p 1873 389 100 0 0 FTD:LONG
p 1873 357 100 0 0 FTE:STRING
p 1873 293 100 0 0 FTF:LONG
p 1873 293 100 0 0 FTG:LONG
p 1873 261 100 0 0 FTH:STRING
p 1873 229 100 0 0 FTI:LONG
p 1873 197 100 0 0 FTJ:LONG
p 1873 453 100 0 0 FTK:STRING
p 1873 453 100 0 0 FTL:LONG
p 1873 421 100 0 0 FTM:LONG
p 1873 389 100 0 0 FTN:STRING
p 1873 357 100 0 0 FTO:LONG
p 1873 293 100 0 0 FTP:STRING
p 2240 1472 100 0 -1 FTVA:LONG
p 2240 1408 100 0 -1 FTVB:STRING
p 1873 421 100 0 0 FTVC:LONG
p 2160 608 100 0 1 SNAM:gmosCarCombineBusy
p 2384 1386 75 0 -1 pproc(OUTB):NPP
[comments]

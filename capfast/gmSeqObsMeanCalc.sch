[schematic2]
uniq 61
[tools]
[detail]
w 3248 592 100 2 c#60 rboat.c#60.p 3056 576 3056 576 eaos.Mean.OUT
w 888 411 100 0 n#59 eaos.initCounter.FLNK 1072 544 1264 544 1264 400 560 400 560 320 816 320 eaos.BeginSadGet.SLNK
w 1296 320 100 2 c#57 eaos.BeginSadGet.OUT 1072 288 1072 288 rboat.c#57.p
w 592 384 100 2 c#55 eaos.BeginSadGet.DOL 816 352 816 352 lboat.c#55.p
w 1936 1920 100 2 c#53 rboat.c#53.p 1728 1904 1728 1904 eaos.EndSadGet.OUT
w 1264 1984 100 2 c#52 lboat.c#52.p 1472 1968 1472 1968 eaos.EndSadGet.DOL
w 2104 1979 100 0 n#51 eaos.EndSadGet.FLNK 1728 1968 2528 1968 2528 1008 2784 1008 ecalcs.MeanCalc.SLNK
w 952 1939 100 0 n#50 efanouts.endFan.LNK2 480 1936 1472 1936 eaos.EndSadGet.SLNK
w 2052 1547 100 2 c#48 lboat.c#48.p 2048 1552 2048 1552 ecalcs.SumCalc.INPB
w 2912 811 100 0 n#47 ecalcs.MeanCalc.VAL 3072 1200 3120 1200 3120 800 2752 800 2752 640 2800 640 eaos.Mean.DOL
w 2912 747 100 0 n#46 ecalcs.MeanCalc.FLNK 3072 1232 3168 1232 3168 736 2704 736 2704 608 2800 608 eaos.Mean.SLNK
w 2652 1011 100 0 n#19 junction 2416 672 2656 672 2656 1360 2784 1360 ecalcs.MeanCalc.INPB
w 2152 962 100 0 n#19 ecalcs.counter.VAL 2336 672 2416 672 2416 960 1936 960 1936 864 2048 864 ecalcs.counter.INPA
w 2582 1394 100 0 n#1 junction 2416 1392 2784 1392 ecalcs.MeanCalc.INPA
w 2142 1682 100 0 n#1 ecalcs.SumCalc.VAL 2336 1392 2416 1392 2416 1680 1904 1680 1904 1584 2048 1584 ecalcs.SumCalc.INPA
w 1794 888 100 0 n#29 efanouts.fan.LNK2 1632 1248 1792 1248 1792 480 junction
w 1536 483 100 0 n#29 eaos.initCounter.OUT 1072 480 2048 480 ecalcs.counter.SLNK
w 1704 1282 100 0 n#27 efanouts.fan.LNK1 1632 1280 1824 1280 1824 1200 junction
w 1424 738 100 0 n#27 eaos.initSum.OUT 1072 736 1824 736 1824 1200 2048 1200 ecalcs.SumCalc.SLNK
w 636 1755 100 0 n#15 efanouts.endFan.LNK1 480 1968 640 1968 640 1552 352 1552 352 1376 junction
w 272 1378 100 0 n#15 inhier.PAUSE.P 144 1376 448 1376 elongouts.disable.SLNK
w 176 1890 100 0 n#36 inhier.END.P 160 1888 240 1888 efanouts.endFan.SLNK
w 354 984 100 0 n#16 efanouts.beginFan.FLNK 448 768 512 768 512 864 352 864 352 1056 junction
w 272 1058 100 0 n#16 inhier.CONTINUE.P 144 1056 448 1056 elongouts.enable.SLNK
w 152 658 100 0 n#31 inhier.BEGIN.P 144 656 208 656 efanouts.beginFan.SLNK
w 936 642 100 0 n#28 eaos.initSum.FLNK 1072 800 1184 800 1184 640 736 640 736 512 816 512 eaos.initCounter.SLNK
w 552 738 100 0 n#26 efanouts.beginFan.LNK1 448 736 704 736 704 768 816 768 eaos.initSum.SLNK
w 776 546 100 0 n#25 hwin.hwin#23.in 784 544 816 544 eaos.initCounter.DOL
w 776 802 100 0 n#24 hwin.hwin#22.in 784 800 816 800 eaos.initSum.DOL
w 834 1152 100 0 n#12 elongouts.enable.OUT 704 1024 832 1024 832 1232 junction
w 880 1234 100 0 n#12 elongouts.disable.OUT 704 1344 832 1344 832 1232 976 1232 elongouts.fanDisable.SLNK
w 392 1410 100 0 n#11 hwin.hwin#10.in 384 1408 448 1408 elongouts.disable.DOL
w 398 1090 100 0 n#8 hwin.hwin#7.in 384 1088 448 1088 elongouts.enable.DOL
w 1294 1234 100 0 n#4 elongouts.fanDisable.VAL 1232 1232 1392 1232 efanouts.fan.SDIS
s 272 2176 130 0 endsad : Value at END written here
s 240 2224 130 0 beginsad : Value at BEGIN written here
s 256 2272 130 0 meansad : Write the mean value here
s 272 2320 130 0 value : The SAD value to be averaged
s 240 2384 130 0 The following macros should be defined:
s 1136 2336 200 0 Compute average value during an observation
s 1248 2272 100 0 The BEGIN link initialises the value sum and counter and
s 1248 2240 100 0 then enables the 1 Hz loop that integrates the value during
s 1248 2208 100 0 the observation. The END link disables the 1 Hz integrator
s 1248 2176 100 0 and computes the mean value by dividing the integral by
s 1248 2144 100 0 the number of samples. The PAUSE and CONTINUE links stop
s 1248 2112 100 0 and start the integrator respectively.
s 2832 2400 140 0 gmSeqObsMeanCalc.sch
s 1440 1568 130 0 Value to be averaged is read
s 1440 1536 130 0 into this record on INPB
s 1328 1824 100 0 Value at END is written to SAD
s 672 208 100 0 Value at BEGIN is written to SAD
[cell use]
use rboat 1728 1863 100 0 c#53
xform 0 1808 1904
use rboat 1072 247 100 0 c#57
xform 0 1152 288
use rboat 3056 535 100 0 c#60
xform 0 3136 576
use lboat 1888 1511 100 0 c#48
xform 0 1968 1552
use lboat 1312 1927 100 0 c#52
xform 0 1392 1968
use lboat 656 311 100 0 c#55
xform 0 736 352
use eaos 2800 519 100 0 Mean
xform 0 2928 608
p 2768 334 100 0 0 EGU:units
p 2864 480 100 0 1 OMSL:closed_loop
p 2544 462 100 0 0 PREC:4
p 3056 528 100 0 -1 def(OUT):$(meansad)
p 3056 576 75 768 -1 pproc(OUT):PP
use eaos 840 424 100 0 initCounter
xform 0 944 512
p 832 606 100 0 -1 DESC:Initial counter value
p 1280 432 100 0 1 EGU:counter
p 1008 432 100 0 1 OMSL:closed_loop
p 1280 400 100 0 1 PREC:0
p 928 416 100 1024 0 name:$(top)$(I)
p 1072 480 75 768 -1 pproc(OUT):PP
use eaos 840 680 100 0 initSum
xform 0 944 768
p 832 862 100 0 -1 DESC:Initial value sum
p 1216 672 100 0 1 EGU:units
p 976 672 100 0 1 OMSL:closed_loop
p 1216 640 100 0 1 PREC:4
p 928 672 100 1024 0 name:$(top)$(I)
p 1072 736 75 768 -1 pproc(OUT):PP
use eaos 1472 1847 100 0 EndSadGet
xform 0 1600 1936
p 1440 1662 100 0 0 EGU:units
p 1536 1792 100 0 1 OMSL:closed_loop
p 1216 1790 100 0 0 PREC:4
p 1328 1984 100 0 -1 def(DOL):$(value)
p 1792 1872 100 0 -1 def(OUT):$(endsad)
p 1728 1904 75 768 -1 pproc(OUT):PP
use eaos 816 231 100 0 BeginSadGet
xform 0 944 320
p 1120 224 100 0 1 EGU:units
p 880 176 100 0 1 OMSL:closed_loop
p 1120 192 100 0 1 PREC:4
p 624 352 100 0 -1 def(DOL):$(value)
p 1136 288 100 0 -1 def(OUT):$(beginsad)
p 1072 288 75 768 -1 pproc(OUT):PP
use oslBorderC -32 -41 100 0 oslBorderC#44
xform 0 1648 1264
p 2908 208 120 256 -1 Title:GMOS IS - compute mean value during observation
use ecalcs 2808 920 100 0 MeanCalc
xform 0 2928 1184
p 2832 878 100 0 1 CALC:B=0?0.0:A/B
p 2768 1454 100 0 -1 DESC:Compute mean value
p 2496 1038 100 0 0 PREC:4
p 2896 912 100 1024 0 name:$(top)$(I)
use ecalcs 2072 392 100 0 counter
xform 0 2192 656
p 2144 590 100 0 1 CALC:A+1
p 2032 926 100 0 -1 DESC:Increment counter
p 1760 542 100 0 0 EGU:counter
p 2160 384 100 1024 0 name:$(top)$(I)
use ecalcs 2072 1112 100 0 SumCalc
xform 0 2192 1376
p 2144 1310 100 0 1 CALC:A+B
p 2032 1646 100 0 -1 DESC:Integrate value
p 1760 1230 100 0 0 PREC:4
p 1904 1552 100 0 -1 def(INPB):$(value)
p 2160 1104 100 1024 0 name:$(top)$(I)
use inhier 168 1848 100 0 END
xform 0 160 1888
use inhier 152 616 100 0 BEGIN
xform 0 144 656
use inhier 152 1016 100 0 CONTINUE
xform 0 144 1056
use inhier 152 1336 100 0 PAUSE
xform 0 144 1376
use efanouts 264 1752 100 0 endFan
xform 0 360 1904
p 320 1870 100 0 0 DISV:1
p 320 2078 100 0 0 SCAN:Passive
p 272 2032 100 0 1 SELM:All
p 352 1744 100 1024 0 name:$(top)$(I)
use efanouts 232 520 100 0 beginFan
xform 0 328 672
p 288 638 100 0 0 DISV:1
p 288 846 100 0 0 SCAN:Passive
p 208 800 100 0 1 SELM:All
p 320 512 100 1024 0 name:$(top)$(I)
use efanouts 1416 1064 100 0 fan
xform 0 1512 1216
p 1472 1182 100 0 1 DISV:0
p 1472 1390 100 0 1 SCAN:1 second
p 1408 1344 100 0 1 SELM:All
p 1504 1056 100 1024 0 name:$(top)$(I)
use hwin 616 504 100 0 hwin#23
xform 0 688 544
p 595 536 100 0 -1 val(in):0
use hwin 616 760 100 0 hwin#22
xform 0 688 800
p 595 792 100 0 -1 val(in):0.0
use hwin 216 1368 100 0 hwin#10
xform 0 288 1408
p 195 1400 100 0 -1 val(in):0
use hwin 216 1048 100 0 hwin#7
xform 0 288 1088
p 195 1080 100 0 -1 val(in):1
use elongouts 472 1288 100 0 disable
xform 0 576 1376
p 432 1470 100 0 -1 DESC:Disable value calculation
p 512 1248 100 0 1 OMSL:closed_loop
p 560 1280 100 1024 0 name:$(top)$(I)
p 704 1344 75 768 -1 pproc(OUT):PP
use elongouts 472 968 100 0 enable
xform 0 576 1056
p 432 1150 100 0 -1 DESC:Enable value calculation
p 512 928 100 0 1 OMSL:closed_loop
p 560 960 100 1024 0 name:$(top)$(I)
p 704 1024 75 768 -1 pproc(OUT):PP
use elongouts 1000 1144 100 0 fanDisable
xform 0 1104 1232
p 880 1342 100 0 -1 DESC:Disable flag for mean value calculation
p 1040 1104 100 0 1 OMSL:supervisory
p 1088 1136 100 1024 0 name:$(top)$(I)
[comments]

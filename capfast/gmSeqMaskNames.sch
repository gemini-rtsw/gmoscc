[schematic2]
uniq 86
[tools]
[detail]
w -220 435 100 0 n#85 efanouts.getMaskNames.LNK3 -304 1136 -224 1136 -224 -256 576 -256 egenSubB.getCassette3Names.SLNK
w 184 715 100 0 n#84 efanouts.getMaskNames.LNK2 -304 1168 -160 1168 -160 704 576 704 egenSubB.getCassette2Names.SLNK
w 152 1667 100 0 n#83 efanouts.getMaskNames.LNK1 -304 1200 -224 1200 -224 1664 576 1664 egenSubB.getCassette1Names.SLNK
w 2776 59 100 0 n#82 embbi.maskCass3Select.VAL 2752 48 2848 48 estringouts.maskName3Out.DOL
w 2792 27 100 0 n#81 embbi.maskCass3Select.FLNK 2752 240 2784 240 2784 16 2848 16 estringouts.maskName3Out.SLNK
w 2936 -69 100 0 n#80 estringouts.maskName3Out.FLNK 3104 32 3136 32 3136 -80 2784 -80 2784 -304 2976 -304 estringouts.maskLoc3Out.SLNK
w 552 171 100 0 n#79 hwin.hwin#72.in 576 160 576 160 egenSubB.getCassette3Names.INPJ
w 552 107 100 0 n#78 hwin.hwin#71.in 576 96 576 96 egenSubB.getCassette3Names.INPL
w 552 43 100 0 n#77 hwin.hwin#70.in 576 32 576 32 egenSubB.getCassette3Names.INPN
w 456 427 100 0 n#68 elongins.cass3.VAL 384 416 576 416 egenSubB.getCassette3Names.INPB
w 104 475 100 0 n#66 hwin.hwin#67.in 128 464 128 464 elongins.cass3.INP
w 104 1435 100 0 n#65 elongins.cass2.INP 128 1424 128 1424 hwin.hwin#64.in
w 456 1387 100 0 n#63 elongins.cass2.VAL 384 1376 576 1376 egenSubB.getCassette2Names.INPB
w 552 1003 100 0 n#54 egenSubB.getCassette2Names.INPN 576 992 576 992 hwin.hwin#61.in
w 552 1067 100 0 n#53 egenSubB.getCassette2Names.INPL 576 1056 576 1056 hwin.hwin#60.in
w 552 1131 100 0 n#52 egenSubB.getCassette2Names.INPJ 576 1120 576 1120 hwin.hwin#59.in
w 2936 891 100 0 n#51 estringouts.maskName2Out.FLNK 3104 992 3136 992 3136 880 2784 880 2784 656 2976 656 estringouts.maskLoc2Out.SLNK
w 2792 987 100 0 n#50 embbi.maskCass2Select.FLNK 2752 1200 2784 1200 2784 976 2848 976 estringouts.maskName2Out.SLNK
w 2776 1019 100 0 n#49 embbi.maskCass2Select.VAL 2752 1008 2848 1008 estringouts.maskName2Out.DOL
w 104 2395 100 0 n#48 hwin.hwin#47.in 128 2384 128 2384 elongins.cass1.INP
w 456 2347 100 0 n#46 elongins.cass1.VAL 384 2336 576 2336 egenSubB.getCassette1Names.INPB
w 552 1963 100 0 n#41 egenSubB.getCassette1Names.INPN 576 1952 576 1952 hwin.hwin#42.in
w 552 2027 100 0 n#40 hwin.hwin#39.in 576 2016 576 2016 egenSubB.getCassette1Names.INPL
w 552 2091 100 0 n#38 hwin.hwin#37.in 576 2080 576 2080 egenSubB.getCassette1Names.INPJ
w 2936 1851 100 0 n#34 estringouts.maskName1Out.FLNK 3104 1952 3136 1952 3136 1840 2784 1840 2784 1616 2976 1616 estringouts.MaskLoc1Out.SLNK
w 2792 1947 100 0 n#32 embbi.maskCass1Select.FLNK 2752 2160 2784 2160 2784 1936 2848 1936 estringouts.maskName1Out.SLNK
w 2776 1979 100 0 n#31 embbi.maskCass1Select.VAL 2752 1968 2848 1968 estringouts.maskName1Out.DOL
w -744 1131 100 0 SLNK inhier.SLNK.P -800 1120 -544 1120 efanouts.getMaskNames.SLNK
s -704 2096 200 0 This gensub routine combine two
s -704 2032 200 0 lookup files and output a list of
s -704 1968 200 0 names into the MBBI state strings
[cell use]
use embbi 1728 -393 100 0 maskCass3Select
xform 0 2240 32
use embbi 1728 567 100 0 maskCass2Select
xform 0 2240 992
use embbi 1728 1527 100 0 maskCass1Select
xform 0 2240 1952
use egenSubB 576 -345 100 0 getCassette3Names
xform 0 720 80
p 656 448 100 0 1 FTA:LONG
p 656 416 100 0 1 FTB:LONG
p 656 384 100 0 1 FTC:STRING
p 672 160 100 0 1 FTJ:STRING
p 672 96 100 0 1 FTL:STRING
p 672 32 100 0 1 FTN:STRING
p 1280 464 100 0 1 FTVA:STRING
p 1280 464 100 0 1 FTVB:STRING
p 1280 432 100 0 1 FTVC:STRING
p 1280 400 100 0 1 FTVD:STRING
p 1280 368 100 0 1 FTVE:STRING
p 1280 304 100 0 1 FTVF:STRING
p 1280 304 100 0 1 FTVG:STRING
p 1280 272 100 0 1 FTVH:STRING
p 1280 240 100 0 1 FTVI:STRING
p 1280 208 100 0 0 FTVJ:STRING
p 1280 176 100 0 0 FTVK:STRING
p 1280 144 100 0 0 FTVL:STRING
p 1280 112 100 0 0 FTVM:STRING
p 1280 80 100 0 0 FTVN:STRING
p 1280 48 100 0 0 FTVO:STRING
p 1280 16 100 0 0 FTVP:STRING
p 544 -384 100 0 1 SNAM:gmSeqMaskCassetteNames
p 928 480 100 0 -1 def(OUTA):$(gm)maskCass3Select.ZRST
p 960 448 100 0 -1 def(OUTB):$(gm)maskCass3Select.ONST
p 960 416 100 0 -1 def(OUTC):$(gm)maskCass3Select.TWST
p 960 384 100 0 -1 def(OUTD):$(gm)maskCass3Select.THST
p 960 352 100 0 -1 def(OUTE):$(gm)maskCass3Select.FRST
p 960 320 100 0 -1 def(OUTF):$(gm)maskCass3Select.FVST
p 960 288 100 0 -1 def(OUTG):$(gm)maskCass3Select.SXST
p 960 256 100 0 -1 def(OUTH):$(gm)maskCass3Select.SVST
p 960 224 100 0 -1 def(OUTI):$(gm)maskCass3Select.EIST
p 864 458 75 0 -1 pproc(OUTA):NPP
use egenSubB 576 615 100 0 getCassette2Names
xform 0 720 1040
p 656 1408 100 0 1 FTA:LONG
p 656 1376 100 0 1 FTB:LONG
p 656 1344 100 0 1 FTC:STRING
p 672 1120 100 0 1 FTJ:STRING
p 672 1056 100 0 1 FTL:STRING
p 672 992 100 0 1 FTN:STRING
p 1280 1424 100 0 1 FTVA:STRING
p 1280 1424 100 0 1 FTVB:STRING
p 1280 1392 100 0 1 FTVC:STRING
p 1280 1360 100 0 1 FTVD:STRING
p 1280 1328 100 0 1 FTVE:STRING
p 1280 1264 100 0 1 FTVF:STRING
p 1280 1264 100 0 1 FTVG:STRING
p 1280 1232 100 0 1 FTVH:STRING
p 1280 1200 100 0 1 FTVI:STRING
p 1280 1168 100 0 0 FTVJ:STRING
p 1280 1136 100 0 0 FTVK:STRING
p 1280 1104 100 0 0 FTVL:STRING
p 1280 1072 100 0 0 FTVM:STRING
p 1280 1040 100 0 0 FTVN:STRING
p 1280 1008 100 0 0 FTVO:STRING
p 1280 976 100 0 0 FTVP:STRING
p 544 576 100 0 1 SNAM:gmSeqMaskCassetteNames
p 928 1440 100 0 -1 def(OUTA):$(gm)maskCass2Select.ZRST
p 960 1408 100 0 -1 def(OUTB):$(gm)maskCass2Select.ONST
p 960 1376 100 0 -1 def(OUTC):$(gm)maskCass2Select.TWST
p 960 1344 100 0 -1 def(OUTD):$(gm)maskCass2Select.THST
p 960 1312 100 0 -1 def(OUTE):$(gm)maskCass2Select.FRST
p 960 1280 100 0 -1 def(OUTF):$(gm)maskCass2Select.FVST
p 960 1248 100 0 -1 def(OUTG):$(gm)maskCass2Select.SXST
p 960 1216 100 0 -1 def(OUTH):$(gm)maskCass2Select.SVST
p 960 1184 100 0 -1 def(OUTI):$(gm)maskCass2Select.EIST
p 864 1418 75 0 -1 pproc(OUTA):NPP
use egenSubB 576 1575 100 0 getCassette1Names
xform 0 720 2000
p 656 2368 100 0 1 FTA:LONG
p 656 2336 100 0 1 FTB:LONG
p 656 2304 100 0 1 FTC:STRING
p 672 2080 100 0 1 FTJ:STRING
p 672 2016 100 0 1 FTL:STRING
p 672 1952 100 0 1 FTN:STRING
p 1280 2384 100 0 1 FTVA:STRING
p 1280 2384 100 0 1 FTVB:STRING
p 1280 2352 100 0 1 FTVC:STRING
p 1280 2320 100 0 1 FTVD:STRING
p 1280 2288 100 0 1 FTVE:STRING
p 1280 2224 100 0 1 FTVF:STRING
p 1280 2224 100 0 1 FTVG:STRING
p 1280 2192 100 0 1 FTVH:STRING
p 1280 2160 100 0 1 FTVI:STRING
p 1280 2128 100 0 0 FTVJ:STRING
p 1280 2096 100 0 0 FTVK:STRING
p 1280 2064 100 0 0 FTVL:STRING
p 1280 2032 100 0 0 FTVM:STRING
p 1280 2000 100 0 0 FTVN:STRING
p 1280 1968 100 0 0 FTVO:STRING
p 1280 1936 100 0 0 FTVP:STRING
p 544 1536 100 0 1 SNAM:gmSeqMaskCassetteNames
p 928 2400 100 0 -1 def(OUTA):$(gm)maskCass1Select.ZRST
p 960 2368 100 0 -1 def(OUTB):$(gm)maskCass1Select.ONST
p 960 2336 100 0 -1 def(OUTC):$(gm)maskCass1Select.TWST
p 960 2304 100 0 -1 def(OUTD):$(gm)maskCass1Select.THST
p 960 2272 100 0 -1 def(OUTE):$(gm)maskCass1Select.FRST
p 960 2240 100 0 -1 def(OUTF):$(gm)maskCass1Select.FVST
p 960 2208 100 0 -1 def(OUTG):$(gm)maskCass1Select.SXST
p 960 2176 100 0 -1 def(OUTH):$(gm)maskCass1Select.SVST
p 960 2144 100 0 -1 def(OUTI):$(gm)maskCass1Select.EIST
p 864 2378 75 0 -1 pproc(OUTA):NPP
use estringouts 2848 -57 100 0 maskName3Out
xform 0 2976 16
p 2912 -112 100 0 1 OMSL:closed_loop
p 3200 0 100 0 -1 def(OUT):$(gm)mskPos.A
p 3104 0 75 768 -1 pproc(OUT):NPP
use estringouts 2976 -377 100 0 maskLoc3Out
xform 0 3104 -304
p 3040 -416 100 0 1 OMSL:closed_loop
p 3056 -240 100 0 1 VAL:IN-BEAM
p 3232 -352 100 0 -1 def(OUT):$(gm)mskPos.B
use estringouts 2976 583 100 0 maskLoc2Out
xform 0 3104 656
p 3040 544 100 0 1 OMSL:closed_loop
p 3056 720 100 0 1 VAL:IN-BEAM
p 3232 608 100 0 -1 def(OUT):$(gm)mskPos.B
use estringouts 2848 903 100 0 maskName2Out
xform 0 2976 976
p 2912 848 100 0 1 OMSL:closed_loop
p 3200 960 100 0 -1 def(OUT):$(gm)mskPos.A
p 3104 960 75 768 -1 pproc(OUT):NPP
use estringouts 2976 1543 100 0 MaskLoc1Out
xform 0 3104 1616
p 3040 1504 100 0 1 OMSL:closed_loop
p 3056 1680 100 0 1 VAL:IN-BEAM
p 3232 1568 100 0 -1 def(OUT):$(gm)mskPos.B
use estringouts 2848 1863 100 0 maskName1Out
xform 0 2976 1936
p 2912 1808 100 0 1 OMSL:closed_loop
p 3200 1920 100 0 -1 def(OUT):$(gm)mskPos.A
p 3104 1920 75 768 -1 pproc(OUT):NPP
use hwin 384 119 100 0 hwin#72
xform 0 480 160
p 272 160 100 0 -1 val(in):$(top)dirLUT.VAL
use hwin 384 55 100 0 hwin#71
xform 0 480 96
p 272 96 100 0 -1 val(in):$(top)mskLUT.VAL
use hwin 384 -9 100 0 hwin#70
xform 0 480 32
p 272 32 100 0 -1 val(in):$(top)mskNamesLUT.VAL
use hwin -64 423 100 0 hwin#67
xform 0 32 464
p -61 456 100 0 -1 val(in):3
use hwin -64 1383 100 0 hwin#64
xform 0 32 1424
p -61 1416 100 0 -1 val(in):2
use hwin 384 951 100 0 hwin#61
xform 0 480 992
p 272 992 100 0 -1 val(in):$(top)mskNamesLUT.VAL
use hwin 384 1015 100 0 hwin#60
xform 0 480 1056
p 272 1056 100 0 -1 val(in):$(top)mskLUT.VAL
use hwin 384 1079 100 0 hwin#59
xform 0 480 1120
p 272 1120 100 0 -1 val(in):$(top)dirLUT.VAL
use hwin -64 2343 100 0 hwin#47
xform 0 32 2384
p -61 2376 100 0 -1 val(in):1
use hwin 384 1911 100 0 hwin#42
xform 0 480 1952
p 272 1952 100 0 -1 val(in):$(top)mskNamesLUT.VAL
use hwin 384 1975 100 0 hwin#39
xform 0 480 2016
p 272 2016 100 0 -1 val(in):$(top)mskLUT.VAL
use hwin 384 2039 100 0 hwin#37
xform 0 480 2080
p 272 2080 100 0 -1 val(in):$(top)dirLUT.VAL
use elongins 128 359 100 0 cass3
xform 0 256 432
use elongins 128 1319 100 0 cass2
xform 0 256 1392
use elongins 128 2279 100 0 cass1
xform 0 256 2352
use ukatcBorderD -960 -761 100 0 ukatcBorderD#44
xform 0 1680 944
p 2000 -624 150 0 -1 Rev:$Revision$
p 3900 -512 120 256 -1 Title:Load mask selection menus
use efanouts -544 983 100 0 getMaskNames
xform 0 -424 1136
p -528 1248 100 0 1 SELM:All
use inhier -816 1079 100 0 SLNK
xform 0 -800 1120
[comments]

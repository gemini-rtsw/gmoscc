[schematic2]
uniq 54
[tools]
[detail]
w 40 331 100 0 n#53 hwin.hwin#52.in 64 320 64 320 egenSubB.getWheel1Names.INPN
w 40 1451 100 0 n#51 hwin.hwin#50.in 64 1440 64 1440 egenSubB.getWheel2Names.INPN
w 40 1579 100 0 n#49 hwin.hwin#46.in 64 1568 64 1568 egenSubB.getWheel2Names.INPJ
w 40 459 100 0 n#48 hwin.hwin#45.in 64 448 64 448 egenSubB.getWheel1Names.INPJ
w 40 1515 100 0 n#44 hwin.hwin#40.in 64 1504 64 1504 egenSubB.getWheel2Names.INPL
w 40 395 100 0 n#43 hwin.hwin#42.in 64 384 64 384 egenSubB.getWheel1Names.INPL
w 2240 459 100 0 n#34 embbi.filtWheel1Select.VAL 2192 448 2336 448 estringouts.wheel1NameOut.DOL
w 2272 427 100 0 n#38 embbi.filtWheel1Select.FLNK 2192 640 2256 640 2256 416 2336 416 estringouts.wheel1NameOut.SLNK
w 2224 1467 100 0 n#31 embbi.filtWheel2Select.VAL 2176 1456 2320 1456 estringouts.wheel2NameOut.DOL
w 2256 1435 100 0 n#30 embbi.filtWheel2Select.FLNK 2176 1648 2240 1648 2240 1424 2320 1424 estringouts.wheel2NameOut.SLNK
w -20 603 100 0 n#27 eais.wheelVal1.VAL -176 512 -16 512 -16 704 64 704 egenSubB.getWheel1Names.INPB
w -424 603 100 0 n#25 hwin.hwin#17.in -320 624 -288 624 -288 592 -512 592 -512 560 -432 560 eais.wheelVal1.INP
w -24 1827 100 0 n#23 eais.wheelVal2.VAL -64 1824 64 1824 egenSubB.getWheel2Names.INPB
w -316 1867 100 2 n#22 hwin.hwin#15.in -320 1872 -320 1872 eais.wheelVal2.INP
w -52 467 100 0 n#13 efanouts.getWheelNames.LNK2 -128 912 -48 912 -48 32 64 32 egenSubB.getWheel1Names.SLNK
w -52 1043 100 0 n#12 efanouts.getWheelNames.LNK1 -128 944 -48 944 -48 1152 64 1152 egenSubB.getWheel2Names.SLNK
w -440 875 100 0 SLNK inhier.SLNK.P -464 864 -368 864 efanouts.getWheelNames.SLNK
s 352 1056 200 0 These gensub routines combine two
s 352 992 200 0 lookup files and output a list of
s 352 928 200 0 names into the MBBI state strings
[cell use]
use hwin -128 1399 100 0 hwin#50
xform 0 -32 1440
p -288 1440 100 0 -1 val(in):$(top)fltNamesLUT.VAL
use hwin -128 343 100 0 hwin#42
xform 0 -32 384
p -224 384 100 0 -1 val(in):$(top)fltLUT.VAL
use hwin -128 1463 100 0 hwin#40
xform 0 -32 1504
p -224 1504 100 0 -1 val(in):$(top)fltLUT.VAL
use hwin -512 583 100 0 hwin#17
xform 0 -416 624
p -509 616 100 0 -1 val(in):1
use hwin -512 1831 100 0 hwin#15
xform 0 -416 1872
p -509 1864 100 0 -1 val(in):2
use hwin -128 407 100 0 hwin#45
xform 0 -32 448
p -224 448 100 0 -1 val(in):$(top)dirLUT.VAL
use hwin -128 1527 100 0 hwin#46
xform 0 -32 1568
p -224 1568 100 0 -1 val(in):$(top)dirLUT.VAL
use hwin -128 279 100 0 hwin#52
xform 0 -32 320
p -288 320 100 0 -1 val(in):$(top)fltNamesLUT.VAL
use estringouts 2336 343 100 0 wheel1NameOut
xform 0 2464 416
p 2400 304 100 0 1 OMSL:closed_loop
p 2496 352 100 0 -1 def(OUT):$(gm)fltPos.A
p 2592 400 75 768 -1 pproc(OUT):NPP
use estringouts 2320 1351 100 0 wheel2NameOut
xform 0 2448 1424
p 2384 1296 100 0 1 OMSL:closed_loop
p 2480 1360 100 0 -1 def(OUT):$(gm)fltPos.B
p 2576 1408 75 768 -1 pproc(OUT):NPP
use eais -432 455 100 0 wheelVal1
xform 0 -304 528
use eais -320 1767 100 0 wheelVal2
xform 0 -192 1840
use efanouts -368 727 100 0 getWheelNames
xform 0 -248 880
p -336 992 100 0 1 SELM:All
use inhier -480 823 100 0 SLNK
xform 0 -464 864
use egenSubB 64 -57 100 0 getWheel1Names
xform 0 208 368
p 144 736 100 0 1 FTA:LONG
p 144 704 100 0 1 FTB:LONG
p 144 672 100 0 1 FTC:STRING
p 144 608 100 0 1 FTE:STRING
p -159 -539 100 0 0 FTJ:STRING
p -159 -539 100 0 0 FTL:STRING
p -159 -539 100 0 0 FTN:STRING
p 848 720 100 0 1 FTVA:STRING
p 848 720 100 0 1 FTVB:STRING
p 848 688 100 0 1 FTVC:STRING
p 848 656 100 0 1 FTVD:STRING
p 848 624 100 0 1 FTVE:STRING
p 848 560 100 0 1 FTVF:STRING
p 848 560 100 0 1 FTVG:STRING
p 848 528 100 0 1 FTVH:STRING
p 848 496 100 0 1 FTVI:STRING
p 848 464 100 0 1 FTVJ:STRING
p 848 464 100 0 1 FTVK:STRING
p 848 464 100 0 1 FTVL:STRING
p 848 464 100 0 1 FTVM:STRING
p 32 -96 100 0 1 SNAM:gmSeqFilterWheelNames
p 416 768 100 0 -1 def(OUTA):$(gm)filtWheel1Select.ZRST
p 448 736 100 0 -1 def(OUTB):$(gm)filtWheel1Select.ONST
p 448 704 100 0 -1 def(OUTC):$(gm)filtWheel1Select.TWST
p 448 672 100 0 -1 def(OUTD):$(gm)filtWheel1Select.THST
p 448 640 100 0 -1 def(OUTE):$(gm)filtWheel1Select.FRST
p 448 608 100 0 -1 def(OUTF):$(gm)filtWheel1Select.FVST
p 448 576 100 0 -1 def(OUTG):$(gm)filtWheel1Select.SXST
p 448 544 100 0 -1 def(OUTH):$(gm)filtWheel1Select.SVST
p 448 512 100 0 -1 def(OUTI):$(gm)filtWheel1Select.EIST
p 448 480 100 0 -1 def(OUTJ):$(gm)filtWheel1Select.NIST
p 448 432 100 0 -1 def(OUTK):$(gm)filtWheel1Select.TEST
p 448 432 100 0 -1 def(OUTL):$(gm)filtWheel1Select.ELST
p 352 746 75 0 -1 pproc(OUTA):NPP
p 352 714 75 0 -1 pproc(OUTB):NPP
p 352 682 75 0 -1 pproc(OUTC):NPP
p 352 650 75 0 -1 pproc(OUTD):NPP
p 352 618 75 0 -1 pproc(OUTE):NPP
p 352 586 75 0 -1 pproc(OUTF):NPP
p 352 554 75 0 -1 pproc(OUTG):NPP
p 352 522 75 0 -1 pproc(OUTH):NPP
p 352 490 75 0 -1 pproc(OUTI):NPP
p 352 458 75 0 -1 pproc(OUTJ):NPP
p 352 426 75 0 -1 pproc(OUTK):NPP
p 352 394 75 0 -1 pproc(OUTL):NPP
use egenSubB 64 1063 100 0 getWheel2Names
xform 0 208 1488
p 144 1840 100 0 1 FTA:LONG
p 144 1808 100 0 1 FTB:LONG
p 144 1776 100 0 1 FTC:STRING
p 144 1712 100 0 1 FTE:STRING
p 160 1568 100 0 1 FTJ:STRING
p 160 1504 100 0 1 FTL:STRING
p 160 1440 100 0 1 FTN:STRING
p 832 1872 100 0 1 FTVA:STRING
p 832 1872 100 0 1 FTVB:STRING
p 832 1840 100 0 1 FTVC:STRING
p 832 1808 100 0 1 FTVD:STRING
p 832 1776 100 0 1 FTVE:STRING
p 832 1712 100 0 1 FTVF:STRING
p 832 1712 100 0 1 FTVG:STRING
p 832 1680 100 0 1 FTVH:STRING
p 832 1648 100 0 1 FTVI:STRING
p 832 1616 100 0 1 FTVJ:STRING
p 832 1584 100 0 1 FTVK:STRING
p 832 1584 100 0 1 FTVL:STRING
p 832 1584 100 0 1 FTVM:STRING
p 32 1024 100 0 1 SNAM:gmSeqFilterWheelNames
p 416 1888 100 0 -1 def(OUTA):$(gm)filtWheel2Select.ZRST
p 448 1856 100 0 -1 def(OUTB):$(gm)filtWheel2Select.ONST
p 448 1824 100 0 -1 def(OUTC):$(gm)filtWheel2Select.TWST
p 448 1792 100 0 -1 def(OUTD):$(gm)filtWheel2Select.THST
p 448 1760 100 0 -1 def(OUTE):$(gm)filtWheel2Select.FRST
p 448 1728 100 0 -1 def(OUTF):$(gm)filtWheel2Select.FVST
p 448 1696 100 0 -1 def(OUTG):$(gm)filtWheel2Select.SXST
p 448 1664 100 0 -1 def(OUTH):$(gm)filtWheel2Select.SVST
p 448 1632 100 0 -1 def(OUTI):$(gm)filtWheel2Select.EIST
p 448 1600 100 0 -1 def(OUTJ):$(gm)filtWheel2Select.NIST
p 448 1552 100 0 -1 def(OUTK):$(gm)filtWheel2Select.TEST
p 448 1552 100 0 -1 def(OUTL):$(gm)filtWheel2Select.ELST
p 352 1866 75 0 -1 pproc(OUTA):NPP
p 352 1834 75 0 -1 pproc(OUTB):NPP
p 352 1802 75 0 -1 pproc(OUTC):NPP
p 352 1770 75 0 -1 pproc(OUTD):NPP
p 352 1738 75 0 -1 pproc(OUTE):NPP
p 352 1706 75 0 -1 pproc(OUTF):NPP
p 352 1674 75 0 -1 pproc(OUTG):NPP
p 352 1642 75 0 -1 pproc(OUTH):NPP
p 352 1610 75 0 -1 pproc(OUTI):NPP
p 352 1578 75 0 -1 pproc(OUTJ):NPP
p 352 1546 75 0 -1 pproc(OUTK):NPP
p 352 1514 75 0 -1 pproc(OUTL):NPP
use embbi 1168 7 100 0 filtWheel1Select
xform 0 1680 432
use embbi 1152 1015 100 0 filtWheel2Select
xform 0 1664 1440
use oslBorderC -624 -425 100 0 oslBorderC#0
xform 0 1056 880
[comments]

[schematic2]
uniq 231
[tools]
[detail]
w 626 1611 100 0 n#230 egenSubA.LimitCheck.FLNK 576 1472 608 1472 608 1600 704 1600 ebos.IlockOut.SLNK
w 964 1563 100 2 n#229 hwout.hwout#228.outp 960 1568 960 1568 ebos.IlockOut.OUT
w 610 1579 100 0 n#227 ebos.IlockOut.SDIS 704 1568 576 1568 egenSubA.LimitCheck.VALU
w 610 1643 100 0 n#226 ebos.IlockOut.DOL 704 1632 576 1632 egenSubA.LimitCheck.VALS
w 292 1851 100 2 n#222 hwin.hwin#221.in 288 1856 288 1856 egenSubA.LimitCheck.INPL
w 292 1915 100 2 n#220 hwin.hwin#219.in 288 1920 288 1920 egenSubA.LimitCheck.INPJ
w 292 1979 100 2 n#218 hwin.hwin#217.in 288 1984 288 1984 egenSubA.LimitCheck.INPH
w 292 2043 100 2 n#216 hwin.hwin#215.in 288 2048 288 2048 egenSubA.LimitCheck.INPF
w 292 2107 100 2 n#214 hwin.hwin#213.in 288 2112 288 2112 egenSubA.LimitCheck.INPD
w 292 2171 100 2 n#210 hwin.hwin#209.in 288 2176 288 2176 egenSubA.LimitCheck.INPB
w 2258 475 100 0 n#159 egenSub.AbsAngles.VALD 2096 944 2224 944 2224 464 2352 464 eaos.AbsY.DOL
w 2378 827 -100 0 n#158 egenSub.AbsAngles.VALC 2096 1008 2320 1008 2320 816 2496 816 eaos.AbsX.DOL
w 1442 539 100 0 n#153 eais.CosPko.VAL 944 144 1136 144 1136 528 1808 528 egenSub.AbsAngles.INPJ
w 1410 603 100 0 n#152 eais.SinPko.VAL 944 400 1072 400 1072 592 1808 592 egenSub.AbsAngles.INPI
w 1346 667 100 0 n#151 eais.CosBas.VAL 944 656 1808 656 egenSub.AbsAngles.INPH
w 1474 731 100 0 n#150 eais.SinBas.VAL 944 912 1200 912 1200 720 1808 720 egenSub.AbsAngles.INPG
w 1442 1051 100 0 n#149 eais.PickoffZero.VAL 944 1136 1136 1136 1136 1040 1808 1040 egenSub.AbsAngles.INPB
w 1474 1115 100 0 n#148 eais.BaseZero.VAL 944 1328 1200 1328 1200 1104 1808 1104 egenSub.AbsAngles.INPA
w 658 971 100 0 n#147 hwin.hwin#20.in 688 960 688 960 eais.SinBas.INP
w 658 715 100 0 n#146 hwin.hwin#18.in 688 704 688 704 eais.CosBas.INP
w 658 459 100 0 n#145 hwin.hwin#19.in 688 448 688 448 eais.SinPko.INP
w 658 203 100 0 n#144 hwin.hwin#17.in 688 192 688 192 eais.CosPko.INP
w 2226 1371 100 0 IncrPko eaos.EncPko.OUT 2080 1360 2432 1360 outhier.IncrPko.p
w 2226 1627 100 0 IncrBas eaos.EncBas.OUT 2080 1616 2432 1616 outhier.IncrBas.p
w 2240 1051 -100 0 c#94 egenSub.AbsAngles.OUTB 2096 1040 2432 1040 outhier.AbsPko.p
w 2240 1115 -100 0 c#89 egenSub.AbsAngles.OUTA 2096 1104 2432 1104 outhier.AbsBas.p
w 2234 923 -100 0 n#155 egenSub.AbsAngles.OUTD 2096 912 2432 912 outhier.Y.p
w 2234 987 -100 0 n#154 egenSub.AbsAngles.OUTC 2096 976 2432 976 outhier.X.p
s 592 2224 100 0 Limit Status Message
s 608 2144 100 0 Effective Y Lo Limit
s 608 2080 100 0 Effective Y Hi Limit
s -112 1760 100 0 Buffer btw Limit and Patrol Area
[cell use]
use hwout 960 1527 100 0 hwout#228
xform 0 1056 1568
p 1168 1552 100 0 -1 val(outp):$(top)$(dev)Ilock PP NMS
use ebos 848 1680 100 0 IlockOut
xform 0 832 1600
p 384 1550 100 0 0 OMSL:closed_loop
p 704 1680 100 0 -1 PV:$(top)$(dev)
p 960 1568 75 768 -1 pproc(OUT):PP
use hwin 96 1815 100 0 hwin#221
xform 0 192 1856
p 80 1840 100 512 -1 val(in):$(top)$(dev)PatrolYHiLim
use hwin 96 1879 100 0 hwin#219
xform 0 192 1920
p 80 1904 100 512 -1 val(in):$(top)$(dev)PatrolYLoLim
use hwin 96 1943 100 0 hwin#217
xform 0 192 1984
p 80 1968 100 512 -1 val(in):$(top)$(dev)PatrolXHiLim
use hwin 96 2007 100 0 hwin#215
xform 0 192 2048
p 80 2032 100 512 -1 val(in):$(top)$(dev)PatrolXLoLim
use hwin 96 2071 100 0 hwin#213
xform 0 192 2112
p 80 2096 100 512 -1 val(in):$(top)$(dev)AbsAngles.VALD
use hwin 96 2135 100 0 hwin#209
xform 0 192 2176
p 80 2160 100 512 -1 val(in):$(top)$(dev)AbsAngles.VALC
use hwin 496 919 100 0 hwin#20
xform 0 592 960
p 499 952 100 512 -1 val(in):#<$(sinBas)>
use hwin 496 407 100 0 hwin#19
xform 0 592 448
p 499 440 100 512 -1 val(in):#<$(sinPko)>
use hwin 496 663 100 0 hwin#18
xform 0 592 704
p 499 696 100 512 -1 val(in):#<$(cosBas)>
use hwin 496 151 100 0 hwin#17
xform 0 592 192
p 499 184 100 512 -1 val(in):#<$(cosPko)>
use egenSubA 432 2256 100 0 LimitCheck
xform 0 432 1840
p 432 2208 100 0 -1 FTVA:STRING
p 448 1632 100 0 -1 FTVS:LONG
p 65 933 100 0 0 FTVT:DOUBLE
p 448 1568 100 0 -1 FTVU:LONG
p 320 1424 100 0 1 INAM:oiInitLimitCheck
p 0 1966 100 0 0 PREC:3
p 272 2256 100 0 -1 PV:$(top)$(dev)
p 320 1360 100 0 1 SCAN:.1 second
p 320 1392 100 0 1 SNAM:oiLimitCheck
use eaos 2512 512 100 0 AbsY
xform 0 2480 432
p 2416 208 100 0 1 EGU:mm
p 2416 272 100 0 0 EGUF:0
p 2416 240 100 0 0 EGUL:0
p 2416 272 100 0 1 HOPR:140.0
p 2416 240 100 0 1 LOPR:-30.0
p 2416 304 100 0 1 OMSL:closed_loop
p 2512 512 100 512 -1 PV:$(top)$(dev)
p 2416 336 100 0 1 SCAN:.1 second
use eaos 2656 864 100 0 AbsX
xform 0 2624 784
p 2560 560 100 0 1 EGU:mm
p 2560 624 100 0 0 EGUF:0
p 2560 592 100 0 0 EGUL:0
p 2560 624 100 0 1 HOPR:130.0
p 2560 592 100 0 1 LOPR:-10.0
p 2560 656 100 0 1 OMSL:closed_loop
p 2656 864 100 512 -1 PV:$(top)$(dev)
p 2560 688 100 0 1 SCAN:.1 second
use eaos 2080 1728 100 0 EncBas
xform 0 1952 1648
p 1792 1374 100 0 0 EGU:motor units
p 1824 1552 100 0 1 OMSL:closed_loop
p 1872 1728 100 0 1 PV:$(top)$(dev)
p 1824 1520 100 0 1 SCAN:.1 second
p 1744 1680 100 512 1 def(DOL):$(top)$(dev)BasDevice.MPOS
p 2080 1568 100 0 0 def(OUT):0.0
use eaos 2080 1472 100 0 EncPko
xform 0 1952 1392
p 1792 1118 100 0 0 EGU:motor units
p 1824 1296 100 0 1 OMSL:closed_loop
p 2064 1472 100 512 1 PV:$(top)$(dev)
p 1824 1264 100 0 1 SCAN:.1 second
p 1744 1424 100 512 1 def(DOL):$(top)$(dev)PkoDevice.MPOS
p 2080 1328 100 0 0 def(OUT):0.0
use eais 944 1408 100 0 BaseZero
xform 0 816 1344
p 432 1086 100 0 0 EGU:encoder units
p 816 1312 70 256 1 PREC:1
p 752 1408 100 0 1 PV:$(top)$(dev)
use eais 944 1216 100 0 PickoffZero
xform 0 816 1152
p 432 894 100 0 0 EGU:encoder units
p 816 1120 70 256 1 PREC:1
p 752 1216 100 0 1 PV:$(top)$(dev)
use eais 944 992 100 0 SinBas
xform 0 816 928
p 624 848 100 0 1 DTYP:XYCOM-566 SE Scanned
p 432 670 100 0 0 EGU:encoder units
p 624 816 100 0 1 EGUF:1.0
p 624 784 100 0 1 EGUL:-1.0
p 432 766 100 0 0 PREC:4
p 752 992 100 0 1 PV:$(top)$(dev)
p 816 816 100 0 1 SCAN:Passive
use eais 944 480 100 0 SinPko
xform 0 816 416
p 608 336 100 0 1 DTYP:XYCOM-566 SE Scanned
p 432 158 100 0 0 EGU:encoder units
p 608 304 100 0 1 EGUF:1.0
p 608 272 100 0 1 EGUL:-1.0
p 432 254 100 0 0 PREC:4
p 752 480 100 0 1 PV:$(top)$(dev)
p 816 304 100 0 1 SCAN:Passive
use eais 944 736 100 0 CosBas
xform 0 816 672
p 608 592 100 0 1 DTYP:XYCOM-566 SE Scanned
p 432 414 100 0 0 EGU:encoder units
p 608 560 100 0 1 EGUF:1.0
p 608 528 100 0 1 EGUL:-1.0
p 432 510 100 0 0 PREC:4
p 752 736 100 0 1 PV:$(top)$(dev)
p 816 560 100 0 1 SCAN:Passive
use eais 944 224 100 0 CosPko
xform 0 816 160
p 608 80 100 0 1 DTYP:XYCOM-566 SE Scanned
p 432 -98 100 0 0 EGU:encoder units
p 608 48 100 0 1 EGUF:1.0
p 608 16 100 0 1 EGUL:-1.0
p 432 -2 100 0 0 PREC:4
p 752 224 100 0 1 PV:$(top)$(dev)
p 816 48 100 0 1 SCAN:Passive
use outhier 2448 1616 100 0 IncrBas
xform 0 2416 1616
use outhier 2448 1360 100 0 IncrPko
xform 0 2416 1360
use outhier 2448 1040 100 0 AbsPko
xform 0 2416 1040
use outhier 2448 1104 100 0 AbsBas
xform 0 2416 1104
use outhier 2448 976 100 0 X
xform 0 2416 976
use outhier 2448 912 100 0 Y
xform 0 2416 912
use egenSub 2096 1200 100 0 AbsAngles
xform 0 1952 768
p 1888 1104 70 0 -1 FTA:DOUBLE
p 1888 1040 70 0 -1 FTB:DOUBLE
p 1888 976 70 0 -1 FTC:DOUBLE
p 1888 912 70 0 -1 FTD:DOUBLE
p 1888 848 70 0 -1 FTE:DOUBLE
p 1888 784 70 0 -1 FTF:DOUBLE
p 1888 720 70 0 -1 FTG:DOUBLE
p 1888 656 70 0 -1 FTH:DOUBLE
p 1888 592 70 0 -1 FTI:DOUBLE
p 1888 528 70 0 -1 FTJ:DOUBLE
p 2016 1104 70 512 -1 FTVA:DOUBLE
p 2016 1040 70 512 -1 FTVB:DOUBLE
p 2016 976 70 512 -1 FTVC:DOUBLE
p 2016 912 70 512 -1 FTVD:DOUBLE
p 1840 272 100 0 1 INAM:oiInitCalcAbsAngles
p 1840 304 70 0 1 PREC:3
p 2080 1200 100 512 1 PV:$(top)$(dev)
p 1952 384 70 256 1 SCAN:.1 second
p 1952 336 100 256 1 SNAM:oiCalcAbsAngles
p 1728 928 70 512 1 def(INPC):$(top)$(dev)BasOffset
p 1728 896 70 512 1 def(INPD):$(top)$(dev)PkoOffset
p 1728 864 70 512 1 def(INPE):$(top)$(dev)XOffset
p 1728 832 70 512 1 def(INPF):$(top)$(dev)YOffset
p 1760 1114 75 0 -1 pproc(INPA):NPP
p 1760 1050 75 0 -1 pproc(INPB):NPP
p 1760 986 75 0 -1 pproc(INPC):NPP
p 1760 922 75 0 -1 pproc(INPD):NPP
p 1760 730 75 0 -1 pproc(INPG):PP
p 1760 666 75 0 -1 pproc(INPH):PP
p 1760 602 75 0 -1 pproc(INPI):PP
p 1760 538 75 0 -1 pproc(INPJ):PP
use gmosBorderC -416 -153 100 0 gmosBorderC#0
xform 0 1264 1152
p 2832 -24 100 512 1 File:gmOiwfsEncoders.sch
p 2244 20 150 0 1 Rev:A
p 2524 96 120 256 -1 Title:OIWFS Sin/Cos Encoders
p 2564 32 100 1024 -1 author:B.Wooff
p 2564 0 100 1024 -1 date:October 3, 2000
[comments]

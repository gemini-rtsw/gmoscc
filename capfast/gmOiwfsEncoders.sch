[schematic2]
uniq 160
[tools]
[detail]
w 1954 811 100 0 n#159 egenSub.AbsAngles.VALD 1792 1280 1920 1280 1920 800 2048 800 eaos.AbsY.DOL
w 2074 1163 -100 0 n#158 egenSub.AbsAngles.VALC 1792 1344 2016 1344 2016 1152 2192 1152 eaos.AbsX.DOL
w 1138 875 100 0 n#153 eais.CosPko.VAL 640 480 832 480 832 864 1504 864 egenSub.AbsAngles.INPJ
w 1106 939 100 0 n#152 eais.SinPko.VAL 640 736 768 736 768 928 1504 928 egenSub.AbsAngles.INPI
w 1042 1003 100 0 n#151 eais.CosBas.VAL 640 992 1504 992 egenSub.AbsAngles.INPH
w 1170 1067 100 0 n#150 eais.SinBas.VAL 640 1248 896 1248 896 1056 1504 1056 egenSub.AbsAngles.INPG
w 1138 1387 100 0 n#149 eais.PickoffZero.VAL 640 1472 832 1472 832 1376 1504 1376 egenSub.AbsAngles.INPB
w 1170 1451 100 0 n#148 eais.BaseZero.VAL 640 1664 896 1664 896 1440 1504 1440 egenSub.AbsAngles.INPA
w 354 1307 100 0 n#147 hwin.hwin#20.in 384 1296 384 1296 eais.SinBas.INP
w 354 1051 100 0 n#146 hwin.hwin#18.in 384 1040 384 1040 eais.CosBas.INP
w 354 795 100 0 n#145 hwin.hwin#19.in 384 784 384 784 eais.SinPko.INP
w 354 539 100 0 n#144 hwin.hwin#17.in 384 528 384 528 eais.CosPko.INP
w 1922 1707 100 0 IncrPko eaos.EncPko.OUT 1776 1696 2128 1696 outhier.IncrPko.p
w 1922 1963 100 0 IncrBas eaos.EncBas.OUT 1776 1952 2128 1952 outhier.IncrBas.p
w 1936 1387 -100 0 c#94 egenSub.AbsAngles.OUTB 1792 1376 2128 1376 outhier.AbsPko.p
w 1936 1451 -100 0 c#89 egenSub.AbsAngles.OUTA 1792 1440 2128 1440 outhier.AbsBas.p
w 1930 1259 -100 0 n#155 egenSub.AbsAngles.OUTD 1792 1248 2128 1248 outhier.Y.p
w 1930 1323 -100 0 n#154 egenSub.AbsAngles.OUTC 1792 1312 2128 1312 outhier.X.p
[cell use]
use eaos 2208 848 100 0 AbsY
xform 0 2176 768
p 2112 544 100 0 1 EGU:mm
p 2112 608 100 0 0 EGUF:0
p 2112 576 100 0 0 EGUL:0
p 2112 608 100 0 1 HOPR:140.0
p 2112 576 100 0 1 LOPR:-30.0
p 2112 640 100 0 1 OMSL:closed_loop
p 2208 848 100 512 -1 PV:$(top)$(dev)
p 2112 672 100 0 1 SCAN:.1 second
use eaos 2352 1200 100 0 AbsX
xform 0 2320 1120
p 2256 896 100 0 1 EGU:mm
p 2256 960 100 0 0 EGUF:0
p 2256 928 100 0 0 EGUL:0
p 2256 960 100 0 1 HOPR:130.0
p 2256 928 100 0 1 LOPR:-10.0
p 2256 992 100 0 1 OMSL:closed_loop
p 2352 1200 100 512 -1 PV:$(top)$(dev)
p 2256 1024 100 0 1 SCAN:.1 second
use eais 640 1744 100 0 BaseZero
xform 0 512 1680
p 128 1422 100 0 0 EGU:encoder units
p 512 1648 70 256 1 PREC:1
p 448 1744 100 0 1 PV:$(top)$(dev)
use eais 640 1552 100 0 PickoffZero
xform 0 512 1488
p 128 1230 100 0 0 EGU:encoder units
p 512 1456 70 256 1 PREC:1
p 448 1552 100 0 1 PV:$(top)$(dev)
use eais 640 1328 100 0 SinBas
xform 0 512 1264
p 320 1184 100 0 1 DTYP:XYCOM-566 SE Scanned
p 128 1006 100 0 0 EGU:encoder units
p 320 1152 100 0 1 EGUF:1.0
p 320 1120 100 0 1 EGUL:-1.0
p 128 1102 100 0 0 PREC:4
p 448 1328 100 0 1 PV:$(top)$(dev)
p 512 1152 100 0 1 SCAN:Passive
use eais 640 816 100 0 SinPko
xform 0 512 752
p 304 672 100 0 1 DTYP:XYCOM-566 SE Scanned
p 128 494 100 0 0 EGU:encoder units
p 304 640 100 0 1 EGUF:1.0
p 304 608 100 0 1 EGUL:-1.0
p 128 590 100 0 0 PREC:4
p 448 816 100 0 1 PV:$(top)$(dev)
p 512 640 100 0 1 SCAN:Passive
use eais 640 1072 100 0 CosBas
xform 0 512 1008
p 304 928 100 0 1 DTYP:XYCOM-566 SE Scanned
p 128 750 100 0 0 EGU:encoder units
p 304 896 100 0 1 EGUF:1.0
p 304 864 100 0 1 EGUL:-1.0
p 128 846 100 0 0 PREC:4
p 448 1072 100 0 1 PV:$(top)$(dev)
p 512 896 100 0 1 SCAN:Passive
use eais 640 560 100 0 CosPko
xform 0 512 496
p 304 416 100 0 1 DTYP:XYCOM-566 SE Scanned
p 128 238 100 0 0 EGU:encoder units
p 304 384 100 0 1 EGUF:1.0
p 304 352 100 0 1 EGUL:-1.0
p 128 334 100 0 0 PREC:4
p 448 560 100 0 1 PV:$(top)$(dev)
p 512 384 100 0 1 SCAN:Passive
use outhier 2144 1952 100 0 IncrBas
xform 0 2112 1952
use outhier 2144 1696 100 0 IncrPko
xform 0 2112 1696
use outhier 2144 1376 100 0 AbsPko
xform 0 2112 1376
use outhier 2144 1440 100 0 AbsBas
xform 0 2112 1440
use outhier 2144 1312 100 0 X
xform 0 2112 1312
use outhier 2144 1248 100 0 Y
xform 0 2112 1248
use eaos 1776 2064 100 0 EncBas
xform 0 1648 1984
p 1488 1710 100 0 0 EGU:motor units
p 1520 1888 100 0 1 OMSL:closed_loop
p 1568 2064 100 0 1 PV:$(top)$(dev)
p 1520 1856 100 0 1 SCAN:.1 second
p 1440 2016 100 512 1 def(DOL):$(top)$(dev)BasDevice.MPOS
p 1776 1904 100 0 0 def(OUT):0.0
use eaos 1776 1808 100 0 EncPko
xform 0 1648 1728
p 1488 1454 100 0 0 EGU:motor units
p 1520 1632 100 0 1 OMSL:closed_loop
p 1760 1808 100 512 1 PV:$(top)$(dev)
p 1520 1600 100 0 1 SCAN:.1 second
p 1440 1760 100 512 1 def(DOL):$(top)$(dev)PkoDevice.MPOS
p 1776 1664 100 0 0 def(OUT):0.0
use egenSub 1792 1536 100 0 AbsAngles
xform 0 1648 1104
p 1584 1440 70 0 -1 FTA:DOUBLE
p 1584 1376 70 0 -1 FTB:DOUBLE
p 1584 1312 70 0 -1 FTC:DOUBLE
p 1584 1248 70 0 -1 FTD:DOUBLE
p 1584 1184 70 0 -1 FTE:DOUBLE
p 1584 1120 70 0 -1 FTF:DOUBLE
p 1584 1056 70 0 -1 FTG:DOUBLE
p 1584 992 70 0 -1 FTH:DOUBLE
p 1584 928 70 0 -1 FTI:DOUBLE
p 1584 864 70 0 -1 FTJ:DOUBLE
p 1712 1440 70 512 -1 FTVA:DOUBLE
p 1712 1376 70 512 -1 FTVB:DOUBLE
p 1712 1312 70 512 -1 FTVC:DOUBLE
p 1712 1248 70 512 -1 FTVD:DOUBLE
p 1536 640 70 0 1 PREC:3
p 1776 1536 100 512 1 PV:$(top)$(dev)
p 1648 720 70 256 1 SCAN:.1 second
p 1648 672 100 256 1 SNAM:oiCalcAbsAngles
p 1424 1264 70 512 1 def(INPC):$(top)$(dev)BasOffset
p 1424 1232 70 512 1 def(INPD):$(top)$(dev)PkoOffset
p 1424 1200 70 512 1 def(INPE):$(top)$(dev)XOffset
p 1424 1168 70 512 1 def(INPF):$(top)$(dev)YOffset
p 1456 1450 75 0 -1 pproc(INPA):NPP
p 1456 1386 75 0 -1 pproc(INPB):NPP
p 1456 1322 75 0 -1 pproc(INPC):NPP
p 1456 1258 75 0 -1 pproc(INPD):NPP
p 1456 1066 75 0 -1 pproc(INPG):PP
p 1456 1002 75 0 -1 pproc(INPH):PP
p 1456 938 75 0 -1 pproc(INPI):PP
p 1456 874 75 0 -1 pproc(INPJ):PP
use hwin 192 1255 100 0 hwin#20
xform 0 288 1296
p 195 1288 100 512 -1 val(in):#<$(sinBas)>
use hwin 192 743 100 0 hwin#19
xform 0 288 784
p 195 776 100 512 -1 val(in):#<$(sinPko)>
use hwin 192 999 100 0 hwin#18
xform 0 288 1040
p 195 1032 100 512 -1 val(in):#<$(cosBas)>
use hwin 192 487 100 0 hwin#17
xform 0 288 528
p 195 520 100 512 -1 val(in):#<$(cosPko)>
use gmosBorderC -416 -153 100 0 gmosBorderC#0
xform 0 1264 1152
p 2832 -24 100 512 1 File:gmOiwfsEncoders.sch
p 2244 20 150 0 1 Rev:A
p 2524 96 120 256 -1 Title:OIWFS Sin/Cos Encoders
p 2564 32 100 1024 -1 author:B.Wooff
p 2564 0 100 1024 -1 date:October 3, 2000
[comments]

[schematic2]
uniq 62
[tools]
[detail]
w 992 555 100 0 n#61 elongouts.getSubHeartbeat.FLNK 48 1600 128 1600 128 864 544 864 544 544 1488 544 ecalcs.CalcDisv.SLNK
w 800 907 100 0 n#60 junction 160 1568 160 896 1488 896 ecalcs.CalcDisv.INPB
w 228 1675 100 0 n#60 elongouts.getSubHeartbeat.VAL 48 1568 224 1568 224 1792 288 1792 eao.checkHeartBeat.DOL
w -232 1611 100 0 c#54 lboat.c#54.p -208 1600 -208 1600 elongouts.getSubHeartbeat.DOL
w 132 779 100 2 c#57 lboat.c#57.p 128 784 128 784 ecalcs.binHeartBeat.INPA
w 1152 811 100 0 n#52 ebos.DisableSubsys.VAL 1024 800 1328 800 1328 832 1488 832 ecalcs.CalcDisv.INPD
w 2296 770 100 0 n#44 elongouts.Disabled.VAL 2144 768 2496 768 outhier.DISV.p
w 1122 1336 100 0 n#42 eao.checkHeartBeat.U0 928 1760 1120 1760 1120 864 1488 864 ecalcs.CalcDisv.INPC
w 1032 1890 100 0 n#36 eao.checkHeartBeat.VAL 928 1888 1184 1888 1184 2080 1328 2080 eseqs.WaitheartBeat.DOL1
w 1760 1154 100 0 n#41 eaos.OldheartBeat.VAL 2144 2080 2304 2080 2304 1152 1264 1152 1264 928 1488 928 ecalcs.CalcDisv.INPA
w 1744 2082 100 0 n#37 eseqs.WaitheartBeat.LNK1 1648 2080 1888 2080 eaos.OldheartBeat.SLNK
w 1016 1986 100 0 n#35 eao.checkHeartBeat.FLNK 928 1984 1152 1984 1152 1760 1328 1760 eseqs.WaitheartBeat.SLNK
w 1824 802 100 0 n#19 ecalcs.CalcDisv.VAL 1776 736 1808 736 1808 800 1888 800 elongouts.Disabled.DOL
w 1808 770 100 0 n#18 ecalcs.CalcDisv.FLNK 1776 768 1888 768 elongouts.Disabled.SLNK
s 2400 2304 150 0 gmSeqSubsysPresent
s 1168 848 100 0 Alarm
s 1328 944 100 0 Old
s 1696 448 100 0 or check heartbeat alarm raised
s 1696 480 100 0 Subsystem inactive if old = new heartbeat
s -96 1152 140 0 This schematic handles the presence (or absence) of the 
s -96 1120 140 0 Components or Detector Controllers.
s -96 1088 140 0 It outputs status showing if the subsystem is active.
s 1168 912 100 0 New
[cell use]
use elongouts -208 1479 100 0 getSubHeartbeat
xform 0 -80 1568
p -176 1680 100 0 1 OMSL:closed_loop
p -128 1520 100 0 1 SCAN:1 second
p -368 1308 100 0 0 def(DOL):$(gm)sad:$(subsys):heartBeat.VAL
p -208 1600 75 1280 -1 palrm(DOL):MS
use elongouts 1912 680 100 0 Disabled
xform 0 2016 768
p 1856 862 100 0 -1 DESC:Indicates a disabled subsystem
p 1952 624 100 0 1 OMSL:closed_loop
use lboat -32 743 100 0 c#57
xform 0 48 784
use lboat -368 1559 100 0 c#54
xform 0 -288 1600
use ecalcs 128 311 100 0 binHeartBeat
xform 0 272 576
p 224 640 100 0 1 CALC:A%2
p 224 480 100 0 1 SCAN:1 second
p -95 181 100 0 0 def(FLNK):0.000000000000000e+00
p -208 816 100 0 -1 def(INPA):$(gm)sad:$(subsys):heartBeat
use ecalcs 1512 456 100 0 CalcDisv
xform 0 1632 720
p 1584 624 100 0 1 CALC:((A=B)||(C=3))||(D)
p 1600 864 100 0 1 SCAN:Passive
p 992 878 100 0 0 def(INPA):0.0
use ebos 768 711 100 0 DisableSubsys
xform 0 896 800
p 832 656 100 0 1 OMSL:supervisory
p 608 640 100 0 1 ONAM:disable
p 608 672 100 0 1 ZNAM:enable
use oslBorderC -480 -137 100 0 oslBorderC#47
xform 0 1200 1168
p 2460 112 120 256 -1 Title:GMOS IS - check presence of subsystem
use outhier 2488 728 100 0 DISV
xform 0 2480 768
use eao 312 1256 100 0 checkHeartBeat
xform 0 608 1712
p 416 1326 100 0 0 ADEL:0.000000000000000e+00
p 452 2042 100 0 1 DESC:Check alarm on heartBeat
p 640 1902 100 0 0 DISS:NO_ALARM
p 416 1550 100 0 0 DISV:1
p 416 1614 100 0 0 DRVH:0.0000000e+00
p 416 1582 100 0 0 DRVL:0.0000000e+00
p 640 1582 100 0 1 EGU:none
p 416 1678 100 0 0 EGUF:0.0000000e+00
p 416 1646 100 0 0 EGUL:0.0000000e+00
p 416 1902 100 0 0 EVNT:0
p 640 1742 100 0 0 HHSV:NO_ALARM
p 640 1806 100 0 0 HIGH:0.0000000e+00
p 640 1870 100 0 0 HIHI:0.0000000e+00
p 640 1966 100 0 0 HOPR:0.0000000e+00
p 640 1710 100 0 0 HSV:NO_ALARM
p 416 1358 100 0 0 HYST:0.000000000000000e+00
p 416 1422 100 0 0 IVOA:Continue normally
p 416 1390 100 0 0 IVOV:0.000000000000000e+00
p 640 1646 100 0 0 LLSV:NO_ALARM
p 640 1838 100 0 0 LOLO:0.0000000e+00
p 640 1934 100 0 0 LOPR:0.0000000e+00
p 640 1774 100 0 0 LOW:0.0000000e+00
p 640 1678 100 0 0 LSV:NO_ALARM
p 416 1838 100 0 1 OMSL:closed_loop
p 416 1774 100 0 0 OROC:0.0000000e+00
p 416 1934 100 0 0 PHAS:0
p 640 1614 100 0 0 PRIO:LOW
p 416 1966 100 0 1 SCAN:10 second
p 416 1486 100 0 0 SIML:0.000000000000000e+00
p 416 1454 100 0 0 SIMS:NO_ALARM
p 416 1518 100 0 0 SIOL:0.000000000000000e+00
p -368 1648 100 0 1 def(DOL):$(gm)sad:$(subsys):heartBeat.VAL
p 896 1760 100 1024 -1 username(U0):SEVR
use eaos 1912 1992 100 0 OldheartBeat
xform 0 2016 2080
p 1904 2190 100 0 -1 DESC:Old value of heartBeat
p 1952 1936 100 0 1 OMSL:supervisory
use eseqs 1352 1672 100 0 WaitheartBeat
xform 0 1488 1920
p 1440 1982 100 0 1 DLY1:9.9
p 1440 1950 100 0 0 DLY2:0.0
p 1744 2046 100 0 0 def(LNK2):0.0
p 1664 2080 75 1024 -1 pproc(LNK1):PP
p 1664 2048 75 1024 -1 pproc(LNK2):NPP
[comments]

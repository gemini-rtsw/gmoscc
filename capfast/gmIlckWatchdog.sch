[schematic2]
uniq 224
[tools]
[detail]
w -880 1600 100 1280 Door_Interlocks inhier.DOORS.P -1952 128 -1152 128 -1152 1600 -800 1600 egenSub.ilockDecision.G
w 338 875 100 0 n#163 junction 1216 864 -480 864 eseqs.ilockMonWait.DOL1
w 1714 875 100 0 n#163 ecalcs.msEnableAnd.VAL 2144 1216 2272 1216 2272 864 1216 864 1216 672 1280 672 ebos.masterEnable.DOL
w 1860 747 100 0 n#163 junction 1856 864 1856 640 1920 640 ecalcs.msNot.INPA
w -958 555 100 0 n#217 eseqs.ilockMonWait.FLNK -800 544 -1056 544 junction
w -1326 11 100 0 n#217 ebis.softIlock.FLNK -1536 0 -1056 0 -1056 1280 -800 1280 egenSub.ilockDecision.SLNK
w -1326 427 100 0 n#217 ebis.doorIlckDefeat.FLNK -1536 416 -1056 416 junction
w -1326 1067 100 0 n#217 ebis.gisError.FLNK -1536 1056 -1056 1056 junction
w -880 1504 100 1280 Soft_Interlock ebis.softIlock.VAL -1536 -32 -1120 -32 -1120 1504 -800 1504 egenSub.ilockDecision.INPH
w -880 1632 100 1280 Door_Interlock_Defeat ebis.doorIlckDefeat.VAL -1536 384 -1184 384 -1184 1632 -800 1632 egenSub.ilockDecision.INPF
w -880 1696 100 1280 Motor_Shutdown ebis.masterDisabled.VAL -1536 704 -1216 704 -1216 1696 -800 1696 egenSub.ilockDecision.INPE
w -880 1760 100 1280 GIS_Demand_Error ebis.gisError.VAL -1536 1024 -1248 1024 -1248 1760 -800 1760 egenSub.ilockDecision.INPD
w -880 1824 100 1280 GIS_Demand ebis.gisDemand.VAL -1536 1376 -1280 1376 -1280 1824 -800 1824 egenSub.ilockDecision.INPC
w -880 1888 100 1280 GIS_Event_Error ebis.gisEventError.VAL -1536 1728 -1312 1728 -1312 1888 -800 1888 egenSub.ilockDecision.INPB
w -880 2064 100 1280 GIS_Event ebis.gisEvent.VAL -1536 2064 -896 2064 -896 1952 -800 1952 egenSub.ilockDecision.INPA
w -1694 507 100 0 n#198 ebis.masterDisabled.FLNK -1536 736 -1312 736 -1312 496 -2016 496 -2016 400 -1792 400 ebis.doorIlckDefeat.SLNK
w -1822 443 100 0 n#195 hwin.hwin#196.in -1792 432 -1792 432 ebis.doorIlckDefeat.INP
w -380 1563 100 0 n#194 egenSub.ilockDecision.OUTB -512 1888 -384 1888 -384 1248 -224 1248 hwout.hwout#193.outp
w -86 1867 100 0 n#191 edfans.ilockFanout.OUTF -80 1856 -32 1856 hwout.hwout#192.outp
w -86 1899 100 0 n#190 edfans.ilockFanout.OUTE -80 1888 -32 1888 hwout.hwout#136.outp
w -86 1931 100 0 n#189 edfans.ilockFanout.OUTD -80 1920 -32 1920 hwout.hwout#135.outp
w -86 1963 100 0 n#188 edfans.ilockFanout.OUTC -80 1952 -32 1952 hwout.hwout#132.outp
w -86 1995 100 0 n#187 edfans.ilockFanout.OUTB -80 1984 -32 1984 hwout.hwout#184.outp
w -86 2027 100 0 n#186 edfans.ilockFanout.OUTA -80 2016 -32 2016 hwout.hwout#185.outp
w 1362 811 100 0 n#183 ecalcs.msNot.VAL 2208 448 2336 448 2336 800 448 800 448 160 672 160 edfans.msEnableFan.DOL
w 1322 779 100 0 n#182 ecalcs.msNot.FLNK 2208 480 2304 480 2304 768 400 768 400 128 672 128 edfans.msEnableFan.SLNK
w 1828 459 100 0 n#181 ebos.masterEnable.FLNK 1536 672 1824 672 1824 256 1920 256 ecalcs.msNot.SLNK
w 1010 171 100 0 n#222 edfans.msEnableFan.OUTB 928 160 1152 160 1152 0 1280 0 ebos.sensorEnable.SLNK
w 1138 331 100 0 n#176 eseqs.wdgEnableHoldoff.LNK5 896 1552 1056 1552 1056 320 1280 320 ebos.optsensorEnable.SLNK
w 1682 907 100 0 n#169 ecalcs.msEnableAnd.FLNK 2144 1248 2240 1248 2240 896 1184 896 1184 640 1280 640 ebos.masterEnable.SLNK
w 370 -117 100 0 n#158 edfans.msEnableFan.FLNK 928 288 1056 288 1056 -128 -256 -128 -256 544 -480 544 eseqs.ilockMonWait.SLNK
w -880 1472 100 1280 Master_Enable eseqs.ilockMonWait.LNK1 -800 864 -992 864 -992 1472 -800 1472 egenSub.ilockDecision.I
w -1694 1163 100 0 n#151 ebis.gisDemand.FLNK -1536 1408 -1312 1408 -1312 1152 -2016 1152 -2016 1040 -1792 1040 ebis.gisError.SLNK
w -1710 1507 100 0 n#150 ebis.gisEventError.FLNK -1536 1760 -1344 1760 -1344 1504 -2016 1504 -2016 1392 -1792 1392 ebis.gisDemand.SLNK
w -1710 1867 100 0 n#149 ebis.gisEvent.FLNK -1536 2096 -1344 2096 -1344 1856 -2016 1856 -2016 1744 -1792 1744 ebis.gisEventError.SLNK
w -206 1699 100 0 n#145 edfans.ilockFanout.OUTH -80 1792 0 1792 0 1696 -352 1696 -352 1536 -256 1536 ebis.ilock.SLNK
w -420 1595 100 0 n#123 egenSub.ilockDecision.FLNK -512 1248 -416 1248 -416 1952 -336 1952 edfans.ilockFanout.SLNK
w -454 1987 100 0 n#122 egenSub.ilockDecision.VALA -512 1984 -336 1984 edfans.ilockFanout.DOL
w 1668 1227 100 0 n#118 ebis.wdgStarted.FLNK 1536 1440 1664 1440 1664 1024 1856 1024 ecalcs.msEnableAnd.SLNK
w 1570 1099 100 0 n#118 ebis.msEnableEnable.FLNK 1536 1088 1664 1088 junction
w 1724 1211 100 0 n#117 ebis.msEnableEnable.VAL 1536 1056 1728 1056 1728 1376 1856 1376 ecalcs.msEnableAnd.INPB
w 1666 1411 100 0 n#116 ebis.wdgStarted.VAL 1536 1408 1856 1408 ecalcs.msEnableAnd.INPA
w -1822 2123 100 0 n#97 ebis.gisEvent.INP -1792 2112 -1792 2112 hwin.hwin#102.in
w -1822 1787 100 0 n#96 ebis.gisEventError.INP -1792 1776 -1792 1776 hwin.hwin#103.in
w -1822 1435 100 0 n#95 ebis.gisDemand.INP -1792 1424 -1792 1424 hwin.hwin#104.in
w -1822 1083 100 0 n#94 ebis.gisError.INP -1792 1072 -1792 1072 hwin.hwin#105.in
w 2434 1979 100 0 n#84 ebos.watchdog.OUT 2464 1968 2464 1968 hwout.hwout#90.outp
w 1506 619 100 0 n#83 ebos.masterEnable.OUT 1536 608 1536 608 hwout.hwout#91.outp
w 1506 -21 100 0 n#82 ebos.sensorEnable.OUT 1536 -32 1536 -32 hwout.hwout#92.outp
w 1506 299 100 0 n#81 ebos.optsensorEnable.OUT 1536 288 1536 288 hwout.hwout#93.outp
w -1781 776 100 2 n#59 ebis.masterDisabled.INP -1792 752 -1792 752 hwin.hwin#61.in
w 488 1531 100 0 n#37 junction 448 1552 448 1520 576 1520 eseqs.wdgEnableHoldoff.DOL6
w 488 1563 100 0 n#37 junction 448 1584 448 1552 576 1552 eseqs.wdgEnableHoldoff.DOL5
w 488 1627 100 0 n#37 hwin.hwin#21.in 448 1616 576 1616 eseqs.wdgEnableHoldoff.DOL3
w 488 1595 100 0 n#37 eseqs.wdgEnableHoldoff.DOL4 576 1584 448 1584 448 1616 junction
w 488 1691 100 0 n#43 hwin.hwin#19.in 448 1680 576 1680 eseqs.wdgEnableHoldoff.DOL1
w 488 1659 100 0 n#43 eseqs.wdgEnableHoldoff.DOL2 576 1648 448 1648 448 1680 junction
w 968 1587 100 0 n#58 eseqs.wdgEnableHoldoff.LNK4 896 1584 1088 1584 1088 1424 1280 1424 ebis.wdgStarted.SLNK
w 968 1659 100 0 n#58 junction 1088 1584 1088 1648 896 1648 eseqs.wdgEnableHoldoff.LNK2
w 984 1627 100 0 n#41 eseqs.wdgEnableHoldoff.LNK3 896 1616 1120 1616 1120 1680 896 1680 eseqs.wdgEnableHoldoff.LNK1
w 1176 1771 100 0 n#41 junction 1120 1680 1120 1760 1280 1760 ebis.wdgBuffer.SLNK
w 2088 2011 100 0 n#30 ebos.watchdog.SLNK 2208 2000 2016 2000 ecalcs.wdgGenerate.FLNK
w 1816 2267 100 0 n#29 ecalcs.wdgGenerate.VAL 2016 1968 2080 1968 2080 2256 1600 2256 1600 2160 1728 2160 ecalcs.wdgGenerate.INPA
w 2120 2043 100 0 n#29 ebos.watchdog.DOL 2208 2032 2080 2032 junction
w 1608 1755 100 0 n#18 ebis.wdgBuffer.VAL 1536 1744 1728 1744 ecalcs.wdgGenerate.SDIS
s 32 2064 100 0 ADDING PP NMS IS AN EPICS BUG WORK-AROUND
s 496 1824 100 0 This record processes once on initialisation.
s -864 1408 100 1280 Simulation
s 1824 96 100 0 ***  optSensorEnable is turned OFF at end of
s 1824 64 100 0 bootup and controlled via Mask Assembly SOR
s 1824 32 100 0 output thereafter.
s 1600 352 100 0 ***
[cell use]
use inhier -1968 128 100 512 DOORS
xform 0 -1952 128
use hwin -1984 391 100 0 hwin#196
xform 0 -1888 432
p -1981 424 100 0 -1 val(in):#<$(defeat)>
use hwin -1984 1031 100 0 hwin#105
xform 0 -1888 1072
p -1981 1064 100 0 -1 val(in):#<$(gisDErr)>
use hwin -1984 1383 100 0 hwin#104
xform 0 -1888 1424
p -1981 1416 100 0 -1 val(in):#<$(gisD)>
use hwin -1984 1735 100 0 hwin#103
xform 0 -1888 1776
p -1981 1768 100 0 -1 val(in):#<$(gisEErr)>
use hwin -1984 2071 100 0 hwin#102
xform 0 -1888 2112
p -1981 2104 100 0 -1 val(in):#<$(gisE)>
use hwin -1984 711 100 0 hwin#61
xform 0 -1888 752
p -1981 744 100 0 -1 val(in):#<$(shtDwn)>
use hwin 256 1639 100 0 hwin#19
xform 0 352 1680
p 259 1672 100 0 -1 val(in):0
use hwin 256 1575 100 0 hwin#21
xform 0 352 1616
p 259 1608 100 0 -1 val(in):1
use ebis -1536 464 100 0 doorIlckDefeat
xform 0 -1664 400
p -1728 320 100 0 1 DTYP:$(xycom)
p -1728 224 100 0 1 ONAM:OK
p -1520 224 100 0 1 OSV:NO_ALARM
p -1552 464 100 512 1 PV:$(top)$(cc)
p -1728 288 100 0 1 SCAN:Passive
p -1728 256 100 0 1 ZNAM:DEFEATED
p -1520 256 100 0 1 ZSV:MAJOR
use ebis -1536 1104 100 0 gisError
xform 0 -1664 1040
p -1965 1115 100 0 0 DESC:TTL switch input
p -1728 944 100 0 1 DTYP:$(xycom)
p -1728 848 100 0 1 ONAM:OK
p -1552 848 100 0 1 OSV:NO_ALARM
p -1728 1104 100 0 1 PV:$(top)$(cc)
p -1728 912 100 0 1 SCAN:Passive
p -1728 880 100 0 1 ZNAM:ERROR
p -1552 880 100 0 1 ZSV:MAJOR
use ebis -1536 1456 100 0 gisDemand
xform 0 -1664 1392
p -1965 1467 100 0 0 DESC:TTL switch input
p -1728 1296 100 0 1 DTYP:$(xycom)
p -1728 1200 100 0 1 ONAM:OK
p -1504 1200 100 0 1 OSV:NO_ALARM
p -1728 1456 100 0 1 PV:$(top)$(cc)
p -1728 1264 100 0 1 SCAN:Passive
p -1728 1232 100 0 1 ZNAM:INTERLOCK
p -1504 1232 100 0 1 ZSV:MAJOR
use ebis -1536 1808 100 0 gisEventError
xform 0 -1664 1744
p -1965 1819 100 0 0 DESC:TTL switch input
p -1728 1648 100 0 1 DTYP:$(xycom)
p -1728 1552 100 0 1 ONAM:OK
p -1536 1552 100 0 1 OSV:NO_ALARM
p -1728 1808 100 0 1 PV:$(top)$(cc)
p -1728 1616 100 0 1 SCAN:Passive
p -1728 1584 100 0 1 ZNAM:ERROR
p -1536 1584 100 0 1 ZSV:MAJOR
use ebis -1536 2144 100 0 gisEvent
xform 0 -1664 2080
p -1965 2155 100 0 0 DESC:TTL switch input
p -1728 1984 100 0 1 DTYP:$(xycom)
p -1728 1888 100 0 1 ONAM:OK
p -1504 1888 100 0 1 OSV:NO_ALARM
p -1728 2144 100 0 1 PV:$(top)$(cc)
p -1728 1952 100 0 1 SCAN:.1 second
p -1728 1920 100 0 1 ZNAM:INTERLOCK
p -1504 1920 100 0 1 ZSV:MAJOR
use ebis -1536 784 100 0 masterDisabled
xform 0 -1664 720
p -1965 795 100 0 0 DESC:TTL switch input
p -1728 624 100 0 1 DTYP:$(xycom)
p -1728 528 100 0 1 ONAM:SHUTDOWN
p -1504 528 100 0 1 OSV:MAJOR
p -1728 784 100 0 1 PV:$(top)$(cc)
p -1728 592 100 0 1 SCAN:$(iointr)
p -1728 560 100 0 1 ZNAM:OK
p -1504 560 100 0 1 ZSV:NO_ALARM
use ebis 1456 1840 100 0 wdgBuffer
xform 0 1408 1760
p 1280 1598 100 0 0 DISV:1
p 1344 1664 100 0 1 DTYP:Soft Channel
p 1344 1568 100 0 1 ONAM:enabled
p 1440 1840 100 512 1 PV:$(top)$(cc)
p 1344 1632 100 0 1 SCAN:Passive
p 1344 1600 100 0 1 ZNAM:disabled
use ebis 1488 1136 100 0 msEnableEnable
xform 0 1408 1072
p 1344 992 100 0 1 DTYP:Soft Channel
p 1344 896 100 0 1 ONAM:ON
p 1280 974 100 0 0 OSV:NO_ALARM
p 1472 1136 100 512 1 PV:$(top)$(cc)
p 1344 960 100 0 1 SCAN:Passive
p 1344 928 100 0 1 ZNAM:OFF
p 1280 1006 100 0 0 ZSV:MINOR
use ebis 1488 1488 100 0 wdgStarted
xform 0 1408 1424
p 1344 1344 100 0 1 DTYP:Soft Channel
p 1344 1248 100 0 1 ONAM:ON
p 1280 1326 100 0 0 OSV:NO_ALARM
p 1456 1488 100 512 1 PV:$(top)$(cc)
p 1344 1312 100 0 1 SCAN:Passive
p 1344 1280 100 0 1 ZNAM:OFF
p 1280 1358 100 0 0 ZSV:MINOR
use ebis -1536 48 100 0 softIlock
xform 0 -1664 -16
p -1965 59 100 0 0 DESC:TTL switch input
p -1728 -112 100 0 1 DTYP:Soft Channel
p -1728 -208 100 0 1 ONAM:INTERLOCK
p -1504 -208 100 0 1 OSV:MAJOR
p -1728 48 100 0 1 PV:$(top)$(cc)
p -1728 -144 100 0 1 SCAN:Passive
p -1728 -176 100 0 1 ZNAM:OK
p -1504 -176 100 0 1 ZSV:NO_ALARM
use ebis 0 1600 100 0 ilock
xform 0 -128 1536
p -429 1611 100 0 0 DESC:TTL switch input
p -192 1440 100 0 1 DTYP:Soft Channel
p -192 1344 100 0 1 ONAM:INTERLOCK
p 32 1344 100 0 1 OSV:MAJOR
p -192 1600 100 0 1 PV:$(top)$(cc)
p -192 1408 100 0 1 SCAN:Passive
p -192 1376 100 0 1 ZNAM:OK
p 32 1376 100 0 1 ZSV:NO_ALARM
use hwout -32 1847 100 0 hwout#136
xform 0 64 1888
p 64 1879 100 0 -1 val(outp):$(top)$(c2)$(translation)Ilock.VAL PP NMS
use hwout -32 1879 100 0 hwout#135
xform 0 64 1920
p 64 1911 100 0 -1 val(outp):$(top)$(c2)$(grating)Ilock.VAL PP NMS
use hwout 1536 247 100 0 hwout#93
xform 0 1632 288
p 1632 279 100 0 -1 val(outp):#<$(encoderEn)>
use hwout 1536 -73 100 0 hwout#92
xform 0 1632 -32
p 1632 -41 100 0 -1 val(outp):#<$(sensorEn)>
use hwout 1536 567 100 0 hwout#91
xform 0 1632 608
p 1632 599 100 0 -1 val(outp):#<$(mstrEn)>
use hwout 2464 1927 100 0 hwout#90
xform 0 2560 1968
p 2560 1959 100 0 -1 val(outp):#<$(wDog)>
use hwout -32 1911 100 0 hwout#132
xform 0 64 1952
p 64 1943 100 0 -1 val(outp):$(top)$(c2)$(filter)Ilock.VAL PP NMS
use hwout -32 1943 100 0 hwout#184
xform 0 64 1984
p 64 1975 100 0 -1 val(outp):$(top)wfs:$(oiwfs)Ilock.VAL PP NMS
use hwout -32 1975 100 0 hwout#185
xform 0 64 2016
p 64 2007 100 0 -1 val(outp):$(top)$(c1)$(mask)Ilock.VAL PP NMS
use hwout -32 1815 100 0 hwout#192
xform 0 64 1856
p 64 1847 100 0 -1 val(outp):$(top)$(c1)$(atmdc)Ilock.VAL PP NMS
use hwout -224 1207 100 0 hwout#193
xform 0 -128 1248
p -128 1239 100 0 -1 val(outp):$(sadtop)$(cc)historyLog2.VAL PP NMS
use ecalcs 2096 688 100 0 msNot
xform 0 2064 432
p 2144 272 100 0 1 CALC:!A
p 1632 318 100 0 0 EGU:0/1
p 2144 240 100 0 1 PINI:NO
p 2064 688 100 512 1 PV:$(top)$(cc)
p 2144 304 100 0 1 SCAN:Passive
use ecalcs 1904 2192 100 0 wdgGenerate
xform 0 1872 1952
p 1952 1776 100 0 1 CALC:!A
p 1952 1808 100 0 1 DISV:0
p 1440 1838 100 0 0 EGU:0/1
p 1888 2192 100 512 1 PV:$(top)$(cc)
p 1952 1840 100 0 1 SCAN:.5 second
use ecalcs 2032 1456 100 0 msEnableAnd
xform 0 2000 1200
p 2080 1040 100 0 1 CALC:A && B
p 1568 1086 100 0 0 EGU:0/1
p 2080 1008 100 0 1 PINI:NO
p 2000 1456 100 512 1 PV:$(top)$(cc)
p 2080 1072 100 0 1 SCAN:Passive
use edfans -112 2160 100 0 ilockFanout
xform 0 -208 1952
p -272 1728 100 768 1 OMSL:closed_loop
p -304 2160 100 0 1 PV:$(top)$(cc)
p -80 2016 75 768 -1 pproc(OUTA):PP
p -80 1984 75 768 -1 pproc(OUTB):PP
p -80 1952 75 768 -1 pproc(OUTC):PP
p -80 1920 75 768 -1 pproc(OUTD):PP
p -80 1888 75 768 -1 pproc(OUTE):PP
p -80 1856 75 768 -1 pproc(OUTF):PP
p -80 1824 75 768 -1 pproc(OUTG):PP
p -80 1792 75 768 -1 pproc(OUTH):PP
p -368 1920 75 1280 -1 pproc(SDIS):PP
use edfans 864 336 100 0 msEnableFan
xform 0 800 128
p 704 -112 100 768 1 OMSL:closed_loop
p 672 336 100 0 1 PV:$(top)$(cc)
p 928 192 75 768 -1 pproc(OUTA):PP
p 928 160 75 768 -1 pproc(OUTB):PP
p 928 128 75 768 -1 pproc(OUTC):PP
use eseqs -608 944 100 512 ilockMonWait
xform 4 -640 704
p -704 416 100 512 1 DLY1:1.0e+00
p -576 368 100 0 0 DLY2:0.0e+00
p -576 336 100 0 0 DLY3:0.0e+00
p -576 304 100 0 0 DLY4:0.0e+00
p -576 272 100 0 0 DLY5:0.0e+00
p -576 240 100 0 0 DLY6:0.0e+00
p -576 208 100 0 0 DLY7:0.0e+00
p -576 176 100 0 0 DLY8:0.0e+00
p -576 144 100 0 0 DLY9:0.0e+00
p -576 112 100 0 0 DLYA:0.0e+00
p -704 448 100 512 1 PINI:NO
p -624 944 100 0 1 PV:$(top)$(cc)
p -704 480 100 512 1 SCAN:Passive
p -464 896 100 512 1 SELM:All
use eseqs 848 1760 100 0 wdgEnableHoldoff
xform 0 736 1520
p 672 1220 100 0 1 DLY1:0.0e+00
p 672 1188 100 0 1 DLY2:0.0e+00
p 672 1156 100 0 1 DLY3:5.0e+00
p 672 1124 100 0 1 DLY4:2.0e+00
p 672 1092 100 0 1 DLY5:0.0e+00
p 672 1060 100 0 1 DLY6:0.0e+00
p 672 1024 100 0 0 DLY7:0.0e+00
p 672 992 100 0 0 DLY8:0.0e+00
p 672 960 100 0 0 DLY9:0.0e+00
p 672 928 100 0 0 DLYA:0.0e+00
p 672 1248 100 0 1 PINI:YES
p 832 1760 100 512 1 PV:$(top)$(cc)
p 672 1280 100 0 1 SCAN:Passive
p 528 1760 100 0 1 SELM:All
p 544 1680 75 1280 -1 pproc(DOL1):NPP
p 544 1648 75 1280 -1 pproc(DOL2):NPP
p 544 1616 75 1280 -1 pproc(DOL3):NPP
p 544 1584 75 1280 -1 pproc(DOL4):NPP
p 544 1392 75 1280 -1 pproc(DOLA):NPP
p 912 1680 75 1024 -1 pproc(LNK1):PP
p 912 1648 75 1024 -1 pproc(LNK2):PP
p 912 1616 75 1024 -1 pproc(LNK3):PP
p 912 1584 75 1024 -1 pproc(LNK4):PP
p 912 1552 75 1024 -1 pproc(LNK5):PP
p 912 1520 75 1024 -1 pproc(LNK6):PP
use notes -2272 2199 100 0 notes#146
xform 0 -2016 2384
p -2244 2510 100 0 -1 COMMENT1:All of the binary records with
p -2244 2478 100 0 -1 COMMENT2:DTYP=XYCOM-240 below will process
p -2244 2448 100 0 -1 COMMENT3:when any one of the bits in the XYCOM
p -2244 2416 100 0 -1 COMMENT4:changes. To prevent a whole cascade of
p -2244 2384 100 0 -1 COMMENT5:processing whenever a single bit changes,
p -2244 2352 100 0 -1 COMMENT6:only one of the records is triggered by
p -2244 2320 100 0 -1 COMMENT7:an I/O interrupt, and the others are
p -2244 2288 100 0 -1 COMMENT8:forward linked
use egenSub -560 2032 100 0 ilockDecision
xform 0 -656 1616
p -704 1984 70 256 -1 FTA:LONG
p -704 1920 70 256 -1 FTB:LONG
p -704 1856 70 256 -1 FTC:LONG
p -704 1792 70 256 -1 FTD:LONG
p -704 1728 70 256 -1 FTE:LONG
p -704 1664 70 256 -1 FTF:LONG
p -704 1600 70 256 -1 FTG:LONG
p -704 1536 70 256 -1 FTH:LONG
p -704 1472 70 256 -1 FTI:LONG
p -704 1408 70 256 -1 FTJ:LONG
p -592 1984 70 512 -1 FTVA:LONG
p -592 1920 70 512 -1 FTVB:STRING
p -592 1856 70 512 -1 FTVC:LONG
p -736 1136 100 0 1 INAM:gmosIlockInit
p -736 2032 100 0 1 PV:$(top)$(cc)
p -736 1168 100 0 1 SCAN:Passive
p -736 1104 100 0 1 SNAM:gmosIlockCombine
p -864 1568 100 512 0 def(INPG):0.0
p -512 1898 75 0 -1 pproc(OUTB):PP
use ukatcBorderD -2416 -697 100 0 ukatcBorderD#120
xform 0 224 1008
p 1296 -560 100 0 -1 ID:$Id$
p 2452 -384 120 256 -1 Project:Gemini Multi-object Optical Spectrometer
p 544 -560 150 0 -1 Rev:$Revision$
p 2444 -448 120 256 -1 Title:GMOS Interlocks and Watchdog Timer
p 2160 -512 100 768 -1 author:B.Wooff and S.M.Beard
p 2160 -544 100 768 -1 date:$Date$
use ebos 1488 400 100 0 optsensorEnable
xform 0 1408 320
p 1344 224 100 0 1 DTYP:$(xycom)
p 1568 192 100 0 1 OMSL:supervisory
p 1344 128 100 0 1 ONAM:OFF
p 1456 400 100 512 1 PV:$(top)$(cc)
p 1344 192 100 0 1 SCAN:Passive
p 1344 160 100 0 1 ZNAM:ON
use ebos 1488 80 100 0 sensorEnable
xform 0 1408 0
p 1344 -96 100 0 1 DTYP:$(xycom)
p 1568 -128 100 0 1 OMSL:supervisory
p 1344 -192 100 0 1 ONAM:OFF
p 1456 80 100 512 1 PV:$(top)$(cc)
p 1344 -128 100 0 1 SCAN:Passive
p 1344 -160 100 0 1 ZNAM:ON
use ebos 1488 720 100 0 masterEnable
xform 0 1408 640
p 1344 544 100 0 1 DTYP:$(xycom)
p 1568 512 100 0 1 OMSL:closed_loop
p 1344 448 100 0 1 ONAM:ON
p 1568 448 100 0 1 OSV:NO_ALARM
p 1456 720 100 512 1 PV:$(top)$(cc)
p 1344 512 100 0 1 SCAN:Passive
p 1344 480 100 0 1 ZNAM:OFF
p 1568 480 100 0 1 ZSV:MINOR
use ebos 2416 2080 100 0 watchdog
xform 0 2336 2000
p 2272 1904 100 0 1 DTYP:$(xycom)
p 2272 1776 100 0 1 OMSL:closed_loop
p 2272 1808 100 0 1 ONAM:HIGH
p 2384 2080 100 512 1 PV:$(top)$(cc)
p 2272 1872 100 0 1 SCAN:Passive
p 2272 1840 100 0 1 ZNAM:LOW
[comments]

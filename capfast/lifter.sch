[schematic2]
uniq 103
[tools]
[detail]
w 882 283 100 0 n#102 hwout.hwout#101.outp 912 272 912 272 lifterTurretIlck.lifterTurretIlck#99.turretIlck
w 1020 923 100 0 n#100 lifterTurretIlck.lifterTurretIlck#99.lifterIlck 912 368 1024 368 1024 1488 1120 1488 egenSub.Lifter.D
w 412 931 100 0 n#61 egenSub.Lifter.INPF 1120 1328 416 1328 416 544 48 544 ebis.downstreamOk.VAL
w 420 419 100 0 n#61 junction 416 544 416 304 592 304 lifterTurretIlck.lifterTurretIlck#99.downPressureOK
w 728 1395 100 0 n#58 ebis.upstreamOk.VAL 48 864 384 864 384 1392 1120 1392 egenSub.Lifter.INPE
w 388 611 100 0 n#58 junction 384 864 384 368 592 368 lifterTurretIlck.lifterTurretIlck#99.upPressureOK
w 466 243 100 0 estop inhier.estop.P 400 240 592 240 lifterTurretIlck.lifterTurretIlck#99.estop
w -192 675 100 0 n#95 ebis.upstreamOk.FLNK 48 896 96 896 96 672 -432 672 -432 560 -208 560 ebis.downstreamOk.SLNK
w 348 371 100 0 n#91 ebis.downstreamOk.FLNK 48 576 352 576 352 176 592 176 lifterTurretIlck.lifterTurretIlck#99.slnk
w -192 243 100 0 n#88 hwin.hwin#84.in -224 240 -112 240 -112 288 -16 288 ecalcs.LifterLock.INPB
w -144 323 100 0 n#87 hwin.hwin#73.in -224 320 -16 320 ecalcs.LifterLock.INPA
w 984 979 100 0 n#83 eseqs.LifterProc.LNK1 896 976 1120 976 egenSub.Lifter.SLNK
w -238 923 100 0 n#77 ebis.upstreamOk.INP -208 912 -208 912 hwin.hwin#80.in
w -238 603 100 0 n#76 ebis.downstreamOk.INP -208 592 -208 592 hwin.hwin#81.in
w 444 627 100 0 n#75 ecalcs.LifterLock.VAL 272 128 448 128 448 1136 1120 1136 egenSub.Lifter.INPI
w 1264 2115 100 0 n#72 edfans.cmdFanout.OUTB 1760 1904 1872 1904 1872 2112 704 2112 704 1936 160 1936 embbos.LifterManCtrl.VAL
w 1214 2083 100 0 command edfans.cmdFanout.OUTA 1760 1936 1840 1936 1840 2080 672 2080 672 1968 junction
w 374 1971 100 0 command embbos.LifterManCtrl.OUT 160 1968 672 1968 672 1680 junction
w 310 1683 100 0 command inhier.command.P -416 1680 1120 1680 egenSub.Lifter.A
w 1468 1627 100 0 n#70 egenSub.Lifter.OUTE 1408 1392 1472 1392 1472 1872 1504 1872 edfans.cmdFanout.SLNK
w -200 1331 100 0 n#65 ebis.LifterUp.FLNK 32 1552 96 1552 96 1328 -448 1328 -448 1216 -224 1216 ebis.LifterDown.SLNK
w -254 1259 100 0 n#49 ebis.LifterDown.INP -224 1248 -224 1248 hwin.hwin#53.in
w -254 1579 100 0 n#48 ebis.LifterUp.INP -224 1568 -224 1568 hwin.hwin#52.in
w 2136 1011 100 0 n#47 embbos.LifterState.OUT 2096 1008 2224 1008 elongouts.LifterStateNum.SLNK
w 1724 1259 100 0 n#36 embbos.LifterState.SLNK 1840 1008 1728 1008 1728 1520 1408 1520 egenSub.Lifter.OUTC
w 2546 979 100 0 state elongouts.LifterStateNum.OUT 2480 976 2672 976 outhier.state.p
w 712 1523 100 0 n#26 ebis.LifterDown.VAL 32 1200 352 1200 352 1520 1120 1520 egenSub.Lifter.INPC
w 568 1587 100 0 n#25 ebis.LifterUp.VAL 32 1520 64 1520 64 1584 1120 1584 egenSub.Lifter.INPB
w 1560 1587 100 0 n#21 egenSub.Lifter.OUTB 1408 1584 1760 1584 1760 1328 1840 1328 ebos.DownValve.SLNK
w 1600 1651 100 0 n#20 egenSub.Lifter.OUTA 1408 1648 1840 1648 ebos.UpValve.SLNK
w 2078 1307 100 0 n#9 ebos.DownValve.OUT 2096 1296 2096 1296 hwout.hwout#8.outp
w 2078 1627 100 0 n#6 hwout.hwout#5.outp 2096 1616 2096 1616 ebos.UpValve.OUT
s -416 176 100 0 Turret device BUSY?
s 864 624 100 0 This record processes once on initialisation.
s 112 560 100 0 Downstream pressure OK?
s 1424 1472 100 0 Interlock to turret rotation
s -352 2144 300 0 This schematic provides control of the pneumatic lifter for the indexer mechanism
s 2176 1488 100 0 binary control of up and down valves
s 864 592 100 0 sequence record provides a callback after 2 seconds
s 128 880 100 0 Upstream pressure OK?
s -416 368 100 0 Turret power on?
s 128 1479 100 0 XYCOM driver bug means interrupts are sometimes missed.
s 128 1447 100 0 "I/O Intr" scan replaced by 10Hz to overcome this problem.
[cell use]
use hwout 2096 1575 100 0 hwout#5
xform 0 2192 1616
p 2192 1607 100 0 -1 val(outp):#<$(lftrVlv)>
use hwout 2096 1255 100 0 hwout#8
xform 0 2192 1296
p 2192 1287 100 0 -1 val(outp):#<$(purgeVlv)>
use hwout 912 231 100 0 hwout#101
xform 0 1008 272
p 1008 263 100 0 -1 val(outp):$(top)$(dev)tDevice.FLT NPP NMS
use lifterTurretIlck 592 135 100 0 lifterTurretIlck#99
xform 0 752 272
use elongouts 2480 1088 100 0 LifterStateNum
xform 0 2352 1008
p 2288 912 100 0 1 OMSL:supervisory
p 2288 1088 100 0 1 PV:$(top)$(dev)
use ecalcs -16 -144 100 0 LifterLock
xform 0 128 112
p -304 -130 100 0 0 ADEL:0.000000000000000e+00
p -109 328 100 0 0 CALC:A&&(B#1)
p -208 -144 100 0 1 PV:$(top)$(dev)
use hwin -416 1527 100 0 hwin#52
xform 0 -320 1568
p -413 1560 100 0 -1 val(in):#<$(liftDisen)>
use hwin -416 1207 100 0 hwin#53
xform 0 -320 1248
p -413 1240 100 0 -1 val(in):#<$(liftEng)>
use hwin -416 279 100 0 hwin#73
xform 0 -320 320
p -413 312 100 0 -1 val(in):$(top)$(dev)tPwrUpdate.VAL
use hwin -400 871 100 0 hwin#80
xform 0 -304 912
p -397 904 100 0 -1 val(in):#<$(upPres)>
use hwin -400 551 100 0 hwin#81
xform 0 -304 592
p -397 584 100 0 -1 val(in):#<$(dwnPres)>
use hwin -416 199 100 0 hwin#84
xform 0 -320 240
p -413 232 100 0 -1 val(in):$(top)$(dev)tDevice.BUSY
use eseqs 848 1056 100 0 LifterProc
xform 0 736 816
p 672 516 100 0 1 DLY1:1.0e+00
p 672 320 100 0 0 DLY7:0.0e+00
p 672 288 100 0 0 DLY8:0.0e+00
p 672 256 100 0 0 DLY9:0.0e+00
p 672 224 100 0 0 DLYA:0.0e+00
p 672 544 100 0 1 PINI:YES
p 832 1056 100 512 1 PV:$(top)$(cc)
p 672 576 100 0 1 SCAN:Passive
p 528 1056 100 0 1 SELM:All
p 544 976 75 1280 -1 pproc(DOL1):NPP
p 544 944 75 1280 -1 pproc(DOL2):NPP
p 544 912 75 1280 -1 pproc(DOL3):NPP
p 544 880 75 1280 -1 pproc(DOL4):NPP
p 544 688 75 1280 -1 pproc(DOLA):NPP
p 912 976 75 1024 -1 pproc(LNK1):PP
p 912 944 75 1024 -1 pproc(LNK2):PP
p 912 912 75 1024 -1 pproc(LNK3):PP
p 912 880 75 1024 -1 pproc(LNK4):PP
p 912 848 75 1024 -1 pproc(LNK5):PP
p 912 816 75 1024 -1 pproc(LNK6):PP
use ebis 32 1600 100 0 LifterUp
xform 0 -96 1536
p -397 1611 100 0 0 DESC:TTL switch input
p -160 1440 100 0 1 DTYP:$(xycom)
p -160 1344 100 0 1 ONAM:NOTUP
p -160 1600 100 0 1 PV:$(top)$(dev)
p -160 1408 100 0 1 SCAN:.1 second
p -160 1376 100 0 1 ZNAM:UP
use ebis 32 1280 100 0 LifterDown
xform 0 -96 1216
p -397 1291 100 0 0 DESC:TTL switch input
p -160 1120 100 0 1 DTYP:$(xycom)
p -160 1024 100 0 1 ONAM:NOTDOWN
p -160 1280 100 0 1 PV:$(top)$(dev)
p -160 1088 100 0 1 SCAN:Passive
p -160 1056 100 0 1 ZNAM:DOWN
use ebis 48 944 100 0 upstreamOk
xform 0 -80 880
p -381 955 100 0 0 DESC:TTL switch input
p -144 784 100 0 1 DTYP:$(xycom)
p -144 688 100 0 1 ONAM:NOTOK
p 32 688 100 0 1 OSV:MAJOR
p -144 944 100 0 1 PV:$(top)
p -144 752 100 0 1 SCAN:.1 second
p -144 720 100 0 1 ZNAM:OK
p 32 720 100 0 1 ZSV:NO_ALARM
use ebis 48 624 100 0 downstreamOk
xform 0 -80 560
p -381 635 100 0 0 DESC:TTL switch input
p -144 464 100 0 1 DTYP:$(xycom)
p -144 368 100 0 1 ONAM:NOTOK
p 32 368 100 0 1 OSV:MAJOR
p -144 624 100 0 1 PV:$(top)
p -144 432 100 0 1 SCAN:Passive
p -144 400 100 0 1 ZNAM:OK
p 32 400 100 0 1 ZSV:NO_ALARM
use edfans 1520 2000 100 0 cmdFanout
xform 0 1632 1872
p 1552 1648 100 768 1 OMSL:supervisory
p 1328 2000 100 0 1 PV:$(top)$(dev)
use ebos 2096 1728 100 0 UpValve
xform 0 1968 1648
p 1608 1804 100 0 0 DESC:turret lift valve
p 1904 1456 100 0 1 DTYP:$(xycom)
p 1904 1552 100 0 1 OMSL:supervisory
p 1904 1520 100 0 1 ONAM:DEACTIVATED
p 1904 1728 100 0 1 PV:$(top)$(dev)
p 1904 1488 100 0 1 ZNAM:ACTIVATED
use ebos 2096 1408 100 0 DownValve
xform 0 1968 1328
p 1608 1484 100 0 0 DESC:turret lift valve
p 1904 1136 100 0 1 DTYP:$(xycom)
p 1904 1232 100 0 1 OMSL:supervisory
p 1904 1200 100 0 1 ONAM:DEACTIVATED
p 1904 1408 100 0 1 PV:$(top)$(dev)
p 1904 1168 100 0 1 ZNAM:ACTIVATED
use notes 1904 1751 100 0 notes#64
xform 0 2160 1936
p 1932 2062 100 0 -1 COMMENT1:All of the binary records with
p 1932 2030 100 0 -1 COMMENT2:DTYP=XYCOM-240 below will process
p 1932 2000 100 0 -1 COMMENT3:when any one of the bits in the XYCOM
p 1932 1968 100 0 -1 COMMENT4:changes. To prevent a whole cascade of
p 1932 1936 100 0 -1 COMMENT5:processing whenever a single bit changes,
p 1932 1904 100 0 -1 COMMENT6:only one of the records is triggered by
p 1932 1872 100 0 -1 COMMENT7:an I/O interrupt, and the others are
p 1932 1840 100 0 -1 COMMENT8:forward linked. BUT SEE BUG REPORT.
use embbos 160 2048 100 0 LifterManCtrl
xform 0 32 1968
p 256 1614 100 0 0 FTSV:NO_ALARM
p -32 1872 100 0 1 OMSL:supervisory
p -32 1808 100 0 1 ONST:DOWN
p 160 1808 100 0 0 ONSV:NO_ALARM
p -128 2030 100 0 0 ONVL:1
p -32 2048 100 0 1 PV:$(top)$(dev)
p -32 1744 100 0 1 THST:STOP
p -128 1966 100 0 0 THVL:3
p -32 1776 100 0 1 TWST:UP
p -128 1998 100 0 0 TWVL:2
p -32 1840 100 0 1 ZRST:OFF
p 160 1840 100 0 0 ZRSV:NO_ALARM
p 160 1968 75 768 -1 pproc(OUT):PP
use embbos 2096 1088 100 0 LifterState
xform 0 1968 1008
p 2192 846 100 0 0 EISV:INVALID
p 2192 750 100 0 0 ELSV:INVALID
p 2192 622 100 0 0 FFSV:INVALID
p 1904 752 100 0 1 FRST:INIT
p 2096 752 100 0 1 FRSV:INVALID
p 1808 974 100 0 0 FRVL:4
p 2192 654 100 0 0 FTSV:INVALID
p 2000 942 100 0 0 FVST:INVALID
p 2192 942 100 0 0 FVSV:INVALID
p 1808 942 100 0 0 FVVL:5
p 2192 814 100 0 0 NISV:INVALID
p 1904 912 100 0 1 OMSL:closed_loop
p 1904 848 100 0 1 ONST:RAISED
p 2096 848 100 0 1 ONSV:NO_ALARM
p 1808 1070 100 0 0 ONVL:1
p 1904 1088 100 0 1 PV:$(top)$(dev)
p 1584 526 100 0 0 SVSV:INVALID
p 2192 910 100 0 0 SXSV:INVALID
p 2192 782 100 0 0 TESV:INVALID
p 1904 784 100 0 1 THST:ERROR
p 2096 784 100 0 1 THSV:MAJOR
p 1808 1006 100 0 0 THVL:3
p 2192 686 100 0 0 TTSV:INVALID
p 2192 718 100 0 0 TVSV:INVALID
p 1904 816 100 0 1 TWST:MOVING
p 2096 816 100 0 1 TWSV:MINOR
p 1808 1038 100 0 0 TWVL:2
p 1904 880 100 0 1 ZRST:ENGAGED
p 2096 880 100 0 1 ZRSV:NO_ALARM
p 2128 1008 75 768 -1 palrm(OUT):MS
p 2096 1008 75 768 -1 pproc(OUT):PP
use inhier -432 1639 100 0 command
xform 0 -416 1680
use inhier 384 199 100 0 estop
xform 0 400 240
use outhier 2640 935 100 0 state
xform 0 2656 976
use egenSub 1376 1744 100 0 Lifter
xform 0 1264 1312
p 963 1627 100 0 0 DESC:turret lifter control
p 1200 1680 100 0 -1 FTA:LONG
p 1200 1616 100 0 -1 FTB:LONG
p 1200 1552 100 0 -1 FTC:LONG
p 1200 1488 100 0 -1 FTD:LONG
p 1200 1424 100 0 -1 FTE:LONG
p 1200 1360 100 0 -1 FTF:LONG
p 1200 1296 100 0 -1 FTG:LONG
p 1200 1232 100 0 -1 FTH:LONG
p 1200 1168 100 0 -1 FTI:LONG
p 1280 1680 100 0 -1 FTVA:LONG
p 1280 1616 100 0 -1 FTVB:LONG
p 1280 1552 100 0 -1 FTVC:LONG
p 1280 1488 100 0 -1 FTVD:LONG
p 1280 1424 100 0 -1 FTVE:LONG
p 1264 1360 100 0 -1 FTVF:STRING
p 1184 848 100 0 1 INAM:lifterInit
p 1184 1744 100 0 1 PV:$(top)$(dev)
p 1184 880 100 0 1 SCAN:Passive
p 1184 816 100 0 1 SNAM:lifterControl
p 1072 1146 75 0 -1 pproc(INPI):PP
p 1408 1658 75 0 -1 pproc(OUTA):PP
p 1408 1594 75 0 -1 pproc(OUTB):PP
p 1408 1530 75 0 -1 pproc(OUTC):PP
p 1408 1466 75 0 -1 pproc(OUTD):PP
p 1408 1402 75 0 -1 pproc(OUTE):PP
use ukatcBorderC -576 -281 100 0 ukatcBorderC#0
xform 0 1104 1024
p 2364 -32 120 256 -1 Title:Grating turret lifter control
p 2080 -96 100 768 -1 author:C.Tierney, S.Beard & S.Prior
[comments]

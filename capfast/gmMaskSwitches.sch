[schematic2]
uniq 435
[tools]
[detail]
w -78 587 100 0 bc_trigger inhier.bc_trigger.P -160 576 64 576 free
w 1690 939 100 0 barcode outhier.barcode.p 1840 928 1600 928 free
w 1690 1259 100 0 zones outhier.zones.p 1840 1248 1600 1248 gmMaskLvdt.gmMaskLvdt#426.zones
w 1416 2219 100 0 switches outhier.switches.p 1840 2208 1088 2208 egenSubD.SwitchWord.VALA
w 470 1387 100 0 process ebis.Cassette.FLNK 320 1376 704 1376 704 1504 800 1504 egenSubD.SwitchWord.SLNK
w 434 779 100 0 n#412 ebis.InFp.VAL 320 768 608 768 608 1920 800 1920 egenSubD.SwitchWord.INPJ
w 418 971 100 0 n#411 ebis.IfuIn.VAL 320 960 576 960 576 1952 800 1952 egenSubD.SwitchWord.INPI
w 642 1995 100 0 n#410 ebis.MaskIn.VAL 320 1152 544 1152 544 1984 800 1984 egenSubD.SwitchWord.INPH
w 626 2027 100 0 n#409 ebis.Cassette.VAL 320 1344 512 1344 512 2016 800 2016 egenSubD.SwitchWord.INPG
w 610 2059 100 0 n#408 ebis.InSlot.VAL 320 1536 480 1536 480 2048 800 2048 egenSubD.SwitchWord.INPF
w 594 2091 100 0 n#407 ebis.Aligned.VAL 320 1728 448 1728 448 2080 800 2080 egenSubD.SwitchWord.INPE
w 578 2123 100 0 n#406 ebis.GrpNotGripped.VAL 320 1920 416 1920 416 2112 800 2112 egenSubD.SwitchWord.INPD
w 562 2155 100 0 n#405 ebis.GrpNotReleased.VAL 320 2112 384 2112 384 2144 800 2144 egenSubD.SwitchWord.INPC
w 610 2187 100 0 n#404 ebis.RelNotReleased.VAL 320 2304 480 2304 480 2176 800 2176 egenSubD.SwitchWord.INPB
w 626 2219 100 0 n#403 ebis.RelNotLocked.VAL 320 2496 512 2496 512 2208 800 2208 egenSubD.SwitchWord.INPA
w 18 1971 100 0 n#358 hwin.hwin#366.in 32 1968 64 1968 ebis.GrpNotGripped.INP
w 18 2163 100 0 n#357 hwin.hwin#365.in 32 2160 64 2160 ebis.GrpNotReleased.INP
w 18 2355 100 0 n#356 hwin.hwin#364.in 32 2352 64 2352 ebis.RelNotReleased.INP
w 18 2547 100 0 n#355 hwin.hwin#363.in 32 2544 64 2544 ebis.RelNotLocked.INP
w 18 1395 100 0 n#315 hwin.hwin#317.in 32 1392 64 1392 ebis.Cassette.INP
w 18 1779 100 0 n#177 hwin.hwin#179.in 32 1776 64 1776 ebis.Aligned.INP
w 18 1587 100 0 n#144 hwin.hwin#146.in 32 1584 64 1584 ebis.InSlot.INP
w 18 819 100 0 n#134 hwin.hwin#135.in 32 816 64 816 ebis.InFp.INP
w 18 1011 100 0 n#133 hwin.hwin#132.in 32 1008 64 1008 ebis.IfuIn.INP
w 18 1203 100 0 n#131 hwin.hwin#130.in 32 1200 64 1200 ebis.MaskIn.INP
[cell use]
use inhier -176 535 100 0 bc_trigger
xform 0 -160 576
use outhier 1808 887 100 0 barcode
xform 0 1824 928
use outhier 1808 1207 100 0 zones
xform 0 1824 1248
use outhier 1808 2167 100 0 switches
xform 0 1824 2208
use egenSubD 992 2272 100 0 SwitchWord
xform 0 944 1840
p 880 2208 70 0 -1 FTA:LONG
p 880 2176 70 0 -1 FTB:LONG
p 880 2144 70 0 -1 FTC:LONG
p 880 2112 70 0 -1 FTD:LONG
p 880 2080 70 0 -1 FTE:LONG
p 880 2048 70 0 -1 FTF:LONG
p 880 2016 70 0 -1 FTG:LONG
p 880 1984 70 0 -1 FTH:LONG
p 880 1952 70 0 -1 FTI:LONG
p 880 1920 70 0 -1 FTJ:LONG
p 880 1888 70 0 0 FTK:LONG
p 1008 2208 70 512 -1 FTVA:LONG
p 800 2272 100 0 1 PV:$(top)$(dev)
p 864 1408 100 0 1 SNAM:mkSwitchWord
use gmMaskLvdt 1312 1159 100 0 gmMaskLvdt#426
xform 0 1456 1248
use hwin -160 1543 100 0 hwin#146
xform 0 -64 1584
p -157 1576 100 0 -1 val(in):#<C0 S26>
use hwin -160 1159 100 0 hwin#130
xform 0 -64 1200
p -157 1192 100 0 -1 val(in):#<C0 S28>
use hwin -160 967 100 0 hwin#132
xform 0 -64 1008
p -157 1000 100 0 -1 val(in):#<C0 S29>
use hwin -160 775 100 0 hwin#135
xform 0 -64 816
p -157 808 100 0 -1 val(in):#<C0 S30>
use hwin -160 1735 100 0 hwin#179
xform 0 -64 1776
p -157 1768 100 0 -1 val(in):#<C0 S25>
use hwin -160 1351 100 0 hwin#317
xform 0 -64 1392
p -157 1384 100 0 -1 val(in):#<C0 S27>
use hwin -160 2503 100 0 hwin#363
xform 0 -64 2544
p -157 2536 100 0 -1 val(in):#<C0 S12>
use hwin -160 2311 100 0 hwin#364
xform 0 -64 2352
p -157 2344 100 0 -1 val(in):#<C0 S13>
use hwin -160 2119 100 0 hwin#365
xform 0 -64 2160
p -157 2152 100 0 -1 val(in):#<C0 S14>
use hwin -160 1927 100 0 hwin#366
xform 0 -64 1968
p -157 1960 100 0 -1 val(in):#<C0 S15>
use ebis 256 1616 100 0 InSlot
xform 0 192 1552
p 64 1488 70 0 1 DTYP:XYCOM-240
p -160 1390 100 0 0 ONAM:SET
p 64 1616 100 0 1 PV:$(top)$(dev)
p 64 1472 70 0 1 SCAN:I/O Intr
p -160 1422 100 0 0 ZNAM:CLEAR
use ebis 256 1232 100 0 MaskIn
xform 0 192 1168
p 64 1104 70 0 1 DTYP:XYCOM-240
p -160 1006 100 0 0 ONAM:SET
p 64 1232 100 0 1 PV:$(top)$(dev)
p 64 1088 70 0 1 SCAN:I/O Intr
p -160 1038 100 0 0 ZNAM:CLEAR
use ebis 256 1040 100 0 IfuIn
xform 0 192 976
p 64 912 70 0 1 DTYP:XYCOM-240
p -160 814 100 0 0 ONAM:SET
p 64 1040 100 0 1 PV:$(top)$(dev)
p 64 896 70 0 1 SCAN:I/O Intr
p -160 846 100 0 0 ZNAM:CLEAR
use ebis 256 848 100 0 InFp
xform 0 192 784
p 64 720 70 0 1 DTYP:XYCOM-240
p -160 622 100 0 0 ONAM:SET
p 64 848 100 0 1 PV:$(top)$(dev)
p 64 704 70 0 1 SCAN:I/O Intr
p -160 654 100 0 0 ZNAM:CLEAR
use ebis 256 1808 100 0 Aligned
xform 0 192 1744
p 64 1680 70 0 1 DTYP:XYCOM-240
p -160 1582 100 0 0 ONAM:SET
p 64 1808 100 0 1 PV:$(top)$(dev)
p 64 1664 70 0 1 SCAN:I/O Intr
p -160 1614 100 0 0 ZNAM:CLEAR
use ebis 256 1424 100 0 Cassette
xform 0 192 1360
p 64 1296 70 0 1 DTYP:XYCOM-240
p -160 1198 100 0 0 ONAM:SET
p 64 1424 100 0 1 PV:$(top)$(dev)
p 64 1280 70 0 1 SCAN:I/O Intr
p -160 1230 100 0 0 ZNAM:CLEAR
use ebis 256 2576 100 0 RelNotLocked
xform 0 192 2512
p 64 2448 70 0 1 DTYP:XYCOM-240
p -160 2350 100 0 0 ONAM:SET
p 64 2576 100 0 1 PV:$(top)$(dev)
p 64 2432 70 0 1 SCAN:I/O Intr
p -160 2382 100 0 0 ZNAM:CLEAR
use ebis 256 2384 100 0 RelNotReleased
xform 0 192 2320
p 64 2256 70 0 1 DTYP:XYCOM-240
p -160 2158 100 0 0 ONAM:SET
p 64 2384 100 0 1 PV:$(top)$(dev)
p 64 2240 70 0 1 SCAN:I/O Intr
p -160 2190 100 0 0 ZNAM:CLEAR
use ebis 256 2192 100 0 GrpNotReleased
xform 0 192 2128
p 64 2064 70 0 1 DTYP:XYCOM-240
p -160 1966 100 0 0 ONAM:SET
p 64 2192 100 0 1 PV:$(top)$(dev)
p 64 2048 70 0 1 SCAN:I/O Intr
p -160 1998 100 0 0 ZNAM:CLEAR
use ebis 256 2000 100 0 GrpNotGripped
xform 0 192 1936
p 64 1872 70 0 1 DTYP:XYCOM-240
p -160 1774 100 0 0 ONAM:SET
p 64 2000 100 0 1 PV:$(top)$(dev)
p 64 1856 70 0 1 SCAN:I/O Intr
p -160 1806 100 0 0 ZNAM:CLEAR
use gmosBorderC -864 263 100 0 gmosBorderC#333
xform 0 816 1568
p 2388 392 100 512 1 File:/data/nimpkish/1/daoinsw/dev/gmos/current/develop/gmMask/capfast/gmMaskSwitches.sch
p 1796 436 150 0 1 Rev:
p 2076 512 120 256 -1 Title:Mask Assembly Switch Sensors
p 2116 448 100 1024 -1 author:B.Wooff
p 2116 416 100 1024 -1 date:May 4, 1999
[comments]

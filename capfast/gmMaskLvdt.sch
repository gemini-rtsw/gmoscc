[schematic2]
uniq 427
[tools]
[detail]
w 1346 1867 100 0 zones outhier.zones.p 1504 1856 1248 1856 egenSub.LvdtZones.OUTA
w 696 1259 100 0 proc eais.Lvdt.FLNK 544 1248 896 1248 896 1184 960 1184 egenSub.LvdtZones.SLNK
w 650 1227 100 0 n#423 eais.Lvdt.VAL 544 1216 816 1216 816 1536 960 1536 egenSub.LvdtZones.INPF
w 690 2187 100 0 n#422 eaos.ExtAtLvdtLo.VAL 544 2176 896 2176 896 1856 960 1856 egenSub.LvdtZones.INPA
w 658 1995 100 0 n#421 eaos.ExtAtGrpLo.VAL 544 1984 832 1984 832 1792 960 1792 egenSub.LvdtZones.INPB
w 626 1803 100 0 n#420 eaos.ExtAtGrpHi.VAL 544 1792 768 1792 768 1728 960 1728 egenSub.LvdtZones.INPC
w 786 1675 100 0 n#419 eaos.ExtAtRelLo.VAL 544 1600 672 1600 672 1664 960 1664 egenSub.LvdtZones.INPD
w 818 1611 100 0 n#418 eaos.ExtAtRelHi.VAL 544 1408 736 1408 736 1600 960 1600 egenSub.LvdtZones.INPE
w 210 1275 100 0 n#417 eais.Lvdt.INP 288 1264 192 1264 hwin.hwin#332.in
s 192 896 100 0 to cope with noisy electronics.
s 192 928 100 0 factor, which may be set to a non-zero value
s 192 960 100 0 The LVDT record has an optional smoothing
[cell use]
use outhier 1472 1815 100 0 zones
xform 0 1488 1856
use egenSub 1152 1952 100 0 LvdtZones
xform 0 1104 1520
p 1040 1856 70 0 -1 FTA:DOUBLE
p 1040 1792 70 0 -1 FTB:DOUBLE
p 1040 1728 70 0 -1 FTC:DOUBLE
p 1040 1664 70 0 -1 FTD:DOUBLE
p 1040 1600 70 0 -1 FTE:DOUBLE
p 1040 1536 70 0 -1 FTF:DOUBLE
p 1168 1888 70 512 -1 FTVA:LONG
p 1168 1824 70 512 -1 FTVB:LONG
p 1168 1760 70 512 -1 FTVC:LONG
p 1168 1696 70 512 -1 FTVD:LONG
p 1168 1632 70 512 -1 FTVE:LONG
p 1168 1568 70 512 -1 FTVF:LONG
p 1168 1504 70 512 -1 FTVG:LONG
p 960 1952 100 0 1 PV:$(top)$(dev)
p 1024 1088 100 0 1 SNAM:mkExtLvdtZones
use eaos 544 1680 100 0 ExtAtRelLo
xform 0 416 1600
p 256 1326 100 0 0 EGU:lvdt position
p 352 1680 100 0 1 PV:$(top)$(dev)
use eaos 544 1488 100 0 ExtAtRelHi
xform 0 416 1408
p 256 1134 100 0 0 EGU:lvdt position
p 352 1488 100 0 1 PV:$(top)$(dev)
use eaos 544 2064 100 0 ExtAtGrpLo
xform 0 416 1984
p 256 1710 100 0 0 EGU:lvdt position
p 352 2064 100 0 1 PV:$(top)$(dev)
use eaos 544 1872 100 0 ExtAtGrpHi
xform 0 416 1792
p 256 1518 100 0 0 EGU:lvdt position
p 352 1872 100 0 1 PV:$(top)$(dev)
use eaos 544 2256 100 0 ExtAtLvdtLo
xform 0 416 2176
p 256 1902 100 0 0 EGU:lvdt position
p 352 2256 100 0 1 PV:$(top)$(dev)
use hwin 0 1223 100 0 hwin#332
xform 0 96 1264
p 112 1200 100 1280 -1 val(in):#<$(lvdt)>
use gmosBorderC -864 263 100 0 gmosBorderC#333
xform 0 816 1568
p 2388 392 100 512 1 File:/data/nimpkish/1/daoinsw/dev/gmos/current/develop/gmMask/capfast/gmMaskLvdt.sch
p 1796 436 150 0 1 Rev:
p 2076 512 120 256 -1 Title:Mask Extractor LVDT Zones
p 2116 448 100 1024 -1 author:B.Wooff
p 2116 416 100 1024 -1 date:September 15, 1999
use eais 544 1296 100 0 Lvdt
xform 0 416 1232
p 288 1152 100 0 1 DTYP:XYCOM-566 SE Scanned
p 32 974 100 0 0 EGU:lvdt position
p 288 1120 100 0 1 EGUF:5.0000000e+00
p 288 1088 100 0 1 EGUL:-5.0000000e+00
p 32 1070 100 0 0 PREC:3
p 352 1296 100 0 1 PV:$(top)$(dev)
p 288 1056 100 0 1 SCAN:.1 second
p 288 1024 100 0 1 SMOO:0.7
[comments]

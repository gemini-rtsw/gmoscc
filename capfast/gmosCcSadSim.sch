[schematic2]
uniq 36
[tools]
[detail]
s 752 1888 220 0 GMOS Simulated CC Status and Alarm Records
s 2736 1904 150 0 gmosCcSadSim.sch
[cell use]
use oslBorderC -176 -521 100 0 oslBorderC#35
xform 0 1504 784
use esirs 1392 23 100 0 atmExpAngle
xform 0 1600 176
p 1424 320 100 0 -1 DESC:Exit prism angle
p 1472 96 100 0 1 EGU:degrees
p 1472 64 100 0 1 FTVL:DOUBLE
p 1664 64 100 0 1 PREC:2
use esirs 800 7 100 0 atmEnpAngle
xform 0 1008 160
p 832 304 100 0 -1 DESC:Entrance prism angle
p 880 80 100 0 1 EGU:degrees
p 880 48 100 0 1 FTVL:DOUBLE
p 1072 48 100 0 1 PREC:2
use esirs 176 -9 100 0 atmPresent
xform 0 384 144
p 208 288 100 0 -1 DESC:atmDC installed?
p 256 64 100 0 0 EGU:
p 256 32 100 0 1 FTVL:LONG
use esirs 800 535 100 0 mskFP
xform 0 1008 688
p 816 848 100 0 -1 DESC:Type of device in focal plane
p 880 576 100 0 1 FTVL:LONG
use esirs 160 535 100 0 mskID
xform 0 368 688
p 224 832 100 0 -1 DESC:ID of mask or IFU in beam
p 240 576 100 0 1 FTVL:STRING
use esirs 1424 1415 100 0 dtaX
xform 0 1632 1568
p 1456 1712 100 0 -1 DESC:Current X position
p 1504 1488 100 0 1 EGU:microns
p 1504 1456 100 0 1 FTVL:DOUBLE
p 1696 1456 100 0 1 PREC:2
use esirs 832 1399 100 0 dtaY
xform 0 1040 1552
p 864 1696 100 0 -1 DESC:Current Y position
p 912 1472 100 0 1 EGU:microns
p 912 1440 100 0 1 FTVL:DOUBLE
p 1104 1440 100 0 1 PREC:2
use esirs 208 1383 100 0 dtaZ
xform 0 416 1536
p 240 1680 100 0 -1 DESC:Current Z position
p 288 1456 100 0 1 EGU:microns
p 288 1424 100 0 1 FTVL:DOUBLE
p 480 1424 100 0 1 PREC:2
use ebos 240 1031 100 0 interlock
xform 0 368 1120
p 128 1248 100 0 -1 DESC:Temporary simulation of CC interlock status
p 256 944 100 0 1 ONAM:INTERLOCK
p 256 976 100 0 1 ZNAM:OK
[comments]

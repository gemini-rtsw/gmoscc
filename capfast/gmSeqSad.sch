[schematic2]
uniq 49
[tools]
[detail]
w 3984 1755 100 0 n#47 junction 3952 1792 3952 1744 4064 1744 gmSeqHealth.gmSeqHealth#36.DC
w 3864 1803 100 0 n#47 elongouts.dcDisabled.VAL 3792 1792 3984 1792 3984 2080 4080 2080 gmSeqState.gmSeqState#35.DC
w 3968 1867 100 0 n#46 junction 3920 2144 3920 1856 4064 1856 gmSeqHealth.gmSeqHealth#36.CC
w 3880 2155 100 0 n#46 elongouts.ccDisabled.VAL 3792 2144 4016 2144 4016 2192 4080 2192 gmSeqState.gmSeqState#35.CC
w 3540 1819 100 2 n#41 elongouts.dcDisabled.DOL 3536 1824 3536 1824 hwin.hwin#43.in
w 3540 2171 100 2 n#40 hwin.hwin#39.in 3536 2176 3536 2176 elongouts.ccDisabled.DOL
s 1808 2736 220 0 GMOS Status and Alarm Database
s 4368 2800 150 0 gmSeqSad.sch
[cell use]
use esirs 176 1223 100 0 grRulingDensity
xform 0 384 1376
p 112 1520 100 0 -1 DESC:Ruling Density of current grating
p 256 1312 100 0 1 EGU:lines/mm
p 112 960 100 0 0 FDSC:Ruling density of grating
p 256 1264 100 0 1 FTVL:LONG
use esirs 832 1223 100 0 grBlazeDir
xform 0 1040 1376
p 752 1536 100 0 -1 DESC:Blaze orientation of current grating
p 768 1024 100 0 0 EGU:0/1
p 768 960 100 0 0 FDSC:Blaze orientation of grating
p 912 1264 100 0 1 FTVL:LONG
use esirs 2512 55 100 0 atmWavelen2
xform 0 2720 208
p 2544 352 100 0 -1 DESC:Upper wavelength for AtmDC
p 2592 128 100 0 1 EGU:nm
p 2448 -208 100 0 0 FDSC:Upper wavelength for AtmDC
p 2592 96 100 0 1 FTVL:DOUBLE
p 2784 96 100 0 1 PREC:2
use esirs 1920 39 100 0 atmWavelen1
xform 0 2128 192
p 1952 336 100 0 -1 DESC:Lower wavelength for AtmDC
p 2000 112 100 0 1 EGU:nm
p 1856 -224 100 0 0 FDSC:Lower wavelength for AtmDC
p 2000 80 100 0 1 FTVL:DOUBLE
p 2192 80 100 0 1 PREC:2
use esirs 1392 23 100 0 atmEnpMean
xform 0 1600 176
p 1424 320 100 0 -1 DESC:Mean entrance prism angle during obs
p 1472 96 100 0 1 EGU:degrees
p 1328 -240 100 0 0 FDSC:Mean entrance prism angle during observation
p 1472 64 100 0 1 FTVL:DOUBLE
p 1664 64 100 0 1 PREC:2
use esirs 800 7 100 0 atmEnpEnd
xform 0 1008 160
p 832 304 100 0 -1 DESC:Entrance prism angle at obs end
p 880 80 100 0 1 EGU:degrees
p 736 -256 100 0 0 FDSC:Entrance prism angle at end of observation
p 880 48 100 0 1 FTVL:DOUBLE
p 1072 48 100 0 1 PREC:2
use esirs 176 -9 100 0 atmEnpStart
xform 0 384 144
p 208 288 100 0 -1 DESC:Entrance prism angle at obs start
p 256 64 100 0 1 EGU:degrees
p 112 -272 100 0 0 FDSC:Entrance prism angle at start of observation
p 256 32 100 0 1 FTVL:DOUBLE
p 448 32 100 0 1 PREC:2
use esirs 1392 423 100 0 atmExpMean
xform 0 1600 576
p 1424 720 100 0 -1 DESC:Mean exit prism angle during obs
p 1472 496 100 0 1 EGU:degrees
p 1328 160 100 0 0 FDSC:Mean exit prism angle during observation
p 1472 464 100 0 1 FTVL:DOUBLE
p 1664 464 100 0 1 PREC:2
use esirs 800 407 100 0 atmExpEnd
xform 0 1008 560
p 832 704 100 0 -1 DESC:Exit prism angle at obs end
p 880 480 100 0 1 EGU:degrees
p 736 144 100 0 0 FDSC:Exit prism angle at end of observation
p 880 448 100 0 1 FTVL:DOUBLE
p 1072 448 100 0 1 PREC:2
use esirs 176 391 100 0 atmExpStart
xform 0 384 544
p 208 688 100 0 -1 DESC:Exit prism angle at obs start
p 256 464 100 0 1 EGU:degrees
p 112 128 100 0 0 FDSC:Exit prism angle at start of observation
p 256 432 100 0 1 FTVL:DOUBLE
p 448 432 100 0 1 PREC:2
use esirs 1392 871 100 0 dtaZMean
xform 0 1600 1024
p 1424 1168 100 0 -1 DESC:Mean Z position during observation
p 1472 944 100 0 1 EGU:microns
p 1328 608 100 0 0 FDSC:Mean focus during observation
p 1472 912 100 0 1 FTVL:DOUBLE
p 1664 912 100 0 1 PREC:2
use esirs 816 855 100 0 dtaZEnd
xform 0 1024 1008
p 848 1152 100 0 -1 DESC:Z position at observation end
p 896 928 100 0 1 EGU:microns
p 752 592 100 0 0 FDSC:Focus at end of observation
p 896 896 100 0 1 FTVL:DOUBLE
p 1088 896 100 0 1 PREC:2
use esirs 192 839 100 0 dtaZStart
xform 0 400 992
p 224 1136 100 0 -1 DESC:Z position at observation start
p 272 912 100 0 1 EGU:microns
p 128 576 100 0 0 FDSC:Focus at start of observation
p 272 880 100 0 1 FTVL:DOUBLE
p 464 880 100 0 1 PREC:2
use esirs 2624 1655 100 0 grWavelen
xform 0 2832 1808
p 2656 1952 100 0 -1 DESC:Grating central wavelength
p 2704 1728 100 0 1 EGU:nanometres
p 2560 1392 100 0 0 FDSC:Grating central wavelength
p 2704 1696 100 0 1 FTVL:DOUBLE
p 2896 1696 100 0 1 PREC:2
use esirs 2080 1655 100 0 grOrder
xform 0 2288 1808
p 2176 1952 100 0 -1 DESC:Grating order
p 2016 1456 100 0 0 EGU:order
p 2016 1392 100 0 0 FDSC:Grating order
p 2160 1696 100 0 1 FTVL:LONG
use esirs 1392 1655 100 0 grName
xform 0 1600 1808
p 1392 1952 100 0 -1 DESC:Name of currently selected grating
p 1328 1392 100 0 0 FDSC:Name of selected grating
p 1472 1696 100 0 1 FTVL:STRING
use esirs 816 1655 100 0 filtName2
xform 0 1024 1808
p 896 1952 100 0 -1 DESC:Name of second filter
p 752 1392 100 0 0 FDSC:Name of second filter
p 896 1696 100 0 1 FTVL:STRING
use esirs 176 1655 100 0 filtName1
xform 0 384 1808
p 272 1952 100 0 -1 DESC:Name of first filter
p 112 1392 100 0 0 FDSC:Name of first filter
p 256 1712 100 0 1 FTVL:STRING
use esirs 2080 2087 100 0 heartBeat
xform 0 2288 2240
p 2144 2400 100 0 -1 DESC:"System alive" heartbeat
p 2192 2128 100 0 1 EGU:Ticks
p 2016 1824 100 0 0 FDSC:System heart beat
p 2192 2176 100 0 1 FTVL:LONG
p 1840 2304 100 0 -1 def(INP):0.0
p 2032 2336 75 1024 -1 pproc(INP):NPP
use esirs 1392 2087 100 0 state
xform 0 1600 2240
p 1488 2400 100 0 -1 DESC:Overall state of GMOS
p 1328 1888 100 0 0 EGU:state
p 1328 1824 100 0 0 FDSC:Whether GMOS is BOOTING, INITIALIZING or RUNNING
p 1472 2128 100 0 1 FTVL:STRING
use esirs 200 2088 100 0 name
xform 0 384 2240
p 352 2400 100 0 -1 DESC:GMOS system name
p 112 1824 100 0 0 FDSC:Mandatory record that holds system name
p 256 2126 100 0 1 FTVL:STRING
p 256 2158 100 0 1 SCAN:Passive
p -288 2366 100 0 0 def(INP):0.0
use esirs 840 2088 100 0 health
xform 0 1024 2240
p 928 2398 100 0 -1 DESC:GMOS health record
p 752 1888 100 0 0 EGU:health
p 752 1824 100 0 0 FDSC:GMOS health record
p 896 2126 100 0 1 FTVL:STRING
p 896 2158 100 0 0 SCAN:Passive
p 352 2366 100 0 0 def(INP):0.0
use esirs 2624 1255 100 0 grEffWavelen
xform 0 2832 1408
p 2656 1552 100 0 -1 DESC:Grating effective wavelength
p 2704 1328 100 0 1 EGU:nanometres
p 2560 992 100 0 0 FDSC:Grating effective wavelength
p 2704 1296 100 0 1 FTVL:DOUBLE
p 2896 1296 100 0 1 PREC:2
use hwin 3344 2135 100 0 hwin#39
xform 0 3440 2176
p 3216 2176 100 0 -1 val(in):$(gm)ccDisabled.VAL
use hwin 3344 1783 100 0 hwin#43
xform 0 3440 1824
p 3216 1824 100 0 -1 val(in):$(gm)dcDisabled.VAL
use elongouts 3536 2055 100 0 ccDisabled
xform 0 3664 2144
p 3376 2062 100 0 0 OMSL:closed_loop
p 3584 2224 100 0 1 SCAN:1 second
p 3312 2208 100 0 0 def(DOL):0.0
use elongouts 3536 1703 100 0 dcDisabled
xform 0 3664 1792
p 3376 1710 100 0 0 OMSL:closed_loop
p 3584 1872 100 0 1 SCAN:1 second
p 3312 1856 100 0 0 def(DOL):0.0
use gmSeqHealth 4064 1671 100 0 gmSeqHealth#36
xform 0 4240 1808
use gmSeqState 4080 2007 100 0 gmSeqState#35
xform 0 4256 2144
use oslBorderD -400 -441 100 0 oslBorderD#26
xform 0 2240 1264
[comments]

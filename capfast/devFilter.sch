[schematic2]
uniq 110
[tools]
[detail]
w -924 715 100 0 n#109 elongouts.Dir.FLNK -176 1248 64 1248 64 1632 -928 1632 -928 -192 448 -192 448 0 576 0 edevctlm.Device.SLNK
w -608 1232 100 0 dir inhier.dir.P -736 1216 -432 1216 elongouts.Dir.SLNK
w -152 779 100 0 vals inhier.vals.P -736 768 480 768 480 384 576 384 edevctlm.Device.VALS
w 2012 1011 100 0 vals estringouts.DeviceStrg.OUT 1984 1264 2016 1264 2016 768 480 768 junction
w 1700 1387 100 0 n#103 embbi.DeviceMenu.FLNK 1664 1504 1696 1504 1696 1280 1728 1280 estringouts.DeviceStrg.SLNK
w 1666 1323 100 0 n#102 embbi.DeviceMenu.VAL 1664 1312 1728 1312 estringouts.DeviceStrg.DOL
w -504 411 100 0 n#97 hwin.hwin#96.in -480 400 -480 400 ebis.Power.INP
w -184 395 100 0 n#95 ebis.Power.FLNK -224 384 -96 384 -96 320 32 320 ebos.PwrUpdate.SLNK
w -120 363 100 0 n#94 ebis.Power.VAL -224 352 32 352 ebos.PwrUpdate.DOL
w 408 299 100 0 n#92 ebos.PwrUpdate.OUT 288 288 576 288 edevctlm.Device.PSTA
w -232 11 100 0 dbug inhier.dbug.P -736 0 320 0 320 64 576 64 edevctlm.Device.DBUG
w -104 139 100 0 simm inhier.simm.P -736 128 576 128 edevctlm.Device.SIMM
w 1288 139 100 0 lswa edevctlm.Device.LSWA 896 128 1728 128 outhier.lswa.p
w 1294 171 100 0 mip edevctlm.Device.MIP 896 160 1728 160 outhier.mip.p
w 1294 459 100 0 ack edevctlm.Device.ACK 896 448 1728 448 outhier.ack.p
w 1288 203 100 0 mpos edevctlm.Device.MPOS 896 192 1728 192 outhier.mpos.p
w 1000 523 100 0 n#59 hwout.hwout#18.outp 1152 512 896 512 edevctlm.Device.OUT
w 1240 395 100 0 message_link edevctlm.Device.MSGL 896 384 1728 384 outhier.msgl.p
w 1258 427 100 0 busy_link edevctlm.Device.BSYL 896 416 1728 416 outhier.bsyl.p
w -184 587 100 0 velo inhier.velo.P -736 576 416 576 416 352 576 352 edevctlm.Device.VELO
w -136 875 100 0 mode inhier.mode.P -736 864 512 864 512 416 576 416 edevctlm.Device.MODE
w 144 976 100 0 devdir elongouts.Dir.OUT -176 1184 -64 1184 -64 960 544 960 544 448 576 448 edevctlm.Device.DIR
s 704 848 100 0 This menu for engineering testing only
[cell use]
use elongouts -432 1127 100 0 Dir
xform 0 -304 1216
p -368 1072 100 0 1 OMSL:closed_loop
p -368 1040 100 0 1 PV:$(top)$(dev)
use estringouts 1984 1344 100 0 DeviceStrg
xform 0 1856 1280
p 1792 1184 100 0 1 OMSL:closed_loop
p 1792 1344 100 0 1 PV:$(top)$(dev)
use embbi 896 1712 100 0 DeviceMenu
xform 0 1152 1296
p 1184 1214 100 0 1 EIST:pos8
p 1184 1118 100 0 1 ELST:pos11
p 1184 990 100 0 1 FFST:INVALID
p 1184 1342 100 0 1 FRST:pos4
p 1184 1022 100 0 1 FTST:park
p 1184 1310 100 0 1 FVST:pos5
p 1184 1182 100 0 1 NIST:pos9
p 1184 1438 100 0 1 ONST:pos1
p 704 1712 100 0 1 PV:$(top)$(dev)
p 1184 1246 100 0 1 SVST:pos7
p 1184 1278 100 0 1 SXST:pos6
p 1184 1150 100 0 1 TEST:pos10
p 1184 1374 100 0 1 THST:pos3
p 1184 1054 100 0 1 TTST:pos11Barcode
p 1184 1086 100 0 1 TVST:pos0Barcode
p 1184 1406 100 0 1 TWST:pos2
p 1184 1470 100 0 1 ZRST:pos0
use ukatcBorderC -1184 -729 100 0 ukatcBorderC#98
xform 0 496 576
p 1756 -480 120 256 -1 Title:GMOS Filter Wheel Device
p 1472 -544 100 768 -1 author:S Prior and S M Beard
use hwin -672 359 100 0 hwin#96
xform 0 -576 400
p -672 432 100 0 -1 val(in):#$(power)
use ebis -288 448 100 0 Power
xform 0 -352 368
p -416 256 100 0 1 DTYP:$(xycom)
p -416 224 100 0 1 ONAM:ON
p -480 448 100 0 1 PV:$(top)$(dev)
p -304 352 100 0 1 SCAN:.1 second
p -416 192 100 0 1 ZNAM:OFF
use ebos 224 416 100 0 PwrUpdate
xform 0 160 320
p 96 192 100 0 1 OMSL:closed_loop
p -288 174 100 0 0 ONAM:ON
p 32 416 100 0 1 PV:$(top)$(dev)
p -288 206 100 0 0 ZNAM:OFF
use inhier -768 16 100 512 dbug
xform 0 -736 0
use inhier -768 144 100 512 simm
xform 0 -736 128
use inhier -768 784 100 512 vals
xform 0 -736 768
use inhier -768 592 100 512 velo
xform 0 -736 576
use inhier -768 880 100 512 mode
xform 0 -736 864
use inhier -768 1232 100 512 dir
xform 0 -736 1216
use outhier 1760 192 100 0 mpos
xform 0 1712 192
use outhier 1760 384 100 0 msgl
xform 0 1712 384
use outhier 1760 416 100 0 bsyl
xform 0 1712 416
use outhier 1760 448 100 0 ack
xform 0 1712 448
use outhier 1760 160 100 0 mip
xform 0 1712 160
use outhier 1760 128 100 0 lswa
xform 0 1712 128
use edevctlm 832 560 100 0 Device
xform 0 736 256
p 960 -128 100 0 1 ACCL:0.5
p 1136 -96 100 0 1 BLCO:-0.25
p 1120 256 100 0 1 BTMO:0
p 640 608 100 0 -1 DESC:Filter wheel $(axis)
p 640 -64 100 0 1 DTYP:DEV CTL OMS 8/44
p 640 -256 100 0 1 EGU:half steps/200
p 1136 -64 100 0 1 FIVL:0.25
p 640 -192 100 0 1 IALG:3
p 640 -224 100 0 1 MRES:200
p 960 -320 100 0 1 MRND:2
p 960 -224 100 0 1 PHLM:15.0
p 960 -256 100 0 1 PLLM:-15.0
p 640 -288 100 0 1 PREC:4
p 1120 288 100 0 1 PTMO:30
p 640 560 100 0 1 PV:$(top)$(dev)
p 640 -320 100 0 1 SIMM:$(simm)
p 640 -128 100 0 1 TDIR:/gemini/gmos/lut
p 992 288 100 0 1 UAPB:YES
p 656 256 100 0 1 UBSB:NO
p 960 -288 100 0 1 UEIP:NO
p 656 288 100 0 1 UPSB:$(upsb)
p 960 -192 100 0 1 VBAS:0.1
p 960 -160 100 0 1 VELO:0.75
p 960 -64 100 0 1 VHLM:2.0
p 960 -96 100 0 1 VLLM:0.1
use hwout 1152 471 100 0 hwout#18
xform 0 1248 512
p 1216 544 100 0 -1 val(outp):#<$(motor)>
[comments]

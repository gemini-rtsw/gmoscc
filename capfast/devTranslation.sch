[schematic2]
uniq 94
[tools]
[detail]
w -264 227 100 0 n#93 elongouts.Dir.FLNK -320 1376 -288 1376 -288 1472 -928 1472 -928 224 448 224 448 352 576 352 edevctlm.Device.SLNK
w -688 1360 100 0 dir inhier.dir.P -736 1344 -576 1344 elongouts.Dir.SLNK
w 484 1379 100 0 vals estringouts.DeviceStrg.OUT 320 1648 480 1648 480 1120 junction
w -152 1131 100 0 vals inhier.vals.P -736 1120 480 1120 480 736 576 736 edevctlm.Device.VALS
w 2 1707 100 0 n#88 embbis.DeviceMenu.VAL 0 1696 64 1696 estringouts.DeviceStrg.DOL
w 36 1691 100 0 n#87 embbis.DeviceMenu.FLNK 0 1728 32 1728 32 1664 64 1664 estringouts.DeviceStrg.SLNK
w -232 363 100 0 dbug inhier.dbug.P -736 352 320 352 320 416 576 416 edevctlm.Device.DBUG
w -104 491 100 0 simm inhier.simm.P -736 480 576 480 edevctlm.Device.SIMM
w 1288 491 100 0 lswa edevctlm.Device.LSWA 896 480 1728 480 outhier.lswa.p
w 1294 523 100 0 mip edevctlm.Device.MIP 896 512 1728 512 outhier.mip.p
w -136 683 100 0 n#72 ebis.Power.FLNK -192 736 -160 736 -160 672 -64 672 ebos.PwrUpdate.SLNK
w 1294 811 100 0 ack edevctlm.Device.ACK 896 800 1728 800 outhier.ack.p
w 1288 555 100 0 mpos edevctlm.Device.MPOS 896 544 1728 544 outhier.mpos.p
w 1000 875 100 0 n#59 hwout.hwout#18.outp 1152 864 896 864 edevctlm.Device.OUT
w 1240 747 100 0 message_link edevctlm.Device.MSGL 896 736 1728 736 outhier.msgl.p
w 1258 779 100 0 busy_link edevctlm.Device.BSYL 896 768 1728 768 outhier.bsyl.p
w -184 939 100 0 velo inhier.velo.P -736 928 416 928 416 704 576 704 edevctlm.Device.VELO
w -136 1227 100 0 mode inhier.mode.P -736 1216 512 1216 512 768 576 768 edevctlm.Device.MODE
w -114 1323 100 0 devdir elongouts.Dir.OUT -320 1312 544 1312 544 800 576 800 edevctlm.Device.DIR
w -152 715 100 0 n#23 ebis.Power.VAL -192 704 -64 704 ebos.PwrUpdate.DOL
w -444 747 100 2 n#33 hwin.hwin#29.in -448 752 -448 752 ebis.Power.INP
w 360 651 100 0 n#22 ebos.PwrUpdate.OUT 192 640 576 640 edevctlm.Device.PSTA
s -160 1856 100 0 This menu for engineering testing only
[cell use]
use elongouts -576 1255 100 0 Dir
xform 0 -448 1344
p -512 1200 100 0 1 OMSL:closed_loop
p -512 1168 100 0 1 PV:$(top)$(dev)
use estringouts 320 1728 100 0 DeviceStrg
xform 0 192 1664
p 128 1568 100 0 1 OMSL:closed_loop
p 128 1728 100 0 1 PV:$(top)$(dev)
use embbis 0 1776 100 0 DeviceMenu
xform 0 -128 1712
p -192 1584 100 0 1 ONST:park
p -192 1776 100 0 1 PV:$(top)$(dev)
p -192 1552 100 0 1 TWST:INVALID
p -192 1616 100 0 1 ZRST:centre
use ukatcBorderC -1200 -553 100 0 ukatcBorderC#86
xform 0 480 752
p 1740 -304 120 256 -1 Title:GMOS CCD Translation Axis Schematic
p 1456 -368 100 768 -1 author:S M Beard
use inhier -768 1360 100 512 dir
xform 0 -736 1344
use inhier -768 1232 100 512 mode
xform 0 -736 1216
use inhier -768 944 100 512 velo
xform 0 -736 928
use inhier -768 1136 100 512 vals
xform 0 -736 1120
use inhier -768 496 100 512 simm
xform 0 -736 480
use inhier -768 368 100 512 dbug
xform 0 -736 352
use outhier 1760 480 100 0 lswa
xform 0 1712 480
use outhier 1760 512 100 0 mip
xform 0 1712 512
use outhier 1760 800 100 0 ack
xform 0 1712 800
use outhier 1760 768 100 0 bsyl
xform 0 1712 768
use outhier 1760 736 100 0 msgl
xform 0 1712 736
use outhier 1760 544 100 0 mpos
xform 0 1712 544
use edevctlm 832 912 100 0 Device
xform 0 736 608
p 960 224 100 0 1 ACCL:$(accl)
p 1152 240 100 0 1 BLCO:0.0000000e+00
p 1152 608 100 0 1 BTMO:0
p 640 960 100 0 -1 DESC:Translation $(axis) axis
p 640 288 100 0 1 DTYP:DEV CTL OMS 8/44
p 640 96 100 0 1 EGU:microns
p 1152 288 100 0 1 FIVL:$(fivl)
p 640 160 100 0 1 IALG:$(ialg)
p 640 128 100 0 1 MRES:$(mres)
p 960 32 100 0 1 MRND:1
p 960 128 100 0 1 PHLM:$(phlm)
p 960 96 100 0 1 PLLM:$(pllm)
p 640 64 100 0 1 PREC:1
p 1152 640 100 0 1 PTMO:30
p 640 32 100 0 1 SIMM:$(simm)
p 640 224 100 0 1 TDIR:/gemini/epics3.13.4/gmos/gmos/data
p 1024 640 100 0 1 UAPB:YES
p 656 608 100 0 1 UBSB:NO
p 960 64 100 0 1 UEIP:NO
p 656 640 100 0 1 UPSB:$(upsb)
p 960 160 100 0 1 VBAS:$(vbas)
p 960 192 100 0 1 VELO:$(velo)
p 960 288 100 0 1 VHLM:$(vhlm)
p 960 256 100 0 1 VLLM:$(vllm)
use hwout 1152 823 100 0 hwout#18
xform 0 1248 864
p 1216 896 100 0 -1 val(outp):#<$(motor)>
use ebos 128 752 100 0 PwrUpdate
xform 0 64 672
p -384 622 100 0 0 OMSL:closed_loop
p -384 526 100 0 0 ONAM:ON
p -64 752 100 0 1 PV:$(top)$(dev)
p -384 558 100 0 0 ZNAM:OFF
p 192 640 75 768 -1 pproc(OUT):NPP
use hwin -640 711 100 0 hwin#29
xform 0 -544 752
p -656 784 100 0 -1 val(in):#<$(power)>
use ebis -192 784 100 0 Power
xform 0 -320 720
p -384 640 100 0 1 DTYP:$(xycom)
p -384 544 100 0 1 ONAM:ON
p -384 784 100 0 1 PV:$(top)$(dev)
p -384 608 100 0 1 SCAN:.1 second
p -384 576 100 0 1 ZNAM:OFF
[comments]

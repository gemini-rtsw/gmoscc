[schematic2]
uniq 144
[tools]
[detail]
w -324 667 100 0 vals inhier.vals.P -1104 1184 -320 1184 -320 160 112 160 112 448 1056 448 1056 656 992 656 estringouts.DeviceStrg.OUT
w 136 171 100 0 vals junction 112 160 208 160 edevctlm.Device.VALS
w -292 715 100 0 mode inhier.mode.P -1104 1248 -288 1248 -288 192 208 192 edevctlm.Device.MODE
w -54 -21 100 0 n#142 ebos.PwrUpdate.OUT -176 -32 128 -32 128 64 208 64 edevctlm.Device.PSTA
w 68 971 100 0 n#141 elongouts.Dir.FLNK 32 1376 64 1376 64 576 0 576 0 -224 208 -224 edevctlm.Device.SLNK
w -682 1347 100 0 dir inhier.dir.P -1104 1344 -224 1344 elongouts.Dir.SLNK
w 708 699 100 0 n#137 embbis.DeviceMenu.FLNK 672 736 704 736 704 672 736 672 estringouts.DeviceStrg.SLNK
w 674 715 100 0 n#133 embbis.DeviceMenu.VAL 672 704 736 704 estringouts.DeviceStrg.DOL
w -356 619 100 0 velo inhier.velo.P -1104 1120 -352 1120 -352 128 208 128 edevctlm.Device.VELO
w 172 763 100 0 devdir elongouts.Dir.OUT 32 1312 176 1312 176 224 208 224 edevctlm.Device.DIR
w -812 75 100 2 n#93 hwin.hwin#96.in -816 80 -816 80 ebis.Power.INP
w -520 43 100 0 n#92 ebis.Power.VAL -560 32 -432 32 ebos.PwrUpdate.DOL
w -504 11 100 0 n#91 ebis.Power.FLNK -560 64 -528 64 -528 0 -432 0 ebos.PwrUpdate.SLNK
w -600 -213 100 0 dbug inhier.dbug.P -1104 -224 -48 -224 -48 -160 208 -160 edevctlm.Device.DBUG
w -472 -85 100 0 simm inhier.simm.P -1104 -96 208 -96 edevctlm.Device.SIMM
w 920 -85 100 0 lswa edevctlm.Device.LSWA 528 -96 1360 -96 outhier.lswa.p
w 926 -53 100 0 mip edevctlm.Device.MIP 528 -64 1360 -64 outhier.mip.p
w 926 235 100 0 ack edevctlm.Device.ACK 528 224 1360 224 outhier.ack.p
w 920 -21 100 0 mpos edevctlm.Device.MPOS 528 -32 1360 -32 outhier.mpos.p
w 632 299 100 0 n#59 hwout.hwout#18.outp 784 288 528 288 edevctlm.Device.OUT
w 872 171 100 0 message_link edevctlm.Device.MSGL 528 160 1360 160 outhier.msgl.p
w 890 203 100 0 busy_link edevctlm.Device.BSYL 528 192 1360 192 outhier.bsyl.p
s 416 496 100 0 This menu for engineering testing only
[cell use]
use elongouts -224 1255 100 0 Dir
xform 0 -96 1344
p -160 1200 100 0 1 OMSL:closed_loop
p -160 1168 100 0 1 PV:$(top)$(dev)
use embbis 672 784 100 0 DeviceMenu
xform 0 544 720
p 480 592 100 0 1 ONST:park
p 480 784 100 0 1 PV:$(top)$(dev)
p 480 560 100 0 1 TWST:INVALID
p 480 624 100 0 1 ZRST:barcode
use estringouts 992 736 100 0 DeviceStrg
xform 0 864 672
p 800 576 100 0 1 OMSL:closed_loop
p 800 736 100 0 1 PV:$(top)$(dev)
use ukatcBorderC -1376 -953 100 0 ukatcBorderC#130
xform 0 304 352
p 1564 -704 120 256 -1 Title:GMOS Grating Tilt Schematic
p 1280 -768 100 768 -1 author:S.Beard, C.Tierney, D.Terrett & S.Prior
use hwin -1008 39 100 0 hwin#96
xform 0 -912 80
p -1024 112 100 0 -1 val(in):#<$(power)>
use inhier -1136 1360 100 512 dir
xform 0 -1104 1344
use inhier -1136 1264 100 512 mode
xform 0 -1104 1248
use inhier -1136 1136 100 512 velo
xform 0 -1104 1120
use inhier -1136 1200 100 512 vals
xform 0 -1104 1184
use inhier -1136 -80 100 512 simm
xform 0 -1104 -96
use inhier -1136 -208 100 512 dbug
xform 0 -1104 -224
use inhier -1168 1472 100 0 id
xform 0 -1104 1456
use ebis -560 112 100 0 Power
xform 0 -688 48
p -752 -32 100 0 1 DTYP:$(xycom)
p -752 -160 100 0 1 ONAM:ON
p -752 112 100 0 1 PV:$(top)$(dev)
p -752 -64 100 0 1 SCAN:.1 second
p -752 -128 100 0 1 ZNAM:OFF
use ebos -240 80 100 0 PwrUpdate
xform 0 -304 0
p -752 -50 100 0 0 OMSL:closed_loop
p -752 -146 100 0 0 ONAM:ON
p -432 80 100 0 1 PV:$(top)$(dev)
p -752 -114 100 0 0 ZNAM:OFF
p -176 -32 75 768 -1 pproc(OUT):NPP
use outhier 1392 -96 100 0 lswa
xform 0 1344 -96
use outhier 1392 -64 100 0 mip
xform 0 1344 -64
use outhier 1392 224 100 0 ack
xform 0 1344 224
use outhier 1392 192 100 0 bsyl
xform 0 1344 192
use outhier 1392 160 100 0 msgl
xform 0 1344 160
use outhier 1392 -32 100 0 mpos
xform 0 1344 -32
use edevctlm 464 336 100 0 Device
xform 0 368 32
p 592 -352 100 0 1 ACCL:500.0
p 816 -336 100 0 1 BLCO:0.0
p 768 32 100 0 1 BTMO:0
p 272 368 100 0 -1 DESC:Grating Tilt $(axis)
p 272 -288 100 0 1 DTYP:DEV CTL OMS 8/44
p 272 -480 100 0 1 EGU:motor steps
p 816 -288 100 0 1 FIVL:126.0
p 272 -416 100 0 1 IALG:4
p 272 -448 100 0 1 MRES:2.0
p 592 -544 100 0 1 MRND:1
p 592 -448 100 0 1 PHLM:9800.0
p 592 -480 100 0 1 PLLM:0.0
p 272 -512 100 0 1 PREC:4
p 768 64 100 0 1 PTMO:30
p 272 -544 100 0 1 SIMM:$(simm)
p 272 -352 100 0 1 TDIR:./data
p 640 64 100 0 1 UAPB:YES
p 288 32 100 0 1 UBSB:NO
p 592 -512 100 0 1 UEIP:NO
p 288 64 100 0 1 UPSB:$(upsb)
p 592 -416 100 0 1 VBAS:125.0
p 592 -384 100 0 1 VELO:500.0
p 592 -288 100 0 1 VHLM:1000.0
p 592 -320 100 0 1 VLLM:10.0
use hwout 784 247 100 0 hwout#18
xform 0 880 288
p 848 320 100 0 -1 val(outp):#<$(motor)>
[comments]

[schematic2]
uniq 129
[tools]
[detail]
w -556 -13 100 0 n#128 hwin.hwin#127.in -704 -112 -560 -112 -560 96 -432 96 ecalcs.BrakeFaker.INPC
w -550 131 100 0 n#124 hwin.hwin#125.in -704 0 -608 0 -608 128 -432 128 ecalcs.BrakeFaker.INPB
w 292 27 100 0 n#121 ebos.BrakeStat.OUT 224 -96 288 -96 288 160 544 160 edevctlm.Device.BSTA
w -102 -53 100 0 n#120 ecalcs.BrakeFaker.FLNK -144 0 -112 0 -112 -64 -32 -64 ebos.BrakeStat.SLNK
w -118 -21 100 0 n#119 ecalcs.BrakeFaker.VAL -144 -32 -32 -32 ebos.BrakeStat.DOL
w 386 203 100 0 n#113 ebos.PwrUpdate.OUT 160 352 288 352 288 192 544 192 edevctlm.Device.PSTA
w -62 -445 100 0 n#112 eais.Brake.SLNK -864 176 -960 176 -960 -448 896 -448 896 160 864 160 edevctlm.Device.BRKL
w -516 -21 100 0 n#111 eais.Brake.FLNK -608 192 -512 192 -512 -224 -432 -224 ecalcs.BrakeFaker.SLNK
w -550 163 100 0 n#110 eais.Brake.VAL -608 160 -432 160 ecalcs.BrakeFaker.INPA
w 316 459 100 0 n#105 elongouts.Dir.FLNK -272 1024 320 1024 320 -96 544 -96 edevctlm.Device.SLNK
w -688 1008 100 0 dir inhier.dir.P -768 992 -528 992 elongouts.Dir.SLNK
w 1634 1227 100 0 n#100 embbi.DeviceMenu.VAL 1632 1216 1696 1216 estringouts.DeviceStrg.DOL
w 1668 1291 100 0 n#99 embbi.DeviceMenu.FLNK 1632 1408 1664 1408 1664 1184 1696 1184 estringouts.DeviceStrg.SLNK
w 1160 675 100 0 vals estringouts.DeviceStrg.OUT 1952 1168 1984 1168 1984 672 384 672 junction
w -216 675 100 0 vals inhier.vals.P -768 672 384 672 384 288 544 288 edevctlm.Device.VALS
w -476 459 100 2 n#93 hwin.hwin#96.in -480 464 -480 464 ebis.Power.INP
w -184 427 100 0 n#92 ebis.Power.VAL -224 416 -96 416 ebos.PwrUpdate.DOL
w -168 395 100 0 n#91 ebis.Power.FLNK -224 448 -192 448 -192 384 -96 384 ebos.PwrUpdate.SLNK
w 464 -29 100 0 dbug inhier.dbug.P 432 -32 544 -32 edevctlm.Device.DBUG
w 464 35 100 0 simm inhier.simm.P 432 32 544 32 edevctlm.Device.SIMM
w 1256 43 100 0 lswa edevctlm.Device.LSWA 864 32 1696 32 outhier.lswa.p
w 1262 75 100 0 mip edevctlm.Device.MIP 864 64 1696 64 outhier.mip.p
w 1262 363 100 0 ack edevctlm.Device.ACK 864 352 1696 352 outhier.ack.p
w 1256 107 100 0 mpos edevctlm.Device.MPOS 864 96 1696 96 outhier.mpos.p
w 968 427 100 0 n#59 hwout.hwout#18.outp 1120 416 864 416 edevctlm.Device.OUT
w 1208 299 100 0 message_link edevctlm.Device.MSGL 864 288 1696 288 outhier.msgl.p
w 1226 331 100 0 busy_link edevctlm.Device.BSYL 864 320 1696 320 outhier.bsyl.p
w -232 579 100 0 velo inhier.velo.P -768 576 352 576 352 256 544 256 edevctlm.Device.VELO
w -200 771 100 0 mode inhier.mode.P -768 768 416 768 416 320 544 320 edevctlm.Device.MODE
w 92 867 100 0 devdir elongouts.Dir.OUT -272 960 -192 960 -192 864 448 864 448 352 544 352 edevctlm.Device.DIR
s -192 -288 100 0 the indexer is engaged.
s -192 -256 100 0 which prevents the turret from moving while
s -192 -224 100 0 The lifter state is treated as a brake status
s 672 752 100 0 This menu for engineering testing only
s 1248 192 100 0 <-- No power control. Power controlled explicitly by assembly record.
s -144 128 100 0 Do not set this FLT field via channel access, connect to
s -192 96 100 0 $(top)$(dev)IlckCA in the lifterTurretIlck schematic instead
[cell use]
use hwin -672 423 100 0 hwin#96
xform 0 -576 464
p -688 496 100 0 -1 val(in):#<$(power)>
use hwin -896 -41 100 0 hwin#125
xform 0 -800 0
p -896 -48 100 0 0 typ(in):val
p -896 -48 100 0 -1 val(in):$(top)grLifter.VALD NPP NMS
use hwin -896 -153 100 0 hwin#127
xform 0 -800 -112
p -896 -160 100 0 0 typ(in):val
p -896 -160 100 0 -1 val(in):$(top)grLifterUp.VAL NPP NMS
use ukatcBorderC -1120 -825 100 0 ukatcBorderC#126
xform 0 560 480
p 1828 -512 120 256 -1 Project:Gemini Multi-object Optical Spectrometer
p 1820 -576 120 256 -1 Title:GMOS Grating Turret Device
p 1536 -640 100 768 -1 author:C.Tierney, S.Beard, S.Prior & D.Terrett
p 1536 -672 100 768 -1 date:$Date$
use eais -608 240 100 0 Brake
xform 0 -736 176
p -1120 -82 100 0 0 EGU:brake state
p -800 240 100 0 1 PV:$(top)$(dev)
use ebos 160 32 100 0 BrakeStat
xform 0 96 -64
p 32 -176 100 0 1 OMSL:closed_loop
p -352 -210 100 0 0 ONAM:disabled
p -32 32 100 0 1 PV:$(top)$(dev)
use ebos 96 464 100 0 PwrUpdate
xform 0 32 384
p -416 334 100 0 0 OMSL:closed_loop
p -416 238 100 0 0 ONAM:ON
p -96 464 100 0 1 PV:$(top)$(dev)
p -416 270 100 0 0 ZNAM:OFF
p 160 352 75 768 -1 pproc(OUT):NPP
use ecalcs -128 160 100 0 BrakeFaker
xform 0 -288 -48
p -368 -352 100 0 1 CALC:A||B||C
p -720 -162 100 0 0 EGU:brake state
p -320 160 100 0 1 PV:$(top)$(dev)
p -368 -320 100 0 1 SCAN:Passive
p -464 160 75 1280 -1 pproc(INPA):NPP
p -176 -64 100 1024 -1 username(U0):FLD0
use elongouts -528 903 100 0 Dir
xform 0 -400 992
p -464 848 100 0 1 OMSL:closed_loop
p -464 816 100 0 1 PV:$(top)$(dev)
use embbi 864 1616 100 0 DeviceMenu
xform 0 1120 1200
p 1152 1118 100 0 1 EIST:posDBarcode
p 1152 1246 100 0 1 FRST:park
p 1152 1214 100 0 1 FVST:posABarcode
p 1152 1086 100 0 1 NIST:INVALID
p 1152 1342 100 0 1 ONST:posB
p 672 1616 100 0 1 PV:$(top)$(dev)
p 1152 1150 100 0 1 SVST:posCBarcode
p 1152 1182 100 0 1 SXST:posBBarcode
p 1152 1278 100 0 1 THST:posD
p 1152 1310 100 0 1 TWST:posC
p 1152 1374 100 0 1 ZRST:posA
use estringouts 1952 1248 100 0 DeviceStrg
xform 0 1824 1184
p 1760 1088 100 0 1 OMSL:closed_loop
p 1760 1248 100 0 1 PV:$(top)$(dev)
use ebis -224 496 100 0 Power
xform 0 -352 432
p -416 352 100 0 1 DTYP:$(xycom)
p -416 240 100 0 1 ONAM:ON
p -416 496 100 0 1 PV:$(top)$(dev)
p -416 320 100 0 1 SCAN:.1 second
p -416 272 100 0 1 ZNAM:OFF
use inhier -800 1008 100 512 dir
xform 0 -768 992
use inhier -800 784 100 512 mode
xform 0 -768 768
use inhier -800 592 100 512 velo
xform 0 -768 576
use inhier -800 688 100 512 vals
xform 0 -768 672
use inhier 400 48 100 512 simm
xform 0 432 32
use inhier 400 -16 100 512 dbug
xform 0 432 -32
use outhier 1728 32 100 0 lswa
xform 0 1680 32
use outhier 1728 64 100 0 mip
xform 0 1680 64
use outhier 1728 352 100 0 ack
xform 0 1680 352
use outhier 1728 320 100 0 bsyl
xform 0 1680 320
use outhier 1728 288 100 0 msgl
xform 0 1680 288
use outhier 1728 96 100 0 mpos
xform 0 1680 96
use edevctlm 800 464 100 0 Device
xform 0 704 160
p 928 -224 100 0 1 ACCL:0.25
p 1104 -192 100 0 1 BLCO:2.0
p 1120 160 100 0 1 BTMO:1
p 608 512 100 0 -1 DESC:Grating Turret Drive
p 608 -160 100 0 1 DTYP:DEV CTL OMS 8/44
p 608 -352 100 0 1 EGU:teeth
p 1104 -160 100 0 1 FIVL:1.0
p 608 -288 100 0 1 IALG:4
p 608 -320 100 0 1 MRES:400.0
p 928 -416 100 0 1 MRND:2
p 928 -320 100 0 1 PHLM:120.0
p 928 -352 100 0 1 PLLM:0.0
p 608 -384 100 0 1 PREC:4
p 1120 192 100 0 1 PTMO:0
p 608 -416 100 0 1 SIMM:$(simm)
p 608 -224 100 0 1 TDIR:/gemini/gmos/lut
p 992 192 100 0 1 UAPB:NO
p 624 160 100 0 1 UBSB:YES
p 928 -384 100 0 1 UEIP:NO
p 624 192 100 0 1 UPSB:$(upsb)
p 928 -288 100 0 1 VBAS:0.75
p 928 -256 100 0 1 VELO:2.5
p 928 -160 100 0 1 VHLM:5.0
p 928 -192 100 0 1 VLLM:0.05
use hwout 1120 375 100 0 hwout#18
xform 0 1216 416
p 1184 448 100 0 -1 val(outp):#<$(motor)>
[comments]

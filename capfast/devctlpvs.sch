[schematic2]
uniq 86
[tools]
[detail]
w -232 363 100 0 dbug inhier.dbug.P -736 352 320 352 320 416 576 416 edevctlm.Device.DBUG
w -104 491 100 0 simm inhier.simm.P -736 480 576 480 edevctlm.Device.SIMM
w 1288 491 100 0 lswa edevctlm.Device.LSWA 896 480 1728 480 outhier.lswa.p
w 1294 523 100 0 mip edevctlm.Device.MIP 896 512 1728 512 outhier.mip.p
w -152 1131 100 0 vals inhier.vals.P -736 1120 480 1120 480 736 576 736 edevctlm.Device.VALS
w -136 683 100 0 n#72 ebis.Power.FLNK -192 736 -160 736 -160 672 -64 672 ebos.PwrUpdate.SLNK
w 1294 811 100 0 ack edevctlm.Device.ACK 896 800 1728 800 outhier.ack.p
w 1288 555 100 0 mpos edevctlm.Device.MPOS 896 544 1728 544 outhier.mpos.p
w 1000 875 100 0 n#59 hwout.hwout#18.outp 1152 864 896 864 edevctlm.Device.OUT
w 1240 747 100 0 message_link edevctlm.Device.MSGL 896 736 1728 736 outhier.msgl.p
w 1258 779 100 0 busy_link edevctlm.Device.BSYL 896 768 1728 768 outhier.bsyl.p
w -184 939 100 0 velo inhier.velo.P -736 928 416 928 416 704 576 704 edevctlm.Device.VELO
w -136 1227 100 0 mode inhier.mode.P -736 1216 512 1216 512 768 576 768 edevctlm.Device.MODE
w -114 1323 100 0 dir inhier.dir.P -736 1312 544 1312 544 800 576 800 edevctlm.Device.DIR
w -152 715 100 0 n#23 ebis.Power.VAL -192 704 -64 704 ebos.PwrUpdate.DOL
w -444 747 100 2 n#33 hwin.hwin#29.in -448 752 -448 752 ebis.Power.INP
w 360 651 100 0 n#22 ebos.PwrUpdate.OUT 192 640 576 640 edevctlm.Device.PSTA
s 480 -41 100 0 NOTE: all the device record properties
s 480 -73 100 0 are initialised using pvload, so the
s 480 -105 100 0 above record does not necessarily have
s 480 -137 100 0 all its properties defined correctly
s 480 -169 100 0 on this Capfast schematic.
[cell use]
use inhier -768 368 100 512 dbug
xform 0 -736 352
use inhier -768 496 100 512 simm
xform 0 -736 480
use inhier -768 1136 100 512 vals
xform 0 -736 1120
use inhier -768 944 100 512 velo
xform 0 -736 928
use inhier -768 1232 100 512 mode
xform 0 -736 1216
use inhier -768 1328 100 512 dir
xform 0 -736 1312
use outhier 1760 544 100 0 mpos
xform 0 1712 544
use outhier 1760 736 100 0 msgl
xform 0 1712 736
use outhier 1760 768 100 0 bsyl
xform 0 1712 768
use outhier 1760 800 100 0 ack
xform 0 1712 800
use outhier 1760 512 100 0 mip
xform 0 1712 512
use outhier 1760 480 100 0 lswa
xform 0 1712 480
use edevctlm 832 912 100 0 Device
xform 0 736 608
p 736 336 100 256 1 PREC:2
p 640 224 100 0 1 TDIR:/gemini/epics3.13.4/gmos/gmos/data
use gmosBorderC -1120 -825 100 0 gmosBorderC#40
xform 0 560 480
p 2132 -696 100 512 1 File:/data/nimpkish/1/daoinsw/dev/gmos/current/develop/gmDeviceControl/capfast/devctlpvs.sch
p 1540 -652 150 0 1 Rev:A
p 1820 -576 120 256 -1 Title:GMOS Device Without Brake (VALS)
p 1860 -640 100 1024 -1 author:B.Wooff
p 1860 -672 100 1024 -1 date:August 26, 1999
use hwout 1152 823 100 0 hwout#18
xform 0 1248 864
p 1216 896 100 0 -1 val(outp):#<$(motor)>
use ebos 128 752 100 0 PwrUpdate
xform 0 64 672
p 0 560 100 0 1 OMSL:closed_loop
p 0 496 100 0 1 ONAM:ON
p -64 752 100 0 1 PV:$(top)$(dev)
p 0 528 100 0 1 ZNAM:OFF
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

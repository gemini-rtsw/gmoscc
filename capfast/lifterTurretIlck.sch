[schematic2]
uniq 112
[tools]
[detail]
w 866 907 100 0 n#111 elongouts.IlckCA.VAL 800 896 992 896 992 1088 1168 1088 ecalcs.pressureIlck.INPD
w 868 971 100 0 n#91 elongouts.IlckFwd.FLNK 800 1152 864 1152 864 800 junction
w 952 803 100 0 n#91 inhier.slnk.P 784 800 1168 800 ecalcs.pressureIlck.SLNK
w 808 939 100 0 n#91 elongouts.IlckCA.FLNK 800 928 864 928 junction
w 1908 995 100 0 turretIlck elongouts.pressureIlnk.OUT 1824 992 2112 992 outhier.turretIlck.p
w 288 944 100 0 lifterIlck edfans.ilckFanout.OUTB 176 1088 272 1088 272 800 368 800 outhier.lifterIlck.p
w 330 1123 100 0 n#105 edfans.ilckFanout.OUTA 176 1120 544 1120 elongouts.IlckFwd.SLNK
w -222 1067 100 0 estop inhier.estop.P -304 1056 -80 1056 edfans.ilckFanout.SLNK
w 960 1123 100 0 n#98 elongouts.IlckFwd.VAL 800 1120 1168 1120 ecalcs.pressureIlck.INPC
w 1500 1019 100 0 n#94 ecalcs.pressureIlck.VAL 1456 992 1504 992 1504 1056 1568 1056 elongouts.pressureIlnk.DOL
w 1488 1027 100 0 n#93 ecalcs.pressureIlck.FLNK 1456 1024 1568 1024 elongouts.pressureIlnk.SLNK
w 892 1291 100 0 n#61 inhier.downPressureOK.P 896 1440 896 1152 1168 1152 ecalcs.pressureIlck.INPB
w 1052 1307 100 0 n#58 inhier.upPressureOK.P 1056 1440 1056 1184 1168 1184 ecalcs.pressureIlck.INPA
[cell use]
use elongouts 1808 1120 100 0 pressureIlnk
xform 0 1696 1024
p 1616 928 100 0 1 OMSL:closed_loop
p 1616 1120 100 0 1 PV:$(top)$(dev)
use elongouts 704 1200 100 0 IlckFwd
xform 0 672 1120
p 560 1024 100 0 1 OMSL:supervisory
p 512 1200 100 0 1 PV:$(top)$(dev)
use elongouts 704 976 100 0 IlckCA
xform 0 672 896
p 560 800 100 0 1 OMSL:supervisory
p 512 976 100 0 1 PV:$(top)$(dev)
use edfans 144 1280 100 0 ilckFanout
xform 0 48 1056
p -64 832 100 768 1 OMSL:supervisory
p -48 1280 100 0 1 PV:$(top)$(dev)
p 176 1120 75 768 -1 pproc(OUTA):PP
p 176 1088 75 768 -1 pproc(OUTB):NPP
use outhier 336 759 100 0 lifterIlck
xform 0 352 800
use outhier 2080 951 100 0 turretIlck
xform 0 2096 992
use inhier 976 1472 100 0 upPressureOK
xform 0 1056 1440
use inhier 784 1472 100 0 downPressureOK
xform 0 896 1440
use inhier 768 759 100 0 slnk
xform 0 784 800
use inhier -320 1015 100 0 estop
xform 0 -304 1056
use ecalcs 1408 1216 100 0 pressureIlck
xform 0 1312 976
p 1248 864 100 0 1 CALC:A||B||C||D
p 1216 1216 100 0 1 PV:$(top)$(dev)
use ukatcBorderC -576 -281 100 0 ukatcBorderC#0
xform 0 1104 1024
p 2364 -32 120 256 -1 Title:Grating turret lifter control
p 2080 -96 100 768 -1 author:C.Tierney, S.Beard & S.Prior
[comments]

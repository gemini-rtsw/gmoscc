[schematic2]
uniq 93
[tools]
[detail]
w 600 -157 100 0 n#87 eseq.Seq.LNK1 -96 -320 352 -320 352 -160 896 -160 junction
w 104 -341 100 0 n#87 eseq.Seq.LNK2 -96 -352 352 -352 352 -320 junction
w 1480 -381 100 0 FLNK ecars.C.FLNK 1280 -384 1728 -384 outhier.FLNK.p
w 1288 -93 100 0 n#87 ecars.C.IVAL 960 -160 896 -160 896 -96 1728 -96 outhier.IVAL.p
w -792 -341 100 0 n#55 hwin.hwin#53.in -864 -352 -672 -352 eseq.Seq.DOL2
w -796 -261 100 0 n#54 hwin.hwin#51.in -864 -192 -800 -192 -800 -320 -672 -320 eseq.Seq.DOL1
w -904 -637 100 0 n#46 inhier.SLNK.P -1088 -640 -672 -640 eseq.Seq.SLNK
s -624 -112 100 0 and then after a delay sets it back to "IDLE".
s -624 -80 100 0 This sequence record sets the CAR record to "BUSY"
s -1056 -304 100 0 CAR_IDLE = 0
s -1056 -144 100 0 CAR_BUSY = 2
s 448 -1488 500 0 gmosCadCar.sch
s -992 208 200 0 (TBD)
[cell use]
use notes -576 -1401 100 0 notes#85
xform 0 -320 -1216
p -48 -1250 100 0 0 AUTHOR:S M Beard
p -548 -1090 100 0 -1 COMMENT1:This schematic represents the contents
p -548 -1122 100 0 -1 COMMENT2:of the heirarchical symbol "gmosCadCar"
p -548 -1152 100 0 -1 COMMENT3:which is used by trivial commands
p -548 -1184 100 0 -1 COMMENT4:to toggle the CAR record to BUSY
p -548 -1216 100 0 -1 COMMENT5:and back to IDLE.
p -548 -1280 100 0 -1 COMMENT7:This GMOS version is copied from the CICS
p -548 -1312 100 0 -1 COMMENT8:and places the CAR record in the same
p -548 -1344 100 0 -1 COMMENT9:lock set. Client ID is ignored.
use outhier 1696 -425 100 0 FLNK
xform 0 1712 -384
use outhier 1696 -137 100 0 IVAL
xform 0 1712 -96
use eseq -672 -729 100 0 Seq
xform 0 -384 -432
p -352 -322 100 0 1 DLY2:$(delay)
p -608 -784 100 0 1 PV:$(top)$(cad)
p -96 -320 75 768 -1 pproc(LNK1):PP
p -96 -352 75 768 -1 pproc(LNK2):PP
use hwin -1056 -393 100 0 hwin#53
xform 0 -960 -352
p -1053 -360 100 0 -1 val(in):0
use hwin -1056 -233 100 0 hwin#51
xform 0 -960 -192
p -1053 -200 100 0 -1 val(in):2
use inhier -1104 -681 100 0 SLNK
xform 0 -1088 -640
use inhier -1104 183 100 0 STAT
xform 0 -1088 224
use ecars 960 -441 100 0 C
xform 0 1120 -272
p 1024 -448 100 0 1 DESC:$(cad) CAR record
p 1024 -480 100 0 1 PV:$(top)$(cad)
use bc200tr -1248 -1656 -100 0 frame
xform 0 432 -352
p 1328 -1488 100 0 1 author:S.M.Beard
p 1552 -1488 100 0 -1 border:C
p 1312 -1520 100 0 1 checked:B.Goodrich
p 1584 -1488 100 0 -1 date:18 Jan 2000
p 1552 -1376 100 0 -1 project:Core Instrument Control System
p 1552 -1440 100 0 -1 title:Wrap up for CAR "busy then idle"
[comments]

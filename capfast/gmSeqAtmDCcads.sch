[schematic2]
uniq 412
[tools]
[detail]
w 482 1475 100 0 c#406 gmSeqAtmTrackCad.gmSeqAtmTrackCad#398.TLNK 416 1472 608 1472 outhier.TLNK.p
w 482 1539 100 0 c#405 gmSeqAtmTrackCad.gmSeqAtmTrackCad#398.LMAX 416 1536 608 1536 outhier.LMAX.p
w 482 1603 100 0 c#404 gmSeqAtmTrackCad.gmSeqAtmTrackCad#398.LMIN 416 1600 608 1600 outhier.LMIN.p
w 482 1667 100 0 TRACK gmSeqAtmTrackCad.gmSeqAtmTrackCad#398.TRACK 416 1664 608 1664 outhier.TRACK.p
w -478 2083 100 0 n#402 eapply.atmApply.INPA -1120 1440 -1408 1440 -1408 2080 512 2080 512 1920 416 1920 gmSeqAtmTrackCad.gmSeqAtmTrackCad#398.VAL
w -478 2115 100 0 n#401 eapply.atmApply.INMA -1120 1408 -1440 1408 -1440 2112 544 2112 544 1856 416 1856 gmSeqAtmTrackCad.gmSeqAtmTrackCad#398.MESS
w -398 1411 100 0 n#400 eapply.atmApply.OCLA -736 1408 0 1408 0 1856 96 1856 gmSeqAtmTrackCad.gmSeqAtmTrackCad#398.ICID
w -422 1443 100 0 n#399 eapply.atmApply.OUTA -736 1440 -48 1440 -48 1920 96 1920 gmSeqAtmTrackCad.gmSeqAtmTrackCad#398.DIR
w -78 2211 100 0 n#397 gmSeqAtmPos.gmSeqAtmPos#386.MESS 1312 1744 1408 1744 1408 2208 -1504 2208 -1504 1344 -1120 1344 eapply.atmApply.INMB
w -78 2179 100 0 n#396 gmSeqAtmPos.gmSeqAtmPos#386.VAL 1312 1776 1376 1776 1376 2176 -1472 2176 -1472 1376 -1120 1376 eapply.atmApply.INPB
w 2 1347 100 0 n#392 eapply.atmApply.OCLB -736 1344 800 1344 800 1744 992 1744 gmSeqAtmPos.gmSeqAtmPos#386.ICID
w -14 1379 100 0 n#391 eapply.atmApply.OUTB -736 1376 768 1376 768 1776 992 1776 gmSeqAtmPos.gmSeqAtmPos#386.DIR
w -1248 1507 100 0 ICID inhier.ICID.P -1328 1504 -1120 1504 eapply.atmApply.CLID
w -1274 1587 100 0 DIR inhier.DIR.P -1328 1584 -1184 1584 -1184 1536 -1120 1536 eapply.atmApply.DIR
w -632 1571 100 0 FLNK eapply.atmApply.FLNK -736 1568 -480 1568 -480 1616 -416 1616 outhier.FLNK.p
w -548 1603 100 0 MESS eapply.atmApply.MESS -736 1504 -544 1504 -544 1712 -416 1712 outhier.MESS.p
w -580 1683 100 0 VAL eapply.atmApply.VAL -736 1536 -576 1536 -576 1840 -416 1840 outhier.VAL.p
s 1216 2544 140 0 gmSeqAtmDC.sch
[cell use]
use outhier -448 1575 100 0 FLNK
xform 0 -432 1616
use outhier -448 1671 100 0 MESS
xform 0 -432 1712
use outhier -448 1799 100 0 VAL
xform 0 -432 1840
use outhier 576 1623 100 0 TRACK
xform 0 592 1664
use outhier 576 1559 100 0 LMIN
xform 0 592 1600
use outhier 576 1495 100 0 LMAX
xform 0 592 1536
use outhier 576 1431 100 0 TLNK
xform 0 592 1472
use gmSeqAtmTrackCad 96 1415 100 0 gmSeqAtmTrackCad#398
xform 0 256 1704
p 384 1808 100 0 1 seta:top $(gm)atmCad
use gmSeqAtmPos 992 1447 100 0 gmSeqAtmPos#386
xform 0 1152 1664
use oslBorderC -1664 103 100 0 oslBorderC#379
xform 0 16 1408
p 1276 352 120 256 -1 Title:GMOS IS - Atmospheric Dispersion Compensator commands
use inhier -1344 1543 100 0 DIR
xform 0 -1328 1584
use inhier -1344 1463 100 0 ICID
xform 0 -1328 1504
use eapply -1120 903 100 0 atmApply
xform 0 -928 1264
[comments]

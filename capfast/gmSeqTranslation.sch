[schematic2]
uniq 409
[tools]
[detail]
w 674 1291 100 0 n#408 gmSeqDtaPos.gmSeqDtaPos#381.TRACKXY 1328 1536 1408 1536 1408 1280 0 1280 0 1536 112 1536 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.TRACKXY
w 674 1323 100 0 n#407 gmSeqDtaPos.gmSeqDtaPos#381.TRACKZ 1328 1504 1376 1504 1376 1312 32 1312 32 1504 112 1504 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.TRACKZ
w 460 1019 100 0 n#406 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.TRACK 432 1536 464 1536 464 512 1120 512 outhier.TRACK.p
w 778 635 100 0 n#405 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.TLNK 432 1568 496 1568 496 624 1120 624 outhier.TLNK.p
w 794 779 100 0 n#404 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.MODE 432 1600 528 1600 528 768 1120 768 outhier.MODE.p
w 842 1211 100 0 n#402 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.XOFF 432 1696 624 1696 624 1200 1120 1200 outhier.XOFF.p
w 826 1083 100 0 n#401 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.YOFF 432 1664 592 1664 592 1072 1120 1072 outhier.YOFF.p
w 810 939 100 0 n#400 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.ZOFF 432 1632 560 1632 560 928 1120 928 outhier.ZOFF.p
w -62 2059 100 0 n#390 gmSeqDtaPos.gmSeqDtaPos#381.MESS 1328 1744 1408 1744 1408 2048 -1472 2048 -1472 1360 -1120 1360 eapply.dtaApply.INMB
w -62 2027 100 0 n#389 gmSeqDtaPos.gmSeqDtaPos#381.VAL 1328 1776 1376 1776 1376 2016 -1440 2016 -1440 1392 -1120 1392 eapply.dtaApply.INPB
w -486 1963 100 0 n#387 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.VAL 432 1776 464 1776 464 1952 -1376 1952 -1376 1456 -1120 1456 eapply.dtaApply.INPA
w -486 1995 100 0 n#386 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.MESS 432 1744 496 1744 496 1984 -1408 1984 -1408 1424 -1120 1424 eapply.dtaApply.INMA
w -6 1371 100 0 n#385 eapply.dtaApply.OCLB -736 1360 784 1360 784 1744 1008 1744 gmSeqDtaPos.gmSeqDtaPos#381.ICID
w -22 1403 100 0 n#384 eapply.dtaApply.OUTB -736 1392 752 1392 752 1776 1008 1776 gmSeqDtaPos.gmSeqDtaPos#381.DIR
w -470 1435 100 0 n#383 eapply.dtaApply.OCLA -736 1424 -144 1424 -144 1744 112 1744 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.ICID
w -486 1467 100 0 n#382 eapply.dtaApply.OUTA -736 1456 -176 1456 -176 1776 112 1776 gmSeqDtaTrackCad.gmSeqDtaTrackCad#403.DIR
w -1286 1515 100 0 ICID inhier.ICID.P -1328 1504 -1184 1504 -1184 1520 -1120 1520 eapply.dtaApply.CLID
w -1286 1595 100 0 DIR inhier.DIR.P -1328 1584 -1184 1584 -1184 1552 -1120 1552 eapply.dtaApply.DIR
w -600 1587 100 0 FLNK eapply.dtaApply.FLNK -736 1584 -416 1584 outhier.FLNK.p
w -664 1523 100 0 MESS eapply.dtaApply.MESS -736 1520 -544 1520 -544 1712 -416 1712 outhier.MESS.p
w -580 1691 100 0 VAL eapply.dtaApply.VAL -736 1552 -576 1552 -576 1840 -416 1840 outhier.VAL.p
s 1216 2544 140 0 gmSeqTranslation.sch
[cell use]
use outhier 1088 727 100 0 MODE
xform 0 1104 768
use outhier 1088 583 100 0 TLNK
xform 0 1104 624
use outhier 1088 887 100 0 ZOFF
xform 0 1104 928
use outhier 1088 1031 100 0 YOFF
xform 0 1104 1072
use outhier 1088 1159 100 0 XOFF
xform 0 1104 1200
use outhier -448 1543 100 0 FLNK
xform 0 -432 1584
use outhier -448 1671 100 0 MESS
xform 0 -432 1712
use outhier -448 1799 100 0 VAL
xform 0 -432 1840
use outhier 1088 471 100 0 TRACK
xform 0 1104 512
use gmSeqDtaTrackCad 112 1447 100 0 gmSeqDtaTrackCad#403
xform 0 272 1664
p 132 1420 100 0 1 seta:top $(top)dtaCad
use gmSeqDtaPos 1008 1447 100 0 gmSeqDtaPos#381
xform 0 1168 1664
use oslBorderC -1632 103 100 0 oslBorderC#379
xform 0 48 1408
p 1308 352 120 256 -1 Title:GMOS IS - detector translation commands
use inhier -1344 1543 100 0 DIR
xform 0 -1328 1584
use inhier -1344 1463 100 0 ICID
xform 0 -1328 1504
use eapply -1120 919 100 0 dtaApply
xform 0 -928 1280
[comments]

[schematic2]
uniq 3
[tools]
[detail]
[cell use]
use gmosDcSim -64 1479 100 0 gmosDcSim#2
xform 0 192 1680
p -64 1456 100 0 1 seta:top g7:dc:
p -64 1424 100 0 1 setb:sadtop g7:sad:dc:
p 336 1440 100 0 -1 setc:CAR_IDLE 0
p 352 1392 100 0 -1 setd:CAR_BUSY 2
p -64 1360 100 0 1 sete:CAR_PAUSED 1
p -44 1340 100 0 1 setf:gm g7:dc:
use oslBorderC -1344 167 100 0 oslBorderC#0
xform 0 336 1472
p 448 304 100 0 -1 ID:$Id$
p -304 304 150 0 -1 Rev:$Revision$
p 1596 416 120 256 -1 Title:GMOS Top Level Components Controller SAD
p 1312 352 100 768 -1 author:S M Beard
p 1312 320 100 768 -1 date:$Date$
[comments]

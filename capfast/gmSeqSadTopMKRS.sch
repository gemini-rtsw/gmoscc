[schematic2]
uniq 3
[tools]
[detail]
s 992 2080 200 0 Top level schematic for the GMOS Sequencer
s 2768 2336 150 0 gmSeqSadTop.sch
s 1200 2016 200 0 Status and Alarm Database
[cell use]
use gmSeqSad 1168 1207 100 0 gmSeqSad#2
xform 0 1488 1392
p 1424 1296 100 0 -1 seta:top g6:sad:
p 1472 1248 100 0 -1 setb:gm g6:
p 1188 1180 100 0 1 setc:sad g6:sad:
use oslBorderC -144 -105 100 0 oslBorderC#1
xform 0 1536 1200
[comments]

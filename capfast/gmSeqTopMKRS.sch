[schematic2]
uniq 2
[tools]
[detail]
s 2768 2336 150 0 gmSeqTop.sch
s 992 2080 200 0 This is the top level schematic for the GMOS Sequencer.
s 992 2016 200 0 Its main purpose is to define the "top" macro for use
s 1008 1952 200 0 by the underlying schematics
[cell use]
use oslBorderC -144 -105 100 0 oslBorderC#1
xform 0 1536 1200
use gmSeq 1192 1024 100 0 gmSeq#0
xform 0 1536 1224
p 1280 1624 100 0 -1 seta:top g6:
p 1280 1592 100 0 -1 setb:gm g6:
p 1680 1672 100 0 -1 setc:CAR_IDLE 0
p 1680 1624 100 0 -1 setd:CAR_BUSY 2
p 1680 1576 100 0 -1 sete:CAR_ERROR 3
p 1680 1512 100 0 -1 setf:CAD_MARK 0
p 1680 1464 100 0 -1 setg:CAD_START 3
p 1280 1560 100 0 -1 seth:sad g6:sad:
p 1264 1512 100 0 -1 seti:cc g6:cc:
p 1264 1464 100 0 -1 setj:dc g6:dc:
p 1456 1464 100 0 -1 setk:sys gmSeq
p 1856 1504 100 0 -1 setl:CAD_PRESET 2
p 1856 1472 100 0 -1 setm:MODE_MOVE 1
p 1280 1712 100 0 -1 setn:tcs tcs:
p 1856 1552 100 0 -1 seto:CAD_STOP 4
p 1856 1600 100 0 -1 setp:CAD_CLEAR 1
[comments]

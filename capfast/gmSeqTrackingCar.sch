[schematic2]
uniq 122
[tools]
[detail]
w 434 2315 100 0 n#120 estringouts.PutMess.FLNK 352 2304 576 2304 elongouts.NotBusy.SLNK
w 594 2083 100 0 n#119 estringouts.PutMess.OUT 352 2272 384 2272 384 2080 864 2080 864 2208 992 2208 ecars.C.IMSS
w 68 2443 100 0 n#118 hwin.hwin#117.in 32 2576 64 2576 64 2320 96 2320 estringouts.PutMess.DOL
w 540 2459 100 0 n#113 hwin.hwin#112.in 496 2592 544 2592 544 2336 576 2336 elongouts.NotBusy.DOL
w 1474 1931 100 0 CFLK ecars.C.FLNK 1312 2048 1376 2048 1376 1920 1632 1920 outhier.CFLK.p
w 1490 2091 100 0 CERR ecars.C.OERR 1312 2176 1408 2176 1408 2080 1632 2080 outhier.CERR.p
w 1442 2219 100 0 CMESS ecars.C.OMSS 1312 2208 1632 2208 outhier.CMESS.p
w 1490 2379 100 0 CVAL ecars.C.VAL 1312 2272 1408 2272 1408 2368 1632 2368 outhier.CVAL.p
w -852 2539 100 0 n#99 gmSeqWaitChange.gmSeqWaitChange#90.NEWVAL -976 2368 -848 2368 -848 2720 -752 2720 ecalcs.Calc.INPA
w -380 2451 100 0 n#107 ecalcs.Calc.VAL -464 2528 -384 2528 -384 2384 -272 2384 efanouts.Fan.SELL
w 892 2059 100 0 n#87 elongouts.Busy.OUT 736 1856 896 1856 896 2272 992 2272 ecars.C.IVAL
w 840 2283 100 0 n#87 elongouts.NotBusy.OUT 832 2272 896 2272 junction
w 450 1931 100 0 n#106 elongouts.Busy.DOL 480 1920 480 1920 hwin.hwin#85.in
w 226 1899 100 0 STLK inhier.STLK.P 32 1888 480 1888 elongouts.Busy.SLNK
w -64 2299 100 0 n#97 efanouts.Fan.LNK4 -80 2288 0 2288 junction
w -4 2331 100 0 n#97 efanouts.Fan.LNK1 -80 2384 0 2384 0 2288 96 2288 estringouts.PutMess.SLNK
w -894 2339 100 0 n#100 gmSeqWaitChange.gmSeqWaitChange#90.FLNK -976 2336 -752 2336 ecalcs.Calc.SLNK
w -436 2427 100 0 n#92 ecalcs.Calc.FLNK -464 2560 -432 2560 -432 2304 -320 2304 efanouts.Fan.SLNK
s -80 1792 100 0 This link is activated
s -80 1760 100 0 as soon as a new tracking
s -80 1728 100 0 command is executed
s -848 2960 100 0 The $(car)C CAR record within the components controller is monitored.
s -848 2896 100 0 the tracking control CAR record is set IDLE or ERR whenever the
s -848 2848 100 0 components controller CAD changes to IDLE or ERR.
[cell use]
use ukatcBorderC -1440 903 100 0 ukatcBorderC#121
xform 0 240 2208
p 1500 1152 120 256 -1 Title:GMOS IS - Tracking Control CAR
p 1216 1088 100 768 -1 author:Author: smb
use hwin 288 1879 100 0 hwin#85
xform 0 384 1920
p 291 1912 100 0 -1 val(in):$(CAR_BUSY)
use hwin 304 2551 100 0 hwin#112
xform 0 400 2592
p 307 2584 100 0 -1 val(in):$(cc)$(car)Assembly.BUSY
use hwin -160 2535 100 0 hwin#117
xform 0 -64 2576
p -157 2568 100 0 -1 val(in):$(cc)$(car)Assembly.MESS
use estringouts 96 2215 100 0 PutMess
xform 0 224 2288
p 160 2192 100 0 1 OMSL:closed_loop
p 160 2160 100 0 1 PV:$(gm)$(car)Car
use outhier 1600 2327 100 0 CVAL
xform 0 1616 2368
use outhier 1600 2167 100 0 CMESS
xform 0 1616 2208
use outhier 1600 2039 100 0 CERR
xform 0 1616 2080
use outhier 1600 1879 100 0 CFLK
xform 0 1616 1920
use inhier 16 1847 100 0 STLK
xform 0 32 1888
use elongouts 480 1799 100 0 Busy
xform 0 608 1888
p 544 1760 100 0 1 OMSL:closed_loop
p 544 1728 100 0 1 PV:$(gm)$(car)Car
p 736 1856 75 768 -1 pproc(OUT):PP
use elongouts 576 2215 100 0 NotBusy
xform 0 704 2304
p 640 2176 100 0 1 OMSL:closed_loop
p 640 2144 100 0 1 PV:$(gm)$(car)Car
p 832 2272 75 768 -1 pproc(OUT):PP
use ecars 992 1991 100 0 C
xform 0 1152 2160
p 1056 1968 100 0 1 PV:$(gm)$(car)
use gmSeqWaitChange -1280 2263 100 0 gmSeqWaitChange#90
xform 0 -1128 2376
p -1280 2240 100 0 1 seta:monitor $(cc)$(car)Assembly.BUSY
p -1280 2208 100 0 1 setb:prefix $(gm)$(car)Car:
use efanouts -320 2167 100 0 Fan
xform 0 -200 2320
p -256 2112 100 0 1 PV:$(gm)$(car)Car
p -304 2448 100 0 1 SELM:Specified
use ecalcs -752 2247 100 0 Calc
xform 0 -608 2512
p -592 2688 100 0 1 CALC:A+1
p -688 2208 100 0 1 PV:$(gm)$(car)Car
[comments]

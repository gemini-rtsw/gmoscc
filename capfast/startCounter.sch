[schematic2]
uniq 279
[tools]
[detail]
w 914 1643 100 0 FLNK efanouts.DirFan.FLNK 192 1888 576 1888 576 1632 1312 1632 outhier.FLNK.p
w 770 2283 100 0 CNT ecalcs.Counter.VAL 992 1984 1024 1984 1024 2272 576 2272 576 2176 704 2176 ecalcs.Counter.INPA
w 1150 1995 100 0 CNT junction 1024 1984 1312 1984 outhier.CNT.p
w 418 1803 100 0 n#277 efanouts.DirFan.LNK3 192 1792 704 1792 ecalcs.Counter.SLNK
w -294 1787 100 0 SLNK inhier.SLNK.P -480 1776 -48 1776 efanouts.DirFan.SLNK
w -270 1867 100 0 DIR inhier.DIR.P -480 1856 0 1856 efanouts.DirFan.SELL
s 320 1808 100 0 DAR_DIR_START = 3
[cell use]
use outhier 1280 1943 100 0 CNT
xform 0 1296 1984
use outhier 1280 1591 100 0 FLNK
xform 0 1296 1632
use inhier -496 1815 100 0 DIR
xform 0 -480 1856
use inhier -496 1735 100 0 SLNK
xform 0 -480 1776
use ukatcBorderC -1120 295 100 0 ukatcBorderC#274
xform 0 560 1600
p 1820 544 120 256 -1 Title:Start Directive Counter
p 1536 480 100 768 -1 author:S M Beard
use ecalcs 848 2224 100 0 Counter
xform 0 848 1968
p 832 2176 100 0 1 CALC:(1+A)%100
p 416 1854 100 0 0 EGU:counter
p 832 2224 100 512 1 PV:$(top)$(dev)
p 832 2144 100 0 1 SCAN:Passive
use efanouts 128 1952 100 0 DirFan
xform 0 72 1792
p 96 1952 100 512 1 PV:$(top)$(dev)
p -96 1904 100 0 1 SELM:Specified
p 224 1792 75 1280 -1 pproc(LNK3):PP
[comments]

[schematic2]
uniq 9
[tools]
[detail]
w 46 1163 100 0 SLNK inhier.SLNK.P -256 1152 384 1152 eseqs.Seq.SLNK
w 734 1451 100 0 n#7 eseqs.Seq.LNK2 704 1440 800 1440 800 1472 junction
w 862 1483 100 0 n#7 eseqs.Seq.LNK1 704 1472 1056 1472 ecars.C.IVAL
w 388 1435 100 2 n#6 hwin.hwin#5.in 384 1440 384 1440 eseqs.Seq.DOL2
w 388 1467 100 2 n#4 hwin.hwin#3.in 384 1472 384 1472 eseqs.Seq.DOL1
[cell use]
use inhier -272 1111 100 0 SLNK
xform 0 -256 1152
use hwin 192 1431 100 0 hwin#3
xform 0 288 1472
p 195 1464 100 0 -1 val(in):2
use hwin 192 1399 100 0 hwin#5
xform 0 288 1440
p 195 1432 100 0 -1 val(in):0
use ecars 1264 1520 100 0 C
xform 0 1216 1360
p 1248 1520 100 512 -1 PV:$(top)$(dev)
use eseqs 608 1552 100 0 Seq
xform 0 544 1312
p 0 -868 100 0 0 DLY2:0.5e+00
p 592 1552 100 512 -1 PV:$(top)$(dev)
p 352 1472 75 1280 -1 pproc(DOL1):NPP
p 352 1440 75 1280 -1 pproc(DOL2):NPP
p 720 1472 75 1024 -1 pproc(LNK1):PP
p 720 1440 75 1024 -1 pproc(LNK2):PP
use gmosBorderC -592 -425 100 0 gmosBorderC#0
xform 0 1088 880
p 2660 -296 100 512 1 File:/data/nimpkish/1/daoinsw/dev/gmos/current/develop/gmOiwfs/capfast/gmOiwfsCmdCar.sch
p 2068 -252 150 0 1 Rev:1.0
p 2348 -176 120 256 -1 Title:Car for tolerance, follow and stop commands
p 2388 -240 100 1024 -1 author:Angelic Ebbers
p 2388 -272 100 1024 -1 date:October 15, 1999
[comments]

[schematic2]
uniq 74
[tools]
[detail]
w 2232 203 100 0 c#72 ecars.Tcar.FLNK 2192 192 2320 192 outhier.FLNK.p
w 1488 907 100 0 n#71 eaos.Busy.OUT 1312 896 1712 896 1712 416 junction
w 1488 459 100 0 n#71 eseqs.CarVals.LNK1 1376 448 1648 448 1648 416 1872 416 ecars.Tcar.IVAL
w 2232 427 100 0 CAR ecars.Tcar.VAL 2192 416 2320 416 outhier.CAR.p
w 152 235 100 0 c#62 inhier.SLNK.P 32 224 320 224 efanouts.fan.SLNK
w 856 130 100 0 n#49 efanouts.fan.LNK3 560 240 704 240 704 128 1056 128 eseqs.CarVals.SLNK
w 674 640 100 0 n#46 efanouts.fan.LNK1 560 304 672 304 672 928 1056 928 eaos.Busy.SLNK
w 1032 450 100 0 n#41 hwin.hwin#24.in 1056 448 1056 448 eseqs.CarVals.DOL1
w 1032 962 100 0 n#37 hwin.hwin#22.in 1056 960 1056 960 eaos.Busy.DOL
s 2112 1632 150 0 gmSeqCadCarA
s 832 480 100 0 IDLE
s 880 1008 100 0 BUSY
s 528 1232 180 0 - toggles CAR record only
[cell use]
use outhier 2288 375 100 0 CAR
xform 0 2304 416
use outhier 2288 151 100 0 FLNK
xform 0 2304 192
use ecars 1872 135 100 0 Tcar
xform 0 2032 304
p 2000 80 100 1024 1 name:$(gm)$(command)$(I)
use inhier 16 183 100 0 SLNK
xform 0 32 224
use oslBorderC -816 -809 100 0 oslBorderC#50
xform 0 864 496
use efanouts 344 88 100 0 fan
xform 0 440 240
p 336 352 100 0 1 SELM:All
p 496 48 100 1024 1 name:$(gm)$(command)$(I)
use eaos 1080 840 100 0 Busy
xform 0 1184 928
p 836 1114 100 0 0 DESC:Writes BUSY to CAR record
p 1120 800 100 0 1 OMSL:closed_loop
p 1408 910 100 0 0 def(DOL):0.0
p 1408 910 100 0 0 def(OUT):0.0
p 1248 1008 100 1024 1 name:$(gm)$(command)$(I)
p 1312 896 75 768 -1 pproc(OUT):PP
use hwin 888 408 100 0 hwin#24
xform 0 960 448
p 878 376 100 0 0 typ(in):val
p 867 440 100 0 -1 val(in):$(CAR_IDLE)
use hwin 888 920 100 0 hwin#22
xform 0 960 960
p 867 952 100 0 -1 val(in):$(CAR_BUSY)
use eseqs 1080 40 100 0 CarVals
xform 0 1216 288
p 1136 334 100 0 1 DLY1:0.2
p 1136 334 100 0 0 DLY2:0.0
p 1456 480 100 0 0 def(LNK1):0.0
p 1488 430 100 0 0 def(LNK2):0.0
p 1216 -16 100 1024 1 name:$(gm)$(command)$(I)
p 1056 480 75 1280 -1 palrm(SELL):NMS
p 1024 448 75 1280 -1 pproc(DOL1):NPP
p 1024 416 75 1280 -1 pproc(DOL2):NPP
p 1376 458 75 0 -1 pproc(LNK1):PP
p 1376 426 75 0 -1 pproc(LNK2):PP
[comments]

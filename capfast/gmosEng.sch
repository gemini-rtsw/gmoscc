[schematic2]
uniq 289
[tools]
[detail]
s 1600 1248 100 0 Filter, Grating and Translation engineering databases to be added.
[cell use]
use gmOiwfsEng 1216 1191 100 0 gmOiwfsEng#288
xform 0 1328 1264
p 1216 1184 100 0 -1 seta:top $(top)wfs:
p 1216 1152 100 0 -1 setb:GO 2
p 1216 1120 100 0 -1 setc:MOVE 1
p 1216 1088 100 0 -1 setd:START 3
p 1216 1056 100 0 -1 sete:INDEX 3
use gmMaskEng 896 1191 100 0 gmMaskEng#287
xform 0 1008 1264
p 896 1184 100 0 -1 seta:top $(top)$(c1)
p 896 1152 100 0 -1 setb:GO 2
p 896 1120 100 0 -1 setc:MOVE 1
p 896 1088 100 0 -1 setd:PARK 4
p 896 1056 100 0 -1 sete:START 3
p 896 1024 100 0 -1 setf:INDEX 3
use gmosBorderC -416 -153 100 0 gmosBorderC#91
xform 0 1264 1152
p 2840 -24 100 512 1 File:/data/nimpkish/1/daoinsw/dev/gmos/current/develop/gmMisc/capfast/gmPreEng.sch
p 1216 1088 100 0 0 IO:
p 2532 160 120 256 -1 Project:Gemini Multi-object Optical Spectrometer
p 2244 20 150 0 1 Rev:
p 2524 96 120 256 -1 Title:GMOS Engineering Test Driver
p 2564 32 100 1024 -1 author:B.Wooff
p 2564 0 100 1024 -1 date:February 17, 2000
p 1152 1056 100 0 0 model:
p 1152 1024 100 0 0 revision:
[comments]

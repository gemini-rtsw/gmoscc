[schematic2]
uniq 3
[tools]
[detail]
s 304 1971 500 0 GMOS - Mauna Kea
s 32 1843 500 0 Real pre focal plane hardware
s 32 1715 500 0 Simulated post focal plane
[cell use]
use gmosCcSub 640 1223 100 0 gmosCcSub#1
xform 0 800 1384
p 544 1176 100 0 1 seta:top g6:
p 544 1144 100 0 1 setb:sadtop g6:sad:
p 544 920 100 0 1 setl:preSimm NONE
p 896 920 100 0 1 setm:postSimm FULL
p 544 888 100 0 1 setn:preUpsb YES
p 896 888 100 0 1 seto:postUpsb NO
p 544 856 100 0 1 setp:preXycom XYCOM-240
p 896 856 100 0 1 setq:postXycom Soft Channel
p 544 824 100 0 1 setr:preIointr I/O Intr
p 896 824 100 0 1 sets:postIointr Passive
p 544 792 100 0 1 sett:bothXycom XYCOM-240
p 544 760 100 0 1 setu:bothIointr I/O Intr
p 896 1176 100 0 -1 setv:gm g6:
use ukatcBorderC -896 -265 100 0 ukatcBorderC#0
xform 0 784 1040
p 896 -128 100 0 -1 ID:$Id$
p 2052 48 120 256 -1 Project:Gemini Multi-object Optical Spectrograph
p 144 -128 150 0 -1 Rev:$Revision$
p 2044 -16 120 256 -1 Title:Components Controller top level database - simulated post focal-plane
p 2084 -80 100 1024 -1 author:S M Beard
p 1760 -112 100 768 -1 date:$Date$
[comments]

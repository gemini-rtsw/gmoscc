[schematic2]
uniq 36
[tools]
[detail]
w 64 731 100 0 c#29 inhier.END.P -64 720 240 720 efanouts.fanObsEnd.SLNK
w 192 1147 100 0 c#28 inhier.CONTINUE.P -64 1136 496 1136 efanouts.fanObsContinue.SLNK
w 224 1707 100 0 c#27 inhier.PAUSE.P -64 1696 560 1696 efanouts.fanObsPause.SLNK
w 104 2123 100 0 c#26 inhier.BEGIN.P -64 2112 320 2112 efanouts.fanObsBegin.SLNK
w 1048 747 100 0 n#25 efanouts.fanObsEnd.LNK3 480 736 1664 736 1664 576 2144 576 gmSeqObsMeanCalc.gmSeqObsMeanCalc#3.END
w 1048 779 100 0 n#24 efanouts.fanObsEnd.LNK2 480 768 1664 768 1664 1264 2144 1264 gmSeqObsMeanCalc.gmSeqObsMeanCalc#2.END
w 1032 811 100 0 n#22 efanouts.fanObsEnd.LNK1 480 800 1632 800 1632 1904 2160 1904 gmSeqObsMeanCalc.gmSeqObsMeanCalc#1.END
w 1232 1163 100 0 n#21 efanouts.fanObsContinue.LNK3 736 1152 1776 1152 1776 640 2144 640 gmSeqObsMeanCalc.gmSeqObsMeanCalc#3.CONTINUE
w 1232 1195 100 0 n#20 efanouts.fanObsContinue.LNK2 736 1184 1776 1184 1776 1328 2144 1328 gmSeqObsMeanCalc.gmSeqObsMeanCalc#2.CONTINUE
w 1224 1227 100 0 n#19 efanouts.fanObsContinue.LNK1 736 1216 1760 1216 1760 1968 2160 1968 gmSeqObsMeanCalc.gmSeqObsMeanCalc#1.CONTINUE
w 1288 1723 100 0 n#18 efanouts.fanObsPause.LNK3 800 1712 1824 1712 1824 704 2144 704 gmSeqObsMeanCalc.gmSeqObsMeanCalc#3.PAUSE
w 1304 1755 100 0 n#16 efanouts.fanObsPause.LNK2 800 1744 1856 1744 1856 1392 2144 1392 gmSeqObsMeanCalc.gmSeqObsMeanCalc#2.PAUSE
w 1304 1787 100 0 n#13 efanouts.fanObsPause.LNK1 800 1776 1856 1776 1856 2032 2160 2032 gmSeqObsMeanCalc.gmSeqObsMeanCalc#1.PAUSE
w 1264 2171 100 0 n#12 efanouts.fanObsBegin.LNK2 560 2160 2016 2160 2016 1456 2144 1456 gmSeqObsMeanCalc.gmSeqObsMeanCalc#2.BEGIN
w 1240 2139 100 0 n#11 efanouts.fanObsBegin.LNK3 560 2128 1968 2128 1968 768 2144 768 gmSeqObsMeanCalc.gmSeqObsMeanCalc#3.BEGIN
w 1286 2203 100 0 n#9 efanouts.fanObsBegin.LNK1 560 2192 2048 2192 2048 2096 2160 2096 gmSeqObsMeanCalc.gmSeqObsMeanCalc#1.BEGIN
s 2384 2496 150 0 gmSeqObsMean.sch
[cell use]
use oslBorderC -464 55 100 0 oslBorderC#35
xform 0 1216 1360
p 2476 304 120 256 -1 Title:GMOS IS - compute mean values
use inhier -80 2071 100 0 BEGIN
xform 0 -64 2112
use inhier -80 1655 100 0 PAUSE
xform 0 -64 1696
use inhier -80 1095 100 0 CONTINUE
xform 0 -64 1136
use inhier -80 679 100 0 END
xform 0 -64 720
use efanouts 320 1975 100 0 fanObsBegin
xform 0 440 2128
p 352 2256 100 0 1 SELM:All
use efanouts 560 1559 100 0 fanObsPause
xform 0 680 1712
p 576 1840 100 0 1 SELM:All
use efanouts 496 999 100 0 fanObsContinue
xform 0 616 1152
p 512 1280 100 0 1 SELM:All
use efanouts 240 583 100 0 fanObsEnd
xform 0 360 736
p 256 864 100 0 1 SELM:All
use gmSeqObsMeanCalc 2160 1799 100 0 gmSeqObsMeanCalc#1
xform 0 2320 2064
p 2384 2080 100 0 -1 seta:top $(top)enpAngle
p 2400 2048 100 0 -1 setb:value $(sad)cc:atmEnpAngle
p 2352 2000 100 0 -1 setc:meansad $(sad)atmEnpMean
p 2368 1968 100 0 -1 setd:beginsad $(sad)atmEnpStart
p 2368 1936 100 0 -1 sete:endsad $(sad)atmEnpEnd
use gmSeqObsMeanCalc 2144 1159 100 0 gmSeqObsMeanCalc#2
xform 0 2304 1424
p 2352 1424 100 0 -1 seta:top $(top)expAngle
p 2372 1404 100 0 -1 setb:value $(sad)cc:atmExpAngle
p 2392 1384 100 0 -1 setc:meansad $(sad)atmExpMean
p 2412 1364 100 0 -1 setd:beginsad $(sad)atmExpStart
p 2432 1344 100 0 -1 sete:endsad $(sad)atmExpEnd
use gmSeqObsMeanCalc 2144 471 100 0 gmSeqObsMeanCalc#3
xform 0 2304 736
p 2352 800 100 0 -1 seta:top $(top)dtaZpos
p 2372 780 100 0 -1 setb:value $(sad)cc:dtaZ
p 2392 760 100 0 -1 setc:meansad $(sad)dtaZMean
p 2412 740 100 0 -1 setd:beginsad $(sad)dtaZStart
p 2432 720 100 0 -1 sete:endsad $(sad)dtaZEnd
[comments]

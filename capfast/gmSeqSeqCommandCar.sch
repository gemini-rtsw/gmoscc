[schematic2]
uniq 39
[tools]
[detail]
w 3368 1003 100 0 CFLK egenSubD.egenSubD#18.FLNK 3312 992 3472 992 outhier.CFLK.p
w 3376 1579 100 0 CERR egenSubD.egenSubD#18.OUTC 3312 1568 3488 1568 outhier.CERR.p
w 3376 1643 100 0 c#32 egenSubD.egenSubD#18.OUTB 3312 1632 3488 1632 outhier.CMESS.p
w 3376 1707 100 0 CVAL egenSubD.egenSubD#18.OUTA 3312 1696 3488 1696 outhier.CVAL.p
w 2320 1339 100 0 n#30 elongouts.elongouts#10.FLNK 2192 1328 2496 1328 junction
w 2736 1035 100 0 n#30 elongouts.elongouts#14.FLNK 2208 2112 2496 2112 2496 1024 3024 1024 egenSubD.egenSubD#18.SLNK
w 2792 1675 100 0 n#26 elongouts.elongouts#14.VAL 2208 2080 2608 2080 2608 1664 3024 1664 egenSubD.egenSubD#18.INPC
w 2784 1579 100 0 n#25 elongouts.elongouts#10.VAL 2192 1296 2592 1296 2592 1568 3024 1568 egenSubD.egenSubD#18.INPF
w 2008 1483 100 0 n#22 ewait.ewait#9.VAL 1520 1472 2544 1472 2544 1632 3024 1632 egenSubD.egenSubD#18.INPD
w 2080 2267 100 0 n#19 ewait.MonChange.VAL 1536 2256 2672 2256 2672 1728 3024 1728 egenSubD.egenSubD#18.INPA
w 1664 1979 100 0 n#16 ewait.MonChange.FLNK 1536 1968 1840 1968 1840 2080 1952 2080 elongouts.elongouts#14.SLNK
w 1648 1195 100 0 n#12 ewait.ewait#9.FLNK 1520 1184 1824 1184 1824 1296 1936 1296 elongouts.elongouts#10.SLNK
s 3200 2528 140 0 gmSeqSeqCommandCar
[cell use]
use outhier 3504 1568 100 0 CERR
xform 0 3472 1568
use outhier 3504 1616 100 0 CMESS
xform 0 3472 1632
use outhier 3504 1680 100 0 CVAL
xform 0 3472 1696
use outhier 3488 976 100 0 CFLK
xform 0 3456 992
use egenSubD 3024 935 100 0 egenSubD#18
xform 0 3168 1360
p 2801 709 100 0 0 FTA:LONG
p 2801 709 100 0 0 FTB:STRING
p 2801 677 100 0 0 FTC:LONG
p 2801 645 100 0 0 FTD:LONG
p 2801 613 100 0 0 FTE:STRING
p 2801 549 100 0 0 FTF:LONG
p 2801 549 100 0 0 FTG:LONG
p 2801 517 100 0 0 FTH:STRING
p 2801 485 100 0 0 FTI:LONG
p 2801 709 100 0 0 FTVA:LONG
p 2801 709 100 0 0 FTVB:STRING
p 2801 677 100 0 0 FTVC:LONG
p 2801 645 100 0 0 FTVD:LONG
p 3024 896 100 0 1 SNAM:gmosCarCombineBusy
p 2656 1696 100 0 -1 def(INPB):$(cc)$(seqcommand)C.OMSS
p 2656 1600 100 0 -1 def(INPE):$(dc)$(seqcommand)C.OMSS
p 2656 1536 100 0 -1 def(INPG):$(gm)is$(seqcommand)C.VAL
p 2640 1504 100 0 -1 def(INPH):$(gm)is$(seqcommand)C.OMSS
p 2656 1472 100 0 -1 def(INPI):$(gm)is$(seqcommand)C.OERR
p 3136 928 100 1024 1 name:$(gm)$(seqcommand)CarCombine
use elongouts 1936 1207 100 0 elongouts#10
xform 0 2064 1296
p 2016 1152 100 0 1 OMSL:closed_loop
p 1648 1360 100 0 -1 def(DOL):$(dc)$(seqcommand)C.OERR
p 2064 1200 100 1024 1 name:$(gm)$(seqcommand)DcCarOerr
use elongouts 1952 1991 100 0 elongouts#14
xform 0 2080 2080
p 2016 1920 100 0 1 OMSL:closed_loop
p 1696 2144 100 0 -1 def(DOL):$(cc)$(seqcommand)C.OERR
p 2112 1968 100 1024 1 name:$(gm)$(seqcommand)CcCarOerr
use ewait 832 1879 -100 0 MonChange
xform 0 1184 2208
p 1155 2456 100 0 1 CALC:A
p 1216 2256 100 0 1 INAP:Yes
p 1216 2224 100 0 0 INBP:No
p 1216 2192 100 0 0 INCP:No
p 960 2032 100 0 1 OOPT:On Change
p 960 2350 100 0 1 SCAN:I/O Intr
p 528 2336 100 0 -1 def(INAN): $(cc)$(seqcommand)C.VAL
p 1120 1840 100 1024 1 name:$(gm)$(seqcommand)CcWaitCar
use ewait 816 1095 100 0 ewait#9
xform 0 1168 1424
p 1139 1672 100 0 1 CALC:A
p 1200 1472 100 0 1 INAP:Yes
p 1200 1440 100 0 0 INBP:No
p 1200 1408 100 0 0 INCP:No
p 944 1248 100 0 1 OOPT:On Change
p 944 1566 100 0 1 SCAN:I/O Intr
p 544 1568 100 0 -1 def(INAN):$(dc)$(seqcommand)C.VAL 
p 1072 1072 100 1024 1 name:$(gm)$(seqcommand)DcWaitCar
use oslBorderC 320 87 100 0 oslBorderC#1
xform 0 2000 1392
p 3260 336 120 256 -1 Title:GMOS IS - combine subsystem CAR status
[comments]

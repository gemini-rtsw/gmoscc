[schematic2]
uniq 40
[tools]
[detail]
w -72 923 100 0 n#37 hwin.hwin#36.in -48 912 -48 912 egenSubB.getGratingNames.INPL
w -72 987 100 0 n#35 hwin.hwin#34.in -48 976 -48 976 egenSubB.getGratingNames.INPJ
w 2168 843 100 0 n#32 embbi.gratingSelect.FLNK 2128 1056 2160 1056 2160 832 2224 832 estringouts.gratNameOut.SLNK
w 2152 875 100 0 n#31 embbi.gratingSelect.VAL 2128 864 2224 864 estringouts.gratNameOut.DOL
w -280 539 100 0 SLNK inhier.SLNK.P -336 528 -176 528 -176 560 -48 560 egenSubB.getGratingNames.SLNK
s 528 1552 200 0 This gensub routine combine two
s 528 1488 200 0 lookup files and output a list of
s 528 1424 200 0 names into the MBBI state strings
[cell use]
use hwin -240 871 100 0 hwin#36
xform 0 -144 912
p -336 912 100 0 -1 val(in):$(top)grLUT.VAL
use hwin -240 935 100 0 hwin#34
xform 0 -144 976
p -352 976 100 0 -1 val(in):$(top)dirLUT.VAL
use ebos 336 295 100 0 grSelectDisplay
xform 0 464 384
p 400 256 100 0 1 OMSL:supervisory
p 400 192 100 0 1 ONAM:disabled
p 400 224 100 0 1 ZNAM:enabled
use estringouts 2224 759 100 0 gratNameOut
xform 0 2352 832
p 2288 720 100 0 1 OMSL:closed_loop
p 2432 784 100 0 -1 def(OUT):$(gm)grSelect.B
p 2480 816 75 768 -1 pproc(OUT):NPP
use inhier -352 487 100 0 SLNK
xform 0 -336 528
use egenSubB -48 471 100 0 getGratingNames
xform 0 96 896
p 32 1264 100 0 1 FTA:LONG
p 32 1232 100 0 1 FTB:LONG
p 32 1200 100 0 1 FTC:STRING
p 32 976 100 0 1 FTJ:STRING
p 32 912 100 0 1 FTL:STRING
p 640 1296 100 0 1 FTVA:STRING
p 640 1296 100 0 1 FTVB:STRING
p 640 1264 100 0 1 FTVC:STRING
p 640 1232 100 0 1 FTVD:STRING
p 640 1200 100 0 1 FTVE:STRING
p 640 1136 100 0 1 FTVF:STRING
p 640 1136 100 0 1 FTVG:STRING
p 640 1104 100 0 1 FTVH:STRING
p 640 1072 100 0 1 FTVI:STRING
p 640 1040 100 0 1 FTVJ:STRING
p 640 1008 100 0 1 FTVK:STRING
p 640 976 100 0 1 FTVL:STRING
p 640 944 100 0 1 FTVM:STRING
p 640 912 100 0 1 FTVN:STRING
p 640 880 100 0 1 FTVO:STRING
p 640 848 100 0 1 FTVP:STRING
p -80 432 100 0 1 SNAM:gmSeqGratingNames
p 304 1296 100 0 -1 def(OUTA):$(gm)gratingSelect.ZRST
p 336 1264 100 0 -1 def(OUTB):$(gm)gratingSelect.ONST
p 336 1232 100 0 -1 def(OUTC):$(gm)gratingSelect.TWST
p 336 1200 100 0 -1 def(OUTD):$(gm)gratingSelect.THST
p 336 1168 100 0 -1 def(OUTE):$(gm)gratingSelect.FRST
p 336 1136 100 0 -1 def(OUTF):$(gm)gratingSelect.FVST
p 336 1104 100 0 -1 def(OUTG):$(gm)gratingSelect.SXST
p 336 1072 100 0 -1 def(OUTH):$(gm)gratingSelect.SVST
p 336 1040 100 0 -1 def(OUTI):$(gm)gratingSelect.EIST
p 336 1008 100 0 -1 def(OUTJ):$(gm)gratingSelect.NIST
p 336 976 100 0 -1 def(OUTK):$(gm)gratingSelect.TEST
p 336 944 100 0 -1 def(OUTL):$(gm)gratingSelect.ELST
p 336 912 100 0 -1 def(OUTM):$(gm)gratingSelect.TVST
p 336 880 100 0 -1 def(OUTN):$(gm)gratingSelect.TTST
p 336 848 100 0 -1 def(OUTO):$(gm)gratingSelect.FTST
p 336 816 100 0 -1 def(OUTP):$(gm)gratingSelect.FFST
p 240 1274 75 0 -1 pproc(OUTA):NPP
use embbi 1104 423 100 0 gratingSelect
xform 0 1616 848
use oslBorderC -624 -425 100 0 oslBorderC#0
xform 0 1056 880
[comments]

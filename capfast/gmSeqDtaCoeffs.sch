[schematic2]
uniq 130
[tools]
[detail]
w 914 1035 100 0 n#129 egenSubD.flexCoeffs.FLNK 1664 1184 1760 1184 1760 1024 128 1024 128 256 1376 256 egenSubD.tempCoeffs.SLNK
w 1954 235 100 0 FLNK egenSubD.tempCoeffs.FLNK 1664 224 2304 224 outhier.FLNK.p
w 728 1219 100 0 SLNK inhier.SLNK.P 128 1216 1376 1216 egenSubD.flexCoeffs.SLNK
s 704 1280 100 0 U - Unused
s 704 1312 100 0 T - Unused
s 704 1344 100 0 S - Unused
s 704 1376 100 0 R - Z flexure thetaZ phase offset angle, PhiZZ
s 704 1408 100 0 Q - Z flexure thetaY phase offset angle, PhiYZ
s 704 1440 100 0 P - Y flexure thetaZ phase offset angle, PhiZY
s 704 1472 100 0 O - Y flexure thetaY phase offset angle, PhiYY
s 704 1504 100 0 N - X flexure thetaZ phase offset angle, PhiZX
s 704 1536 100 0 M - X flexure thetaY phase offset angle, PhiYX
s 704 1568 100 0 L - Grating reference angle, psi
s 704 1600 100 0 K - Grating anamorphic factor in focus, Hz
s 704 1632 100 0 J - Grating anamorphic factor in Y, Hy
s 704 1664 100 0 I - Second focus flexure coefficient, Bz
s 704 1696 100 0 H - Second Y flexure coefficient, By
s 704 1728 100 0 G - Second X flexure coefficient, Bx
s 704 1760 100 0 F - First focus flexure coefficient, Az
s 704 1792 100 0 E - First Y flexure coefficient, Ay
s 704 1824 100 0 D - First X flexure coefficient, Ax
s 704 1856 100 0 C - Zero point for Z flexure, Z0
s 704 1888 100 0 B - Zero point for Y flexure, Y0
s 256 1920 100 0 --------------------------------
s 256 1952 100 0 Flexure Control Coefficients
s 704 1920 100 0 A - Zero point for X flexure, X0
s 864 2160 200 0 models in the detector tracking system
s 864 2208 200 0 This schematic pulls in the coefficients for the 
s 2464 2304 100 0 $Id$
s 2768 112 200 0 GMOS Sequencer
s 2496 16 100 0 Chris Mayer
s 2768 64 100 0 Translation Model Coefficients
s 2496 128 100 0 Copyright
s 2496 96 100 0 Observatory
s 2496 64 100 0 Sciences Ltd.
s 704 2048 150 0 These coefficients are set using PvLoad on initialisation.
s 704 800 100 0 F - Second focus temperature coefficient, T2z
s 704 832 100 0 E - Second Y temperature coefficient, T2y
s 704 864 100 0 D - Second X temperature coefficient, T2x
s 704 896 100 0 C - First focus temperature coefficient, Tx
s 704 928 100 0 B - First Y temperature coefficient, Ty
s 704 960 100 0 A - First X temperature coefficient, Tx
s 256 960 100 0 --------------------------------
s 256 992 100 0 Temperature Control Coefficients
[cell use]
use egenSubD 1376 1127 100 0 flexCoeffs
xform 0 1520 1552
p 1219 1867 100 0 0 DESC:Access flexure model coeffs
p 1456 1920 100 0 1 FTA:DOUBLE
p 1456 1888 100 0 1 FTB:DOUBLE
p 1456 1856 100 0 1 FTC:DOUBLE
p 1456 1824 100 0 1 FTD:DOUBLE
p 1456 1792 100 0 1 FTE:DOUBLE
p 1456 1760 100 0 1 FTF:DOUBLE
p 1456 1728 100 0 1 FTG:DOUBLE
p 1456 1696 100 0 1 FTH:DOUBLE
p 1456 1664 100 0 1 FTI:DOUBLE
p 1456 1632 100 0 1 FTJ:DOUBLE
p 1456 1600 100 0 1 FTK:DOUBLE
p 1456 1568 100 0 1 FTL:DOUBLE
p 1456 1536 100 0 1 FTM:DOUBLE
p 1456 1504 100 0 1 FTN:DOUBLE
p 1456 1472 100 0 1 FTO:DOUBLE
p 1456 1440 100 0 1 FTP:DOUBLE
p 1456 1408 100 0 1 FTQ:DOUBLE
p 1456 1376 100 0 1 FTR:DOUBLE
p 1456 1344 100 0 1 FTS:DOUBLE
p 1456 1312 100 0 1 FTT:DOUBLE
p 1456 1280 100 0 1 FTU:DOUBLE
p 1440 1088 100 0 1 INAM:
p 1488 1168 100 0 1 PREC:4
p 1424 976 100 0 0 SCAN:Passive
p 1440 1056 100 0 1 SNAM:gmSeqDtaFlexCoeffs
use egenSubD 1376 167 100 0 tempCoeffs
xform 0 1520 592
p 1219 907 100 0 0 DESC:Access temperature model coeffs
p 1456 960 100 0 1 FTA:DOUBLE
p 1456 928 100 0 1 FTB:DOUBLE
p 1456 896 100 0 1 FTC:DOUBLE
p 1456 864 100 0 1 FTD:DOUBLE
p 1456 832 100 0 1 FTE:DOUBLE
p 1456 800 100 0 1 FTF:DOUBLE
p 1456 768 100 0 1 FTG:DOUBLE
p 1456 736 100 0 1 FTH:DOUBLE
p 1456 704 100 0 1 FTI:DOUBLE
p 1456 672 100 0 1 FTJ:DOUBLE
p 1456 640 100 0 1 FTK:DOUBLE
p 1456 608 100 0 1 FTL:DOUBLE
p 1456 576 100 0 1 FTM:DOUBLE
p 1456 544 100 0 1 FTN:DOUBLE
p 1456 512 100 0 1 FTO:DOUBLE
p 1456 480 100 0 1 FTP:DOUBLE
p 1456 448 100 0 1 FTQ:DOUBLE
p 1456 416 100 0 1 FTR:DOUBLE
p 1456 384 100 0 1 FTS:DOUBLE
p 1456 352 100 0 1 FTT:DOUBLE
p 1456 320 100 0 1 FTU:DOUBLE
p 1440 128 100 0 1 INAM:
p 1488 208 100 0 1 PREC:4
p 1424 16 100 0 0 SCAN:Passive
p 1440 96 100 0 1 SNAM:gmSeqDtaTempCoeffs
use inhier 112 1175 100 0 SLNK
xform 0 128 1216
use outhier 2272 183 100 0 FLNK
xform 0 2288 224
use bc200tr -96 -152 -100 0 frame
xform 0 1584 1152
[comments]

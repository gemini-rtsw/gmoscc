[schematic2]
uniq 345
[tools]
[detail]
w 3420 2539 100 0 n#343 ecad8.update.MLNK 3040 2048 3424 2048 3424 3040 3680 3040 elongouts.updateFwdMsk.SLNK
w 3250 2499 100 0 n#342 ecad8.update.VALC 3040 2496 3520 2496 3520 2560 3680 2560 elongouts.updateFwdGr.DOL
w 3234 2563 100 0 n#341 ecad8.update.VALB 3040 2560 3488 2560 3488 2816 3680 2816 elongouts.updateFwdFl.DOL
w 3452 2843 100 0 n#340 ecad8.update.VALA 3040 2624 3456 2624 3456 3072 3680 3072 elongouts.updateFwdMsk.DOL
w 3922 2755 100 0 n#331 elongouts.updateFwdFl.OUT 3936 2752 3968 2752 hwout.hwout#338.outp
w 3922 2507 100 0 n#329 elongouts.updateFwdGr.OUT 3936 2496 3968 2496 hwout.hwout#337.outp
w 3922 3019 100 0 n#328 elongouts.updateFwdMsk.OUT 3936 3008 3968 3008 hwout.hwout#339.outp
w 4002 2891 100 0 n#327 elongouts.updateFwdMsk.FLNK 3936 3072 4480 3072 4480 2880 3584 2880 3584 2784 3680 2784 elongouts.updateFwdFl.SLNK
w 4002 2635 100 0 n#326 elongouts.updateFwdFl.FLNK 3936 2816 4480 2816 4480 2624 3584 2624 3584 2528 3680 2528 elongouts.updateFwdGr.SLNK
w 4770 1483 100 0 OERR ecars.updateC.OERR 4640 1472 4960 1472 4960 1440 5088 1440 outhier.OERR.p
w 4770 1515 100 0 OMSS ecars.updateC.OMSS 4640 1504 4960 1504 4960 1536 5088 1536 outhier.OMSS.p
w 2690 1739 100 0 n#241 ecad8.update.STLK 3040 1952 3232 1952 3232 1728 2208 1728 2208 1088 2688 1088 eseq.updateSeq.SLNK
w 3464 1387 100 0 n#228 eseq.updateSeq.LNK2 3264 1376 3712 1376 3712 1408 junction
w 3464 1419 100 0 n#228 eseq.updateSeq.LNK1 3264 1408 3712 1408 3712 1568 3648 1568 elongins.updateEvent.VAL
w 4840 1347 100 0 c#236 ecars.updateC.FLNK 4640 1344 5088 1344 outhier.CFLK.p
w 4648 1635 100 0 CAR ecars.updateC.IVAL 4320 1568 4256 1568 4256 1632 5088 1632 outhier.CAR.p
w 2568 1387 100 0 n#225 hwin.hwin#233.in 2496 1376 2688 1376 eseq.updateSeq.DOL2
w 2564 1467 100 0 n#224 hwin.hwin#234.in 2496 1536 2560 1536 2560 1408 2688 1408 eseq.updateSeq.DOL1
w 3496 1091 100 0 FLNK eseq.updateSeq.FLNK 3264 1088 3776 1088 outhier.FLNK.p
w 2386 2251 100 0 INPB inhier.INPB.P 2240 2240 2592 2240 2592 2528 2720 2528 ecad8.update.INPB
w 2370 2379 100 0 B inhier.B.P 2240 2368 2560 2368 2560 2560 2720 2560 ecad8.update.B
w 3112 2787 100 0 MESS ecad8.update.MESS 3040 2784 3232 2784 3232 2976 3360 2976 outhier.MESS.p
w 2450 2499 100 0 INPA inhier.INPA.P 2240 2496 2528 2496 2528 2592 2720 2592 ecad8.update.INPA
w 2462 2627 100 0 A inhier.A.P 2240 2624 2720 2624 ecad8.update.A
w 3196 2955 100 0 VAL ecad8.update.VAL 3040 2816 3200 2816 3200 3104 3360 3104 outhier.VAL.p
w 2462 2819 100 0 DIR inhier.DIR.P 2240 2816 2720 2816 ecad8.update.DIR
s 2256 2976 150 0 are capable of executing the UPDATE command.
s 2256 3040 150 0 NOTE: Only the mask, filter and grating assemblies
s 2304 1584 100 0 CAR_BUSY = 2
s 2304 1424 100 0 CAR_IDLE = 0
s 3856 1376 100 0 to the assemC CAR record
s 3856 1424 100 0 This SNL also connects
s 3664 2256 100 0 CAD RECORD FEATURE WORK AROUND
s 3664 2208 100 0 The CAD record activates its outputs regardless
s 3664 2176 100 0 of the directive received, so intermediate
s 3664 2144 100 0 longout records need to be used.
[cell use]
use hwout 3968 2455 100 0 hwout#337
xform 0 4064 2496
p 4064 2487 100 0 -1 val(outp):$(top)$(c2)$(grating)Assembly.MODE
use hwout 3968 2711 100 0 hwout#338
xform 0 4064 2752
p 4064 2743 100 0 -1 val(outp):$(top)$(c2)$(filter)Assembly.MODE
use hwout 3968 2967 100 0 hwout#339
xform 0 4064 3008
p 4064 2999 100 0 -1 val(outp):$(top)$(c1)$(mask)Assembly.MODE
use elongouts 3680 2951 100 0 updateFwdMsk
xform 0 3808 3040
p 3744 2912 100 0 1 OMSL:closed_loop
p 3744 2880 100 0 1 PV:$(top)cc:
use elongouts 3680 2695 100 0 updateFwdFl
xform 0 3808 2784
p 3744 2656 100 0 1 OMSL:closed_loop
p 3744 2624 100 0 1 PV:$(top)cc:
use elongouts 3680 2439 100 0 updateFwdGr
xform 0 3808 2528
p 3744 2400 100 0 1 OMSL:closed_loop
p 3744 2368 100 0 1 PV:$(top)cc:
use ukatcBorderC 2048 711 100 0 ukatcBorderC#296
xform 0 3728 2016
p 3840 848 100 0 -1 ID:$Id$
p 4996 1024 120 256 -1 Project:Gemini Multi-object Optical Spectrograph
p 3088 848 150 0 -1 Rev:$Revision$
p 4988 960 120 256 -1 Title:CAD/CAR records for global Update command
p 4704 896 100 768 -1 author:$Author$
p 4704 864 100 768 -1 date:$Date$
use hwin 2304 1495 100 0 hwin#234
xform 0 2400 1536
p 2307 1528 100 0 -1 val(in):2
use hwin 2304 1335 100 0 hwin#233
xform 0 2400 1376
p 2307 1368 100 0 -1 val(in):0
use outhier 5056 1591 100 0 CAR
xform 0 5072 1632
use outhier 5056 1303 100 0 CFLK
xform 0 5072 1344
use outhier 3744 1047 100 0 FLNK
xform 0 3760 1088
use outhier 3328 2935 100 0 MESS
xform 0 3344 2976
use outhier 3328 3063 100 0 VAL
xform 0 3344 3104
use outhier 5056 1495 100 0 OMSS
xform 0 5072 1536
use outhier 5056 1399 100 0 OERR
xform 0 5072 1440
use snlLink 3680 1495 100 0 snlLink#240
xform 0 3984 1568
p 3824 1488 100 0 1 seta:snlfile systemCar.stpp
use elongins 3392 1511 100 0 updateEvent
xform 0 3520 1584
p 3136 1582 100 0 0 EGU:CAR event
p 3456 1472 100 0 1 PV:$(top)cc:
use eseq 2688 999 100 0 updateSeq
xform 0 2976 1296
p 3008 1406 100 0 1 DLY2:1.0
p 2752 944 100 0 1 PV:$(top)cc:
p 3264 1408 75 768 -1 pproc(LNK1):PP
p 3264 1376 75 768 -1 pproc(LNK2):PP
use inhier 2224 2199 100 0 INPB
xform 0 2240 2240
use inhier 2224 2327 100 0 B
xform 0 2240 2368
use inhier 2224 2455 100 0 INPA
xform 0 2240 2496
use inhier 2224 2583 100 0 A
xform 0 2240 2624
use inhier 2224 2775 100 0 DIR
xform 0 2240 2816
use ecars 4320 1287 100 0 updateC
xform 0 4480 1456
p 4384 1248 100 0 1 DESC:CAR record for update command
p 4384 1216 100 0 1 PV:$(top)cc:
use ecad8 2720 1863 100 0 update
xform 0 2880 2368
p 2784 1840 100 0 1 DESC:Update command
p 2816 2560 100 0 1 FTVA:LONG
p 2816 2528 100 0 1 FTVB:LONG
p 2816 2496 100 0 1 FTVC:LONG
p 2816 2464 100 0 1 FTVD:LONG
p 2816 2432 100 0 1 FTVE:LONG
p 2816 2400 100 0 1 FTVF:LONG
p 2816 2368 100 0 1 FTVG:LONG
p 2816 2336 100 0 1 FTVH:LONG
p 2784 1808 100 0 1 INAM:gmosCadModeInit
p 2784 1744 100 0 1 PV:$(top)cc:
p 2784 1776 100 0 1 SNAM:gmosCadUpdate
[comments]

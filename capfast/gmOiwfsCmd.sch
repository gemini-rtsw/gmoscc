[schematic2]
uniq 204
[tools]
[detail]
w 450 2603 100 0 n#202 hwin.hwin#203.in 448 2592 512 2592 ecad8.Cad.INPG
w 1522 2827 100 0 n#201 elongouts.Dir.SLNK 1408 2688 1376 2688 1376 2816 1728 2816 1728 3008 1664 3008 elongouts.Mode.FLNK
w 1522 2539 100 0 n#200 elongouts.FollowMode.SLNK 1408 2400 1376 2400 1376 2528 1728 2528 1728 2720 1664 2720 elongouts.Dir.FLNK
w 1412 2427 100 2 n#199 hwin.hwin#198.in 1408 2432 1408 2432 elongouts.FollowMode.DOL
w 956 2891 100 0 follow_enable ecad8.Cad.VALC 832 2880 1152 2880 1152 2368 1408 2368 elongouts.FollowMode.SDIS
w 1026 2955 100 0 directive ecad8.Cad.VALB 832 2944 1280 2944 1280 2720 1408 2720 elongouts.Dir.DOL
w 1090 3019 100 0 mode ecad8.Cad.VALA 832 3008 1408 3008 elongouts.Mode.DOL
w 450 2667 100 0 n#184 hwin.hwin#185.in 448 2656 512 2656 ecad8.Cad.INPF
w 450 2731 100 0 n#183 hwin.hwin#182.in 448 2720 512 2720 ecad8.Cad.INPE
w 450 2795 100 0 n#180 hwin.hwin#181.in 448 2784 512 2784 ecad8.Cad.INPD
w 450 2859 100 0 n#176 hwin.hwin#175.in 448 2848 512 2848 ecad8.Cad.INPC
w 1010 2347 100 0 n#173 ecad8.Cad.STLK 832 2336 1248 2336 1248 2976 1408 2976 elongouts.Mode.SLNK
w 1180 2923 100 0 n#171 ecad8.Cad.FLNK 832 2464 1184 2464 1184 3392 1408 3392 estringouts.Mess.SLNK
w 1668 3371 100 2 n#159 hwout.hwout#151.outp 1664 3376 1664 3376 estringouts.Mess.OUT
w 962 3171 100 0 n#150 estringouts.Mess.DOL 1408 3424 1152 3424 1152 3168 832 3168 ecad8.Cad.MESS
w 1786 2667 100 0 AssemblyDirective elongouts.Dir.OUT 1664 2656 2112 2656 outhier.DIR.p
w 1858 2955 100 0 AssemblyMode elongouts.Mode.OUT 1664 2944 2112 2944 outhier.MODE.p
w 46 2923 100 0 FollowState inhier.FollowState.P -288 2912 512 2912 ecad8.Cad.INPB
w 46 2987 100 0 ActiveState inhier.ActiveState.P -288 2976 512 2976 ecad8.Cad.INPA
[cell use]
use hwin 256 2551 100 0 hwin#203
xform 0 352 2592
p 240 2592 100 512 -1 val(in):$(gm)cc:masterEnable.VAL
use hwin 256 2807 100 0 hwin#175
xform 0 352 2848
p 240 2848 100 512 -1 val(in):$(top)probeAssembly.INIT
use hwin 256 2743 100 0 hwin#181
xform 0 352 2784
p 240 2784 100 512 -1 val(in):$(top)probeAssembly.INDX
use hwin 256 2679 100 0 hwin#182
xform 0 352 2720
p 240 2720 100 512 -1 val(in):$(top)probeAssembly.ILCK
use hwin 256 2615 100 0 hwin#185
xform 0 352 2656
p 240 2656 100 512 -1 val(in):$(top)probeAssembly.ASTA
use hwin 1216 2391 100 0 hwin#198
xform 0 1312 2432
p 1219 2424 100 0 -1 val(in):0
use elongouts 1584 3056 100 0 Mode
xform 0 1536 2976
p 1456 2880 100 0 1 OMSL:closed_loop
p 1584 3056 100 512 -1 PV:$(top)$(dev)
use elongouts 1568 2768 100 0 Dir
xform 0 1536 2688
p 1456 2576 100 0 1 OMSL:closed_loop
p 1568 2768 100 512 -1 PV:$(top)$(dev)
p 1664 2656 75 768 -1 pproc(OUT):PP
use elongouts 1568 2480 100 0 FollowMode
xform 0 1536 2400
p 1488 2368 70 0 1 DISV:-1
p 1456 2304 100 0 1 OMSL:closed_loop
p 1568 2480 100 512 -1 PV:$(top)$(dev)
p 1744 2368 100 0 1 def(OUT):$(top)followState
p 1664 2368 75 768 -1 pproc(OUT):PP
use ecad8 784 3248 100 0 Cad
xform 0 672 2752
p 704 2976 100 0 -1 FTVA:LONG
p 704 2912 100 0 -1 FTVB:LONG
p 752 2848 100 512 -1 FTVC:LONG
p 576 2224 100 0 1 INAM:
p 576 2192 100 0 1 SNAM:oiwfs$(dev)Cad
p 768 3248 100 512 -1 name:$(top)$(dev)
p 832 2346 75 0 -1 pproc(STLK):PP
use gmosBorderC -768 1527 100 0 gmosBorderC#164
xform 0 912 2832
p 2484 1656 100 512 1 File:gmOiwfsCmd.sch
p 1892 1700 150 0 1 Rev:1.0
p 2172 1776 120 256 -1 Title:One TCS Command
p 2212 1712 100 1024 -1 author:A.Ebbers & B.Wooff
p 2212 1680 100 1024 -1 date:October 2, 2000
use hwout 1664 3335 100 0 hwout#151
xform 0 1760 3376
p 1872 3376 100 0 -1 val(outp):$(top)rejectMess.IMSS NPP NMS
use estringouts 1584 3456 100 0 Mess
xform 0 1536 3392
p 1440 3312 100 0 1 OMSL:closed_loop
p 1584 3456 100 512 -1 PV:$(top)$(dev)
p 1664 3376 75 768 -1 pproc(OUT):NPP
use outhier 2080 2615 100 0 DIR
xform 0 2096 2656
use outhier 2080 2903 100 0 MODE
xform 0 2096 2944
use inhier -400 3008 100 0 ActiveState
xform 0 -288 2976
use inhier -400 2864 100 0 FollowState
xform 0 -288 2912
[comments]

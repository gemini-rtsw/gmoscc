[schematic2]
uniq 532
[tools]
[detail]
w 4132 -165 100 2 n#523 hwout.hwout#524.outp 4128 -160 4128 -160 ebos.ap6b7.OUT
w 4132 187 100 2 n#520 hwout.hwout#521.outp 4128 192 4128 192 ebos.ap6b6.OUT
w 3556 27 100 2 n#519 ebos.ap6b2.OUT 3552 32 3552 32 hwout.hwout#518.outp
w 4132 1243 100 2 n#516 ebos.ap6b3.OUT 4128 1248 4128 1248 hwout.hwout#515.outp
w 4132 891 100 2 n#509 hwout.hwout#510.outp 4128 896 4128 896 ebos.ap6b4.OUT
w 4132 539 100 2 n#508 hwout.hwout#511.outp 4128 544 4128 544 ebos.ap6b5.OUT
w 3556 379 100 2 n#507 ebos.ap6b1.OUT 3552 384 3552 384 hwout.hwout#504.outp
w 3556 731 100 2 n#506 ebos.ap6b0.OUT 3552 736 3552 736 hwout.hwout#505.outp
w 3556 1083 100 2 n#499 hwout.hwout#500.outp 3552 1088 3552 1088 ebos.ap5b5.OUT
w 2980 -165 100 2 n#496 hwout.hwout#497.outp 2976 -160 2976 -160 ebos.ap5b4.OUT
w 2980 187 100 2 n#495 hwout.hwout#492.outp 2976 192 2976 192 ebos.ap5b3.OUT
w 2980 539 100 2 n#494 hwout.hwout#493.outp 2976 544 2976 544 ebos.ap5b2.OUT
w 2980 891 100 2 n#487 hwout.hwout#488.outp 2976 896 2976 896 ebos.ap5b1.OUT
w 2980 1243 100 2 n#484 hwout.hwout#485.outp 2976 1248 2976 1248 ebos.ap5b0.OUT
s 3696 1648 300 0 CARD 0
s 2912 1520 200 0 This schematic contains records connected to XYCOM outputs that are not used anywhere else in the database.
[cell use]
use ebos 4048 -48 100 0 ap6b7
xform 0 4000 -128
p 3936 -224 100 0 1 DTYP:$(xycom)
p 3936 -288 100 0 1 OMSL:supervisory
p 3936 -320 100 0 1 ONAM:ON
p 4048 -48 100 512 1 PV:$(top)$(cc)
p 3936 -256 100 0 1 SCAN:Passive
p 3936 -352 100 0 1 ZNAM:OFF
use hwout 4128 -201 100 0 hwout#524
xform 0 4224 -160
p 4336 -160 100 0 -1 val(outp):#<$(ap6b7)>
use ebos 4048 304 100 0 ap6b6
xform 0 4000 224
p 3935 127 100 0 1 DTYP:$(xycom)
p 3936 64 100 0 1 OMSL:supervisory
p 3936 32 100 0 1 ONAM:ON
p 4048 304 100 512 1 PV:$(top)$(cc)
p 3936 96 100 0 1 SCAN:Passive
p 3936 0 100 0 1 ZNAM:OFF
use hwout 4128 151 100 0 hwout#521
xform 0 4224 192
p 4336 192 100 0 -1 val(outp):#<$(ap6b6)>
use hwout 3552 -9 100 0 hwout#518
xform 0 3648 32
p 3760 32 100 0 -1 val(outp):#<$(ap6b2)>
use ebos 3472 144 100 0 ap6b2
xform 0 3424 64
p 3359 -33 100 0 1 DTYP:$(xycom)
p 3360 -96 100 0 1 OMSL:supervisory
p 3360 -128 100 0 1 ONAM:ON
p 3472 144 100 512 1 PV:$(top)$(cc)
p 3360 -64 100 0 1 SCAN:Passive
p 3360 -160 100 0 1 ZNAM:OFF
use hwout 4128 1207 100 0 hwout#515
xform 0 4224 1248
p 4336 1248 100 0 -1 val(outp):#<$(ap6b3)>
use ebos 4048 1360 100 0 ap6b3
xform 0 4000 1280
p 3935 1183 100 0 1 DTYP:$(xycom)
p 3936 1120 100 0 1 OMSL:supervisory
p 3936 1088 100 0 1 ONAM:ON
p 4048 1360 100 512 1 PV:$(top)$(cc)
p 3936 1152 100 0 1 SCAN:Passive
p 3936 1056 100 0 1 ZNAM:OFF
use ebos 4048 1008 100 0 ap6b4
xform 0 4000 928
p 3935 831 100 0 1 DTYP:$(xycom)
p 3936 768 100 0 1 OMSL:supervisory
p 3936 736 100 0 1 ONAM:ON
p 4048 1008 100 512 1 PV:$(top)$(cc)
p 3936 800 100 0 1 SCAN:Passive
p 3936 704 100 0 1 ZNAM:OFF
use ebos 4048 656 100 0 ap6b5
xform 0 4000 576
p 3936 480 100 0 1 DTYP:$(xycom)
p 3936 416 100 0 1 OMSL:supervisory
p 3936 384 100 0 1 ONAM:ON
p 4048 656 100 512 1 PV:$(top)$(cc)
p 3936 448 100 0 1 SCAN:Passive
p 3936 352 100 0 1 ZNAM:OFF
use hwout 4128 503 100 0 hwout#511
xform 0 4224 544
p 4336 544 100 0 -1 val(outp):#<$(ap6b5)>
use hwout 4128 855 100 0 hwout#510
xform 0 4224 896
p 4336 896 100 0 -1 val(outp):#<$(ap6b4)>
use hwout 3552 695 100 0 hwout#505
xform 0 3648 736
p 3760 736 100 0 -1 val(outp):#<$(ap6b0)>
use hwout 3552 343 100 0 hwout#504
xform 0 3648 384
p 3760 384 100 0 -1 val(outp):#<$(ap6b1)>
use ebos 3472 496 100 0 ap6b1
xform 0 3424 416
p 3359 319 100 0 1 DTYP:$(xycom)
p 3360 256 100 0 1 OMSL:supervisory
p 3360 224 100 0 1 ONAM:ON
p 3456 496 100 512 1 PV:$(top)$(cc)
p 3360 288 100 0 1 SCAN:Passive
p 3360 192 100 0 1 ZNAM:OFF
use ebos 3472 848 100 0 ap6b0
xform 0 3424 768
p 3359 671 100 0 1 DTYP:$(xycom)
p 3360 608 100 0 1 OMSL:supervisory
p 3360 576 100 0 1 ONAM:ON
p 3472 848 100 512 1 PV:$(top)$(cc)
p 3360 640 100 0 1 SCAN:Passive
p 3360 544 100 0 1 ZNAM:OFF
use ebos 3472 1200 100 0 ap5b5
xform 0 3424 1120
p 3360 1024 100 0 1 DTYP:$(xycom)
p 3360 960 100 0 1 OMSL:supervisory
p 3360 928 100 0 1 ONAM:ON
p 3472 1200 100 512 1 PV:$(top)$(cc)
p 3360 992 100 0 1 SCAN:Passive
p 3360 896 100 0 1 ZNAM:OFF
use hwout 3552 1047 100 0 hwout#500
xform 0 3648 1088
p 3760 1088 100 0 -1 val(outp):#<$(ap5b5)>
use ebos 2896 -48 100 0 ap5b4
xform 0 2848 -128
p 2783 -225 100 0 1 DTYP:$(xycom)
p 2784 -288 100 0 1 OMSL:supervisory
p 2784 -320 100 0 1 ONAM:ON
p 2896 -48 100 512 1 PV:$(top)$(cc)
p 2784 -256 100 0 1 SCAN:Passive
p 2784 -352 100 0 1 ZNAM:OFF
use hwout 2976 -201 100 0 hwout#497
xform 0 3072 -160
p 3184 -160 100 0 -1 val(outp):#<$(ap5b4)>
use hwout 2976 503 100 0 hwout#493
xform 0 3072 544
p 3184 544 100 0 -1 val(outp):#<$(ap5b2)>
use hwout 2976 151 100 0 hwout#492
xform 0 3072 192
p 3184 192 100 0 -1 val(outp):#<$(ap5b3)>
use ebos 2896 304 100 0 ap5b3
xform 0 2848 224
p 2784 128 100 0 1 DTYP:$(xycom)
p 2784 64 100 0 1 OMSL:supervisory
p 2784 32 100 0 1 ONAM:ON
p 2880 304 100 512 1 PV:$(top)$(cc)
p 2784 96 100 0 1 SCAN:Passive
p 2784 0 100 0 1 ZNAM:OFF
use ebos 2896 656 100 0 ap5b2
xform 0 2848 576
p 2784 480 100 0 1 DTYP:$(xycom)
p 2784 416 100 0 1 OMSL:supervisory
p 2784 384 100 0 1 ONAM:ON
p 2896 656 100 512 1 PV:$(top)$(cc)
p 2784 448 100 0 1 SCAN:Passive
p 2784 352 100 0 1 ZNAM:OFF
use ebos 2896 1008 100 0 ap5b1
xform 0 2848 928
p 2783 831 100 0 1 DTYP:$(xycom)
p 2784 768 100 0 1 OMSL:supervisory
p 2784 736 100 0 1 ONAM:ON
p 2896 1008 100 512 1 PV:$(top)$(cc)
p 2784 800 100 0 1 SCAN:Passive
p 2784 704 100 0 1 ZNAM:OFF
use hwout 2976 855 100 0 hwout#488
xform 0 3072 896
p 3184 896 100 0 -1 val(outp):#<$(ap5b1)>
use ebos 2896 1360 100 0 ap5b0
xform 0 2848 1280
p 2783 1183 100 0 1 DTYP:$(xycom)
p 2784 1120 100 0 1 OMSL:supervisory
p 2784 1088 100 0 1 ONAM:ON
p 2896 1360 100 512 1 PV:$(top)$(cc)
p 2784 1152 100 0 1 SCAN:Passive
p 2784 1056 100 0 1 ZNAM:OFF
use hwout 2976 1207 100 0 hwout#485
xform 0 3072 1248
p 3184 1248 100 0 -1 val(outp):#<$(ap5b0)>
use ukatcBorderC 2416 -713 100 0 ukatcBorderC#473
xform 0 4096 592
p 5356 -464 120 256 -1 Title:XYCOM unassigned outputs
p 5072 -528 100 768 -1 author:S M Beard and S Prior
[comments]

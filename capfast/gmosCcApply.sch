[schematic2]
uniq 176
[tools]
[detail]
w 1130 1227 100 0 n#175 ecad8.ecad8#141.STLK 1064 1216 1256 1216 hwout.hwout#174.outp
w 646 1611 100 0 n#173 hwin.hwin#172.in 608 1600 744 1600 ecad8.ecad8#141.INPE
w 646 1675 100 0 n#171 hwin.hwin#170.in 608 1664 744 1664 ecad8.ecad8#141.INPD
w 646 1739 100 0 n#169 hwin.hwin#168.in 608 1728 744 1728 ecad8.ecad8#141.INPC
w 646 1803 100 0 n#167 hwin.hwin#166.in 608 1792 744 1792 ecad8.ecad8#141.INPB
w 646 1867 100 0 n#165 hwin.hwin#164.in 608 1856 744 1856 ecad8.ecad8#141.INPA
w -702 1707 100 0 INMC inhier.INMC.P -768 1696 -576 1696 -576 1920 -448 1920 eapply.apply.INMC
w -718 1803 100 0 INPC inhier.INPC.P -768 1792 -608 1792 -608 1952 -448 1952 eapply.apply.INPC
w -574 1995 100 0 INMB inhier.INMB.P -768 1888 -640 1888 -640 1984 -448 1984 eapply.apply.INMB
w -590 2027 100 0 INPB inhier.INPB.P -768 1984 -672 1984 -672 2016 -448 2016 eapply.apply.INPB
w -590 2155 100 0 CLID inhier.CLID.P -768 2080 -672 2080 -672 2144 -448 2144 eapply.apply.CLID
w 2 2219 100 0 FLNK eapply.apply.FLNK -64 2208 128 2208 outhier.FLNK.p
w 2 1963 100 0 OUTC eapply.apply.OUTC -64 1952 128 1952 outhier.OUTC.p
w 2 2027 100 0 OUTB eapply.apply.OUTB -64 2016 128 2016 outhier.OUTB.p
w 258 2443 100 0 n#144 ecad8.ecad8#141.MESS 1064 2048 1184 2048 1184 2432 -608 2432 -608 2048 -448 2048 eapply.apply.INMA
w 258 2411 100 0 n#143 ecad8.ecad8#141.VAL 1064 2080 1152 2080 1152 2400 -576 2400 -576 2080 -448 2080 eapply.apply.INPA
w 310 2091 100 0 n#142 eapply.apply.OUTA -64 2080 744 2080 ecad8.ecad8#141.DIR
w -626 2187 100 0 DIR inhier.DIR.P -768 2176 -448 2176 eapply.apply.DIR
[cell use]
use hwout 1256 1175 100 0 hwout#174
xform 0 1352 1216
p 1464 1216 100 0 -1 val(outp):$(gm)applyIdle.PROC
use hwin 416 1559 100 0 hwin#172
xform 0 512 1600
p 88 1592 100 0 -1 val(in):$(gm)CcMEnableMonChange.VAL
use hwin 416 1623 100 0 hwin#170
xform 0 512 1664
p 88 1656 100 0 -1 val(in):$(gm)DcReadoutMonChange.VAL
use hwin 416 1687 100 0 hwin#168
xform 0 512 1728
p 184 1720 100 0 -1 val(in):$(gm)dcDisabled.VAL
use hwin 416 1751 100 0 hwin#166
xform 0 512 1792
p 208 1784 100 0 -1 val(in):$(gm)observeC.VAL
use hwin 416 1815 100 0 hwin#164
xform 0 512 1856
p 192 1848 100 0 -1 val(in):$(gm)cc:applyC.VAL
use inhier -784 2135 100 0 DIR
xform 0 -768 2176
use inhier -784 2039 100 0 CLID
xform 0 -768 2080
use inhier -784 1943 100 0 INPB
xform 0 -768 1984
use inhier -784 1847 100 0 INMB
xform 0 -768 1888
use inhier -784 1751 100 0 INPC
xform 0 -768 1792
use inhier -784 1655 100 0 INMC
xform 0 -768 1696
use outhier 96 1975 100 0 OUTB
xform 0 112 2016
use outhier 96 1911 100 0 OUTC
xform 0 112 1952
use outhier 96 2167 100 0 FLNK
xform 0 112 2208
use ecad8 744 1127 100 0 ecad8#141
xform 0 904 1632
p 848 1992 100 0 1 FTVA:LONG
p 816 1952 100 0 1 MFLG:TWO STATES
p 800 2152 100 0 1 SNAM:gmSeqConfigEnd
p 856 1120 100 1024 -1 name:$(top)configEnd
use eapply -192 2256 100 0 apply
xform 0 -256 1904
p -368 2256 100 0 1 PV:$(top)$(cc)
use ukatcBorderC -1344 167 100 0 ukatcBorderC#0
xform 0 336 1472
p 448 304 100 0 -1 ID:$Id$
p -304 304 150 0 -1 Rev:$Revision$
p 1596 416 120 256 -1 Title:Components Controller Database
p 1312 352 100 768 -1 author:S M Beard
p 1312 320 100 768 -1 date:$Date$
[comments]

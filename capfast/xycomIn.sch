[schematic2]
uniq 433
[tools]
[detail]
w 3938 571 100 0 n#421 hwin.hwin#426.in 3968 560 3968 560 ebis.bp1b7.INP
w 3938 891 100 0 n#420 hwin.hwin#427.in 3968 880 3968 880 ebis.bp0b7.INP
s 3952 1024 300 0 CARD 1
s 2928 1056 300 0 CARD 0
s 2720 1262 200 0 This schematic contains records connected to XYCOM inputs that are not used anywhere else in the database.
[cell use]
use ebis 4160 592 100 0 bp1b7
xform 0 4096 528
p 3795 603 100 0 0 DESC:TTL switch input
p 4032 432 100 0 1 DTYP:$(xycom)
p 4032 336 100 0 1 ONAM:ON
p 3968 592 100 0 1 PV:$(top)$(cc)
p 4032 400 100 0 1 SCAN:$(iointr)
p 4032 368 100 0 1 ZNAM:OFF
use ebis 4160 912 100 0 bp0b7
xform 0 4096 848
p 3795 923 100 0 0 DESC:TTL switch input
p 4032 752 100 0 1 DTYP:$(xycom)
p 4032 656 100 0 1 ONAM:ON
p 3968 912 100 0 1 PV:$(top)$(cc)
p 4032 720 100 0 1 SCAN:$(iointr)
p 4032 688 100 0 1 ZNAM:OFF
use hwin 3776 519 100 0 hwin#426
xform 0 3872 560
p 3779 552 100 0 -1 val(in):#<$(bp1b7)>
use hwin 3776 839 100 0 hwin#427
xform 0 3872 880
p 3779 872 100 0 -1 val(in):#<$(bp0b7)>
use ukatcBorderC 2336 -889 100 0 ukatcBorderC#419
xform 0 4016 416
p 5276 -640 120 256 -1 Title:XYCOM unassigned inputs
p 4992 -704 100 768 -1 author:S M Beard and S Prior
[comments]

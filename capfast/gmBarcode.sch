[schematic2]
uniq 67
[tools]
[detail]
w 600 1043 100 0 VAL ebarcode.Barcode.VAL 512 1040 736 1040 736 1104 864 1104 estringouts.BarcodeString.DOL
w 1166 835 100 0 VAL junction 736 1040 736 832 1632 832 outhier.VAL.p
w 184 1099 100 0 n#63 hwin.channel.in 160 1088 256 1088 ebarcode.Barcode.INP
w 1358 1059 100 0 OUT estringouts.BarcodeString.OUT 1120 1056 1632 1056 outhier.OUT.p
w 664 1075 100 0 n#60 ebarcode.Barcode.FLNK 512 1072 864 1072 estringouts.BarcodeString.SLNK
w -96 1008 100 0 SLNK inhier.SLNK.P -208 1024 160 1024 160 1056 256 1056 ebarcode.Barcode.SLNK
s -256 1936 500 0 GMOS Barcode Reader Records
s 816 1248 100 0 This second record is needed to provide an OUT link
[cell use]
use outhier 1600 1015 100 0 OUT
xform 0 1616 1056
use outhier 1600 791 100 0 VAL
xform 0 1616 832
use gmosBorderC -1024 -185 100 0 gmosBorderC#64
xform 0 656 1120
p 2228 -56 100 512 1 File:$filename$
p 1636 -12 150 0 1 Rev:$Revision$
p 1916 64 120 256 -1 Title:GMOS barcode reader
p 1956 0 100 1024 -1 author:Angelic Ebbers and Steven Beard
use ebarcode 560 1120 100 0 Barcode
xform 0 384 1056
p 320 976 100 0 1 DTYP:barcodeScanner
p 320 1120 100 0 -1 PV:$(top)$(dev)$(name)
p 320 944 100 0 1 SCAN:Passive
p 336 1056 100 0 -1 Type:barcode
p 368 976 100 1024 0 name:$(top)$(I)
use estringouts 1168 1136 100 0 BarcodeString
xform 0 992 1072
p 928 992 100 0 1 OMSL:closed_loop
p 928 1136 100 0 -1 PV:$(top)$(dev)$(name)
p 880 1136 100 256 0 def(DOL):0
p 976 992 100 1024 0 name:$(top)$(I)
p 1200 1056 75 768 -1 palrm(OUT):NMS
p 1120 1056 75 768 -1 pproc(OUT):$(pproc)
use inhier -224 983 100 0 SLNK
xform 0 -208 1024
use hwin -32 1047 100 0 channel
xform 0 64 1088
p -304 1120 100 0 -1 val(in):@$(port_address),$(cos_channel)
[comments]

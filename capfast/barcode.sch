[schematic2]
uniq 65
[tools]
[detail]
w -136 1259 100 0 n#63 hwin.channel.in -160 1248 -64 1248 ebarcode.barcode.INP
w 878 1227 100 0 OUT estringouts.string_hier.OUT 800 1216 992 1216 outhier.OUT.p
w 280 1203 100 0 n#61 ebarcode.barcode.VAL 192 1200 416 1200 416 1264 544 1264 estringouts.string_hier.DOL
w 344 1235 100 0 n#60 ebarcode.barcode.FLNK 192 1232 544 1232 estringouts.string_hier.SLNK
w -416 1168 100 0 SLNK inhier.SLNK.P -528 1184 -160 1184 -160 1216 -64 1216 ebarcode.barcode.SLNK
s 608 928 100 0 Why is a chain of so many records needed???
s 608 1008 100 0 Actually, the link is ok because the next record in the chain is a stringout
s 608 1104 100 0 N.B. Is this link supposed to be PP?
s 608 1072 100 0 may cause assembly record to process twice, since it is already waiting on a timer.
[cell use]
use gmosBorderC -1024 -185 100 0 gmosBorderC#64
xform 0 656 1120
p 1916 64 120 256 -1 Title:GMOS barcode reader
p 1956 0 100 1024 -1 author:Angelic Ebbers
use ebarcode 320 1280 100 0 barcode
xform 0 64 1216
p 0 1136 100 0 1 DTYP:barcodeScanner
p 0 1280 100 0 -1 PV:$(top)$(dev)$(cos_channel)
p 16 1216 100 0 -1 Type:barcode
p 48 1136 100 1024 0 name:$(top)$(I)
use outhier 960 1175 100 0 OUT
xform 0 976 1216
use estringouts 928 1296 100 0 string_hier
xform 0 672 1232
p 608 1152 100 0 1 OMSL:closed_loop
p 608 1296 100 0 -1 PV:$(top)$(dev)$(cos_channel)
p 560 1296 100 256 0 def(DOL):0
p 656 1152 100 1024 0 name:$(top)$(I)
p 800 1216 75 768 -1 pproc(OUT):PP
use inhier -544 1143 100 0 SLNK
xform 0 -528 1184
use hwin -352 1207 100 0 channel
xform 0 -256 1248
p -624 1280 100 0 -1 val(in):@$(port_address),$(cos_channel)
[comments]

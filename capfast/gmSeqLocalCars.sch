[schematic2]
uniq 361
[tools]
[detail]
w -70 5691 100 0 n#338 elongouts.ObsCarDisconVal.OUT 96 4976 192 4976 192 5680 -272 5680 -272 5568 junction
w -366 5571 100 0 n#338 gmSeqObserveCar.gmSeqObserveCar#337.CVAL -560 5488 -448 5488 -448 5568 -224 5568 ecars.observeC.IVAL
w -198 5203 100 0 n#341 elongouts.ObsCarDisconVal.FLNK 96 5040 144 5040 144 5200 -480 5200 -480 5344 junction
w -486 5347 100 0 n#341 gmSeqObserveCar.gmSeqObserveCar#337.CFLK -560 5344 -352 5344 -352 5376 -224 5376 ecars.observeC.SLNK
w -254 4283 100 0 n#359 ecalcouts.waitForDcDisconnected.FLNK -464 4272 16 4272 16 4896 -304 4896 -304 5008 -160 5008 elongouts.ObsCarDisconVal.SLNK
w -269 5104 100 0 n#355 hwin.hwin#354.in -368 5136 -368 5040 -160 5040 elongouts.ObsCarDisconVal.DOL
w -502 5395 100 0 n#340 gmSeqObserveCar.gmSeqObserveCar#337.CERR -560 5392 -384 5392 -384 5472 -224 5472 ecars.observeC.IERR
w -350 5507 100 0 n#339 gmSeqObserveCar.gmSeqObserveCar#337.CMESS -560 5440 -416 5440 -416 5504 -224 5504 ecars.observeC.IMSS
w 754 4147 100 0 n#310 gmSeqSeqCommandCar.gmSeqSeqCommandCar#285.CFLK 640 4144 928 4144 ecars.parkC.SLNK
w 694 4195 100 0 n#309 gmSeqSeqCommandCar.gmSeqSeqCommandCar#285.CERR 640 4192 808 4192 808 4240 928 4240 ecars.parkC.IERR
w 822 4275 100 0 n#308 gmSeqSeqCommandCar.gmSeqSeqCommandCar#285.CMESS 640 4240 776 4240 776 4272 928 4272 ecars.parkC.IMSS
w 754 4579 100 0 n#307 gmSeqSeqCommandCar.gmSeqSeqCommandCar#283.CFLK 640 4576 928 4576 ecars.datumC.SLNK
w 686 4627 100 0 n#306 gmSeqSeqCommandCar.gmSeqSeqCommandCar#283.CERR 640 4624 792 4624 792 4672 928 4672 ecars.datumC.IERR
w 814 4707 100 0 n#305 gmSeqSeqCommandCar.gmSeqSeqCommandCar#283.CMESS 640 4672 760 4672 760 4704 928 4704 ecars.datumC.IMSS
w 754 4979 100 0 n#304 gmSeqSeqCommandCar.gmSeqSeqCommandCar#284.CFLK 640 4976 928 4976 ecars.initC.SLNK
w 694 5027 100 0 n#303 gmSeqSeqCommandCar.gmSeqSeqCommandCar#284.CERR 640 5024 808 5024 808 5072 928 5072 ecars.initC.IERR
w 822 5107 100 0 n#302 gmSeqSeqCommandCar.gmSeqSeqCommandCar#284.CMESS 640 5072 776 5072 776 5104 928 5104 ecars.initC.IMSS
w 754 5411 100 0 n#301 gmSeqSeqCommandCar.gmSeqSeqCommandCar#282.CFLK 640 5408 928 5408 ecars.testC.SLNK
w 686 5459 100 0 n#300 gmSeqSeqCommandCar.gmSeqSeqCommandCar#282.CERR 640 5456 792 5456 792 5504 928 5504 ecars.testC.IERR
w 814 5539 100 0 n#299 gmSeqSeqCommandCar.gmSeqSeqCommandCar#282.CMESS 640 5504 760 5504 760 5536 928 5536 ecars.testC.IMSS
w 782 4339 100 0 n#296 gmSeqSeqCommandCar.gmSeqSeqCommandCar#285.CVAL 640 4288 696 4288 696 4336 928 4336 ecars.parkC.IVAL
w 798 4771 100 0 n#294 gmSeqSeqCommandCar.gmSeqSeqCommandCar#283.CVAL 640 4720 728 4720 728 4768 928 4768 ecars.datumC.IVAL
w 806 5171 100 0 n#292 gmSeqSeqCommandCar.gmSeqSeqCommandCar#284.CVAL 640 5120 744 5120 744 5168 928 5168 ecars.initC.IVAL
w 790 5603 100 0 n#290 gmSeqSeqCommandCar.gmSeqSeqCommandCar#282.CVAL 640 5552 712 5552 712 5600 928 5600 ecars.testC.IVAL
s 1600 5968 140 0 gmSeqLocalCars.sch
[cell use]
use ecalcouts -784 4087 100 0 waitForDcDisconnected
xform 0 -624 4208
p -712 4120 100 0 -1 CALC:A
p -184 4908 100 0 0 DESC:calcout record
p 48 4638 100 0 0 DOPT:Use CALC
p 48 4670 100 0 0 OOPT:Transition To Non-zero
p -696 4320 100 0 1 SCAN:Passive
p -368 4464 60 0 0 def(INPA):$(gm)dcDisabled.VAL
p -368 4464 60 0 0 def(INPB):0.0
p -368 4464 60 0 0 def(INPC):0.0
p -832 4280 75 0 -1 pproc(INPA):CPP
use hwin -560 5095 100 0 hwin#354
xform 0 -464 5136
p -592 5120 100 0 -1 val(in):$(CAR_ERROR)
use elongouts -160 4919 100 0 ObsCarDisconVal
xform 0 -32 5008
p -128 5088 100 0 1 OMSL:closed_loop
use oslBorderC -1248 3527 100 0 oslBorderC#351
xform 0 432 4832
p 1692 3776 120 256 -1 Title:GMOS Instrument Sequencer CAR records
use gmSeqObserveCar -800 5287 100 0 gmSeqObserveCar#337
xform 0 -680 5440
use ecars -224 5287 100 0 observeC
xform 0 -64 5456
use ecars 928 4887 100 0 initC
xform 0 1088 5056
use ecars 928 4487 100 0 datumC
xform 0 1088 4656
use ecars 928 4055 100 0 parkC
xform 0 1088 4224
use ecars 928 5319 100 0 testC
xform 0 1088 5488
use gmSeqSeqCommandCar 408 5367 100 0 gmSeqSeqCommandCar#282
xform 0 520 5504
p 424 5344 100 0 -1 seta:seqcommand test
use gmSeqSeqCommandCar 408 4535 100 0 gmSeqSeqCommandCar#283
xform 0 520 4672
p 428 4508 100 0 -1 seta:seqcommand datum
use gmSeqSeqCommandCar 408 4935 100 0 gmSeqSeqCommandCar#284
xform 0 520 5072
p 428 4908 100 0 -1 seta:seqcommand init
use gmSeqSeqCommandCar 408 4103 100 0 gmSeqSeqCommandCar#285
xform 0 520 4240
p 428 4076 100 0 -1 seta:seqcommand park
[comments]

GMOS Instrument Sequencer Checkout.

Assumptions are ....

        * The GMOS components control database has been loaded.

        * The mask, filter and grating lookup tables (mask.lut, flt.lut,
          and gr.lut) exists and contain barcodes valid for the current
          instrument configuration.

        * There are no interlocks and access doors are closed or cheated.

        * There is nothing obstructing any of the instrument components.

	* The detector translation assembly is NOT in follow mode.

        If so, it should be safe to check out the instrument.
	
This script sets up for the full checkout, "checkout instrument"...


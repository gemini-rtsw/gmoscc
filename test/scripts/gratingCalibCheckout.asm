GMOS Grating Assembly Checkout ... grating assembly calibration check out.

Assumptions are ....

        * The GMOS components control database has been loaded.

	* The grating assembly has already been initialised and
          indexed, and preferably checked out with the basic check out
          script using

               checkout grating

	* The grating calibration lookup tables (gr.lut and aux_gr.lut)
          exist and contains calibrations valid for the current configuration.

        * There is nothing obstructing the grating turret or grating cells.

       ONLY EXECUTE THIS SCRIPT IF YOU ARE CONFIDENT THE GRATING TURRET
       CAN BE RAISED AND LOWERED UNATTENDED.

GMOS Mask Assembly Checkout ... mask update check out.

This script updates the mask assembly and then places in turn each one
of the installed masks into the beam and then returns it to the
cassette.

Assumptions are ....

        * The IOC has been depowered and re-started cold, or the
          mask assembly has been reset into its startup state.

        * The GMOS components control database has been loaded.

        * The mask lookup table (msk.lut) exists and contains at least 
          two mask IDs (preferably more) valid for its current
          configuration.

        It is safe to select and load masks....

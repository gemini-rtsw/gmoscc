GMOS Mask Assembly Checkout ... mask assembly check out

Assumptions are ....

        * The IOC has been depowered and re-started cold, or the
          mask assembly has been reset into its startup state.

        * The GMOS components control database has been loaded.

        * The mask lookup table (msk.lut) exists and contains mask IDs
          valid for its current configuration. The first mask in the
          lookup table (msk.lut) should be an IFU and the last mask
          should be a mask.

        It is safe to select and load masks....

GMOS Grating Assembly Checkout ... grating assembly basic check out.

Assumptions are ....

        * The IOC has been depowered and re-started cold, or the grating
          assembly has been reset to its startup state.

        * The GMOS components control database has been loaded.

        * The grating lookup table (gr.lut) exists and contains
          grating IDs valid for its current configuration.

        * There is nothing obstructing the turret or grating cells.

        NOTE: The first time this script lowers the grating turret
        you will be prompted to verify that the turret is in the
        correct place. Therafter, the script will assume that the
        turret will operate reliably.

GMOS Grating Assembly Checkout ... grating turret device check out.

This FULL check out script contains A LOT of prompts for verifying the
turret position visually. The script will not attempt to lower the lifter
and can be run in -auto mode to drive the turret automatically to all of
its defined positions. READ THESE ASSUMPTIONS FIRST, THOUGH. 

NOTE: This is a FULL checkout script. You will need to check the turret
      position several times.

Assumptions are ....

        * The IOC has been depowered and re-started cold, or the
          grating turret device has been reset to its startup state.

        * The GMOS components control database has been loaded.

        * THE GRATING LIFTER *MUST* BE RAISED.

        * It is safe to move the grating turret...

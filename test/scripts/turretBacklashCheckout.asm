GMOS Grating Assembly Checkout ... grating turret backlash removal experiment.

This script runs an experiment to see if how the grating turret moves
when its backlash is taken up slowly before each positive movement.
The script can be run once or repeated to check for juddering in the movement.

Assumptions are ....

        * The GMOS components control database has been loaded.

        * The grating turret has already been initialised and
          indexed, and preferably checked out with the basic check out
          script using

               checkout turret

        * The turret is initially at position B

        * THE GRATING LIFTER *MUST* BE RAISED.

        * It is safe to move the grating turret...

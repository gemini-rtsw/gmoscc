GMOS Grating Assembly Checkout ... grating turret device checkout
for backlash removal setup.

This script is the initial part of the turret backlash compensation
test and is designed to check that it is safe to run the
script turretBacklashCheckout.scr.

THIS SCRIPTS IS A STOP-GAP WHILE THE TURRET MICROSWITCHES ARE BROKEN.

Assumptions are ....

	The IOC has been depowered and re-started cold.

	--- THE TURRET MICROSWITCHES ARE BROKEN ---

	The database "gmosCcTop.db" has been freshly loaded
	and the grating turret has not been initialized or
	indexed, and the device is stationary and depowered.

	In addition, the grating lifter MUST be raised.
        i.e.

		grtDevice.HPVL   = 0
		grtDevice.MIP    = STOPPED
		grtDevice.PSTA   = 0
		grLifterDown.VAL = NOTDOWN
		grLifterUp.VAL   = UP

	It is safe to move the grating turret.

	ENSURE THAT THE GRATING TURRET HAS BEEN MANUALLY
	POSITIONED AT ITS LOWER (A POSITION) HARD LIMIT.
	THE LIFTER *MUST* BE IN A RAISED POSITION.


Check cross talk as Z stage is moved
------------------------------------

This script makes the following assumptions:

* The GMOS detector controller and components controller databases
  are loaded and ready for use.

* You have configured the detector controller and already defined an
  appropriate exposure time, gain setting, readout speed and region of
  of interest. The script will set file names automatically.

* You have initialized and indexed the translation assembly.

* You have already selected an appropriate pin hole mask and have
  already positioned the filter wheels and grating assembly.
  Please make a separate note of the filter, mask and grating used.

* The GMOS environment is dark and it is safe to begin observations.

* You have checked that the best focus position assumed by the script
  is apporopriate.

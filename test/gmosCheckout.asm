
Carry out automated checks on all GMOS components
-------------------------------------------------

This script makes the following assumptions:

* The GMOS components controller database is loaded and ready for use.

* Suitable lookup tables exist in the /gemini/epics3.13.4/gmos/gmos/data directory.

* It is safe to move all the GMOS components.

Don't forget to specify "-noifu" if the mask assembly does not have an
IFU.

NOTE: If you are running this script at engineering level (3 or 4) there
will be some interactive tests at the beginning regardless of whether you
specified -auto. Please follow the instructions on the screen and have the
engineering control screens ready. 

This script will take several times longer to run than executing all the
individual scripts manually because all the tests will be run
sequentially. 


GMOS Detector Controller Software Checkout.

NOTE: This script will check out the basic functions of the NOAO/GMOS
      detector controller. The script checks that the detector
      controller software responds as expected to various commands.

      The script does not assess the quality of the data generated
      by the detector controller to assess the quality of the chips
      or the amount of dark current etc... Separate procedures are
      necessary to do this.

      The script also does not test the interface with the Gemini DHS.

Assumptions are ....

        * The GMOS/NOAO detector control database has been loaded.

        * The "fitssaver" process has been started and the "useFits"
          command successfully executed on the detector controller
          VxWorks console.

        * It is safe to operate the shutter (unless the "noshutter"
          version of the database has been loaded).

        * It is safe to operate the detector controller electronics.

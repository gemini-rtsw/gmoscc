/*
************************************************************************
*  U K     A S T R O N O M Y    T E C H N O L O G Y    C E N T R E     *
*                                                                      *
*  Royal Observatory, Blackford Hill, Edinburgh, EH9 3HJ, UK.          *
*                                                                      *
*  Funded by the Particle Physics and Astronomy Research Council.      *
*                                                                      *
************************************************************************
*
*   FILENAME
*   --------
*   lifterSim.stpp
*
*   DESCRIPTION
*   -----------
*   This file contains EPICS state transition language program to simulate
*   the activate of the indexer turret lifter microswitches. It is for test
*   purposes only and should only be loaded from the startup file when the
*   real mechanism is not available or not functioning.
*
*   FUNCTION NAME(S)
*   ----------------
*   lifter_ss  - Simulate action of turret and lifter microswitches
*
*   PROGRAM
*   --------
*   lifterSim
*
*   INVOCATION
*   -----------
*   Include the following line in the VxWorks startup script:
*
*   seq &lifterSim, "top=gm:cc:, dev=dd"
*
*   PARAMETERS (all are input)
*   --------------------------
*      top      string    String to be substituted for the {top} macro
*                         used to for the top level name of the prefix
*                         EPICS database. The default value is "gm:cc:"
*
*      dev      string    String to be substituted for the {dev} macro
*                         used for the name of the device. The default
*                         value is "dd"
*
*   RETURN VALUE
*   ------------
*   N/A
*
*   LIMITATIONS
*   -----------
*   This file needs to be edited every time the contents of the grt.lut
*   lookup table changes.
*
*   AUTHORS
*   -------
*   Sean Prior, UKATC  (srp@roe.ac.uk) based on assemblyCar.stpp (smb@roe.ac.uk)
*   Steven Beard, UKATC (smb@roe.ac.uk) - Corrected states of status switches.
*
*/
/* *INDENT-OFF* */
/* *INDENT-ON* */

program lifterSim ("top=gm:cc:, dev=gr")

%%static char rcsid[]="$Id$";

%%#include <stdlib.h>
%%#include <stdioLib.h>
%%#include <string.h>
%%#include <ctype.h>
%%#include <logLib.h>

%%#include <cad.h>
%%#include <cadRecord.h>
%%#include <carRecord.h>

%%#include "lifter.h"       /* Contains GR_ constants */

/*
 * The following constants must agree with the positions contained in the grating turret
 * lookup table grt.lut.
 */

#define POSA_LO  103.0
#define POSA_HI  104.0
#define POSB_LO   13.0
#define POSB_HI   14.0
#define POSC_LO   43.0
#define POSC_HI   44.0
#define POSD_LO   73.0
#define POSD_HI   74.0

/*
 * The following declarations assign variables to fields in the EPICS database.
 */

long valveUp;
assign valveUp to "{top}{dev}UpValve.VAL";
monitor valveUp;

long valveDown;
assign valveDown to "{top}{dev}DownValve.VAL";
monitor valveDown;

long lifterUp;
assign lifterUp to "{top}{dev}LifterUp.VAL";

long lifterDown;
assign lifterDown to "{top}{dev}LifterDown.VAL";
monitor lifterDown;

double turretAngle;
assign turretAngle to "{top}{dev}tDevice.MPOS";
monitor turretAngle;

long swPosA;
assign swPosA to "{top}{dev}TurretPosA.VAL";

long swPosB;
assign swPosB to "{top}{dev}TurretPosB.VAL";

long swPosC;
assign swPosC to "{top}{dev}TurretPosC.VAL";

long swPosD;
assign swPosD to "{top}{dev}TurretPosD.VAL";

/* ===================================================================== */

ss lifter_ss
{
  state init
  {
       when ( pvConnectCount( ) == pvChannelCount( ) )
      {
          logMsg("SNL connected for lifterSim\n", 0, 0, 0, 0, 0 ,0);
          logMsg("WARNING - IT IS ASSUMED THE GRATING LIFTER IS BEING SIMULATED\n", 0, 0, 0, 0, 0 ,0);

          /* Initialise the turret switch states */

          lifterDown = GR_SWITCH_ACTIVE;
          lifterUp = GR_SWITCH_INACTIVE;
          pvPut(lifterDown);
          pvPut(lifterUp);
      } state idle
  }
 
  /* --------------------------------------------------------------------- */

  state idle
  {
      when ( valveUp == GR_VALVE_ACTIVE && valveDown == GR_VALVE_INACTIVE && lifterUp == GR_SWITCH_INACTIVE)
      {
      } state raiseTurret

      when ( valveUp == GR_VALVE_INACTIVE && valveDown == GR_VALVE_ACTIVE && lifterDown == GR_SWITCH_INACTIVE)
      {
      } state lowerTurret
  }

  /* --------------------------------------------------------------------- */

  state raiseTurret
  {
      when(delay(0.5))
      {
          logMsg("Raising simulated turret...\n", 0, 0, 0, 0, 0 ,0);
          lifterDown = GR_SWITCH_INACTIVE;
          pvPut(lifterDown);
      } state travelUp
  }

  state travelUp
  {
      when(delay(2.0))
      {
          lifterUp = GR_SWITCH_ACTIVE;
          pvPut(lifterUp);
      } state idle
  }
  
  /* --------------------------------------------------------------------- */

  state lowerTurret
  {
      when(delay(0.5))
      {
          logMsg("Lowering simulated turret...\n", 0, 0, 0, 0, 0 ,0);
          lifterUp = GR_SWITCH_INACTIVE;
          pvPut(lifterUp);
      } state travelDown
  }

  state travelDown
  {
      when(delay(2.0))
      {
          lifterDown = GR_SWITCH_ACTIVE;
          pvPut(lifterDown);
      } state idle
  }
}

/* ===================================================================== */

ss lifterPos_ss
{
  state init
  {
       when ( pvConnectCount( ) == pvChannelCount( ) )
      {
           logMsg("SNL connected for lifterPos\n", 0, 0, 0, 0, 0 ,0);

           /* Initialise the simulated in position switches */

           swPosA = GR_SWITCH_INACTIVE;
           swPosB = GR_SWITCH_INACTIVE;
           swPosC = GR_SWITCH_INACTIVE;
           swPosD = GR_SWITCH_INACTIVE;

           pvPut(swPosA);
           pvPut(swPosB);
           pvPut(swPosC);
           pvPut(swPosD);

      } state idle
  }
 
  /* --------------------------------------------------------------------- */

  state idle
  {
       when(delay(0.5))
       {
       } state checkStatus
  }

  /* --------------------------------------------------------------------- */

  state checkStatus
  {
      when ( lifterDown == GR_SWITCH_ACTIVE && turretAngle > POSA_LO && turretAngle < POSA_HI)
      {
      } state positionA

      when ( lifterDown == GR_SWITCH_ACTIVE && turretAngle > POSB_LO && turretAngle < POSB_HI)
      {
      } state positionB

      when ( lifterDown == GR_SWITCH_ACTIVE && turretAngle > POSC_LO && turretAngle < POSC_HI)
      {
      } state positionC

      when ( lifterDown == GR_SWITCH_ACTIVE && turretAngle > POSD_LO && turretAngle < POSD_HI)
      {
      } state positionD

      when ( lifterDown == GR_SWITCH_INACTIVE)
      {
      } state up
             
  }

  /* --------------------------------------------------------------------- */

  state up
  {
      when()
      {
           swPosA = GR_SWITCH_INACTIVE;
           swPosB = GR_SWITCH_INACTIVE;
           swPosC = GR_SWITCH_INACTIVE;
           swPosD = GR_SWITCH_INACTIVE;

           pvPut(swPosA);
           pvPut(swPosB);
           pvPut(swPosC);
           pvPut(swPosD);

      } state idle
  }

  /* --------------------------------------------------------------------- */

  state positionA
  {
      when()
      {
           swPosA = GR_SWITCH_ACTIVE;
           swPosB = GR_SWITCH_INACTIVE;
           swPosC = GR_SWITCH_INACTIVE;
           swPosD = GR_SWITCH_INACTIVE;

           pvPut(swPosA);
           pvPut(swPosB);
           pvPut(swPosC);
           pvPut(swPosD);

      } state idle
  }

  /* --------------------------------------------------------------------- */

  state positionB
  {
      when()
      {
           swPosA = GR_SWITCH_INACTIVE;
           swPosB = GR_SWITCH_ACTIVE;
           swPosC = GR_SWITCH_INACTIVE;
           swPosD = GR_SWITCH_INACTIVE;

           pvPut(swPosA);
           pvPut(swPosB);
           pvPut(swPosC);
           pvPut(swPosD);

      } state idle
  }

  /* --------------------------------------------------------------------- */

  state positionC
  {
      when()
      {
           swPosA = GR_SWITCH_INACTIVE;
           swPosB = GR_SWITCH_INACTIVE;
           swPosC = GR_SWITCH_ACTIVE;
           swPosD = GR_SWITCH_INACTIVE;

           pvPut(swPosA);
           pvPut(swPosB);
           pvPut(swPosC);
           pvPut(swPosD);

      } state idle
  }

  /* --------------------------------------------------------------------- */

  state positionD
  {
      when()
      {
           swPosA = GR_SWITCH_INACTIVE;
           swPosB = GR_SWITCH_INACTIVE;
           swPosC = GR_SWITCH_INACTIVE;
           swPosD = GR_SWITCH_ACTIVE;

           pvPut(swPosA);
           pvPut(swPosB);
           pvPut(swPosC);
           pvPut(swPosD);

      } state idle
  }
}

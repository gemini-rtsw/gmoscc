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
*   softCar.stpp
*
*   DESCRIPTION
*   -----------
*   This file contains the EPICS state transition language program for
*   managing the CAR record associated with a GMOS soft command. Typical
*   soft commands are "debug" and "simulate", where the corresponding
*   CAR record just flips quickly to BUSY and back to IDLE. The program
*   monitors an "Event" record assumed to be controlled from the CAD record
*   and transmits changes of state to the corresponding CAR record. SNL is used
*   because it ensures the CAD and CAR records are in different lock
*   sets.
*
*   FUNCTION NAME(S)
*   ----------------
*   soft_ss  - Manage soft (trivial) CAR record.
*
*   PROGRAM
*   --------
*   softCar
*
*   INVOCATION
*   -----------
*   Include the following line in the VxWorks startup script:
*
*   seq &softCar, "top=gm:cc:, cad=debug"
*
*   PARAMETERS (all are input)
*   --------------------------
*      top      string    String to be substituted for the {top} macro
*                         used to for the top level name of the prefix
*                         EPICS database. The default value is "gm:cc:"
*
*      cad      string    String to be substituted for the {cad} macro
*                         used for the name of the CAD record. The default
*                         value is "init"
*
*   RETURN VALUE
*   ------------
*   N/A
*
*   AUTHOR
*   ------
*   Steven Beard, UKATC  (smb@roe.ac.uk)
*
*/
/* *INDENT-OFF* */
/*
 * $Log$
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.1  2000/04/10 09:20:04  gmos
 * gmtools and gmoscc merged
 *
 * Revision 1.2  1999/11/30 13:31:06  gmos
 * UKATC banner added.
 *
 * Revision 1.1  1999/08/02 10:51:34  gmos
 * First version
 *
 *
 */
/* *INDENT-ON* */

program softCar ("top=gm:cc:, cad=debug")

/*
 * The following option is needed to make the code reentrant.
 * This allows a separate copy of this sequence program to be
 * loaded for each CAD/CAR pair.
 */

option +r;

%%static char rcsid[]="$Id$";

%%#include <stdlib.h>
%%#include <stdioLib.h>
%%#include <string.h>
%%#include <ctype.h>
%%#include <logLib.h>

%%#include <cad.h>
%%#include <carRecord.h>
%%#include <menuCarstates.h>

/* #define VERBOSE */        /* Define this for verbose output */

/*
 * The following declarations assign variables to fields in the EPICS database.
 */

long cadBusy;   /* BUSY output from the CAD record */
assign cadBusy to "{top}{cad}Event.VAL";
monitor cadBusy;

long carIval;   /* State input to the CAR record */
assign carIval to "{top}{cad}C.IVAL";

long carIerr;   /* Error number input to the CAR record */
assign carIerr to "{top}{cad}C.IERR";

string carImss;   /* Error message input to the CAR record */
assign carImss to "{top}{cad}C.IMSS";

string carName;   /* Name of CAR record */
assign carName to "{top}{cad}C.NAME";


/* ===================================================================== */

ss soft_ss
{

/*+
 *   Function name:
 *   State set "soft_ss"
 *
 *   Purpose:
 *   Manage CAD/CAR record pair associated with a GMOS soft command
 *
 *   Invocation:
 *   Invoked automatically when sequence program "softCar" starts.
 *
 *   Parameters: (">" input, "!" modified, "<" output)
 *   None
 *
 *   Return value:
 *   N/A
 *
 *   External functions:
 *   EPICS sequencer functions used are
 *   pvGet      - Get a process variable
 *   pvPut      - Put a process variable
 *
 *   External variables:
 *   See definitions above
 *
 *   Requirements:
 *   Same as requirements for program "softCar"
 *
 *   Limitations:
 *   Same as limitations for program "softCar"
 *
 *-
 */

/* --------------------------------------------------------------------- */

  state init
  {

/*
 * state init - This is the state the soft_ss state set begins in
 *              when it first starts running.
 */
 
/* Wait until all the appropriate connections have been made. */

    when ( pvConnectCount( ) == pvChannelCount( ) )
    {
      pvGet( carName );
      logMsg( "SNL for %s connected.\n", carName, 0, 0, 0, 0, 0 );

      /* Carry out any initialisation here. */

      /* Jump to the "idle" state. */

#ifdef VERBOSE
    logMsg( "%s - INIT-IDLE.\n", carName, 0, 0, 0, 0, 0 );
#endif

    } state idle
  }
 
/* --------------------------------------------------------------------- */

  state idle
  {

/*
 * state idle - Remain in this state until the CAD record event goes BUSY.
 *              When this happens set the CAR record to busy and jump to
 *              the BUSY state.
 */

    when ( cadBusy == menuCarstatesBUSY )
    {

      carIval = menuCarstatesBUSY;
      pvPut( carIval );

#ifdef VERBOSE
    logMsg( "%s - IDLE-BUSY.\n", carName, 0, 0, 0, 0, 0 );
#endif

    } state busy
  }


/* --------------------------------------------------------------------- */

  state busy
  {

/*
 * state busy - Remain in this state until the CAD record event goes IDLE or ERROR.
 */

    when ( cadBusy == menuCarstatesIDLE )
    {

      /*
       * When the CAD record event goes IDLE IDLE reflect that fact in the CAR record
       * and jump to the IDLE state.
       */

      carIval = menuCarstatesIDLE;
      pvPut( carIval );

#ifdef VERBOSE
    logMsg( "%s - BUSY-IDLE.\n", carName, 0, 0, 0, 0, 0 );
#endif

    } state idle

/*                               --  --  --                              */

    when ( cadBusy == menuCarstatesERROR )
    {

      /*
       * When an error occurs transfer the error information to the CAR record
       * and jump to the ERROR state.
       */

      strcpy( carImss, "Soft error" );
      pvPut( carImss );

      carIerr = -1;
      pvPut( carIerr );

      carIval = menuCarstatesERROR;
      pvPut( carIval );

#ifdef VERBOSE
    logMsg( "%s - BUSY-ERROR.\n", carName, 0, 0, 0, 0, 0 );
#endif

    } state error
  }

/* --------------------------------------------------------------------- */

  state error
  {

/*
 * state error - Remain in this state the CAD record event goes back to IDLE or BUSY.
 */

    when ( cadBusy == menuCarstatesIDLE )
    {
      /*
       * When the CAD record event goes IDLE clear the CAR record
       * and jump to the IDLE state.
       */

      carIerr = 0;
      pvPut( carIerr );

      carIval = menuCarstatesIDLE;
      pvPut( carIval );

#ifdef VERBOSE
    logMsg( "%s - ERROR-IDLE.\n", carName, 0, 0, 0, 0, 0 );
#endif

    } state idle

/*                               --  --  --                              */

    when ( cadBusy == menuCarstatesBUSY )
    {
      /*
       * When the CAD record event goes BUSY clear the CAR record
       * and jump to the BUSY state.
       */

      carIerr = 0;
      pvPut( carIerr );

      carIval = menuCarstatesBUSY;
      pvPut( carIval );

#ifdef VERBOSE
    logMsg( "%s - ERROR-BUSY.\n", carName, 0, 0, 0, 0, 0 );
#endif

    } state busy
  }
}

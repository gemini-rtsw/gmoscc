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
*   systemCar.stpp
*
*   DESCRIPTION
*   -----------
*   This file contains the EPICS state transition language program for
*   managing the CAR record associated with a GMOS system command. Typical
*   soft commands are "init" and "datum", which have a separate "commandC"
*   CAR record which has to change to BUSY while that particular command
*   is executing. The program detects an IDLE-BUSY transition by monitoring
*   an "Event" record assumed to be controlled from the CAD record, and detects a BUSY-IDLE
*   transition by monitoring the "assemC" CAR record for all the assemblies. SNL is used
*   because it ensures the CAD and CAR records are in different lock
*   sets.
*
*   FUNCTION NAME(S)
*   ----------------
*   system_ss  - Manage system CAR record.
*
*   PROGRAM
*   --------
*   systemCar
*
*   INVOCATION
*   -----------
*   Include the following line in the VxWorks startup script:
*
*   seq &systemCar, "top=gm:cc:, cad=debug"
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
*   LIMITATIONS
*   -----------
*   Even this program may be too complex to express safely in SNL.
*
*   AUTHOR
*   ------
*   Steven Beard, UKATC  (smb@roe.ac.uk)
*
*/
/* *INDENT-OFF* */
/*
 * $Log$
 * Revision 1.1  2001/11/28 20:08:53  mbec
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.2  2000/07/25 11:10:42  gmos
 * Timeout message modified.
 *
 * Revision 1.1  2000/04/10 09:20:04  gmos
 * gmtools and gmoscc merged
 *
 * Revision 1.2  1999/11/30 13:31:07  gmos
 * UKATC banner added.
 *
 * Revision 1.1  1999/08/02 10:51:34  gmos
 * First version
 *
 *
 */
/* *INDENT-ON* */

program systemCar ("top=gm:cc:, cad=debug")

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


#define VERBOSE         /* Define this for verbose output */

/*
 * The following declarations assign variables to fields in the EPICS database.
 */

long cadBusy;   /* BUSY output from the CAD record */
assign cadBusy to "{top}{cad}Event.VAL";
monitor cadBusy;

long assemBusy;  /* Status of the combined assembly CAR record */
assign assemBusy to "{top}assemC.IVAL";
monitor assemBusy;

long carIval;   /* State input to the CAR record */
assign carIval to "{top}{cad}C.IVAL";

long carIerr;   /* Error number input to the CAR record */
assign carIerr to "{top}{cad}C.IERR";

string carImss;   /* Error message input to the CAR record */
assign carImss to "{top}{cad}C.IMSS";

string carName;   /* Name of CAR record */
assign carName to "{top}{cad}C.NAME";


/* ===================================================================== */

ss system_ss
{

/*+
 *   Function name:
 *   State set "system_ss"
 *
 *   Purpose:
 *   Manage CAD/CAR record pair associated with a GMOS soft command
 *
 *   Invocation:
 *   Invoked automatically when sequence program "systemCar" starts.
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
 *   Same as requirements for program "systemCar"
 *
 *   Limitations:
 *   Same as limitations for program "systemCar"
 *
 *-
 */

/* --------------------------------------------------------------------- */

  state init
  {

/*
 * state init - This is the state the system_ss state set begins in
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
 *              the STARTING state.
 */

    when ( cadBusy == menuCarstatesBUSY )
    {

      carIval = menuCarstatesBUSY;
      pvPut( carIval );

#ifdef VERBOSE
    logMsg( "%s - IDLE-STARTING.\n", carName, 0, 0, 0, 0, 0 );
#endif

    } state starting
  }

/* --------------------------------------------------------------------- */

  state starting
  {

/*
 * state starting - Remain in this state until the global assembly CAR record goes BUSY.
 *                  When this happens jump to the BUSY state.
 *                  This state will time out after 30 seconds.
 */

    when ( assemBusy == menuCarstatesBUSY )
    {

#ifdef VERBOSE
    logMsg( "%s - STARTING-BUSY.\n", carName, 0, 0, 0, 0, 0 );
#endif

    } state busy

/*                               --  --  --                              */

    when ( delay(30.0) )
    {

    logMsg( "%s - Assemblies did not go BUSY. STARTING state timed out.\n", carName, 0, 0, 0, 0, 0 );

    } state busy
  }

/* --------------------------------------------------------------------- */

  state busy
  {

/*
 * state busy - Remain in this state until the CAD record event and the global assembly CAR record both go IDLE,
 * or the assembly CAR changes to ERROR. (It is assumed the database has been defined so the CAD event
 * never changes to ERROR).
 */

    when ( (cadBusy == menuCarstatesIDLE) && (assemBusy == menuCarstatesIDLE) )
    {

      /*
       * When both CAR records go IDLE reflect that fact in the CAR record
       * and jump to the IDLE state.
       */

      carIval = menuCarstatesIDLE;
      pvPut( carIval );

#ifdef VERBOSE
      logMsg( "%s - BUSY-IDLE.\n", carName, 0, 0, 0, 0, 0 );
#endif

    } state idle

/*                               --  --  --                              */

    when ( assemBusy == menuCarstatesERROR )
    {

      /*
       * When an assembly error occurs transfer the error information to the CAR record
       * and jump to the ERROR state.
       */

      strcpy( carImss, "Assembly error" );
      pvPut( carImss );

      carIerr = -1;
      pvPut( carIerr );

      carIval = menuCarstatesERROR;
      pvPut( carIval );

#ifdef VERBOSE
      logMsg( "%s - BUSY-ERROR (assembly error).\n", carName, 0, 0, 0, 0, 0 );
#endif

    } state error
  }

/* --------------------------------------------------------------------- */

  state error
  {

/*
 * state error - Remain in this state until both CAR records go back to IDLE or the assembly CAR
 * changes to BUSY.
 */

    when ( (cadBusy == menuCarstatesIDLE) && (assemBusy == menuCarstatesIDLE) )
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

    when ( assemBusy == menuCarstatesBUSY )
    {

      /*
       * When the assembly CAR record goes BUSY clear the output CAR record
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

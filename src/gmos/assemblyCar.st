/*
*   FILENAME
*   --------
*   assemblyCar.stpp
*
*   DESCRIPTION
*   -----------
*   This file contains the EPICS state transition language program for
*   managing the CAR record associated with a GMOS assembly. The program
*   monitors the BUSY field of the assembly record and transmits changes
*   of state to the corresponding assembly CAR record. SNL is used
*   because it ensures the assembly and CAR records are in different lock
*   sets.
*
*   FUNCTION NAME(S)
*   ----------------
*   assembly_ss  - Manage assembly CAR record.
*
*   PROGRAM
*   --------
*   assemblyCar
*
*   INVOCATION
*   -----------
*   Include the following line in the VxWorks startup script:
*
*   seq &assemblyCar, "top=gm:cc:, dev=dd"
*
*   NOTE
*   ----
*   This task needs to be run at a sufficiently high priority so the task which results in the
*   assembly record going BUSY does not itself lock out this task and prevent it responding to
*   the IDLE-BUSY event.
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
*
*   AUTHOR
*   ------
*   Steven Beard, UKATC  (smb@roe.ac.uk)
*
*/
/* *INDENT-OFF* */
/*
 * $Log$
 * Revision 1.1  2001/11/28 20:08:44  mbec
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.2  2001/01/26 11:39:22  gmos
 * Fixed trivial typo in comment.
 *
 * Revision 1.1  2000/04/10 09:19:58  gmos
 * gmtools and gmoscc merged
 *
 * Revision 1.3  1999/11/16 13:54:18  gmos
 * Added ERROR to BUSY transition
 *
 * Revision 1.2  1999/06/14 16:17:18  gmos
 * More comments added
 *
 * Revision 1.1  1999/06/11 16:32:03  gmos
 * First version of SNL for handling CAR record
 *
 */
/* *INDENT-ON* */

program assemblyCar ("top=gm:cc:, dev=dd")

/*
 * The following option is needed to make the code reentrant.
 * This allows a separate copy of this sequence program to be
 * loaded for each assembly.
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

#define VERBOSE
/*
 * The following declarations assign variables to fields in the EPICS database.
 */

long assemblyBusy;   /* BUSY output from the assembly record */
assign assemblyBusy to "{top}{dev}Assembly.BUSY";
monitor assemblyBusy;

string assemblyMess;   /* Message output from the assembly record */
assign assemblyMess to "{top}{dev}Assembly.MESS";

string assemblyName;   /* Name of assembly record */
assign assemblyName to "{top}{dev}Assembly.NAME";

long carIval;          /* State input to the CAR record */
assign carIval to "{top}{dev}C.IVAL";

long carIerr;          /* Error status input to the CAR record */
assign carIerr to "{top}{dev}C.IERR";

string carImss;        /* Message input to the CAR record */
assign carImss to "{top}{dev}C.IMSS";


/* ===================================================================== */

ss assembly_ss
{

/*+
 *   Function name:
 *   State set "assembly_ss"
 *
 *   Purpose:
 *   Manage CAR record associated with a GMOS assembly
 *
 *   Invocation:
 *   Invoked automatically when sequence program "assemblyCar" starts.
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
 *   Same as requirements for program "assemblyCar"
 *
 *   Limitations:
 *   Same as limitations for program "assemblyCar"
 *
 *-
 */

/* --------------------------------------------------------------------- */

  state init
  {

/*
 * state init - This is the state the assembly_ss state set begins in
 *              when it first starts running.
 */
 
/* Wait until all the appropriate connections have been made. */
    when ( pvConnectCount( ) == pvChannelCount( ) )
    {
      pvGet( assemblyName );
      logMsg( "SNL for %s connected.\n", assemblyName, 0, 0, 0, 0, 0 );

      /* Carry out any initialisation here. */

      /* Jump to the "idle" state. */
	#ifdef VERBOSE
		 printf("assemblyCar: INIT-IDLE\n");
      	#endif
    } state idle
  }
 
/* --------------------------------------------------------------------- */

  state idle
  {

/*
 * state idle - Remain in this state until the assembly record goes BUSY.
 *              When this happens set the CAR record to busy and jump to
 *              the BUSY state.
 */
    when ( assemblyBusy == menuCarstatesBUSY )
    {
      carIval = menuCarstatesBUSY;
      pvPut( carIval );
     	#ifdef VERBOSE
       		printf("assemblyCar: IDLE-BUSY\n");	
	#endif
    } state busy
  }

/* --------------------------------------------------------------------- */

  state busy
  {

/*
 * state busy - Remain in this state until the assembly record goes IDLE or ERROR.
 */
    when ( assemblyBusy == menuCarstatesIDLE )
    {

      /*
       * When the assembly record goes IDLE reflect that fact in the CAR record
       * and jump to the IDLE state.
       */

      carIval = menuCarstatesIDLE;
      pvPut( carIval );
     	#ifdef VERBOSE
		printf("assemblyCar: BUSY-IDLE\n");
	#endif

    } state idle

/*                               --  --  --                              */

    when ( assemblyBusy == menuCarstatesERROR )
    {

      /*
       * When an error occurs transfer the error information to the CAR record
       * and jump to the ERROR state.
       */

      pvGet( assemblyMess );
      strcpy( carImss, assemblyMess );
      pvPut( carImss );

      carIerr = -1;
      pvPut( carIerr );

      carIval = menuCarstatesERROR;
      pvPut( carIval );

	#ifdef VERBOSE
		printf("assemblyCar: BUSY-ERROR\n");
	#endif

    } state error
  }

/* --------------------------------------------------------------------- */

  state error
  {

/*
 * state error - Remain in this state until the assembly record goes BUSY again
 * or back to IDLE.
 */
    when ( assemblyBusy == menuCarstatesIDLE )
    {
      /*
       * When the assembly record goes IDLE clear the CAR record
       * and jump to the IDLE state.
       */

      carIerr = 0;
      pvPut( carIerr );

      carIval = menuCarstatesIDLE;
      pvPut( carIval );
	
	#ifdef VERBOSE 
		printf("assemblyCar: ERROR-IDLE\n");
	#endif
    } state idle

/*                               --  --  --                              */

    when ( assemblyBusy == menuCarstatesBUSY )
    {

      /*
       * When the assembly record goes BUSY clear the CAR record
       * and jump to the BUSY state.
       */

      carIerr = 0;
      pvPut( carIerr );

      carIval = menuCarstatesBUSY;
      pvPut( carIval );
	#ifdef VERBOSE	
		printf("assemblyCar: ERROR-BUSY");
	#endif

    } state busy
  }
}

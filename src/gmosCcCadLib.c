static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id$"};
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
*   gmosCadLib.c
*
*   DESCRIPTION
*   -----------
*   This file contains the source for functions used by the GMOS
*   system CAD records. These functions are used to validate the
*   directives and attributes given to the CAD record.
*
*   There are various classes of functions in this module. Some are
*   connected with specific system commands and others are utility
*   functions which may be called from within a CAD record.
*
*   NOTE
*   ----
*   These functions are based on the ones written for the CICS.
*
*   FUNCTION NAME(S)
*   ----------------
*
*   Specific system commands...
*
*   gmosCadInit            - Handle init command
*   gmosCadDatum           - Handle datum command
*   gmosCadPark            - Handle park command
*   gmosCadTest            - Handle test command
*   gmosCadUpdate          - Handle update command
*   gmosCadDebug           - Handle debug command
*   gmosCadSimulate        - Handle simulate command
*
*   Utilities...
*
*   gmosCadAttributeFanout - Fan out a single attribute to several outputs 
*   gmosCadDirFanout       - Fan out a directive to CADs connected to the outputs
*   gmosCadModeFanout      - Fan out a mode to assembly records connected to the outputs
*
*   DEPENDENCIES
*   ------------
*   The names of the subroutines in this file should be identical to those
*   declared in the SNAM field of each CAD record. If a change is made to
*   the name of a subroutine, that change should be reflected in the SNAM
*   field, and vice versa.
*
*   The ordering of the attributes within each CAD record (A, B, C...) is
*   defined in the description of the interface between the CAD database
*   and its clients. Changes to that interface should be reflected in this
*   file.
*
*   LIMITATIONS
*   ------------
*
*   AUTHOR
*   ------
*   Steven Beard  (smb@roe.ac.uk)
*
*   HISTORY
*   -------
*   28-Mar-1996: Original version for CICS.                             (smb)
*   12-Aug-1997: Final CICS version.                                    (smb)
*   09-Jul-1999: Modified for GMOS. Removed CICS-specific stuff.        (smb)
*/
/* *INDENT-OFF* */
/*
 * $Log$
 * Revision 1.7  2001/02/23 16:33:52  gmos
 * Removed ancient and misleading BUG WORK AROUND messages.
 *
 * Revision 1.6  2000/12/19 13:30:24  gmos
 * Allow the DEBUG command to accept the QUIET string.
 *
 * Revision 1.5  2000/07/25 11:09:48  gmos
 * Modified to work around poor design feature of CAD record, in which outputs are triggered for every directive.
 *
 * Revision 1.4  2000/06/06 13:33:07  gmos
 * DEBUG and SIMULATE commands now set global debug and simulate variables
 *
 * Revision 1.3  2000/04/27 13:54:12  gmos
 * Disable printf statements
 *
 * Revision 1.2  2000/03/14 17:02:49  gmos
 * UKATC logo added
 *
 * Revision 1.1  1999/08/02 14:22:53  gmos
 * First version
 *
 */
/* *INDENT-ON* */

#include  <vxWorks.h>
#include  <semLib.h>
#include  <types.h>
#include  <math.h>
#include  <time.h>
#include  <stdlib.h>
#include  <stdioLib.h>
#include  <string.h>
#include  <logLib.h>

#include  <dbDefs.h>
#include  <dbEvent.h>
#include  <cadRecord.h>
#include  <dbCommon.h>
#include  <recSup.h>
#include  <cad.h>
#include  <car.h>

#include  "recAssControl.h"

#include  "gmosCcCadLib.h"

/* #define   VERBOSE */              /* Define this for verbose messages */

/* Global veriables from external modules */

extern int drvOmsVmeDebug;
extern int devBarcodeDebug;
extern int devBarcodeSimulate;
extern int lifterDebug;

/* Global variables */

SEM_ID    gmosCadModeSem = NULL;   /* Semaphore to protect MODE fields from simultaneous access */

/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadModeInit
 *
 *   Purpose:
 *   Initialise mode handling CAD record
 *
 *   Description:
 *   This routine initialises the outputs of a mode handling CAD record to
 *   sensible values.
 *
 *   Invocation:
 *   status = gmosCadModeInit( struct cadRecord *pcad )
 *
 *   Parameters in:
 *
 *   Parameters out:
 *      < pcad-vala   long        Initial mode
 *      < pcad->mess  string      failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure. It is also assumed that input B of the CAD record has been
 *   connected to the "applyC.IVAL" field of the instrument "applyC" record
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   06-Jun-1996: Original version as CADreboot.                 (smb)
 *   12-Aug-1997: Modified into general purpose function, using
 *                pcad->name to determine name of command.       (smb)
 *   03-Jul-1999: GMOS version.                                  (smb)
 *-
 */

long gmosCadModeInit( struct cadRecord *pcad ) 
{
    long status;                         /* return status */

#ifdef VERBOSE
    printf( "gmosCadModeInit: %s\n", pcad->name );
#endif

/* Initialise CAD status */

    status = CAD_ACCEPT;

/* Create the global MODE semaphore, if not already created. */

    if ( gmosCadModeSem == NULL )
    {
        gmosCadModeSem = semMCreate ( SEM_Q_FIFO | SEM_DELETE_SAFE );
        if ( gmosCadModeSem == NULL )
        {
            printf ("gmosCadModeInit: Failed to create gmosCadModeSem semaphore\n");
            status = CAD_REJECT;
        }
    }

/* Initialise the MODE output to INIT. */

    /* status = gmosCadModeFanout( DAR_MODE_INIT, pcad ); */

    return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadInit
 *
 *   Purpose:
 *   Function for init CAD record
 *
 *   Description:
 *   This routine is called whenever the a CAD record is processed for the
 *   init command. The command cannot be started while the
 *   instrument is BUSY and the STOP directive is meaningless.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   Invocation:
 *   status = gmosCadInit( struct cadRecord *pcad )
 *
 *   Parameters in:
 *      > pcad->dir   string      CAD directive
 *      > pcad->b     string      Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CAR_BUSY.
 *
 *   Parameters out:
 *      < pcad->mess  string      failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure. It is also assumed that input B of the CAD record has been
 *   connected to the "applyC.IVAL" field of the instrument "applyC" record
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   06-Jun-1996: Original version as CADreboot.                 (smb)
 *   12-Aug-1997: Modified into general purpose function, using
 *                pcad->name to determine name of command.       (smb)
 *   03-Jul-1999: GMOS version.                                  (smb)
 *-
 */

long gmosCadInit( struct cadRecord *pcad ) 
{
  long status;                         /* return status */
  char message[MAX_STRING_SIZE];       /* descriptive message */

#ifdef VERBOSE
  printf( "gmosCadInit: %s %d\n", pcad->name, pcad->dir );
#endif

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/*
 * CAD MARK directive detected. Fan out an "INIT" MODE directive to any attached
 * records.
 */

       status = gmosCadModeFanout( DAR_MODE_INIT, pcad );
       break;

    case CAD_PRESET:

/*
 * CAD PRESET directive detected. There are no command-specific attributes
 * to be checked. Reject the command if the second attribute (B) indicates the
 * instrument is busy. It is assumed that if the command is successful the MODE
 * directives will be forwarded to the assembly records.
 */

      if ( *(long *)pcad->b == CAR_BUSY )
      {
          strncpy( pcad->mess, "Instrument is busy", MAX_STRING_SIZE-1 );
          status = CAD_REJECT;
      }
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      break;

    case CAD_START:

/*
 * CAD START directive detected. No processing required. It is assumed the CAD record
 * will trigger further processing in the EPICS database.
 */
      break;

    case CAD_STOP:

/*
 * CAD STOP directive detected. It is not possible to stop this kind of command
 * once it has started, so reject the directive.
 */
      strncpy( pcad->mess, "Cannot be stopped", MAX_STRING_SIZE-1 );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      sprintf( message, "Unrecognised CAD directive - %4d", pcad->dir );
      strncpy( pcad->mess, message, MAX_STRING_SIZE-1 );
      status = CAD_REJECT;
      break;

  }
  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadTest
 *
 *   Purpose:
 *   Function for test CAD record
 *
 *   Description:
 *   This routine is called whenever the a CAD record is processed for the
 *   test command. The command cannot be started while the
 *   instrument is BUSY and the STOP directive is meaningless.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   Invocation:
 *   status = gmosCadTest( struct cadRecord *pcad )
 *
 *   Parameters in:
 *      > pcad->dir   string      CAD directive
 *      > pcad->b     string      Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CAR_BUSY.
 *
 *   Parameters out:
 *      < pcad->mess  string      failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure. It is also assumed that input B of the CAD record has been
 *   connected to the "applyC.IVAL" field of the instrument "applyC" record
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   06-Jun-1996: Original version as CADreboot.                 (smb)
 *   12-Aug-1997: Modified into general purpose function, using
 *                pcad->name to determine name of command.       (smb)
 *   03-Jul-1999: GMOS version.                                  (smb)
 *-
 */

long gmosCadTest( struct cadRecord *pcad ) 
{
  long status;                         /* return status */
  char message[MAX_STRING_SIZE];       /* descriptive message */

#ifdef VERBOSE
  printf( "gmosCadTest: %s %d\n", pcad->name, pcad->dir );
#endif

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/*
 * CAD MARK directive detected. Fan out a "TEST" MODE directive to any attached
 * records.
 */
       status = gmosCadModeFanout ( DAR_MODE_TEST, pcad );
       break;

    case CAD_PRESET:

/*
 * CAD PRESET directive detected. There are no command-specific attributes
 * to be checked. Reject the command if the second attribute (B) indicates the
 * instrument is busy. It is assumed that if the command is successful the MODE
 * directives will be forwarded to the assembly records.
 */

      if ( *(long *)pcad->b == CAR_BUSY )
      {
          strncpy( pcad->mess, "Instrument is busy", MAX_STRING_SIZE-1 );
          status = CAD_REJECT;
      }
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      break;

    case CAD_START:

/*
 * CAD START directive detected. No processing required. It is assumed the CAD record
 * will trigger further processing in the EPICS database.
 */
      break;

    case CAD_STOP:

/*
 * CAD STOP directive detected. It is not possible to stop this kind of command
 * once it has started, so reject the directive.
 */
      strncpy( pcad->mess, "Cannot be stopped", MAX_STRING_SIZE-1 );
      status = CAD_REJECT;
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      sprintf( message, "Unrecognised CAD directive - %4d", pcad->dir );
      strncpy( pcad->mess, message, MAX_STRING_SIZE-1 );
      status = CAD_REJECT;
      break;

  }
  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadDatum
 *
 *   Purpose:
 *   Function for datum CAD record
 *
 *   Description:
 *   This routine is called whenever the a CAD record is processed for the
 *   datum command. It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   Example commands: test, reboot
 *
 *   Invocation:
 *   status = gmosCadDatum( struct cadRecord *pcad )
 *
 *   Parameters in:
 *      > pcad->dir   string      CAD directive
 *      > pcad->b     string      Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CAR_BUSY.
 *
 *   Parameters out:
 *      < pcad->mess  string      failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure. It is also assumed that input B of the CAD record has been
 *   connected to the "applyC.IVAL" field of the instrument "applyC" record
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   06-Jun-1996: Original version as CADreboot.                 (smb)
 *   12-Aug-1997: Modified into general purpose function, using
 *                pcad->name to determine name of command.       (smb)
 *   03-Jul-1999: GMOS version.                                  (smb)
 *-
 */

long gmosCadDatum( struct cadRecord *pcad ) 
{
  long status;                         /* return status */
  char message[MAX_STRING_SIZE];       /* descriptive message */

#ifdef VERBOSE
  printf( "gmosCadDatum: %s %d\n", pcad->name, pcad->dir );
#endif

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/*
 * CAD MARK directive detected. Fan out an "INDEX" MODE directive to any attached
 * records.
 */
      status = gmosCadModeFanout ( DAR_MODE_INDEX, pcad );
      break;

    case CAD_PRESET:

/*
 * CAD PRESET directive detected. There are no command-specific attributes
 * to be checked. It is assumed that if the command is successful the MODE
 * directives will be forwarded to the assembly records.
 */

      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      break;

    case CAD_START:

/*
 * CAD START directive detected. No processing required. It is assumed the CAD record
 * will trigger further processing in the EPICS database.
 */
      break;

    case CAD_STOP:

/*
 * CAD STOP directive detected. No processing required. It is assumed the CAD record
 * will trigger further processing in the EPICS database.
 */
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      sprintf( message, "Unrecognised CAD directive - %4d", pcad->dir );
      strncpy( pcad->mess, message, MAX_STRING_SIZE-1 );
      status = CAD_REJECT;
      break;

  }
  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadUpdate
 *
 *   Purpose:
 *   Function for update CAD record
 *
 *   Description:
 *   This routine is called whenever the a CAD record is processed for the
 *   update command. The command cannot be started while the
 *   instrument is BUSY.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   Example commands: test, reboot
 *
 *   Invocation:
 *   status = gmosCadUpdate( struct cadRecord *pcad )
 *
 *   Parameters in:
 *      > pcad->dir   string      CAD directive
 *      > pcad->b     string      Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CAR_BUSY.
 *
 *   Parameters out:
 *      < pcad->mess  string      failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure. It is also assumed that input B of the CAD record has been
 *   connected to the "applyC.IVAL" field of the instrument "applyC" record
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   06-Jun-1996: Original version as CADreboot.                 (smb)
 *   12-Aug-1997: Modified into general purpose function, using
 *                pcad->name to determine name of command.       (smb)
 *   03-Jul-1999: GMOS version.                                  (smb)
 *-
 */

long gmosCadUpdate( struct cadRecord *pcad ) 
{
  long status;                         /* return status */
  char message[MAX_STRING_SIZE];       /* descriptive message */

#ifdef VERBOSE
  printf( "gmosCadUpdate: %s %d\n", pcad->name, pcad->dir );
#endif

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/*
 * CAD MARK directive detected. Fan out an "UPDATE" MODE directive to any attached
 * records.
 */
      status = gmosCadModeFanout ( DAR_MODE_UPDATE, pcad );
      break;

    case CAD_PRESET:

/*
 * CAD PRESET directive detected. There are no command-specific attributes
 * to be checked. Reject the command if the second attribute (B) indicates the
 * instrument is busy. It is assumed that if the command is successful the MODE
 * directives will be forwarded to the assembly records.
 */

      if ( *(long *)pcad->b == CAR_BUSY )
      {
          strncpy( pcad->mess, "Instrument is busy", MAX_STRING_SIZE-1 );
          status = CAD_REJECT;
      }
      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      break;

    case CAD_START:

/*
 * CAD START directive detected. No processing required. It is assumed the CAD record
 * will trigger further processing in the EPICS database.
 */
      break;

    case CAD_STOP:

/*
 * CAD STOP directive detected. No processing required. It is assumed the CAD record
 * will trigger further processing in the EPICS database.
 */
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      sprintf( message, "Unrecognised CAD directive - %4d", pcad->dir );
      strncpy( pcad->mess, message, MAX_STRING_SIZE-1 );
      status = CAD_REJECT;
      break;

  }
  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadPark
 *
 *   Purpose:
 *   Function for park CAD record
 *
 *   Description:
 *   This routine is called whenever the a CAD record is processed for the
 *   init command. It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *
 *   Example commands: test, reboot
 *
 *   Invocation:
 *   status = gmosCadPark( struct cadRecord *pcad )
 *
 *   Parameters in:
 *      > pcad->dir   string      CAD directive
 *      > pcad->b     string      Instrument busy flag. The command will be
 *                                rejected if this flag happens to be exactly
 *                                equal to CAR_BUSY.
 *
 *   Parameters out:
 *      < pcad->mess  string      failure message written to CAD MESS field
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure. It is also assumed that input B of the CAD record has been
 *   connected to the "applyC.IVAL" field of the instrument "applyC" record
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   06-Jun-1996: Original version as CADreboot.                 (smb)
 *   12-Aug-1997: Modified into general purpose function, using
 *                pcad->name to determine name of command.       (smb)
 *   03-Jul-1999: GMOS version.                                  (smb)
 *-
 */

long gmosCadPark( struct cadRecord *pcad ) 
{
  long status;                         /* return status */
  char message[MAX_STRING_SIZE];       /* descriptive message */

#ifdef VERBOSE
  printf( "gmosCadPark: %s %d\n", pcad->name, pcad->dir );
#endif

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/*
 * CAD MARK directive detected. Fan out a "PARK" MODE directive to any attached
 * records.
 */
      status = gmosCadModeFanout ( DAR_MODE_PARK, pcad );
      break;

    case CAD_PRESET:

/*
 * CAD PRESET directive detected. There are no command-specific attributes
 * to be checked. It is assumed that if the command is successful the MODE
 * directives will be forwarded to the assembly records.
 */

      break;

    case CAD_CLEAR:

/* CAD CLEAR directive detected. */

      break;

    case CAD_START:

/*
 * CAD START directive detected. No processing required. It is assumed the CAD record
 * will trigger further processing in the EPICS database.
 */
      break;

    case CAD_STOP:

/*
 * CAD STOP directive detected. No processing required. It is assumed the CAD record
 * will trigger further processing in the EPICS database.
 */
      break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

      sprintf( message, "Unrecognised CAD directive - %4d", pcad->dir );
      strncpy( pcad->mess, message, MAX_STRING_SIZE-1 );
      status = CAD_REJECT;
      break;

  }
  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadDebug
 *
 *   Purpose:
 *   User defined function for "debug" CAD record
 *
 *   Description:
 *   This routine is called whenever the debug CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *   If a valid debug mode is given the mode is fanned out.
 *
 *   Invocation:
 *   status = gmosCadDebug( struct cadRecord *pcad )
 *
 *   Parameters in:
 *      > pcad->dir   string      CAD directive
 *      > pcad->a     string      CAD input attribute A
 *                                = Requested debug mode
 *
 *   Parameters out:
 *      < pcad->mess  string      failure message written to CAD MESS field
 *      < pcad->vala  long        Debug mode - 1
 *      < pcad->valb  long        Debug mode - 2
 *      < pcad->valc  long        Debug mode - 3
 *      < pcad->vald  long        Debug mode - 4
 *      < pcad->vale  long        Debug mode - 5
 *      < pcad->valf  long        Debug mode - 6
 *      < pcad->valg  long        Debug mode - 7
 *      < pcad->valh  long        Debug mode - 8
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   17-Mar-1997: Original version.                        (smb)
 *   09-Jul-1999: GMOS version.                            (smb)
 *-
 */

long gmosCadDebug( struct cadRecord *pcad ) 
{
  long status;                         /* return status */
  char message[MAX_STRING_SIZE];       /* descriptive message */

#ifdef VERBOSE
  printf( "gmosCadDebug: %s %d %s\n", pcad->name, pcad->dir, (char *)pcad->a );
#endif

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/*
 * CAD MARK directive detected. Nothing needs to be done.
 */
       break;

    case CAD_PRESET:

/*
 * CAD PRESET directive detected. Check the debug mode is recognised
 * and fan out the selected debug mode to VALA, etc...
 */

       if ( strcmp( (char *)pcad->a, "QUIET" ) == 0 )
       {
          status = gmosCadModeFanout( DAR_DBUG_QUIET, pcad );
       }
       else if ( strcmp( (char *)pcad->a, "NONE" ) == 0 )
       {
          status = gmosCadModeFanout( DAR_DBUG_NONE, pcad );
       }
       else if ( strcmp( (char *)pcad->a, "MIN" ) == 0 )
       {
           status = gmosCadModeFanout( DAR_DBUG_MIN, pcad );
       }
       else if ( strcmp( (char *)pcad->a, "FULL" ) == 0 )
       {
          status = gmosCadModeFanout( DAR_DBUG_FULL, pcad );
       }
       else if ( strcmp( (char *)pcad->a, "MAX" ) == 0 )
       {
          status = gmosCadModeFanout( DAR_DBUG_MAX, pcad );
       }
       else
       {
           sprintf( message, "Unrecognised debug mode - %.8s", (char *)pcad->a );
           strncpy( pcad->mess, message, MAX_STRING_SIZE-1 );
           status = CAD_REJECT;
       }

       break;

    case CAD_CLEAR:

/*
 * CAD CLEAR directive detected. Nothing needs to be done.
 */

       break;

    case CAD_START:

/*
 * CAD START directive detected. Set global variables to switch GMOS utilities into debugging mode.
  */

       if ( strcmp( (char *)pcad->a, "NONE" ) == 0)
       {
           drvOmsVmeDebug = 0;
           devBarcodeDebug = 0;		/* BAR_DBUG_NONE    */
           lifterDebug = 0;		/* LIFTER_DBUG_NONE */
       }
       else if ( strcmp( (char *)pcad->a, "MIN" ) == 0)
       {
           drvOmsVmeDebug = 1;
           devBarcodeDebug = 0;		/* BAR_DBUG_NONE    */
           lifterDebug = 0;		/* LIFTER_DBUG_NONE */
       }
       else
       {
           drvOmsVmeDebug = 1;
           devBarcodeDebug = 1;		/* BAR_DBUG_MIN    */
           lifterDebug = 1;		/* LIFTER_DBUG_MIN */
       }
       break;

    case CAD_STOP:

/*
 * CAD STOP directive detected. The debug command completes immediately,
 * so the STOP directive is meaningless
 */

       strncpy( pcad->mess, "Cannot be stopped", MAX_STRING_SIZE-1 );
       status = CAD_REJECT;
       break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

       sprintf( message, "Unrecognised CAD directive - %4d", pcad->dir );
       strncpy( pcad->mess, message, MAX_STRING_SIZE-1 );
       status = CAD_REJECT;
       break;
  }

  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadSimulate
 *
 *   Purpose:
 *   User defined function for "simulate" CAD record
 *
 *   Description:
 *   This routine is called whenever the simulate CAD record is processed.
 *   It checks that the command is acceptable and returns a status and a
 *   message, which are written to the VAL and MESS fields of the CAD record.
 *   If a valid simulation mode is given the mode is fanned out.
 *
 *   Invocation:
 *   status = gmosCadSimulate( struct cadRecord *pcad )
 *
 *   Parameters in:
 *      > pcad->dir   string      CAD directive
 *      > pcad->a     string      CAD input attribute A
 *                                = Requested simulation mode
 *
 *   Parameters out:
 *      < pcad->mess  string      failure message written to CAD MESS field
 *      < pcad->vala  long        Simulation mode - 1
 *      < pcad->valb  long        Simulation mode - 2
 *      < pcad->valc  long        Simulation mode - 3
 *      < pcad->vald  long        Simulation mode - 4
 *      < pcad->vale  long        Simulation mode - 5
 *      < pcad->valf  long        Simulation mode - 6
 *      < pcad->valg  long        Simulation mode - 7
 *      < pcad->valh  long        Simulation mode - 8
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   09-Jul-1999: GMOS version.                            (smb)
 *-
 */

long gmosCadSimulate( struct cadRecord *pcad ) 
{
  long status;                         /* return status */
  char message[MAX_STRING_SIZE];       /* descriptive message */

#ifdef VERBOSE
  printf( "gmosCadSimulate: %s %d %s\n", pcad->name, pcad->dir, (char *)pcad->a );
#endif

/* Initialise CAD status */

  status = CAD_ACCEPT;

/* Switch according to the CAD directive in DIR field */

  switch (pcad->dir)
  {
    case CAD_MARK:

/*
 * CAD MARK directive detected. Nothing needs to be done.
 */
       break;

    case CAD_PRESET:

/*
 * CAD PRESET directive detected. Check the simulation mode is recognised
 * and fan out the selected simulation mode to VALA, etc...
 */

       if ( strcmp( (char *)pcad->a, "NONE" ) == 0 )
       {
          status = gmosCadModeFanout( DAR_SIM_NONE, pcad );
       }
       else if ( strcmp( (char *)pcad->a, "VSM" ) == 0 )
       {
          status = gmosCadModeFanout( DAR_SIM_VSM, pcad );
       }
       else if ( strcmp( (char *)pcad->a, "FAST" ) == 0 )
       {
          status = gmosCadModeFanout( DAR_SIM_FAST, pcad );
       }
       else if ( strcmp( (char *)pcad->a, "FULL" ) == 0 )
       {
          status = gmosCadModeFanout( DAR_SIM_FULL, pcad );
       }
       else
       {
           sprintf( message, "Unrecognised simulation mode - %.8s", (char *)pcad->a );
           strncpy( pcad->mess, message, MAX_STRING_SIZE-1 );
           status = CAD_REJECT;
       }
       break;

    case CAD_CLEAR:

/*
 * CAD CLEAR directive detected. Nothing needs to be done.
 */

       break;

    case CAD_START:

/*
 * CAD START directive detected. Set global variables to switch GMOS utilities into simulation mode.
  * Issue a warning because this CAD record will not effect the simulation of XYCOM records
  * or the grating lifter.
  */

       printf ("WARNING: Simulation mode will not be set properly for XYCOM records and grating lifter.\n");


       if ( strcmp( (char *)pcad->a, "NONE" ) == 0)
       {
           devBarcodeSimulate = 0;	/* BAR_SIM_NONE    */
       }
       else
       {
           devBarcodeSimulate = 3;	/* BAR_SIM_FULL    */
       }
       break;

    case CAD_STOP:

/*
 * CAD STOP directive detected. The simulation command completes immediately,
 * so the STOP directive is meaningless
 */

       strncpy( pcad->mess, "Cannot be stopped", MAX_STRING_SIZE-1 );
       status = CAD_REJECT;
       break;

    default:

/* Unrecognised CAD directive detected. This is regarded as an error. */

       sprintf( message, "Unrecognised CAD directive - %4d", pcad->dir );
       strncpy( pcad->mess, message, MAX_STRING_SIZE-1 );
       status = CAD_REJECT;
       break;
  }

  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadAttributeFanout
 *
 *   Purpose:
 *   Fan out attribute A to VALA, VALB, VALC, etc...
 *
 *   Description:
 *   This routine is fans out the contents of attribute A to up to eight
 *   CAD outputs at once. It can be used by commands such as "debug" and
 *   "simulate" which fan out a debug or simulation mode to other records.
 *
 *   Invocation:
 *   status = gmosCadAttributeFanout( struct cadRecord *pcad )
 *
 *   Parameters in:
 *      > pcad->a     string      CAD input attribute A
 *      > pcad->ctyp  short       Number of outputs
 *      > pcad->ftva  ushort      Type of attribute VALA
 *      > pcad->ftvb  ushort      Type of attribute VALB
 *      etc...
 *
 *   Parameters out:
 *      < pcad->vala  (ftva)      Output attribute - VALA
 *      < pcad->valb  (ftvb)      Output attribute - VALB
 *      etc...
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure
 *
 *   Limitations:
 *   Only 8 outputs are dealt with at the moment, although a CAD record can have
 *   up to 20.
 *
 *   Bugs:
 *   Checking the output type with FTVA, FTVB etc does not work. The type
 *   claims to be STRING no matter what is defined in the database.
 *   I have no idea why - SMB.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   09-Jul-1999: Original version.                    (smb)
 *-
 */

long gmosCadAttributeFanout( struct cadRecord *pcad ) 
{
    long status;                         /* return status */
    int  size;                           /* Number of output attributes */
    int  nconv;                          /* Number of successful conversions */

#ifdef VERBOSE
  printf( "gmosCadAttributeFanout: %s ctyp=%d A=%s, types=%d %d %d %d %d %d %d %d\n",
          pcad->name, pcad->ctyp, (char *)pcad->a, pcad->ftva, pcad->ftvb, pcad->ftvc, pcad->ftvd,
          pcad->ftve, pcad->ftvf, pcad->ftvg, pcad->ftvh );
#endif

/* Initialise CAD status */

    status = CAD_ACCEPT;

/* Determine the number of output attributes */

    size = (int) pcad->ctyp;

/* Copy at least the first two 2 attributes. Switch according to the output attribute type. */

    if ( pcad->ftva == DBF_STRING )
    {
        strncpy ( (char *)pcad->vala, (char *)pcad->a, MAX_STRING_SIZE-1 );
    }
    else if ( pcad->ftva == DBF_LONG )
    {
        nconv = sscanf ( (char *)pcad->a, "%ld", (long *)pcad->vala );
        if ( nconv != 1 )
        {
            strncpy ( pcad->mess, "Conversion failure for VALA", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }
    }
    else if ( pcad->ftva == DBF_DOUBLE  )
    {
        nconv = sscanf ( (char *)pcad->a, "%lf", (double *)pcad->vala );
        if ( nconv != 1 )
        {
            strncpy ( pcad->mess, "Conversion failure for VALA", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }
    }
    else
    {
        strncpy ( pcad->mess, "Unrecognised type for VALA", MAX_STRING_SIZE-1 );
        status = CAD_REJECT;
    }

    if ( pcad->ftvb == DBF_STRING )
    {
        strncpy ( (char *)pcad->valb, (char *)pcad->a, MAX_STRING_SIZE-1 );
    }
    else if ( pcad->ftvb == DBF_LONG )
    {
        nconv = sscanf ( (char *)pcad->a, "%ld", (long *)pcad->valb );
        if ( nconv != 1 )
        {
            strncpy ( pcad->mess, "Conversion failure for VALB", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }
    }
    else if ( pcad->ftvb == DBF_DOUBLE  )
    {
        nconv = sscanf ( (char *)pcad->a, "%lf", (double *)pcad->valb );
        if ( nconv != 1 )
        {
            strncpy ( pcad->mess, "Conversion failure for VALB", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }
    }
    else
    {
        strncpy ( pcad->mess, "Unrecognised type for VALB", MAX_STRING_SIZE-1 );
        status = CAD_REJECT;
    }

/* Copy the next two attributes if this is a CAD4 or larger */

    if ( size >= 4 )
    {
        if ( pcad->ftvc == DBF_STRING )
        {
            strncpy ( (char *)pcad->valc, (char *)pcad->a, MAX_STRING_SIZE-1 );
        }
        else if ( pcad->ftvc == DBF_LONG )
        {
            nconv = sscanf ( (char *)pcad->a, "%ld", (long *)pcad->valc );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALB", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else if ( pcad->ftvc == DBF_DOUBLE  )
        {
            nconv = sscanf ( (char *)pcad->a, "%lf", (double *)pcad->valc );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALC", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else
        {
            strncpy ( pcad->mess, "Unrecognised type for VALC", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }

        if ( pcad->ftvd == DBF_STRING )
        {
            strncpy ( (char *)pcad->vald, (char *)pcad->a, MAX_STRING_SIZE-1 );
        }
        else if ( pcad->ftvd == DBF_LONG )
        {
            nconv = sscanf ( (char *)pcad->a, "%ld", (long *)pcad->vald );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALB", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else if ( pcad->ftvd == DBF_DOUBLE  )
        {
            nconv = sscanf ( (char *)pcad->a, "%lf", (double *)pcad->vald );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALD", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else
        {
            strncpy ( pcad->mess, "Unrecognised type for VALD", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }
    }

/* Copy the next four attributes if this is a CAD8 or larger */

    if ( size >= 8 )
    {
        if ( pcad->ftve == DBF_STRING )
        {
            strncpy ( (char *)pcad->vale, (char *)pcad->a, MAX_STRING_SIZE-1 );
        }
        else if ( pcad->ftve == DBF_LONG )
        {
            nconv = sscanf ( (char *)pcad->a, "%ld", (long *)pcad->vale );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALE", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else if ( pcad->ftvc == DBF_DOUBLE  )
        {
            nconv = sscanf ( (char *)pcad->a, "%lf", (double *)pcad->vale );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALE", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else
        {
            strncpy ( pcad->mess, "Unrecognised type for VALE", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }

        if ( pcad->ftvd == DBF_STRING )
        {
            strncpy ( (char *)pcad->valf, (char *)pcad->a, MAX_STRING_SIZE-1 );
        }
        else if ( pcad->ftvf == DBF_LONG )
        {
            nconv = sscanf ( (char *)pcad->a, "%ld", (long *)pcad->valf );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALF", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else if ( pcad->ftvf == DBF_DOUBLE  )
        {
            nconv = sscanf ( (char *)pcad->a, "%lf", (double *)pcad->valf );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALF", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else
        {
            strncpy ( pcad->mess, "Unrecognised type for VALF", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }

        if ( pcad->ftvg == DBF_STRING )
        {
            strncpy ( (char *)pcad->valg, (char *)pcad->a, MAX_STRING_SIZE-1 );
        }
        else if ( pcad->ftvg == DBF_LONG )
        {
            nconv = sscanf ( (char *)pcad->a, "%ld", (long *)pcad->valg );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALG", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else if ( pcad->ftvg == DBF_DOUBLE  )
        {
            nconv = sscanf ( (char *)pcad->a, "%lf", (double *)pcad->valg );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALG", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else
        {
            strncpy ( pcad->mess, "Unrecognised type for VALG", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }

        if ( pcad->ftvh == DBF_STRING )
        {
            strncpy ( (char *)pcad->valf, (char *)pcad->a, MAX_STRING_SIZE-1 );
        }
        else if ( pcad->ftvh == DBF_LONG )
        {
            nconv = sscanf ( (char *)pcad->a, "%ld", (long *)pcad->valh );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALH", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else if ( pcad->ftvh == DBF_DOUBLE  )
        {
            nconv = sscanf ( (char *)pcad->a, "%lf", (double *)pcad->valh );
            if ( nconv != 1 )
            {
                strncpy ( pcad->mess, "Conversion failure for VALH", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
        else
        {
            strncpy ( pcad->mess, "Unrecognised type for VALH", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }
    }

/* Apologise if there are more than 8 outputs. */

    if ( size > 8 )
    {
        logMsg( "gmosCadAttributeFanout:%s - Sorry, only 8 outputs are fanned out.\n",
          (int) pcad->name, 0, 0, 0, 0, 0);
    }

    return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadDirFanout
 *
 *   Purpose:
 *   Fan out CAD directive VALA, VALB, VALC, etc...
 *
 *   Description:
 *   This routine fans out the given directive to up to eight
 *   CAD outputs at once. It can be used, for example, by commands
 *   don't have any attributes which need to mark other CAD records
 *   when executed.
 *
 *   Invocation:
 *   status = gmosCadDirFanout( const char * dir, struct cadRecord *pcad )
 *
 *   Parameters in:
 *      > dir         string      CAD directive encoded as a string
 *      > pcad->ctyp  short       Number of outputs
 *
 *   Parameters out:
 *      < pcad->vala  string      Output directive - VALA
 *      etc...
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure. All the output attributes must be of type STRING.
 *
 *   Limitations:
 *   Only 8 outputs are dealt with at the moment, although a CAD record can have
 *   up to 20.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   09-Jul-1999: Original version.                    (smb)
 *-
 */

long gmosCadDirFanout( const char * dir, struct cadRecord *pcad ) 
{
    long status;                         /* return status */
    int  size;                           /* Number of output attributes */

#ifdef VERBOSE
  printf( "gmosCadDirFanout: %s size=%d dir=%s, types=%d %d %d %d %d %d %d %d\n",
          pcad->name, pcad->ctyp, dir, pcad->ftva, pcad->ftvb, pcad->ftvc, pcad->ftvd,
          pcad->ftve, pcad->ftvf, pcad->ftvg, pcad->ftvh );
#endif

/* Initialise CAD status */

    status = CAD_ACCEPT;

/* Determine the number of output attributes */

    size = (int) pcad->ctyp;

/* Copy at least to the first two outputs, but only if the they are of STRING type. */

    if ( pcad->vala != NULL )
    {
        if ( pcad->ftva == DBF_STRING )
        {
            strncpy ( (char *)pcad->vala, dir, MAX_STRING_SIZE-1 );
        }
        else
        {
            strncpy ( pcad->mess, "VALA not STRING", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }
    }
    else
    {
        /* Only report an error if the first output is not connected. */

        strncpy ( pcad->mess, "NULL pointer for VALA", MAX_STRING_SIZE-1 );
        status = CAD_REJECT;
    }

    if ( pcad->valb != NULL )
    {
        if ( pcad->ftvb == DBF_STRING )
        {
            strncpy ( (char *)pcad->valb, dir, MAX_STRING_SIZE-1 );
        }
        else
        {
            strncpy ( pcad->mess, "VALB not STRING", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }
    }

/* Copy the next two attributes if this is a CAD4 or larger */

    if ( size >= 4 )
    {
        if ( pcad->valc != NULL )
        {
            if ( pcad->ftvc == DBF_STRING )
            {
                strncpy ( (char *)pcad->valc, dir, MAX_STRING_SIZE-1 );
            }
            else
            {
                strncpy ( pcad->mess, "VALC not STRING", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }

        if ( pcad->vald != NULL )
        {
            if ( pcad->ftvd == DBF_STRING )
            {
                strncpy ( (char *)pcad->vald, dir, MAX_STRING_SIZE-1 );
            }
            else
            {
                strncpy ( pcad->mess, "VALD not STRING", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
    }

/* Copy the next four attributes if this is a CAD8 or larger */

    if ( size >= 8 )
    {
        if ( pcad->vale != NULL )
        {
            if ( pcad->ftve == DBF_STRING )
            {
                strncpy ( (char *)pcad->vale, dir, MAX_STRING_SIZE-1 );
            }
            else
            {
                strncpy ( pcad->mess, "VALE not STRING", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }

        if ( pcad->valf != NULL )
        {
            if ( pcad->ftvf == DBF_STRING )
            {
                strncpy ( (char *)pcad->valf, dir, MAX_STRING_SIZE-1 );
            }
            else
            {
                strncpy ( pcad->mess, "VALF not STRING", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }

        if ( pcad->valg != NULL )
        {
            if ( pcad->ftvg == DBF_STRING )
            {
                strncpy ( (char *)pcad->valg, dir, MAX_STRING_SIZE-1 );
            }
            else
            {
                strncpy ( pcad->mess, "VALG not STRING", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }

        if ( pcad->valh != NULL )
        {
            if ( pcad->ftvh == DBF_STRING )
            {
                strncpy ( (char *)pcad->valf, dir, MAX_STRING_SIZE-1 );
            }
            else
            {
                strncpy ( pcad->mess, "VALH not STRING", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
    }

/* Apologise if there are more than 8 outputs. */

    if ( size > 8 )
    {
        logMsg( "gmosCadDirFanout:%s - Sorry, only 8 directives are fanned out.\n",
          (int) pcad->name, 0, 0, 0, 0, 0);
    }

    return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCadModeFanout
 *
 *   Purpose:
 *   Fan out mode to VALA, VALB, VALC, etc...
 *
 *   Description:
 *   This routine is fans out the given mode to up to eight
 *   CAD outputs at once. It can be used by commands such as "init" and
 *   "datum" which fan out an INIT or INDEX mode to several assembly
 *   records. The output must be integer.
 *
 *   Invocation:
 *   status = gmosCadModeFanout( const int mode, struct cadRecord *pcad )
 *
 *   Parameters in:
 *      > mode        string      Mode required.
 *      > pcad->a     string      CAD input attribute A
 *      > pcad->ctyp  short       Number of outputs
 *      > pcad->ftva  ushort      Type of attribute VALA
 *      > pcad->ftvb  ushort      Type of attribute VALB
 *      etc...
 *
 *   Parameters out:
 *      < pcad->vala  (long)      Output attribute - VALA
 *      < pcad->valb  (long)      Output attribute - VALB
 *      etc...
 *
 *   Return value:
 *      < status      long        Status value written to CAD VAL field
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   It is assumed the CAD record has already been initialised and the
 *   directive and any attributes have already been assembled into the pcad
 *   structure
 *
 *   Limitations:
 *   Only 8 outputs are dealt with at the moment, although a CAD record can have
 *   up to 20.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   09-Jul-1999: Original version.                    (smb)
 *-
 */

long gmosCadModeFanout( const long mode, struct cadRecord *pcad ) 
{
    long status;                         /* return status */
    int  size;                           /* Number of output attributes */

#ifdef VERBOSE
  printf( "gmosCadModeFanout: %s size=%d mode=%d types=%d %d %d %d %d %d %d %d\n",
          pcad->name, pcad->ctyp, mode, pcad->ftva, pcad->ftvb, pcad->ftvc, pcad->ftvd,
          pcad->ftve, pcad->ftvf, pcad->ftvg, pcad->ftvh );
#endif

/* Initialise CAD status */

    status = CAD_ACCEPT;

/* Determine the number of output attributes */

    size = (int) pcad->ctyp;

/*
 * Take the mode semaphore before fanning out the mode. This protects against this function
 * being executed more than once simultaneously in separate threads.
 */

    if ( semTake( gmosCadModeSem, WAIT_FOREVER ) == ERROR )
    {
        strncpy ( pcad->mess, "Failed to take gmosCadModeSem semaphore", MAX_STRING_SIZE-1 );
        status = CAD_REJECT;
        return status;
    }

/* Copy at least the first two 2 attributes. Switch according to the output attribute type. */

    if ( pcad->vala != NULL )
    {
        if ( pcad->ftva == DBF_LONG )
        {
            *(long *)pcad->vala = mode;
        }
        else
        {
            strncpy ( pcad->mess, "Invalid type for VALA", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }
    }
    else
    {
        /* Only report an error if the first output is not connected. */

        strncpy ( pcad->mess, "NULL pointer for VALA", MAX_STRING_SIZE-1 );
        status = CAD_REJECT;
    }

    if ( pcad->valb != NULL )
    {
        if ( pcad->ftvb == DBF_LONG )
        {
            *(long *)pcad->valb = mode;
        }
        else
        {
            strncpy ( pcad->mess, "Invalid type for VALB", MAX_STRING_SIZE-1 );
            status = CAD_REJECT;
        }
    }

/* Copy the next two attributes if this is a CAD4 or larger */

    if ( size >= 4 )
    {
        if ( pcad->valc != NULL )
        {
            if ( pcad->ftvc == DBF_LONG )
            {
                *(long *)pcad->valc = mode;
            }
            else
            {
                strncpy ( pcad->mess, "Invalid type for VALC", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }

        if ( pcad->vald != NULL )
        {
            if ( pcad->ftvd == DBF_LONG )
            {
                *(long *)pcad->vald = mode;
            }
            else
            {
                strncpy ( pcad->mess, "Invalid type for VALD", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
    }

/* Copy the next four attributes if this is a CAD8 or larger */

    if ( size >= 8 )
    {
        if ( pcad->vale != NULL )
        {
            if ( pcad->ftve == DBF_LONG )
            {
                *(long *)pcad->vale = mode;
            }
            else
            {
                strncpy ( pcad->mess, "Invalid type for VALE", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }

        if ( pcad->valf != NULL )
        {
            if ( pcad->ftvf == DBF_LONG )
            {
                *(long *)pcad->valf = mode;
            }
            else
            {
                strncpy ( pcad->mess, "Invalid type for VALF", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }

        if ( pcad->valg != NULL )
        {
            if ( pcad->ftvg == DBF_LONG )
            {
                *(long *)pcad->valg = mode;
            }
            else
            {
                strncpy ( pcad->mess, "Invalid type for VALG", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }

        if ( pcad->valh != NULL )
        {
            if ( pcad->ftvh == DBF_LONG )
            {
                *(long *)pcad->valh = mode;
            }
            else
            {
                strncpy ( pcad->mess, "Invalid type for VALH", MAX_STRING_SIZE-1 );
                status = CAD_REJECT;
            }
        }
    }

/*
 * Give back the mode semaphore
 */

    semGive( gmosCadModeSem );

/* Apologise if there are more than 8 outputs. */

    if ( size > 8 )
    {
        logMsg( "gmosCadModeFanout:%s - Sorry, only 8 outputs are fanned out.\n",
          (int) pcad->name, 0, 0, 0, 0, 0);
    }

    return status;
}

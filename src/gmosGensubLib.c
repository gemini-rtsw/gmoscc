static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id$"};
/*
*   FILENAME
*   -------- 
*   gmosGensubLib.c
*
*   PURPOSE
*   -------
*   Library of general purpose GMOS functions executed by "genSub" records.
*
*   FUNCTION NAME(S)
*   ----------------
*   gmosCarCombineMax        - Combine multiple CAR states, finding the maximum state
*   gmosCarCombineBusy       - Combine multiple CAR states in IDLE-PAUSED-ERROR-BUSY order
*   gmosDoorIlockCombine     - Combine the states of all eight door interlocks
*   gmosHealthCombineString  - Combine multiple health values stored as strings
*   gmosHealthCombineInt     - Combine multiple health values stored as integers
*   gmosSimmFanout           - Fan out a simulation mode to high/low software layers
*   gmosStateInit            - Initialises the GMOS state
*   gmosStateCombine         - Combines multiple assembly states into one value
*   gmosIlockInit            - Initialises the GMOS interlock event
*   gmosIlockCombine         - Combines and reports GMOS interlock events
*   gmosStringConcat         - Concatenate two or more string inputs together
*   gmosStringFilter         - Pass a string as long as it differs from a given tag
*   gmosCheckDirective       - Check and report on directives passed from apply record (debugging)
*   gmosDtaTemperatureSimInit- Initialise the filtering algorithm for the dta
*   gmosDtaTemperatureSim    - Low pass filtering algorithm for the detector translator assembly
*   gmosTemperatureChangeInit- Initialise the rate of change of temperature calculation
*   gmosTemperatureChange    - Calculate the hourly rate of change of temperature
*   gmosMakeGratString       - Construct the config string for the grating assembly
*
*   DEPENDENCIES
*   ------------
*
*   LIMITATIONS
*   -----------
*
*   AUTHORS
*   -------
*   Steven Beard  (smb@roe.ac.uk)
*   Chris Tierney (cjt@roe.ac.uk)
*
*   HISTORY
*   -------
*   24-Jan-1997: Original version as cicsCarCombine.c.            (smb)
*   01-Jul-1999: Original GMOS version, based on CICS libraries.  (smb)
*                (SEE BELOW FOR REST OF HISTORY)
*/
/* *INDENT-OFF* */
/*
 * $Log$
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.15  2001/02/28 15:19:32  gmos
 * Modified gmosIlockCombine to use string constants with logMsg rather than a message buffer. Extra checking in gmosMakeGratString. Some strcpy calls converted to strncpy.
 *
 * Revision 1.14  2001/02/13 10:46:21  gmos
 * Bob added gmosDoorIlockCombine() function and changed gmosIlockCombine() function so only motor shutdown or soft interlock can cause CC Interlock. All interlock related status is logged (available on VALC).
 *
 * Revision 1.13  2001/01/10 17:54:45  gmos
 * Added function for constructing config string for grating assembly.
 *
 * Revision 1.12  2000/11/03 13:27:35  gmos
 * Modified gmosStateCombine function to change the state to BOOTING when master enable is switched off.
 *
 * Revision 1.11  2000/09/28 16:30:19  gmos
 * Modified interlock combination to write message to VALB and prevent message storms on the console.
 *
 * Revision 1.10  2000/09/11 11:11:59  gmos
 * Fixed bug that produced transient false values on initialisation.
 *
 * Revision 1.9  2000/09/08 11:51:19  gmos
 * Smooth the temperature differences by averaging with previous value.
 *
 * Revision 1.8  2000/09/01 09:16:36  gmos
 * Random tabs removed from source code.
 *
 * Revision 1.7  2000/08/31 15:49:07  gmos
 * Temperature change monitoring functions added.
 *
 * Revision 1.6  2000/08/23 10:05:07  gmos
 * Added second filtered temperature.
 *
 * Revision 1.5  2000/05/26 10:55:39  cjt
 * Added support for adjustment of detector trans position with thermal model.
 * Added support for switching off faulty temperature sensors. Minor typos fixed.
 *
 * Revision 1.4  2000/05/15 16:49:39  gmos
 * Include tickLib to remove compiler warning
 *
 * Revision 1.3  2000/04/10 09:20:02  gmos
 * gmtools and gmoscc merged
 *
 * Revision 1.11  2000/02/25 11:48:47  gmos
 * Always respond to soft interlock regardless of master enable
 *
 * Revision 1.10  2000/02/24 17:28:50  gmos
 * Do not report interlock while master enable switched off. This should eliminate spurious messages while system starting up.
 *
 * Revision 1.9  2000/01/24 16:43:37  gmos
 * gmosIlockCombine added for combining GMOS interlocks
 *
 * Revision 1.8  2000/01/21 15:39:36  gmos
 * Fixed bug in gmosStateCombine
 *
 * Revision 1.7  2000/01/19 17:39:08  gmos
 * Added generation of state for GMOS state veriable in SAD
 *
 * Revision 1.6  2000/01/10 11:10:22  gmos
 * Added gmosCheckDirective debugging function
 *
 * Revision 1.5  1999/10/07 10:31:18  gmos
 * gmosStateInit function added.
 *
 * Revision 1.4  1999/08/04 17:00:54  gmos
 * Added gmosStateCombine for combining assembly states
 *
 * Revision 1.3  1999/08/02 14:12:51  gmos
 * New function for combining CAR values TCS-style
 *
 * Revision 1.2  1999/07/09 08:44:07  gmos
 * Removed DEBUG macro, since it may clash
 *
 * Revision 1.1  1999/07/08 14:07:00  gmos
 * Moved from gmoscc module
 *
 * Revision 1.1  1999/07/07 13:42:58  gmos
 * First version
 *
 */
/* *INDENT-ON* */

#include  <vxWorks.h>
#include  <types.h>
#include  <stdlib.h>
#include  <stdioLib.h>
#include  <string.h>
#include  <logLib.h>
#include  <sysLib.h>
#include  <tickLib.h>
#include  <taskLib.h>
#include  <wdLib.h>
#include  <callback.h>
#include  <math.h>

#include  <dbDefs.h>
#include  <genSubRecord.h>
#include  <cad.h>
#include  <car.h>
#include  <dbCommon.h>
#include  <recSup.h>

#include  "recAssControl.h"

/* #define VERBOSE */     /* Define this for verbose messages */

/* Pass and fail values returned by genSub functions. */

#define GOOD 0
#define BAD  1

/* Other constants */

#define GM_SAD_STATE_BOOTING        0
#define GM_SAD_STATE_INITIALIZING   1
#define GM_SAD_STATE_RUNNING        2
#define GM_SAD_STATE_CONFIGURING    3

#define MAX_CAR_RECORDS 7

#define FIRST_PERIODIC_SCAN 3

#define TEMPCHANGE_INTERVAL       600        /* Time interval for temp. change monitoring (seconds) */
#define TEMPCHANGE_SHORT_INTERVAL 5          /* Time to skip if temp. change record is busy         */
#define TEMPCHANGE_NSENSORS       9          /* Number of temperature sensors.                      */

#define MESS_BUFF_SIZE            80         /* Message buffer size */
#define SCRATCH_BUFF_SIZE         128        /* Scratch buffer size */


/* Globals */

double scanPeriods[] = {10.0, 5.0, 2.0, 1.0, 0.5, 0.2, 0.1, 0.05, 0.01, 0.05};

struct callback {
    CALLBACK      callback;
    dbCommon      *pRecord;
    WDOG_ID       wd_id;
};

/* Private structure to contain temperature change information. */

typedef struct
{
    SEM_ID    cmdLock;		/* Timer semaphore.			*/
    SEM_ID    dataLock;		/* Database protection semaphore.	*/
    long      timeout;		/* Private timeout flag.		*/
    void      *timer;		/* Timer callback info.			*/
    double    previousTemp[TEMPCHANGE_NSENSORS];
                                /* Previous temperatures.		*/
    double    previousDiff[TEMPCHANGE_NSENSORS];
                                /* Previous changes.			*/
    double    prevPrevDiff[TEMPCHANGE_NSENSORS];
                                /* The changes before that.		*/
}  TEMPCHANGE_PRIVATE;

/* Local function prototypes */

static void   tempchangeCallback( struct callback *);
void          callbackRequest();


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCarCombineBusy
 *
 *   Purpose:
 *   Combine together multiple CAR states, finding the maximum state
 *
 *   Description:
 *   This function reads the states of up to seven CAR records
 *   and generates an event based on the maximum of those states.
 *
 *   - An IDLE event is generated when all CAR records are idle.
 *   - A PAUSED event is generated when one of the CAR records is PAUSED
 *     and all the others are IDLE.
 *   - A BUSY event is generated when any of the CAR records is BUSY
 *     but none are in the ERROR state.
 *   - An ERROR event is generated when any of the CAR records are in
 *     the ERROR state (regardless of the other states).
 *
 *   The OERR and OMSS fields of the winning CAR record are passed to
 *   the output.
 *
 *   The function is designed to be used with the large (21 inputs/outputs)
 *   variety of the EPICS "genSub" record.
 *
 *   Invocation:
 *   status = gmosCarCombineBusy( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    long     Event delivered to CAR record 1
 *      > pgensub->b    string   Error message of CAR record 1
 *      > pgensub->c    long     Error number of CAR record 1
 *
 *      etc...
 *
 *      > pgensub->s    long     Event delivered to CAR record 7
 *      > pgensub->t    string   Error message of CAR record 7
 *      > pgensub->u    long     Error number of CAR record 7
 *
 *   Parameters out:
 *      < pgensub->vala long     Resulting CAR event
 *      < pgensub->valb string   Resulting error message
 *      < pgensub->valc long     Resulting error number
 *      < pgensub->vald long     Index of winning CAR record
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   This function works best if the inputs come from CAR events and not
 *   CAR states. This means connecting the A,D,G,J,M,P and S inputs of the
 *   genSub record to the .IVAL fields of the individual CAR records
 *   and not to the .VAL fields
 *
 *   Limitations:
 *   Only 7 CAR records can be combined at once, due to the limit number
 *   of inputs on the genSub record. Larger numbers can be combined by
 *   joining several genSub records in a tree structure.
 *
 *   A better implementation of this function would be one which remembers
 *   which inputs were the most recent ones to have changed and give these
 *   inputs priority. As it stands at the moment, an old error can mask
 *   the appearance of a new error.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   24-Jan-1997: Original version, based on tcsCarValCombine,
 *                but with client ID included.                     (smb)
 *   25-Jan-1997: Generate index of winning CAR event.             (smb)
 *   01-Jul-1999: Converted into GMOS function. Client ID dropped. (smb)
 *   08-Jul-1999: Watch out for NULL pointers.                     (smb)
 *-
 */

long gmosCarCombineBusy( struct genSubRecord *pgensub ) 
{
    long   status;       /* return status */

    long   curStates[ MAX_CAR_RECORDS ];
                         /* Array storing current CAR record states seen on the inputs */
    int    i;            /* Counter */

    long   outVal;       /* Output CAR event value */
    long   outErr;       /* Output error number */
    char   outMess[ MAX_STRING_SIZE ];
                         /* Output error message */
    long   outIndex;     /* Output index */

/* Initialise the status */

    status = GOOD;

/* Find out the current states, assuming zero if any of the pointers are NULL */

    curStates[0] = (pgensub->a != NULL) ? *(long *)pgensub->a : 0;
    curStates[1] = (pgensub->d != NULL) ? *(long *)pgensub->d : 0;
    curStates[2] = (pgensub->g != NULL) ? *(long *)pgensub->g : 0;
    curStates[3] = (pgensub->j != NULL) ? *(long *)pgensub->j : 0;
    curStates[4] = (pgensub->m != NULL) ? *(long *)pgensub->m : 0;
    curStates[5] = (pgensub->p != NULL) ? *(long *)pgensub->p : 0;
    curStates[6] = (pgensub->s != NULL) ? *(long *)pgensub->s : 0;

#ifdef VERBOSE
    printf ("gmosCarCombineBusy: ");
    for ( i=0; i<7; i++) printf ("%d ", curStates[i]);
    printf ("\n");
#endif

/*
 * Skip through the inputs, determining the most significant current state in the order IDLE-PAUSED-ERROR-BUSY
 * and make that state the output. Any states outside the range CAR_IDLE to CAR_ERROR are ignored.
 */

        outVal = CAR_IDLE;
        outErr = 0;
        outIndex = 0;

        for ( i=0; i < MAX_CAR_RECORDS; i++)
        {
            if ( curStates[i] == CAR_BUSY )
            {
                outVal = curStates[i];
                outIndex = i;
            }
            else if ( (curStates[i] == CAR_ERROR) && (outVal != CAR_BUSY) )
            {
                outVal = curStates[i];
                outIndex = i;
            }
            else if ( (curStates[i] == CAR_PAUSED) && (outVal != CAR_BUSY) && (outVal != CAR_ERROR))
            {
                outVal = curStates[i];
                outIndex = i;
            }
        }

        switch (outIndex)
        {
            case (0):
                outErr = *(long *)pgensub->c;
                strncpy ( outMess, (char *)pgensub->b, MAX_STRING_SIZE-1 );
                break;

            case (1):
                outErr = *(long *)pgensub->f;
                strncpy ( outMess, (char *)pgensub->e, MAX_STRING_SIZE-1 );
                break;

            case (2):
                outErr = *(long *)pgensub->i;
                strncpy ( outMess, (char *)pgensub->h, MAX_STRING_SIZE-1 );
                break;

            case (3):
                outErr = *(long *)pgensub->l;
                strncpy ( outMess, (char *)pgensub->k, MAX_STRING_SIZE-1 );
                break;

            case (4):
                outErr = *(long *)pgensub->o;
                strncpy ( outMess, (char *)pgensub->n, MAX_STRING_SIZE-1 );
                break;

            case (5):
                outErr = *(long *)pgensub->r;
                strncpy ( outMess, (char *)pgensub->q, MAX_STRING_SIZE-1 );
                break;

            case (6):
                outErr = *(long *)pgensub->u;
                strncpy ( outMess, (char *)pgensub->t, MAX_STRING_SIZE-1 );
                break;

            default:
                strncpy ( outMess, "CAR combination failure", MAX_STRING_SIZE-1 );
                status = BAD;
                break;
        }

/*
 * Output the resulting index, error message, error number and state.
 */

   *(long *)pgensub->vald = outIndex + 1;
   *(long *)pgensub->valc = outErr;
   strncpy ( (char *)pgensub->valb, outMess, MAX_STRING_SIZE-1 );
   *(long *)pgensub->vala = outVal;

   return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCarCombineMax
 *
 *   Purpose:
 *   Combine together multiple CAR states.
 *
 *   Description:
 *   This function reads the states of up to seven CAR records
 *   and generates an event based on those states.
 *
 *   - An IDLE event is generated when all CAR records are idle.
 *   - A PAUSED event is generated when one of the CAR records is PAUSED
 *     and all the others are IDLE.
 *   - A BUSY event is generated when any of the CAR records is BUSY
 *     but none are in the ERROR state.
 *   - An ERROR event is generated when any of the CAR records are in
 *     the ERROR state (regardless of the other states).
 *
 *   The OERR and OMSS fields of the winning CAR record are passed to
 *   the output.
 *
 *   The function is designed to be used with the large (21 inputs/outputs)
 *   variety of the EPICS "genSub" record.
 *
 *   Invocation:
 *   status = gmosCarCombineMax( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    long     Event delivered to CAR record 1
 *      > pgensub->b    string   Error message of CAR record 1
 *      > pgensub->c    long     Error number of CAR record 1
 *
 *      etc...
 *
 *      > pgensub->s    long     Event delivered to CAR record 7
 *      > pgensub->t    string   Error message of CAR record 7
 *      > pgensub->u    long     Error number of CAR record 7
 *
 *   Parameters out:
 *      < pgensub->vala long     Resulting CAR event
 *      < pgensub->valb string   Resulting error message
 *      < pgensub->valc long     Resulting error number
 *      < pgensub->vald long     Index of winning CAR record
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   This function works best if the inputs come from CAR events and not
 *   CAR states. This means connecting the A,D,G,J,M,P and S inputs of the
 *   genSub record to the .IVAL fields of the individual CAR records
 *   and not to the .VAL fields
 *
 *   Limitations:
 *   Only 7 CAR records can be combined at once, due to the limit number
 *   of inputs on the genSub record. Larger numbers can be combined by
 *   joining several genSub records in a tree structure.
 *
 *   If a new error happens while an existing input is already in an ERROR
 *   state a new error message will be directed to the CAR record. The
 *   CAR record will not output this new error because it only does this
 *   when changing to the ERROR state from something else. This is a
 *   deficiency in the CAR record. It could be overcome by toggling the
 *   output state to BUSY and then ERROR.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   24-Jan-1997: Original version, based on tcsCarValCombine,
 *                but with client ID included.                     (smb)
 *   25-Jan-1997: Generate index of winning CAR event.             (smb)
 *   01-Jul-1999: Converted into GMOS function. Client ID dropped. (smb)
 *   08-Jul-1999: Watch out for NULL pointers.                     (smb)
 *-
 */

long gmosCarCombineMax( struct genSubRecord *pgensub ) 
{
    long   status;       /* return status */

    long   curStates[ MAX_CAR_RECORDS ];
                         /* Array storing current CAR record states seen on the inputs */
    int    i;            /* Counter */

    long   outVal;       /* Output CAR event value */
    long   outErr;       /* Output error number */
    char   outMess[ MAX_STRING_SIZE ];
                         /* Output error message */
    long   outIndex;     /* Output index */

/* Initialise the status */

    status = GOOD;

/* Find out the current states, assuming zero if any of the pointers are NULL */

    curStates[0] = (pgensub->a != NULL) ? *(long *)pgensub->a : 0;
    curStates[1] = (pgensub->d != NULL) ? *(long *)pgensub->d : 0;
    curStates[2] = (pgensub->g != NULL) ? *(long *)pgensub->g : 0;
    curStates[3] = (pgensub->j != NULL) ? *(long *)pgensub->j : 0;
    curStates[4] = (pgensub->m != NULL) ? *(long *)pgensub->m : 0;
    curStates[5] = (pgensub->p != NULL) ? *(long *)pgensub->p : 0;
    curStates[6] = (pgensub->s != NULL) ? *(long *)pgensub->s : 0;

#ifdef VERBOSE
    printf ("gmosCarCombineMax: ");
    for ( i=0; i<7; i++) printf ("%d ", curStates[i]);
    printf ("\n");
#endif

/*
 * Skip through the inputs, determining the highest current state and make that state the
 * output. Any states outside the range CAR_IDLE to CAR_ERROR are ignored.
 */

        outVal = CAR_IDLE;
        outErr = 0;
        outIndex = 0;

        for ( i=0; i < MAX_CAR_RECORDS; i++)
        {
            if ( (curStates[i] <= CAR_ERROR) && (curStates[i] > outVal) )
            {
                outVal = curStates[i];
                outIndex = i;
            }
        }

        switch (outIndex)
        {
            case (0):
                outErr = *(long *)pgensub->c;
                strncpy ( outMess, (char *)pgensub->b, MAX_STRING_SIZE-1 );
                break;

            case (1):
                outErr = *(long *)pgensub->f;
                strncpy ( outMess, (char *)pgensub->e, MAX_STRING_SIZE-1 );
                break;

            case (2):
                outErr = *(long *)pgensub->i;
                strncpy ( outMess, (char *)pgensub->h, MAX_STRING_SIZE-1 );
                break;

            case (3):
                outErr = *(long *)pgensub->l;
                strncpy ( outMess, (char *)pgensub->k, MAX_STRING_SIZE-1 );
                break;

            case (4):
                outErr = *(long *)pgensub->o;
                strncpy ( outMess, (char *)pgensub->n, MAX_STRING_SIZE-1 );
                break;

            case (5):
                outErr = *(long *)pgensub->r;
                strncpy ( outMess, (char *)pgensub->q, MAX_STRING_SIZE-1 );
                break;

            case (6):
                outErr = *(long *)pgensub->u;
                strncpy ( outMess, (char *)pgensub->t, MAX_STRING_SIZE-1 );
                break;

            default:
                strncpy ( outMess, "CAR combination failure", MAX_STRING_SIZE-1 );
                status = BAD;
                break;
        }

/* Output the resulting index, error message, error number and state. */

   *(long *)pgensub->vald = outIndex + 1;
   *(long *)pgensub->valc = outErr;
   strncpy ( (char *)pgensub->valb, outMess, MAX_STRING_SIZE-1 );
   *(long *)pgensub->vala = outVal;

  return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   gmosDoorIlockCombine
 *
 *   Purpose:
 *   Combine together GMOS door interlock signals into one status word. 
 *
 *   Description:
 *   This function reads the GMOS door interlock signals. 
 *   
 *
 *   Invocation:
 *   status = gmosDoorIlockCombine( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    long    door1        (0 if closed)
 *      > pgensub->b    long    door2        (0 if closed)
 *      > pgensub->c    long    door3        (0 if closed)
 *      > pgensub->d    long    door4        (0 if closed)
 *      > pgensub->e    long    door5        (0 if closed)
 *      > pgensub->f    long    door6        (0 if closed)
 *      > pgensub->g    long    door7        (0 if closed)
 *      > pgensub->h    long    door8        (0 if closed)
 *
 *   Parameters out:
 *      < pgensub->vala long    doorInterlocks output value 
 *
 *   Return value:
 *      < status        long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *      None
 *
 *   Limitations:
 *
 *   Author:
 *   Bob Wooff 
 *
 *   History:
 *   25-Jan-2001: Original GMOS version.                            (bmw)
 *
 *   
 *-
 */

long gmosDoorIlockCombine( struct genSubRecord *pgensub ) 
{
    long status = 0;                    /* return status                 */
    long door1, door2, door3, door4,    /* individual door interlocks    */
         door5, door6, door7, door8;
    long doorInterlocks;                /* combined door interlocks      */


    /*
     *  Read individual interlocks 
     */

    door1       = *(long *)pgensub->a;
    door2       = *(long *)pgensub->b;
    door3       = *(long *)pgensub->c;
    door4       = *(long *)pgensub->d;
    door5       = *(long *)pgensub->e;
    door6       = *(long *)pgensub->f;
    door7       = *(long *)pgensub->g;
    door8       = *(long *)pgensub->h;


    /*
     *  Combine them into a single value
     */

    doorInterlocks = ( door1    | door2<<1 | door3<<2 | door4<<3 |
                       door5<<4 | door6<<5 | door7<<6 | door8<<7   );

    /*
     *  Copy the combined interlocks out to VALA
     */

    *(long *)pgensub->vala = doorInterlocks;

    return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   gmosHealthCombineString
 *
 *   Purpose:
 *   Combine together multiple health states stored as strings.
 *
 *   Description:
 *   This function reads the states of up to ten health records, where
 *   the health is stored as a string, and generates a combined health.
 *
 *   - The result will be GOOD if none of the inputs are WARNING or BAD.
 *   - The result will be WARNING if any of the inputs are WARNING
 *     but none of the inputs are BAD.
 *   - The result will be BAD if any of the inputs are BAD.
 *
 *   The message contained in the winning health record is passed to
 *   the output.
 *
 *   The function is designed to be used with the large (21 inputs/outputs)
 *   variety of the EPICS "genSub" record.
 *
 *   Invocation:
 *   status = gmosHealthCombineString( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    string  Value of health record 1
 *      > pgensub->b    string  Message contained in health record 1
 *      > pgensub->c    string  Value of health record 2
 *      > pgensub->d    string  Message contained in health record 2
 *      > pgensub->e    string  Value of health record 3
 *      > pgensub->f    string  Message contained in health record 3
 *      > pgensub->g    string  Value of health record 4
 *      > pgensub->h    string  Message contained in health record 4
 *      > pgensub->i    string  Value of health record 5
 *      > pgensub->j    string  Message contained in health record 5
 *      > pgensub->k    string  Value of health record 6
 *      > pgensub->l    string  Message contained in health record 6
 *      > pgensub->m    string  Value of health record 7
 *      > pgensub->n    string  Message contained in health record 7
 *      > pgensub->o    string  Value of health record 8
 *      > pgensub->p    string  Message contained in health record 8
 *      > pgensub->q    string  Value of health record 9
 *      > pgensub->r    string  Message contained in health record 9
 *      > pgensub->s    string  Value of health record 10
 *      > pgensub->t    string  Message contained in health record 10
 *
 *   Parameters out:
 *      < pgensub->vala string  Resulting health
 *      < pgensub->valb string  Winning message
 *      < pgensub->valc long    Index of winning health input
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *   If any or all the input health values are silly the result is still
 *   GOOD. Only the strings "WARNING" and "BAD" are recognised.
 *
 *   I expect this function will be very inefficient, since it deals with
 *   lots of character strings. It would be much more efficient to combine
 *   health values as integers by taking the maximum. It should be ok if
 *   the function is executed only when the health changes and is not
 *   executed continuously. If efficiency is important gmosHealthCombineInt
 *   should be used instead.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   24-Jan-1997: Original version.                                (smb)
 *   25-Jan-1997: Generate index of winning health input.          (smb)
 *   01-Jul-1999: GMOS version allowing 10 inputs with efficiency
 *                improvements.                                    (smb)
 *   
 *-
 */

long gmosHealthCombineString( struct genSubRecord *pgensub ) 
{
  long status;                       /* return status */
  char outHealth[MAX_STRING_SIZE];   /* Output health value */
  char inMess[MAX_STRING_SIZE];      /* Input message */
  char outMess[MAX_STRING_SIZE];     /* Output message */
  long outIndex;                     /* Index of winning health input */

/* Initialise the status */

  status = GOOD;

/*
 * The default output value is GOOD and the default message is the first
 * non-null and non-blank message (or a null message if there are no non-null
 * and non-blank messages). The default index corresponds to the message
 * chosen (or is 1 if there are no messages found).
 */

  strncpy( outHealth, "GOOD", MAX_STRING_SIZE-1 );
  strncpy( outMess, "", MAX_STRING_SIZE-1 );

  outIndex = 1;

  if ( (strlen((char *)pgensub->b) != 0) && (strspn((char *)pgensub->b," ") != strlen((char *)pgensub->b)) )
  {
    strncpy( outMess, (char *)pgensub->b, MAX_STRING_SIZE-1 );
    outIndex = 1;
  }
  else if ( (strlen((char *)pgensub->d) != 0) && (strspn((char *)pgensub->d," ") != strlen((char *)pgensub->d)) )
  {
    strncpy( outMess, (char *)pgensub->d, MAX_STRING_SIZE-1 );
    outIndex = 2;
  }
  else if ( (strlen((char *)pgensub->f) != 0) && (strspn((char *)pgensub->f," ") != strlen((char *)pgensub->f)) )
  {
    strncpy( outMess, (char *)pgensub->f, MAX_STRING_SIZE-1 );
    outIndex = 3;
  }
  else if ( (strlen((char *)pgensub->h) != 0) && (strspn((char *)pgensub->h," ") != strlen((char *)pgensub->h)) )
  {
    strncpy( outMess, (char *)pgensub->h, MAX_STRING_SIZE-1 );
    outIndex = 4;
  }
  else if ( (strlen((char *)pgensub->j) != 0) && (strspn((char *)pgensub->j," ") != strlen((char *)pgensub->j)) )
  {
    strncpy( outMess, (char *)pgensub->j, MAX_STRING_SIZE-1 );
    outIndex = 5;
  }
  else if ( (strlen((char *)pgensub->l) != 0) && (strspn((char *)pgensub->l," ") != strlen((char *)pgensub->l)) )
  {
    strncpy( outMess, (char *)pgensub->l, MAX_STRING_SIZE-1 );
    outIndex = 6;
  }
  else if ( (strlen((char *)pgensub->n) != 0) && (strspn((char *)pgensub->n," ") != strlen((char *)pgensub->n)) )
  {
    strncpy( outMess, (char *)pgensub->n, MAX_STRING_SIZE-1 );
    outIndex = 7;
  }
  else if ( (strlen((char *)pgensub->p) != 0) && (strspn((char *)pgensub->p," ") != strlen((char *)pgensub->p)) )
  {
    strncpy( outMess, (char *)pgensub->p, MAX_STRING_SIZE-1 );
    outIndex = 8;
  }
  else if ( (strlen((char *)pgensub->r) != 0) && (strspn((char *)pgensub->r," ") != strlen((char *)pgensub->r)) )
  {
    strncpy( outMess, (char *)pgensub->r, MAX_STRING_SIZE-1 );
    outIndex = 9;
  }
  else if ( (strlen((char *)pgensub->t) != 0) && (strspn((char *)pgensub->t," ") != strlen((char *)pgensub->t)) )
  {
    strncpy( outMess, (char *)pgensub->t, MAX_STRING_SIZE-1 );
    outIndex = 10;
  }

/*
 * If any health value is WARNING this will override the GOOD value and result 
 * in a WARNING output value. If the message associated with this WARNING
 * value is non-null and non-blank it will override the default message.
 */

  if ( (strcmp( (char *)pgensub->a, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->a, "warning") == 0) )
  {
    strncpy( outHealth, "WARNING", MAX_STRING_SIZE-1 );
    outIndex = 1;

    strncpy( inMess, (char *)pgensub->b, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->c, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->c, "warning") == 0) )
  {
    strncpy( outHealth, "WARNING", MAX_STRING_SIZE-1 );
    outIndex = 2;

    strncpy( inMess, (char *)pgensub->d, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->e, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->e, "warning") == 0) )
  {
    strncpy( outHealth, "WARNING", MAX_STRING_SIZE-1 );
    outIndex = 3;

    strncpy( inMess, (char *)pgensub->f, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->g, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->g, "warning") == 0) )
  {
    strncpy( outHealth, "WARNING", MAX_STRING_SIZE-1 );
    outIndex = 4;

    strncpy( inMess, (char *)pgensub->h, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->i, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->i, "warning") == 0) )
  {
    strncpy( outHealth, "WARNING", MAX_STRING_SIZE-1 );
    outIndex = 5;

    strncpy( inMess, (char *)pgensub->j, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->k, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->k, "warning") == 0) )
  {
    strncpy( outHealth, "WARNING", MAX_STRING_SIZE-1 );
    outIndex = 6;

    strncpy( inMess, (char *)pgensub->l, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->m, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->m, "warning") == 0) )
  {
    strncpy( outHealth, "WARNING", MAX_STRING_SIZE-1 );
    outIndex = 7;

    strncpy( inMess, (char *)pgensub->n, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->o, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->o, "warning") == 0) )
  {
    strncpy( outHealth, "WARNING", MAX_STRING_SIZE-1 );
    outIndex = 8;

    strncpy( inMess, (char *)pgensub->p, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->q, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->q, "warning") == 0) )
  {
    strncpy( outHealth, "WARNING", MAX_STRING_SIZE-1 );
    outIndex = 9;

    strncpy( inMess, (char *)pgensub->r, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->s, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->s, "warning") == 0) )
  {
    strncpy( outHealth, "WARNING", MAX_STRING_SIZE-1 );
    outIndex = 10;

    strncpy( inMess, (char *)pgensub->t, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

/*
 * If any health value is BAD this will override any GOOD or BAD values
 * and result in a BAD output value. If the message associated with this BAD
 * value is non-null and non-blank it will override the default message.
 */

  if ( (strcmp( (char *)pgensub->a, "BAD") == 0) ||
       (strcmp( (char *)pgensub->a, "bad") == 0) )
  {
    strncpy( outHealth, "BAD", MAX_STRING_SIZE-1 );
    outIndex = 1;

    strncpy( inMess, (char *)pgensub->b, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->c, "BAD") == 0) ||
       (strcmp( (char *)pgensub->c, "bad") == 0) )
  {
    strncpy( outHealth, "BAD", MAX_STRING_SIZE-1 );
    outIndex = 2;

    strncpy( inMess, (char *)pgensub->d, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->e, "BAD") == 0) ||
       (strcmp( (char *)pgensub->e, "bad") == 0) )
  {
    strncpy( outHealth, "BAD", MAX_STRING_SIZE-1 );
    outIndex = 3;

    strncpy( inMess, (char *)pgensub->f, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->g, "BAD") == 0) ||
       (strcmp( (char *)pgensub->g, "bad") == 0) )
  {
    strncpy( outHealth, "BAD", MAX_STRING_SIZE-1 );
    outIndex = 4;

    strncpy( inMess, (char *)pgensub->h, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->i, "BAD") == 0) ||
       (strcmp( (char *)pgensub->i, "bad") == 0) )
  {
    strncpy( outHealth, "BAD", MAX_STRING_SIZE-1 );
    outIndex = 5;

    strncpy( inMess, (char *)pgensub->j, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->k, "BAD") == 0) ||
       (strcmp( (char *)pgensub->k, "bad") == 0) )
  {
    strncpy( outHealth, "BAD", MAX_STRING_SIZE-1 );
    outIndex = 6;

    strncpy( inMess, (char *)pgensub->l, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->m, "BAD") == 0) ||
       (strcmp( (char *)pgensub->m, "bad") == 0) )
  {
    strncpy( outHealth, "BAD", MAX_STRING_SIZE-1 );
    outIndex = 7;

    strncpy( inMess, (char *)pgensub->n, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->o, "BAD") == 0) ||
       (strcmp( (char *)pgensub->o, "bad") == 0) )
  {
    strncpy( outHealth, "BAD", MAX_STRING_SIZE-1 );
    outIndex = 8;

    strncpy( inMess, (char *)pgensub->p, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->q, "BAD") == 0) ||
       (strcmp( (char *)pgensub->q, "bad") == 0) )
  {
    strncpy( outHealth, "BAD", MAX_STRING_SIZE-1 );
    outIndex = 9;

    strncpy( inMess, (char *)pgensub->r, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

  else if ( (strcmp( (char *)pgensub->s, "BAD") == 0) ||
       (strcmp( (char *)pgensub->s, "bad") == 0) )
  {
    strncpy( outHealth, "BAD", MAX_STRING_SIZE-1 );
    outIndex = 10;

    strncpy( inMess, (char *)pgensub->t, MAX_STRING_SIZE-1 );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strncpy( outMess, inMess, MAX_STRING_SIZE-1 );
    }
  }

/* Output the resulting health value, message and index. */

   strncpy( (char *)pgensub->vala, outHealth, MAX_STRING_SIZE-1 );
   strncpy( (char *)pgensub->valb, outMess, MAX_STRING_SIZE-1 );
   *(long *)pgensub->valc = outIndex;

  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosHealthCombineInt
 *
 *   Purpose:
 *   Combine together multiple health states where health is stored as integers.
 *
 *   Description:
 *   This function reads the states of up to ten health records, where
 *   the health is stored as an integer, and generates a combined health.
 *   It is assumed that the health values are designed so the higher the
 *   value the worse the health. The maximum health value, together with
 *   its assoiated message, is passed to the output.
 *
 *   The function is designed to be used with the large (21 inputs/outputs)
 *   variety of the EPICS "genSub" record.
 *
 *   Invocation:
 *   status = gmosHealthCombineInt( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    long    Value of health record 1
 *      > pgensub->b    string  Message contained in health record 1
 *      > pgensub->c    long    Value of health record 2
 *      > pgensub->d    string  Message contained in health record 2
 *      > pgensub->e    long    Value of health record 3
 *      > pgensub->f    string  Message contained in health record 3
 *      > pgensub->g    long    Value of health record 4
 *      > pgensub->h    string  Message contained in health record 4
 *      > pgensub->i    long    Value of health record 5
 *      > pgensub->j    string  Message contained in health record 5
 *      > pgensub->k    long    Value of health record 6
 *      > pgensub->l    string  Message contained in health record 6
 *      > pgensub->m    long    Value of health record 7
 *      > pgensub->n    string  Message contained in health record 7
 *      > pgensub->o    long    Value of health record 8
 *      > pgensub->p    string  Message contained in health record 8
 *      > pgensub->q    long    Value of health record 9
 *      > pgensub->r    string  Message contained in health record 9
 *      > pgensub->s    long    Value of health record 10
 *      > pgensub->t    string  Message contained in health record 10
 *
 *   Parameters out:
 *      < pgensub->vala long    Resulting health value
 *      < pgensub->valb string  Winning message
 *      < pgensub->valc long    Index of winning health input
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   The underlying assumption is that increasing value means increasingly worse
 *   health. The function will not work for other health schemes.
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   08-Jul-1999: Original GMOS version.                         (smb)
 *   
 *-
 */

long gmosHealthCombineInt( struct genSubRecord *pgensub ) 
{
    long status;                       /* return status */
    long outHealth;                    /* Output health value */
    char outMess[MAX_STRING_SIZE];     /* Output message */
    long outIndex;                     /* Index of winning health input */

/* Initialise the status */

    status = GOOD;

/*
 * Find the maximum of all the health inputs and write this to outHealth.
 * The default value is the very first one.
 */

    outHealth = *(long *)pgensub->a;
    strncpy ( outMess, (char *)pgensub->b, MAX_STRING_SIZE-1 );
    outIndex = 1;

    if ( (pgensub->c != NULL) && (*(long *)pgensub->c > outHealth) )
    {
        outHealth = *(long *)pgensub->c;
        strncpy ( outMess, (char *)pgensub->d, MAX_STRING_SIZE-1 );
        outIndex = 2;
    }
    if ( (pgensub->e != NULL) && (*(long *)pgensub->e > outHealth) )
    {
        outHealth = *(long *)pgensub->e;
        strncpy ( outMess, (char *)pgensub->f, MAX_STRING_SIZE-1 );
        outIndex = 3;
    }
    if ( (pgensub->g != NULL) && (*(long *)pgensub->g > outHealth) )
    {
        outHealth = *(long *)pgensub->g;
        strncpy ( outMess, (char *)pgensub->h, MAX_STRING_SIZE-1 );
        outIndex = 4;
    }
    if ( (pgensub->i != NULL) && (*(long *)pgensub->i > outHealth) )
    {
        outHealth = *(long *)pgensub->i;
        strncpy ( outMess, (char *)pgensub->j, MAX_STRING_SIZE-1 );
        outIndex = 5;
    }
    if ( (pgensub->k != NULL) && (*(long *)pgensub->k > outHealth) )
    {
        outHealth = *(long *)pgensub->k;
        strncpy ( outMess, (char *)pgensub->l, MAX_STRING_SIZE-1 );
        outIndex = 6;
    }
    if ( (pgensub->m != NULL) && (*(long *)pgensub->m > outHealth) )
    {
        outHealth = *(long *)pgensub->m;
        strncpy ( outMess, (char *)pgensub->n, MAX_STRING_SIZE-1 );
        outIndex = 7;
    }
    if ( (pgensub->o != NULL) && (*(long *)pgensub->o > outHealth) )
    {
        outHealth = *(long *)pgensub->o;
        strncpy ( outMess, (char *)pgensub->p, MAX_STRING_SIZE-1 );
        outIndex = 8;
    }
    if ( (pgensub->q != NULL) && (*(long *)pgensub->q > outHealth) )
    {
        outHealth = *(long *)pgensub->q;
        strncpy ( outMess, (char *)pgensub->r, MAX_STRING_SIZE-1 );
        outIndex = 9;
    }
    if ( (pgensub->s != NULL) && (*(long *)pgensub->s > outHealth) )
    {
        outHealth = *(long *)pgensub->s;
        strncpy ( outMess, (char *)pgensub->t, MAX_STRING_SIZE-1 );
        outIndex = 10;
    }

/* Output the resulting health value, message and index. */

    *(long *)pgensub->vala = outHealth;
    strncpy( (char *)pgensub->valb, outMess, MAX_STRING_SIZE-1 );
    *(long *)pgensub->valc = outIndex;

    return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosSimmFanout
 *
 *   Purpose:
 *   Fan out a simulation mode to high and low software layers.
 *
 *   Description:
 *   This function fans out a given simulation mode with the following
 *   assumptions:
 *
 *   1) The first output goes to the high level layer software and all other
 *      (up to 9) outputs go to the low level layer software.
 *
 *   2) The simulation mode is to be passed on as follows:
 *
 *   Input mode        High layer mode        Low layer mode
 *   ----------        ---------------        --------------
 *   NONE              NONE                   NONE
 *   FAST              NONE                   FAST
 *   FULL              NONE                   FULL
 *   MAX               FULL                   VSM
 *   VSM               VSM                    VSM
 *
 *   It is assumed that VSM simulation should only exercise the uppermost
 *   interface layer of code and MAX simulation should only exercise the
 *   high level layer of code. Only when we switcb to FAST and FULL simulation
 *   does any communication reach the low level code, and communication with
 *   actual hardware only takes place when the simulation mode is NONE.
 *
 *   Invocation:
 *   status = gmosSimmFanout( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    string  Input simulation mode
 *      > pgensub->b    long    Number of outputs (2-10)
 *
 *   Parameters out:
 *      < pgensub->vala string  Simulation mode for high level software
 *      < pgensub->valb string  Simulation mode for low level software - 1
 *      < pgensub->valc string  Simulation mode for low level software - 2
 *      < pgensub->vald string  Simulation mode for low level software - 3
 *      < pgensub->vale string  Simulation mode for low level software - 4
 *      < pgensub->valf string  Simulation mode for low level software - 5
 *      < pgensub->valg string  Simulation mode for low level software - 6
 *      < pgensub->valh string  Simulation mode for low level software - 7
 *      < pgensub->vali string  Simulation mode for low level software - 8
 *      < pgensub->valj string  Simulation mode for low level software - 9
 *
 *   Return value
 *      < status e    long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *   The rule for deciding on the high level and low level simulation mode is
 *   arbitrary and hard-wired into this function. It would be possible to make
 *   the function more flexible by reading the rule from a file, but this would
 *   reduce the efficiency. If such flexibility is needed it would be possible
 *   to use a lookup table record instead.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   08-Jul-1999: Original version.                                (smb)   
 *-
 */

long gmosSimmFanout( struct genSubRecord *pgensub ) 
{
    long status;                       /* return status */
    char highMode[MAX_STRING_SIZE];    /* High level simulation mode */
    char lowMode[MAX_STRING_SIZE];     /* Low level simulation mode  */
    long outputs;                      /* Number of outputs (2-10) */

/* Initialise the status */

    status = GOOD;

/* Determine the high and low level simulation mode according to the input mode */

    if ( (strcmp( (char *)pgensub->a, "NONE") == 0) || (strcmp( (char *)pgensub->a, "none") == 0) )
    {
        strcpy (highMode, "NONE");
        strcpy (lowMode, "NONE");
    }
    else if ( (strcmp( (char *)pgensub->a, "FAST") == 0) || (strcmp( (char *)pgensub->a, "fast") == 0) )
    {
        strcpy (highMode, "NONE");
        strcpy (lowMode, "FAST");
    }
    else if ( (strcmp( (char *)pgensub->a, "FULL") == 0) || (strcmp( (char *)pgensub->a, "full") == 0) )
    {
        strcpy (highMode, "NONE");
        strcpy (lowMode, "FULL");
    }
    else if ( (strcmp( (char *)pgensub->a, "MAX") == 0) || (strcmp( (char *)pgensub->a, "max") == 0) )
    {
        strcpy (highMode, "FULL");
        strcpy (lowMode, "VSM");
    }
    else if ( (strcmp( (char *)pgensub->a, "VSM") == 0) || (strcmp( (char *)pgensub->a, "vsm") == 0) )
    {
        strcpy (highMode, "VSM");
        strcpy (lowMode, "VSM");
    }
    else
    {
        logMsg ("gmosSimmFanout: Invalid input mode %s", (int) (char *)pgensub->a, 0, 0, 0, 0, 0);
        status = BAD;
    }

    if ( pgensub->b != NULL )
    {
        outputs = *(long *)pgensub->b;
    }
    else
    {
        outputs = 10;
    }

/* Write the high and low level modes to the outputs. */

    if ( status == GOOD )
    {
        strncpy ( (char *)pgensub->vala, highMode, MAX_STRING_SIZE-1 );
        strncpy ( (char *)pgensub->valb, lowMode, MAX_STRING_SIZE-1 );
        if ( outputs >= 3 ) strncpy ( (char *)pgensub->valc, lowMode, MAX_STRING_SIZE-1 );
        if ( outputs >= 4 ) strncpy ( (char *)pgensub->vald, lowMode, MAX_STRING_SIZE-1 );
        if ( outputs >= 5 ) strncpy ( (char *)pgensub->vale, lowMode, MAX_STRING_SIZE-1 );
        if ( outputs >= 6 ) strncpy ( (char *)pgensub->valf, lowMode, MAX_STRING_SIZE-1 );
        if ( outputs >= 7 ) strncpy ( (char *)pgensub->valg, lowMode, MAX_STRING_SIZE-1 );
        if ( outputs >= 8 ) strncpy ( (char *)pgensub->valh, lowMode, MAX_STRING_SIZE-1 );
        if ( outputs >= 9 ) strncpy ( (char *)pgensub->vali, lowMode, MAX_STRING_SIZE-1 );
        if ( outputs >= 10 ) strncpy ( (char *)pgensub->valj, lowMode, MAX_STRING_SIZE-1 );
    }

    return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosStateInit
 *
 *   Purpose:
 *   Initialise the state to BOOTING where state values are stored as integers.
 *
 *   Description:
 *   This function generates a state value which corresponds to BOOTING. 
 *
 *   The function is designed to be used with the large (21 inputs/outputs)
 *   variety of the EPICS "genSub" record.
 *
 *   Invocation:
 *   status = gmosStateInit( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *
 *   Parameters out:
 *      < pgensub->vala long    Booting state value
 *      < pgensub->vald long    Booting state value
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   04-Aug-1999: Original GMOS version.                         (smb)
 *   
 *-
 */

long gmosStateInit( struct genSubRecord *pgensub ) 
{
    long status;                       /* return status */

/* Initialise the status */

    status = GOOD;

/* Output the booting state and a zero winning index. */

    *(long *)pgensub->vala = GM_SAD_STATE_BOOTING;
    *(long *)pgensub->valb = 0;
    *(long *)pgensub->valc = 0;
    *(long *)pgensub->vald = GM_SAD_STATE_BOOTING;

    return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosStateCombine
 *
 *   Purpose:
 *   Combine together multiple assembly states where state values are stored as integers.
 *
 *   Description:
 *   This function reads the states of up to ten assemblies, where
 *   the state is stored as an integer, and generates a combined state.
 *   It is assumed that the state values are designed so the higher the
 *   value the more important the state. 
 *
 *   The function is designed to be used with the large (21 inputs/outputs)
 *   variety of the EPICS "genSub" record.
 *
 *   Invocation:
 *   status = gmosStateCombine( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    long    State of assembly 1
 *      > pgensub->b    long    Command action status of assembly 1
 *      > pgensub->c    long    State of assembly 2
 *      > pgensub->d    long    Command action status of assembly 2
 *      > pgensub->e    long    State of assembly 3
 *      > pgensub->f    long    Command action status of assembly 3
 *      > pgensub->g    long    State of assembly 4
 *      > pgensub->h    long    Command action status of assembly 4
 *      > pgensub->i    long    State of assembly 5
 *      > pgensub->j    long    Command action status of assembly 5
 *      > pgensub->k    long    State of assembly 6
 *      > pgensub->l    long    Command action status of assembly 6
 *      > pgensub->m    long    State of assembly 7
 *      > pgensub->n    long    Command action status of assembly 7
 *      > pgensub->o    long    State of assembly 8
 *      > pgensub->p    long    Command action status of assembly 8
 *      > pgensub->q    long    State of assembly 9
 *      > pgensub->r    long    Command action status of assembly 9
 *      > pgensub->s    long    State of assembly 10
 *      > pgensub->t    long    Command action status of assembly 10
 *      > pgensub->u    long    Master enable state (0=disabled, 1=enabled)
 *
 *   Parameters out:
 *      < pgensub->vala long    Resulting state value combined with command action status
 *      < pgensub->valb long    Resulting state value ignoring command action status
 *      < pgensub->valc long    Index of winning state input
 *      < pgensub->vald long    State converted to GMOS SAD state
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   04-Aug-1999: Original GMOS version.                                (smb)
 *   02-Nov-2000: State should be BOOTING until master enable enabled.  (smb)
 *   
 *-
 */

long gmosStateCombine( struct genSubRecord *pgensub ) 
{
    long status;                       /* return status */
    long outState1;                    /* Output state value with command action status */
    long outState2;                    /* Output state value without command action status */
    long outIndex;                     /* Index of winning health input */

/* Initialise the status */

    status = GOOD;

/* The output state will be BOOTING until master enable is enabled. */

    if ( *(long *)pgensub->u == 0 )
    {

       *(long *)pgensub->vala = GM_SAD_STATE_BOOTING;
       *(long *)pgensub->valb = 0;
       *(long *)pgensub->valc = 0;
       *(long *)pgensub->vald = GM_SAD_STATE_BOOTING;
    }
    else
    {
    

/*
 * Find the maximum of all the state inputs and write this to outState2.
 * The default value is the very first one.
 */

       outState2 = *(long *)pgensub->a;
       outIndex = 1;

       if ( (pgensub->c != NULL) && (*(long *)pgensub->c > outState2) )
       {
           outState2 = *(long *)pgensub->c;
           outIndex = 2;
       }
       if ( (pgensub->e != NULL) && (*(long *)pgensub->e > outState2) )
       {
           outState2 = *(long *)pgensub->e;
           outIndex = 3;
       }
       if ( (pgensub->g != NULL) && (*(long *)pgensub->g > outState2) )
       {
           outState2 = *(long *)pgensub->g;
           outIndex = 4;
       }
       if ( (pgensub->i != NULL) && (*(long *)pgensub->i > outState2) )
       {
           outState2 = *(long *)pgensub->i;
           outIndex = 5;
       }
       if ( (pgensub->k != NULL) && (*(long *)pgensub->k > outState2) )
       {
           outState2 = *(long *)pgensub->k;
           outIndex = 6;
       }
       if ( (pgensub->m != NULL) && (*(long *)pgensub->m > outState2) )
       {
           outState2 = *(long *)pgensub->m;
           outIndex = 7;
       }
       if ( (pgensub->o != NULL) && (*(long *)pgensub->o > outState2) )
       {
           outState2 = *(long *)pgensub->o;
           outIndex = 8;
       }
       if ( (pgensub->q != NULL) && (*(long *)pgensub->q > outState2) )
       {
           outState2 = *(long *)pgensub->q;
           outIndex = 9;
       }
       if ( (pgensub->s != NULL) && (*(long *)pgensub->s > outState2) )
       {
           outState2 = *(long *)pgensub->s;
           outIndex = 10;
       }

/*
 * Copy the resulting status to outState1, unless one of the command action inputs
 * indicates an error, in which case set the output state to ERROR.
 */

       if ( (pgensub->b != NULL) && (*(long *)pgensub->b == CAR_ERROR) )
       {
           outState1 = DAR_STATE_ERROR;
       }
       else if ( (pgensub->d != NULL) && (*(long *)pgensub->d == CAR_ERROR) )
       {
           outState1 = DAR_STATE_ERROR;
       }
       else if( (pgensub->f != NULL) && (*(long *)pgensub->f == CAR_ERROR) )
       {
           outState1 = DAR_STATE_ERROR;
       }
       else if( (pgensub->h != NULL) && (*(long *)pgensub->h == CAR_ERROR) )
       {
           outState1 = DAR_STATE_ERROR;
       }
       else if( (pgensub->j != NULL) && (*(long *)pgensub->j == CAR_ERROR) )
       {
           outState1 = DAR_STATE_ERROR;
       }
       else if( (pgensub->l != NULL) && (*(long *)pgensub->l == CAR_ERROR) )
       {
           outState1 = DAR_STATE_ERROR;
       }
       else if( (pgensub->n != NULL) && (*(long *)pgensub->n == CAR_ERROR) )
       {
           outState1 = DAR_STATE_ERROR;
       }
       else if( (pgensub->p != NULL) && (*(long *)pgensub->p == CAR_ERROR) )
       {
           outState1 = DAR_STATE_ERROR;
       }
       else if( (pgensub->r != NULL) && (*(long *)pgensub->r == CAR_ERROR) )
       {
           outState1 = DAR_STATE_ERROR;
       }
       else if( (pgensub->t != NULL) && (*(long *)pgensub->t == CAR_ERROR) )
       {
           outState1 = DAR_STATE_ERROR;
       }
       else
       {
           outState1 = outState2;
       }

/* Output the resulting state value, message and index. */

       *(long *)pgensub->vala = outState1;
       *(long *)pgensub->valb = outState2;
       *(long *)pgensub->valc = outIndex;

/* Output the GMOS SAD state */

       if (outState2 == DAR_STATE_INITIALIZING)
       {
           *(long *)pgensub->vald = GM_SAD_STATE_INITIALIZING;
       }
       else if ((outState2 == DAR_STATE_IDLE) || (outState1 == DAR_STATE_TESTING))
       {
           *(long *)pgensub->vald = GM_SAD_STATE_RUNNING;
       }
       else
       {
           *(long *)pgensub->vald = GM_SAD_STATE_CONFIGURING;
       }
    }

    return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosIockInit
 *
 *   Purpose:
 *   Initialise the interlock event.
 *
 *   Description:
 *   This function initialises the interlock output to zero (no interlock). 
 *
 *   Invocation:
 *   status = gmosIlockInit( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *
 *   Parameters out:
 *      < pgensub->vala long    Interlock output value (0=OK, 1=interlock)
 *      < pgensub->valc long    Combined interlock status word (0x0 if ok)
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   21-Jan-2000: Original GMOS version.                         (smb)
 *   
 *-
 */

long gmosIlockInit( struct genSubRecord *pgensub ) 
{
    long status;                       /* return status */

/* Initialise the status */

    status = GOOD;

/* Initialise the interlock outputs. */

    *(long *)pgensub->vala = 0;
    *(long *)pgensub->valc = 0;

    return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosIlockCombine
 *
 *   Purpose:
 *   Combine together GMOS interlock signals and decide whether to assert 
 *   an interlock.
 *
 *   Description:
 *   This function reads the GMOS interlock signals. If an interlock is 
 *   detected the function reports it and changes the output value from 
 *   0 to 1, which will trigger an interlock throughout the mechanism control
 *   software. In simulation mode the hardware interlocks are ignored but a
 *   software flag may be used to simulate an interlock. In both cases
 *   the software flag may be used as a software "emergency stop" button.
 *
 *   Hardware interlocks are only responded to when a delayed "master enable" 
 *   signal is set to 1. This prevents the software responding to temporary
 *   interlocks reported by the electronics when the system is first startup up.
 *   
 *
 *   Invocation:
 *   status = gmosIlockCombine( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    long    GMOS interlock event         (1 if ok)
 *      > pgensub->b    long    GMOS interlock event error   (1 if ok)
 *      > pgensub->c    long    GIS interlock demand         (1 if ok)
 *      > pgensub->d    long    GIS interlock demand error   (1 if ok)
 *      > pgensub->e    long    GMOS motor shutdown          (0 if ok)
 *      > pgensub->f    long    GMOS door interlock defeat   (1 if ok)
 *      > pgensub->g    long    Door interlocks (all 8)      (0 if ok)
 *      > pgensub->h    long    Soft interlock               (0 if ok)
 *      > pgensub->i    long    Delayed master enable        (1 if enabled)
 *      > pgensub->j    long    Simulation flag              (>0 if simulating)
 *
 *   Parameters out:
 *      < pgensub->vala long    Interlock output value 
 *                              (0 if ok, 1 to generate interlock)
 *      < pgensub->valb string  Interlock message
 *      < pgensub->valc long    Combined interlock status word (0x0 if ok)
 *
 *   Return value:
 *      < status        long    Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *      The EPICS database should arrange for the masterEnable input to 
 *      change value a short delay after the real masterEnable signal
 *      changes. This gives the electronics time to cancel interlock 
 *      signals after masterEnable has gone high before this software
 *      starts reacting to those signals.
 *
 *   Limitations:
 *      logMsg is a delayed action message display function which allows
 *      the code in this genSub to execute in parallel. The advantage of
 *      using logMsg is that it speeds 
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   21-Jan-2000: Original GMOS version.                            (smb)
 *   25-Feb-2000: Only react to an interlock when master enable is  (smb)
 *                on. This prevents interlocks being reported 
 *                erroneously at system startup.             
 *   27-Sep-2000: Only report a message to the console when the     (smb)
 *                interlock status changes. This prevents message
 *                storms. Also send message to VALB.    
 *   26-Jan-2001: Change to match new interlock hardware            (bmw)
 *   28-Feb-2001: The logMsg statements should use string constants
 *                rather than the "message" buffer, to prevent the
 *                buffer changing before logMsg has had a chance
 *                to display it.                                    (smb)
 *
 *   
 *-
 */

long gmosIlockCombine( struct genSubRecord *pgensub ) 
{
    long status;                        /* return status                  */

    long gisEvent, gisEventError;       /* GIS event and event error      */
    long gisDemand, gisDemandError;     /* GIS interlock demand to GMOS   */
    long motorShutdown;                 /* GMOS motor shutdown signal     */
    long doorIlckDefeat;                /* GMOS door interlock defeat     */
    long doorInterlocks;                /* 7 door interlocks plus e-stop  */
    long softIlock;                     /* Software emergency stop        */
    long simflag;                       /* Simulation mode (not used yet) */
    long masterEnable;                  /* Delayed master enable          */
    long ccInterlock;                   /* Desired interlock state        */
    long prevCcInterlock;               /* Previous interlock state       */
    long interlockStatus;               /* Combined interlock status word */
    long prevInterlockStatus;           /* Prev. combined interlock status*/

    char message[MESS_BUFF_SIZE];       /* Output message                 */


    /* Initialise the status and message */

    status = GOOD;
    message[0] = '\0';

    /*
     * Get the current interlock values from the hardware.
     */
                                                  /* active low? */ 
    gisEvent            = *(long *)pgensub->a;    /*   yes       */
    gisEventError       = *(long *)pgensub->b;    /*   yes       */
    gisDemand           = *(long *)pgensub->c;    /*   yes       */
    gisDemandError      = *(long *)pgensub->d;    /*   yes       */
    motorShutdown       = *(long *)pgensub->e;
    doorIlckDefeat      = *(long *)pgensub->f;    /*   yes       */
    doorInterlocks      = *(long *)pgensub->g;
    softIlock           = *(long *)pgensub->h;
    masterEnable        = *(long *)pgensub->i;    /*   yes       */
    simflag             = *(long *)pgensub->j;


    /* 
     * Save the old interlock state and combined status word. If this is
     * the first time this function is called the VALA and VALC fields will
     * have been initialised in gmosIlockInit
     */

    prevCcInterlock     = *(long *)pgensub->vala;
    prevInterlockStatus = *(long *)pgensub->valc;

    /*
     * Combine all the individual interlock pieces into a single status word.
     *
     * Bitmap mnemonics for GMOS interlock status (note that door interlocks
     * are already combined in this order - see gmosDoorIlockCombine function):
     * 
     *                                 error
     *                                 state
     *
     * door1                 0x00000001  1
     * door2                 0x00000002  1
     * door3                 0x00000004  1
     * door4                 0x00000008  1
     * door5                 0x00000010  1
     * door6                 0x00000020  1
     * door7                 0x00000040  1
     * door8                 0x00000080  1
     * Door Interlock Defeat 0x00000100  0	
     * GMOS Motor Shutdown   0x00000200  1
     * GMOS Event            0x00000400  0
     * GMOS Event Error      0x00000800  0
     * GIS Demand            0x00001000  0
     * GIS Demand Error      0x00002000  0
     * Soft Interlock        0x00004000  1
     * Master Enable         0x00008000  0
     * Simulation mode       0x00010000  n/a
     *
     * When combining, change the error states of the error-when-0 items
     * so that they are all error-when-1.
     */

    interlockStatus = (  doorInterlocks     | !doorIlckDefeat<<8 | 
                         motorShutdown<<9   | !gisEvent<<10      | 
                        !gisEventError<<11  | !gisDemand<<12     | 
                        !gisDemandError<<13 |  softIlock<<14     |
                        !masterEnable<<15   |  simflag<<16         );

    /*
     * Issue a message to the console whenever a status item changes.
     */

    if (interlockStatus != prevInterlockStatus)
    {
#ifdef VERBOSE
        logMsg( "gmosIlockCombine: *** new interlock status: %#x  (old status: %#x )  ***\n",
                interlockStatus, prevInterlockStatus, 0, 0, 0, 0);
#endif

        if ( (interlockStatus     & 0x00000001) != 
             (prevInterlockStatus & 0x00000001)    )
        {
            if (doorInterlocks & 0x00000001)
               logMsg ("gmosIlockCombine: *** E.E.1 Front Door is OPEN!            ***\n",
                        0,0,0,0,0,0);
            else
               logMsg ("gmosIlockCombine:     E.E.1 Front Door is now closed.\n",
                        0,0,0,0,0,0);

        }

        if ( (interlockStatus     & 0x00000002) != 
             (prevInterlockStatus & 0x00000002)   )
        {
            if (doorInterlocks & 0x00000002)
               logMsg ("gmosIlockCombine: *** E.E.1 Back Door is OPEN!             ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     E.E.1 Back Door is now closed.\n",
                        0,0,0,0,0,0);

        }

        if ( (interlockStatus     & 0x00000004) != 
             (prevInterlockStatus & 0x00000004)   )
        {
            if (doorInterlocks & 0x00000004)
               logMsg ("gmosIlockCombine: *** E.E.2 Front Door is OPEN!            ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     E.E.2 Front Door is now closed.\n",
                        0,0,0,0,0,0);

        }

        if ( (interlockStatus     & 0x00000008) != 
             (prevInterlockStatus & 0x00000008)   )
        {
            if (doorInterlocks & 0x00000008)
               logMsg ("gmosIlockCombine: *** E.E.2 Back Door is OPEN!             ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     E.E.2 Back Door is now closed.\n",
                        0,0,0,0,0,0);

        }

        if ( (interlockStatus     & 0x00000010) != 
             (prevInterlockStatus & 0x00000010)   )
        {
            if (doorInterlocks & 0x00000010)
               logMsg ("gmosIlockCombine: *** Grating/Camera Door is OPEN!         ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     Grating/Camera Door is now closed.\n",
                        0,0,0,0,0,0);

        }

        if ( (interlockStatus     & 0x00000020) != 
             (prevInterlockStatus & 0x00000020)   )
        {
            if (doorInterlocks & 0x00000020)
               logMsg ("gmosIlockCombine: *** Mask Door is OPEN!                   ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     Mask Door is now closed.\n",
                        0,0,0,0,0,0);

        }

        if ( (interlockStatus     & 0x00000040) != 
             (prevInterlockStatus & 0x00000040)   )
        {
            if (doorInterlocks & 0x00000040)
               logMsg ("gmosIlockCombine: *** Grating/Filter Door is OPEN!         ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     Grating/Filter Door is now closed.\n",
                        0,0,0,0,0,0);

        }

        if ( (interlockStatus     & 0x00000080) != 
             (prevInterlockStatus & 0x00000080)   )
        {
            if (doorInterlocks & 0x00000080)
               logMsg ("gmosIlockCombine: *** Emergency Stop activated!            ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     Emergency Stop cleared.\n",
                        0,0,0,0,0,0);
        }

        if ( (interlockStatus     & 0x00000100) != 
             (prevInterlockStatus & 0x00000100)   )
        {
            if (doorIlckDefeat == 0)
               logMsg ("gmosIlockCombine: *** Door Interlocks defeated!            ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     Door Interlocks not defeated.\n",
                        0,0,0,0,0,0);
        }

        if ( (interlockStatus     & 0x00000200) !=
             (prevInterlockStatus & 0x00000200)   )
        {
            if (motorShutdown == 1)
               logMsg ("gmosIlockCombine: *** Shutting down GMOS motors!!          ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     GMOS Motors back online.\n",
                        0,0,0,0,0,0);

        }

        if ( (interlockStatus     & 0x00000400) != 
             (prevInterlockStatus & 0x00000400)   )
        {
            if (gisEvent == 0)
               logMsg ("gmosIlockCombine: *** GMOS Interlock Event!                ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     GMOS Interlock Event cleared.\n",
                        0,0,0,0,0,0);

        }


        if ( (interlockStatus     & 0x00000800) != 
             (prevInterlockStatus & 0x00000800)   )
        {
            if (gisEventError == 0)
               logMsg ("gmosIlockCombine: *** Invalid GMOS Interlock Event State!  ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     GMOS Interlock Event state error cleared.\n",
                        0,0,0,0,0,0);
        }

        if ( (interlockStatus     & 0x00001000) != 
             (prevInterlockStatus & 0x00001000)   )
        {
            if (gisDemand == 0)
               logMsg ("gmosIlockCombine: *** GIS Interlock Demand!                ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     GIS Interlock Demand cleared.\n",
                        0,0,0,0,0,0);
        }

        if ( (interlockStatus     & 0x00002000) != 
             (prevInterlockStatus & 0x00002000)   )
        {
            if (gisDemandError == 0)
               logMsg ("gmosIlockCombine: *** Invalid GIS Interlock Demand state!  ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     GIS Interlock Demand state error cleared.\n",
                        0,0,0,0,0,0);
        }

        if ( (interlockStatus     & 0x00004000) != 
             (prevInterlockStatus & 0x00004000)   )
        {
            if (softIlock != 0)
               logMsg ("gmosIlockCombine: *** GMOS soft interlock!                 ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     GMOS soft interlock cleared.\n",
                        0,0,0,0,0,0);
        }

        if ( (interlockStatus     & 0x00008000) != 
             (prevInterlockStatus & 0x00008000)   )
        {
            if (masterEnable == 0)
               logMsg ("gmosIlockCombine: *** GMOS Master Enable disabled!         ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     GMOS Master Enable enabled.\n",
                        0,0,0,0,0,0);
        }

        if ( (interlockStatus     & 0x00010000) != 
             (prevInterlockStatus & 0x00010000)   )
        {
            if (simflag != 0)
               logMsg ("gmosIlockCombine: *** GMOS in Simulation Mode!             ***\n",
                        0,0,0,0,0,0);

            else
               logMsg ("gmosIlockCombine:     GMOS is no longer in Simulation Mode.\n",
                        0,0,0,0,0,0);
        }
    }   /* end of if (interlockStatus != prevInterlockStatus) */


    /* 
     *  The CC can only be interlocked by the hardware interlock (motor shutdown)
     *  or by a soft interlock.
     *
     *  The motor shutdown is checked only if the bootup disable signal 
     *  is enabled (high) and the software is not running in simulation mode.
     * 
     *  The soft interlock input is checked regardless of simulation mode and 
     *  value of the master enable signal. 
     */

    if ( (simflag == DAR_SIM_NONE && masterEnable  == 1 && motorShutdown == 1) ||
         (softIlock != 0) )
    {
        ccInterlock = 1;

        if (ccInterlock != prevCcInterlock)
        {
            logMsg( "gmosIlockCombine: *** Asserting GMOS CC Interlock!!        ***\n", 0, 0, 0, 0, 0, 0);
        }

        /*
         *  Build an interlock status message. The message has to be
         *  extremely terse because it has to fit into MAX_STRING_SIZE 
         *  characters.
         */

        sprintf( message, "Interlock:%.4s%.4s%.4s%.4s%.4s%.4s Status:%#x",
            (!gisEvent       ? " Evt" : " "),
            (!gisEventError  ? "+Err" : " "),
            (!gisDemand      ? " Dmd" : " "),
            (!gisDemandError ? "+Err" : " "),
            (motorShutdown   ? " Sht" : " "),
            (softIlock       ? " Sft" : " "),
             interlockStatus );
    }
    else
    {
        /*  
         *  No CC Interlock, so issue a message to the console (if it was
         *  previously interlocked) and clear the error message string.
         */

        ccInterlock = 0;

        if (ccInterlock != prevCcInterlock)
        {
            logMsg( "gmosIlockCombine:     Removing GMOS CC Interlock.\n", 
                        0, 0, 0, 0, 0, 0);
        }
        strcpy( message, " " );
    }


    /*
     *  Updated the record outputs.
     */
 
    *(long *)pgensub->vala = ccInterlock;
    strncpy( pgensub->valb, message, MAX_STRING_SIZE-1 );
    *(long *)pgensub->valc = interlockStatus;

    return status;
}

/* ===================================================================== */

/*+
 *   Function name:
 *   gmosStringConcat
 *
 *   Purpose:
 *   Concatenate two strings together.
 *
 *   Description:
 *   This function concatenates two input strings together to make one
 *   output string. The strings are separated with a "+" symbol.
 *
 *   For example, this function can be used to combine two "state" strings
 *   together (such as "moving" and "stopped") to make a combined state
 *   ("moving+stopped").
 *
 *   The function is designed to be used with the EPICS "genSub" record.
 *
 *   Invocation:
 *   status = gmosStringConcat( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    string  Input string 1
 *      > pgensub->b    string  Input string 2
 *
 *   Parameters out:
 *      < pgensub->vala string  Resulting output string
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *   If the combined string is longer than MAX_STRING_SIZE, the function
 *   will simply chop off the right hand part of the string. No attempt
 *   is made to truncate both input strings in an intelligent way.
 *
 *   The function could be extended to work with more than two strings.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   18-Jul-1997: Original CICS version.                            (smb)
 *   08-Jul-1999: More flexible GMOS version.                       (smb)
 *-
 */

long gmosStringConcat( struct genSubRecord *pgensub ) 
{
  long status;                         /* return status */
  char string[MAX_STRING_SIZE * 2];    /* Resulting string */

/* Initialise the status */

  status = GOOD;

/* Build up the output string */

  sprintf( string, "%s + %s", (char *)pgensub->a, (char *)pgensub->b );

/*
 * Copy as much of the string as possible to the output, truncating it
 * if it is longer than MAX_STRING_SIZE-1.
 */

  strncpy( (char *)pgensub->vala, string, MAX_STRING_SIZE-1 );

  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosStringFilter
 *
 *   Purpose:
 *   Filter out any string matching a given tag.
 *
 *   Description:
 *   This function passes a string from input A to output A, but only if
 *   it does not match a tag provided in input B.
 *
 *   For example, this function can be used to allow the upper layers of
 *   software to request any position from the lower layers except "kill",
 *   or it can be used to prevent a new attribute being passed on when the
 *   user has set it to "don't care". 
 *
 *   The function is designed to be used with the EPICS "genSub" record.
 *
 *   Invocation:
 *   status = gmosStringFilter( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    string  Input string
 *      > pgensub->b    string  Tag to be filtered out
 *
 *   Parameters out:
 *      < pgensub->vala string  Output string
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *   It is probably more efficient to do this string filtering in device
 *   support rather than in a genSub,
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   18-Aug-1997: Original version.                                (smb)
 *-
 */

long gmosStringFilter( struct genSubRecord *pgensub ) 
{
  long status;                         /* return status */

/* Initialise the status */

  status = GOOD;

/* Test the string against the tag provided, and only copy it to the output
 * if the string is non blank and does not match the tag.
 */

  if ( (strlen((char *)pgensub->a) != 0) &&
       (strspn((char *)pgensub->a," ") != strlen((char *)pgensub->a)) )
  {

    if ( strncmp((char *)pgensub->a, (char *)pgensub->b, MAX_STRING_SIZE-1) != 0 )
    {

/* Match with tag not found. Copy the input to the output. */

      strncpy( (char *)pgensub->vala, (char *)pgensub->a, MAX_STRING_SIZE-1 );
    }
  }
  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosCheckDirective
 *
 *   Purpose:
 *   Report and check directives passed through.
 *
 *   Description:
 *
 *   Invocation:
 *   status = gmosCheckDirective( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    long  Input directive
 *
 *   Parameters out:
 *      < pgensub->vala long  Output string
 *
 *   Return value:
 *      < status       long   Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   15-Dec-1999: Original version.                                (smb)
 *-
 */

long gmosCheckDirective( struct genSubRecord *pgensub ) 
{
  long status;                         /* return status */

  long dir;
  long ticks;

/* Initialise the status */

  status = GOOD;

  dir = *(long *) pgensub->a;
  ticks = (long) tickGet();

  switch ( dir )
  {
      case CAD_MARK:
         printf ("gmosCheckDirective: MARK directive at %ld ticks\n", ticks);
         break;

      case CAD_CLEAR:
         printf ("gmosCheckDirective: CLEAR directive at %ld ticks\n", ticks);
         break;

      case CAD_PRESET:
         printf ("gmosCheckDirective: PRESET directive at %ld ticks\n", ticks);
         break;

      case CAD_START:
         printf ("gmosCheckDirective: START directive at %ld ticks\n", ticks);
         break;

      case CAD_STOP:
         printf ("gmosCheckDirective: STOP directive at %ld ticks\n", ticks);
         break;

      default:
         printf ("gmosCheckDirective: unrecognised directive, %ld, at %ld ticks\n", dir, ticks);
         break;

  }

  *(long *) pgensub->vala = dir;

  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosDtaTemperatureSimInit
 *
 *   Purpose:
 *   Setup Low pass filtering algorithm for dta internal temperature simulation.
 *
 *   Description:
 *
 *   Invocation:
 *   status = gmosDtaTemperatureSimInit( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    double  Input measured external dta temperature
 *
 *   Parameters out:
 *      < pgensub->vala double  Output measured external dta temperature
 *      < pgensub->valb double  Output filtered temperature 1
 *      < pgensub->valc double  Output filtered temperature 2
 *      < pgensub->vald double  Scan period of this record
 *
 *   Return value:
 *      < status       long   Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Chris Tierney  (cjt@roe.ac.uk)
 *
 *   History:
 *   16-May-2000: Original version.                                (cjt)
 *   23-Aug-2000: Added second filtered temperature                (cjt)
 *-
 */

long gmosDtaTemperatureSimInit (struct genSubRecord *pgensub) 
{
     long status = GOOD;                         /* return status */
     
     if (pgensub->scan < FIRST_PERIODIC_SCAN)
     {
          printf ("gmosDtaTemperatureSimInit: gensub SCAN field must be periodic\n");
          pgensub->disa = pgensub->disv;
          *(double *)pgensub->vald = 0;
          status = BAD;
     }
     else if (pgensub->scan >= 11)
     {
          printf ("gmosDtaTemperatureSimInit: gensub SCAN rate too high, filter may not generate correct time constant\n");
          *(double *)pgensub->vald = scanPeriods[pgensub->scan - FIRST_PERIODIC_SCAN];
          status = BAD;
     } 
     else
     {
          *(double *)pgensub->vald = scanPeriods[pgensub->scan - FIRST_PERIODIC_SCAN];
     }

     *(long *)pgensub->vale = 1; /* To indicate the first pass */

     return (status);
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosDtaTemperatureSim
 *
 *   Purpose:
 *   Low pass filtering algorithm for dta internal temperature simulation.
 *
 *   Description:
 *
 *   Invocation:
 *   status = gmosDtaTemperatureSim( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    double  Input measured external dta temperature
 *      > pgensub->b    double  Time constant for the filtering algorithm
 *      > pgensub->vald double  Scan rate of this record
 *
 *   Parameters out:
 *      < pgensub->vala double  Output measured external dta temperature
 *      < pgensub->valb double  Output filtered temperature 1
 *      < pgensub->valc double  Output filtered temperature 2
 *
 *   Return value:
 *      < status       long   Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Chris Tierney  (cjt@roe.ac.uk)
 *
 *   History:
 *   16-May-2000: Original version.                                (cjt)
 *   23-Aug-2000: Added second filtered temperature output.        (cjt)
 *-
 */

long gmosDtaTemperatureSim (struct genSubRecord *pgensub) 
{
     long status  = GOOD;                         /* return status */
     double k1, k2;
     double alpha1, alpha2;
    
     if (* (long *)pgensub->vale)
     {
          /* Assume equilibrium upon startup */
          
          *(double *)pgensub->vala = *(double *)pgensub->a;
          *(double *)pgensub->valb = *(double *)pgensub->a;
          *(double *)pgensub->valc = *(double *)pgensub->a;

          *(long *)pgensub->vale = 0;
     }
     else
     {
          /* Filter thereafter */

          alpha1 = exp (- (*(double *)pgensub->vald / *(double *)pgensub->b));
          alpha2 = exp (- (*(double *)pgensub->vald / *(double *)pgensub->c));
          k1 = (1 - alpha1) / 2;
          k2 = (1 - alpha2) / 2;
          
          *(double *)pgensub->valc = (k2 * (*(double *)pgensub->a + *(double *)pgensub->vala)) + (alpha2 * *(double *)pgensub->valc);
          *(double *)pgensub->valb = (k1 * (*(double *)pgensub->a + *(double *)pgensub->vala)) + (alpha1 * *(double *)pgensub->valb);
          *(double *)pgensub->vala = *(double *)pgensub->a;
     }

     return (status);
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosTemperatureChangeInit
 *
 *   Purpose:
 *   Initialisation function for gmosTemperatureChange.
 *
 *   Description:
 *   This function initialises the watchdog timer structures used by the rate of
 *   change of temperature calculation function.
 *
 *   Invocation:
 *   status = gmosTemperatureChangeInit( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      None
 *
 *   Parameters out:
 *      None
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   29-Aug-2000: Original GMOS version.                         (smb)
 *   05-Sep-2000: Added array of previous changes.               (smb)
 *   
 *-
 */

long gmosTemperatureChangeInit( struct genSubRecord *pgensub ) 
{
    long               status;             /* return status				*/
    TEMPCHANGE_PRIVATE *pPriv = NULL;      /* Pointer to device private structure.	*/
    struct callback    *pcallback = NULL;  /* Pointer to callback structure.		*/
    int                i;                  /* Loop counter.				*/

#ifdef VERBOSE
    printf ("gmosTemperatureChangeInit: Entry.\n");
#endif

/* Initialise the status */

    status = GOOD;

/*
 * Create a private structure for this record.
 */

    pPriv = (TEMPCHANGE_PRIVATE *) malloc (sizeof(TEMPCHANGE_PRIVATE));
    if ( pPriv == NULL ) 
    {
        printf ("gmosTemperatureChangeInit: Failed to malloc device private structure.\n");
        status = BAD;
        return (status);
    }
    pgensub->dpvt = (void *) pPriv;

/*
 *  Create the MUTEX semaphore to protect access to the data within the
 *  private structure at initialisation time.
 */
     
    if ( (pPriv->dataLock = semBCreate( SEM_Q_FIFO, SEM_FULL) ) == NULL )
    {
        printf ("gmosTemperatureChangeInit: Failed to create dataLock semaphore.\n");
        status = BAD;
        return (status);
    }
    semTake( pPriv->dataLock, WAIT_FOREVER );

/*
 * Initialise the private structure, setting the initial previous temperatures
 * to a negative initial value well below absolute zero.
 */

    pPriv->timeout = FALSE;
    pPriv->timer = NULL;

    for ( i=0; i<TEMPCHANGE_NSENSORS; i++)
    {
        pPriv->previousTemp[i] = -1000.0;
        pPriv->previousDiff[i] = 0.0;
        pPriv->prevPrevDiff[i] = 0.0;
    }
    semGive( pPriv->dataLock );


/*
 *  Create the MUTEX semaphore to protect the timeout in the
 *  private structure during asynchronous callback access.
 */
     
    if ( (pPriv->cmdLock = semBCreate( SEM_Q_FIFO, SEM_FULL) ) == NULL )
    {
        printf ("gmosTemperatureChangeInit: Failed to create cmdLock semaphore.\n");
        status = BAD;
        return (status);
    }

/*
 *  Make room for the timer callback structure and set it up.
 */

    pcallback = (struct callback *)(calloc(1,sizeof(struct callback)));
    if ( pcallback == NULL )
    {
        printf ("gmosTemperatureChangeInit: Failed to calloc callback structure.\n");
        status = BAD;
        return ( status );
    }

    pPriv->timer = (void *)pcallback;
    callbackSetCallback( tempchangeCallback, &pcallback->callback );
    pcallback->pRecord = (struct dbCommon *)pgensub;

    if ( ( pcallback->wd_id = wdCreate() ) == NULL )
    { 
        printf ("gmosTemperatureChangeInit: Failed to create watchdog.\n");
        status = BAD;
        return (status);
    }
    callbackSetPriority( priorityLow, &pcallback->callback );

    return (status);
}


/* ===================================================================== */

/*+
 *   Function name:
 *   gmosTemperatureChange
 *
 *   Purpose:
 *   Calculate the hourly temperature change.
 *
 *   Description:
 *   This function rechedules itself every TEMPCHANGE_INTERVAL seconds and uses
 *   the differences in the input temperatures to calculate the hourly rate of
 *   temperature change. The hourly rate prediction is smoothed using the previous
 *   two measurements to reduce the effect of noise in the temperature readings.
 *   The changes are written to the outputs. A combined average rate of change is
 *   also calculated, ignoring zero changes from disconnected or disabled sensors.
 *
 *   Invocation:
 *   status = gmosTemperatureCombine( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    double    Temperature 0
 *      > pgensub->b    double    Temperature 1
 *      > pgensub->c    double    Temperature 2
 *      > pgensub->d    double    Temperature 3
 *      > pgensub->e    double    Temperature 4
 *      > pgensub->f    double    Temperature 5
 *      > pgensub->g    double    Temperature 6
 *      > pgensub->h    double    Temperature 7
 *      > pgensub->i    double    Temperature 8
 *
 *   Parameters out:
 *      < pgensub->vala double    Hourly rate of change 0
 *      < pgensub->valb double    Hourly rate of change 1
 *      < pgensub->valc double    Hourly rate of change 2
 *      < pgensub->vald double    Hourly rate of change 3
 *      < pgensub->vale double    Hourly rate of change 4
 *      < pgensub->valf double    Hourly rate of change 5
 *      < pgensub->valg double    Hourly rate of change 6
 *      < pgensub->valh double    Hourly rate of change 7
 *      < pgensub->vali double    Hourly rate of change 8
 *      < pgensub->valj double    Combined average rate of change
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *      The gensub record to which this function is attached needs to have its PINI
 *      field set to YES to kick off the first execution. Thereafter the watchdog timer
 *      will run the function at regular intervals.
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   29-Aug-2000: Original GMOS version.                         (smb)
 *   
 *-
 */

long gmosTemperatureChange( struct genSubRecord *pgensub ) 
{
    long   status;          /* return status				*/
    double temperature[TEMPCHANGE_NSENSORS];
                            /* Input temperatures.			*/
    double diff, smoothed;  /* Actual and smoothed differences.         */
    double change[TEMPCHANGE_NSENSORS];
                            /* Changes in temperature.			*/
    double factor;          /* Multiplication factor for hourly change.	*/                            
    double mean;            /* Mean temperature change.			*/
    int    i, count;        /* Loop counter and general counter.	*/

    TEMPCHANGE_PRIVATE 
                *pPriv;     /* Pointer to device private structure.	*/
    struct callback
                *pCallback; /* Structure passed to callback watchdog.	*/
    int    waitTime;        /* Time delay given to callback watchdog.	*/

    status = GOOD;

    /*
     * Extract the address of the device private structure from the gensub record
     * structure, then extract the address of the callback structure from the device
     * private structure. Abort if either of these is NULL.
     */

    pPriv = (TEMPCHANGE_PRIVATE *) (pgensub->dpvt);
    if ( pPriv == NULL )
    {
        printf ("gmosTemperatureChange: NULL pPriv pointer.\n");
        return (BAD);
    }

    pCallback = (struct callback *)(pPriv->timer);
    if ( pCallback == NULL )
    {
        printf ("gmosTemperatureChange: NULL pCallback pointer.\n");
        return (BAD);
    }

    /* Obtain the current temperatures from fields A-I */

    temperature[0] = *(double *) pgensub->a;
    temperature[1] = *(double *) pgensub->b;
    temperature[2] = *(double *) pgensub->c;
    temperature[3] = *(double *) pgensub->d;
    temperature[4] = *(double *) pgensub->e;
    temperature[5] = *(double *) pgensub->f;
    temperature[6] = *(double *) pgensub->g;
    temperature[7] = *(double *) pgensub->h;
    temperature[8] = *(double *) pgensub->i;

#ifdef VERBOSE
    printf ("gmosTemperatureChange: Temperatures and previous temperatures:\n");
    for (i=0; i<TEMPCHANGE_NSENSORS; i++)
    {
        printf ("%d: %f %f\n", i, temperature[i], pPriv->previousTemp[i]);
    }
#endif


    /*
     * Calculate the temperature differences, assuming that a large negative value
     * is the initial value. Ignore a previous temperature of exactly 0.00000 as this
     * suggests the previous input had not yet been initialised.
     */

    factor = 3600.0/(double) TEMPCHANGE_INTERVAL;

    semTake( pPriv->dataLock, WAIT_FOREVER );

    for (i=0; i<TEMPCHANGE_NSENSORS; i++)
    {
        if ( pPriv->previousTemp[i] < -300.0 )
        {
            /* Initial value */
            change[i] = 0.0;
            pPriv->prevPrevDiff[i] = 0.0;
            pPriv->previousDiff[i] = 0.0;
        }
        else if ( fabs(pPriv->previousTemp[i]) < 1.0e-6 )
        {
            /* Previous input was not yet initialised. Reset */
            change[i] = 0.0;
            pPriv->prevPrevDiff[i] = 0.0;
            pPriv->previousDiff[i] = 0.0;
        }
        else
        {
            /*
             * Calculate how the temperature has changed in TEMPCHANGE_INTERVAL seconds,
             * smooth this difference by taking a (1.0, 1.0, 0.5) weighted mean with the
             * two previous values, then multiply it by "factor" to predict the hourly
             * temperature change.
             */

            diff = temperature[i] - pPriv->previousTemp[i];
            smoothed = (diff + pPriv->previousDiff[i] + (0.5*pPriv->prevPrevDiff[i]))
                       / (double) 2.5;
            change[i] = factor * smoothed;
            pPriv->prevPrevDiff[i] = pPriv->previousDiff[i];
            pPriv->previousDiff[i] = diff;
        }
        pPriv->previousTemp[i] = temperature[i];
    }

#ifdef VERBOSE
    printf ("gmosTemperatureChange: Changes and previous changes:\n");
    for (i=0; i<TEMPCHANGE_NSENSORS; i++)
    {
        printf ("%d: %f %f %f\n", i, change[i], pPriv->previousDiff[i], pPriv->prevPrevDiff[i]);
    }
#endif

    semGive( pPriv->dataLock );


    /* Output the temperature changes to fields VALA-VALI */

    *(double *)pgensub->vala = change[0];
    *(double *)pgensub->valb = change[1];
    *(double *)pgensub->valc = change[2];
    *(double *)pgensub->vald = change[3];
    *(double *)pgensub->vale = change[4];
    *(double *)pgensub->valf = change[5];
    *(double *)pgensub->valg = change[6];
    *(double *)pgensub->valh = change[7];
    *(double *)pgensub->vali = change[8];

    /* Calculate the average temperature change, excluding any zero changes. */

    count = 0;
    mean = 0.0;
    for (i=0; i<TEMPCHANGE_NSENSORS; i++)
    {
        if ( fabs(change[i]) > 0.0 )
        {
           mean += change[i];
           count++;
        }
    }
    if ( count > 0 )
        mean = mean / (double) count;

    /* Output the average temperature change to field VALJ */

    *(double *)pgensub->valj = mean;


    /* Ask the watchdog timer for a callback in TEMPCHANGE_INTERVAL seconds */

    waitTime = (int) (TEMPCHANGE_INTERVAL * sysClkRateGet() );

    if(  wdStart(pCallback->wd_id, waitTime, (FUNCPTR)callbackRequest, (int)pCallback) == ERROR )
    {
        logMsg ("gmosTemperatureChange: Failed to start watchdog for callback.\n", 0,0,0,0,0,0);
        status = BAD;
    }

    return (status);
}

/* ===================================================================== */


/*
 *+
 * FUNCTION NAME:
 * tempchangeCallback
 *
 * INVOCATION:
 * tempchangeCallback ( pcallback);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *
 * FUNCTION VALUE:
 * none.
 *
 * PURPOSE:
 * Handle timer expiry callback
 * This is the function called upon expiry of the watchdog timer.
 *
 * DESCRIPTION:
 * This is the function called upon expiry of the watchdog timer.
 * If the record is currently processing, then start the timer again
 * otherwise set the timeout flag and process the record.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 */
static void tempchangeCallback
(
    struct callback    *pcallback    /* Pointer to callback structure.		*/
)
{
    struct genSubRecord
                *pGensub;            /* Pointer to gensub record structure.	*/
    TEMPCHANGE_PRIVATE 
                *pPriv;              /* Piunter to device private structure.	*/
    int         waitTime;            /* Time to wait in seconds.		*/


#ifdef VERBOSE
    logMsg ("tempchangeCallback: pcallback=%p\n", (int) pcallback,0,0,0,0,0);
#endif

    /*
     * Extract the address of the gensub record structure and the device private
     * structure from the callback structure. Abort if either of these is NULL.
     */

    pGensub = (struct genSubRecord *) pcallback->pRecord;
    if ( pGensub == NULL )
    {
        logMsg ("tempchangeCallback: pGensub is NULL\n", 0,0,0,0,0,0);
        return;
    }

    pPriv = pGensub->dpvt;
    if ( pPriv == NULL )
    {
        logMsg ("tempchangeCallback: pPriv is NULL\n", 0,0,0,0,0,0);
        return;
    }


    /*
     *  If currently processing the record then call the timer again
     *  in a little while.
     */

    if(pGensub->pact) 
    {
        struct callback *pcallback = NULL;

        if ( ( pcallback = ( struct callback *)pPriv->timer ) == NULL )
        {
            logMsg ("tempchangeCallback: timer is NULL\n", 0,0,0,0,0,0);
            return;
        }

#ifdef VERBOSE
        logMsg ("tempchangeCallback: Record busy - retry\n", 0,0,0,0,0,0);
#endif

        waitTime = (int) (TEMPCHANGE_SHORT_INTERVAL * sysClkRateGet() );
        if( pcallback->wd_id == NULL ) 
        {
            if ( ( pcallback->wd_id = wdCreate() ) == NULL )
            {
                logMsg ("tempchangeCallback: No room for watchdog timer\n", 0,0,0,0,0,0);
                return;
            }
        }

        callbackSetPriority(priorityLow, &pcallback->callback);
        if(  wdStart(pcallback->wd_id, waitTime, (FUNCPTR)callbackRequest, 
                (int)pcallback) == ERROR )
        {
                logMsg ("tempchangeCallback: Failed to restart watchdog timer\n", 0,0,0,0,0,0);
                return;
        }
    }
    else
    {
        /*
         *  The timer expired, set the timeout flag and
         *  call the record to process.
         */

        semTake( pPriv->cmdLock, WAIT_FOREVER );
        pPriv->timeout = TRUE;
        semGive( pPriv->cmdLock );
        dbProcess((struct dbCommon *)pGensub);
    }
}

/* ===================================================================== */

/*+
 *   Function name:
 *   gmosMakeGratString
 *
 *   Purpose:
 *   Construct the grating assembly config string.
 *
 *   Description:
 *   This function reads in the three double precision values on inputs a, b, c
 *   and formats a string, containing these data, for the grating assembly to use.
 *
 *   Invocation:
 *   status = gmosMakeGratString( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    double    Tiltscale parameter
 *      > pgensub->b    double    Backlash parameter
 *      > pgensub->c    double    Forwardlash parameter
 *
 *   Parameters out:
 *      < pgensub->vala string    Grating assembly config string
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *      The gensub record to which this function is attached needs to have its PINI
 *      field set to YES to kick off the first execution.
 *
 *   Limitations:
 *
 *   Author:
 *   Chris Tierney  (cjt@roe.ac.uk)
 *
 *   History:
 *   10-Jan-2001: Original GMOS version.                                  (cjt)
 *   28-Feb-2001: Reworked so there is an explicit check on VALA never
 *                being written with more than MAX_STRING_SIZE-1
 *                characters. Floating point formats should round rather
 *                than truncate.                                          (smb)
 *   
 *-
 */

long gmosMakeGratString (struct genSubRecord *pgensub)
{
     long   status = GOOD;                        /* return status                      */
     double tiltScale;                            /* The input tiltscale value          */
     double backlash;                             /* The input backlash value           */
     double forwardlash;                          /* The input forwardlash value        */
     char   valString[MAX_STRING_SIZE];           /* Double value formatted as a string */
     char   tempString[SCRATCH_BUFF_SIZE];        /* Scratch space                      */

     /* Fetch the parameters from the gensub inputs. */

     tiltScale   = *(double *) pgensub->a;
     backlash    = *(double *) pgensub->b;
     forwardlash = *(double *) pgensub->c;

     /*
      * Make up a string looking something like this
      * 
      *    TSCALE 106.867 BLASH 1.25 FLASH 0.037
      *
      * in which the floating point values are written to
      * strings which are then truncated, to ensure the
      * whole thing can fit into MAX_STRING_SIZE-1 characters.
      */

     strcpy (tempString, "TSCALE ");
     sprintf (valString, "%7.3f", tiltScale);
     strncat (tempString, valString, 7);

     strcat (tempString, " BLASH ");
     sprintf (valString, "%4.2f", backlash);
     strncat (tempString, valString, 4);

     strcat (tempString, " FLASH ");
     sprintf (valString, "%5.3f", forwardlash);
     strncat (tempString, valString, 5);

     /* Finally, copy the string to VALA, ensuring it is truncated to MAX_STRING_SIZE-1 */

     strncpy( (char *)pgensub->vala, tempString, MAX_STRING_SIZE-1);

     return (status);
}





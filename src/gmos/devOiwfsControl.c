/*
************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) <1998>                       (c) <1998>
 * National Research Council        Conseil national de recherches
 * Ottawa, Canada, K1A 0R6          Ottawa, Canada, K1A 0R6
 * All rights reserved              Tous droits reserves
 *                    
 * NRC disclaims any warranties,    Le CNRC denie toute garantie
 * expressed, implied, or statu-    enoncee, implicite ou legale,
 * tory, of any kind with respect   de quelque nature que se soit,
 * to the software, including       concernant le logiciel, y com-
 * without limitation any war-      pris sans restriction toute
 * ranty of merchantability or      garantie de valeur marchande
 * fitness for a particular pur-    ou de pertinence pour un usage
 * pose.  NRC shall not be liable   particulier.  Le CNRC ne
 * in any event for any damages,    pourra en aucun cas etre tenu
 * whether direct or indirect,      responsable de tout dommage,
 * special or general, consequen-   direct ou indirect, particul-
 * tial or incidental, arising      ier ou general, accessoire ou
 * from the use of the software.    fortuit, resultant de l'utili-
 *                                  sation du logiciel.
 *
 ************************************************************************
 *
 * FILENAME
 * devOiwfsControl.c
 *
 * PURPOSE:
 * EPICS Assembly Control Record device support code for the 
 * GMOS on-instrument wavefront sensor (OIWFS).  Defines how
 * the OIWFS will respond to each possible command.
 *
 * FUNCTION NAME(S)
 * oiBusyStateChange    Respond to changes in device command states.
 * oiCancelCommand      Stop a motion or command in progress.
 * oiCheckAttributes    Check command arguments for validity.
 * oiConvertTarget      Convert from X-Y to base-pickoff coordinates
 * oiDevInit            Initialize device support code.
 * oiExecuteCommand     Start base and pickoff stage command execution.
 * oiIndexMode          Re-index the base and pickoff stages.
 * oiInitDeviceSupport  Startup initialization of device support code.
 * oiInitMode           Re-initialize the code and both devices.
 * oiMoveMode           Move to a new X-Y position and shut down.
 * oiTestMode           Test the assembly without moving anything.
 * oiTrackMode          Start position tracking (move without shutting down).
 * oiUpdateMode         do nothing since there is nothing to update.
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.1  2001/11/28 20:08:47  mbec
 * *** empty log message ***
 *
 * Revision 1.2  2001/04/23 18:24:43  smb
 * DEBUG macro changed from logMsg to printf so it can display floating point values (bug 196)
 *
 * Revision 1.12  2001/03/20 13:40:29  gmos
 * Modified DEBUG macro. All files now use printf() rather than logMsg(). All also print the output from taskName(0).
 *
 * Revision 1.11  2001/03/20 10:04:31  gmos
 * PARK position changed from top left to bottom right corner of patrol area, to keep probe safe from wrenches.
 *
 * Revision 1.10  2001/03/02 14:14:58  gmos
 * Added db_post_events after input fields changed in PARK mode.
 *
 * Revision 1.9  2001/02/28 17:24:12  gmos
 * Renamed gmMessageLevels.h to darMessageLevels.h.
 *
 * Revision 1.8  2001/02/23 15:43:25  gmos
 * Converted to same messaging scheme as the filter, grating and translation 
 * assemblies. Messages of type DAR_DBUG_MIN changed to DAR_MSG_FATAL, ERROR, 
 * WARNING or MIN as appropriate.
 *
 * Revision 1.7  2000/11/23 01:41:53  gmos
 * Change ballpark index position to -5 degrees.
 *
 * Revision 1.6  2000/11/10 00:14:32  gmos
 * Cleaned up use of MESS field and errorMessage variable.  Added
 * some comments.
 *
 * Revision 1.5  2000/10/16 23:54:34  gmos
 * Put CHECKSTAT wrapper on recGblPutLinkValue & recGblGetFastLink calls.
 * Added debugging in oiBusyStateChange().  Added debugging in
 * oiConvertTarget().  Added debugging and semaphore in oiProcessFault().
 * Corrected some comments.  Have baseActive & pickoffActive check for
 * OI_CMD_IDLE or not instead of 0 !0.  Use oiCancelCommand() to STOP
 * TRACK mode commands even if IDLE.
 *
 * Revision 1.4  2000/07/12 17:02:07  gmos
 * Changed recGblPutLinkValue calls for vel1 and vel2 to use DBR_DOUBLE rather 
 * than DBR_FLOAT, which was messing up the numbers in INDEX and would have 
 * screwed up the timeout calculations.
 *
 * Revision 1.3  2000/05/30 16:29:08  gmos
 * New version commented by William Rambold. Master enable bug fixed.
 *
 * Revision 1.2  2000/04/14 10:00:38  gmos
 * Merged pre and post FP software
 *
 * Revision 1.1  2000/04/10 09:20:00  gmos
 * gmtools and gmoscc merged
 *
 * Revision 1.18  2000/04/04 00:33:28  wooff
 * Changed strcpy's to strncpy's and added lots of comments in
 * the indexing section.
 *
 * Revision 1.17  2000/03/31 22:38:41  wooff
 * Cleaned up some error messages.
 *
 * Revision 1.16  2000/02/15 23:25:18  angelic
 * implemented park mode
 *
 * Revision 1.15  2000/02/10 21:16:28  angelic
 * added NULL function ptr to DSET for processFault
 *
 * Revision 1.14  2000/02/02 00:12:57  daoinsw
 * Changed it so that the timeout is 3 times was it was.
 *
 * Revision 1.13  2000/01/31 23:39:48  daoinsw
 * Removed reference to devOiwfsControl.h as it was empty.
 *
 * Revision 1.12  2000/01/31 20:21:00  dunn
 * Made velocity as an input, d, and timeout relative to velocity.
 *
 * Revision 1.11  1999/12/21 22:50:00  angelic
 * stop directive now stops both parts of a 2 part move
 *
 * Revision 1.10  1999/12/13 23:11:54  angelic
 * reject update mode, added some semaphore locks
 *
 * Revision 1.9  1999/11/17 18:44:19  angelic
 * added semaphore calls to protect the private structure
 *
 * Revision 1.8  1999/11/09 18:04:44  angelic
 * fix 2 part move to save target at start of first part.
 * clear second part of 2 part move when a new move directive is
 * received
 *
 * Revision 1.7  1999/11/04 22:32:36  angelic
 * moved revision 1.5.3.1.1.1 to revision 1.7
 * hopefully this will stop RCS of getting the branches mixed up
 *
 * Revision 1.5.3.1.1.1  1999/11/04 22:25:21  angelic
 * conversion to angles now performed here
 * fixed timeouts for motion parameter change
 * don't send park mode check to device layer (or change mode to park)
 *
 * Revision 1.5.3.1  1999/10/04 23:53:05  wooff
 * Does three part index if necessary.
 *
 * Revision 1.5.2.1  1999/09/22 23:27:32  wooff
 * Added code to oiBusyStateChange and oiMoveMode to do an intermediate move if
 * the limits would otherwise be hit.
 *
 * Revision 1.5.1.1  1999/09/08 17:25:04  rambold
 * Generated fake index target position in simm mode
 *
 * Revision 1.5  1999/07/30 18:44:01  wooff
 * Changes to index method.
 *
 * Revision 1.4  1999/07/08 18:25:19  wooff
 * Added access to current encoder positions from deviceControl.MPOS and
 * from absolute encoders.  Now does a three part index.
 *
 * Revision 1.3  1999/07/07 20:39:15  rambold
 * handover to B. Wooff
 *
 * Revision 1.2  1999/05/07 17:11:06  dunn
 * Changed to handle multiple bus changes.
 *
 * Revision 1.1  1999/04/28 21:04:49  dunn
 * Initial revision
 *
 *INDENT-ON*
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/

/*
 *  Includes
 */

#include        <vxWorks.h>
#include        <types.h>
#include        <stdioLib.h>
#include        <lstLib.h>
#include        <string.h>
#include        <stdlib.h>
#include        <wdLib.h>
#include        <math.h>

#include        <alarm.h>
#include        <callback.h>
#include        <dbDefs.h>
#include        <dbAccess.h>
#include        <dbFldTypes.h>
#include        <dbEvent.h>
#include        <devSup.h>
#include        <errMdef.h>
#include        <recSup.h>
#include        <special.h>
#include        <logLib.h>
#include        <tickLib.h>
#include        <sysLib.h>
#include        <taskLib.h>

#include        <assemblyControlRecord.h>
#include        <recAssControl.h>
#include	<darMessageLevels.h>     /* GMOS message level definitions. */
#include        <gmOiwfsCalc.h>


/*
 *  Assembly Control Record command input field access mnemonics
 */

#define OI_X_TARGET         *(double *)par->a   /* x target position        */
#define OI_Y_TARGET         *(double *)par->b   /* y target position        */
#define OI_VELOCITY         *(double *)par->d   /* motion velocity          */

#define OI_X_HI_LIMIT       par->aahl           /* maximum X position       */
#define OI_X_LOW_LIMIT      par->aall           /* minimum X position       */
#define OI_Y_HI_LIMIT       par->abhl           /* maximum Y position       */
#define OI_Y_LOW_LIMIT      par->abll           /* minimum Y position       */


/*
 *  PARK position targets - lower right corner
 */

#define OI_X_PARK_POS       par->aahl           /* max X                    */
#define OI_Y_PARK_POS       par->abll           /* min Y                    */


/*
 *  Assembly Control Record sensor input field access mnemonics
 */

#define OI_BAS_INCR         *(double *)par->sij /* base inc. encoder        */
#define OI_PKO_INCR         *(double *)par->sik /* pickoff incr. encoder    */
#define OI_BAS_ABS          *(double *)par->sil /* base absolute encoder    */
#define OI_PKO_ABS          *(double *)par->sim /* pickoff absolute encoder */
#define OI_Y_ABS            *(double *)par->sin /* calculated Y position    */


/*
 *  Internal constant definitions
 */
 
#define OI_PRESET_ANGLE      -5.0   /* base,pickoff pre-index target angles */
#define OI_Y_HI_LIM         134.0   /* Y high limit, to predict pre-moves   */
#define OI_Y_LO_LIM         -22.0   /* Y low limit, to predict pre-moves    */

#define OI_START_TIMEOUT    1       /* timeout for device record cmd start  */
#define OI_STOP_TIMEOUT     5       /* timeout for device record cmd stop   */
#define OI_MAX_RUN_TIMEOUT  240     /* maximum time for device cmd execution*/
#define OI_MIN_RUN_TIMEOUT  10      /* minimum time for device cmd exectuion*/

#define OI_CMD_IDLE         0       /* device is idle awaiting a command    */
#define OI_CMD_STARTING     1       /* start directive issued to device     */
#define OI_CMD_EXECUTING    2       /* device record has gone busy          */
#define OI_CMD_STOPPING     3       /* stop directive issued to device      */

#define D2R                 (M_PI/180.0)    /* degrees to radians           */
#define R2D                 (1.0/D2R)       /* radians to degrees           */


          
/*
 *  Device support function prototypes
 */

static long oiBusyStateChange( ASSEMBLY_CONTROL_RECORD * );
static long oiCancelCommand ( ASSEMBLY_CONTROL_RECORD * );
static long oiCheckAttributes( ASSEMBLY_CONTROL_RECORD * );
static long oiDevInit( unsigned );
static long oiIndexMode (ASSEMBLY_CONTROL_RECORD * );
static long oiInitDeviceSupport( ASSEMBLY_CONTROL_RECORD * );
static long oiInitMode (ASSEMBLY_CONTROL_RECORD * );
static long oiMoveMode (ASSEMBLY_CONTROL_RECORD * );
static long oiProcessFault (ASSEMBLY_CONTROL_RECORD * );
static long oiTestMode (ASSEMBLY_CONTROL_RECORD * );
static long oiTrackMode (ASSEMBLY_CONTROL_RECORD * );
static long oiUpdateMode (ASSEMBLY_CONTROL_RECORD * );


/*
 * Internal function prototypes
 */

static long oiConvertTarget (ASSEMBLY_CONTROL_RECORD *); 
static long oiExecuteCommand (ASSEMBLY_CONTROL_RECORD *); 


/*
 *  Device support function definition structure.   This is published
 *  as a public structure to allow the assembly control record to access
 *  the device support functions directly.
 */

struct {
    long            number;
    DEVSUPFUN       devReport;
    DEVSUPFUN       devInit;
    DEVSUPFUN       initDeviceSupport;
    DEVSUPFUN       devGetIoIntInfo;
    DEVSUPFUN       checkAttributes;
    DEVSUPFUN       stopDirective;
    DEVSUPFUN       initMode;
    DEVSUPFUN       moveMode;
    DEVSUPFUN       trackMode;
    DEVSUPFUN       indexMode;
    DEVSUPFUN       testMode;
    DEVSUPFUN       ackReceived;
    DEVSUPFUN       updateMode;
    DEVSUPFUN       processFault;
} devOiwfsAssembly = {
    14,
    NULL,
    oiDevInit,
    oiInitDeviceSupport,
    NULL,
    oiCheckAttributes,
    oiCancelCommand,
    oiInitMode,
    oiMoveMode,
    oiTrackMode,
    oiIndexMode,
    oiTestMode,
    oiBusyStateChange,
    oiUpdateMode,
    oiProcessFault
    };


/*
 *  Internal control structure definition.   This structure holds
 *  all of the oiwfs status information between executions of the assembly
 *  control record.
 */

typedef struct {
    SEM_ID      mutexSem;          /* mutual exclusion semaphore            */
    char        errorMessage[MAX_STRING_SIZE];  /* root error message       */
    long        baseActive;        /* current base command state            */    
    double      baseAngle;         /* current target for base stage         */
    double      baseVelocity;      /* current base stage velocity           */
    long        pickoffActive;     /* current pickoff command state         */
    double      pickoffAngle;      /* current target for pko stage          */
    double      pickoffVelocity;   /* current pko stage velocity            */
    short       preIndexMove;      /* pre-index motion flag                 */
    short       pre2IndexMove;     /* pre-pre-index motion flag             */
    short       inLimMove;         /* intermediate move motion flag         */
    short       recoverTarget;     /* use X and Y targets below for move    */
    double      Xtarget;           /* final X target (for intermediate move)*/
    double      Ytarget;           /* final Y target (for intermediate move)*/
} OI_DEV_CONTROL_PRIVATE;


/*
 *  Debugging macro to send the system time, error string and one
 *  value to the vxWorks logging system if the current debugging
 *  level exceeds the given debugging threshold.
 */

#define DEBUG(l,FMT,V)                                                  \
{                                                                       \
    int k=l;                                                            \
    if (k <= par->dbug)                                                 \
    printf ("%s: "FMT, taskName(0), tickGet(), par->name, V);           \
}


/*
 * Save the first error message received.   This will prevent subsequent
 * error messages from overwriting the root cause of the problem.
 */

#define SET_ERR_MSG(MSG)                                                \
{                                                                       \
    if (!strlen (pDevPvt->errorMessage))                                \
    strncpy (pDevPvt->errorMessage, MSG, MAX_STRING_SIZE - 1);          \
}


/*
 * Clear out the saved error message after it has been dealt with.  This
 * allows the next error message to be saved....
 */

#define CLEAR_ERR_MSG                                                   \
{                                                                       \
    pDevPvt->errorMessage[0] = '\0';                                    \
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiBusyStateChange
 *
 * INVOCATION:
 * status = oiBusyStateChange (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)     assemblyControl record structure.
 *
 *
 * FUNCTION VALUE:
 * (long) function return status.
 *
 * PURPOSE:
 * Respond to a device control record command start or completion
 *
 * DESCRIPTION:
 * This function is called directly by the assemblyControl Record whenever
 * one of two things happens...
 *
 *   - The timeout timer that was set for the action in progress expires
 *         meaning that the action took too long to complete.
 *   - Either the base or pickoff stage device control records starts or
 *         finishes a commanded action.
 *
 *
 *  The deviceControl Records operate on the action model as follows:
 *      - When an action is started by issuing the START directive
 *          the action state BUSY field changes from IDLE to BUSY.
 *      - When an action completes successfully the action state
 *          BUSY field changes from BUSY to IDLE.
 *      - When an action fails for some reason during execution
 *          the action state BUSY field changes from BUSY to ERROR
 *          and an error message appears on the MESS field.
 *      - When an action is stopped by issuing a STOP directive 
 *          the action state BUSY field will change from BUSY to IDLE
 *          as soon as the action has been cancelled.
 *
 *  The assemblyControl Record detects changes in the deviceRecord BUSY
 *  field for both the base and pickoff stages and calls this function
 *  when they occurr.
 *
 *
 *  Due to the complex geometry of the OIWFS probe motion one or two 
 *  intermediate motions may be required to move the probe from one 
 *  position to another if the probe is to avoid hitting the limits.
 *  This function will automatically set up and execute the next motion 
 *  in the sequence if the pre2Index, preIndex or inLimMove flags have
 *  been set to request a second or third motion.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long oiBusyStateChange
(
    ASSEMBLY_CONTROL_RECORD *par        /* assembly control record struct.  */
)
{
    OI_DEV_CONTROL_PRIVATE *pDevPvt;    /* Internal control structure       */
    int baseBusyChange;                 /* Base command state has changed   */
    int pickoffBusyChange;              /* Pickoff command state has changed*/
    int dummy;                          /* Other command state has changed  */
    unsigned short indexType;           /* Indexing mode for selected device*/    
    double baseTarget;                  /* Desired base stage angle         */
    double pickoffTarget;               /* Desired pickoff stage angle      */
    double basIndexVel;                 /* Desired base stage index vel.    */
    double pkoIndexVel;                 /* Desired pickoff stage index vel. */
    double basRelMove;                  /* Angle from base posn to center   */
    double pkoRelMove;                  /* Angle from pickoff posn to center*/
    long nRequest = 1;                  /* Number of items to send on link. */
    long status = DAR_S_SUCCESS;        /* Function status return value.    */
    long runTimeout;                    /* Maximum motion time              */

    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiBusyStateChange: entry%c\n", ' ');

    pDevPvt = (OI_DEV_CONTROL_PRIVATE *) assGetPrivateStruct (par);


    /*
     * Not expecting a response, it must be engineeering types mucking
     * around with the device control records.   Nothing we can do so 
     * just gnore it.
     */

    if ( pDevPvt->baseActive == OI_CMD_IDLE && 
         pDevPvt->pickoffActive == OI_CMD_IDLE )
    {
        DEBUG(DAR_MSG_MIN, 
              "<%ld> %s:oiBusyStateChange: unexpected busy state change%c\n", ' ');
        return status;
    }


    /*
     *  React immediately to timeouts generated by the assembly record.
     *  This will happen if:
     *      - a device control record does not start an action 
     *          after receiving a START directive.
     *      - a device action takes longer than expected.
     *      - a device control record does not stop an action
     *          after receiving a STOP directive. 
     *  In all cases abort the command and return an error message
     *  describing what happened.  
     *
     */

    if (assCommandTimedOut (par))
    {
        semTake (pDevPvt->mutexSem, WAIT_FOREVER);

        if (pDevPvt->baseActive == OI_CMD_STARTING)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Base stage did not start in time%c\n", ' ');
            SET_ERR_MSG ( "Base command did not execute" );
        }

        else if (pDevPvt->pickoffActive == OI_CMD_STARTING)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Pickoff stage did not start in time%c\n", ' ');
            SET_ERR_MSG ( "Pickoff command did not execute" );
        }

        else if (pDevPvt->baseActive == OI_CMD_STOPPING)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Base stage did not stop in time%c\n", ' ');
            SET_ERR_MSG ( "Base did not stop in time" );
        }

        else if (pDevPvt->pickoffActive == OI_CMD_STOPPING)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Pickoff stage did not stop in time%c\n", ' ');
            SET_ERR_MSG ( "Pickoff did not stop in time" );
        }

        else if (pDevPvt->baseActive == OI_CMD_EXECUTING)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Base stage did not finish in time%c\n", ' ');
            SET_ERR_MSG ( "Base motion timeout" );
        }

        else if (pDevPvt->pickoffActive == OI_CMD_EXECUTING)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Pickoff stage did not finish in time%c\n", ' ');
            SET_ERR_MSG ( "Pickoff motion timeout" );
        }

        else
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Unexpected command timeout%c\n", ' ');
            SET_ERR_MSG ( "Unexpected command timeout" );
        }

        semGive (pDevPvt->mutexSem);

        return (oiCancelCommand (par));
    }


    /*
     *  Must be a device control record action state change
     *  then.   Ask the assembly record which device states have changed
     *  since we last asked.
     */

    assGetBusResponse (par, &baseBusyChange, &pickoffBusyChange,
                       &dummy, &dummy, &dummy);


    /*
     * Respond to changes in base stage action state
     */

    if (baseBusyChange) switch (par->bus1)
    {
        /*
         * Device action has finished successfully, clear the device active 
         * flag to indicate this.
         */

        case DAR_DEV_BUSY_IDLE:
            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->baseActive = OI_CMD_IDLE;
            semGive (pDevPvt->mutexSem);
            break;


        /*
         * Device action has started successfully, set the device active flag
         * from OI_CMD_STARTING to OI_CMD_EXECUTING to indicate this.
         */

        case DAR_DEV_BUSY_BUSY:
            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->baseActive = OI_CMD_EXECUTING;
            semGive (pDevPvt->mutexSem);
            break;


        /*
         * Device action has failed for some reason, clear the device active 
         * flag and abort the OIWFS assembly command.
         */

        case DAR_DEV_BUSY_ERROR:
            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->baseActive = OI_CMD_IDLE;
            semGive (pDevPvt->mutexSem);
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Base stage error%c\n", ' ');
            oiCancelCommand (par);
            break;
    }


    /*
     * Respond to changes in pickoff stage action state
     */

    if (pickoffBusyChange) switch (par->bus2)
    {
        /*
         * Device action has finished successfully, clear the device active 
         * flag to indicate this.
         */

        case DAR_DEV_BUSY_IDLE:
            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->pickoffActive = OI_CMD_IDLE;
            semGive (pDevPvt->mutexSem);
            break;

        /*
         * Device action has started successfully, set the device active flag
         * from OI_CMD_STARTING to OI_CMD_EXECUTING to indicate this.
         */

        case DAR_DEV_BUSY_BUSY:
            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->pickoffActive = OI_CMD_EXECUTING;
            semGive (pDevPvt->mutexSem);
            break;

        /*
         * Device action has failed for some reason, clear the device active 
         * flag and abort the OIWFS assembly command.
         */

        case DAR_DEV_BUSY_ERROR:
            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->pickoffActive = OI_CMD_IDLE;
            semGive (pDevPvt->mutexSem);
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Pickoff stage error%c\n", ' ');
            oiCancelCommand (par);
            break;
    }


    /*
     * If both devices are busy then the motion has started successfully
     * so start the motion timeout timer.  The maximum motion time is
     * calculated based on the velocity of motion.  For very fast motions use
     * a minimum timeout value.
     */

    if (par->bus1 == DAR_DEV_BUSY_BUSY && par->bus2 == DAR_DEV_BUSY_BUSY)
    {
        assStopTimer (par, &status);
        assStartTimer(par, &status,
                     (((runTimeout = OI_MAX_RUN_TIMEOUT-36*OI_VELOCITY) < 0 ) ?
                                        OI_MIN_RUN_TIMEOUT : runTimeout));
    }




    /*
     *  If both devices are idle then a motion has completed.  It will  
     *  be necessary to check the multiple motion flags to see if more
     *  intermediate moves are necessary to complete the re-positioning
     *  of the probe.
     */

    if ( par->bus1 == DAR_DEV_BUSY_IDLE &&
         par->bus2 == DAR_DEV_BUSY_IDLE )

    {
        /*
         *  If the pre2IndexMove flag is set then there are two motions
         *  remaining to complete the index sequence.  Set up the next motion 
         *  in the sequence here.
         */

        if (pDevPvt->pre2IndexMove)
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange:calculating pre-index move%c\n",' ');

            /*
             *  Clear the three part move flag and set the two part move
             *  flag so that the next move in the sequence will start
             *  when this one finishes.
             */ 

            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->pre2IndexMove = FALSE;
            pDevPvt->preIndexMove = TRUE;
            semGive (pDevPvt->mutexSem);

            
            /*
             *  Insure that both device control records are running in index 
             *  mode 0 (move to the given position even though the index 
             *  position is not valid).
             */            

            indexType = 0;

            /*CHECKSTAT( (status = recGblPutLinkValue (&(par->sor), 
                    (void *) par, DBR_SHORT,& (indexType), &nRequest)),
                    return(oiCancelCommand (par)) );*/
           CHECKSTAT((status = dbPutLink(&(par->sor),DBR_SHORT, &(indexType), nRequest)),return(oiCancelCommand(par)) );

            /*CHECKSTAT( (status = recGblPutLinkValue (&(par->sot), 
                    (void *) par, DBR_SHORT, &(indexType), &nRequest)),
                    return(oiCancelCommand (par)) );*/
	    CHECKSTAT((status = dbPutLink(&(par->sot),DBR_SHORT, &(indexType), nRequest)),return(oiCancelCommand(par)) );

            /*
             *  Calculate the relative angular motion required to bring the 
             *  probe close to the indexing position of each stage.
             *  Scale the velocities of the two stages to simulate a 
             *  two-axis coordinated motion.
             */

            baseTarget = OI_BAS_INCR - OI_BAS_ABS + OI_PRESET_ANGLE;
            pickoffTarget = OI_PKO_INCR - OI_PKO_ABS + OI_PRESET_ANGLE;

            basRelMove = fabs(OI_PRESET_ANGLE - OI_BAS_ABS);
            pkoRelMove = fabs(OI_PRESET_ANGLE - OI_PKO_ABS);

            if (basRelMove > pkoRelMove)
            {
                basIndexVel = OI_VELOCITY;
                pkoIndexVel = OI_VELOCITY * pkoRelMove / basRelMove;
            }
            else
            {
                pkoIndexVel = OI_VELOCITY;
                basIndexVel = OI_VELOCITY * basRelMove / pkoRelMove;
            }


            /*
             *  Then write the calculated angles and velocities to the
             *  base and pickoff stages. 
             */

            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange: base target:%f\n", baseTarget);
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange: base velocity:%f\n", basIndexVel);
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange: pickoff target:%f\n", pickoffTarget);
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange: pickoff velocity:%f\n", pkoIndexVel);

            /*CHECKSTAT( (status = recGblPutLinkValue (&(par->pos1), 
                    (void *) par, DBR_DOUBLE, &(baseTarget), &nRequest)),
                    return(oiCancelCommand (par)) );

            CHECKSTAT( (status = recGblPutLinkValue (&(par->pos2), 
                    (void *) par, DBR_DOUBLE, &(pickoffTarget), &nRequest)),
                    return(oiCancelCommand (par)) );

            CHECKSTAT( (status = recGblPutLinkValue (&(par->vel1), 
                    (void *) par, DBR_DOUBLE, &(basIndexVel), &nRequest)),
                    return(oiCancelCommand (par)) );

            CHECKSTAT( (status = recGblPutLinkValue (&(par->vel2), 
                    (void *) par, DBR_DOUBLE, &(pkoIndexVel), &nRequest)),
                    return(oiCancelCommand (par)) );
	    */

            CHECKSTAT ( (status = dbPutLink (&(par->pos1),DBR_DOUBLE,&(baseTarget) ,nRequest)) ,return(oiCancelCommand(par)));
            CHECKSTAT ((status = dbPutLink(&(par->pos2),DBR_DOUBLE,&(pickoffTarget),nRequest)),return(oiCancelCommand(par)));
            CHECKSTAT ((status = dbPutLink(&(par->vel1),DBR_DOUBLE,&(basIndexVel),nRequest)),return(oiCancelCommand(par)));
            CHECKSTAT ((status = dbPutLink(&(par->vel2),DBR_DOUBLE,&(pkoIndexVel),nRequest)),return(oiCancelCommand(par)));


	return (oiExecuteCommand (par));
        }


        /*
         *  If the preIndexMove flag is set then there is one motion
         *  left in the indexing sequence.   Set it up and start it
         *  here.
         */

        if (pDevPvt->preIndexMove)
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange: index on home switch%c\n",' ');

            /*
             *  Clear the multiple part move flags so that the final motion
             *  will be started when this one finishes.
             */ 

            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->pre2IndexMove = FALSE;
            pDevPvt->preIndexMove = FALSE;
            semGive (pDevPvt->mutexSem);

            /*
             *  Return the device control record index types to 2 (index on
             *  the home switch going in the positive direction.  
             */

            indexType = 2;
            /*CHECKSTAT( (status = recGblPutLinkValue (&(par->sor), 
                    (void *) par, DBR_SHORT, &(indexType), &nRequest)),
                    return(oiCancelCommand (par)) );*/
 	    CHECKSTAT((status = dbPutLink(&(par->sor),DBR_SHORT,&(indexType),nRequest)),return(oiCancelCommand (par)));

            /*CHECKSTAT( (status = recGblPutLinkValue (&(par->sot), 
                    (void *) par, DBR_SHORT, &(indexType), &nRequest)),
                    return(oiCancelCommand (par)) );
	     */
	     CHECKSTAT((status = dbPutLink(&(par->sot),DBR_SHORT,&(indexType),nRequest)),return(oiCancelCommand (par)));
            /*
             *  Reset velocities to max since this is a very short motion
             *  and send them to the device control records.  No angluar
             *  positions targets are required here since the stages will 
             *  seek the home switches on their own.
             */

            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->baseVelocity = OI_VELOCITY;
            pDevPvt->pickoffVelocity = OI_VELOCITY;
            semGive (pDevPvt->mutexSem);

            /*CHECKSTAT( (status = recGblPutLinkValue (&(par->vel1), 
                    (void *) par, DBR_DOUBLE, 
                    &(pDevPvt->baseVelocity), &nRequest)),
                    return(oiCancelCommand (par)) );
	     */
            CHECKSTAT( (status = dbPutLink(&(par->vel1),DBR_DOUBLE,&(pDevPvt->baseVelocity),nRequest)) ,return(oiCancelCommand(par)) );
            
	    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->vel2), 
                    (void *) par, DBR_DOUBLE, 
                    &(pDevPvt->pickoffVelocity), &nRequest)),
                    return(oiCancelCommand (par)) );*/

	    CHECKSTAT((status = dbPutLink(&(par->vel2),DBR_DOUBLE,&(pDevPvt->pickoffVelocity),nRequest)),return(oiCancelCommand(par)));

            return (oiExecuteCommand (par));

        }


        /*
         *  If the inLimMove flag is set then the requested move would
         *  have hit the limits so an intermediate move was made to 
         *  avoid them.  This move has finished so set up the stages
         *  to go to the final desired position.
         */

        if (pDevPvt->inLimMove)
        {

            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange: in-limit move done, recover targets%c\n",' ');
            /*
             *  Set the recoverTarget flag so that oiConvertTarget will
             *  use the saved target position when calculating the
             *  base and pickoff angles required for the probe position.
             *  The saved target position was already checked to see if
             *  it could be reached, but oiConvertTarget must be called
             *  again anyway (it should never fail here).
             */

            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->recoverTarget = TRUE;
            semGive (pDevPvt->mutexSem);

            if (oiConvertTarget (par))
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:oiBusyStateChange: Angles calculated cannot be reached!%c\n",' ');
                SET_ERR_MSG ("Probe target not accessible (part 2)");
                return(oiCancelCommand (par));
            }

            /*
             *  Intermediate move is finished, reset flag
             */

            semTake (pDevPvt->mutexSem, WAIT_FOREVER);
            pDevPvt->inLimMove = FALSE;
            semGive (pDevPvt->mutexSem);


            /*
             *  Write the calculated angles and velocities to the
             *  base and pickoff stage device control records.
             */
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange: base target:%f\n", 
                  pDevPvt->baseAngle);

            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange: base velocity:%f\n", 
                  pDevPvt->baseVelocity);

            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange: pickoff target:%f\n", 
                  pDevPvt->pickoffAngle);

            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiBusyStateChange: pickoff velocity:%f\n", 
                  pDevPvt->pickoffVelocity);
/*
            CHECKSTAT( (status = recGblPutLinkValue (&(par->pos1), 
                    (void *) par, DBR_DOUBLE, 
                    &(pDevPvt->baseAngle), &nRequest)),
                    return(oiCancelCommand (par)) );

            CHECKSTAT( (status = recGblPutLinkValue (&(par->pos2), 
                    (void *) par, DBR_DOUBLE,
                    &(pDevPvt->pickoffAngle), &nRequest)),
                    return(oiCancelCommand (par)) );

            CHECKSTAT( (status = recGblPutLinkValue (&(par->vel1), 
                    (void *) par, DBR_DOUBLE,
                    &(pDevPvt->baseVelocity), &nRequest)),
                    return(oiCancelCommand (par)) );

            CHECKSTAT( (status = recGblPutLinkValue (&(par->vel2), 
                    (void *) par, DBR_DOUBLE,
                    &(pDevPvt->pickoffVelocity), &nRequest)),
                    return(oiCancelCommand (par)) );
*/

CHECKSTAT((status = dbPutLink(&(par->pos1),DBR_DOUBLE, &(pDevPvt->baseAngle), nRequest)),return(oiCancelCommand(par)) );
CHECKSTAT((status = dbPutLink(&(par->pos2),DBR_DOUBLE, &(pDevPvt->pickoffAngle), nRequest)),return(oiCancelCommand(par)) );
CHECKSTAT((status = dbPutLink(&(par->vel1),DBR_DOUBLE, &(pDevPvt->baseVelocity), nRequest)),return(oiCancelCommand(par)) );
CHECKSTAT((status = dbPutLink(&(par->vel2),DBR_DOUBLE, &(pDevPvt->pickoffVelocity), nRequest)),return(oiCancelCommand(par)) );

            return (oiExecuteCommand (par));

        }
    }



    /*
     *  If neither device is busy then the re-positioning action has
     *  either completed successfully or encountered an error along the
     *  way.  Either way signal the assembly record that the command
     *  has completed.
     */

    if (par->bus1 != DAR_DEV_BUSY_BUSY && 
        par->bus2 != DAR_DEV_BUSY_BUSY)
    {
        assStopTimer (par, &status);

        semTake (pDevPvt->mutexSem, WAIT_FOREVER);

        if (par->bus1 == DAR_DEV_BUSY_ERROR)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Base stage failed command%c\n", ' ');
            SET_ERR_MSG ( "Base error, see base status" );
        }

        if (par->bus2 == DAR_DEV_BUSY_ERROR)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiBusyStateChange: Pickoff stage failed command%c\n", ' ');
            SET_ERR_MSG ( "Pickoff error, see pickoff status" );
        }

        status = (strlen(pDevPvt->errorMessage)) ? DAR_E_DEVICE:DAR_S_SUCCESS;
        pDevPvt->preIndexMove = FALSE;
        pDevPvt->pre2IndexMove = FALSE;

        assCommandFinish (par, status, pDevPvt->errorMessage);
        CLEAR_ERR_MSG;
        semGive (pDevPvt->mutexSem);
    }

    return (status);
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiCancelCommand
 *
 * INVOCATION:
 * status = oiCancelCommand (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)     assemblyControl record structure.
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Cancel a command in progress
 *
 * DESCRIPTION:
 * Stop the command currently in progress, clean up any intermediate motion
 * flags and stop any device actions.
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
 * If STOP directive to Base stage fails (recGblPut* returns non-zero status) 
 * then STOP directive to Pickoff stage won't be attempted.
 *-
 ************************************************************************
 */

static long oiCancelCommand 
(
    ASSEMBLY_CONTROL_RECORD *par        /* assemblyControl record structure */
)
{
    OI_DEV_CONTROL_PRIVATE *pDevPvt;    /* Internal control structure       */
    unsigned short dir = DAR_DEV_DIR_STOP; /* deviceControl stop directive  */
    long nRequest = 1;                  /* Number of items to send on link  */
    long status = DAR_S_SUCCESS;        /* Function return status           */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiCancelCommand: entry%c\n", ' ');

    pDevPvt = (OI_DEV_CONTROL_PRIVATE *) assGetPrivateStruct (par);


    /*
     *  Clear intermediate move flags
     */

    semTake (pDevPvt->mutexSem, WAIT_FOREVER);
    pDevPvt->pre2IndexMove = FALSE;
    pDevPvt->preIndexMove = FALSE;
    pDevPvt->inLimMove = FALSE;
    semGive (pDevPvt->mutexSem);


    /*
     *  If neither device is currently active then we are finished and it is
     *  safe to tell the assemblyControl record that we have cancelled the 
     *  command (unless it's a TRACK mode command). 
     */

    if ( pDevPvt->baseActive == OI_CMD_IDLE && 
         pDevPvt->pickoffActive == OI_CMD_IDLE &&
         par->mode != DAR_MODE_TRACK )
    {
        assCommandFinish (par, status, pDevPvt->errorMessage);
        CLEAR_ERR_MSG;
        return status;
    }


    /*
     *  Otherwise one or both stages are active so issue the stop command to 
     *  the active device(s), set their activity flag(s) to stopping and set 
     *  the timeout timer to insure that the action(s) stop immediately.
     *  The assemblyControl record will be told the command has been cancelled
     *  when both devices reach to the IDLE state.
     */

    if ( pDevPvt->baseActive != OI_CMD_IDLE ||
         (par->mode == DAR_MODE_TRACK && par->bus1 != DAR_DEV_BUSY_ERROR) )
    {
        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiCancelCommand: stopping base stage %c\n", ' ');
        /*CHECKSTAT( (status = recGblPutLinkValue (&(par->odr1), 
                (void *) par, DBR_SHORT, &dir, &nRequest)),
                return(status) );*/
        CHECKSTAT((status = dbPutLink(&(par->odr1),DBR_SHORT, &dir, nRequest)),return(status) ); 
	semTake (pDevPvt->mutexSem, WAIT_FOREVER);
        pDevPvt->baseActive = OI_CMD_STOPPING;
        semGive (pDevPvt->mutexSem);
        assStopTimer (par, &status);
        assStartTimer (par, &status, OI_STOP_TIMEOUT);
     }

    if ( pDevPvt->pickoffActive != OI_CMD_IDLE ||
         (par->mode == DAR_MODE_TRACK  && par->bus2 != DAR_DEV_BUSY_ERROR))
    {
        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiCancelCommand: stopping pickoff stage %c\n", ' ');
        /*CHECKSTAT( (status = recGblPutLinkValue (&(par->odr2), 
                (void *) par, DBR_SHORT, &dir, &nRequest)),
                return(status) );*/
        CHECKSTAT((status = dbPutLink(&(par->odr2),DBR_SHORT, &dir, nRequest)),return(status) );
        semTake (pDevPvt->mutexSem, WAIT_FOREVER);
        pDevPvt->pickoffActive = OI_CMD_STOPPING;
        semGive (pDevPvt->mutexSem);
        assStopTimer (par, &status);
        assStartTimer (par, &status, OI_STOP_TIMEOUT);
    }

    return (status);
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiCheckAttributes
 *
 * INVOCATION:
 * status = oiCheckAttributes (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)     assemblyControl record structure.
 *
 * FUNCTION VALUE:
 * attribute accept or reject flag.
 *
 * PURPOSE:
 * Check to see if the requested action is valid at this time
 *
 * DESCRIPTION:
 * Description of the algorithm.
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
 ************************************************************************
 */

static long oiCheckAttributes 
(
    ASSEMBLY_CONTROL_RECORD *par        /* assemblyControl record structure */
)
{
    OI_DEV_CONTROL_PRIVATE *pDevPvt;    /* Internal control structure       */
    char buffer[128];                   /* character scratchpad buffer      */
    unsigned short dir;                 /* deviceControl record directive   */
    unsigned short mode;                /* deviceControl record mode        */
    unsigned short returnedAck;         /* deviceControl acknowledge flag   */
    long nRequest = 1;                  /* Number of values to send on link */
    long status;                        /* Link access status               */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiCheckAttributes: entry%c\n", ' ');

    pDevPvt = (OI_DEV_CONTROL_PRIVATE *) assGetPrivateStruct (par);


    /*
     *  Reject update requests since there is nothing to update!
     */
   
    if (par->mode == DAR_MODE_UPDATE)
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:oiCheckAttributes: UPDATE mode invalid for OIWFS%c\n",' ');
        assAddErrorMessage (par, 
                            "UPDATE mode invalid for OIWFS assembly");
        return DAR_ACK_VAL_REJECT;
    }


    /*
     *  If this is a request to "park" the probe then set the X-Y coordinates
     *  to the defined position (should be out of the light path in one of
     *  the corners).
     */

    if (par->mode == DAR_MODE_PARK)
    {
       OI_X_TARGET = OI_X_PARK_POS;
       OI_Y_TARGET = OI_Y_PARK_POS;
       db_post_events(par, &OI_X_TARGET, DBE_VALUE);
       db_post_events(par, &OI_Y_TARGET, DBE_VALUE);
    }


    /*
     *  If this is a motion request then check to insure that the requested
     *  probe position can be reached.
     */

    if (par->mode == DAR_MODE_MOVE || 
        par->mode == DAR_MODE_TRACK ||
        par->mode == DAR_MODE_PARK)
    {
        /*
         * Is the target within motion limits?
         */

        if (OI_X_TARGET < OI_X_LOW_LIMIT || OI_X_TARGET > OI_X_HI_LIMIT)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiCheckAttributes: X target out of range%c\n", ' ');
            sprintf (buffer, 
                     "X target range %f->%f", OI_X_LOW_LIMIT, OI_X_HI_LIMIT);
            assAddErrorMessage (par, buffer);
            return DAR_ACK_VAL_REJECT;
        }

        if (OI_Y_TARGET < par->abll || OI_Y_TARGET > par->abhl)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiCheckAttributes: Y target out of range%c\n", ' ');
            sprintf (buffer,
                     "Y target range %f->%f", OI_Y_LOW_LIMIT, OI_Y_HI_LIMIT);
            assAddErrorMessage (par, buffer);
            return DAR_ACK_VAL_REJECT;
        }


        /*
         * Calculate the base and pickoff stage angles required to reach
         * the target position.  Reject the command if the conversion fails.
         */

        status = oiConvertTarget (par);

        if (status)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiCheckAttributes: Angles calculated cannot be reached!%c\n",' ');
            assAddErrorMessage (par, "Probe target not accessible");
            return DAR_ACK_VAL_REJECT;
        }

        
        /*
         *  Pass the converted base and pickoff stage angles and velocities
         *  to the deviceControl records.   The CHECK directive will be issued
         *  later to insure that they can reach these positions.
         */

        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiCheckAttributes: base target:%f\n", 
              pDevPvt->baseAngle);

        /*CHECKSTAT( (status = recGblPutLinkValue (&(par->pos1), 
                (void *) par, DBR_DOUBLE,
                &(pDevPvt->baseAngle), &nRequest)),
                return DAR_ACK_VAL_REJECT);
	*/
	CHECKSTAT((status = dbPutLink(&(par->pos1),DBR_DOUBLE,&(pDevPvt->baseAngle), nRequest)),return DAR_ACK_VAL_REJECT );
        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiCheckAttributes: pickoff target:%f\n", 
              pDevPvt->pickoffAngle);

        /*CHECKSTAT( (status = recGblPutLinkValue (&(par->pos2), 
                (void *) par, DBR_DOUBLE,
                &(pDevPvt->pickoffAngle), &nRequest)),
                return DAR_ACK_VAL_REJECT);*/
        CHECKSTAT((status = dbPutLink(&(par->pos2),DBR_DOUBLE,&(pDevPvt->pickoffAngle), nRequest)),return DAR_ACK_VAL_REJECT );

        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiCheckAttributes: base velocity:%f\n", 
              pDevPvt->baseVelocity);

        /*CHECKSTAT( (status = recGblPutLinkValue (&(par->vel1), 
                (void *) par, DBR_DOUBLE,
                &(pDevPvt->baseVelocity), &nRequest)),
                return DAR_ACK_VAL_REJECT);*/
        CHECKSTAT((status = dbPutLink(&(par->vel1),DBR_DOUBLE,&(pDevPvt->baseVelocity), nRequest)),return DAR_ACK_VAL_REJECT );

        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiCheckAttributes: pickoff velocity:%f\n", 
              pDevPvt->pickoffVelocity);

        /*CHECKSTAT( (status = recGblPutLinkValue (&(par->vel2), 
                (void *) par, DBR_DOUBLE,
                &(pDevPvt->pickoffVelocity), &nRequest)),
                return DAR_ACK_VAL_REJECT);
	*/
        CHECKSTAT((status = dbPutLink(&(par->vel2),DBR_DOUBLE,&(pDevPvt->pickoffVelocity), nRequest)),return DAR_ACK_VAL_REJECT );
    }


    /*
     * Pass the requested  mode to the deviceControl records.
     * Use move mode for a park request as the devices don't
     * support park mode.
     */

    if (par->mode == DAR_MODE_PARK)
    {
        mode = DAR_MODE_MOVE;
    }
    else
    {
        mode = par->mode;
    }

    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->mod1), 
            (void *) par, DBR_SHORT,
            &mode, &nRequest)),
            return DAR_ACK_VAL_REJECT);*/
    CHECKSTAT((status = dbPutLink(&(par->mod1),DBR_SHORT,&mode, nRequest)),return DAR_ACK_VAL_REJECT );

    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->mod2), 
            (void *) par, DBR_SHORT,
            &mode, &nRequest)),
            return DAR_ACK_VAL_REJECT);*/
      CHECKSTAT((status = dbPutLink(&(par->mod2),DBR_SHORT,&mode, nRequest)),return DAR_ACK_VAL_REJECT );


    /*
     *  If we are in position tracking mode then the devices must react to
     *  new target positions as quickly as possible
     *  so we skip the final check by the deviceControl records.  In all
     *  other cases issue the CHECK directive to both the base and pickoff
     *  stages and look at the acknowledge (ACK) field of each to see 
     *  if the requested action is valid at this time.
     */

    if (par->mode != DAR_MODE_TRACK)
    {

        dir = DAR_DEV_DIR_CHECK;

	CHECKSTAT((status = dbPutLink(&(par->odr1),DBR_SHORT,&dir,nRequest)),return DAR_ACK_VAL_REJECT);
        CHECKSTAT((status = dbGetLink(&(par->ack1),DBR_USHORT,&returnedAck,0,0)),return DAR_ACK_VAL_REJECT);

	if (returnedAck != DAR_DEV_VAL_ACCEPT )
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiCheckAttributes: Base command rejected%c\n",' ');
            assAddErrorMessage (par, "Command rejected, see base status");
            return DAR_ACK_VAL_REJECT;
        }

        CHECKSTAT((status = dbPutLink(&(par->odr2),DBR_SHORT,&dir,nRequest)),return DAR_ACK_VAL_REJECT);
        CHECKSTAT((status = dbGetLink(&(par->ack2),DBR_USHORT,&returnedAck,0,0)),return DAR_ACK_VAL_REJECT);

        if (returnedAck != DAR_DEV_VAL_ACCEPT )
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:oiCheckAttributes: Pickoff command rejected%c\n",' ');
            assAddErrorMessage (par, "Command rejected, see pickoff status");
            return DAR_ACK_VAL_REJECT;
        }
    }

    return (DAR_ACK_VAL_ACCEPT);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiConvertTarget
 *
 * INVOCATION:
 * status = oiConvertTarget (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)   assemblyControl record structure.
 *
 * FUNCTION VALUE:
 * (long) conversion status flag:
 *      0 - successful conversion
 *     -1 - target outside patrol area
 *
 * PURPOSE:
 * To convert rectangular to polar coordinates for the OIWFS
 *
 * DESCRIPTION:
 * This function accepts probe positions as X-Y values, expressed in 
 * millimeters from the home position, and converts them to the base
 * and pickoff stage angles required to place the probe at that position
 * using the gmCalculateProbeAngles function in the gmOiwfsCalc.c module.
 * This function should always return successfully if the patrol area
 * is defined properly.
 *
 * The X-Y target position is taken directly from the assembly record
 * A and B field unless the recoverTarget flag is set (indicating that this
 * is the second half of a two part move) in which case it is taken 
 * from the place where it was saved int the internal control structure.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * gmOiwfsCalculateProbeAngles in the gmOiwfsCalc.c module.
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long oiConvertTarget 
(
    ASSEMBLY_CONTROL_RECORD *par        /* assemblyControl record structure */
)
{
    OI_DEV_CONTROL_PRIVATE *pDevPvt;    /* Internal control structure       */
    double basRelMove;                  /* Angular displacement of base     */
    double pkoRelMove;                  /* Angluar displacement of pickoff  */
    double bAngle;                      /* Calculated angle of base stage   */
    double pAngle;                      /* Calculated angle of pickoff stage*/
    double probeAngle;                  /* Angle of probe head wrt detector */
    long status = DAR_S_SUCCESS;        /* Conversion status flag           */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiConvertTarget: entry%c\n", ' ');

    pDevPvt = (OI_DEV_CONTROL_PRIVATE *) assGetPrivateStruct (par);


    /* 
     *  If this is the second part of a 2 part move then calculate angles
     *  based on the original target position that was saved before the first 
     *  part was executed and then clear the two part motion flag.
     */

    if ( pDevPvt->recoverTarget == TRUE)
    {
        status = gmOiwfsCalculateProbeAngles (pDevPvt->Xtarget,
                                              pDevPvt->Ytarget,
                                              1,
                                              &bAngle, &pAngle, &probeAngle);
        semTake (pDevPvt->mutexSem, WAIT_FOREVER);
        pDevPvt->recoverTarget = FALSE;
        semGive (pDevPvt->mutexSem);
    }


    /*
     *  Otherwise use the target position written to the assembly record
     *  A and B attribute fields
     */

    else
    {
        status = gmOiwfsCalculateProbeAngles (OI_X_TARGET,
                                              OI_Y_TARGET,
                                              1,
                                              &bAngle, &pAngle, &probeAngle);
    }


    /*
     *  If it was not possible to convert this target position to angles
     *  then return here...
     */

    if (status != DAR_S_SUCCESS) 
    {
        DEBUG(DAR_MSG_ERROR, "<%ld> %s:oiConvertTarget: gmOiwfsCalculateProbeAngles failed%c\n", ' ');
        return status;
    }

    /*
     *  Otherwise it should be possible to reach this position so convert
     *  the angles from radians to degrees (the native units of the two
     *  device control records) and simulate a 2-axis coordinated motion by
     *  scaling the velocities so that both stages reach their target 
     *  angles at roughly the same time.
     */

    semTake (pDevPvt->mutexSem, WAIT_FOREVER);
    pDevPvt->baseAngle = bAngle * R2D;  
    pDevPvt->pickoffAngle = pAngle * R2D;

    basRelMove = fabs(pDevPvt->baseAngle - OI_BAS_INCR);
    pkoRelMove = fabs(pDevPvt->pickoffAngle - OI_PKO_INCR);

    if (basRelMove > pkoRelMove)
    {
        pDevPvt->baseVelocity = OI_VELOCITY;
        pDevPvt->pickoffVelocity = OI_VELOCITY * pkoRelMove / basRelMove;
    }

    else
    {
        pDevPvt->pickoffVelocity = OI_VELOCITY;
        pDevPvt->baseVelocity = OI_VELOCITY * basRelMove / pkoRelMove;
    }

    semGive (pDevPvt->mutexSem);

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiDevInit
 *
 * INVOCATION:
 * status = oiDevInit (pass);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pass  (int) Pass number (function called twice during initialization)
 *
 * FUNCTION VALUE:
 * (long) initialization pass success code.
 *
 * PURPOSE:
 * Initialize global doaDevice device support functions
 *
 * DESCRIPTION:
 * Called before and after an EPICS database has been loaded.
 * First (before) and second (after) invocation simply return
 * since there is nothing global to be set up for the device
 * support code.
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
 ************************************************************************
 */


static long oiDevInit (unsigned    after)
{

    /*
     *  Nothing to do, so we were very successful in doing it!
     */
   
    return DAR_S_SUCCESS;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiExecuteCommand
 *
 * INVOCATION:
 * status = oiExecuteCommand (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)    assemblyControl record structure.
 *
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Execute a pre-configured command on base and pickoff stages
 *
 * DESCRIPTION:
 * Start the execution of a probe action using the following algorithm:
 *
 *      Send the GO directive to both the base and pickoff stages.
 *      Check the ACK field of each record, if either of the actions 
 *        was rejected then cancel the command.
 *      Otherwise set the timeout timer to the maximum length of time
 *        the actions should take.   This allows the assemblyControl record
 *        to abort an action that has hung up for some reason.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * Target position, operating mode and motion velocity must have been
 * loaded into the two deviceControl records before calling this
 * function.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long oiExecuteCommand 
(
    ASSEMBLY_CONTROL_RECORD *par        /* assemblyControl record structure */
)
{
    OI_DEV_CONTROL_PRIVATE *pDevPvt;    /* Internal control structure       */
    unsigned short dir;                 /* Directive value written to record*/
    long nRequest = 1;                  /* Number of items to send on link  */
    unsigned short returnedAck;         /* Command accept/reject flag       */
    long status = DAR_S_SUCCESS;        /* Function status                  */
    long runTimeout;                    /* Maximum execution time for cmd   */

    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiExecuteCommand: entry%c\n", ' ');

    pDevPvt = (OI_DEV_CONTROL_PRIVATE *) assGetPrivateStruct (par);


    /*
     *  We can assume that the target position, operating mode and motion 
     *  velocity have already been written to the deviceControl records by
     *  one of the motion setup functions.  All that is required is to 
     *  write the GO directive to each record to start executing the preset
     *  action.
     */

    semTake (pDevPvt->mutexSem, WAIT_FOREVER);

    dir = DAR_DEV_DIR_GO;
    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->odr1), 
            (void *) par, DBR_SHORT, &dir, &nRequest)),
            return (oiCancelCommand (par)) );*/
    CHECKSTAT((status = dbPutLink(&(par->odr1),DBR_SHORT, &dir, nRequest)),return(oiCancelCommand(par)) );

    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->odr2), 
            (void *) par, DBR_SHORT, &dir, &nRequest)),
            return (oiCancelCommand (par)) );*/
     CHECKSTAT((status = dbPutLink(&(par->odr2),DBR_SHORT, &dir, nRequest)),return(oiCancelCommand(par)) );


    /*
     *  Check that the deviceControl records accepted the GO command.  
     *  If the command was accepted set the appropriate stageActive flag
     *  to indicate that a GO directive has been sent an we expect to see
     *  and IDLE to BUSY transition on the device BUSY field.
     *
     *  If either of the two stages rejected the command call the 
     *  oiCancelCommand function to cancel the commands sent to the stages 
     *  then return.
     */

    /*CHECKSTAT( (status = recGblGetFastLink (&(par->ack1), 
            (void *) par, &returnedAck)),
            return (oiCancelCommand (par)) );*/
     CHECKSTAT((status = dbGetLink(&(par->ack1),DBR_USHORT,&returnedAck,0,0)),return (oiCancelCommand (par)));

    if (returnedAck != DAR_DEV_VAL_ACCEPT )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:oiExecuteCommand: Base Stage rejected command%c\n",' ');
        SET_ERR_MSG ( "base stage rejected command" );
        semGive (pDevPvt->mutexSem);
        return (oiCancelCommand (par));       
    }

    pDevPvt->baseActive = OI_CMD_STARTING;

    /*CHECKSTAT( (status = recGblGetFastLink (&(par->ack2), 
            (void *) par, &returnedAck)),
            return (oiCancelCommand (par)) );*/
    CHECKSTAT((status = dbGetLink(&(par->ack2),DBR_USHORT,&returnedAck,0,0)),return (oiCancelCommand (par)));
    if (returnedAck != DAR_DEV_VAL_ACCEPT )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:oiExecuteCommand: Pickoff Stage rejected command%c\n",' ');
        SET_ERR_MSG ( "pickoff stage rejected command" );
        semGive (pDevPvt->mutexSem);
        return (oiCancelCommand (par));
    }

    pDevPvt->pickoffActive = OI_CMD_STARTING;
    semGive (pDevPvt->mutexSem);


    /*
     *  Start the assemblyRecord timeout timer to the maximum length of
     *  time this action is expected to take.
     */
     
    /*
     *  If both of two stages are busy at this point then this is a 
     *  position update received during motion so re-calculate the motion 
     *  timeout based on the stage velocity.
     */

    if (par->bus1 == DAR_DEV_BUSY_BUSY && par->bus2 == DAR_DEV_BUSY_BUSY)
    {
        DEBUG(DAR_MSG_MAX, 
              "<%ld> %s:oiExecuteCommand: Re-calculate motion timeout%c\n",' ');
        assStopTimer (par, &status);
        assStartTimer (par, &status, 
                       (((runTimeout=OI_MAX_RUN_TIMEOUT-36*OI_VELOCITY)<0 ) ?
                        OI_MIN_RUN_TIMEOUT : runTimeout ) ) ;
        DEBUG(DAR_MSG_MAX, 
              "<%ld> %s:oiExecuteCommand: Motion timeout re-calculated:%ld\n",runTimeout);
    }


    /*
     *  Otherwise set the timeout for the maximum time it should take the
     *  stages to set their BUSY fields from IDLE to BUSY in response to the
     *  GO commands.
     */

    else
    {
        assStopTimer (par, &status);
        assStartTimer (par, &status, OI_START_TIMEOUT);
    }

    return status;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiIndexMode
 *
 * INVOCATION:
 * status = oiIndexMode (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) assemblyControl record structure.
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Find probe zero reference position
 *
 * DESCRIPTION:
 * Re-index the base and pickoff stages using the following algorithm:
 *
 *      Calculate base and pickoff stage positions to bring them just
 *        below their index positions.  Use the absolute encoders to
 *        calculate offsets to the current positions since
 *        the incremental ones can not be trusted here.
 *
 *      Check to see if the path the probe will take during this move
 *        will go outside the travel limits.
 *
 *      If there is a danger of this happening look at the motions
 *      in more detail as follows:
 *
 *          If in the lower half of the Y range of motion:
 *
 *              If base will move in positive direction move the pickoff 
 *                to mid-range instead.
 *
 *              Else if pickoff will move in the positive direction move the
 *                base to mid-range instead.
 *
 *              Else ok to continue with original targets.
 *
 *          Else must be in upper half of Y range of motion:
 *
 *              If base will move in negative direction move the pickoff 
 *                to mid-range instead.
 *
 *              Else if pickoff will move in the negative direction move the
 *                base to mid-range instead.
 *
 *              Else ok to continue with original targets. 
 *
 *      Simulate coordinated motion by scaling velocities.
 *
 *      Load targets and velocities into device control records.
 *
 *      Start the motion by calling executeCommand.
 *
 *                
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 *-
 ************************************************************************
 */

static long oiIndexMode 
(
    ASSEMBLY_CONTROL_RECORD *par        /* assemblyControl record structure */
)
{
    OI_DEV_CONTROL_PRIVATE *pDevPvt;    /* Internal control structure       */
    unsigned short indexType;           /* base or pickoff indexing mode    */
    double baseTarget;                  /* Intermediate base target position*/
    double pickoffTarget;               /* Intermediate pickoff target pos  */
    double basVel;                      /* Base stage motion velocity       */
    double pkoVel;                      /* Pickoff stage motion velocity    */
    double basRel;                      /* Base stage motion required       */
    double pkoRel;                      /* Pickoff stage motion required    */
    double yMin;                        /* Calculated Y minimum travel      */
    double yMax;                        /* Calculated Y maximum travel      */
    double basMidR;                     /* Calculate base mid range         */
    double pkoMidR;                     /* Calculated pickoff mid range     */
    long   nRequest = 1;                /* Number of items to write on link */
    long   status = DAR_S_SUCCESS;      /* Function status                  */

    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiIndexMode: entry%c\n", ' ');

    pDevPvt = (OI_DEV_CONTROL_PRIVATE *) assGetPrivateStruct (par);


    /*
     *  At least two motions will be required to index the stages, one to
     *  get close to the index position and one to find the home switches.
     *  Set the preIndex flag to indicate that the first move is being
     *  started by this function here.   If more motions are required the 
     *  flags will be adjusted below...
     */


    semTake (pDevPvt->mutexSem, WAIT_FOREVER);
    pDevPvt->pre2IndexMove = FALSE;
    pDevPvt->preIndexMove = TRUE;
    semGive (pDevPvt->mutexSem);


    /*
     *  Change base and pickoff stage indexing modes to zero (go to the
     *  given position even though the index position is not valid).
     */

    indexType = 0;
    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->sor), 
            (void *) par, DBR_SHORT,
            &(indexType), &nRequest)),
            return(oiCancelCommand (par)) );*/
    CHECKSTAT((status = dbPutLink(&(par->sor),DBR_SHORT, &(indexType), nRequest)),return(oiCancelCommand(par)) );

    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->sot), 
            (void *) par, DBR_SHORT,
            &(indexType), &nRequest)),
            return(oiCancelCommand (par)) );*/
     CHECKSTAT((status = dbPutLink(&(par->sot),DBR_SHORT, &(indexType), nRequest)),return(oiCancelCommand(par)) );

    /*
     *  In simulation mode we do not have to worry about hitting the limits
     *  or missing the home switches so simply send them to the zero position
     *  at full speed.
     */

    if (par->simm)
    {
        DEBUG(DAR_MSG_FULL,
              "<%ld> %s:oiIndexMode: simulating pre-index move%c\n", ' ');

        baseTarget    = 0;
        pickoffTarget = 0;
        pkoVel = OI_VELOCITY;
        basVel = OI_VELOCITY;
    }


    /*
     *  Otherwise this is the real world where things can go wrong...
     */

    else
    {
        /*
         *  Since we have been asked to find the index position we can
         *  not trust the incremental encoders to tell us where we are.
         *  Use the absolute encoders (which are not very accurate) to
         *  determine how far we are from the index position then calculate
         *  the base and pickoff stage offsets required to bring us to
         *  the preset angle positions.   These positions are set to
         *  insure that we wind up on the correct side of the actual
         *  home switches which are in the center of travel.
         */

        baseTarget    = OI_BAS_INCR - OI_BAS_ABS + OI_PRESET_ANGLE;
        pickoffTarget = OI_PKO_INCR - OI_PKO_ABS + OI_PRESET_ANGLE;


        /*
         *  Determine if the motion calculated above is likely to travel into
         *  the soft limits (we may be up in one corner) by calling the
         *  gmOiwfsCalculateVerticalRange function in the gmOiwfsCalc module.
         *  If this function returns a zero value then it thinks the 
         *  current probe trajectory to the preset angles will not hit the
         *  limits en-route so we can use the calculated targets directly.
         */

        if (gmOiwfsCalculateVerticalRange( OI_BAS_ABS * D2R,
                                           OI_PKO_ABS * D2R,
                                           OI_PRESET_ANGLE * D2R,
                                           OI_PRESET_ANGLE * D2R,
                                           (OI_Y_LO_LIM + 10.0),
                                           (OI_Y_HI_LIM - 10.0),
                                           &yMin, &yMax,
                                           &basMidR, &pkoMidR ) == 0)
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:oiIndexMode: using single pre-index move%c\n",' ');
        }


        /*
         *  Bad news, we are in an awkward spot and have to be much more
         *  careful about how we get to the pre-index position.
         */

        else
        {
          /*
             *  The motion will probably hit a limit so a second pre-index
             *  motion may be required.  We can't use the mid-point targets 
             *  returned above if the probe is near the limits.  Since we 
             *  don't know for sure it is safest to ignore them and do 
             *  some checking for ourselves as follows:
             *
             *  Check the current Y position reported by the absolute
             *  encoders. The patrol area Y coordinate determines where the 
             *  base and pickoff soft limits are located. Handle the lower
             *  range of travel first.
             */

            if (OI_Y_ABS < ((OI_Y_LO_LIM + OI_Y_HI_LIM)/2.0))
            {
                /*
                 *  Check to see if the base target would drive the probe tip 
                 *  towards the limit.
                 */

                if ((OI_PRESET_ANGLE - OI_BAS_ABS) > 0)
                {
                    /*
                     *  Can't move base in a positive direction because that 
                     *  would take it further into the limit.  Move 
                     *  the pickoff nearer to the mid-Y range of the patrol
                     *  area. Move the pickoff stage 15 degrees, plus 10% of 
                     *  absolute angle, in the negative direction.  Move the 
                     *  base stage 1 degree in the negative directin.  This
                     *  is black magic but it works so don't question it!
                     */

                    semTake (pDevPvt->mutexSem, WAIT_FOREVER);
                    pDevPvt->preIndexMove = FALSE;
                    pDevPvt->pre2IndexMove = TRUE;
                    semGive (pDevPvt->mutexSem);
                    pickoffTarget = (OI_PKO_INCR - 15.0 - (OI_PKO_ABS / 10.0));
                    baseTarget = (OI_BAS_INCR - 1.0);
                    DEBUG(DAR_MSG_FULL, 
                          "<%ld> %s:oiIndexMode:pko avoids Y low lim%c\n",' ');

                } 


                /*
                 *  Base motion is ok, check to see if the pickoff target 
                 *  would drive the probe tip towards the limit.
                 */

                else if ((OI_PRESET_ANGLE - OI_PKO_ABS) > 0)
                {
                    /*
                     *  Can't move pickoff in positive direction for the same
                     *  reason as above where we couldn't move the base so
                     *  perform some different magic.
                     */

                    semTake (pDevPvt->mutexSem, WAIT_FOREVER);
                    pDevPvt->preIndexMove = FALSE;
                    pDevPvt->pre2IndexMove = TRUE;
                    semGive (pDevPvt->mutexSem);
                    baseTarget = (OI_BAS_INCR - 15.0 - (OI_BAS_ABS / 10.0));
                    pickoffTarget = (OI_PKO_INCR - 1.0);
                    DEBUG(DAR_MSG_FULL, 
                          "<%ld> %s:oiIndexMode:base avoids Y low lim%c\n",' ');
                }

                /*
                 *  Both motions are in the correct direction so it is safe to
                 *  use the pre-index targets after all.
                 */

                else
                {
                    DEBUG(DAR_MSG_FULL, 
                          "<%ld> %s:oiIndexMode:won't hit Y lo lim%c\n",' ');
                }
            }


            /*  If it is not in the lower half then it mus be in the
             *  upper half of travel so check this area.
             */

            else
            {
                /*
                 *  Check to see if the base target would drive the probe tip 
                 *  towards the limit.
                 */ 

                if ((OI_PRESET_ANGLE - OI_BAS_ABS) < 0)
                {
                    /*
                     *  Can't move base in negative direction because that 
                     *  would take it towards the limit, so instead move the
                     *  pickoff 30 degrees positive into the mid-Y range of 
                     *  the patrol area.  Move the base stage one degree in
                     *  the positive direction also.  More black magic.
                     */

                    semTake (pDevPvt->mutexSem, WAIT_FOREVER);
                    pDevPvt->preIndexMove = FALSE;
                    pDevPvt->pre2IndexMove = TRUE;
                    semGive (pDevPvt->mutexSem);
                    pickoffTarget = (OI_PKO_INCR + 30.0);
                    baseTarget = (OI_BAS_INCR + 1.0);
                    DEBUG(DAR_MSG_FULL, 
                          "<%ld> %s:oiIndexMode:pko avoids Y hi lim%c\n",' ');
                }


                /*
                 *  Base motion is ok, check to see if the pickoff target 
                 *  would drive the probe tip towards the limit.
                 */

                else if ((OI_PRESET_ANGLE - OI_PKO_ABS) < 0)
                {
                    /*
                     *  Can't move pickoff in negative direction for the same
                     *  reason as above where we couldn't move the base.
                     *  Black magic time again.
                     */

                    semTake (pDevPvt->mutexSem, WAIT_FOREVER);
                    pDevPvt->preIndexMove = FALSE;
                    pDevPvt->pre2IndexMove = TRUE;
                    semGive (pDevPvt->mutexSem);
                    baseTarget = (OI_BAS_INCR + 30.0);
                    pickoffTarget = (OI_PKO_INCR + 1.0);
                    DEBUG(DAR_MSG_FULL, 
                          "<%ld> %s:oiIndexMode:base avoids Y hi lim%c\n",' ');
                }

                /*
                 *  Both motions are in the correct direction so it is safe to
                 *  use the pre-index targets after all.
                 */

                else
                {
                    DEBUG(DAR_MSG_FULL, 
                          "<%ld> %s:oiIndexMode:won't hit Y hi lim%c\n",' ');
                }
            }
        }


        /*
         *  Scale the velocities to simulate a coordinated move of the
         *  base and pickoff stages.
         */

        basRel        = fabs(baseTarget - OI_BAS_INCR);
        pkoRel        = fabs(pickoffTarget - OI_PKO_INCR);

        if (basRel > pkoRel)
        {
            basVel = OI_VELOCITY;
            pkoVel = OI_VELOCITY * pkoRel / basRel;
        }
        else
        {
            pkoVel = OI_VELOCITY;
            basVel = OI_VELOCITY * basRel / pkoRel;
        }
    }


    /*
     *  Load the calculated targets and velocities into the base and
     *  pickoff stage device control records then start the motion
     *  by calling executeCommand.
     */

    DEBUG(DAR_MSG_FULL, 
          "<%ld> %s:oiIndexMode: base target:%f\n", baseTarget);
    DEBUG(DAR_MSG_FULL, 
          "<%ld> %s:oiIndexMode: base velocity:%f\n", basVel);
    DEBUG(DAR_MSG_FULL, 
          "<%ld> %s:oiIndexMode: pickoff target:%f\n", pickoffTarget);
    DEBUG(DAR_MSG_FULL, 
          "<%ld> %s:oiIndexMode: pickoff velocity:%f\n", pkoVel);

    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->pos1), 
            (void *) par, DBR_DOUBLE,
            &(baseTarget), &nRequest)),
            return(oiCancelCommand (par)) );*/
    CHECKSTAT((status = dbPutLink(&(par->pos1),DBR_DOUBLE, &(baseTarget), nRequest)),return(oiCancelCommand(par)) );

    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->pos2), 
            (void *) par, DBR_DOUBLE,
            &(pickoffTarget), &nRequest)),
            return(oiCancelCommand (par)) );*/
    CHECKSTAT((status = dbPutLink(&(par->pos2),DBR_DOUBLE, &(pickoffTarget), nRequest)),return(oiCancelCommand(par)) );

    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->vel1), 
            (void *) par, DBR_DOUBLE,
            &(basVel), &nRequest)),
            return(oiCancelCommand (par)) );*/
    CHECKSTAT((status = dbPutLink(&(par->vel1),DBR_DOUBLE, &(basVel), nRequest)),return(oiCancelCommand(par)) );

    /*CHECKSTAT( (status = recGblPutLinkValue (&(par->vel2), 
            (void *) par, DBR_DOUBLE,
            &(pkoVel), &nRequest)),
            return(oiCancelCommand (par)) );*/
    CHECKSTAT((status = dbPutLink(&(par->vel2),DBR_DOUBLE, &(pkoVel), nRequest)),return(oiCancelCommand(par)) );

    return (oiExecuteCommand (par));
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiInitDeviceSupport
 *
 * INVOCATION:
 * status = oiInitDeviceSupport (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) assemblyControl record structure.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Initialize device support-specific functions
 *
 * DESCRIPTION:
 * Create an internal control structure to keep information specific to 
 * this instantiation of the record.  
 * Initialize critical record and internal control structure fields.
 *
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long oiInitDeviceSupport 
(
    ASSEMBLY_CONTROL_RECORD *par        /* assemblyControl record structure */
)
{
    OI_DEV_CONTROL_PRIVATE *pDevPvt;    /* internal control structure       */
    long status = DAR_S_SUCCESS;        /* status return                    */

    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiInitDeviceSupport: entry%c\n", ' ');


    /*
     * Create an internal control structure for this record and save it in
     * the parent record's structure.
     */

    pDevPvt = malloc (sizeof(OI_DEV_CONTROL_PRIVATE));
    if (pDevPvt == NULL)
    {
        status = DAR_E_MALLOC;
        recGblRecordError (status, par, __FILE__ ":no room: device private");
        return (status);
    }

    assAttachPrivateStruct (par, (void *) pDevPvt);


    /*
     * Create a mutual exclusion semaphore to protect the private structure
     * during asynchronous callback access.
     */
    
    pDevPvt->mutexSem = NULL;
    if (  (pDevPvt->mutexSem =
            semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE) ) == NULL )
    {
        DEBUG(DAR_MSG_FATAL, 
              "<%ld> %s:oiInitDeviceSupport: mutex failed.%c\n", ' ' );
        status = DAR_E_MALLOC;
        recGblRecordError (status, par, __FILE__ ":no room for mutex creation");
        return( status );
    }


    /*
     *  Initialize the stage active and motion modifier flags
     */

    semTake (pDevPvt->mutexSem, WAIT_FOREVER);
    CLEAR_ERR_MSG;
    pDevPvt->baseActive = OI_CMD_IDLE;
    pDevPvt->pickoffActive = OI_CMD_IDLE;
    pDevPvt->preIndexMove = FALSE;
    pDevPvt->pre2IndexMove = FALSE;
    pDevPvt->inLimMove = FALSE;
    pDevPvt->recoverTarget = FALSE;
    semGive (pDevPvt->mutexSem);

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiInitMode
 *
 * INVOCATION:
 * status = oiInitMode (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) assemblyControl record structure.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Initialize the base and pickoff stages
 *
 * DESCRIPTION:
 * Execute the initialization mode command that has already been 
 * loaded into the base and pickoff stages.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * INIT must have been written to the base and pickoff stage devicControl
 * record MODE fields.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long oiInitMode (ASSEMBLY_CONTROL_RECORD *par)
{
    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiInitMode: entry%c\n", ' ');

    
    /*
     *  Mode field has already been loaded so just execute the command.
     */

    return (oiExecuteCommand (par));

}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiMoveMode
 *
 * INVOCATION:
 * status = oiMoveMode (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) assemblyControl record structure.
 *
 * FUNCTION VALUE:
 * (long) function return status.
 *
 * PURPOSE:
 * Move the probe to a new position
 *
 * DESCRIPTION:
 * Execute a MODE of MOVE.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long oiMoveMode 
(
    ASSEMBLY_CONTROL_RECORD *par        /* assemblyControl record structure */
)
{
    OI_DEV_CONTROL_PRIVATE *pDevPvt;    /* Internal control structure       */
    double basMid;                      /* Intermediate base target         */
    double pkoMid;                      /* Intermediate pickoff target      */
    double basMidR;                     /* Above base target in radians     */
    double pkoMidR;                     /* Above pickoff tgarget in radians */
    double yMin;                        /* Minimum safe Y position          */
    double yMax;                        /* Maximum safe Y position          */
    double basRelMove;                  /* Distance base will move          */
    double pkoRelMove;                  /* Distance pickoff will move       */
    double basVel;                      /* Intermediate motion base vel.    */
    double pkoVel;                      /* Intermediate motion pickoff vel. */
    long nRequest = 1;                  /* Number of items to write to link */
    long   status = DAR_S_SUCCESS;      /* Function status                  */

    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiMoveMode: entry%c\n", ' ');

    pDevPvt = (OI_DEV_CONTROL_PRIVATE *) assGetPrivateStruct (par);


    /*
     *  In simulation mode there are no limits so just go to
     *  the target positions.
     */

    if (par->simm)
    {
        return (oiExecuteCommand (par));
    }


    /*
     *  Otherwise this is the real world where things can get messy...
     *  Check to see if the trajectory the probe tip will take in moving
     *  to the new position is likely to take it outside the safe motion
     *  limits.   If the calculation function returns non-zero then the
     *  probe will go outside the limits and an intermediate motion is
     *  required.
     */

    if (gmOiwfsCalculateVerticalRange( OI_BAS_INCR * D2R,
                                       OI_PKO_INCR * D2R,
                                       pDevPvt->baseAngle * D2R,
                                       pDevPvt->pickoffAngle * D2R,
                                       OI_Y_LO_LIM, OI_Y_HI_LIM,
                                       &yMin, &yMax,
                                       &basMidR, &pkoMidR ) )
    {
        /*
         *  Intermediate move is required, convert the intermediate
         *  positions calculated above from radians to degs
         */

        basMid = basMidR * R2D;
        pkoMid = pkoMidR * R2D;

        semTake (pDevPvt->mutexSem, WAIT_FOREVER);
        pDevPvt->inLimMove = TRUE;
        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiMoveMode: intermediate move required%c\n", ' ');

        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiMoveMode: current base:%f\n", OI_BAS_INCR);
        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiMoveMode: current pickoff:%f\n", OI_PKO_INCR);
        /*
         *  Save the original target positoins to be recovered after the
         *  intermediate motion is finished.
         */ 

        pDevPvt->Xtarget = OI_X_TARGET;
        pDevPvt->Ytarget = OI_Y_TARGET;
        semGive (pDevPvt->mutexSem);


        /*
         *  Write intermediate move positions to target devices
         */

        /*CHECKSTAT( (status = recGblPutLinkValue (&(par->pos1), 
                (void *) par, DBR_DOUBLE,
                &(basMid), &nRequest)),
                return(oiCancelCommand (par)) );*/
        CHECKSTAT((status = dbPutLink(&(par->pos1),DBR_DOUBLE, &(basMid), nRequest)),return(oiCancelCommand(par)) );

        /*CHECKSTAT( (status = recGblPutLinkValue (&(par->pos2), 
                (void *) par, DBR_DOUBLE,
                &(pkoMid), &nRequest)),
                return(oiCancelCommand (par)) );*/
        CHECKSTAT((status = dbPutLink(&(par->pos2),DBR_DOUBLE, &(pkoMid), nRequest)),return(oiCancelCommand(par)) );

        /*
         *  Simulate a coordinated motion by scaling the velocities.
         */

        basRelMove = fabs(basMid - OI_BAS_INCR);
        pkoRelMove = fabs(pkoMid - OI_PKO_INCR);

        if (basRelMove > pkoRelMove)
        {
            basVel = OI_VELOCITY;
            pkoVel = OI_VELOCITY * pkoRelMove / basRelMove;
        }
        else
        {
            basVel = OI_VELOCITY * basRelMove / pkoRelMove;
            pkoVel = OI_VELOCITY;
        }

        /*
         *  Write intermediate move velocities to target devices
         */

        /*CHECKSTAT( (status = recGblPutLinkValue (&(par->vel1), 
                (void *) par, DBR_DOUBLE,
                &(basVel), &nRequest)),
                return(oiCancelCommand (par)) );*/
        CHECKSTAT((status = dbPutLink(&(par->vel1),DBR_DOUBLE, &(basVel), nRequest)),return(oiCancelCommand(par)) );

        /*CHECKSTAT( (status = recGblPutLinkValue (&(par->vel2), 
                (void *) par, DBR_DOUBLE,
                &(pkoVel), &nRequest)),
                return(oiCancelCommand (par)) );*/
        CHECKSTAT((status = dbPutLink(&(par->vel2),DBR_DOUBLE, &(pkoVel), nRequest)),return(oiCancelCommand(par)) );

        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiMoveMode: base target:%f\n", basMid);
        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiMoveMode: base velocity:%f\n", basVel);
        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiMoveMode: pickoff target:%f\n", pkoMid);
        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:oiMoveMode: pickoff velocity:%f\n", pkoVel);

    }

    else
    {

        /*
         *  Intermediate move is not required, so reset the flag here
         *  in case of a move while moving directive
         */

        semTake (pDevPvt->mutexSem, WAIT_FOREVER);
        pDevPvt->inLimMove = FALSE;
        semGive (pDevPvt->mutexSem);
    }

    /*
     *  Start the motion by calling the executeCommand function.
     */

    return (oiExecuteCommand (par));
}




/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiProcessFault
 *
 * INVOCATION:
 * status = oiProcessFault (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Kills the current process in response to an internal interlock
 *
 * DESCRIPTION:
 * If the record is busy processing a command abort it immediately by calling
 * oiCancelCommand.
 * Otherwise there is noting for device support to do so simply return.
 * 
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * 
 *-
 ************************************************************************
 */

static long oiProcessFault
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.           */
)
{
    OI_DEV_CONTROL_PRIVATE *pDevPvt;    /* Internal control structure       */
    long     status = DAR_S_SUCCESS;    /* Return function status.          */

    DEBUG(DAR_MSG_MAX, "<%ld> %s:oiProcessFault: entry%c\n", ' ');

    pDevPvt = (OI_DEV_CONTROL_PRIVATE *) assGetPrivateStruct (par);


    /*
     *   Kill any active processes immediately.
     */

    if (par->busy == DAR_BUSY_BUSY)
    {
        DEBUG(DAR_MSG_LOG, "<%ld> %s:oiProcessFault: Cancelling current command%c\n", ' ');
        semTake (pDevPvt->mutexSem, WAIT_FOREVER);
        SET_ERR_MSG ( "External interlock detected!" );
        semGive (pDevPvt->mutexSem);
        status = DAR_E_FLT_CLR;
        oiCancelCommand( par );
    } 

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiTestMode
 *
 * INVOCATION:
 * status = oiTestMode (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) assemblyControl record structure.
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Test the base and pickoff stages to insure that they are functional
 *
 * DESCRIPTION:
 * Execute the test mode command that has already been 
 * loaded into the base and pickoff stages.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * TEST must have been written to the base and pickoff stage devicControl
 * record MODE fields.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long oiTestMode 
(
    ASSEMBLY_CONTROL_RECORD *par        /* assemblyControl record structure */
)
{
    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiTestMode: entry%c\n", ' ');

    /*
     *  Mode field has already been loaded so just execute the command.
     */

    return (oiExecuteCommand (par));
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiTrackMode
 *
 * INVOCATION:
 * status = oiTrackMode (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to device support structure.
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Execute a MODE of TRACK
 *
 * DESCRIPTION:
 * Move to the position that has already been loaded into the base
 * and pickoff stages.  Note that no limit checking of any sort is
 * done in tracking mode since there is no way to tell the tracking
 * source that it is making an invalid move.....
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * TRACK must have been written to the base and pickoff stage devicControl
 * record MODE fields.  The base and pickoff target positions and velocities
 * must have been loaded into these records as well.
 *
 * Execute a MODE of TRACK.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long oiTrackMode 
(
    ASSEMBLY_CONTROL_RECORD *par        /* assemblyControl record structure */
)
{
    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiTrackMode: entry%c\n", ' ');

    /*
     *  Mode, position and velocity fields have already been loaded so 
     *  just execute the command.
     */

    return (oiExecuteCommand (par));
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiUpdateMode
 *
 * INVOCATION:
 * status = oiUpdateMode (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) assemblyControl record structure.
 *
 * FUNCTION VALUE:
 * (long) success code.
 *
 * PURPOSE:
 * Accept update requests without doing anything
 *
 * DESCRIPTION:
 * The OIWFS does not have any removable devices to update so simply
 * indicate that the command completed successfully.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long oiUpdateMode 
(
    ASSEMBLY_CONTROL_RECORD *par        /* assemblyControl record structure */
)
{
    long     status = DAR_S_SUCCESS;    /* Returned function status.    */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:oiUpdateMode: doing nothing%c\n", ' ');


    /*
     *   Nothing to update so tell record that command has completed
     *   successfully.
     */

    assCommandFinish( par, status, NULL );

    return (status);
}

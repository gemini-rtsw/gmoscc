static char rcsid[] = "$Id$";
/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) <1998>                           (c) <1998>
 * National Research Council            Conseil national de recherches
 * Ottawa, Canada, K1A 0R6              Ottawa, Canada, K1A 0R6
 * All rights reserved                  Tous droits reserves
 *                    
 * NRC disclaims any warranties,        Le CNRC denie toute garantie
 * expressed, implied, or statu-        enoncee, implicite ou legale,
 * tory, of any kind with respect       de quelque nature que se soit,
 * to the software, including           concernant le logiciel, y com-
 * without limitation any war-          pris sans restriction toute
 * ranty of merchantability or          garantie de valeur marchande
 * fitness for a particular pur-        ou de pertinence pour un usage
 * pose.  NRC shall not be liable       particulier.  Le CNRC ne
 * in any event for any damages,        pourra en aucun cas etre tenu
 * whether direct or indirect,          responsable de tout dommage,
 * special or general, consequen-       direct ou indirect, particul-
 * tial or incidental, arising          ier ou general, accessoire ou
 * from the use of the software.        fortuit, resultant de l'utili-
 *                                      sation du logiciel.
 *
 ************************************************************************
 *
 * FILENAME
 * recAssControl.c
 *
 * PURPOSE:
 * Provide EPICS Record Support code for the assemblyControl record.
 *
 * FUNCTION NAME(S)
 * alarm                    Raise an alarm.
 * assAttachPrivateStruct   Save a private struct pointer in record struct.
 * assAddErrorMessage       Set the error message field.
 * assCommandFinish         Indicate that command processing has finished.
 * assCommandTimedOut       Return command timeout flag.
 * assDisplayPrivateStruct  display contents of private structure (for debugging)
 * assGetPrivateStruct      return the saved private struct pointer.
 * assSimulateLevel         Return current simulation level.
 * assStartTimer            Start the command timeout timer.
 * assStopTimer             Stop the command timeout timer.
 * busyReceivedFromDevice   React to a device control record BUSY state change.
 * completeCommand          Set status for a complete command.
 * cvtDbAddr                To handle attributes A-E.
 * executeState             Execute a function via an indirect pointer.
 * getAlarmDouble           Determine alarm double info about a field.
 * assGetBusResponse        Return flags indicating which BUSY fields changed.
 * getControlDouble         Determine control info about a field.
 * getGraphicsDouble        Determine graphics info about a field.
 * getPrecision             Get precision of a field for display.   
 * getValue                 Return the VAL field.   
 * idleState                Change record to an IDLE state.
 * initDevLinks             Initialize the device record links.
 * initFields               Initialize fields that require allocs.
 * initLinks                Initialize input and output links.
 * initRecord               Initialize the record.
 * initTimer                Initialize the watchdog timer.
 * linkSet                  Initialize the set links.
 * monitor                  Raise monitors on selected fields.
 * process                  Process the record.
 * processStart             Process a start/preset directive.
 * processDirective         Process all directives.
 * processState             Process a state change.
 * readInputLinks           Read appropriate input links.
 * setFields                Create variable size database fields.
 * setFlags                 Set record status flag fields.
 * special                  Trigger internal reprocessing on field change.
 * specialCallback          Internal reprocessing callback task.
 * stoppingState            Stop a command in progress.
 * timerCallback            Command timeout timer callback task.
 * unsetFlags               Clear record status flag fields.
 * writeOutputLinks         Write data to selected output links.
 *
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.24  2001/03/20 13:40:31  gmos
 * Modified DEBUG macro. All files now use printf() rather than logMsg(). All also print the output from taskName(0).
 *
 * Revision 1.23  2001/03/07 13:43:35  gmos
 * Further HEALTH developments - Set health BAD if a TEST command fails. Set health WARNING is a general motion commands results in a loss of index.
 *
 * Revision 1.22  2001/03/06 16:30:31  gmos
 * A better idea. Set the health to BAD after an INIT, INDEX or PARK failure if the index is lost and to WARNING if the index is preserved.
 *
 * Revision 1.21  2001/03/06 16:15:44  gmos
 * Set the health to WARNING rather than BAD after an INIT, INDEX or PARK failure. BAD is reserved for catestrophic errors.
 *
 * Revision 1.20  2001/03/01 14:14:27  gmos
 * Switched to use assembly message levels defined in darMessageLevels.h.
 *
 * Revision 1.19  2001/02/26 16:28:34  gmos
 * Changed two more DEBUG messages associated with errors that don't appear to set the MESS field, in cvtDbAddr and executeState, from DAR_MSG_MIN to DAR_MSG_FATAL.
 *
 * Revision 1.18  2001/02/23 13:19:18  gmos
 * VERSION changed to 2.0 ready for GMOS acceptance tests.
 *
 * Revision 1.17  2000/11/16 10:25:46  gmos
 * Extra checks included in assGetPrivateStructure. Magic value included in private structure. assDisplayprivateStructure debugging function added.
 *
 * Revision 1.16  2000/11/10 14:10:15  gmos
 * Reset health to GOOD if any movement command finishes successfully.
 *
 * Revision 1.15  2000/11/10 00:15:20  gmos
 * Prevent clearing of rejection MESS after successful command in
 * assCommandFinish().
 *
 * Revision 1.14  2000/10/26 08:47:08  gmos
 * Obviously not tested well enough. V1.13 ignored the next command immediately after a fault was cleared. V1.12 restored.
 *
 * Revision 1.13  2000/10/24 12:51:23  gmos
 * Reset health to GOOD when fault cleared. Tested in simulation mode.
 *
 * Revision 1.12  2000/09/26 15:20:56  gmos
 * Fatal errors which are not reported using recGblRecordError or via the MESS field changed from DAR_MSG_MIN to DAR_MSG_FATAL so a message always appears.
 *
 * Revision 1.11  2000/09/07 15:49:19  gmos
 * INIT, INDX and PARK flags now set correctly in VSM mode. The only thing missing was the lastcommand. setFlags was not needed.
 *
 * Revision 1.10  2000/09/07 08:40:18  gmos
 * Use setFlags to set the INIT, INDX and PARK flags in VSM simulation mode.
 *
 * Revision 1.9  2000/09/06 16:54:25  gmos
 * Set assembly record INIT, INDX and PARK flags in VSM simulation mode.
 *
 * Revision 1.8  2000/09/04 09:29:31  gmos
 * Allow simulation mode to be changed in the ERR state as well as in the IDLE state.
 *
 * Revision 1.7  2000/07/25 11:11:14  gmos
 * Do not clear MARK field if PRESET succeeds.
 *
 * Revision 1.6  2000/07/12 17:00:22  gmos
 * Changed unsetFlags so that UPDATE failure causes loss of INDX flag and any movement failure causes loss of PARK flag. Moved the ILCK section into the INDX flag part. Added comments to setFlags and unsetFlags.
 *
 * Revision 1.5  2000/07/06 17:25:22  gmos
 * Williams new code, tested and debugged, chris and bobs modifications added
 *
 * Revision 1.30  2000/02/15 23:22:15  angelic
 * assembly starts with INIT flag false
 *
 * Revision 1.29  2000/02/10 21:37:11  angelic
 * removed function processFault
 * added call to DSET function processFault if it exists
 *
 * Revision 1.28  2000/02/08 22:45:16  angelic
 * fixed so that lastCommand is unset when a stop directive is received
 * removed redundant setting of keepIndex in assCommandFinish
 * cleaned up some DEBUG statements (set to MIN)
 *
 * Revision 1.27  2000/02/05 02:19:25  angelic
 * prevent PRESET while BUSY from rejecting
 *
 * Revision 1.26  2000/02/04 21:50:13  angelic
 * fixed so that mark is properly cleared after a start directive
 *
 * Revision 1.25  2000/02/03 21:39:17  angelic
 * Fixed bug so that the Val field is not cleared when the assembly
 * is not marked
 *
 * Revision 1.24  1999/12/21 22:48:08  angelic
 * assembly must be initialized before indexing
 * unless there's a failure, assembly starts up initialized
 *
 * Revision 1.23  1999/11/16 19:59:17  angelic
 * added semaphore calls to protect the private structure
 *
 * Revision 1.22  1999/11/13 00:38:23  angelic
 * fixed passing of DBUG level changes down to devices
 * fixed bug in VSM simulation mode
 *
 * Revision 1.21  1999/11/06 01:12:41  angelic
 * fixed busy-err transition for command failures
 * do not clear index when incorrect barcode is read
 *
 * Revision 1.20  1999/11/04 23:44:38  angelic
 * fixed command post processing so that the record will respond to pp links
 *
 * Revision 1.19  1999/11/03 23:06:59  angelic
 * fixed Move while moving, PRESET is no longer passed to Device if Busy
 * fixed fault handling, fixed semaphore conflict accessing pPriv
 *
 * Revision 1.18  1999/10/27 00:22:19  rambold
 * Cleaned up prior to final acceptance tests
 *
 * Revision 1.17  1999/10/25 18:14:37  angelic
 * fixed fault handling and pp response bug
 *
 * Revision 1.16  1999/10/05 17:21:11  rambold
 * *** empty log message ***
 *
 * Revision 1.15  1999/09/02 20:12:17  angelic
 * implemented passing dbug and simm out to dbgl and siml
 * don't init sensor out links in simulation mode
 * base number of devices on directive rahter than position link
 *
 * Revision 1.14  1999/08/18 23:51:56  angelic
 * fixed clearing of index and error message
 *
 * Revision 1.13  1999/08/13 19:36:38  angelic
 * added check for indexed before sending a move to deviceControl records
 *
 * Revision 1.12  1999/07/30 17:50:41  rambold
 * removed setting mark field in cvtDbaddr
 *
 * Revision 1.11  1999/07/28 19:54:03  rambold
 * changed error reporting and cleared busy flags on read
 *
 * Revision 1.10  1999/07/23 20:50:00  angelic
 * debugging
 *
 * Revision 1.9  1999/06/25 17:49:34  angelic
 * fixed additional ErrMess string length memory problem
 *
 * Revision 1.8  1999/06/22 21:25:14  angelic
 * debugging
 *
 * Revision 1.7  1999/06/16 20:38:17  angelic
 * *** empty log message ***
 *
 * Revision 1.6  1999/06/02 18:19:37  dunn
 * Fixed a simulate mode check.
 *
 * Revision 1.5  1999/05/27 15:57:35  dunn
 * Change to make special() mark the record if there is a change in MODE>
 *
 * Revision 1.4  1999/05/26 18:07:41  dunn
 * Lots of changes and bug fixes.  Most changes are the providing
 * of functions to be used by the device layer.  They all begin
 * with "ass", for assemblyControl.
 *
 * Revision 1.3  1999/05/12 20:56:21  dunn
 * Some variable cleanup.
 *
 * Revision 1.2  1999/05/07 17:11:06  dunn
 * Changed it so that special calls a callback which will then process
 * the record.
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
#include        <taskLib.h>

#include        <alarm.h>
#include        <callback.h>
#include        <dbDefs.h>
#include        <dbAccess.h>
#include        <dbFldTypes.h>
#include        <devSup.h>
#include        <errMdef.h>
#include        <recSup.h>
#include        <special.h>
#include        <logLib.h>
#include        <tickLib.h>         
#include        <sysLib.h>          
#include        <dbRecDes.h>        
#include        <dbEvent.h>         

#include        <cad.h>
#include        <assemblyControlRecord.h>
#include        <recAssControl.h>
#include        <devAssControl.h>

#include	<darMessageLevels.h>     /* Assembly message level definitions.               */


/*
 *  Local Defines
 */

/* Provide NULL pointers for EPICS functions not implemented.   */
#define report                  NULL    /* no function supplied */
#define initialize              NULL    /* no function supplied */
#define getArrayInfo            NULL    /* no function supplied */
#define getEnumStr              NULL    /* no function supplied */
#define getEnumStrs             NULL    /* no function supplied */
#define putEnumStr              NULL    /* no function supplied */
#define putArrayInfo            NULL    /* no function supplied */
#define getUnits                NULL    /* no function supplied.*/

/* Define system constants */
#define DAR_ACKNOWLEDGE_TIME    5       /* BUSY time acknowledge cycle      */
#define VERSION                 "2.0"   /* Record code version number       */
#define DAR_STOP_CMD            -99     /* bogus command stopped flag       */

/* Define interlock field processing flags */
#define DAR_FAULT_CLEAR         0       /* no fault detected                */
#define DAR_FAULT_WARNING       1       /* fault detected, need to callback */
#define DAR_FAULT_NEW           2       /* time to handle fault             */

/* These are the names of the attribute input fields */
static char Ifldnames[DAR_ARG_MAX][FLDNAME_SZ+1] =
{ "A", "B", "C", "D", "E" };

/* These are the names of the sensor input fields */
static char SIfldnames[DAR_ARG_MAX][FLDNAME_SZ+1] =
{ "SIJ", "SIK", "SIL", "SIM", "SIN" };

/* These are the names of the value output fields */
static char Ofldnames[DAR_ARG_MAX][FLDNAME_SZ+1] =
{ "VALA", "VALB", "VALC", "VALD", "VALE" };

/* This is a buffer area into which one of the above is copied */
static char fldnames[DAR_ARG_MAX][FLDNAME_SZ+1];




/*
 * Bitmap mnemonics for EPICS record filds that need monitors raised
 * or output links processed
 */

#define RECORD_DIR              0x00000001
#define RECORD_MODE             0x00000002
#define RECORD_INIT             0x00000004
#define RECORD_INDX             0x00000008
#define RECORD_HTH              0x00000010
#define RECORD_BUSY             0x00000020
#define RECORD_ACK              0x00000040
#define RECORD_FLT              0x00000080

#define RECORD_DBUG             0x00000100
#define RECORD_SIMM             0x00000200
#define RECORD_MARK             0x00000400
#define RECORD_VAL              0x00000800
#define RECORD_PARK             0x00001000
#define RECORD_MESS             0x00002000
#define RECORD_ASTA             0x00800000

/*
 ************************************************************************
 * Somewhat funky macros that allow the record and device support code to
 * control which fields have monitors raised or output links processed
 * whenever a record processes.
 *
 * MONITOR(RECORD_XXX) will cause the record support code to raise a monitor
 * on the given field when the record next processes.
 * 
 * Saying TRIGGER(M_XXX) will cause the record support code to trigger the
 * output link for the given field when the record processes.
 *
 *    --- NOTE WELL ---
 *    The macros below assume that the variable "par" exists and points to a
 *    assemblyControl record, like so:
 *        ASSEMBLY_CONTROL_RECORD *par;
 *    No check is made in this code to ensure that this really is true.
 ******************************************************************************
 */

#define MONITOR(a)              par->mmap |= (a);
#define MONITORED(a)            par->mmap & (a)
#define UNMONITOR(a)            par->mmap &= ~(a);
#define UNMONITOR_ALL           par->mmap = 0;

#define TRIGGER(a)              par->lmap |= (a);
#define TRIGGERED(a)            par->lmap & (a)
#define UNTRIGGER(a)            par->lmap &= ~(a);
#define UNTRIGGER_ALL           par->lmap = 0;


/*
 *  Control structure template for callback function
 */

struct callback {
    CALLBACK        callback;           /* system callback structure */
    struct dbCommon *precord;           /* common record field struct */
    WDOG_ID         wd_id;              /* ID number for this watchdog */
};


/*
 *  Local function Prototypes
 */

/* epics support */
static long     cvtDbaddr(struct dbAddr * );
static long     getAlarmDouble(struct dbAddr *, struct dbr_alDouble *);
static long     getControlDouble(struct dbAddr *, struct dbr_ctrlDouble *);
static long     getGraphicDouble(struct dbAddr *, struct dbr_grDouble *);
static long     getPrecision(struct dbAddr *, long *);
static long     getValue(ASSEMBLY_CONTROL_RECORD *, struct valueDes *);
static long     process(ASSEMBLY_CONTROL_RECORD *);
static long     special(struct dbAddr *, int);


/* internal processing */
static void     alarm(ASSEMBLY_CONTROL_RECORD *);
static long     busyReceivedFromDevice(ASSEMBLY_CONTROL_RECORD * );
void            callbackRequest();
static void     completeCommand(ASSEMBLY_CONTROL_RECORD *, long * );
static long     executeState( ASSEMBLY_CONTROL_RECORD *, int, DEVSUPFUN );
static long     idleState( ASSEMBLY_CONTROL_RECORD *, long );
static long     initDevLinks( ASSEMBLY_CONTROL_RECORD    * );
static long     initFields(ASSEMBLY_CONTROL_RECORD *);
static long     initLinks(ASSEMBLY_CONTROL_RECORD *);
static long     initRecord(ASSEMBLY_CONTROL_RECORD *, int);
static long     initTimer( ASSEMBLY_CONTROL_RECORD * );
static long     linkSet(ASSEMBLY_CONTROL_RECORD *,struct link *,int,char *,int *);
static void     monitor(ASSEMBLY_CONTROL_RECORD *);
static long     processDirective( ASSEMBLY_CONTROL_RECORD * );
static long     processStart( ASSEMBLY_CONTROL_RECORD * );
static long     processState( ASSEMBLY_CONTROL_RECORD * );
static long     readInputLinks(ASSEMBLY_CONTROL_RECORD *, int );
static long     setFields(ASSEMBLY_CONTROL_RECORD *,void **,unsigned short *);
static void     setFlags(ASSEMBLY_CONTROL_RECORD * );
static void     specialCallback( struct callback *);
static long     stoppingState( ASSEMBLY_CONTROL_RECORD * );
static void     timerCallback( struct callback *);
static void     unsetFlags(ASSEMBLY_CONTROL_RECORD * );
static long     writeOutputLinks (ASSEMBLY_CONTROL_RECORD *);


/*
 *  Data Structures
 */

/* EPICS record support function definition structure */
struct rset assemblyControlRSET = {
    RSETNUMBER,
    report,
    initialize,
    initRecord,
    process,
    special,
    getValue,
    cvtDbaddr,
    getArrayInfo,
    putArrayInfo,
    getUnits,
    getPrecision,
    getEnumStr,
    getEnumStrs,
    putEnumStr,
    getGraphicDouble,
    getControlDouble,
    getAlarmDouble
};


/*
 *  Macros
 */

#define DEBUG(l,FMT,V)                                                  \
{                                                                       \
    int k=l;                                                            \
    if ( k <= par->dbug)                                                \
    printf ("%s: "FMT, taskName(0), tickGet(), par->name, V);                              \
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * alarm
 *
 * INVOCATION:
 * alarm (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Generate EPICS system alarms
 *
 * DESCRIPTION:
 * Raise an alarm if the udf field is set.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static void alarm
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.    */
)
{
    DEBUG(DAR_MSG_MAX, "<%d> %s:alarm: entry%c\n", ' ');

    if (par->udf == TRUE)
    {
        recGblSetSevr(par, UDF_ALARM, INVALID_ALARM);
    }

    return;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * assAddErrorMessage
 *
 * INVOCATION:
 * assAddErrorMessage ( par, errorString );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 * (>) errorString  (char *) Pointer to an error string.
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Save the given errorString passed in the record MESS field.
 *
 * DESCRIPTION:
 * Copy the error string, up to the maximum number of EPICS string characters
 * into the record MESS field and then raise a monitor on the field.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

void assAddErrorMessage
(
    ASSEMBLY_CONTROL_RECORD
            *par,               /* (in)  Ptr to ass. rec.    */
    char    *errorString        /* (in)  Ptr. to error msg.    */
)
{
    DEBUG(DAR_MSG_MAX, "<%d> %s:assAddErrorMessage:entry:%s\n",errorString);


    /*
     * Copy up to MAX_STRING_SIZE -1 characters into the MESS field
     */

    strncpy (par->mess, errorString, MAX_STRING_SIZE-1 );
    MONITOR(RECORD_MESS);
    return;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * assAttachPrivateStruct
 *
 * INVOCATION:
 * assAttachPrivateStruct ( par, pDevPrivate );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 * (>) pDevPrivate  (void *) Pointer to the device layer private structure.
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Save device support control structure location.
 *
 * DESCRIPTION:
 * Store the given pointer to the device support's control structure in the 
 * internal control structure for this record.   In future it can be recovered
 * by the device support code via the assGetPrivateStruct function.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * The record's internal control structure must have already been created
 * and a pointer to it stored in the record dpvt field.
 *
 * SEE ALSO:
 * assGetPrivateStruct
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

void assAttachPrivateStruct
(
    ASSEMBLY_CONTROL_RECORD *par,           /* (in)  Ptr to ass. rec.    */
    void    *pDevPrivate                    /* (in)  Ptr. to device priv.*/
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struct */

    DEBUG(DAR_MSG_MAX, "<%d> %s:assAttachPrivateStruct: entry%c\n", ' ');
 

    /*
     * Store device support struct ptr in record internal control struct 
     */

    pPriv->pPrivate = (void *) pDevPrivate;

}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * assCommandFinish
 *
 * INVOCATION:
 * assCommandFinish ( par, cmdStatus, errorMessage );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 * (>) cmdStatus  (int)                 Command processing success code.  
 * (>) errorMessage (char *)            Processing error message to be saved. 
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Signal the completion of command processing.
 *
 * DESCRIPTION:
 * Indicate to the record processing code that device code has
 * finished all processing of the current command and set the error
 * message and internal command success flag via the following algorithm:
 *
 *      IF processing error occurred
 *      {
 *          Save error message
 *          Clear command success flag
 *      }
 *      ELSE
 *      {
 *          Clear error message
 *          Set command success flag
 *      }
 *      Set command completion flag
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * Error message supplied will overwrite any message saved previously
 * via a call to the assAddErrorMessage function.
 *-
 ************************************************************************
 */

void assCommandFinish
(
    ASSEMBLY_CONTROL_RECORD  *par,  /* (in)  Ptr to ass. rec.    */
    int     cmdStatus,              /* (in)  Command finish status.    */
    char    *errorMsg               /* (in)  Error message.        */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt;     /* Internal control struct */

    DEBUG(DAR_MSG_MAX, "<%d> %s:assCommandFinish: entry%c\n", ' ');
   

    /*
     * If command failed save message, raise monitor and clear success flag
     */

    if ( cmdStatus != DAR_S_SUCCESS )
    {
        DEBUG(DAR_MSG_ERROR,"<%d> %s:assCommandFinish:command failed - %s\n",
              errorMsg );
        strncpy (par->mess, errorMsg, MAX_STRING_SIZE-1 );
        MONITOR (RECORD_MESS);
        pPriv->cmdResponse = FALSE;
    }


    /*
     * Otherwise success, clear any existing message and set success flag 
     */

    else
    {
        DEBUG(DAR_MSG_FULL, "<%d> %s:assCommandFinish: command success%c\n",' ' );

/* uncomment this if you want command rejection error messages
 * cleared by successful command completion 
 *      if (strlen (par->mess))
 *      {
 *          *par->mess = '\0';
 *          MONITOR (RECORD_MESS);
 *      }
 */
        pPriv->cmdResponse = TRUE;
    }
  

    /*
     *  Either way command is finished, so set flag to process the finish.
     */

    pPriv->cmdFinished = TRUE;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * assCommandTimedOut
 *
 * INVOCATION:
 * result = assCommandTimedOut( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 *
 * FUNCTION VALUE:
 * (int) TRUE indicates the command has timed out, otherwise FALSE.
 *
 * PURPOSE:
 * Return command timeout information.
 *
 * DESCRIPTION:
 * Return the current state of the command timedout flag to the calling
 * function.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

int assCommandTimedOut
(
    ASSEMBLY_CONTROL_RECORD *par            /* (in)  Ptr to ass. rec.  */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struct */

    DEBUG(DAR_MSG_MAX, "<%d> %s:assCommandTimedOut: entry%c\n", ' ');


/*
 * Return the current state of the command timedout flag.
 */

    return ( pPriv->cmdTimedOut );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * assGetPrivateStruct
 *
 * INVOCATION:
 * myPrivatePtr = assGetPrivateStruct ( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 *
 * FUNCTION VALUE:
 * (void *) Pointer to the device layer private structure (NULL if there is a problem).
 *
 * PURPOSE:
 * Recover the device support control structure pointer.
 *
 * DESCRIPTION:
 * The pointer saved by the last call to assAttachPrivateStruct is
 * returned to the calling function.  This is ususally used by the 
 * device support code to recover the device support control structure 
 * associated with the record instance being processed.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * A pointer must have been saved by a call to assAttachPrivateStruct.
 *
 * SEE ALSO:
 * assAttachPrivateStruct
 * assDisplayprivateStruct
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

void *assGetPrivateStruct
(
    ASSEMBLY_CONTROL_RECORD *par            /* (in)  Ptr to ass. rec.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struct   */

    DEBUG(DAR_MSG_MAX, "<%d> %s:assGetPrivateStruct: entry%c\n", ' ');

    /*
     * Check the pointers supplied
     */

     if ( (par == NULL) || (pPriv == NULL) )
     {

        DEBUG(DAR_MSG_FATAL, "<%d> %s:assGetPrivateStruct: bad private structure pointer%c\n", ' ');
        return ( NULL );
     }

    /*
     * Check the magic value is still intact. If this value is not seen it is likely that the
     * private structure pointer has become corrupted.
     */

     if ( pPriv->magic != DAR_MAGIC )
     {

        DEBUG(DAR_MSG_FATAL, "<%d> %s:assGetPrivateStruct: magic value not seen, pPriv=%p\n", pPriv);
        return ( NULL );
     }

    /*
     * Check the device support control structure pointer is not NULL.
     */

     if ( pPriv->pPrivate == NULL )
     {

        DEBUG(DAR_MSG_FATAL, "<%d> %s:assGetPrivateStruct: NULL device support structure pointer%c\n", ' ');
        return ( NULL );
     }

    /*
     * Otherwise, return the contents of the local control struct pPrivate field
     */

     return ( pPriv->pPrivate );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * assDisplayPrivateStruct
 *
 * INVOCATION:
 * assDisplayPrivateStruct ( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Display the contents of the private structure.
 *
 * DESCRIPTION:
 * The contents of the private structure are displayed on the VxWorks
 * console with printf statements. The function is more use when executed at
 * the VxWorks console.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * assAttachPrivateStruct
 * assGetPrivateStruct
 *
 * DEFICIENCIES:
 * Use of this function will severely degrade performance. It is for debugging only.
 *-
 ************************************************************************
 */

void assDisplayPrivateStruct
(
    ASSEMBLY_CONTROL_RECORD *par            /* (in)  Ptr to ass. rec.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struct   */

    printf ( "Contents of assembly structure at par=%p, pPriv=%p\n",
              par, pPriv );

    if ( (par == NULL) || (pPriv == NULL) )
    {
        printf ("NULL structure!\n");
    }
    else
    {
        printf ("pPriv->bus1        = %d\n", pPriv->bus1);
        printf ("pPriv->bus2        = %d\n", pPriv->bus2);
        printf ("pPriv->bus3        = %d\n", pPriv->bus3);
        printf ("pPriv->bus4        = %d\n", pPriv->bus4);
        printf ("pPriv->busChange   = %d\n", pPriv->busChange);
        printf ("pPriv->cmdFinished = %d\n", pPriv->cmdFinished);
        printf ("pPriv->showBusy    = %d\n", pPriv->showBusy);
        printf ("pPriv->cmdResponse = %d\n", pPriv->cmdResponse);
        printf ("pPriv->keepIndex   = %d\n", pPriv->keepIndex);
        printf ("pPriv->cmdTimedOut = %d\n", pPriv->cmdTimedOut);
        printf ("pPriv->debug       = %d\n", pPriv->debug);
        printf ("pPriv->fault       = %d\n", pPriv->fault);
        printf ("pPriv->lastCommand = %d\n", pPriv->lastCommand);
        printf ("pPriv->currCommand = %d\n", pPriv->currCommand);
        printf ("pPriv->pPrivate    = %p\n", pPriv->pPrivate);
        printf ("pPriv->simulation  = %d\n", pPriv->simulation);
        printf ("pPriv->special     = %p\n", pPriv->special);
        printf ("pPriv->timeout     = %d\n", pPriv->timeout);
        printf ("pPriv->timer       = %p\n", pPriv->timer);
        printf ("pPriv->magic       = %x (c.f. %x)\n", pPriv->magic, DAR_MAGIC);
    }

    return;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * assSimulateLevel
 *
 * INVOCATION:
 * sim = assSimulateLevel ( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 *
 * FUNCTION VALUE:
 * (int) Simulate level. 0=None, 1=Vsm, 2=FAST 3=FULL.
 *
 * PURPOSE:
 * Return the current processing simulation level.
 *
 * DESCRIPTION:
 * The processing simulation level currently in effect is returned to the
 * calling function.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

int assSimulateLevel
(
    ASSEMBLY_CONTROL_RECORD *par            /* (in)  Ptr to ass. rec.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Ptr to private rec. struct.    */

    DEBUG(DAR_MSG_MAX, "<%d> %s:assSimulateLevel: entry%c\n", ' ');

    /*
     * return the current state of the simulation flag
     */

    return ( pPriv->simulation );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * assStartTimer
 *
 * INVOCATION:
 * assStartTimer ( par, &status, secs );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 * (<) status (long *) Function status return.
 * (>) secs (int)      Preset value for the timer in seconds.
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Start the watchdog timer.
 *
 * DESCRIPTION:
 * The command timeout timer is preset and started by a call to this function
 * via the following algorithm:
 *
 *      Recover timeout timer callback structure
 *      Convert pre-load time from seconds to system ticks
 *      Ask the EPICS watchdog task to invoke the associated callback 
 *          function when the given time interval expires
 *  
 * Usage:   The timeout timer is started by the device support code when a 
 * new command is recived.  This timer is pre-loaded with a time interval 
 * longer than the command is expected to take.  When the command finishes 
 * this timer is stopped by a call to assStopTimer.  If the timer expires 
 * before the command completes then something must have gone wrong.   The 
 * callback function invoked by the EPICS watchdog function after the timeout
 * interval will abort the command in progress and try to return control to 
 * the record processing code. 
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * initTimer
 * assStopTimer
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */
void    assStartTimer
(
    ASSEMBLY_CONTROL_RECORD *par,       /* (in)  ptr to Ass record     */
    long        *status,                /* (out) Function status.      */
    int         secs                    /* (in)  Seconds to run timer. */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt;     /* Internal control struct */
    struct callback *pCallback = pPriv->timer;  /* Timeout timer structure */
    int waitTime;                               /* Time to wait.           */
       
    DEBUG(DAR_MSG_MAX, "<%d> %s:assStartTimer: entry, secs=%d\n", secs );


    /*
     * Convert seconds to system ticks for EPICS timer use
     */

    waitTime = (int) (secs * vxTicksPerSecond );


    /*
     *  Start the EPICS watchdog timer identified in the timeout struct.
     *  Generate a system error message if the timer can not be started.
     */

    if(  wdStart(pCallback->wd_id, waitTime, (FUNCPTR)callbackRequest,
        (int)pCallback) == ERROR )
    {
        *status = DAR_E_TIMER_CAN;
        recGblRecordError (*status, par, __FILE__ ": start timeout failed");
    }

    return;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * assStopTimer
 *
 * INVOCATION:
 * status = assStopTimer (par, &status);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 * (<) status (long *) Function return status.
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Stop the command timeout timer.
 *
 * DESCRIPTION:
 * Calls the EPICS watchdog cancel function to stop the command
 * timeout timer.  This function is usually called when command
 * processing is finished and the command is complete.  See assStartTimer
 * for more information on how this timer is used.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *      assStartTimer
 *      initTimer
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

void    assStopTimer
(
    ASSEMBLY_CONTROL_RECORD   *par,         /* (in)  Ptr to ass. rec.    */
    long        *status                     /* (out) Function status.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt;     /* Internal control struct */
    struct callback *pCallback = pPriv->timer;  /* Timeout timer structure */
   

    /*
     * Cancel the EPICS watchdog timer associated with the command timeout
     * function.
     */

    if ( wdCancel( pCallback->wd_id ) == ERROR )
    {
        *status = DAR_E_TIMER_CAN;
        par->hlth = DAR_HLTH_BAD;
        MONITOR(RECORD_HTH);
        recGblRecordError (*status, par, __FILE__ ": stop timeout failed");
    }
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * busyReceivedFromDevice
 *
 * INVOCATION:
 * status = busyReceivedFromDevice (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 *
 * FUNCTION VALUE:
 * (long) processing return code.
 *
 * PURPOSE:
 * React when an attached deviceControl record starts or finishes a command.
 *
 * DESCRIPTION:
 * Invoke the device support code ackReceived function in response to a
 * device control signalling the start or completion of a command.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */


static long busyReceivedFromDevice
(
    ASSEMBLY_CONTROL_RECORD *par            /* (in)  Ptr to ass rec.    */
)
{
    ASSEMBLY_CONTROL_DSET *pdset;               /* Device support functions */
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt;     /* Internal control struct. */
    long    status = DAR_S_SUCCESS;             /* Function status.         */

    DEBUG(DAR_MSG_MAX, "<%d> %s:busyReceivedFromDevice: entry%c\n", ' ');

    pdset = (ASSEMBLY_CONTROL_DSET *) (par->dset);

    /*
     * Copy the timeout flag to the command timedout flag and then invoke
     * the device support ackReceived funtion to indicate that the last
     * deviceRecord command sent has either been acknowledged or completed.
     */
 
    pPriv->cmdTimedOut = pPriv->timeout;

    DEBUG(DAR_MSG_MAX, "<%d> %s:calling ackReceived%c\n", ' ');
    status = (*pdset->ackReceived) (par);
    DEBUG(DAR_MSG_MAX, "<%d> %s:ackReceived returns %ld\n", status);

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * completeCommand
 *
 * INVOCATION:
 * completeCommand (par, &status);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 * (<) status (long *) Function status returned.
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Perform the EPICS tidy-up tasks required at the end of record processing.
 *
 * DESCRIPTION:
 * This function is called each time the record is processed, just before
 * returning control to the EPICS system.  It updates the assembly status
 * flag fields, processes the output links and handles the raising of monitors
 * and alarms via the following algorithm:
 *
 *      IF cmdFinished flag is set to indicate command completion
 *      {
 *          Set status flag fields to reflect the results of processing
 *          Clear cmdFinished flag to acknowledge completion
 *      }
 *      Write all output links that were marked for updating during processing
 *      Raise alarm if the UDF field is set
 *      Raise monitors on fields that were marked as changed during processing
 *      IF post processing requested 
 *      {
 *          Trigger the EPICS forward link
 *          Update the record timestamp
 *      }
 *
 * Note that the handling of command completion changed during the 
 * implementation of this record.  Originaly the par->pp field was set
 * to request "post processing" (trigger the forward link and read timestamp)
 * after a comand since it may take many processing cycles of the record 
 * to fully process a command.  Later it was decided to do post processing
 * every time the record was processed in order to fool EPICS into allowing
 * passive processing of the record during a command.   The flag cmdFinished
 * is now used to indicate that this processing of the record completed the
 * command and that the record can now accept another command.   
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static void completeCommand
(
    ASSEMBLY_CONTROL_RECORD *par,           /* (in)  Ptr to ass rec.   */
    long    *status                         /* Function status.        */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struct.  */

    DEBUG(DAR_MSG_MAX, "<%d> %s:completeCommand: entry, pp=%d\n", par->pp);

    /*
     * If the command has been fully processed then update the assembly status
     * fields to indicate the current state of the assembly and return
     * to the idle state ready to accept a new command.
     */
 
    if ( pPriv->cmdFinished )
    {
        /*
         * A false cmdResponse means that the command failed so clear
         * the status flags to tell the world that we are no longer
         * certain of the state of our components.
         */

        if ( pPriv->cmdResponse == FALSE )
        {
            unsetFlags( par );
        } 

        /* 
         * Otherwise the command was successful so update the status
         * flags based on the type of command just finished.
         */

        else
        {
            setFlags( par );
        }


        /*
         * Clear the finished flag and return the command processing
         * state machine to the idle state ready for another command.
         */

        pPriv->cmdFinished = FALSE;
        *status = idleState( par, *status );
        DEBUG(DAR_MSG_MAX,"<%d> %s:completeCommand: Cmd finished, busy=%d\n",
              par->busy );
    }               


    /*
     * Write data to all marked output links, raise alarms on selected
     * fields and raise monitors on all marked data fields.
     */

    CHECKSTAT( ( *status = writeOutputLinks (par)) , return );
    alarm(par);
    monitor (par);


    /*
     * If the post processing flag has been set then process the forward
     * link and update the record time stamp.
     */

    if ( par->pp )
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:completeCommand: post processing%c\n", ' ');
        par->pp = FALSE;
        recGblFwdLink(par);   
        recGblGetTimeStamp(par);
    }
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * cvtDbAddr
 *
 * INVOCATION:
 * status = dbAddr (paddr);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (!) paddr  (struct dbAddr *)     Pointer to database address structure.
 *
 * FUNCTION VALUE:
 * (long) function success code
 *
 * PURPOSE:
 * Update the database address structure for variable-type fields
 *
 * DESCRIPTION:
 * Set the datatype and value of fields where the datatype can be specified.
 * The EPICS system will pass a pointer to the dbAddr structure for the
 * database field it wants the information for and this function will
 * fill in the type and location information if it a variable datatype
 * field via the following algorithm:
 *
 *      IF the field name has only one letter
 *      {
 *          Pre-load local pointers with the information for field A
 *      }
 *      ELSE IF the field name starts with the letter S
 *      {
 *          Pre-load local pointers with the information for field SIJ
 *      }
 *      ELSE IF the field name starts with the letter V
 *      {
 *          Pre-load local pointers with the information for field VALA
 *      }
 *      ELSE
 *      {
 *          Return without modifying dbAddr structure
 *      }
 *      FOR each of the 5 related fields in each set
 *      {
 *          IF the local pointer name matches the given field name
 *          {
 *              Copy the local pointer information into the dbAddr structure
 *              Exit the FOR loop
 *          }
 *          Increment local pointers to reference the next field in the set
 *      }
 *      
 * For this record these are fields A-E, SIJ-K, VALA-E.
 *
 * EXTERNAL VARIABLES:
 * fldnames, Ifldnames, SIfldnames, Ofldnames
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * Currently will only handle datatypes of STRING, DOUBLE and LONG.
 *-
 ************************************************************************
 */

static long cvtDbaddr
(
    struct dbAddr *paddr        /* pointer to dbAddr structure for field */
)
{
    int                 i;              /* generic index thing */
    ASSEMBLY_CONTROL_RECORD *par;       /* pointer to record structure */
    void                **ppfield;      /* pointer to field address */
    unsigned short      *ptype;         /* pointer to data type */
    struct fldDes       *pdesc;         /* pointer to field description */
    char                *name;          /* pointer to field name */
    const char          *p = NULL;      /* generic character pointer thing */


    /*
     * Recover the record, field description and field name information
     * from the dbAddr structure.
     */

    par = (ASSEMBLY_CONTROL_RECORD *)paddr->precord;
   
    DEBUG(DAR_MSG_MAX, "<%d> %s:cvtDbAddr: entry, paddr=%p\n", paddr );
   
    pdesc = (struct fldDes *)paddr->pfldDes;
    name = (char *)strtok((char *)pdesc->fldname, " ");
    p = name;

    DEBUG(DAR_MSG_MAX, "<%d> %s:cvtDbAddr: name=%s\n", name );

    if ( p == NULL )
    {
        DEBUG(DAR_MSG_FATAL, "<%d> %s:cvtDbAddr: Error, null name%c\n", ' ');
        return( DAR_E_NULL_NAME );
    }


    /*
     *  If the field name has only one letter, then must be one of the
     *  attribute fields (A-E) so point to par->a.
     */

    if( strlen(name) == 1 )
    {
        ppfield = &par->a;
        ptype = &par->fta;
        memcpy( fldnames, Ifldnames, DAR_ARG_MAX*(FLDNAME_SZ+1) );
        paddr->special = SPC_MOD;
    }


    /*
     * If the fild name starts with the letter S, then it must be one
     * of the sensor input fields (SIJ-SIN) so point to par->sij.
     */

    else if ( *p == 'S' )
    {
        ppfield = &par->sij;
        ptype = &par->ftsj;
        memcpy( fldnames, SIfldnames, DAR_ARG_MAX*(FLDNAME_SZ+1) );
    }


    /*
     * If the fild name starts with the letter V, then it must be one
     * of the value output fields (VALA-VALE) so point to par->vala.
     */

    else if ( *p == 'V' )
    {
        ppfield = &par->vala;
        ptype = &par->fta;
        memcpy( fldnames, Ofldnames, DAR_ARG_MAX*(FLDNAME_SZ+1) );
    }


    /*
     * Otherwise it is a field with a fixed data type so there is
     * nothing to do.   Exit here without modifying the dbAddr structure.
     */

    else
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:cvtDbAddr: BUS, ignoring.%c\n", ' ');
        return( DAR_S_SUCCESS );
    }


    /*
     *  Now start with the field in the local pointers and run through
     *  each of the field names in the set until you find a match.  If
     *  a match is found update the dbAddr structure with the local pointer
     *  information and exit.  Not finding a match simply means that
     *  the given field name was not what we thought it was so simply
     *  return without affecting the dbAddr structure.
     */

    for( i=0; i<DAR_ARG_MAX; ppfield++, i++, ptype++ )
    {
        if( !strcmp(name, fldnames[i]) )
        {
            paddr->pfield         = *ppfield;
            paddr->no_elements    = 1;
            paddr->field_type     = *ptype;
            paddr->dbr_field_type = *ptype;
            if( *ptype == DBF_STRING ) 
            {
                paddr->field_size = MAX_STRING_SIZE;
            }
            else if( *ptype == DBF_DOUBLE )
            {
                paddr->field_size = sizeof(double);
            }
            else if( *ptype == DBF_LONG )
            {
                paddr->field_size = sizeof(long);
            }
            DEBUG(DAR_MSG_MAX, "<%d> %s:cvtDbAddr: Found , name=%s\n", name );
            break;
        }
    }

    return( DAR_S_SUCCESS );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * executeState
 *
 * INVOCATION:
 * status = executeState (par, state, functPtr );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 * (>) state (int) New operating mode for the assembly.
 * (>) functPtr (DEVSUPFUN) Pointer to the device support structure function
 *                for the mode selected.
 *
 * FUNCTION VALUE:
 * (long) record processing success code.
 *
 * PURPOSE:
 * Call the given device support function.
 *
 * DESCRIPTION:
 * Start processing a new command by saving the new operating mode
 * and calling the appropriate device support mode function.   This 
 * function does not check to insure that it has been passed a valid mode 
 * function, it simply calls whatever function it was passed.....
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * This function assumes that the function it was asked to call
 * takes only one argument and that is a pointer to the record
 * structure.  No checking is done to insure that this is the
 * case so there is a possiblilty of crashing if the wrong function
 * pointer is supplied!!!!!
 *-
 ************************************************************************
 */

static long executeState
(
    ASSEMBLY_CONTROL_RECORD *par,       /* (in)  Ptr to ass. rec.       */
    int            state,               /* (in)  New operating mode.    */
    DEVSUPFUN        funcPtr            /* (in)  Ptr to dev. supp func. */
)
{
    long    status = DAR_S_SUCCESS;         /* Local status.            */

    DEBUG(DAR_MSG_MAX, "<%d> %s:executeState: entry%c\n", ' ');


    /*
     * If this is a new operating mode then update the assembly state
     * output status field.
     */

    if( par->asta != state )
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:executeState: entering state:%d\n", state);
        par->asta = state;
        MONITOR(RECORD_ASTA);
    }
   
   
    /*
     *  Call the given function which should start command processing.
     */
    
    if ( funcPtr )
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:executeState: calling function%c\n", ' ');
        status = (*funcPtr ) (par);
        DEBUG(DAR_MSG_MAX, "<%d> %s:executeState: function returns %ld\n",
              status);
    }
    else
    {
        DEBUG(DAR_MSG_FATAL, "<%d> %s:executeState: no function:state=%d\n",
              state );
        status = DAR_E_DEV_PTR;
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * getAlarmDouble
 *
 * INVOCATION:
 * status = getAlarmDouble (paddr, pad);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) paddr  (struct dbAddr *)     Pointer to database address structure.
 * (!) pad (struct dbr_alDouble *)  Alarm double stuff is returned here.
 *
 * FUNCTION VALUE:
 * (long) record processing success code (DAR_S_SUCCESS).
 *
 * PURPOSE:
 * Return alarm double information for display
 *
 * DESCRIPTION:
 * Simply return generic alarm double information for all fields.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * Does not check status of recGbl call, always returns success.
 *-
 ************************************************************************
 */

static long getAlarmDouble
(
    struct dbAddr       *paddr,     /* (in)  Database field.    */
    struct dbr_alDouble *pad        /* (out) Alarm info.        */
)
{
    recGblGetAlarmDouble(paddr, pad);

    return( DAR_S_SUCCESS );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * assGetBusResponse
 *
 * INVOCATION:
 * assGetBusResponse (par, &bus1, &bus2, &bus3, &bus4, &bus5 );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 * (<) bus1 (int *) BUS1 field has changed flag.
 * (<) bus2 (int *) BUS2 field has changed flag.
 * (<) bus3 (int *) BUS3 field has changed flag.
 * (<) bus4 (int *) BUS4 field has changed flag.
 * (<) bus5 (int *) BUS5 field has changed flag.
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Return flags to indicate which device control record BUSY fields have changed
 *
 * DESCRIPTION:
 * Copy the internal busy field changed flags into the given locations.
 * This function is calle by the device support code in response to an
 * ackReceived call in order to determine which Device Control Record
 * BUSY fields changed since this function was last called.   It
 * returns a TRUE (changed) or FALSE (unchanged) flag for each field.
 * After returning the flags it resets them all to FALSE so that only those
 * which change after this will be identified.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

void assGetBusResponse
(
    ASSEMBLY_CONTROL_RECORD *par,   /* (in)  Ptr to ass. rec.    */
    int        *bus1,               /* (out) save BUS1 flag here.    */
    int        *bus2,               /* (out) save BUS2 flag here.    */
    int        *bus3,               /* (out) save BUS3 flag here.    */
    int        *bus4,               /* (out) save BUS4 flag here.    */
    int        *bus5                /* (out) save BUS5 flag here.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struct.    */

    DEBUG(DAR_MSG_MAX, "<%d> %s:assGetBusResponse: entry%c\n", ' ');
 

    /*
     * Save the current state of the flags
     */

    *bus1 = pPriv->bus1;
    *bus2 = pPriv->bus2;
    *bus3 = pPriv->bus3;
    *bus4 = pPriv->bus4;
    *bus5 = pPriv->bus5;


    /*
     * And then reset them to trap new changes
     */

    pPriv->bus1 = FALSE;
    pPriv->bus2 = FALSE;
    pPriv->bus3 = FALSE;
    pPriv->bus4 = FALSE;
    pPriv->bus5 = FALSE;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * getControlDouble
 *
 * INVOCATION:
 * status = getControlDouble (paddr, pcd);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) paddr  (struct dbAddr *) Pointer to database address structure.
 * (!) pcd (dbr_ctrlDouble *)   Control double stuff is returned here.
 *
 * FUNCTION VALUE:
 * (long) record processing success code.
 *
 * PURPOSE:
 * Update the control double information for display
 *
 * DESCRIPTION:
 * Return generic control double information for all fields.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * Does not check status of recGbl call, always returns success.
 *-
 ************************************************************************
 */

static long getControlDouble
(
    struct dbAddr     *paddr,           /* (in)  Database addr str.    */
    struct dbr_ctrlDouble *pcd          /* (out) Control double info.  */
)
{
    recGblGetControlDouble(paddr, pcd);
   
    return( DAR_S_SUCCESS );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * getGraphicsDouble
 *
 * INVOCATION:
 * status = getGraphicsDouble (paddr, pgd);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) paddr  (struct dbAddr *) Pointer to database address structure.
 * (!) pcd (dbr_grDouble *)     Graphics double stuff is returned here.
 *
 * FUNCTION VALUE:
 * (long) record processing success code.
 *
 * PURPOSE:
 * Update the graphics double information for display
 *
 * DESCRIPTION:
 * Return generic graphics double information for all fields.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * Does not check status of recGbl call, always returns success.
 *-
 ************************************************************************
 */

static long getGraphicDouble
(
    struct dbAddr       *paddr,     /* (in)  Ptr to db addr struct. */
    struct dbr_grDouble *pgd        /* (out) Graphics double stuff goes here.*/
)
{
    recGblGetGraphicDouble(paddr, pgd);

    return( DAR_S_SUCCESS );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * getPrecision
 *
 * INVOCATION:
 * status = getPrecision (paddr, precision);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) paddr  (struct dbAddr *) Pointer to database address structure.
 * (<) precision (long *)       Precision is returned here
 *
 * FUNCTION VALUE:
 * (long) record processing success code.
 *
 * PURPOSE:
 * Return the record display precision value for selected fields
 *
 * DESCRIPTION:
 * Return the value of the PREC field to set the display precision
 * for the following fields:  VAL, A->E, VALA->VALE AND SIJ->SIN. 
 * For all other fields return the generic precision value.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * Does not check status of recGbl call, always returns success.
 * Returns a single precision value for all fields.
 *-
 ************************************************************************
 */

static long getPrecision
(
    struct dbAddr *paddr,           /* (in)  Field to be displayed  */
    long     *precision             /* (out) Display precision.    */
)
{
    ASSEMBLY_CONTROL_RECORD *par;    /* Ptr to ass. rec    */

    par = (ASSEMBLY_CONTROL_RECORD *) paddr->precord;

    DEBUG(DAR_MSG_MAX, 
	  "<%d> %s:getPrecision: entry%c\n", ' ');

    /*
     *  Explicitly set the precision for the following fields.
     */

    *precision = par->prec;

    if ( paddr->pfield == (void *) par->a   ||
         paddr->pfield == (void *) par->b   ||
         paddr->pfield == (void *) par->c   ||
         paddr->pfield == (void *) par->d   ||
         paddr->pfield == (void *) par->e   ||
         paddr->pfield == (void *) par->vala   ||
         paddr->pfield == (void *) par->valb   ||
         paddr->pfield == (void *) par->valc   ||
         paddr->pfield == (void *) par->vald   ||
         paddr->pfield == (void *) par->vale   ||
         paddr->pfield == (void *) par->sij ||
         paddr->pfield == (void *) par->sik ||
         paddr->pfield == (void *) par->sil ||
         paddr->pfield == (void *) par->sim ||
         paddr->pfield == (void *) par->sin )
    {
        if ( paddr->field_type == DBR_DOUBLE ||
             paddr->field_type == DBR_FLOAT )
        {
            return (DAR_S_SUCCESS);
        }
    }

    /*
     *  Otherwise return global precision.
     */ 

    recGblGetPrec(paddr, precision);

    return ( DAR_S_SUCCESS );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * getValue
 *
 * INVOCATION:
 * status = getValue (par, pvdes);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)   EPICS database struct for this record
 * (!) pvdes (struct valueDes *)          Value field stuff goes here
 *
 * FUNCTION VALUE:
 * (long) record processing success code.
 *
 * PURPOSE:
 * Return EPICS value (VAL) field information
 *
 * DESCRIPTION:
 * Return EPICS value (VAL) field information.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * None.
 *
 * DEFICIENCIES:
 * Assumes that the value field is of type LONG.
 *-
 ************************************************************************
 */

static long getValue
(
    ASSEMBLY_CONTROL_RECORD*par,        /* (in)  Assembly record struct.  */
    struct valueDes     *pvdes          /* (out) Value information.    */
)
{
    DEBUG(DAR_MSG_MAX, "<%d> %s:getValue: entry%c\n", ' ');

    /*
     * Fill the value description structure with the value field info.
     */

    pvdes->no_elements = 1;
    pvdes->pvalue = (void *)( &par->val );
    pvdes->field_type = DBF_LONG;

    return ( DAR_S_SUCCESS );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * idleState
 *
 * INVOCATION:
 * status = idleState (par, status);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 * (<) status (long)                    Function status to be returned.
 *
 * FUNCTION VALUE:
 * (long) record processing success code, DAR_S_SUCCESS, or passed in
 * failure code.
 *
 * PURPOSE:
 * Handle the transition from command execution to idle states.
 *
 * DESCRIPTION:
 * Set the ASTA field to indicate that the record is now in the idle
 * state ready to accept another command.  Set the BUSY field to indicate
 * that command processing has finished and indicate if the previous
 * command was successful or not.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long idleState
(
    ASSEMBLY_CONTROL_RECORD *par,       /* (in)  Ptr to ass. rec.    */
    long        status                  /* (in)  Status to be returned.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struct */

    DEBUG(DAR_MSG_MAX, "<%d> %s:idleState: entry%c\n", ' ');

    /*
     * Insure that the transition to idle is reflected in the status field.
     */

    if (par->asta != DAR_STATE_IDLE)
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:idleState:entering IDLE state, busy=%d\n",
        par->busy );
        par->asta = DAR_STATE_IDLE; 
        MONITOR(RECORD_ASTA);
    }

    /*
     *  If the record was busy processing a command set the BUSY field
     *  to indicate if the command was successful or not.
     */

    if ( par->busy == DAR_BUSY_BUSY )
    {
        par->busy = (pPriv->cmdResponse == TRUE) ? DAR_BUSY_IDLE  :
            DAR_BUSY_ERROR;
        DEBUG(DAR_MSG_MAX, "<%d> %s:idleState: changin busy, busy=%d\n",
        par->busy );
        MONITOR(RECORD_BUSY);
    }


    /* 
     *  And then hand back the status argument (Bizarre!)
     */
      
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * initDevLinks
 *
 * INVOCATION:
 * status = initDevLinks (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * (long) record processing success code.
 *
 * PURPOSE:
 * Initialize deviceControl record related database links.
 *
 * DESCRIPTION:
 * Initialize input and output links to the deviceControl record and determine
 * number of devices attached to the record.  Since these links always
 * come in sets of consecutive links the first link in the set is pointed
 * to and the function linkSet is called to do the actual work.
 *
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * linkSet.
 *
 * DEFICIENCIES:
 * This function (via linkSet) assumes that the device records are
 * attached to consecutive sets of links (1,2,3,4 not 1,2,4,5).  The
 * number of DIR links found is returned as the total number of records
 * connected.
 *-
 ************************************************************************
 */

static long initDevLinks
(
    ASSEMBLY_CONTROL_RECORD    *par    /* (in)  Ptr to ass. rec.    */
)
{
    int     numDevs = 0;                /* Aattached record counter */
    struct  link *plink;                /* Generic link pointer.    */
    long    status = DAR_S_SUCCESS;     /* Function return status.  */

    DEBUG(DAR_MSG_MAX, "<%d> %s:initDevLinks: entry%c\n", ' ');


    /*
     *  Initialize device control record accept/reject field reading links.
     */

    plink = &par->ack1;
    CHECKSTAT( (status = linkSet( par, plink, 1, "ACK", &numDevs )),
        return(status) );
    DEBUG(DAR_MSG_MAX, "<%d> %s:initDevLinks: after ACK link, numDevs=%d\n",
        numDevs );

  
    /*
     *  Initialize device control record mode field writing links.
     */

    plink = &par->mod1;
    CHECKSTAT( ( status = linkSet( par, plink, 0, "MOD", &numDevs ) ),
        return(status) );
    DEBUG(DAR_MSG_MAX, "<%d> %s:initDevLinks: after MOD link, numDevs=%d\n",
        numDevs );


    /*
     *  Initialize device control record velocity field writing links.
     */

    plink = &par->vel1;
    CHECKSTAT( ( status = linkSet( par, plink, 0, "VEL", &numDevs ) ),
        return(status) );
    DEBUG(DAR_MSG_MAX, "<%d> %s:initDevLinks: after VEL link, numDevs=%d\n",
        numDevs );


    /*
     *  Initialize device control record target position field writing links.
     */

    plink = &par->pos1;
    CHECKSTAT( ( status = linkSet( par, plink, 0, "POS", &numDevs ) ),
        return(status) );
    DEBUG(DAR_MSG_MAX, "<%d> %s:initDevLinks: after POS link, numDevs=%d\n",
        numDevs );


    /*
     *  Initialize device control record directive field writing links.
     */

    plink = &par->odr1;
    CHECKSTAT( (status = linkSet( par, plink, 0, "ODR", &numDevs ) ),
        return(status) );
    DEBUG(DAR_MSG_MAX, "<%d> %s:initDevLinks: after ODR link, numDevs=%d\n",
        numDevs );

    /*
     *  Set the system "number of connected devices" variable to relfect
     *  the number of directive links found.
     */

    par->nmdv = numDevs;
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * initFields
 *
 * INVOCATION:
 * status = initFields (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * (long) record processing code.
 *
 * PURPOSE:
 * Create database fields whose datatype is configurable on startup.
 *
 * DESCRIPTION:
 * Create database fields that have a user configurable type.   For this
 * record these are A->E, VALA->VALE and SIJ->SIN.   Since these fields
 * always come in blocks of consecutive fields this function simply sets
 * a pointer to the first field in the block and lets the function setFields
 * do the actual work of allocating space for the fields.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * setFields.
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long initFields
(
    ASSEMBLY_CONTROL_RECORD    *par    /* (in)  Ptr to ass. rec.    */
)
{
    long status = DAR_S_SUCCESS;

    DEBUG(DAR_MSG_MAX, "<%d> %s:initFields: entry%c\n", ' ');


    /*
     *  Create attribute input fields.
     */

    CHECKSTAT( (status = setFields( par, &par->a, &par->fta)),
        return(status )) ;
    DEBUG(DAR_MSG_MAX, "<%d> %s:initFields: for attributes.%c\n", ' ');


    /*
     *  Create sensor input fields.
     */

    CHECKSTAT( (status = setFields( par, &par->sij, &par->ftsj)),
        return(status )) ;
    DEBUG(DAR_MSG_MAX, "<%d> %s:initFields: for SI's .%c\n", ' ');


    /*
     *  Create value output fields.
     */

    CHECKSTAT( (status = setFields( par, &par->vala, &par->fta)),
        return(status )) ;
    DEBUG(DAR_MSG_MAX, "<%d> %s:initFields: for VAL's .%c\n", ' ');


    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * initLinks
 *
 * INVOCATION:
 * status = initLinks (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * (long) record processing success code.
 *
 * PURPOSE:
 * Initialize all EPICS input and output links
 *
 * DESCRIPTION:
 * Initialize all EPICS database links for this record.  Each link is
 * checked to see if it is connected to something before being initialized.
 * All links except the error message output link are set up as fast links.
 * Because the message output link writes string values it is setup as a
 * normal output link.
 *
 * Links that are connected to the device control records are initialized
 * via the initDevLinks function. 
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * initDevLinks.
 *
 * DEFICIENCIES:
 * All sensor output links are created as type ENUM.
 *-
 ************************************************************************
 */

static long initLinks
(
    ASSEMBLY_CONTROL_RECORD    *par    /* (in)  Ptr to ass. rec.    */
)
{
    struct  link *plink;                /* Generic link pointer.    */
    long status = DAR_S_SUCCESS;

    DEBUG(DAR_MSG_MAX, "<%d> %s:initLinks: entry%c\n", ' ');


    /*
     * Initialize the debug level writing link
     */

    if (par->dbgl.type != CONSTANT)
    {
        CHECKFNSTAT( recGblInitFastOutLink (&(par->dbgl),(void *) par,
                     DBR_ENUM, "DBUG"), status, return( status ) );
    }


    /*
     * Initialize the simulation level writing link
     */

    if (par->siml.type != CONSTANT)
    {
        CHECKFNSTAT( recGblInitFastOutLink (&(par->siml),(void *) par,
                     DBR_ENUM, "SIMM"), status, return( status ) );
    }


    /*
     * Initialize the command state writing link
     */

    if (par->bsyl.type != CONSTANT)
    {
        CHECKFNSTAT( recGblInitFastOutLink (&(par->bsyl),(void *) par,
                     DBR_ENUM, "BUSY"), status, return( status ) );
    }


    /*
     * Initialize the error message writing link
     */ 

    if( par->msgl.type == PV_LINK )
    {
        CHECKFNSTAT( dbCaAddOutlink (&(par->msgl),(void *) par, "MESS"),
                     status, return( status ) );
    }


    /*
     *  Initialize sensor value writing links.
     */ 

    plink = &par->sor;

    CHECKFNSTAT( recGblInitFastOutLink ( (plink), (void *) par,
                 DBR_ENUM, "SOR" ), status, return(status) );
    CHECKFNSTAT( recGblInitFastOutLink ( (plink), (void *) par,
                 DBR_ENUM, "SOS" ), status, return(status) );
    CHECKFNSTAT( recGblInitFastOutLink ( (plink), (void *) par,
                 DBR_ENUM, "SOT" ), status, return(status) );
    CHECKFNSTAT( recGblInitFastOutLink ( (plink), (void *) par,
                 DBR_ENUM, "SOU" ), status, return(status) );
    CHECKFNSTAT( recGblInitFastOutLink ( (plink), (void *) par,
                 DBR_ENUM, "SOV" ), status, return(status) );

    /*
     *  Initialize the links attached to device control records.
     */

    status = initDevLinks( par );

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * initRecord
 *
 * INVOCATION:
 * initRecord (par, pass);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 * (>) pass (int)                       Pass number
 *
 * FUNCTION VALUE:
 * (long) initialization pass success code.
 *
 * PURPOSE:
 * Initialize instance of assemblyControl record and device support
 *
 * DESCRIPTION:
 * This function is called directly by the EPICS system during the system
 * initialization phase.  It is called twice, once to set itself up to
 * be linked to other records and once when all records are ready to be
 * linked together.
 * This function creates an instance of the assembly record with the
 * given type of device support via the following algorithm:
 *
 *      If this is the first pass
 *      {
 *          Initialize database fields that have a configurable datatype.
 *          Return immediately.
 *      }
 *      
 *      If device support definition struct or critical functions are missing
 *      {
 *          Generate an error messge.
 *          Return immediately.
 *      }
 *
 *      Initialize all database links to other records.
 *      Create and initialize an internal control structure to save 
 *          information specific to this instance of the record.
 *      Initialize the two watchdog timers necessary for record operation.
 *      Initialize the device support code.
 *      Set the initial values of the output status fields      
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
 * Clears the interlock field without looking at it... interlocks will be lost.
 * Sets the value undefined flag... which is never cleared!
 *-
 ************************************************************************
 */

static long initRecord
(
    ASSEMBLY_CONTROL_RECORD   *par,         /* (in)  Ptr to ass. rec.   */
    int pass                                /* (in)  Number of pass.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = NULL;      /* Internal control struct.  */
    ASSEMBLY_CONTROL_DSET *pdset;           /* Device support interface  */
    long     status = DAR_S_SUCCESS;        /* Return function status.   */

    DEBUG(DAR_MSG_MAX, "<%d> %s:initRecord: entry pass = %d\n", pass);


    /*
     * Set up fields with user-defined types on the first pass.
     */

    if (pass == 0)
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:initRecord: entry, first pass%c\n", ' ');
        status = initFields( par );
        return( status );
    }


    /*
     *  Check that we have a device-support interface function entry table.
     */

    if (!(pdset = (ASSEMBLY_CONTROL_DSET *) (par->dset)))
    {
        par->hlth = DAR_HLTH_BAD;
        MONITOR(RECORD_HTH);
        recGblRecordError(S_dev_noDSET, (void *) par, 
                          "assemblyControl: initRecord");
        return (S_dev_noDSET);
    }


    /*
     * Insure that device support code is supplying the required functions.
     */

    if ((pdset->number < 13 ) ||
        (pdset->initDeviceSupport == NULL) ||
        (pdset->checkAttributes == NULL) ||
        (pdset->stopDirective == NULL) ||
        (pdset->initMode == NULL) ||
        (pdset->moveMode == NULL) ||
        (pdset->ackReceived == NULL) ||
        (pdset->processFault == NULL))
    {
        par->hlth = DAR_HLTH_BAD;
        MONITOR(RECORD_HTH);
        recGblRecordError(S_dev_missingSup, (void *) par,
                          "assemblyControl: initRecord");
        return (S_dev_missingSup);
    }


    /*
     * Initialize all database links to other records
     */

    CHECKSTAT( (status = initLinks (par)), return(status) );


    /*
     * Create an internal control structure for this instance of the record and
     * store the structure pointer in this record's device private field.
     */

    pPriv = malloc (sizeof(ASS_CONTROL_PRIVATE));
    if ( pPriv == NULL )
    {
        status = DAR_E_MALLOC;
        par->hlth = DAR_HLTH_BAD;
        MONITOR(RECORD_HTH);
        recGblRecordError (status, par, __FILE__ ":no room for private str.");
        return( status );
    }
    par->dpvt = (void *) pPriv;


    /*
     *  Initialize the private structure created above.
     */

    pPriv->bus1 = 0;                    /* clear field change trigger flag */
    pPriv->bus2 = 0;                    /* clear field change trigger flag */
    pPriv->bus3 = 0;                    /* clear field change trigger flag */
    pPriv->bus4 = 0;                    /* clear field change trigger flag */
    pPriv->bus5 = 0;                    /* clear field change trigger flag */
    pPriv->busChange = FALSE;           /* clear any field has changed flag */
    pPriv->cmdFinished = FALSE;         /* clear end-of-command flag */
    pPriv->cmdResponse = TRUE;          /* set command was successful flag */
    pPriv->cmdTimedOut = FALSE;         /* clear command has timedout flag */
    pPriv->debug = DAR_DBUG_NONE;       /* start in no debug mode */
    pPriv->fault = FALSE;               /* clear fault line detected flag */
    pPriv->keepIndex = FALSE;           /* enable status field updating */
    pPriv->lastCommand = DAR_STOP_CMD;  /* pretend last directive was STOP */
    pPriv->currCommand = DAR_STOP_CMD;  /* pretend this directive is STOP */
    pPriv->pPrivate = NULL;             /* no device support struct yet */
    pPriv->simulation = DAR_SIM_NONE;   /* start in no simulation mode */
    pPriv->timeout = FALSE;             /* clear the timeout detected flag */
    pPriv->timer = NULL;                /* no command timeout callback yet */
    pPriv->special = NULL;              /* no special callback yet */
    pPriv->magic = DAR_MAGIC;           /* load magic value */



    /*
     * Create the two watchdog callback timers, one to force processing
     * on command timeout and one to force processing when a passive-process
     * field is accessed.
     */

    CHECKSTAT( ( status = initTimer (par)) , return(status) );


    /*
     * Set up device support via the initialization function
     */

    DEBUG(DAR_MSG_MAX, "<%d> %s:calling initDeviceSupport%c\n", ' ');
    status = (*pdset->initDeviceSupport) (par);
    DEBUG(DAR_MSG_MAX, "<%d> %s:initDeviceSupport returns %ld\n", status);
    CHECKSTAT( status, return(status) );


    /*
     * Set initial values for internal information fields
     */
    
    (void) strcpy( par->vers, VERSION );    /* publish the code version */
    par->udf = TRUE;            /* set value is undefined field ??? */
    par->mmap = 0;              /* clear monitor trigger flags */
    par->pp = FALSE;            /* clear post processing flag */
    par->ilck = 0;              /* clear the interlock field??? */

   
    /*
     *  Set initial values for the record status output fields.   Health
     *  is initially set to warning since the devices need to be indexed
     *  before the assembly is ready for use.
     */
    
    par->asta = DAR_STATE_STARTING;
    par->busy = DAR_BUSY_IDLE;
    *par->mess = '\0';
    par->val  = DAR_ACK_VAL_ACCEPT;
    par->hlth = DAR_HLTH_WARNING;
    par->indx = 0;
    par->init = 0;
    par->park = 0;
    par->mark = 0;
   
    MONITOR(RECORD_MESS);		/* Added by CJT */
    MONITOR(RECORD_ASTA);
    MONITOR(RECORD_BUSY);
    MONITOR(RECORD_VAL);
    MONITOR(RECORD_HTH);
    MONITOR(RECORD_INIT);

    /*
     *  Place the record in the idle state ready to accept commands.
     */

    status = idleState( par, status );
    par->pp = FALSE;

    DEBUG(DAR_MSG_MAX, "<%d> %s:initRecord: exit, status = %ld\n", status );
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * initTimer
 *
 * INVOCATION:
 * status = initTimer (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * (long) record processing code.
 *
 * PURPOSE:
 * Create and initialize the timeout and special watchdog timers
 *
 * DESCRIPTION:
 * This record must process itself if a passive-process field
 * has been written to or if a command takes too long to process.
 * This is done via two callback tasks invoked by the EPICS watchdog
 * function.  This function creates the two watchdog timers and
 * identifies the callback functions to be called by each via the
 * followiong algorithm:
 *
 *      Create command timeout callback structure.
 *      Save ptr to it in the local control struct for this record.
 *      Register the callback with the EPICS callback system.
 *      Create an EPICS watchdog timer for the command timeout.
 *      
 *      Create a special timeout callback structure.
 *      Save ptr to it in the local control struct for this record.
 *      Register the callback with the EPICS callback system.
 *      Create an EPICS watchdog timer for the special timeout.
 *      
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * startTimer, stopTimer
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long initTimer
(
    ASSEMBLY_CONTROL_RECORD    *par    /* (in)  Ptr to ass. rec.    */
)
{
    struct callback *pcallback = NULL;       /* Callback structure ptr.    */
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt;  /* Internal control struct.   */
    long status = DAR_S_SUCCESS;             /* Function return status.    */

    DEBUG(DAR_MSG_MAX, "<%d> %s:initTimer: entry%c\n", ' ');


    /*
     *  Create a command timeout timer callback structure and save it in
     *  the internal control structure for this record.
     */

    pcallback = (struct callback *)(calloc(1,sizeof(struct callback)));
    if ( pcallback == NULL )
    {
        DEBUG(DAR_MSG_FATAL, "<%d> %s:initTimer: no room for timer callback structure%c\n", ' ');
        status = DAR_E_MALLOC;
        par->hlth = DAR_HLTH_BAD;
        MONITOR(RECORD_HTH);
        return ( status );
    }

    pPriv->timer = (void *)pcallback;


    /*
     *  Register the command timeout callback with the EPICS callback system
     *  and then create a watchdog timer to invoke it via the EPICS
     *  watchdog tasking system.
     */

    callbackSetCallback( timerCallback, &pcallback->callback );
    pcallback->precord = (struct dbCommon *)par;

    if ( ( pcallback->wd_id = wdCreate() ) == NULL )
    {
        status = DAR_E_MALLOC;
        par->hlth = DAR_HLTH_BAD;
        MONITOR(RECORD_HTH);
        recGblRecordError (status, par, __FILE__ ":no room for watchdog timer");
        return( status );
    }

    callbackSetPriority( priorityLow, &pcallback->callback );
    DEBUG(DAR_MSG_MAX, "<%d> %s:After setting watchdog%c\n", ' ');


    /*
     *  Create a special callback structure and save it in the internal
     *  control structure.
     */

    pcallback = (struct callback *)(calloc(1,sizeof(struct callback)));
    if ( pcallback == NULL )
    {
        DEBUG(DAR_MSG_FATAL, "<%d> %s:initTimer: no room for special callback structure%c\n", ' ');
        status = DAR_E_MALLOC;
        par->hlth = DAR_HLTH_BAD;
        MONITOR(RECORD_HTH);
        return ( status );
    }

    pPriv->special = (void *)pcallback;


    /*
     *  Register the special callback with the EPICS callback system
     *  and then create a watchdog timer to invoke it via the EPICS
     *  watchdog tasking system.
     */

    callbackSetCallback( specialCallback, &pcallback->callback );
    pcallback->precord = (struct dbCommon *)par;

    if ( ( pcallback->wd_id = wdCreate() ) == NULL )
    {
        status = DAR_E_MALLOC;
        par->hlth = DAR_HLTH_BAD;
        MONITOR(RECORD_HTH);
        recGblRecordError (status, par, __FILE__ ":no room for watchdog timer");
        return( status );
    }

    callbackSetPriority( priorityLow, &pcallback->callback );
    DEBUG(DAR_MSG_MAX, "<%d> %s:After setting special watchdog%c\n", ' ');

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * linkSet
 *
 * INVOCATION:
 * status = linkSet( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)  EPICS database struct for this record
 * (>) plink (struct link)  Ptr to start of links.
 * (>) inLink (int )  If set indicates its an input link.
 * (>) prefix (char*)  Record name prefix.
 * (<) numDeves (int *)  Number of attached devices.
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Initialize blocks of related input or output database links
 *
 * DESCRIPTION: 
 * This record assigns attribute, value, sensor and Device Control Record
 * communications links in consecutive sets of DAR_ARG_MAX.   This function
 * is passed the information for the first link in a set and it then
 * proceeds to initialize each link in the set that has a database
 * connection to it via the following algorithm:
 *
 *      For each field in the set
 *      {
 *          If a database link has been made to this field
 *          {
 *              If it is an input link
 *              {
 *                  Initialize it as a fast input link.
 *              }
 *              Else
 *              {
 *                  Initialize it as a fast output link.
 *              }
 *          Increment the number of links found
 *          }
 *      }
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * None.
 *
 * DEFICIENCIES:
 * The assembly record assumes that links are always attached consecutively.
 * If the there are gaps in the attachment of links (i.e. A,B,E rather 
 * than A,B,C) then record processing will fail.
 *-
 ************************************************************************
 */

static long linkSet
(
    ASSEMBLY_CONTROL_RECORD *par,   /* (in)  Assembly record struc.     */
    struct link  *plink,            /* (in)  Pointer to first link.     */
    int inLink,                     /* (in)  T = inputs F = outputs.    */
    char *prefix,                   /* (in)  Rec. name prefix.          */
    int  *numDevs                   /* (out) Num of links connected.    */
)
{
    int i;                         /* Field counter.    */
    char fldName[FLDNAME_SZ+1];    /* Field name.       */
    long status = DAR_S_SUCCESS;   /* Function status.  */


    DEBUG(DAR_MSG_MAX, "<%d> %s:linkSet: entry%c\n", ' ');

    *numDevs = 0;
      

    /*
     * Process each link in turn starting with the given one
     */
 
    for( i = 0 ; i < DAR_ARG_MAX; i++, plink++ )
    {   
        /*
         * If there is a link connected to this field (CONSTANT means not
         * connected) then intialize as either a fast input or fast output
         * link as instructed by the inLink flag.
         */

        if( (*plink).type != CONSTANT )
        {
            sprintf (fldName, "%s%d", prefix, i+1);
            DEBUG(DAR_MSG_MAX, "<%d> %s:linkSet: initialized %s\n", fldName );
            if ( inLink )
            {
                CHECKFNSTAT( recGblInitFastInLink ( (plink), (void *) par,
                DBR_ENUM, fldName ), status, break );
            }
            else
            {
                CHECKFNSTAT( recGblInitFastOutLink ( (plink), (void *) par,
                DBR_ENUM, fldName ), status, break );
            }
        *numDevs+=1;
        }

        /*
         *  If it is not connected just note the fact.
         */

        else
        {
            DEBUG(DAR_MSG_MAX, "<%d> %s:linkSet:Link %s unattached\n",fldName);
        }
    }

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * monitor
 *
 * INVOCATION:
 * monitor (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Raise monitors on selected EPICS record fields
 *
 * DESCRIPTION:
 * Raise monitors on those fields that were marked as changed during
 * record processing.  These filds were marked by setting the associated
 * bit in the monitor bitmask field via the MONITOR macro.  The following
 * algorithm is used:
 *
 *      Set system monitor masking field by resetting the alarms.
 *      Test each output field monitor bit and raise a monitor if it is set.
 *      Test each input field monitor bit and raise a monitor if it is set.
 *      Clear all of the monitor bits to indicate monitors have been raised.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * This function resets all alarms.   If it is called after the function
 * alarm() then it will undo any alarms generated!
 *-
 ************************************************************************
 */

static void monitor
(
    ASSEMBLY_CONTROL_RECORD *par            /* (in)  Ptr to ass. rec.    */
)
{
    unsigned short  monitorMask;    /* System monitor mask.        */
   
    DEBUG(DAR_MSG_MAX, "<%d> %s:monitor: entry%c\n", ' ');
    
    monitorMask = recGblResetAlarms(par);


    /*
     *  Post changes on input fields
     */

    if (MONITORED(RECORD_FLT))  db_post_events(par, &par->ilck, DBE_VALUE);
    if (MONITORED(RECORD_DIR))  db_post_events(par, &par->dir, DBE_VALUE);
    if (MONITORED(RECORD_MODE)) db_post_events(par, &par->mode, DBE_VALUE);
    if (MONITORED(RECORD_DBUG)) db_post_events(par, &par->dbug, DBE_VALUE);
    if (MONITORED(RECORD_SIMM)) db_post_events(par, &par->simm, DBE_VALUE);


    /*
     *  Post changes on output fields, ensuring that VAL and BUSY are last.
     */

    if (MONITORED(RECORD_MESS)) db_post_events(par, &par->mess, DBE_VALUE);
    if (MONITORED(RECORD_ASTA)) db_post_events(par, &par->asta, DBE_VALUE);
    if (MONITORED(RECORD_MARK)) db_post_events(par, &par->mark, DBE_VALUE);
    if (MONITORED(RECORD_INIT)) db_post_events(par, &par->init, DBE_VALUE);
    if (MONITORED(RECORD_INDX)) db_post_events(par, &par->indx, DBE_VALUE);
    if (MONITORED(RECORD_PARK)) db_post_events(par, &par->park, DBE_VALUE);
    if (MONITORED(RECORD_HTH))  db_post_events(par, &par->hlth, DBE_VALUE);
    if (MONITORED(RECORD_VAL))  db_post_events(par, &par->val,  DBE_VALUE);
    if (MONITORED(RECORD_BUSY)) db_post_events(par, &par->busy, DBE_VALUE);

    UNMONITOR_ALL;

    return;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * process
 *
 * INVOCATION:
 * status = process (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record.
 *
 * FUNCTION VALUE:
 * (long) record processing success code.
 *
 * PURPOSE:
 * Process the assemblyControl record
 *
 * DESCRIPTION:
 * Process is the primary entry point from the EPICS system.   It implements
 * the assembly record itself and is called for one of the following reasons:
 *
 * 1) A hardware interlock (fault) was detected.
 * 2) The command being executed took too long to complete.
 * 3) An attached device control record started or finished a command.
 * 4) A new directive was issued to the record.
 *
 * The record is processed using the following algorithm:
 *
 *      If the record is already being processed
 *      {
 *          Return immediately.
 *      }
 *
 *      If a fault has been detected
 *      {
 *          Call device support to abort the command.
 *      }
 *
 *      Else if the command has timed out
 *      {
 *          If this is a delay (see below) to keep the busy field visible
 *          {
 *              Indicate that the command has finished processing.
 *          }
 *
 *          Else this is a real timeout so
 *          {
 *              Call device support to abort the command.
 *          }
 *      }
 *  
 *      Else if there has been a change in device control record busy state
 *      {
 *          Call device support which will react to this change.
 *      }
 *
 *      Else it must be a new directive so
 *      {
 *          Call the record directive processing function.
 *
 *          If this was a preset directive or the directive was rejected
 *          {
 *              Return here without affecting any command in progress.
 *          }
 *
 *          If the command was completed during directive processing
 *          {
 *              Set a flag to keep the BUSY field visible (see above)
 *              Start a command timeout to re-process the record after 1 sec.
 *          }
 *      }
 *
 *      Update any status fields affected by the processing.
 *
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long process
(
    ASSEMBLY_CONTROL_RECORD    *par    /* (in)  Assembly record ptr.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struct.   */
    long    status = DAR_S_SUCCESS;         /* Returned function status.  */
    ASSEMBLY_CONTROL_DSET *pdset;           /* Device support function list */

    DEBUG(DAR_MSG_MAX, "<%d> %s:process: entry, pact = %d\n",  par->pact);
    
    pdset = (ASSEMBLY_CONTROL_DSET *) (par->dset);

    /*
     * If the record is being processed by someone else then return
     * immediately.
     */

    if( par->pact )
    {
        return( DAR_S_SUCCESS );
    }
    

    /*
     * Set the record processing flag to prevent anyone else from trying
     * to process this record until we are finished.
     */

    par->pact = TRUE;

    DEBUG(DAR_MSG_MAX, "<%d> %s:process:timeout = %ld\n", pPriv->timeout);


    /*
     *  If the fault flag has been set then we have been called to do
     *  fault processing.
     */

    if( pPriv->fault == DAR_FAULT_NEW)
    {
        /*
         * A fault has been detected!  Call device support fault 
         * handling function immediately.   The device support will know
         * how to react to this condition.
         */

        pPriv->fault = DAR_FAULT_CLEAR;
        par->hlth = DAR_HLTH_BAD;
        MONITOR(RECORD_HTH);
        DEBUG(DAR_MSG_LOG, "<%d> %s:process: fault occurred%c\n", ' ');

        DEBUG(DAR_MSG_MAX, "<%d> %s:calling processFault%c\n", ' ');
        status = (*pdset->processFault) (par);
        DEBUG(DAR_MSG_MAX, "<%d> %s:processFault returns %ld\n", status);
    }


    /*
     *  If the timeout flag has been set then we have been called to do
     *  command timeout processing.
     */

    else if( pPriv->timeout )
    {       
        /*
         *  If showBusy is set then this is not really a command timeout,
         *  it just means that it is the end of a command
         *  acknowledgement cycle and it is time to reset the BUSY flag.
         */

        if ( pPriv->showBusy == TRUE )
        {
            pPriv->showBusy = FALSE;
            pPriv->cmdFinished = TRUE;
            par->pp = TRUE;
            pPriv->timeout = FALSE;
        }
        

        /*
         * Otherwise a command has taken too long to execute so something
         * has gone seriously wrong.   Calling the device support function
         * busyReceivedFromDevice with the timeout flag set will cause it
         * to immediately abort the command.
         */
 
        else
        {
            status = busyReceivedFromDevice( par );
            pPriv->timeout = FALSE;       
        }
    }


    /*
     * The busChange flag indicates that one of the Device Control 
     * Records has either started or finished a command.   If the
     * flag is set then we are being processed in reaction to this 
     * change in command state.
     */

    else if( pPriv->busChange )
    {
        /*
         *  Call the device support busyReceivedFromDevice function to
         *  acknowledge this fact and decide what to do next.
         */

        pPriv->busChange = FALSE;
        status = busyReceivedFromDevice( par );
        DEBUG(DAR_MSG_MAX, "<%d> %s:busyReceivedFromDevice status = %ld\n", 
              status );
    }


    /*
     * If none of the above then this must be a normal processing caused
     * by the recipt of a new command directive.
     */

    else
    {
        /*
         * Process the command according to the directive just recieved.
         */

        status = processDirective(par);
        
        /*
         * If this was a just a preset (check command arguments) or if the
         * given directive was rejected as being invalid then terminate
         * record processing immediately and return.
         */

        if ((par->dir == DAR_DIR_PRESET) || (par->val == DAR_ACK_VAL_REJECT))
        {
            par->pact = FALSE;
            writeOutputLinks (par);
            alarm(par);
            monitor (par);
            recGblFwdLink(par);   
            return(status);
        }
 

        /*
         * If the cmdFinished flag has been set after directive processing
         * then the command was finished too quickly for external processes
         * to see.   Set the showBusy flag to indicate that this is a
         * command acknowledge cycle and start the command timeout timer
         * to re-process the record in one second.   This will leave the
         * BUSY field set to busy for one second even though the command
         * has finished processing already.
         */ 

        if ( pPriv->cmdFinished )
        {
            pPriv->cmdFinished = FALSE;
            pPriv->showBusy = TRUE;
            assStartTimer( par, &status, 1 );
        }   
    }


    /*
     * Now update whatever status fields were affected by the processing 
     * and clear the Processing ACTive flag so that others can process this 
     * record and we are done.
     */

    completeCommand( par, &status );

    par->pact = FALSE;
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * processDirective
 *
 * INVOCATION:
 * status = processDirective (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)  EPICS database struct for this record
 *                    record structure.
 *
 * FUNCTION VALUE:
 * (long) function return status.
 *
 * PURPOSE:
 * Start processing a new command directive.
 *
 * DESCRIPTION:
 * Start command execution based on the value of the directive field
 * via the following algorithm:
 *
 *      Return immediately if recod not marked and not a mark command.
 *      Clear command rejection and error message states.
 *      Read the input links to update remaining input fields.
 *      Write output links to update simulation and debug links.
 *      Return an error immediately if the fault line is asserted.
 *
 *      Process the record based on the command directive as follows:
 *      {
 *          CLEAR - clear the mark field to disable directive processing.
 *          MARK - set the mark field to enable the next directive.
 *          PRESET - check command inputs by calling startingState. 
 *          START - start a command by calling startingState.
 *          STOP - terminates a non-simulated command by calling stoppingState.
 *          Any other directive - return failure code.
 *      }
 *
 *     Return immediately if the command was rejected.
 *     Set cmdFinished flag if the command failed.
 *     If we are stopping a command, overwrite currentCommand with STOP code.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */


static long processDirective
(
    ASSEMBLY_CONTROL_RECORD *par    /* (in)  Ptr to ass rec.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struct. */
    long status = DAR_S_SUCCESS;            /* Function return status   */


    DEBUG(DAR_MSG_MAX, "<%d> %s:processDirective:entry, dir=%d\n", par->dir);

    /*
     *  If record has not been marked for processing ignore everything
     *  except a mark directive.
     */

    if (par->mark == 0 && par->dir != DAR_DIR_MARK)
    {
        return( DAR_S_SUCCESS );
    }


    /*
     *  Start of directive processing.   Clear any existing error messages
     *  or command rejections.
     */

    *par->mess = '\0';
    MONITOR (RECORD_MESS);		/* Added by CJT */

    par->val = DAR_ACK_VAL_ACCEPT;
    MONITOR(RECORD_VAL);


    /*
     * Read the input links to update remaining input fields.
     */

    status = readInputLinks (par, FALSE );
    CHECKSTAT( status, return(status) );


    /*
     * Write output links to update simulation and debug links
     */
 
    status = writeOutputLinks (par);
    CHECKSTAT( status, return(status) );


    /*
     *  If the record fault line is active (interlocked) then we can
     *  not process the record until it has been cleared.   Return
     *  this fact immediately as an error...
     */

    if ( par->ilck )
    {
        DEBUG(DAR_MSG_ERROR, "<%d> %s:Assembly is interlocked%c\n",' ');
        assAddErrorMessage(par, "Assembly is interlocked." );
        par->val = DAR_ACK_VAL_REJECT;
        MONITOR(RECORD_VAL);
        par->mark = 0;
        MONITOR(RECORD_MARK);
        status = DAR_E_FLT_CLR;
        return( status );
    }

    /*
     *  Process the record based on the command directive.
     */

    switch (par->dir)
    {

        /*
         * CLEAR clears the mark field to disable directive processing.
         */

        case DAR_DIR_CLEAR:
            DEBUG(DAR_MSG_MIN, "<%d> %s:process CLEAR directive%c\n", ' ');
            if ( par->mark != 0 )
            {
                par->mark = 0;
                MONITOR(RECORD_MARK);
            }
            break;


        /*
         * MARK explicitly sets the mark field to enable the next directive.
         */

        case DAR_DIR_MARK:
            DEBUG(DAR_MSG_MIN, "<%d> %s:process MARK directive%c\n", ' ');
            if ( par->mark != 1 )
            {
                par->mark = 1;
                MONITOR(RECORD_MARK);
            }
            break;
  

        /*
         * Both PRESET and START process the directive by calling the processStart
         * function. The MARK field remains set after a successful PRESET directive
         * so that if a START directive follows it will execute. The MARK field is
         * cleared if a PRESET directive fails, and is cleared after a START directive.
         */

        case DAR_DIR_PRESET:
        case DAR_DIR_START:
            DEBUG(DAR_MSG_MIN, "<%d> %s:process PRESET/START directive%c\n", ' ');
            status = processStart( par );

            if ( (par->dir == DAR_DIR_START) || (status != DAR_S_SUCCESS) )
            {
               par->mark = 0;
               MONITOR(RECORD_MARK);
            }
            break;


        /*
         * STOP directive terminates a command in an orderly way by
         * calling the stoppingState function.   If there is no active
         * command to stop we set the record busy anyway to acknowledge
         * receipt of the directive.
         */
                  
        case DAR_DIR_STOP:
            DEBUG(DAR_MSG_MIN, "<%d> %s:process STOP directive%c\n", ' ');
            if (par->busy != DAR_BUSY_BUSY )
            {
                par->busy = DAR_BUSY_BUSY;
                MONITOR(RECORD_BUSY);                           
            }

            /*
             *  If we are not in virtual simulation mode then switch
             *  to stopping state and this will terminate the command.
             *  In virtual simulation mode there is no command to stop
             *  so just signal completion.
             */

            if (pPriv->simulation != DAR_SIM_VSM )
            {
                status = stoppingState( par );
            }
            else
            {
                par->pp = TRUE;
                pPriv->cmdFinished = TRUE;
            }

            par->mark = 0;
            MONITOR(RECORD_MARK);
            break;

        
        /*
         * Any other value in the directive field is invalid so
         * just flag it as an error.
         */

        default:
            DEBUG(DAR_MSG_ERROR, "<%d> %s:process bogus directive%c\n", ' ');
            assAddErrorMessage(par, "Invalid directive");
            status = DAR_E_INV_DIR;
            break;
           
    }   /* switch (par->dir) */


    /*
     * If the command was rejected return here.
     */

    if (par->val == DAR_ACK_VAL_REJECT)
    {
        return (status);
    }


    /*
     * If the command was accepted but failed immediately set the
     * cmdFinished flag to keep the BUSY field set for at least one
     * second.
     */

    if (status < DAR_S_SUCCESS || (strlen (par->mess)))
    {
        par->pp = TRUE;
        pPriv->cmdFinished = TRUE;
    }


    /*
     *  If we have successfully started a STOP directive
     *  then replace last command with the value of DAR_STOP_CMD.
     */

    if ( par->dir == DAR_DIR_STOP )
    {
        pPriv->lastCommand = DAR_STOP_CMD;
    }
                                             
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * processStart
 *
 * INVOCATION:
 * status = processStart (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)  EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * (long) function return status.
 *
 * PURPOSE:
 * Check command validity and execute it if the directive is START.
 *
 * DESCRIPTION:
 * This function is called when a PRESET or START directive is received
 * by the record.  It checks that it is possible to execute the command 
 * at this time and that all input attributes are valid.   If the directive 
 * is START then begin execution of the command.  The following algorithm 
 * is used to do this:
 *
 *      If not initialized then reject anything other than INIT or TEST.
 *      If not indexed then reject all motion commands.
 *      If moving or tracking only accept other MOVE or TRACK commands.
 *      Call device support to check all of the input attributes.
 *      Exit here if doing a PRESET or command was rejected. 
 *      Call processState to execute the command based on its mode. 
 *      If started successfully then save mode being executed
 *
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */


static long processStart
(
    ASSEMBLY_CONTROL_RECORD *par            /* (in)  Ptr to ass rec.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struc.    */
    ASSEMBLY_CONTROL_DSET *pdset;           /* Device support functions   */
    char ErrMess[MAX_STRING_SIZE];          /* Temp message buffer        */
    long status = DAR_S_SUCCESS;            /* Function return status.    */

    DEBUG(DAR_MSG_MAX, "<%d> %s:processStart: entry%c\n", ' ');

    pdset = (ASSEMBLY_CONTROL_DSET *) (par->dset);


    /*
     *  Record must have been initialized before executing any commands
     *  other than init or test.
     */

    if (par->init == 0 &&
        par->mode != DAR_MODE_INIT &&
        par->mode != DAR_MODE_TEST)
    {
        DEBUG(DAR_MSG_LOG, "<%d> %s:Assembly not initialized%c\n",' ');
        assAddErrorMessage(par, "You must INIT first." );
        status = DAR_E_MODE;
    }


    /*
     *  Devices must be indexed before any motion commands can be accepted.
     */

    if (!status &&
        par->indx == 0 &&
        par->mode != DAR_MODE_INIT && 
        par->mode != DAR_MODE_INDEX && 
        par->mode != DAR_MODE_TEST)
    {
        DEBUG(DAR_MSG_LOG, "<%d> %s:Assembly not indexed%c\n",' ');
        assAddErrorMessage(par, "You must INDEX first." );
        status = DAR_E_MODE;
    }
 

    /*
     *  If a MOVE or TRACK command is being processed already then reject
     *  anything except another MOVE or TRACK command.
     */

    if (! status && par->busy == DAR_BUSY_BUSY &&
        !( (par->mode == DAR_MODE_TRACK && par->asta == DAR_STATE_TRACKING ) ||
	     ( ( par->mode == DAR_MODE_MOVE || par->mode == DAR_MODE_PARK ) && 
            par->asta == DAR_STATE_TRACKING ) ||
	     ( ( par->mode == DAR_MODE_MOVE || par->mode == DAR_MODE_PARK ) && 
            par->asta == DAR_STATE_MOVING ) ) )
    {   
        DEBUG(DAR_MSG_LOG, "<%d> %s: Rejecting command while moving%c\n", ' ');
        sprintf (ErrMess, "Assembly is moving, state:%8d",par->asta );
        assAddErrorMessage(par, ErrMess);
        status = DAR_E_BUSY;
    }


    /*
     *  Call device support to check all of the input attributes
     *  and insure that the request is valid at this time.
     */

    if (!status)
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:calling checkAttributes%c\n", ' ');
        status = (*pdset->checkAttributes) (par);
        DEBUG(DAR_MSG_MAX, "<%d> %s:checkAttributes returns %ld\n", status);
    }

    /*
     *  If the command was rejected of we are just doing a PRESET 
     *  directive then exit here.
     */

    if (status || par->dir == DAR_DIR_PRESET)
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:processStart: PRESET finished%c\n", ' ');
        if ( status != DAR_S_SUCCESS )                                      
        {
            par->mark = 0;
            MONITOR(RECORD_MARK);
            par->val = DAR_ACK_VAL_REJECT;
            MONITOR(RECORD_VAL);
        }        
        return( status );
    }


    /*
     * The command was accepted so it is time to set the processing status
     * output field "BUSY" to BUSY to indicate that command processing has
     * started.
     */

    if (par->busy != DAR_BUSY_BUSY)
    {
        par->busy = DAR_BUSY_BUSY;
        MONITOR(RECORD_BUSY);
    }


    /*
     *  In virtual simulation mode save the command mode being processed for
     * future reference, then set the command finished flag and return since we
     * have nothing else to do. Returning at this point with the finished flag
     * set will trigger a 1 second acknowledgement cycle.
     */

    if( pPriv->simulation == DAR_SIM_VSM )
    {
        if (!status)
        {
            pPriv->lastCommand = par->mode;
        }
        par->pp = TRUE;
        pPriv->cmdFinished = TRUE;
        return( status );
    }


    /*
     * Now call processState to execute the command based on its mode. 
     */             

    DEBUG(DAR_MSG_MAX, "<%d> %s:processStart: process the state %c\n", ' ');
    status = processState(par); 


    /*
     *  If the command was started successfully then save the command mode 
     *  being processed for future reference. 
     */

    if (!status)
    {
         pPriv->lastCommand = par->mode;
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * processState
 *
 * INVOCATION:
 * status = processState (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)  EPICS database struct for this record
 *                    record structure.
 *
 * FUNCTION VALUE:
 * (long) function return status.
 *
 * PURPOSE:
 * Select device support function to be called to process record.
 *
 * DESCRIPTION:
 * Continue processing the record by:
 *
 *      Saving the mode to be processed in the internal control struct.
 *      Selecting the device support function appropriate for the command mode.
 *      Executing the selected function.
 *      Updating the output state status variable if function was successful.
 *      Setting the Post Processing flag to insure output fields are updated.
 *
 *     
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */


static long processState
(
    ASSEMBLY_CONTROL_RECORD *par    /* (in)  Ptr to ass rec.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Internal control struct.  */
    DEVSUPFUN funcPtr;                      /* Device support funct ptr. */
    ASSEMBLY_CONTROL_DSET *pdset;           /* Device support functions  */
    int state = DAR_STATE_IDLE;             /* Record operating state.   */
    long status = DAR_S_SUCCESS;            /* Function return status.   */


    DEBUG(DAR_MSG_MAX, "<%d> %s:processState: entry%c\n", ' ');
    DEBUG(DAR_MSG_MAX, "<%d> %s:processState: mode = %d\n", par->mode);

    pdset = (ASSEMBLY_CONTROL_DSET *) (par->dset);


    /*
     * Save the mode we have been asked to execute then start execution
     * based on the mode field.
     */

    pPriv->currCommand = par->mode;

    switch (par->mode) {

        /* 
         * INIT mode sets the health to GOOD and selects the
         * device support initialization function.
         */

        case DAR_MODE_INIT:
            state = DAR_STATE_INITIALIZING;
            funcPtr = pdset->initMode;
            par->hlth = DAR_HLTH_GOOD;
            MONITOR(RECORD_HTH);
            break;


        /* 
         * Both MOVE and PARK modes select the device support moving
         * function.
         */

        case DAR_MODE_MOVE:
        case DAR_MODE_PARK:
            state = DAR_STATE_MOVING;
            funcPtr = pdset->moveMode;
            break;

        /*
         * TRACK mode selects the device support tracking function.
         */

        case DAR_MODE_TRACK:
            state = DAR_STATE_TRACKING;
            funcPtr = pdset->trackMode;
            break;

        /*
         * TEST mode selects the device support testing function.
         */

        case DAR_MODE_TEST:
            state = DAR_STATE_TESTING;
            funcPtr = pdset->testMode;
            break;


        /* 
         * INDEX mode selects the device support indexing function.
         */

        case DAR_MODE_INDEX:
            state = DAR_STATE_INDEXING;
            funcPtr = pdset->indexMode;
            break;

        /*
         * UPDATE mode selects the device support updating function.
         */

        case DAR_MODE_UPDATE:
            state = DAR_STATE_UPDATING;
            funcPtr = pdset->updateMode;
            break;
    
        /*
         * Any other value in the mode field causes an error.
         */

        default:
            status = DAR_E_MODE;
            status = idleState( par, status );
            par->pp = TRUE;
            return status;
    }                         
   

    /*
     *  Now invoke the function selected above.
     */

    status = executeState( par, state, funcPtr );

   
    /*
     *  If started successfully then note the change in state
     */
    
    if ( status == DAR_S_SUCCESS && state != par->asta)
    {
        par->asta = state;
        MONITOR(RECORD_ASTA);
    }

    /*
     * Always set pp to true so that the command will complete and
     * the record will respond to passive process directives.
     */

    par->pp = TRUE;
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * readInputLinks
 *
 * INVOCATION:
 * status = readInputLinks (par, readAcks);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)  EPICS database struct for this record
 * (>) readAcks (int)  Read only the device control record input links
 *
 * FUNCTION VALUE:
 * (long) record processing success code.
 *
 * PURPOSE:
 * Read values from EPICS input links
 *
 * DESCRIPTION:
 * Either read the device support ACK fields or respond to changes
 * in the debug and simulation states via the following algorithm: 
 *
 *      If readAcks flag is set
 *      {
 *          If link is attached to a record then read it.
 *      }
 *
 *      Else
 *      {
 *          Read debug link and update debug state.
 *          If record is IDLE, read simulation link and update sim state.
 *      }
 *
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * Does not appear to anthing with the ACK values once it has read them.
 *-
 ************************************************************************
 */

static long readInputLinks
(
    ASSEMBLY_CONTROL_RECORD *par,       /* (in) Ptr. to ass rec.      */
    int readAcks                        /* (in) Only read dev lnks    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Ptr to ass private.    */
    long        status = DAR_S_SUCCESS;     /* Local status.          */
    unsigned short sval;                    /* Value of ack field     */


    DEBUG(DAR_MSG_MAX, "<%d> %s:readInputLinks entry: readAcks:%d\n", readAcks);

    /*
     *  If the readAcks flag is set then read each of the the attached
     *  device control record command acknowledge (ACK) links.
     */

     if( readAcks )
     {

        if (par->ack1.type != CONSTANT)
        {
            CHECKFNSTAT(recGblGetFastLink (&(par->ack1), (void *) par, &sval),
                        status, return(status) );
        }

        if (par->ack2.type != CONSTANT)
        {
            CHECKFNSTAT(recGblGetFastLink (&(par->ack2), (void *) par, &sval),
                        status, return(status) );
        }
    
        if (par->ack3.type != CONSTANT)
        {
            CHECKFNSTAT(recGblGetFastLink (&(par->ack3), (void *) par, &sval),
                        status, return(status) );
        } 

        if (par->ack4.type != CONSTANT)
        {
            CHECKFNSTAT(recGblGetFastLink (&(par->ack4), (void *) par, &sval),
                        status, return(status) );
        }

        if (par->ack5.type != CONSTANT)
        {
            CHECKFNSTAT(recGblGetFastLink (&(par->ack5), (void *) par, &sval),
            status, return(status) );
        }
    }


    /* 
     * Otherwise read the debug and simulation level links.
     */

    else
    {
        /*
        *  Detect changes to the debug level
        */

        if (pPriv->debug != par->dbug)
        {
            DEBUG(DAR_MSG_MAX, "<%d> %s:readInputLinks: new dbug:%d\n", par->dbug);
            pPriv->debug = par->dbug;
            MONITOR(RECORD_DBUG);
        }


        /*
         *  Only detect changes to the simulation mode but only
         *  if record is IDLE or ERR.
         */
     
        if ( (par->busy == DAR_BUSY_IDLE || 
              par->busy == DAR_BUSY_ERROR) &&
             pPriv->simulation != par->simm )
        {
            DEBUG(DAR_MSG_MAX, "<%d> %s:readInputLinks: new simm:%d\n", par->simm);
            pPriv->simulation = par->simm;
            MONITOR(RECORD_SIMM);
        }
    }    

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * setFields
 *
 * INVOCATION:
 * status = setFields (par, &ppVal, &pType);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)  EPICS database struct for this record
 * (<) ppVal (void **)                   Pointer to first value pointer.
 * (>) pType (unsigned short *)          Pointer to first data type.

 *
 * FUNCTION VALUE:
 * (long) record return function status.
 *
 * PURPOSE:
 * Dynamically create database fields with user-defined datatypes.
 *
 * DESCRIPTION:
 * Database fields with user-defined datatypes are not created automatically
 * by the epics system since the size of each field is not fixed when the
 * system is made.   This function will create each of these fields and
 * save the pointer to the created field in the EPICS database structure
 * for this record.   Because these user defined fields always come in 
 * blocks of DAR_ARG_MAX consecutive fields this function accepts the first
 * of these fields and then automatically fills in the rest of the block
 * via the following algorithm:
 *
 *      For each field up to DAR_ARG_MAX
 *      {
 *          If type is STRING allocate MAX_STRING_SIZE bytes.
 *          Else if type is DOUBLE allocate sizeof(double) bytes.
 *          Else if type is LONG allocate sizeof(long) bytes.
 *          Else return an error.
 * 
 *          If allocation failed return an error. 
 *      }
 *
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * None.
 *
 * DEFICIENCIES:
 * Currently only handles STRING, DOUBLE. and LONG data types.
 *-
 ************************************************************************
 */

static long setFields
(
    ASSEMBLY_CONTROL_RECORD *par,       /* (in)  Ptr to ass. rec.      */
    void                **ppVal,        /* (in) Pointer field ptr      */
    unsigned short      *pType          /* (in) Pointer to field type. */
)
{
    int     i;                          /* Field Counter.              */
    long    status = DAR_S_SUCCESS;     /* Returned function status.   */

    DEBUG(DAR_MSG_MAX, "<%d> %s:setFields: entry%c\n", ' ');


    /*
     *  Cycle through DAR_MAX sequential fields starting with the
     *  given one and save the pointer to the field created.
     */

    for( i = 0; i < DAR_ARG_MAX; i++, ppVal++, pType++ )
    {
        /* Create a string field */
        if( *pType == DBF_STRING )
        {
            *ppVal = (char *)calloc(1, MAX_STRING_SIZE);
        }
        
        /* Create a double field */
        else if( *pType == DBF_DOUBLE )
        {
            *ppVal  = (char *)calloc(1, sizeof(double));
        }

        /* Create a long field */
        else if( *pType == DBF_LONG )
        {
            *ppVal  = (char *)calloc(1, sizeof(long));
        }
 
        /* None of the above, field type not supported so return an error */
        else
        {
            par->hlth = DAR_HLTH_WARNING;
            MONITOR(RECORD_HTH);
            status = S_db_badChoice;
            recGblRecordError (status, (void *)par,
                               "recAssemblyControl(init_record)");
            return status;
        }

        /* Allocation failed, return an error */
        if (*ppVal == NULL)
        {
            status = -1;
            recGblRecordError (status, par, 
                               __FILE__ ":ran out of memory creating fields");
            return status;
        }
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * setFlags
 *
 * INVOCATION:
 * setFlags (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)  EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Update the initialized, indexed and parked flags.
 *
 * DESCRIPTION:
 * Update the initialized, indexed and parked flags based on the
 * type of processing just completed.  This function assumes that
 * all command processing was been completed successfully before
 * it was called. Called from completeCommand.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * completeCommand & unsetFlags
 *
 * DEFICIENCIES:
 * 1. Clears PARK flag when the STOP directive is issued while the 
 * assembly is IDLE.
 *-
 ************************************************************************
 */


static void setFlags
(
    ASSEMBLY_CONTROL_RECORD *par    /* (in)  Ptr to ass rec.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt;   /* Internal control struct.   */

    DEBUG(DAR_MSG_MAX, "<%d> %s:setFlags: entry, lastCmd=%d\n",
          pPriv->lastCommand );

    /*
     *  If the initialization command (mode = INIT) completed then
     *  update the INIT field.
     */

    if ( pPriv->lastCommand == DAR_MODE_INIT && par->init != 1 )
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:setFlags: init.\n%c", ' ' );
        par->init = 1;
        MONITOR(RECORD_INIT);
    }


    /*  If the index command (mode = INDEX) completed then update the
     *  INDX field.
     */

    else if ( pPriv->lastCommand == DAR_MODE_INDEX  && par->indx != 1 )
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:setFlags: indx.\n%c", ' ' );
        par->indx = 1;
        MONITOR(RECORD_INDX);
    }
        

    /*
     *  If the park command (mode=PARK) completed then update the PARK
     *  field.
     */

    else if ( pPriv->lastCommand == DAR_MODE_PARK && par->park != 1 )
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:setFlags: park.\n%c", ' ' );
        par->park = 1;
        MONITOR(RECORD_PARK);
    }


    /*
     *  Health must be good if a movement command completed succesfully
     *  so update HLTH field.
     */

    if ( pPriv->lastCommand == DAR_MODE_INDEX  || 
         pPriv->lastCommand == DAR_MODE_MOVE   ||
         pPriv->lastCommand == DAR_MODE_TRACK  ||
         pPriv->lastCommand == DAR_MODE_UPDATE ||
         pPriv->lastCommand == DAR_MODE_PARK )
    {
        par->hlth = DAR_HLTH_GOOD;
        MONITOR(RECORD_HTH);
    }


    /*
     *  If the PARK flag is set and any command involving motion
     *  was completed then clear the park flag since we have
     *  probably moved away from the park position. Will also
     *  clear the PARK flag if a STOP command is issued when IDLE.
     */

    if ( par->park  == 1 &&
        ( pPriv->lastCommand == DAR_MODE_MOVE ||
          pPriv->lastCommand == DAR_MODE_TRACK ||
          pPriv->lastCommand == DAR_MODE_INDEX ||
          pPriv->lastCommand == DAR_MODE_UPDATE ||
          pPriv->lastCommand == DAR_STOP_CMD ) )
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:setFlags: park=0.\n%c", ' ' );
        par->park = 0;
        MONITOR(RECORD_PARK);
    }

}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * special
 *
 * INVOCATION:
 * status = special (paddr, after);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) paddr  (struct dbAddr *) Pointer to database field structure.
 * (>) after  (int)             Pre/post processing flag.
 *
 * FUNCTION VALUE:
 * (long) record processing success code.
 *
 * PURPOSE:
 * Perform special pre/post processing on selected fields
 *
 * DESCRIPTION:
 * If a database field is created with a DBR_SPECIAL attribute then
 * this function will automatically be called by the EPICS system
 * before the field is updated and after it has been updated.  This
 * function identifies the input attribute (A->E), interlock (ILCK) and
 * device control record processing state change (BUS1->BUS5) fields
 * as requiring special processing.  
 *
 * If the fault or device control record busy state change fields are
 * written to then the asembly control record processing function must
 * be explicitly invoked to react to this information.   This is done
 * by setting a watchdog timer which will process the record via a
 * callback task at a later date.   At the time this record was written
 * it was not understood how to enable the normal EPICS Passive Processing
 * sytem so this callback method was used instead.
 * 
 * The update of a field is handled using the following algorithm:
 *
 *      Ignore the call to this function before the field is set.
 *
 *      If the fault field was written to and it is now non-zero
 *      {   
 *          Set the internal fault detected flag.
 *          If no special watchdog timer exists
 *          {
 *              Create a new special watchdog timer.
 *              IF creating the timer failed then return an error.
 *          }
 *          Start the special watchdog timer.
 *          If starting the timer failed then return an error.
 *      }
 *          
 *      Else if a device control record state field was written to
 *      {
 *          Set an internal flag to identify which field just changed.
 *          Start the special watchdog timer.
 *          If starting timer failed then return an error.
 *      }
 *
 *      Else assume that an attribute field was written to so mark the record.
 *    
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * specialCallback
 *
 * DEFICIENCIES:
 * A check is made in fault field processing to create a watchdog timer
 * if it does not exist, busy field processing does not do this.
 * A write to an attribute field calls monitor which will raise monitors
 * on all marked fields!!!  This could happen any time while the record
 * is processing!!!
 *-
 ************************************************************************
 */

static long special
(
    struct dbAddr *    paddr,        /* (in)  Address of field.    */
    int                after         /* (in)  Order called.        */
)
{
    ASSEMBLY_CONTROL_RECORD  *par;      /* Assembly rec. pointer.    */
    ASS_CONTROL_PRIVATE *pPriv;         /* Internal control struct.  */
    struct callback *pCallback;         /* Callback timer struct. */
    int  waitTime;                      /* Delay before callback */
    long  status = DAR_S_SUCCESS;       /* Function return status */


    /*
     * Recover record information from the database field struct.
     */

    par = (ASSEMBLY_CONTROL_RECORD *) paddr->precord;
    pPriv = par->dpvt;


    /*
     *  Ignore the pre-writing call to this function.   
     */
    
    if( !after )
    {
        return( DAR_S_SUCCESS );
    }

    DEBUG(DAR_MSG_MAX, "<%d> %s:special: entry....%c\n", ' ');
    

    /*
     * Interlock (fault) field writes get the highest priority!!
     */
    
    if (paddr->pfield == (void *) &par->ilck)
    {  
        /*
         * Only generate a callback if the field has been set.
         */

        if ( par->ilck )
        {
            DEBUG(DAR_MSG_LOG, "<%d> %s:special: ilck occurred%c\n", ' ');
            MONITOR(RECORD_FLT);
     
            pPriv->fault = DAR_FAULT_WARNING;
            pCallback = (struct callback *)(pPriv->special);
            waitTime = (int) ( 2 );


            /*
             *  Create a watchdog timer is one does not already exist.
             */

            if( pCallback->wd_id == NULL )
            {
                if ( ( pCallback->wd_id = wdCreate() ) == NULL )
                {
                    status = DAR_E_MALLOC;
                    par->hlth = DAR_HLTH_BAD;
                    MONITOR(RECORD_HTH);
                    recGblRecordError (status, par, 
                                       __FILE__ ":no room for watchdog timer");
                    return( status );
                }
            }

            /*
             * Start the watchdog timer to re-process the record.
             */
      
            if (wdStart(pCallback->wd_id, waitTime, (FUNCPTR)callbackRequest,
                (int)pCallback) == ERROR )
            {
                status = DAR_E_TIMER_CAN;
                DEBUG(DAR_MSG_FATAL, "<%d> %s:special:ilck:timer failed%c\n",
                      ' ');
            }
        }
    }


    /*
     *  There was a change in one of the attached BUS[1-5] values.
     */

    else if (paddr->pfield == (void *) &par->bus1 ||
             paddr->pfield == (void *) &par->bus2 ||
             paddr->pfield == (void *) &par->bus3 ||
             paddr->pfield == (void *) &par->bus4 ||
             paddr->pfield == (void *) &par->bus5 )
    {

        /*
         * Determine which of the fields has changed and set an internal
         * flag to record this.
         */

        if ( paddr->pfield == (void *) &par->bus1 )
        {
            pPriv->bus1 = TRUE;
            DEBUG(DAR_MSG_MAX, "<%d> %s:special: bus1=%d\n", par->bus1 );
        }
        else if ( paddr->pfield == (void *) &par->bus2 )
        {
            pPriv->bus2 = TRUE;
            DEBUG(DAR_MSG_MAX, "<%d> %s:special: bus2=%d\n", par->bus2 );
        }
        else if ( paddr->pfield == (void *) &par->bus3 )
        {
            pPriv->bus3 = TRUE;
            DEBUG(DAR_MSG_MAX, "<%d> %s:special: bus3=%d\n", par->bus3 );
        }
        else if ( paddr->pfield == (void *) &par->bus4 )
        {
            pPriv->bus4 = TRUE;
            DEBUG(DAR_MSG_MAX, "<%d> %s:special: bus4=%d\n", par->bus4 );
        }
        else if ( paddr->pfield == (void *) &par->bus5 )
        {
            pPriv->bus5 = TRUE;
            DEBUG(DAR_MSG_MAX, "<%d> %s:special: bus5=%d\n", par->bus5 );
        }

        /*
         *  Start the special callback timer, which will process the record.
         */

        pCallback = (struct callback *)(pPriv->special);
        waitTime = (int) ( 2 );
        if (wdStart(pCallback->wd_id, waitTime, (FUNCPTR)callbackRequest,
            (int)pCallback) == ERROR )
        {
            status = DAR_E_TIMER_CAN;
        }
    }


    /*
     *  Otherwise, assume it means there was a change in one of the 
     *  attributes OR mode fields. Mark the record to enable directive
     *  processing.
     */

    else
    {
        if ( par->mark == 0 )
        {
            par->mark = 1;
            MONITOR(RECORD_MARK);
            monitor (par);
        }
    }
   
    DEBUG(DAR_MSG_MAX, "<%d> %s:special: pact=%d ..... \n", par->pact );

    return ( DAR_S_SUCCESS );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * specialCallback
 *
 * INVOCATION:
 * specialCallback (pcallback);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) struct callback *pcallback    Ptr to callback struct.
 *                                       
 * FUNCTION VALUE:
 * none.
 *
 * PURPOSE:
 * Process the record in response to the expiry of the "special" timer.
 *
 * DESCRIPTION:
 * This is the function called by the "special" watchdog timer when it 
 * expires.   This timer was set by the special function when it detected an 
 * input field access.  It will try to re-process the record by calling the
 * EPICS dbProcess function.  If the record is already being processed
 * when the watchdog timer expires then re-start the timer and try again
 * in a little while...
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * initTimer, special.
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */
static void specialCallback   
(
    struct callback *pcallback      /* (in)  Ptr to callback struct.*/
)
{
    ASSEMBLY_CONTROL_RECORD *par;           /* Ptr to ass. rec.          */
    ASS_CONTROL_PRIVATE *pPriv;             /* Internal control struct.  */
    int waitTime;                           /* Delay time  */


    /*
     * Recover record information from the callback struct.
     */

    par = (ASSEMBLY_CONTROL_RECORD *) pcallback->precord;
    pPriv = par->dpvt;

    DEBUG(DAR_MSG_MAX, "<%d> %s:specialCallback: entry%c\n", ' ');


    /*
     *  If the record is being processed by someone else when the timer
     *  expires then re-start the timer.
     */

    if(par->pact)
    {
        struct callback *pcallback = NULL;

        /*
         * Recover original callback structure.
         */

        if (( pcallback = ( struct callback *)pPriv->special ) == NULL)
        {
            DEBUG(DAR_MSG_FATAL, "<%d> %s:specialCallback:no special timer%c\n",
                ' ');
            return;
        }

        waitTime = (int) ( 2 );

        
        /* 
         * Check to make sure that we have a watchdog timer to use for
         * the delayed callback.   If not it is necessary to re-create one.
         */

        if (pcallback->wd_id == NULL)
        {
            if (( pcallback->wd_id = wdCreate()) == NULL)
            {
                recGblRecordError (DAR_E_MALLOC, par, 
                                   __FILE__ ":no room for watchdog timer");
                return;
            }
        }
        callbackSetPriority(priorityLow, &pcallback->callback);
        if (wdStart (pcallback->wd_id, waitTime, (FUNCPTR) callbackRequest,
            (int)pcallback) == ERROR )
        {
            recGblRecordError (DAR_E_TIMER_CAN, par,
                               __FILE__ ":start watchdog timer failed");
        }

        DEBUG(DAR_MSG_FULL, "<%d> %s:specialCallback: record busy .. retry%c\n",
            ' ');
    }

    /*
     *  Otherwise the record is available.   Invoke the process function
     *  directly.
     */

    else
    {
       /*
        *  Indicate if this is a fault or busy change re-processing.
        */

       if ((par->ilck) && (pPriv->fault == DAR_FAULT_WARNING))
        {
            pPriv->fault = DAR_FAULT_NEW;
        }
        else
        {
            pPriv->busChange = TRUE;
        } 

        /*
         *  Then call the EPICS record processing function.
         */

        dbProcess((struct dbCommon *)par);
    }
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * stoppingState
 *
 * INVOCATION:
 * status = stoppingState (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Stop a command that is currently executing.
 *
 * DESCRIPTION:
 * Stop any command that is currently executing by calling the device 
 * support stopDirective function.  It is necessary for the device support
 * code to do this because the manner of stopping is very implementation
 * specific.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long stoppingState
(
    ASSEMBLY_CONTROL_RECORD
                *par        /* (in)  Ptr to ass. rec.    */
)
{
    long     status = DAR_S_SUCCESS;    /* Return function status.    */
    ASSEMBLY_CONTROL_DSET *pdset;       /* Device support functions   */

    DEBUG(DAR_MSG_MAX, "<%d> %s:stoppingState: entry%c\n", ' ');

    pdset = (ASSEMBLY_CONTROL_DSET *) (par->dset);


    /*
     *  Call the device support code stopDirective function, which
     *  will bring the current command to an orderly stop.
     */
    
    DEBUG(DAR_MSG_MAX, "<%d> %s:calling stopDirective%c\n", ' ');
    status = (*pdset->stopDirective) (par);
    DEBUG(DAR_MSG_MAX, "<%d> %s:stopDirective returns %d\n", status);

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * timerCallback
 *
 * INVOCATION:
 * timerCallback (pcallback);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) struct callback *pcallback    Ptr to callback struct.
 *
 * FUNCTION VALUE:
 * none.
 *
 * PURPOSE:
 * Process the record in response to a command timeout expiry.
 *
 * DESCRIPTION:
 * This is the function called by the "command timeout" watchdog timer when
 * it expires.   This timer was set when a command started executing and
 * should have been stopped when the command finished.   If the timer
 * expired then something went wrong. It will try to re-process the record by 
 * calling the EPICS dbProcess function.  If the record is already being 
 * processed when the watchdog timer expires then re-start the timer and try 
 * again in a little while...
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
static void timerCallback   
(
    struct callback    *pcallback    /* (in)  Ptr to callback struct.*/
)
{
    ASSEMBLY_CONTROL_RECORD *par;          /* Ptr to ass. rec.         */
    ASS_CONTROL_PRIVATE *pPriv;            /* Ptr to assembly private. */
    int waitTime;                          /* Time to wait.            */


    /*
     * Recover record information from the callback struct.
     */

    par = (ASSEMBLY_CONTROL_RECORD *) pcallback->precord;
    pPriv = par->dpvt;

    DEBUG(DAR_MSG_MAX, "<%d> %s:timerCallback: entry%c\n", ' ');

    /*
     *  If the record is being processed by someone else when the timer
     *  expires then re-start the timer.
     */

    if(par->pact)
    {
        /*
         * Recover original callback structure.
         */

        struct callback *pcallback = NULL;

        if ( ( pcallback = ( struct callback *)pPriv->timer ) == NULL )
        {
            DEBUG(DAR_MSG_FATAL, "<%d> %s:timerCallback: timer is null%c\n",
            ' ');
            return;
        }

        waitTime = (int) (10 * vxTicksPerSecond );


        /* 
         * Check to make sure that we have a watchdog timer to use for
         * the delayed callback.   If not it is necessary to re-create one.
         */

        if( pcallback->wd_id == NULL )
        {
            if ( ( pcallback->wd_id = wdCreate() ) == NULL )
            {
                recGblRecordError (DAR_E_MALLOC, par, 
                                   __FILE__ ":no room for watchdog timer");
                return;
            }
        }

        callbackSetPriority(priorityLow, &pcallback->callback);

        if (wdStart(pcallback->wd_id, waitTime, (FUNCPTR)callbackRequest,
            (int)pcallback) == ERROR)
        {
            recGblRecordError (DAR_E_TIMER_CAN, par,
                               __FILE__ ":start watchdog timer failed");
        }
    }


    /*
     *  Otherwise the record is available.   Invoke the process function
     *  directly.
     */

    else
    {
        pPriv->timeout = TRUE;
        dbProcess((struct dbCommon *)par);
    }
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * unsetFlags
 *
 * INVOCATION:
 * void = unsetFlags (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)  EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Called in completeCommand when a command fails.
 * Clear INIT, INDX or PARK status fields as required.
 *
 * DESCRIPTION:
 * Clear INIT, INDX or PARK fields in depending on what went wrong during
 * the command that has just finished executing.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * setFlags & completeCommand
 *
 * DEFICIENCIES:
 * 1. If an interlock occurs during an assembly INIT, the INDX flag is unset
 * here, which doesn't really make sense but it is because the devices
 * lose their index during an INIT when their FLT lines are set to 1.
 * For consistency, the INDX & PARK flags are unset if an interlock occurs
 * during any command.
 * 2. Heath is set to BAD only when INDEX, PARK & INIT commands fail.
 *-
 ************************************************************************
 */

static void unsetFlags
(
    ASSEMBLY_CONTROL_RECORD *par    /* (in)  Ptr to ass rec.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt;    /* Internal control struct. */

    DEBUG(DAR_MSG_MAX, "<%d> %s:unsetFlags: entry, dir=%d\n", par->dir);


    /*
     *  Only change the flags in response to an action of some sort.
     */

    if ( par->dir == DAR_DIR_START || par->ilck )
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:unsetFlags: currCommand=%d\n",
              pPriv->currCommand);

        /*
         * If an initialization command failed then clear the initialized
         * status output field (INIT).
         */

        if ( par->init != 0 && pPriv->currCommand == DAR_MODE_INIT )
        {
            DEBUG(DAR_MSG_MAX, "<%d> %s:unsetFlags: init=0%c\n", ' ');
            par->init = 0;
            MONITOR(RECORD_INIT);
        }


        /*
         * Clear the indexed field flag (INDX) if any motion command failed
         * or if an interlock was detected during the command.  Note that 
         * this can be overridden by setting the keepIndex flag during 
         * processing if the error did not affect the reference position of 
         * the devices.
         */

        if (!pPriv->keepIndex && par->indx != 0 &&
            (pPriv->currCommand == DAR_MODE_INDEX ||
             pPriv->currCommand == DAR_MODE_MOVE ||
             pPriv->currCommand == DAR_MODE_TRACK ||
             pPriv->currCommand == DAR_MODE_PARK ||
             pPriv->currCommand == DAR_MODE_UPDATE ||
	     par->ilck))
        {
            DEBUG(DAR_MSG_MAX, "<%d> %s:unsetFlags: indx=0%c\n", ' ');
            par->indx = 0;
            MONITOR(RECORD_INDX);
        }


        /*
         * If any motion command failed or if an interlock was detected 
         * during the command, then clear the parked status field (PARK).
         */

        if (par->park != 0 &&  
            (pPriv->currCommand == DAR_MODE_INDEX ||
             pPriv->currCommand == DAR_MODE_MOVE ||
             pPriv->currCommand == DAR_MODE_TRACK ||
             pPriv->currCommand == DAR_MODE_PARK ||
             pPriv->currCommand == DAR_MODE_UPDATE ||
             par->ilck))
        {
            DEBUG(DAR_MSG_MAX, "<%d> %s:unsetFlags: park=0%c\n", ' ');
            par->park = 0;
            MONITOR(RECORD_PARK);
        }


        /*
         * If a INIT, INDEX or UPDATE command failed, and that failure resulted
         * in a loss of index, set the health to BAD. If an INIT, INDEX or UPDATE
         * command failed without losing index set the health to WARNING. If any
         * other motion command failed with a loss of index then set the health
         * to WARNING. If a TEST command fails the health should go BAD regardless.
         */

        if ( pPriv->currCommand == DAR_MODE_INIT || 
             pPriv->currCommand == DAR_MODE_INDEX ||
             pPriv->currCommand == DAR_MODE_UPDATE )
        {
            if ( pPriv->keepIndex )
            {
               par->hlth = DAR_HLTH_WARNING;
            }
            else
            {
               par->hlth = DAR_HLTH_BAD;
            }
            MONITOR(RECORD_HTH);
        }
        else if ( pPriv->currCommand == DAR_MODE_MOVE ||
                  pPriv->currCommand == DAR_MODE_PARK ||
                  pPriv->currCommand == DAR_MODE_TRACK )
        {
            if ( !pPriv->keepIndex )
            {
               par->hlth = DAR_HLTH_WARNING;
               MONITOR(RECORD_HTH);
            }
        }
        else if ( pPriv->currCommand == DAR_MODE_TEST )
        {
            par->hlth = DAR_HLTH_BAD;
            MONITOR(RECORD_HTH);
        }


        /* clear the keepIndex flag */

        if ( pPriv->keepIndex )
        {
            pPriv->keepIndex = FALSE;
        }
    }
    else
    {
        DEBUG(DAR_MSG_MAX, "<%d> %s:unsetFlags: doing nothing%c\n", ' ');
    }

}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * writeOutputLinks
 *
 * INVOCATION:
 * status = writeOutputLinks (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) EPICS database struct for this record
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Write output links for selected fields.
 *
 * DESCRIPTION:
 * Update all output links that are marked for writing by the previous
 * processing.  Insure that all output values whose values have
 * changed are marked for writing first.   A field is marked for writing
 * by using the TRIGGER() Macro to set the bitmap bit for that field in
 * the mmap field.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 *
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long writeOutputLinks
(
    ASSEMBLY_CONTROL_RECORD    *par    /* (in)  Ptr to ass rec.    */
)
{
    long status = DAR_S_SUCCESS;    /* Returned function status.    */
    long nRequest = 1;              /* Number of writes.            */

    DEBUG(DAR_MSG_MAX, "<%d> %s:writeOutputLinks: entry%c\n", ' ');


    /*
     *  If a field has been marked to have monitors raised then it had
     *  changed so make sure that the then write trigger is also set 
     *  for these fields
     */                                

    if (MONITORED(RECORD_MESS))
    {
        TRIGGER(RECORD_MESS);
    }

    if (MONITORED(RECORD_SIMM))
    {
        TRIGGER(RECORD_SIMM);
    }

    if (MONITORED(RECORD_DBUG))
    {
        TRIGGER(RECORD_DBUG);
    }

    if (MONITORED(RECORD_BUSY))
    {
        TRIGGER(RECORD_BUSY);
    }


    /*
     * And then write new values to all triggered links.
     */

    if (TRIGGERED(RECORD_MESS))
    {
        CHECKSTAT( ( status = recGblPutLinkValue (&(par->msgl), (void *) par,
        DBR_STRING, par->mess, &nRequest)) , return(status) );
    }

    if (TRIGGERED(RECORD_SIMM))
    {
        CHECKSTAT( (status = recGblPutFastLink (&(par->siml), (void *) par,
        &(par->simm))), return(status) );
    }

    if (TRIGGERED(RECORD_DBUG))
    {
        CHECKSTAT( (status = recGblPutFastLink (&(par->dbgl), (void *) par,
        &(par->dbug))), return(status) );
    }

    if (TRIGGERED(RECORD_BUSY))
    {
        CHECKSTAT( (status = recGblPutFastLink (&(par->bsyl), (void *) par,
        &(par->busy))), return(status) );
    }


    /*
     * Clear all of the trigger bits so that only changes after this will
     * trigger new writes.
     */

    UNTRIGGER_ALL;

    return( status );
}

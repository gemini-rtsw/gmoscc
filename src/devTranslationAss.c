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
 * FILENAME
 * devTranslationAss.c
 *
 * PURPOSE:
 * EPICS Translation Device Support code for the assemblyControl record.
 *
 * NOTE:
 * Contains code that needs finalizing after thermal measurements are completed.
 * This code is identified with a comment containing the string "COLDTESTS".
 *
 * FUNCTION NAME(S)
 * dtaActivateDevice	Send info off to device records attached.
 * dtaCheckBus		Check whether device records attached are busy.
 * dtaAckReceived	A busy link activated.
 * dtaCheckAttributes	Check input attributes.
 * dtaDevInit		Initialize device support code.
 * dtaIndexMode		Execute INDEX mode command.
 * dtaInitDeviceSupport	Initialize record's device support code.
 * dtaInitMode		Execute INIT mode command.
 * dtaMoveMode		Execute MOVE or PARK mode command.
 * dtaStartMode		Execute mode command.
 * dtaStopDirective	Execute STOP directive to attached records.
 * dtaTestMode		Execute the TEST mode.
 * dtaTrackMode		Execute the TRACK mode.
 * dtaTranslateDir	Translate dir directive for device record.
 * dtaProcessFault	Tidy up after fault (interlock occurred)
 *
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.35  2001/02/28 17:24:13  gmos
 * Renamed gmMessageLevels.h to darMessageLevels.h.
 *
 * Revision 1.34  2001/02/26 16:30:35  gmos
 * Removed CHECK_NULL macro.
 *
 * Revision 1.33  2001/02/23 18:13:22  gmos
 * Commenting improved. Discard old commented out code (except for thermal correction code). Include a printf within FL_CHECK_NULL and GR_CHECK_NULL just in case.
 *
 * Revision 1.32  2001/02/20 12:05:08  gmos
 * COLDTESTS comments reworded to indicate their is work still to be done after the thermal measurements are finalized.
 *
 * Revision 1.31  2001/02/08 13:51:36  gmos
 * Unexpected device acknowledgement is no longer a warning. Some really long lines wrapped at column 105.
 *
 * Revision 1.30  2000/12/15 16:53:17  gmos
 * Reduced X and Y axis velocities to prevent them hitting soft limit while indexing.
 *
 * Revision 1.29  2000/12/08 09:55:17  gmos
 * Updated to use new message type scheme. Additional comments added.
 *
 * Revision 1.28  2000/11/16 13:04:07  gmos
 * Magic number checks included.
 *
 * Revision 1.27  2000/11/16 10:43:47  gmos
 * Checks on the return from assGetprivateStructure added throughout.
 *
 * Revision 1.26  2000/11/10 17:17:49  gmos
 * Increased debug level of BUSY to ERROR message
 *
 * Revision 1.25  2000/11/10 17:15:39  gmos
 * Improved clarity of device failure messages.
 *
 * Revision 1.24  2000/09/06 13:02:37  gmos
 * Added extra option for lut conversion - default is now "no lookup table
 * calculations" rather than just "no offsets added".
 *
 * Revision 1.23  2000/09/05 16:41:39  gmos
 * Modified range checking in dtaCheckAttributes ().
 *
 * Revision 1.22  2000/08/29 10:10:18  gmos
 * Added output of filtered temperature from assembly on SOV.
 *
 * Revision 1.21  2000/08/18 10:37:20  gmos
 * Fixed bug with output of temperature during checkAttributes ().
 *
 * Revision 1.20  2000/08/16 16:59:55  gmos
 * Added reset of thermal model at UPDATE command (and INIT - commented during
 * cold tests). dtaUpdateMode dset function created.
 *
 * Revision 1.19  2000/08/16 15:00:48  gmos
 * Added slection of more modes for correction. X and Y axes are now selectable
 * separately from Z axis. Also fixed bug from last revision! x-talk correction
 * now accumulates on the thermal offset properly, again.
 *
 * Revision 1.18  2000/08/16 13:34:19  gmos
 * Changed dtaCheckAtrributes () to calculate offsets always, apply selectively.
 * Improved error checking.
 *
 * Revision 1.17  2000/08/08 16:13:48  cjt
 * Moved range checking to after offset corrections. Made x-talk correction
 * cumulative to thermal correction.
 *
 * Revision 1.16  2000/08/07 10:02:00  gmos
 * Write LUT corrections to VALA, VALB and VALC
 *
 * Revision 1.15  2000/07/18 09:30:27  cjt
 * Added checking of keywords on first line of lookup tables.
 *
 * Revision 1.14  2000/07/14 08:40:02  gmos
 * Removed recGblRecordError calls when there is a file I/O rather than a record error. Translation stage will now report when it cant open a LUT file.
 *
 * Revision 1.13  2000/07/06 16:24:28  cjt
 * Fixed bug in last version!
 *
 * Revision 1.12  2000/07/06 14:54:28  cjt
 * Added temperature and thermal gradient luts for all axes. Implemented temp correction.
 * Removed gradient correction for now.
 *
 * Revision 1.11  2000/06/26 13:32:34  gmos
 * Removed logMsg from DEBUG macro altogether
 *
 * Revision 1.10  2000/06/08 10:45:57  cjt
 * Updated references to (ASSEMBLY_CONTROL_RECORD *) from "pdr" to "par".
 *
 * Revision 1.9  2000/06/02 15:22:50  gmos
 * Modified to use cyclic lookup table for cross talk. Also modified to use assAddErrorMessage to report error messages.
 *
 * Revision 1.8  2000/05/26 10:55:39  cjt
 * Added support for adjustment of detector trans position with thermal model.
 * Added support for switching off faulty temperature sensors. Minor typos fixed.
 *
 * Revision 1.7  2000/05/22 11:15:37  gmos
 * Commented out printf statements on INIT
 *
 * Revision 1.6  2000/05/18 17:00:43  gmos
 * Removed redundant devFilterAss.h devGratingAss.h and devTranslationAss.h files
 *
 * Revision 1.5  2000/05/18 09:00:29  gmos
 * Modified translation assembly to accept lookup table file
 *
 * Revision 1.4  2000/05/15 16:51:16  gmos
 * X Y and Z velocities can now all be different
 *
 * Revision 1.3  2000/05/11 15:01:59  gmos
 * Assembly name included in error messages
 *
 * Revision 1.2  2000/04/03 15:43:07  cjt
 * Version 5 merged into old version 4 files. *NEW.* files removed. Correction to half-step mode support in post focal plane device records.
 *
 * Revision 1.2  2000/03/14 17:03:29  gmos
 * rcid statement modified to eliminate compiler warnings
 *
 * Revision 1.1  2000/03/03 16:46:17  pbt
 * For use with new assembly record: added dtaProcessFault to handle interlock
 *
 * Revision 1.1  1999/12/10 14:33:43  gmos
 * Contents of assembly modules merged into gmoscc
 *
 * Revision 1.3  1999/12/02 15:46:37  gmos
 * Velocity corrected. Do not check given positions when mode is INDEX
 *
 * Revision 1.2  1999/11/24 12:17:49  gmos
 * Make sure all error messages are reported with DAR_DEBUG_NONE to ensure thet are not lost
 *
 * Revision 1.1  1999/11/18 15:42:44  gmos
 * Renamed from devTranslationControl to devTranslationAss
 *
 * Revision 1.4  1999/06/29 09:36:31  gmos
 * checkStat macro changed to CHECKSTAT
 *
 * Revision 1.3  1999/06/16 08:44:35  gmos
 * Replace DEBUG macro with DTADEBUG
 *
 * Revision 1.2  1999/06/07 16:46:42  gmos
 * Fixed bugs and updated attribute limit checking
 *
 * Revision 1.1  1999/06/07 14:01:36  gmos
 * First version committed to CVS
 *
 *
 *INDENT-ON*
 *
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

#include        <alarm.h>
#include        <callback.h>
#include        <dbDefs.h>
#include        <dbAccess.h>
#include        <dbFldTypes.h>
#include        <devSup.h>
#include        <errMdef.h>
#include        <recSup.h>
#include        <special.h>
#include        <tickLib.h>		/* Contains tickGet function used by DEBUG macro */
#include	<sysLib.h>		/* Contains sysClkRateGet.                       */
#include        <dbEvent.h>

#include        <assemblyControlRecord.h> /* Assembly record general header.             */
#include        <recAssControl.h>	/* Assembly record record support header.        */
#include	<darMessageLevels.h>     /* GMOS message level definitions.               */

#include        <gmosLutLib.h>          /* GMOS lookup table library.                    */


/*
 *  Local Defines
 */

#define DTA_LUT_FIRST_STRING    "translation"	/* The first line of the translation lookup     */
						/* table file must contain this string.         */

#define DTA_MESSAGE_LENGTH	128		/* Length of error message string               */

#define DTA_NUM_AXES		3		/* Number of axes - X, Y and Z                  */

#define DTA_VELOCITY_X		 110.0		/* X axis target velocity (microns/s)           */
#define DTA_VELOCITY_Y		  40.0		/* Y axis target velocity (microns/s)           */
#define DTA_VELOCITY_Z		1200.0		/* Z axis target velocity (microns/s)           */
#define DTA_COMMAND_TIMEOUT	  30		/* Normal command timeout in seconds.           */

/*
 * The following status codes supplement the DAR_ codes within recAssControl.h
 */

#define DTA_BAD_STRUCTURE       -401            /* Return status - file open/read/write failed. */
#define DTA_BAD_DEV_CONNECT	-402		/* Return status - device not connected.	*/
#define DTA_FILEIO_ERROR        -403            /* Return status - corrupted data structure.    */
#define DTA_LUT_ERROR           -404            /* Return status - LUT calibration failed.      */

/*
 * Mnemonics for sensor inputs.
 */

#define DTA_EXTERN_TEMP_INPUT       par->sij    /* Sensor input for the measured external stage temp  */
#define DTA_FILTERED_TEMP_INPUT_1   par->sik    /* Sensor input for the filtered external temperature */
#define DTA_FILTERED_TEMP_INPUT_2   par->sil    /* Sensor input for the inferred internal stage temp  */


/*
 *  Device support function prototypes - LOCAL
 */

static long dtaAckReceived( ASSEMBLY_CONTROL_RECORD * );
static long dtaCheckAttributes( ASSEMBLY_CONTROL_RECORD * );
static long dtaIndexMode (ASSEMBLY_CONTROL_RECORD * );
static long dtaInitDeviceSupport( ASSEMBLY_CONTROL_RECORD * );
static long dtaInitMode (ASSEMBLY_CONTROL_RECORD * );
static long dtaMoveMode (ASSEMBLY_CONTROL_RECORD * );
static long dtaStopDirective( ASSEMBLY_CONTROL_RECORD * );
static long dtaTestMode (ASSEMBLY_CONTROL_RECORD * );
static long dtaTrackMode (ASSEMBLY_CONTROL_RECORD * );
static long dtaUpdateMode (ASSEMBLY_CONTROL_RECORD * );
static long dtaProcessFault (ASSEMBLY_CONTROL_RECORD * );


/*
 * Internal function prototypes - LOCAL
 */
  
static long dtaActivateDevice( ASSEMBLY_CONTROL_RECORD * );
static long dtaCheckBus( ASSEMBLY_CONTROL_RECORD *, const int, int );
static long dtaStartMode (ASSEMBLY_CONTROL_RECORD * , char * );
static long dtaTranslateDir( unsigned short );
static long dtaReadConfig(ASSEMBLY_CONTROL_RECORD *par);


/*
 *  Data Structures - GLOBAL
 */

/*
 *  Device support entry table.
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
    DEVSUPFUN       processFault;        	 /* Added 1/3/00 PBT */
}  devTranslationAssembly = {
    14,						/* Number                        */
    NULL,					/* Report function               */
    NULL,					/* Initialise                    */
    dtaInitDeviceSupport,			/* Initialise record             */
    NULL,					/* Get I/O interrupt information */
						/* User defined functions:       */
    dtaCheckAttributes,				/*    Check attributes           */
    dtaStopDirective,				/*    Process STOP directive     */
    dtaInitMode,				/*    Process INIT mode          */
    dtaMoveMode,				/*    Process MOVE and PARK modes*/
    dtaTrackMode,				/*    Process TRACK mode         */
    dtaIndexMode,				/*    Process INDEX mode         */
    dtaTestMode,				/*    Process TEST mode          */
    dtaAckReceived,				/*    Acknowledgement received   */
    dtaUpdateMode,				/*    Process UPDATE mode        */
    dtaProcessFault                             /*    Process interlock.         */
    };

/*
 *  Internal private device support structure.
 */

typedef struct devConfig {
     int                thermAdjustXY;         /* Use thermal model to adjust XY positions? */
     int                thermAdjustZ;          /* Use thermal model to adjust Z position?   */
     int                xtalkAdjustXY;         /* Use x-talk model to adjust XY positions?  */
     int                xtalkAdjustZ;          /* Use x-talk model to adjust Z position?    */
     int                calculateNoOffsets;    /* Flag to switch off all lookup table usage */
     double             zeroOffsetTemperature; /* Temperature at which zero offset should be calculated */
     long               magic;                 /* Magic value to guard against pointer corruption */
} DTA_DEV_CONFIG;

typedef struct {
    double		position[DTA_NUM_AXES];	/* position value for each axis	*/
    double		velocity[DTA_NUM_AXES];	/* velocity value for each axis	*/
    int			busyMode;		/* Current busy mode.		*/
    int			dta1Finished;		/* X axis finished.		*/
    int			dta2Finished;		/* Y axis finished.		*/
    int			dta3Finished;		/* Z axis finished.		*/
    int			cmdActive;		/* Command is activated.	*/
    SEM_ID              mutexSem;		/* mutual exclusion semaphore 	*/
    ASSEMBLY_CONTROL_RECORD   
    			*pRecord;		/* calling record structure 	*/
    DTA_DEV_CONFIG      *pTranslationPriv;      /* Private translation state struct */
    gmosLookupTable     xToDeltaYLut;		/* Delta Y as a function of X   */
    gmosLookupTable     xToDeltaZLut;		/* Delta Z as a function of X   */
    gmosLookupTable     yToDeltaXLut;		/* Delta X as a function of Y   */
    gmosLookupTable     yToDeltaZLut;		/* Delta Z as a function of Y   */
    gmosLookupTable     zToDeltaXLut;		/* Delta X as a function of Z   */
    gmosLookupTable     zToDeltaYLut;		/* Delta X as a function of Z   */
    gmosLookupTable     gToDeltaXLut;		/* Delta X as a function of G   */
    gmosLookupTable     gToDeltaYLut;		/* Delta Y as a function of G   */
    gmosLookupTable     gToDeltaZLut;		/* Delta Z as a function of G   */
    gmosLookupTable     tToDeltaXLut;		/* Delta X as a function of T   */
    gmosLookupTable     tToDeltaYLut;		/* Delta Y as a function of T   */
    gmosLookupTable     tToDeltaZLut;		/* Delta Z as a function of T   */
    long                magic;                  /* magic value to guard against pointer corruption */
} DTA_DEV_PRIVATE;

#define DTA_MAGIC  0x20647461                   /* magic value for translation data structures.    */

/*
 *  Macros
 *
 *  DEBUG   - Logs a message if the debug level of the message is less than the current
 *            debugging mode. The time and record name are prepended.
 *
 *  DTADEBUG - A translation-specific wrapper for the DEBUG macro which avoids repeating
 *            the "<%d> %s:" format throughout the code.
 */

#define DEBUG(l,FMT,V)						\
{								\
    int k=l;							\
    if (k <= par->dbug)						\
    {                                                           \
        printf ((FMT), tickGet(), par->name, (V));		\
    }								\
}

#define DTADEBUG(l,FMT,V)					\
{								\
    DEBUG (l, "<%d> %s:" FMT, V);                               \
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaActivateDevice
 *
 * INVOCATION:
 * status = dtaActivateDevice (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Send position, velocity, mode and directive to all the devices.
 *
 * DESCRIPTION:
 * This function is called when the time comes to trigger all the devices
 * connected to the assembly record.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * A fundamental assumption of this function is that all the devices
 * can be triggered simultaneously.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long dtaActivateDevice
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
    unsigned short	
    		dir;			/* Directive value.		*/
    unsigned short	
    		mode;			/* Mode value.			*/
    int		i;			/* Counter.			*/
    DTA_DEV_PRIVATE 
    		*pDtaPriv;		/* Ptr. to dev. support struc.	*/
    long	nRequest = 1;		/* Number to send on link.	*/
    unsigned short 	
    		returnedAck;		/* Returned ack from device.	*/
    long	status = DAR_S_SUCCESS;	/* Function status.		*/
    char	tmpPtr[DTA_MESSAGE_LENGTH];
    char	*svals;			/* Tmp. string ptr.		*/
    struct link	*dirLink = &par->odr1;	/* Dir's links.			*/
    struct link	*modLink = &par->mod1;	/* Mod's links.			*/
    struct link	*posLink = &par->pos1;	/* Pos's links.			*/
    struct link	*velLink = &par->vel1;	/* Vel's links.			*/
    struct link	*ackLink = &par->ack1;	/* Acks's links.		*/


    pDtaPriv =  ( DTA_DEV_PRIVATE *) assGetPrivateStruct( par );

    DTADEBUG(DAR_MSG_MAX, "dtaActivateDevice: entry, cmdActive=%d\n", 
    		pDtaPriv->cmdActive );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pDtaPriv == NULL )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaActivateDevice: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pDtaPriv->magic != DTA_MAGIC )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaActivateDevice: No magic value seen in data structure%c\n",
            ' ' );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Setup and initialize variables.
     */

    dir = dtaTranslateDir( par->dir );
    mode = par->mode;


    /*
     *  Cycle through all output links, they should be sequential.
     */

    for( i = 0 ; i < DTA_NUM_AXES; i++, modLink++, dirLink++,
	    posLink++, velLink++, ackLink++ )
    {

	/*
	 *  Skip if the position link is not connected.
	 */

	if (posLink->type == CONSTANT)
	{
	    continue;
	}


	/*
	 *  Write the positions as strings, assuming the output is connected to the VALS field
         *  of the device records.
         *  In PARK mode write the string "park" instead and convert the output mode to MOVE.
	 */


        if ( par->mode == DAR_MODE_PARK )
        {
            strcpy( tmpPtr, "park");
            svals = tmpPtr;
            mode = DAR_MODE_MOVE;
        }
        else
        {
            sprintf( tmpPtr, "%f", pDtaPriv->position[i] );
            svals = tmpPtr;
        }

	CHECKSTAT( (status = recGblPutLinkValue( posLink, (void *) par, 
		DBR_STRING, svals, &nRequest)), return(status) );
	DTADEBUG(DAR_MSG_MIN, "dtaActivateDevice: Putting POS, %s\n", 
		svals );


	/*
	 *  Write the Velocity.  WARNING, must write a double.
	 */

        CHECKSTAT( (status = recGblPutLinkValue( velLink, (void *) par,
		DBR_DOUBLE, &(pDtaPriv->velocity[i]), &nRequest)), return(status) );
	DTADEBUG(DAR_MSG_FULL, "dtaActivateDevice: Putting VEL, %f\n", 
		pDtaPriv->velocity[i]  );


	/*
	 *  Write the mode.
	 */

        CHECKSTAT( (status = recGblPutLinkValue( modLink, (void *) par, 
		DBR_SHORT, &mode, &nRequest)), return(status) );
	DTADEBUG(DAR_MSG_FULL, "dtaActivateDevice: Putting MOD, %d\n", 
		mode  );


	/*
	 *  Write the directive, translate first.
	 */

        CHECKSTAT( (status = recGblPutLinkValue( dirLink, (void *) par, 
		DBR_SHORT, (void *) (&dir), &nRequest)), return(status) );
	DTADEBUG(DAR_MSG_FULL, "dtaActivateDevice: Putting DIR, %d\n",
		    dir );

	/*
	 *  Check the response from the device.
	 */

	status = recGblGetFastLink( ackLink, (void *) par, &returnedAck);
	DTADEBUG(DAR_MSG_FULL, "dtaActivateDevice: Got response: %d\n", 
		returnedAck );

	if (returnedAck != DAR_DEV_VAL_ACCEPT )
	{
	    DTADEBUG(DAR_MSG_ERROR, "dtaActivateDevice: Translation device %d rejected command\n", 
		    i);

	    status = DAR_E_DEVICE;
            if ( i == 0 )
                assCommandFinish( par, status, "Translation X axis rejected command");
            else if ( i == 1 )
                assCommandFinish( par, status, "Translation Y axis rejected command");
            else
                assCommandFinish( par, status, "Translation Z axis rejected command");

	    pDtaPriv->cmdActive = FALSE;
	    break;
	}
	semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
	pDtaPriv->cmdActive = TRUE;
	semGive (pDtaPriv->mutexSem);

    }

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaAckReceived
 *
 * INVOCATION:
 * status = dtaAckReceived( par ); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization code.
 *
 * PURPOSE:
 * Handle change in device control record state or watchdog callback.
 *
 * DESCRIPTION:
 * This function is called when the Assembly Control Record detects
 * that an attached device control record has either started or
 * completed a command, through a change to one of the BUS1-BUS5 fields.
 * The function is also called whenever the watchdog timer expires.
 * Handling this state change is accomplished via the following algorithm:
 *
 *   if a command is active then
 *   	ignore this
 *   else if we timed out, then
 *	indicate the command finished by calling assCommandFinish(), supplying
 *	an error message.
 *   else if any of the BUSx fields have a response then
 *    	For any lines that are set, then call dtaCheckBus() to handle
 *	the change.
 *   otherwise an error
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * Assumes that BUS1=X axis, BUS2=Y axis, BUS3 = Z axis 
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None known. 
 *-
 ************************************************************************
 */

static long dtaAckReceived
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
    int         bus1;			/* Response from bus1.		*/
    int         bus2;			/* Response from bus2.		*/
    int         bus3;			/* Response from bus3.		*/
    int         bus4;			/* Response from bus4.		*/
    int         bus5;			/* Response from bus5.		*/
    DTA_DEV_PRIVATE 
    		*pDtaPriv;		/* Ptr to private dev. struct.	*/
    long	status = DAR_S_SUCCESS;	/* Function status.		*/


    pDtaPriv =  ( DTA_DEV_PRIVATE *) assGetPrivateStruct( par );

    DTADEBUG(DAR_MSG_MAX, "dtaAckReceived: entry, cmdActive=%d\n", 
    		pDtaPriv->cmdActive );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pDtaPriv == NULL )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaAckReceived: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pDtaPriv->magic != DTA_MAGIC )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaAckReceived: No magic value seen in data structure%c\n",
            ' ' );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Ask assembly control record which command states have changed.
     */

    assGetBusResponse( par, &bus1, &bus2, &bus3, &bus4, &bus5 );

    semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
    if ( pDtaPriv->cmdActive == FALSE )
    {
       /*
        * No command is active, so no change in the BUS links was expected.
        * This can typically happen when someone is using the device level
        * engineering screen or the assembly record's special processing gets
        * called for a reason other than a device response. Ignore it.
        */

	DTADEBUG(DAR_MSG_MIN, "dtaAckReceived: unexpected device response ignored%c\n",
		' ');
    }
    else if ( assCommandTimedOut( par ) )
    {
	/*
	 *  A command has timed out..
	 */

	DTADEBUG(DAR_MSG_ERROR, "dtaAckReceived: translationcommand timed out%c\n", ' ');

	status = DAR_E_TIMEDOUT;
	assCommandFinish( par, status, "Translation command timed out" );
	pDtaPriv->cmdActive = FALSE;
    }
    else if ( bus1 == TRUE || bus2 == TRUE || bus3 == TRUE  )
    {
    	/*
	 *  else response received from the device(s), handle it.
	 */

	if ( status == DAR_S_SUCCESS && bus1 == TRUE )
	{
	    /*
             *  X axis response.
             */

	    if ( par->bus1 == DAR_DEV_BUSY_IDLE )
	    {
		pDtaPriv->dta1Finished = TRUE;
	    }
	    DTADEBUG(DAR_MSG_MIN, "dtaAckReceived, X axis response (bus1)=%d\n", 
		    par->bus1 );
	    status = dtaCheckBus( par, 1, par->bus1 );
	}

	if ( status == DAR_S_SUCCESS && bus2 == TRUE )
	{
	    /*
             * Y axis response.
             */

	    if ( par->bus2 == DAR_DEV_BUSY_IDLE )
	    {
		pDtaPriv->dta2Finished = TRUE;
	    }
	    DTADEBUG(DAR_MSG_MIN, "dtaAckReceived, Y axis response (bus2)=%d\n",
		    par->bus2 );
	    status = dtaCheckBus( par, 2, par->bus2 );
	}

	if ( status == DAR_S_SUCCESS && bus3 == TRUE )
	{
	    /*
             *  Z axis response.
             */

	    if ( par->bus3 == DAR_DEV_BUSY_IDLE )
	    {
		pDtaPriv->dta3Finished = TRUE;
	    }
	    DTADEBUG(DAR_MSG_MIN, "dtaAckReceived, Z axis response (bus3)=%d\n",
		    par->bus3 );
	    status = dtaCheckBus( par, 3, par->bus3 );
	}
    }
    else
    {
    	/*
	 *  Received bus acknowledgment from something other than bus1, bus2 or bus3.
	 */

	DTADEBUG(DAR_MSG_ERROR, "dtaAckReceived, response from unexpected device%c\n", ' ');
    }

    semGive (pDtaPriv->mutexSem);
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaCheckAttributes
 *
 * INVOCATION:
 * status = dtaCheckAttributes( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * To check the attributes in the assemblyControl record
 *
 * DESCRIPTION:
 * To check the attributes in the assemblyControl record.
 * For the translation assembly this involves checking
 * that the X, Y and Z positions are within range, taking
 * into account any cross talk or thermal corrections.
 * Executed during a PRESET prior to a START.
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

static long	dtaCheckAttributes
(
    ASSEMBLY_CONTROL_RECORD
              *par			/* (in)  Ptr to assembly record structure     */
)
{
     DTA_DEV_PRIVATE *pDtaPriv;		              /* Ptr to private dev. struct.                */
     DTA_DEV_CONFIG  *pDtaConfig;	              /* Ptr to device configuration stricture.     */
     long     status;			              /* Returned function status.                  */
     char     errMsg[DTA_MESSAGE_LENGTH];             /* Error message.                             */
     double   correctX, correctY, correctZ;           /* End up being the adjusted X, Y and Z values*/
     double   deltaX, deltaY, deltaZ;	              /* Delta X, Y and Z values                    */
     double   xtalkDeltaX, xtalkDeltaY, xtalkDeltaZ;  /* Total delta X, Y and Z values              */
     double   thermDeltaX, thermDeltaY, thermDeltaZ;  /* Total delta X, Y and Z values              */
     double   temperature;		              /* Temperature for the G and T lookup tables  */
     double   filteredTemperature;		      /* Filtered temp for the T lookup table       */
     double   thermalGradient;		              /* Thermal gradient for G and T lookup tables */
     long     offsetSelection;		              /* Use XYZ lookup tables                      */
     int      i;			              /* Index variable.                            */
     long     one = 1;

     status = DAR_S_SUCCESS;

     pDtaPriv   = (DTA_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDtaConfig = (DTA_DEV_CONFIG *) pDtaPriv->pTranslationPriv;

     DTADEBUG(DAR_MSG_MAX, "dtaCheckAttributes: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pDtaPriv == NULL || pDtaConfig == NULL )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaCheckAttributes: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pDtaPriv->magic != DTA_MAGIC || pDtaConfig->magic != DTA_MAGIC )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaCheckAttributes: No magic value seen in data structure%c\n",
            ' ' );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

     /*
      * Input attributes only need to be checked if the mode is MOVE.
      */

     if ( par->mode == DAR_MODE_MOVE )
     {

	  /*
	   * Check that each of the attributes are not NULL, have DOUBLE type and are within range.
	   * It is assumed that if both the lower and upper limit parameters are zero the allowed
	   * range is unlimited.
	   */

	  /* Attribute A */

	  if ( status == DAR_S_SUCCESS )
	  {
	       if ( par->a == NULL )
	       {
		    DTADEBUG(DAR_MSG_FATAL, "dtaCheckAttributes: Attr A is NULL%c\n", ' ');
		    assAddErrorMessage (par, "Attr A is NULL");
		    status = DAR_E_ATT;
	       }
	       else if ( par->fta != DBF_DOUBLE )
	       {
		    DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: Attr A is not DOUBLE%c\n", ' ');
		    assAddErrorMessage (par, "Attr A is not DOUBLE");
		    status = DAR_E_ATT;
	       }
	  }

	  /* Attribute B */

	  if ( status == DAR_S_SUCCESS )
	  {
	       if ( par->b == NULL )
	       {
		    DTADEBUG(DAR_MSG_FATAL, "dtaCheckAttributes: Attr B is NULL%c\n", ' ');
		    assAddErrorMessage (par, "Attr B is NULL");
		    status = DAR_E_ATT;
	       }
	       else if ( par->ftb != DBF_DOUBLE )
	       {
		    DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: Attr B is not DOUBLE%c\n", ' ');
		    assAddErrorMessage (par, "Attr B is not DOUBLE");
		    status = DAR_E_ATT;
	       }
	  }

	  /* Attribute C */

	  if ( status == DAR_S_SUCCESS )
	  {
	       if ( par->c == NULL )
	       {
		    DTADEBUG(DAR_MSG_FATAL, "dtaCheckAttributes: Attr C is NULL%c\n", ' ');
		    assAddErrorMessage (par, "Attr C is NULL");
		    status = DAR_E_ATT;
	       }
	       else if ( par->ftc != DBF_DOUBLE )
	       {
		    DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: Attr C is not a DOUBLE%c\n", ' ');
		    assAddErrorMessage (par, "Attr C is not DOUBLE");
		    status = DAR_E_ATT;
	       }
	  }

	  /* Attribute D */

	  if ( status == DAR_S_SUCCESS )
	  {
	       if ( par->d == NULL )
	       {
		    DTADEBUG(DAR_MSG_FATAL, "dtaCheckAttributes: Attr D is NULL%c\n", ' ');
		    assAddErrorMessage (par, "Attr D is NULL");
		    status = DAR_E_ATT;
	       }
	       else if ( par->ftd != DBF_LONG )
	       {
		    DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: Attr D is not a LONG%c\n", ' ');
		    assAddErrorMessage (par, "Attr D is not LONG");
		    status = DAR_E_ATT;
	       }
	       else
	       {
		    offsetSelection = *(long *)par->d;

		    pDtaConfig->calculateNoOffsets = 0;
		    pDtaConfig->thermAdjustXY = 0;
		    pDtaConfig->thermAdjustZ  = 0;
		    pDtaConfig->xtalkAdjustXY = 0;
		    pDtaConfig->xtalkAdjustZ  = 0;

		    switch (offsetSelection)
		    {
		    case (0):
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: All offset calculation   OFF%c\n",
                                  ' ');
			 pDtaConfig->calculateNoOffsets = 1;
			 break;

		    case (1):
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk XY  compensation  OFF%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk Z   compensation  OFF%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal XY compensation  OFF%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal Z  compensation  OFF%c\n",
                                  ' ');
			 break;

		    case (2):
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk XY  compensation  ON%c\n",
                                 ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk Z   compensation  OFF%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal XY compensation  OFF%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal Z  compensation  OFF%c\n",
                                  ' ');
			 pDtaConfig->xtalkAdjustXY = 1;
			 break;

		    case (3):
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk XY  compensation  ON%c\n",
                                 ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk Z   compensation  ON%c\n",
                                 ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal XY compensation  OFF%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal Z  compensation  OFF%c\n",
                                  ' ');
			 pDtaConfig->xtalkAdjustXY = 1;
			 pDtaConfig->xtalkAdjustZ  = 1;
			 break;

		    case (4):
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk XY  compensation  OFF%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk Z   compensation  OFF%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal XY compensation  ON%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal Z  compensation  OFF%c\n",
                                  ' ');
			 pDtaConfig->thermAdjustXY = 1;
			 break;

		    case (5):
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk XY  compensation  OFF%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk Z   compensation  OFF%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal XY compensation  ON%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal Z  compensation  ON%c\n",
                                  ' ');
			 pDtaConfig->thermAdjustXY = 1;
			 pDtaConfig->thermAdjustZ  = 1;
			 break;

		    case (6):
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk XY  compensation  ON%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk Z   compensation  OFF%c\n",
                                 ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal XY compensation  ON%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal Z  compensation  OFF%c\n",
                                  ' ');
			 pDtaConfig->thermAdjustXY = 1;
			 pDtaConfig->xtalkAdjustXY = 1;
			 break;

		    case (7):
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk XY  compensation  ON%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: x-talk Z   compensation  ON%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal XY compensation  ON%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_MIN, "dtaCheckAttributes: Thermal Z  compensation  ON%c\n",
                                  ' ');
			 pDtaConfig->thermAdjustXY = 1;
			 pDtaConfig->thermAdjustZ  = 1;
			 pDtaConfig->xtalkAdjustXY = 1;
			 pDtaConfig->xtalkAdjustZ  = 1;
			 break;
		    default:
			 DTADEBUG(DAR_MSG_WARNING,
                                  "dtaCheckAttributes: Confused over selection of corrections%c\n",
                                  ' ');
			 DTADEBUG(DAR_MSG_WARNING,
                                  "dtaCheckAttributes: Corrections switched OFF%c\n", ' ');
			 status = DAR_E_BAD_ATTR;
			 break;
		    }
	       }
	  }

	  /*
	   * Calculate lookup table corrections to the input coordinates.
	   * Applied later if the correct flags are set.
	   */

	  /* Save the given values and initialise the total delta values. */
	  
	  correctX = *(double *) par->a;
	  correctY = *(double *) par->b;
	  correctZ = *(double *) par->c;
	  
	  DTADEBUG(DAR_MSG_FULL, "dtaCheckAttributes: Given X is %f\n", correctX);
	  DTADEBUG(DAR_MSG_FULL, "dtaCheckAttributes: Given Y is %f\n", correctY);
	  DTADEBUG(DAR_MSG_FULL, "dtaCheckAttributes: Given Z is %f\n", correctZ);
	  
	  thermDeltaX = thermDeltaY = thermDeltaZ = 0.0;
	  xtalkDeltaX = xtalkDeltaY = xtalkDeltaZ = 0.0;
	  
	  temperature         =               *(double *)DTA_EXTERN_TEMP_INPUT;
	  filteredTemperature =               *(double *)DTA_FILTERED_TEMP_INPUT_1;
	  thermalGradient     = temperature - *(double *)DTA_FILTERED_TEMP_INPUT_2;

/*
 * COLDTESTS the above temp gradient should use the filtered temperature when the correct
 * time constant is found
 */
	       
	  /*
	   * Apply the lookup tables one at a time, as long as there are at least 2 elements
	   * stored. It is assumed that the adjustment only needs to be crude, and that
	   * iterating the application of the tables is not necessary.
	   */

	  if (!pDtaConfig->calculateNoOffsets)
	  {
	       if (status == DAR_S_SUCCESS)
	       {
		    /* T to delta X table */

		    deltaX = 0.0;
		    if ( pDtaPriv->tToDeltaXLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->tToDeltaXLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply( temperature, &(pDtaPriv->tToDeltaXLut), &deltaX );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaXLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaXLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    thermDeltaX += deltaX;
	       }

	       if (status == DAR_S_SUCCESS)
	       {
		    /* T to delta Y table */

		    deltaY = 0.0;
		    if ( pDtaPriv->tToDeltaYLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->tToDeltaYLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply( temperature, &(pDtaPriv->tToDeltaYLut), &deltaY );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaYLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaYLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    thermDeltaY += deltaY;
	       }

	       if (status == DAR_S_SUCCESS)
	       {
		    /* T to delta Z table */

		    deltaZ = 0.0;
		    if ( pDtaPriv->tToDeltaZLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->tToDeltaZLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply( temperature, &(pDtaPriv->tToDeltaZLut), &deltaZ );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaZLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaZLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    thermDeltaZ += deltaZ;
	       }


	       /*
		* A set of offsets for subtraction from those just calculated.
		* These allow the offsets to be reset at a defined temperature (during UPDATE mode).
		*/

	       if (status == DAR_S_SUCCESS)
	       {
		    /* T to delta X table */

		    deltaX = 0.0;
		    if ( pDtaPriv->tToDeltaXLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->tToDeltaXLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply(pDtaConfig->zeroOffsetTemperature,
                                          &(pDtaPriv->tToDeltaXLut), &deltaX );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaXLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaXLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    thermDeltaX -= deltaX;
	       }

	       if (status == DAR_S_SUCCESS)
	       {
		    /* T to delta Y table */

		    deltaY = 0.0;
		    if ( pDtaPriv->tToDeltaYLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->tToDeltaYLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply(pDtaConfig->zeroOffsetTemperature,
                                          &(pDtaPriv->tToDeltaYLut), &deltaY );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaYLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaYLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    thermDeltaY -= deltaY;
	       }

	       if (status == DAR_S_SUCCESS)
	       {
		    /* T to delta Z table */

		    deltaZ = 0.0;
		    if ( pDtaPriv->tToDeltaZLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->tToDeltaZLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply(pDtaConfig->zeroOffsetTemperature,
                                         &(pDtaPriv->tToDeltaZLut), &deltaZ );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaZLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: tToDeltaZLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    thermDeltaZ -= deltaZ;
	       }
	  }

	  /*
	   * If everything has worked log the corrections to output fields, 
	   * if not, reset offsets to zero and log.
	   */

	  if (status != DAR_S_SUCCESS)
	  {
	       thermDeltaX = 0.0;
	       thermDeltaY = 0.0;
	       thermDeltaZ = 0.0;
	  }

	  DTADEBUG(DAR_MSG_MAX,
                   "dtaCheckAttributes: Total delta X for thermal model is %f\n", thermDeltaX);
	  DTADEBUG(DAR_MSG_MAX,
                   "dtaCheckAttributes: Total delta Y for thermal model is %f\n", thermDeltaY);
	  DTADEBUG(DAR_MSG_MAX,
                   "dtaCheckAttributes: Total delta Z for thermal model is %f\n", thermDeltaY);
		    
/* COLDTESTS - the following six lines can be removed after thermal model is finalized */

	  *(double *) par->vala = thermDeltaX;
	  *(double *) par->valb = thermDeltaY;
	  *(double *) par->valc = thermDeltaZ;
	  MONITOR_VALA
	  MONITOR_VALB
	  MONITOR_VALC

/* COLDTESTS - the following line can be removed after thermal model is finalized  */

	  recGblPutLinkValue (&(par->sou), (void *) par, DBR_DOUBLE, &(temperature), &one);
	  recGblPutLinkValue (&(par->sov), (void *) par, DBR_DOUBLE, &(filteredTemperature), &one);

	  /* Now apply the corrections if required to do so */

	  if (pDtaConfig->thermAdjustXY)
	  {		    
	       correctX += thermDeltaX;
	       correctY += thermDeltaY;
	  }

	  if (pDtaConfig->thermAdjustZ)
	  {		    
	       correctZ += thermDeltaZ;
	  }

	  /* Now the cross-talk corrections */

	  if (!pDtaConfig->calculateNoOffsets)
	  {
	       if (status == DAR_S_SUCCESS)
	       {
		    /* X to delta Y table */

		    deltaY = 0.0;
		    if ( pDtaPriv->xToDeltaYLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->xToDeltaYLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply( correctX, &(pDtaPriv->xToDeltaYLut), &deltaY );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: xToDeltaYLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: xToDeltaYLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    xtalkDeltaY += deltaY;
	       }

	       if (status == DAR_S_SUCCESS)
	       {
		    /* X to delta Z table */

		    deltaZ = 0.0;
		    if ( pDtaPriv->xToDeltaZLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->xToDeltaZLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply( correctX, &(pDtaPriv->xToDeltaZLut), &deltaZ );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: xToDeltaZLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: xToDeltaZLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    xtalkDeltaZ += deltaZ;
	       }

	       if (status == DAR_S_SUCCESS)
	       {
		    /* Y to delta X table */

		    deltaX = 0.0;
		    if ( pDtaPriv->yToDeltaXLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->yToDeltaXLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply( correctY, &(pDtaPriv->yToDeltaXLut), &deltaX );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: yToDeltaXLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: yToDeltaXLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    xtalkDeltaX += deltaX;
	       }

	       if (status == DAR_S_SUCCESS)
	       {
		    /* Y to delta Z table */

		    deltaZ = 0.0;
		    if ( pDtaPriv->yToDeltaZLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->yToDeltaZLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply( correctY, &(pDtaPriv->yToDeltaZLut), &deltaZ );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: yToDeltaZLut: %s\n",
                                      errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: yToDeltaZLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    xtalkDeltaZ += deltaZ;
	       }

	       if (status == DAR_S_SUCCESS)
	       {
		    /* Z to delta X table */

		    deltaX = 0.0;
		    if ( pDtaPriv->zToDeltaXLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->zToDeltaXLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply( correctZ, &(pDtaPriv->zToDeltaXLut), &deltaX );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: zToDeltaXLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: zToDeltaXLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    xtalkDeltaX += deltaX;
	       }

	       if (status == DAR_S_SUCCESS)
	       {
		    /* Z to delta Y table */

		    deltaY = 0.0;
		    if ( pDtaPriv->zToDeltaYLut.numberOfEntries >= 2 )
		    {

			 /* Check the LUT */

			 status = gmosLutCheck( &(pDtaPriv->zToDeltaYLut) );
			 if ( status == GMOSLUT_S_OK )
			 {

			      /* Translate via the lut */

			      status = gmosLutApply( correctZ, &(pDtaPriv->zToDeltaYLut), &deltaY );
			      if ( status != GMOSLUT_S_OK )
			      {
				   gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
				   DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: zToDeltaYLut: %s\n",
                                      errMsg);
				   assAddErrorMessage (par, errMsg);
				   status = DTA_LUT_ERROR;
			      }
			 }
			 else
			 {
			      gmosLutMessage( status, DTA_MESSAGE_LENGTH, errMsg );
			      DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: zToDeltaYLut: %s\n",
				    errMsg);
			      assAddErrorMessage (par, errMsg);
			      status = DTA_LUT_ERROR;
			 }

		    }
		    xtalkDeltaY += deltaY;
	       }
	  }

	  /*
	   * If everything has worked log the corrections to output fields, 
	   * if not, reset offsets to zero and log.
	   */

	  if (status != DAR_S_SUCCESS)
	  {
	       xtalkDeltaX = 0.0;
	       xtalkDeltaY = 0.0;
	       xtalkDeltaZ = 0.0;
	  }

	  DTADEBUG(DAR_MSG_MAX, "dtaCheckAttributes: Total delta X for x-talk is %f\n", xtalkDeltaX);
	  DTADEBUG(DAR_MSG_MAX, "dtaCheckAttributes: Total delta Y for x-talk is %f\n", xtalkDeltaY);
	  DTADEBUG(DAR_MSG_MAX, "dtaCheckAttributes: Total delta Z for x-talk is %f\n", xtalkDeltaY);
		    
/* COLDTESTS - the following two lines can be removed after thermal model is finalized  */

	  recGblPutLinkValue (&(par->sor), (void *) par, DBR_DOUBLE, &(xtalkDeltaX), &one);
	  recGblPutLinkValue (&(par->sos), (void *) par, DBR_DOUBLE, &(xtalkDeltaY), &one);

	  /* Now apply the corrections if required to do so */

	  if (pDtaConfig->xtalkAdjustXY)
	  {
	       correctX += xtalkDeltaX;
	       correctY += xtalkDeltaY;
	  }

	  if (pDtaConfig->xtalkAdjustZ)
	  {
	       correctZ += xtalkDeltaZ;
	  }

	  /*
	   * Finally, check that the applied corrections have not shifted the XYZ coordinates
	   * outside the allowed range. Reject the values if they have.
	   */

	  if ( correctX < (double)par->aall && ((double)par->aall != 0.0 || (double)par->aahl != 0.0))
	  {
	       DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: Corrected X < %f\n", (double)par->aall);
	       sprintf (errMsg, "Corrected X (%10f) < %10f", correctX, (double)par->aall );
	       assAddErrorMessage (par, errMsg);
	       status = DAR_E_BAD_ATTR;
	  }
	  else if ( correctX > (double)par->aahl && ((double)par->aall != 0.0 || (double)par->aahl != 0.0))
	  {
	       DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: Corrected X > %f\n", (double)par->aahl);
	       sprintf (errMsg, "Corrected X (%10f) > %10f", correctX, (double)par->aahl );
	       assAddErrorMessage (par, errMsg);
	       status = DAR_E_BAD_ATTR;
	  }

	  if ( correctY < (double)par->abll && ((double)par->abll != 0.0 || (double)par->abhl != 0.0))
	  {
	       DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: Corrected Y < %f\n", (double)par->abll);
	       sprintf (errMsg, "Corrected Y (%10f) < %10f", correctY, (double)par->abll );
	       assAddErrorMessage (par, errMsg);
	       status = DAR_E_BAD_ATTR;
	  }
	  else if ( correctY > (double)par->abhl && ((double)par->abll != 0.0 || (double)par->abhl != 0.0))
	  {
	       DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: Corrected Y > %f\n", (double)par->abhl);
	       sprintf (errMsg, "Corrected Y (%10f) > %10f", correctY, (double)par->abhl );
	       assAddErrorMessage (par, errMsg);
	       status = DAR_E_BAD_ATTR;
	  }

	  if ( correctZ < (double)par->acll && ((double)par->acll != 0.0 || (double)par->achl != 0.0))
	  {
	       DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: Corrected Z < %f\n", (double)par->acll);
	       sprintf (errMsg, "Corrected Z (%10f) < %10f", correctZ, (double)par->acll );
	       assAddErrorMessage (par, errMsg);
	       status = DAR_E_BAD_ATTR;
	  }
	  else if ( correctZ > (double)par->achl && ((double)par->acll != 0.0 || (double)par->achl != 0.0))
	  {
	       DTADEBUG(DAR_MSG_ERROR, "dtaCheckAttributes: Corrected Z > %f\n", (double)par->achl);
	       sprintf (errMsg, "Corrected Z (%10f) > %10f", correctZ, (double)par->achl );
	       assAddErrorMessage (par, errMsg);
	       status = DAR_E_BAD_ATTR;
	  }

	  if ((status == DAR_S_SUCCESS))
	  {
	       /* Write the positions and velocities to the device records */
	     
	       DTADEBUG(DAR_MSG_FULL, "dtaCheckAttributes: Moving X to %f\n", correctX);
	       DTADEBUG(DAR_MSG_FULL, "dtaCheckAttributes: Moving Y to %f\n", correctY);
	       DTADEBUG(DAR_MSG_FULL, "dtaCheckAttributes: Moving Z to %f\n", correctZ);

	       pDtaPriv->position[0] = correctX;
	       pDtaPriv->position[1] = correctY;
	       pDtaPriv->position[2] = correctZ;
	       pDtaPriv->velocity[0] = DTA_VELOCITY_X;
	       pDtaPriv->velocity[1] = DTA_VELOCITY_Y;
	       pDtaPriv->velocity[2] = DTA_VELOCITY_Z;
	  }
     }
     else if ( par->mode == DAR_MODE_TRACK )
     {
	  /*
	   * TRACK mode is not implemented.
	   */

	  DTADEBUG(DAR_MSG_ERROR,
		   "dtaCheckAttributes: TRACK mode not implemented%c\n", ' ');
	  assAddErrorMessage (par, "TRACK mode not implemented");
	  status = DAR_E_MODE;
     }
/*     else if ( par->mode == DAR_MODE_UPDATE )
       {
*/	  /*
	   * UPDATE mode is not appropriate for the detector translation mechanism.
	   */

          /* COMMENTED OUT - In UPDATE mode is now used to reset the temperature calibration */

/*	  DTADEBUG(DAR_MSG_ERROR,
	  "dtaCheckAttributes: UPDATE mode not appropriate for this assembly%c\n", ' ');
          assAddErrorMessage (par, "UPDATE mode not appropriate");
	  status = DAR_E_MODE;
	  }
*/     else
 {
      /*
       * In other modes set the positions to zero and the velocity to a suitable value.
       */

      for ( i=0; i<DTA_NUM_AXES; i++ )
      {
	   pDtaPriv->position[i] = 0.0;
      }
      pDtaPriv->velocity[0] = DTA_VELOCITY_X;
      pDtaPriv->velocity[1] = DTA_VELOCITY_Y;
      pDtaPriv->velocity[2] = DTA_VELOCITY_Z;
 }

     return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaCheckBus
 *
 * INVOCATION:
 * status = dtaCheckBus( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 * (>) devnum (int)  Device number
 * (>) busVal (int)  Value of the bus change.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * To check the change in a busy (BUS) field
 *
 * DESCRIPTION:
 *  Check what kind of busy response we received.
 *    if ERROR then
 *	The command failed on that device.
 *	Stop the timer, set an error message,
 *	Set the cmdFinished flag to TRUE and cmdActive flag to FALSE.
 *    else if BUSY then
 *	Just ignore this change. The device hasn't finished yet.
 *    else if IDLE then
 *	That means something completed on that device so:
 *	If this is the last device then
 *        Stop the timer
 *        Set the cmdActive flag to false
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * The function assumes there are three connected devices.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 *-
 ************************************************************************
 */

static long	dtaCheckBus
(
    ASSEMBLY_CONTROL_RECORD 
    			*par,		/* (in)  Ptr to assembly record structure */
    const int           devnum,         /* (in)  Device number          */
    int			busVal		/* (in)  Bus value.		*/
)
{
    DTA_DEV_PRIVATE 
    		*pDtaPriv;		/* Ptr to private dev. struct.	*/
    long	status = DAR_S_SUCCESS;	/* Function status.		*/
    int		totalFinished;		/* Num finished devices.	*/


    pDtaPriv =  ( DTA_DEV_PRIVATE *) assGetPrivateStruct( par );

    DTADEBUG(DAR_MSG_MAX, "dtaCheckBus: entry, bus=%d\n", busVal);

    /*
     * Trap any problem with the internal data structures
     */

     if ( pDtaPriv == NULL )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaCheckBus: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pDtaPriv->magic != DTA_MAGIC )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaCheckBus: No magic value seen in data structure%c\n", ' ' );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Handle the busy field value change.
     */

    switch( busVal )
    {
	case DAR_DEV_BUSY_ERROR:
	    /*
	     * Stop the timer, set error status, indicated that
	     * the command has finished.
	     */

	    DTADEBUG(DAR_MSG_FULL, "dtaCheckBus, BUSY to ERROR transition detected%c\n", 
		    ' ');
	    assStopTimer( par, &status );

	    status = DAR_E_DEVICE;
            if ( devnum == 1 )
	       assCommandFinish( par, status, "Translation X axis failed" );
            else if ( devnum == 2 )
	       assCommandFinish( par, status, "Translation Y axis failed" );
            else
	       assCommandFinish( par, status, "Translation Z axis failed" );

	    pDtaPriv->cmdActive = FALSE;
	    break;

	case DAR_DEV_BUSY_BUSY:
	    /*
	     *  Ignore the busy change.
	     */

	    DTADEBUG(DAR_MSG_FULL, "dtaCheckBus, BUSY to BUSY transition detected - ignored%c\n", 
		    ' ');
	    break;

	case DAR_DEV_BUSY_IDLE:
	    /*
	     *  Check to see if all devices are finished, 
	     *  and if so then the command is finished, so stop the timer
	     *  set the bus flags to false.
	     */

	    totalFinished = pDtaPriv->dta1Finished + pDtaPriv->dta2Finished +
	    		pDtaPriv->dta3Finished;

	    DTADEBUG(DAR_MSG_FULL, "dtaCheckBus, BUSY to IDLE transition detected%c\n", ' ');

            if ( totalFinished >= par->nmdv )
	    {
		/*
		 *  All devices have completed.
		 * This is the end of the command, so say command is
		 *  finished to the record code, and command not active
		 *  to the device code.
		 */

		DTADEBUG(DAR_MSG_FULL, "dtaCheckBus, all devices have finished%c\n", 
		    ' ');

		assStopTimer( par, &status );
		assCommandFinish( par, status, NULL );
		pDtaPriv->cmdActive = FALSE;
	    }
	    else
	    {
		DTADEBUG(DAR_MSG_MAX, "dtaCheckBus, not all devices finished yet%c\n", 
		    ' ');
	    }
	    break;

	default:
	    DTADEBUG(DAR_MSG_ERROR, "dtaCheckBus, invalid device acknowledgement: %d\n",
               busVal );
	    assStopTimer( par, &status );

	    status = DAR_E_DEVICE;
	    assCommandFinish( par, status, "Translation, invalid device busy field");
	    pDtaPriv->cmdActive = FALSE;
	    break;
    }
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaIndexMode
 *
 * INVOCATION:
 * status = dtaIndexMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Execute a MODE of INDEX
 *
 * DESCRIPTION:
 * Execute a MODE of INDEX by calling dtaStartMode().
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

static long dtaIndexMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
    long 	status = DAR_S_SUCCESS;

    DTADEBUG(DAR_MSG_MAX, "dtaIndexMode: entry%c\n", ' ');

    status = dtaStartMode( par, "index" );
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaInitDeviceSupport
 *
 * INVOCATION:
 * status = dtaInitDeviceSupport (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization code.
 *
 * PURPOSE:
 * Initialize device support-specific functions
 *
 * DESCRIPTION:
 * Create device support private structure and initialize values.
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

static long dtaInitDeviceSupport
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		 /* (in)  Ptr to ass. rec.	*/
)
{
    long status = DAR_S_SUCCESS;         /* Returned function status.	*/
    DTA_DEV_PRIVATE *pDtaPriv;	         /* Private dev. struct.	*/
    DTA_DEV_CONFIG  *pDtaConfig;         /* Private Config structure    */
    int i;

    DTADEBUG(DAR_MSG_MAX, "dtaInitDeviceSupport: entry%c\n", ' ');


    /*
     *  Create private device support structure for this record.
     */

    pDtaPriv = malloc (sizeof(DTA_DEV_PRIVATE));
    if ( pDtaPriv == NULL ) 
    {
	DTADEBUG(DAR_MSG_FATAL, "dtaInitDeviceSupport: malloc for DTA_DEV_PRIVATE failed.%c\n", 
		' ');

	status = DAR_E_MALLOC;
        recGblRecordError (status, par, __FILE__ ":no room for device private");
        return( status );
    }


    /*
     *  Link this structure to the record support private structure.
     */
     
    assAttachPrivateStruct ( par, (void *) pDtaPriv ); 


    /*
     * Create the MUTEX semaphore to protect the private structure
     * during asynchronous callback access.
     */
     
    pDtaPriv->mutexSem = NULL;
    if (  (pDtaPriv->mutexSem = 
	    semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE) ) == NULL )
    {
	DTADEBUG(DAR_MSG_FATAL, "dtaInitDeviceSupport: semMCreate for pDtaPriv->mutexSem failed.%c\n",
		 ' ' );

        status = DAR_E_MALLOC;
        recGblRecordError (status, par, __FILE__ ":no room for mutex creation");
        return( status );
    }

     /*
      *  Create a configuration structure for the assembly and write the pointer
      *  to the private device support structure.
      */

     pDtaConfig = (DTA_DEV_CONFIG *) malloc (sizeof(DTA_DEV_CONFIG));

     if ( pDtaConfig == NULL ) 
     {
	  DTADEBUG(DAR_MSG_FATAL, "dtaInitDeviceSupport: malloc for DTA_DEV_CONFIG failed.%c\n",
                   ' ');

	  status = DAR_E_MALLOC;
	  recGblRecordError (status, par, __FILE__ ":no room for device config");
	  return( status );
     }
     pDtaPriv->pTranslationPriv = pDtaConfig;

     pDtaConfig->magic = DTA_MAGIC;

    /*
     * Save record, callback and structure pointers
     */  

    semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
     
    pDtaPriv->pRecord = par;

    for ( i=0; i<DTA_NUM_AXES; i++ )
        pDtaPriv->position[i] = 0.0;

    pDtaPriv->velocity[0] = DTA_VELOCITY_X;
    pDtaPriv->velocity[1] = DTA_VELOCITY_Y;
    pDtaPriv->velocity[2] = DTA_VELOCITY_Z;
    pDtaPriv->cmdActive = FALSE; 
    pDtaPriv->dta1Finished = FALSE; 
    pDtaPriv->dta2Finished = FALSE; 
    pDtaPriv->dta3Finished = FALSE;
    pDtaPriv->magic        = DTA_MAGIC;

    semGive (pDtaPriv->mutexSem);

    /*
     * Initialise the correction factors output to zero.
     */

    *(double *) par->vala = 0.0;
    *(double *) par->valb = 0.0;
    *(double *) par->valc = 0.0;
    MONITOR_VALA;
    MONITOR_VALB;
    MONITOR_VALC;

    DTADEBUG(DAR_MSG_FULL, "dtaInitDeviceSupport: finished, status=%ld\n",  
	    status );

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaInitMode
 *
 * INVOCATION:
 * status = dtaInitMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Execute a MODE of INIT
 *
 * DESCRIPTION:
 * Execute a MODE of INIT by calling dtaStartMode().
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

static long dtaInitMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
     DTA_DEV_PRIVATE *pDtaPriv;		/* Ptr to private dev. struct.                */
     DTA_DEV_CONFIG  *pDtaConfig;	/* Ptr to device configuration stricture.     */
     long status = DAR_S_SUCCESS;	/* Returned function status.                  */
     long one = 1;

     DTADEBUG(DAR_MSG_MAX, "dtaInitMode: entry%c\n", ' ');

     pDtaPriv   = (DTA_DEV_PRIVATE *) assGetPrivateStruct(par);
     pDtaConfig = (DTA_DEV_CONFIG *) pDtaPriv->pTranslationPriv;

    /*
     * Trap any problem with the internal data structures
     */

     if ( pDtaPriv == NULL || pDtaConfig == NULL )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaInitMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pDtaPriv->magic != DTA_MAGIC || pDtaConfig->magic != DTA_MAGIC )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaInitMode: No magic value seen in data structure%c\n",
            ' ' );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }


/* COLDTESTS - the following line can be UNCOMMENTED after the thermal model is finalized  */

/*     pDtaConfig->zeroOffsetTemperature = *(double *)DTA_EXTERN_TEMP_INPUT; */

/* COLDTESTS - the above temperature should be the filtered one when the correct time constant is found */

/* COLDTESTS - the following two lines can be removed after the GMOS thermal model is finalized  */

     pDtaConfig->zeroOffsetTemperature = 0.0;
     recGblPutLinkValue (&(par->sot), (void *) par, DBR_DOUBLE,
                         &(pDtaConfig->zeroOffsetTemperature), &one);
     
     /*
      * Read the translation configuration file.
      */

     if ((status = dtaReadConfig(par)) != DAR_S_SUCCESS)
     {
	  return(status);
     }

     status = dtaStartMode(par, "init");

     return(status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaMoveMode
 *
 * INVOCATION:
 * status = dtaMoveMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization code.
 *
 * PURPOSE:
 * Execute a MODE of MOVE or PARK
 *
 * DESCRIPTION:
 * Execute a MODE of MOVE or PARK by calling dtaStartMode().
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

static long dtaMoveMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
    long 	status = DAR_S_SUCCESS;	/* Return function status.	*/

    DTADEBUG(DAR_MSG_MAX, "dtaMoveMode: entry. Mode = %d\n", par->mode);

    status = dtaStartMode( par, "move" );
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaStartMode
 *
 * INVOCATION:
 * status = dtaStartMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute the requested MODE.
 *
 * DESCRIPTION:
 * Send the modem, directive, position and velocity to the attached
 * devices by calling dtaActivateDevice(). If all the devices
 * accept their commands then start the watchdog timer.
 *
 * The assembly record will sleep after calling this function, and will
 * be woken either by a change in device status or by the watchdog timer
 * expiring. (See dtaAckReceived).
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

static long dtaStartMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*par,		/* (in)  Ptr to ass. rec.	*/
    char		*mode		/* (in)	 Mode to execute	*/
)
{
    DTA_DEV_PRIVATE 
    		*pDtaPriv;		/* Ptr to dev. support struc.	*/
    long 	status = DAR_S_SUCCESS;	/* Return function status.	*/


    pDtaPriv =  ( DTA_DEV_PRIVATE *) assGetPrivateStruct( par );

    DTADEBUG(DAR_MSG_MAX, "dtaStartMode: entry, mode=%s\n", mode );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pDtaPriv == NULL )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaStartMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pDtaPriv->magic != DTA_MAGIC )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaStartMode: No magic value seen in data structure%c\n",
            ' ' );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Activate the attached deviceControl records and start the command timer.
     */

    semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
    pDtaPriv->dta1Finished = FALSE;
    pDtaPriv->dta2Finished = FALSE;
    pDtaPriv->dta3Finished = FALSE;
    semGive (pDtaPriv->mutexSem);

    status = dtaActivateDevice( par );
    if ( status == DAR_S_SUCCESS )
    {
	assStartTimer( par, &status, DTA_COMMAND_TIMEOUT );
    }
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaStopDirective
 *
 * INVOCATION:
 * status = dtaStopDirective (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute a STOP directive
 *
 * DESCRIPTION:
 * Execute a STOP directive. Sends a STOP to the attached devices
 * and starts a watchdog timer. After calling this function the
 * assembly record will sleep and wake up when the devices have
 * stopped or the watchdog timer has expired.
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

static long dtaStopDirective
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
    struct link	*dirLink;		/* Dir's link.			*/
    long 	status = DAR_S_SUCCESS;	/* Return function status.	*/
					/* Private rec. support struc.	*/
    DTA_DEV_PRIVATE 
    		*pDtaPriv;		/* Private dev. support struc.	*/
    unsigned short	
    		dir;			/* Directive to send.		*/
    long	nRequest = 1;		/* Number of requested.		*/


    pDtaPriv =  ( DTA_DEV_PRIVATE *) assGetPrivateStruct( par );

    DTADEBUG(DAR_MSG_MAX, "dtaStopDirective: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pDtaPriv == NULL )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaStopDirective: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pDtaPriv->magic != DTA_MAGIC )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaStopDirective: No magic value seen in data structure%c\n",
            ' ' );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Get the current device record's busy state.  If already busy, then
     *  the next change will be to idle and may happen as soon as we
     *  issue a stop, , so be ready.
     *  First set the cmdActive flag (if BUSY is BUSY),
     *  Translate the directive and put that value out on the DIR link.
     */

    pDtaPriv->busyMode = par->bus1;
    pDtaPriv->cmdActive = ( pDtaPriv->busyMode == DAR_DEV_BUSY_IDLE )? FALSE : 
    		TRUE;

    dir = dtaTranslateDir( par->dir );

    DTADEBUG(DAR_MSG_FULL, "dtaStopDirective: writing to X axis%c\n",
       ' ');

    dirLink = &par->odr1;
    CHECKSTAT( (status = recGblPutLinkValue( dirLink, (void *) par, 
	    DBR_SHORT, (void *) (&dir), &nRequest)) , return(status) );

    if ( par->nmdv >= 2 )
    {
	/*
	 * We have 2 or more devices attached.
	 */

        DTADEBUG(DAR_MSG_FULL, "dtaStopDirective: writing to Y axis%c\n",
           ' ');

	dirLink = &par->odr2;
	CHECKSTAT( (status = recGblPutLinkValue( dirLink, (void *) par, 
		DBR_SHORT, (void *) (&dir), &nRequest)) , return(status) );
    }

    if ( par->nmdv == 3 )
    {
	/*
	 * Assume we have 3 devices attached.
	 */

        DTADEBUG(DAR_MSG_FULL, "dtaStopDirective: writing to Z axis%c\n",
           ' ');

	dirLink = &par->odr3;
	CHECKSTAT( (status = recGblPutLinkValue( dirLink, (void *) par, 
		DBR_SHORT, (void *) (&dir), &nRequest)) , return(status) );
    }


    if ( pDtaPriv->busyMode == DAR_DEV_BUSY_IDLE )
    {
	/*
	 *  Device record is currently idle, so we will want to start 
	 *  the timer as the device record will go busy then back to
	 *  idle or err.
	 */

	pDtaPriv->cmdActive = TRUE;
	assStartTimer( par, &status, DTA_COMMAND_TIMEOUT );
	DTADEBUG(DAR_MSG_FULL, "dtaStopDirective: starting timer%c\n", ' ');
    }
    else
    {
	DTADEBUG(DAR_MSG_FULL, "dtaStopDirective: device was already busy%c\n", ' ');
    }
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaTestMode
 *
 * INVOCATION:
 * status = dtaTestMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute a MODE of TEST
 *
 * DESCRIPTION:
 * Execute a MODE of TEST by calling dtaStartMode().
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

static long dtaTestMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
    long 	status = DAR_S_SUCCESS;	/* Returned function status.	*/
    DTA_DEV_PRIVATE *pDtaPriv;		/* Ptr to private dev. struct.                */


    DTADEBUG(DAR_MSG_MAX, "dtaTestMode: entry%c\n", ' ');

    /*
     * Check the integrity of the device private structures
     */

    pDtaPriv   = (DTA_DEV_PRIVATE *) assGetPrivateStruct(par);

    if ( pDtaPriv == NULL )
    {
        DTADEBUG(DAR_MSG_FATAL, "dtaTestMode: Bad device private data structure%c\n", ' ' );
        assDisplayPrivateStruct( par );

        status = DTA_BAD_STRUCTURE;
        assCommandFinish( par, status, "Bad internal data structures");
        return (status);
    }
    else if ( pDtaPriv->magic != DTA_MAGIC )
    {
        DTADEBUG(DAR_MSG_FATAL, "dtaTestMode: No magic value seen in data structure%c\n",
          ' ' );

        status = DTA_BAD_STRUCTURE;
        assCommandFinish( par, status, "no magic value seen in data structure");
        return (status);
    }
    else if ( par->dbug >= DAR_MSG_MAX )
    {
        /* Display the private structure in MAX debug mode. */

        assDisplayPrivateStruct( par );
    }


    /*
     *  Check that all position, mode, directive, ack links
     *  are NOT CONSTANT.
     */

    if ( par->pos1.type == CONSTANT || par->ack1.type == CONSTANT || 
    	par->mod1.type == CONSTANT || par->odr1.type == CONSTANT ||
	par->vel1.type == CONSTANT )
    {
	/*
	 *  Check the X axis links.
	 */

	DTADEBUG(DAR_MSG_ERROR, "dtaTestMode: X axis links not connected.%c\n", ' ' );

        status = DTA_BAD_DEV_CONNECT;
	assCommandFinish( par, status, "X axis links not connected");
        return (status);
    }
    else if ( par->pos2.type == CONSTANT || par->ack2.type == CONSTANT || 
    	par->mod2.type == CONSTANT || par->odr2.type == CONSTANT ||
	par->vel2.type == CONSTANT )
    {
	/*
	 *  Check the Y axis links.
	 */

	DTADEBUG(DAR_MSG_ERROR, "dtaTestMode: Y axis links not connected.%c\n", ' ' );

        status = DTA_BAD_DEV_CONNECT;
	assCommandFinish( par, status, "Y axis links not connected");
    }
    else if ( par->pos3.type == CONSTANT || par->ack3.type == CONSTANT || 
    	par->mod3.type == CONSTANT || par->odr3.type == CONSTANT ||
	par->vel3.type == CONSTANT )
    {
	/*
	 *  Check the Z axis links.
	 */

	DTADEBUG(DAR_MSG_ERROR, "dtaTestMode: Z axis links not connected.%c\n", ' ' );

        status = DTA_BAD_DEV_CONNECT;
	assCommandFinish( par, status, "Z axis links not connected");
        return (status);
    }

    status = dtaStartMode( par, "test" );
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaTrackMode
 *
 * INVOCATION:
 * status = dtaTrackMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute a MODE of TRACK
 *
 * DESCRIPTION:
 * Execute a MODE of TRACK by calling dtaStartMode().
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

static long dtaTrackMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
    long 	status = DAR_S_SUCCESS;	/* Returned function status.	*/


    DTADEBUG(DAR_MSG_MAX, "dtaTrackMode: entry%c\n", ' ');

    status = dtaStartMode( par, "track" );
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaUpdateMode
 *
 * INVOCATION:
 * status = dtaUpdateMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute a MODE of UPDATE
 *
 * DESCRIPTION:
 * Execute a MODE of UPDATE by calling dtaStartMode().
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

static long dtaUpdateMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
     DTA_DEV_PRIVATE *pDtaPriv;		/* Ptr to private dev. struct.                */
     DTA_DEV_CONFIG  *pDtaConfig;	/* Ptr to device configuration stricture.     */
     ASS_CONTROL_PRIVATE *assPriv;      /* Shouldn't be changing this !               */
     char notAnErrMsg[DTA_MESSAGE_LENGTH];   /* The UPDATE message. */
     long status = DAR_S_SUCCESS;	/* Returned function status.                  */
     long one = 1;

     DTADEBUG(DAR_MSG_MAX, "dtaUpdateMode: entry%c\n", ' ');

     pDtaPriv   = (DTA_DEV_PRIVATE *) assGetPrivateStruct(par);
     pDtaConfig = (DTA_DEV_CONFIG *) pDtaPriv->pTranslationPriv;

    /*
     * Trap any problem with the internal data structures
     */

     if ( pDtaPriv == NULL || pDtaConfig == NULL )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaUpdateMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );
         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pDtaPriv->magic != DTA_MAGIC || pDtaConfig->magic != DTA_MAGIC )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaUpdateMode: No magic value seen in data structure%c\n", ' ' );
         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }


/* COLDTESTS - the below temperature should use the filtered on when the correct time constant is found */

     pDtaConfig->zeroOffsetTemperature = *(double *)DTA_EXTERN_TEMP_INPUT;

/* COLDTESTS - the following line can be removed after the GMOS thermal model is finalized  */

     recGblPutLinkValue (&(par->sot), (void *) par, DBR_DOUBLE,
                         &(pDtaConfig->zeroOffsetTemperature), &one);
     
    /*
     * The command is finished. It is assumed the assembly record will
     * keep the informational messages written to its message field.
     */

     sprintf(notAnErrMsg, "%s%4.2f%s", "Thermal model reset at ",
             pDtaConfig->zeroOffsetTemperature, " C");

     DTADEBUG(DAR_MSG_LOG, "dtaUpdateMode: %s\n", notAnErrMsg);

     assCommandFinish (par, status, notAnErrMsg);

     assPriv = (ASS_CONTROL_PRIVATE *) par->dpvt;
     assPriv->cmdResponse = TRUE;

     return (status);
}
/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaTranslateDir
 *
 * INVOCATION:
 * status = dtaTranslateDir (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) inDir  (unsigned short) Directive to translate.
 *
 * FUNCTION VALUE:
 * (long) translated directive.
 *
 * PURPOSE:
 * Translate DIRective
 *
 * DESCRIPTION:
 * Translate DIRective from the assemblyControl record values to the
 * deviceControl record.
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
 * Only will translate START and STOP.
 *-
 ************************************************************************
 */

static long dtaTranslateDir
(
	unsigned short	inDir		/* (in)  Dir to translate.	*/
)
{
    if ( inDir == DAR_DIR_START )
    {
	return( DAR_DEV_DIR_GO );
    }
    else
    {
	return( DAR_DEV_DIR_STOP );
    }
}

/* Following function added by PBT 3/3/00                               */
/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaProcessFault
 *
 * INVOCATION:
 * status = dtaProcessFault( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Clear up after a fault (interlock) occurred.
 *
 * DESCRIPTION:
 * If command is active, finish command
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * - other function name.
 *
 * DEFICIENCIES:
 * 
 *-
 ************************************************************************
 */

static long dtaProcessFault(ASSEMBLY_CONTROL_RECORD *par)
{
    DTA_DEV_PRIVATE *pDtaPriv;   /* Ptr to private dev. struct.	*/
    long status = DAR_S_SUCCESS;	/* Function status.		*/
    
    pDtaPriv = ( DTA_DEV_PRIVATE *) assGetPrivateStruct( par );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pDtaPriv == NULL )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaProcessFault: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pDtaPriv->magic != DTA_MAGIC )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaProcessFault: No magic value seen in data structure%c\n",
             ' ' );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");

         /* Do not return. Carry on and attempt the interlock anyway. */
     }


/* Is a command active? If not, do nothing */

    if (pDtaPriv->cmdActive)
     {

       /* Finish the command */

       DTADEBUG(DAR_MSG_ERROR, "dtaProcessFault: translation interlock occurred - aborting%c\n",
          ' ');

       status = DAR_E_FLT_CLR;
       assCommandFinish( par, status, "Translation interlock occurred" );
       pDtaPriv->cmdActive = FALSE;
     }
     
     return ( status );
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dtaReadConfig
 *
 * INVOCATION:
 * status = dtaReadConfig (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 * Read the current translation configuration file
 *
 * DESCRIPTION:
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

static long dtaReadConfig(ASSEMBLY_CONTROL_RECORD *par)
{
     ASS_CONTROL_PRIVATE *pPriv = par->dpvt;	/* Private rec. support struct.	*/
     DTA_DEV_PRIVATE *pDtaPriv;			/* Ptr to private dev. struct.	*/
     DTA_DEV_CONFIG  *pDtaConfig;
     long status = DAR_S_SUCCESS;		/* Return function status.	*/
     char scratchBuf[256];
     FILE *fp;

     char   lutname;
     double position;
     double delta1, delta2, delta3;
     int nx, ny, nz, ng, nt;

     pDtaPriv = ( DTA_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDtaConfig = pDtaPriv->pTranslationPriv;

     DTADEBUG(DAR_MSG_MAX, "dtaReadConfig: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pDtaPriv == NULL || pDtaConfig == NULL )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaReadConfig: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pDtaPriv->magic != DTA_MAGIC || pDtaConfig->magic != DTA_MAGIC )
     {
         DTADEBUG(DAR_MSG_FATAL, "dtaReadConfig: No magic value seen in data structure%c\n",
            ' ' );

         status = DTA_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

     /* First read the current lookup table */

     *scratchBuf = '\0';
     strncat( scratchBuf, par->tdir, MAX_STRING_SIZE );
     strcat( scratchBuf, "/" );
     strncat(scratchBuf, par->tfil, MAX_STRING_SIZE );

     if ((fp = fopen(scratchBuf, "r")) == NULL)
     {
          DTADEBUG(DAR_MSG_ERROR, "dtaReadConfig: failed to open LUT file %.40s\n", scratchBuf);

          /* A file open error should not affect the index status */
          semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
          pPriv->keepIndex = TRUE;
          semGive (pDtaPriv->mutexSem);

          status = DTA_FILEIO_ERROR;
          assCommandFinish(par, status, "Failed to open LUT file");

	  return (status);
     }

     /* The first line should contain the string "translation". Check this first */

     nx = ny = nz = ng = nt = 0;

     if (fgets(scratchBuf, sizeof scratchBuf, fp) == NULL) 
     {
	  DTADEBUG(DAR_MSG_ERROR, "dtaReadConfig: failed to read LUT file%c\n", '.');

	  /* A file read error should not affect the index status */
	  semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
	  pPriv->keepIndex = TRUE;
	  semGive (pDtaPriv->mutexSem);

	  status = DTA_FILEIO_ERROR;
	  assCommandFinish(par, status, "Failed to read LUT file");
     }
     else if (strncmp (scratchBuf, DTA_LUT_FIRST_STRING, 11))
     {
	  DTADEBUG(DAR_MSG_ERROR, "dtaReadConfig: could not find \"%s\" string\n",
             DTA_LUT_FIRST_STRING);

	  /* A file read error should not affect the index status */
	  semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
	  pPriv->keepIndex = TRUE;
	  semGive (pDtaPriv->mutexSem);

	  status = DTA_FILEIO_ERROR;
	  assCommandFinish(par, status, "Incorrect LUT file type");
     }
     else
     {

	  /* Process one entry at a time */
                          
	  while (fgets(scratchBuf, sizeof scratchBuf, fp) != NULL)
	  {
	       if (*scratchBuf != '#' && *scratchBuf != '\n') /* Ignore comments and blank lines */
	       {
		    if (sscanf(scratchBuf, "%c %lf %lf %lf %lf", &lutname, &position,
                               &delta1, &delta2, &delta3)
			!= 5)
		    {
			 DTADEBUG(DAR_MSG_ERROR, "dtaReadConfig: parse failed: %.60s\n", scratchBuf);

			 /* A file read error should not affect the index status */
			 semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
			 pPriv->keepIndex = TRUE;
			 semGive (pDtaPriv->mutexSem);

			 status = DTA_FILEIO_ERROR;
			 assCommandFinish(par, status, "Failed to read LUT file");
			 break;
		    }
		    else
		    {
			 /* read line was successful, populate internal store - if we have space left */

			 if ( lutname == 'X' )
			 {

			      /* Add an entry to the X to DeltaY and X to DeltaZ lookup tables. */

/* COMMENT OUT - printf for development and testing purposes only
   printf( "dtaReadConfig: Adding %f %f %f to X tables element %d\n",
           position, delta1, delta2, nx);
*/

			      if (nx <= MAX_GMOS_LUT_ENTRIES)
			      {
				   pDtaPriv->xToDeltaYLut.input[nx]    = position;
				   pDtaPriv->xToDeltaYLut.measured[nx] = delta1;
				   pDtaPriv->xToDeltaZLut.input[nx]    = position;
				   pDtaPriv->xToDeltaZLut.measured[nx] = delta2;
				   nx++;
			      }
			      else
			      {
				   DTADEBUG(DAR_MSG_ERROR,
                                            "dtaReadConfig: Too many X LUT entries, %d\n ", nx);

				   /* A file read error should not affect the index status */
				   semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
				   pPriv->keepIndex = TRUE;
				   semGive (pDtaPriv->mutexSem);

				   status = DTA_FILEIO_ERROR;
				   assCommandFinish(par, status, "Too many X LUT entries");
				   break;
			      }
			 }
			 else if ( lutname == 'Y' )
			 {

			      /* Add an entry to the Y to DeltaX and Y to DeltaZ lookup tables. */

/* COMMENT OUT - printf for development and testing purposes only
   printf( "dtaReadConfig: Adding %f %f %f to Y tables element %d\n",
           position, delta1, delta2, ny);
*/

			      if (ny <= MAX_GMOS_LUT_ENTRIES)
			      {
				   pDtaPriv->yToDeltaXLut.input[ny]    = position;
				   pDtaPriv->yToDeltaXLut.measured[ny] = delta1;
				   pDtaPriv->yToDeltaZLut.input[ny]    = position;
				   pDtaPriv->yToDeltaZLut.measured[ny] = delta2;
				   ny++;
			      }
			      else
			      {
				   DTADEBUG(DAR_MSG_ERROR,
                                            "dtaReadConfig: Too many Y LUT entries, %d\n ", ny);

				   /* A file read error should not affect the index status */
				   semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
				   pPriv->keepIndex = TRUE;
				   semGive (pDtaPriv->mutexSem);

				   status = DTA_FILEIO_ERROR;
				   assCommandFinish(par, status, "Too many Y LUT entries");
				   break;
			      }
			 }
			 else if ( lutname == 'Z' )
			 {

			      /* Add an entry to the Z to DeltaX and Z to DeltaY lookup tables. */

/* COMMENT OUT - printf for development and testing purposes only
   printf( "dtaReadConfig: Adding %f %f %f to Z tables element %d\n",
           position, delta1, delta2, nz);
*/

			      if (nz <= MAX_GMOS_LUT_ENTRIES)
			      {
				   pDtaPriv->zToDeltaXLut.input[nz]    = position;
				   pDtaPriv->zToDeltaXLut.measured[nz] = delta1;
				   pDtaPriv->zToDeltaYLut.input[nz]    = position;
				   pDtaPriv->zToDeltaYLut.measured[nz] = delta2;
				   nz++;
			      }
			      else
			      {
				   DTADEBUG(DAR_MSG_ERROR,
                                            "dtaReadConfig: Too many Z LUT entries, %d\n ", nz);

				   /* A file read error should not affect the index status */
				   semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
				   pPriv->keepIndex = TRUE;
				   semGive (pDtaPriv->mutexSem);

				   status = DTA_FILEIO_ERROR;
				   assCommandFinish(par, status, "Too many Z LUT entries");
				   break;
			      }
			 }
			 else if ( lutname == 'G' )
			 {

			      /* Add an entry to the G to DeltaX and G to DeltaY lookup tables. */

/* COMMENT OUT - printf for development and testing purposes only
   printf( "dtaReadConfig: Adding %f %f %f %f to G tables element %d\n",
           position, delta1, delta2, delta3, ng);
*/

			      if (ng <= MAX_GMOS_LUT_ENTRIES)
			      {
				   pDtaPriv->gToDeltaXLut.input[ng]    = position;
				   pDtaPriv->gToDeltaXLut.measured[ng] = delta1;
				   pDtaPriv->gToDeltaYLut.input[ng]    = position;
				   pDtaPriv->gToDeltaYLut.measured[ng] = delta2;
				   pDtaPriv->gToDeltaZLut.input[ng]    = position;
				   pDtaPriv->gToDeltaZLut.measured[ng] = delta3;
				   ng++;
			      }
			      else
			      {
				   DTADEBUG(DAR_MSG_ERROR,
                                            "dtaReadConfig: Too many G LUT entries, %d\n ", ng);

				   /* A file read error should not affect the index status */
				   semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
				   pPriv->keepIndex = TRUE;
				   semGive (pDtaPriv->mutexSem);

				   status = DTA_FILEIO_ERROR;
				   assCommandFinish(par, status, "Too many G LUT entries");
				   break;
			      }
			 }
			 else if ( lutname == 'T' )
			 {

			      /* Add an entry to the T to DeltaZ lookup table. */

/* COMMENT OUT - printf for development and testing purposes only
   printf( "dtaReadConfig: Adding %f %f %f %f to T table element %d\n",
           position, delta1, delta2, delta3, nt);
*/

			      if (nt <= MAX_GMOS_LUT_ENTRIES)
			      {
				   pDtaPriv->tToDeltaXLut.input[nt]    = position;
				   pDtaPriv->tToDeltaXLut.measured[nt] = delta1;
				   pDtaPriv->tToDeltaYLut.input[nt]    = position;
				   pDtaPriv->tToDeltaYLut.measured[nt] = delta2;
				   pDtaPriv->tToDeltaZLut.input[nt]    = position;
				   pDtaPriv->tToDeltaZLut.measured[nt] = delta3;
				   nt++;
			      }
			      else
			      {
				   DTADEBUG(DAR_MSG_ERROR,
                                            "dtaReadConfig: Too many T LUT entries, %d\n ", nt);

				   /* A file read error should not affect the index status */
				   semTake (pDtaPriv->mutexSem, WAIT_FOREVER);
				   pPriv->keepIndex = TRUE;
				   semGive (pDtaPriv->mutexSem);

				   status = DTA_FILEIO_ERROR;
				   assCommandFinish(par, status, "Too many T LUT entries");
				   break;
			      }
			 }
		    }
	       }
	  }
     }
     pDtaPriv->xToDeltaYLut.numberOfEntries = nx;
     pDtaPriv->xToDeltaZLut.numberOfEntries = nx;
     pDtaPriv->yToDeltaXLut.numberOfEntries = ny;
     pDtaPriv->yToDeltaZLut.numberOfEntries = ny;
     pDtaPriv->zToDeltaXLut.numberOfEntries = nz;
     pDtaPriv->zToDeltaYLut.numberOfEntries = nz;
     pDtaPriv->gToDeltaXLut.numberOfEntries = ng;
     pDtaPriv->gToDeltaYLut.numberOfEntries = ng;
     pDtaPriv->gToDeltaZLut.numberOfEntries = ng;
     pDtaPriv->tToDeltaXLut.numberOfEntries = nt;
     pDtaPriv->tToDeltaYLut.numberOfEntries = nt;
     pDtaPriv->tToDeltaZLut.numberOfEntries = nt;

     fclose (fp);


     return(status);
}


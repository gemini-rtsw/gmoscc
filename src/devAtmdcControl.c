static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id$"};

/*
 * FILENAME
 * devAtmdcControl.c
 *
 * PURPOSE:
 * EPICS AtmDC Device Support code for the assemblyControl record.
 *
 * FUNCTION NAME(S)
 * atmActivateDevice	Send info off to device records attached.
 * atmCheckBus		Check whether device records attached are busy.
 * atmAckReceived	A busy link activated.
 * atmCheckAttributes	Check input attributes.
 * atmDevInit		Initialize device support code.
 * atmIndexMode		Execute INDEX mode command.
 * atmInitDeviceSupport	Initialize record's device support code.
 * atmInitMode		Execute INIT mode command.
 * atmMoveMode		Execute MOVE or PARK mode command.
 * atmStartMode		Execute mode command.
 * atmStopDirective	Execute STOP directive to attached records.
 * atmTestMode		Execute the TEST mode.
 * atmTrackMode		Execute the TRACK mode.
 * atmTranslateDir	Translate dir directive for device record.
 * atmProcessFault	Tidy up after fault (interlock occurred)
 *
 * AUTHOR:
 * S M Beard, UKATC (Modified from example provided by J Dunn, DAO)
 *
 * DEFICIENCIES:
 * - Based on old version of devTranslationAss, so needs bringing up to date.
 * - STOP directive not implemented
 *
 * BUGS:
 *
 *INDENT-OFF*
 *   $Log$
 *   Revision 1.4  2001/02/28 15:15:15  gmos
 *   A couple of strcpy calls converted to strncpy.
 *
 *   Revision 1.3  2001/02/23 18:11:57  gmos
 *   Trivial formatting change.
 *
 *   Revision 1.2  2001/02/23 16:33:52  gmos
 *   Removed ancient and misleading BUG WORK AROUND messages.
 *
 *   Revision 1.1  2000/09/08 11:38:59  gmos
 *   Dummy code for the Atmospheric Dispersion Compensator, based on the old code for the detector translation assembly.
 *
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
#include	<logLib.h>
#include	<tickLib.h>		/* For tick get.		*/
#include	<sysLib.h>		/* For sysClkRateGet.		*/

#include	<assemblyControlRecord.h>
#include	<recAssControl.h>


/*
 *  Local Defines
 */

#define atmDevReport		NULL		/* no report provided 	*/
#define atmDevGetIoIntInfo	NULL		/* no ioint info provided */
#define atmUpdateMode		NULL		/* no function.		*/

#define ATM_VELOCITY		1.5		/* CCD AtmDC target velocity */

typedef struct devInfo {
    struct link		*dirLink;		/* Dir's links.		*/
    struct link		*modLink;		/* Mod's links.		*/
    struct link		*posLink;		/* Pos's links.		*/
    struct link		*velLink;		/* Vel's links.		*/
    struct link		*ackLink;		/* Acks's links.	*/
    int			dev1Finished;		/* Dev 1 finished.	*/
    int			dev2Finished;		/* Dev 2 finished.	*/
} DEV_INFO;



/*
 *  Device support function prototypes - LOCAL
 */

static long atmAckReceived( ASSEMBLY_CONTROL_RECORD * );
static long atmCheckAttributes( ASSEMBLY_CONTROL_RECORD * );
static long atmIndexMode (ASSEMBLY_CONTROL_RECORD * );
static long atmInitDeviceSupport( ASSEMBLY_CONTROL_RECORD * );
static long atmInitMode (ASSEMBLY_CONTROL_RECORD * );
static long atmMoveMode (ASSEMBLY_CONTROL_RECORD * );
static long atmStopDirective( ASSEMBLY_CONTROL_RECORD * );
static long atmTestMode (ASSEMBLY_CONTROL_RECORD * );
static long atmTrackMode (ASSEMBLY_CONTROL_RECORD * );
static long atmProcessFault (ASSEMBLY_CONTROL_RECORD * );


/*
 * Internal function prototypes - LOCAL
 */
  
static long atmActivateDevice( ASSEMBLY_CONTROL_RECORD * );
static long atmCheckBus( ASSEMBLY_CONTROL_RECORD *, int );
static long atmStartMode (ASSEMBLY_CONTROL_RECORD * , char * );
static long atmTranslateDir( unsigned short );

typedef struct {
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
    DEVSUPFUN       processFault;         /* Added 1/3/00 PBT */
} ASSEMBLY_CONTROL_DSET;


/*
 *  Data Structures - GLOBAL
 */


ASSEMBLY_CONTROL_DSET devAtmdcAssembly = {
    13,						/* Number                        */
    atmDevReport,				/* Report function               */
    NULL,					/* Initialise                    */
    atmInitDeviceSupport,			/* Initialise record             */
    atmDevGetIoIntInfo,				/* Get I/O interrupt information */
						/* User defined functions:       */
    atmCheckAttributes,				/*    Check attributes           */
    atmStopDirective,				/*    Process STOP directive     */
    atmInitMode,				/*    Process INIT mode          */
    atmMoveMode,				/*    Process MOVE and PARK modes*/
    atmTrackMode,				/*    Process TRACK mode         */
    atmIndexMode,				/*    Process INDEX mode         */
    atmTestMode,				/*    Process TEST mode          */
    atmAckReceived,				/*    Acknowledgement received   */
    atmUpdateMode,				/*    Process UPDATE mode        */
    atmProcessFault                             /* Added 3/3/00 PBT */
    };   


/*
 *  Internal private device support structure.
 */

typedef struct {
    int			busyMode;		/* Current busy mode.		*/
    int			bus1Finished;		/* Bus1 finished.		*/
    int			bus2Finished;		/* Bus2 finished.		*/
    int			bus3Finished;		/* Bus3 finished.		*/
    int			bus4Finished;		/* Bus4 finished.		*/
    int			bus5Finished;		/* Bus5 finished.		*/
    int			cmdActive;		/* Command is activated.	*/
    SEM_ID              mutexSem;		/* mutual exclusion semaphore 	*/
    ASSEMBLY_CONTROL_RECORD   
    			*pRecord;		/* calling record structure 	*/
    int			simulation;		/* simulation mode 		*/
    double		simPosition;		/* simulated readback value 	*/
    double		simVelocity;		/* simulated readback value 	*/
    double		position[DAR_ARG_MAX];	/* position value for each axis	*/
    double		velocity;		/* velocity value for both axes	*/
} DEV_ASS_CONTROL_PRIVATE;


/*
 *  Macros
 *
 *  DEBUG   - Logs a message if the debug level of the message is less than the current
 *            debugging mode. The time and record name are prepended.
 *
 *  ATMDEBUG - A AtmDC-specific wrapper for the DEBUG macro which avoids repeating
 *            the "<%d> %s:" format throughout the code.
 */

#define DEBUG(l,FMT,V)						\
{								\
    int k=l;							\
    if (k <= pdr->dbug)						\
	logMsg (FMT, (int) tickGet(), (int) pdr->name, (int) V, 0, 0, 0); \
}


#define ATMDEBUG(l,FMT,V)					\
{								\
    DEBUG (l, "<%d> %s:" FMT, V);                               \
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmActivateDevice
 *
 * INVOCATION:
 * status = atmActivateDevice (pdr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute a MODE to a device
 *
 * DESCRIPTION:
 * Execute a MODE to a device.
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

static long atmActivateDevice
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr		/* (in)  Ptr to ass. rec.	*/
)
{
    unsigned short	
    		dir;			/* Directive value.		*/
    unsigned short	
    		mode;			/* Mode value.			*/
    int		i;			/* Counter.			*/
    DEV_ASS_CONTROL_PRIVATE 
    		*pDevAss;		 /* Ptr. to dev. support struc.	*/
    long	nRequest = 1;		/* Number to send on link.	*/
    unsigned short 	
    		returnedAck;		/* Returned ack from device.	*/
    long	status = DAR_S_SUCCESS;	/* Function status.		*/
    char	tmpPtr[124];
    char	*svals;			/* Tmp. string ptr.		*/
    struct link	*dirLink = &pdr->odr1;	/* Dir's links.			*/
    struct link	*modLink = &pdr->mod1;	/* Mod's links.			*/
    struct link	*posLink = &pdr->pos1;	/* Pos's links.			*/
    struct link	*velLink = &pdr->vel1;	/* Vel's links.			*/
    struct link	*ackLink = &pdr->ack1;	/* Acks's links.		*/


    pDevAss =  ( DEV_ASS_CONTROL_PRIVATE *) assGetPrivateStruct( pdr );
    ATMDEBUG(DAR_DBUG_FULL, "atmActivateDevice: entry, cmdActive=%d\n", 
    		pDevAss->cmdActive );

   
    /*
     *  Setup initialize variables.
     */

    dir = atmTranslateDir( pdr->dir );
    mode = pdr->mode;


    /*
     *  Cycle through all output links, they should be sequential.
     */

    for( i = 0 ; i < DAR_ARG_MAX; i++, modLink++, dirLink++,
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
	 *  Write the Position to VALS, a string.
         *  In PARK mode write the string "park" to VALS instead and convert the output mode to MOVE.
	 */


        if ( pdr->mode == DAR_MODE_PARK )
        {
            strcpy( tmpPtr, "park");
            svals = tmpPtr;
            mode = DAR_MODE_MOVE;
        }
        else
        {
            sprintf( tmpPtr, "%f", pDevAss->position[i] );
            svals = tmpPtr;
        }

	CHECKSTAT( (status = recGblPutLinkValue( posLink, (void *) pdr, 
		DBR_STRING, svals, &nRequest)), return(status) );
	ATMDEBUG(DAR_DBUG_MIN, "atmActivateDevice: Putting POS, %s\n", 
		svals );


	/*
	 *  Write the Velocity.  WARNING, must write a double.
	 */

        CHECKSTAT( (status = recGblPutLinkValue( velLink, (void *) pdr,
		DBR_DOUBLE, &(pDevAss->velocity), &nRequest)), return(status) );
	ATMDEBUG(DAR_DBUG_MIN, "atmActivateDevice: Putting VEL, %ld\n", 
		pDevAss->velocity  );


	/*
	 *  Write the mode.
	 */

        CHECKSTAT( (status = recGblPutLinkValue( modLink, (void *) pdr, 
		DBR_SHORT, &mode, &nRequest)), return(status) );
	ATMDEBUG(DAR_DBUG_MIN, "atmActivateDevice: Putting MOD, %d\n", 
		mode  );


	/*
	 *  Write the directive, translate first.
	 */

        CHECKSTAT( (status = recGblPutLinkValue( dirLink, (void *) pdr, 
		DBR_SHORT, (void *) (&dir), &nRequest)), return(status) );
	ATMDEBUG(DAR_DBUG_FULL, "atmActivateDevice: Putting DIR, %d\n",
		    dir );


	/*
	 *  Check the response from the device.
	 */

	status = recGblGetFastLink( ackLink, (void *) pdr, &returnedAck);
	ATMDEBUG(DAR_DBUG_MIN, "atmActivateDevice: Got response: %d\n", 
		returnedAck );
	ATMDEBUG(DAR_DBUG_FULL, "atmActivateDevice: after get ack, cmdActive=%d\n",
		    pDevAss->cmdActive );
	if (returnedAck != DAR_DEV_VAL_ACCEPT )
	{
	    ATMDEBUG(DAR_DBUG_NONE, "atmActivateDevice: Device failed: %d\n", 
		    returnedAck);
	    status = DAR_E_DEVICE;
	    assCommandFinish( pdr, status, "Error, device failed command.");
	    pDevAss->cmdActive = FALSE;
	    break;
	}
	semTake (pDevAss->mutexSem, WAIT_FOREVER);
	pDevAss->cmdActive = TRUE;
	semGive (pDevAss->mutexSem);

    }

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmAckReceived
 *
 * INVOCATION:
 * status = atmAckReceived( pdr ); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization code.
 *
 * PURPOSE:
 * Handle a changed BUS[1-5] field change in the record
 *
 * DESCRIPTION:
 * Handle a changed BUS[1-5] field change in the record.
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

static long atmAckReceived
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr		/* (in)  Ptr to ass. rec.	*/
)
{
    int		bus1;			/* Response from bus1.		*/
    int		bus2;			/* Response from bus2.		*/
    int		bus3;			/* Response from bus3.		*/
    int		bus4;			/* Response from bus4.		*/
    int		bus5;			/* Response from bus5.		*/
    DEV_ASS_CONTROL_PRIVATE 
    		*pDevAss;		/* Ptr to private dev. struct.	*/
    long	status = DAR_S_SUCCESS;	/* Function status.		*/


    pDevAss =  ( DEV_ASS_CONTROL_PRIVATE *) assGetPrivateStruct( pdr );


    ATMDEBUG(DAR_DBUG_FULL, "atmAckReceived: entry, cmdActive=%d\n", 
    		pDevAss->cmdActive );

    /*
     *  Get bus responses we can check.
     */

    assGetBusResponse( pdr, &bus1, &bus2, &bus3, &bus4, &bus5 );

    semTake (pDevAss->mutexSem, WAIT_FOREVER);
    if ( pDevAss->cmdActive == FALSE )
    {
	/*
	 *  No command is active, so expected no change in the BUS links,
	 *  ignore it.
	 */

	ATMDEBUG(DAR_DBUG_MIN, "atmAckReceived: cmd resp. not expected%c\n",
		' ');
    }
    else if ( assCommandTimedOut( pdr ) )
    {
	/*
	 *  else we may have timed out of a command.
	 */

	ATMDEBUG(DAR_DBUG_NONE, "atmAckReceived: cmd timed out%c\n", ' ');
	status = DAR_E_TIMEDOUT;
	assCommandFinish( pdr, status, "Error, command timed out" );
    }
    else if ( bus1 == TRUE || bus2 == TRUE || 
    	bus3 == TRUE || bus4 == TRUE || bus5 == TRUE )
    {
    	/*
	 *  else response received from the device(s), handle it.
	 */

	if ( bus1 == TRUE )
	{
	    if ( pdr->bus1 == DAR_DEV_BUSY_IDLE )
	    {
		pDevAss->bus1Finished = TRUE;
	    }
	    ATMDEBUG(DAR_DBUG_MIN, "atmAckReceived, resp bus 1=%d\n", 
		    pdr->bus1 );
	    status = atmCheckBus( pdr, pdr->bus1 );
	}
	if ( status == DAR_S_SUCCESS && bus2 == TRUE )
	{
	    if ( pdr->bus2 == DAR_DEV_BUSY_IDLE )
	    {
		pDevAss->bus2Finished = TRUE;
	    }
	    ATMDEBUG(DAR_DBUG_MIN, "atmAckReceived, resp bus 2=%d\n",
		    pdr->bus2 );
	    status = atmCheckBus( pdr, pdr->bus2 );
	}
	if ( status == DAR_S_SUCCESS && bus3 == TRUE )
	{
	    if ( pdr->bus3 == DAR_DEV_BUSY_IDLE )
	    {
		pDevAss->bus3Finished = TRUE;
	    }
	    ATMDEBUG(DAR_DBUG_MIN, "atmAckReceived, resp bus 3=%d\n",
		    pdr->bus3 );
	    status = atmCheckBus( pdr, pdr->bus3 );
	}
	if ( status == DAR_S_SUCCESS && bus4 == TRUE )
	{
	    if ( pdr->bus4 == DAR_DEV_BUSY_IDLE )
	    {
		pDevAss->bus4Finished = TRUE;
	    }
	    ATMDEBUG(DAR_DBUG_MIN, "atmAckReceived, resp bus 4=%d\n",
		    pdr->bus4 );
	    status = atmCheckBus( pdr, pdr->bus4 );
	}
	if ( status == DAR_S_SUCCESS && bus5 == TRUE )
	{
	    if ( pdr->bus5 == DAR_DEV_BUSY_IDLE )
	    {
		pDevAss->bus5Finished = TRUE;
	    }
	    ATMDEBUG(DAR_DBUG_MIN, "atmAckReceived, resp bus 5=%d\n",
		    pdr->bus5 );
	    status = atmCheckBus( pdr, pdr->bus5 );
	}
    }
    else
    {
    	/*
	 *  Received bus acknowledgment from something other than bus1, bus2 or bus3.
	 */

	ATMDEBUG(DAR_DBUG_MIN, "atmAckReceived, invalid bus response%c\n", ' ');
    }

    semGive (pDevAss->mutexSem);
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmCheckAttributes
 *
 * INVOCATION:
 * status = atmCheckAttributes( pdr );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
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
 * Executed during a PRESET prior to a START.
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
 * None.
 *-
 ************************************************************************
 */

static long	atmCheckAttributes
(
    ASSEMBLY_CONTROL_RECORD *pdr	/* (in)  Ptr to assembly record structure */
)
{
    DEV_ASS_CONTROL_PRIVATE 
    		*pDevAss;		/* Ptr to private dev. struct.	*/
    long	status = DAR_S_SUCCESS;	/* Returned function status.	*/
    double	value, lower, upper;    /* Value, lower and upper limit	*/
    int         i;			/* Index variable.		*/


    pDevAss =  ( DEV_ASS_CONTROL_PRIVATE *) assGetPrivateStruct( pdr );

    ATMDEBUG(DAR_DBUG_FULL, "atmCheckAttributes: entry%c\n", ' ');

    /*
     * Input attributes only need to be checked if the mode is MOVE.
     */

    if ( pdr->mode == DAR_MODE_MOVE )
    {

        /*
         * Check that each of the attributes are not NULL, have DOUBLE type and are within range.
         * It is assumed that if both the lower and upper limit parameters are zero the allowed
         * range is unlimited.
         */

        /* Attribute A */

	if ( status == DAR_S_SUCCESS )
	{
            if ( pdr->a == NULL )
            {
                ATMDEBUG(DAR_DBUG_NONE, "atmCheckAttributes: Attr A is NULL%c\n", ' ');
                sprintf (pdr->mess, "Attr A is NULL");
                status = DAR_E_ATT;
            }
            else if ( pdr->fta != DBF_DOUBLE )
            {
                ATMDEBUG(DAR_DBUG_NONE, "atmCheckAttributes: Attr A is not DOUBLE\n", ' ');
                sprintf (pdr->mess, "Attr A is not DOUBLE");
                status = DAR_E_ATT;
            }
            else if ( ( (double)pdr->aall != 0.0 ) || ( (double)pdr->aahl != 0.0 ) )
            {
                value = *(double *)pdr->a;
                lower = (double)pdr->aall;
                upper = (double)pdr->aahl;

                if ( value < lower )
                {
                    ATMDEBUG(DAR_DBUG_NONE, "atmCheckAttributes: Attr A is < %f\n",
                      lower );
                    sprintf (pdr->mess, "Attr A (%10f) < %10f", value, lower );
	            status = DAR_E_ATT;
                }
                else if ( value > upper )
                {
                    ATMDEBUG(DAR_DBUG_NONE, "atmCheckAttributes: Attr A is > %f\n",
                      upper );
                    sprintf (pdr->mess, "Attr A (%10f) > %10f", value, upper );
	            status = DAR_E_ATT;
                }
            }
        }

        /* Attribute B */

	if ( status == DAR_S_SUCCESS )
	{
            if ( pdr->b == NULL )
            {
                ATMDEBUG(DAR_DBUG_NONE, "atmCheckAttributes: Attr B is NULL%c\n", ' ');
                sprintf (pdr->mess, "Attr B is NULL");
                status = DAR_E_ATT;
            }
            else if ( pdr->ftb != DBF_DOUBLE )
            {
                ATMDEBUG(DAR_DBUG_NONE, "atmCheckAttributes: Attr B is not DOUBLE\n", ' ');
                sprintf (pdr->mess, "Attr B is not DOUBLE");
                status = DAR_E_ATT;
            }
            else if ( ( (double)pdr->abll != 0.0 ) || ( (double)pdr->abhl != 0.0 ))
            {
                value = *(double *)pdr->b;
                lower = (double)pdr->abll;
                upper = (double)pdr->abhl;

                if ( value < lower )
                {
                    ATMDEBUG(DAR_DBUG_NONE, "atmCheckAttributes: Attr B is < %f\n",
                      lower );
                    sprintf (pdr->mess, "Attr B (%10f) < %10f", value, lower );
	            status = DAR_E_ATT;
                }
                else if ( value > upper )
                {
                    ATMDEBUG(DAR_DBUG_NONE, "atmCheckAttributes: Attr B is > %f\n",
                      upper );
                    sprintf (pdr->mess, "Attr B (%10f) > %10f", value, upper );
	            status = DAR_E_ATT;
                }
            }
        }


	if ( status == DAR_S_SUCCESS )
        {
	    /*
             * Set the positions and velocity. (Only the first two positions are used by the
             * AtmDC assembly. 
	     */

	    pDevAss->position[0] = *(double *) pdr->a;
	    pDevAss->position[1] = *(double *) pdr->b;
	    pDevAss->velocity    = ATM_VELOCITY;
        }
    }
    else if ( pdr->mode == DAR_MODE_TRACK )
    {
        /*
         * TRACK mode is not implemented.
         */

        ATMDEBUG(DAR_DBUG_NONE,
          "atmCheckAttributes: TRACK mode not implemented\n", ' ');
        strncpy (pdr->mess, "TRACK mode not implemented", MAX_STRING_SIZE-1);
        status = DAR_E_MODE;
    }
    else if ( pdr->mode == DAR_MODE_UPDATE )
    {
        /*
         * UPDATE mode is not appropriate for the detector AtmDC mechanism.
         */

        ATMDEBUG(DAR_DBUG_NONE,
          "atmCheckAttributes: UPDATE mode not appropriate for this assembly\n", ' ');
        strncpy (pdr->mess, "UPDATE mode not appropriate", MAX_STRING_SIZE-1);
        status = DAR_E_MODE;
    }
    else
    {
        /*
         * In other modes set the positions to zero and the velocity to a suitable value.
         */

         for ( i=0; i<DAR_ARG_MAX; i++ )
         {
	     pDevAss->position[i] = 0.0;
         }
	 pDevAss->velocity    = ATM_VELOCITY;
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmCheckBus
 *
 * INVOCATION:
 * status = atmCheckBus( pdr );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 * (>) busVal (int ) Value of the bus change.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * To check the change in the BUS line
 *
 * DESCRIPTION:
 * To check the change in the BUS line.  
 *  Check what kind of busy response we received.
 *  - If ERR, then the current cmd failed, and do stop stuff,
 *    assStopTimer, cmdActive=FALSE, BUSY=DAR_VAL_REJECT.
 *  - If BUSY, then ignore it.
 *  - If IDLE, then this device finished.
 *    - If this is the last device then stop the timer.
 *    - Set the cmdActive flag to false.
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
 *-
 ************************************************************************
 */

static long	atmCheckBus
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr,		/* (in)  Ptr to assembly record structure */
    int			busVal		/* (in)  Bus value.		*/
)
{
    DEV_ASS_CONTROL_PRIVATE 
    		*pDevAss;		/* Ptr to private dev. struct.	*/
    long	status = DAR_S_SUCCESS;	/* Function status.		*/
    int		totalFinished;		/* Num finished devices.	*/


    pDevAss =  ( DEV_ASS_CONTROL_PRIVATE *) assGetPrivateStruct( pdr );


    ATMDEBUG(DAR_DBUG_FULL, "atmCheckBus: entry, bus=%d\n", busVal);


    /*
     *  Handle the bus value change.
     */

    switch( busVal )
    {
	case DAR_DEV_BUSY_ERROR:
	    /*
	     * Stop the timer, set error status, indicated that
	     * the command has finished.
	     */

	    ATMDEBUG(DAR_DBUG_NONE, "atmCheckBus, ERROR: entry%c\n", 
		    ' ');
	    assStopTimer( pdr, &status );
	    status = DAR_E_DEVICE;
	    assCommandFinish( pdr, status, "Error received from device record" );
	    pDevAss->cmdActive = FALSE;
	    break;

	case DAR_DEV_BUSY_BUSY:
	    /*
	     *  Ignore the busy change.
	     */

	    ATMDEBUG(DAR_DBUG_FULL, "atmCheckBus, BUSY: entry%c\n", 
		    ' ');
	    break;

	case DAR_DEV_BUSY_IDLE:
	    /*
	     *  Check to see if all devices are finished, 
	     *  and if so then the command is finished, so stop the timer
	     *  set the bus flags to false.
	     */

	    totalFinished = pDevAss->bus1Finished + pDevAss->bus2Finished +
	    		pDevAss->bus3Finished + pDevAss->bus4Finished +
			pDevAss->bus5Finished;
	    ATMDEBUG(DAR_DBUG_MIN, "atmCheckBus, bus=IDLE%c\n", ' ');

            if ( pdr->nmdv == totalFinished )
	    {
		/*
		 *  All devices have completed.
		 * This is the end of the command, so say command is
		 *  finished to the record code, and cmd not active
		 *  to the device code.
		 */

		assStopTimer( pdr, &status );
		assCommandFinish( pdr, status, NULL );
		pDevAss->cmdActive = FALSE;
		ATMDEBUG(DAR_DBUG_MIN, "atmCheckBus, cmd done%c\n", 
		    ' ');
	    }
	    else
	    {
		ATMDEBUG(DAR_DBUG_MIN, "atmCheckBus, not done yet%c\n", 
		    ' ');
	    }
	    break;

	default:
	    ATMDEBUG(DAR_DBUG_NONE, "atmCheckBus, invalid value received from bus:%d\n", busVal );
	    assAddErrorMessage( pdr, "Error, invalid bus value.");
	    break;
    }
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmIndexMode
 *
 * INVOCATION:
 * status = atmIndexMode (pdr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Execute a MODE of INDEX
 *
 * DESCRIPTION:
 * Execute a MODE of INDEX.
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

static long atmIndexMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr		/* (in)  Ptr to ass. rec.	*/
)
{
    long 	status = DAR_S_SUCCESS;


    ATMDEBUG(DAR_DBUG_FULL, "atmIndexMode: entry%c\n", ' ');

    status = atmStartMode( pdr, "index" );
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmInitDeviceSupport
 *
 * INVOCATION:
 * status = atmInitDeviceSupport (pdr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
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

static long atmInitDeviceSupport
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr		/* (in)  Ptr to ass. rec.	*/
)
{
    long	 status = DAR_S_SUCCESS; /* Returned function status.	*/
    DEV_ASS_CONTROL_PRIVATE *pDevAss;	/* Private dev. struct.		*/
    int          i;


    ATMDEBUG(DAR_DBUG_FULL, "atmInitDeviceSupport: entry%c\n", ' ');


    /*
     *  Create private device support structure for this record.
     */

    pDevAss = malloc (sizeof(DEV_ASS_CONTROL_PRIVATE));
    if ( pDevAss == NULL ) 
    {
	ATMDEBUG(DAR_DBUG_NONE, "atmInitDeviceSupport: malloc failed.%c\n", 
		' ');
	status = -1;
        recGblRecordError (status, pdr, __FILE__ ":no room for device private");
        return( status );
    }


    /*
     *  Link this structure to the record support private structure.
     */
     
    assAttachPrivateStruct ( pdr, (void *) pDevAss ); 


    /*
     * Create the MUTEX semaphore to protect the private structure
     * during asynchronous callback access.
     */
     
    pDevAss->mutexSem = NULL;
    if (  (pDevAss->mutexSem = 
	    semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE) ) == NULL )
    {
	ATMDEBUG(DAR_DBUG_NONE, "atmInitDeviceSupport: mutex failed.%c\n", 
		' ' );
        status = DAR_E_MALLOC;
        recGblRecordError (status, pdr, __FILE__ ":no room for mutex creation");
        return( status );
    }


    /*
     * Save record, callback and structure pointers
     */  

    semTake (pDevAss->mutexSem, WAIT_FOREVER);
     
    pDevAss->pRecord = pdr;
    pDevAss->simPosition = 0.0;
    pDevAss->simVelocity = 0.0;

    for ( i=0; i<DAR_ARG_MAX; i++ )
        pDevAss->position[i] = 0.0;

    pDevAss->velocity = ATM_VELOCITY;
    pDevAss->simulation = DAR_SIM_NONE; 
    pDevAss->cmdActive = FALSE; 
    pDevAss->bus1Finished = FALSE; 
    pDevAss->bus2Finished = FALSE; 
    pDevAss->bus3Finished = FALSE; 
    pDevAss->bus4Finished = FALSE; 
    pDevAss->bus5Finished = FALSE; 

    semGive (pDevAss->mutexSem);

    ATMDEBUG(DAR_DBUG_FULL, "atmInitDeviceSupport: finished,status=%ld\n",  
	    status );

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmInitMode
 *
 * INVOCATION:
 * status = atmInitMode (pdr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Execute a MODE of INIT
 *
 * DESCRIPTION:
 * Execute a MODE of INIT by calling atmStartMode().
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

static long atmInitMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr		/* (in)  Ptr to ass. rec.	*/
)
{
    long 	status = DAR_S_SUCCESS;	/* Return function status.	*/

    ATMDEBUG(DAR_DBUG_FULL, "atmInitMode: entry%c\n", ' ');


    /*
     * There are no lookup tables to read.
     */


    status = atmStartMode( pdr, "init" );

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmMoveMode
 *
 * INVOCATION:
 * status = atmMoveMode (pdr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization code.
 *
 * PURPOSE:
 * Execute a MODE of MOVE or PARK
 *
 * DESCRIPTION:
 * Execute a MODE of MOVE or PARK by calling atmStartMode().
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

static long atmMoveMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr		/* (in)  Ptr to ass. rec.	*/
)
{
    long 	status = DAR_S_SUCCESS;	/* Return function status.	*/

    ATMDEBUG(DAR_DBUG_FULL, "atmMoveMode: entry. Mode = %d\n", pdr->mode);

    status = atmStartMode( pdr, "move" );
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmStartMode
 *
 * INVOCATION:
 * status = atmStartMode (pdr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute the passed in MODE
 *
 * DESCRIPTION:
 * Execute the passed in MODE by sending the MODE, DIR, POS, VEL to
 * all attached deviceControl records.  If they return accept, then
 * start the watchdog timer.
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

static long atmStartMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr,		/* (in)  Ptr to ass. rec.	*/
    char		*mode		/* (in)	 Mode to execute	*/
)
{
    DEV_ASS_CONTROL_PRIVATE 
    		*pDevAss;		/* Ptr to dev. support struc.	*/
    long 	status = DAR_S_SUCCESS;	/* Return function status.	*/


    pDevAss =  ( DEV_ASS_CONTROL_PRIVATE *) assGetPrivateStruct( pdr );

    ATMDEBUG(DAR_DBUG_FULL, "atmStartMode: entry, mode=%s\n", mode );


    /*
     *  Activate the attached deviceControl records and start the command timer.
     */

    semTake (pDevAss->mutexSem, WAIT_FOREVER);
    pDevAss->bus1Finished = FALSE;
    pDevAss->bus2Finished = FALSE;
    pDevAss->bus3Finished = FALSE;
    pDevAss->bus4Finished = FALSE;
    pDevAss->bus5Finished = FALSE;
    semGive (pDevAss->mutexSem);

    status = atmActivateDevice( pdr );
    if ( status == DAR_S_SUCCESS )
    {
	assStartTimer( pdr, &status, 30 );
    }
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmStopDirective
 *
 * INVOCATION:
 * status = atmStopDirective (pdr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute a STOP directive
 *
 * DESCRIPTION:
 * Execute a STOP directive.
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

static long atmStopDirective
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr		/* (in)  Ptr to ass. rec.	*/
)
{
    struct link	*dirLink;		/* Dir's link.			*/
    long 	status = DAR_S_SUCCESS;	/* Return function status.	*/
					/* Private rec. support struc.	*/
    DEV_ASS_CONTROL_PRIVATE 
    		*pDevAss;		/* Private dev. support struc.	*/
    unsigned short	
    		dir;			/* Directive to send.		*/
    long	nRequest = 1;		/* Number of requested.		*/


    pDevAss =  ( DEV_ASS_CONTROL_PRIVATE *) assGetPrivateStruct( pdr );

    ATMDEBUG(DAR_DBUG_FULL, "atmStopDirective: entry%c\n", ' ');


    /*
     *  Get the current device record's busy state.  If already busy, then
     *  the next change will be to idle and may happen as soon as we
     *  issue a stop, , so be ready.
     *  First set the cmdActive flag (if BUSY is BUSY),
     *  Translate the directive and put that value out on the DIR link.
     */

    pDevAss->busyMode = pdr->bus1;
    pDevAss->cmdActive = ( pDevAss->busyMode == DAR_DEV_BUSY_IDLE )? FALSE : 
    		TRUE;
    ATMDEBUG(DAR_DBUG_FULL, "stoppingDirective:write dir, bus1==%d\n",
		pDevAss->busyMode );
    dir = atmTranslateDir( pdr->dir );
    dirLink = &pdr->odr1;
    CHECKSTAT( (status = recGblPutLinkValue( dirLink, (void *) pdr, 
	    DBR_SHORT, (void *) (&dir), &nRequest)) , return(status) );

    if ( pdr->nmdv >= 2 )
    {
	/*
	 * We have 2 or more devices attached.
	 */

	dirLink = &pdr->odr2;
	CHECKSTAT( (status = recGblPutLinkValue( dirLink, (void *) pdr, 
		DBR_SHORT, (void *) (&dir), &nRequest)) , return(status) );
    }

    if ( pdr->nmdv == 3 )
    {
	/*
	 * Assume we have 3 devices attached.
	 */

	dirLink = &pdr->odr3;
	CHECKSTAT( (status = recGblPutLinkValue( dirLink, (void *) pdr, 
		DBR_SHORT, (void *) (&dir), &nRequest)) , return(status) );
    }

    ATMDEBUG(DAR_DBUG_MIN, "stoppingDirective: Putting DIR, %d\n", 
	    dir  );


    if ( pDevAss->busyMode == DAR_DEV_BUSY_IDLE )
    {
	/*
	 *  Device record is currently idle, so we will want to start 
	 *  the timer as the device record will go busy then back to
	 *  idle or err.
	 */

	pDevAss->cmdActive = TRUE;
	assStartTimer( pdr, &status, 30 );
	ATMDEBUG(DAR_DBUG_FULL, "atmStopDirective: starting timer%c\n", ' ');
    }
    else
    {
	ATMDEBUG(DAR_DBUG_FULL, "atmStopDirective: bus was already busy%c\n", ' ');
    }
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmTestMode
 *
 * INVOCATION:
 * status = atmTestMode (pdr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute a MODE of TEST
 *
 * DESCRIPTION:
 * Execute a MODE of TEST by calling atmStartMode().
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

static long atmTestMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr		/* (in)  Ptr to ass. rec.	*/
)
{
    long 	status = DAR_S_SUCCESS;	/* Returned function status.	*/


    ATMDEBUG(DAR_DBUG_FULL, "atmTestMode: entry%c\n", ' ');

    status = atmStartMode( pdr, "test" );
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmTrackMode
 *
 * INVOCATION:
 * status = atmTrackMode (pdr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute a MODE of TRACK
 *
 * DESCRIPTION:
 * Execute a MODE of TRACK by calling atmStartMode().
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

static long atmTrackMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*pdr		/* (in)  Ptr to ass. rec.	*/
)
{
    long 	status = DAR_S_SUCCESS;	/* Returned function status.	*/


    ATMDEBUG(DAR_DBUG_FULL, "atmTrackMode: entry%c\n", ' ');

    status = atmStartMode( pdr, "track" );
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * atmTranslateDir
 *
 * INVOCATION:
 * status = atmTranslateDir (pdr); 
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

static long atmTranslateDir
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
 * atmProcessFault
 *
 * INVOCATION:
 * status = atmProcessFault( pdr );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
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

static long atmProcessFault(ASSEMBLY_CONTROL_RECORD *pdr)
{
    DEV_ASS_CONTROL_PRIVATE *pDevAss;   /* Ptr to private dev. struct.	*/
    long status = DAR_S_SUCCESS;	/* Function status.		*/
    
    pDevAss = ( DEV_ASS_CONTROL_PRIVATE *) assGetPrivateStruct( pdr );

/* Is a command active? If not, do nothing */
    if (pDevAss->cmdActive)
     {
/* Finish the command */
       DEBUG(DAR_DBUG_MIN, "<%d> %s:atmProcessFault: interlock occurred%c\n",' ');
       status = DAR_E_FLT_CLR;
       assCommandFinish( pdr, status, "Interlock occurred" );
       pDevAss->cmdActive = FALSE;
     }
     
     return ( status );
      
}
    
    

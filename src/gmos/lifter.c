static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id$"};

/* ===================================================================== */
/* INDENT OFF */
/*+
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
 * -------- 
 * lifter.c
 * 
 * PURPOSE
 * -------
 * Control the raising and lowering of the GMOS indexer turret
 * 
 * FUNCTION NAME(S)
 * ----------------
 * lifterState     - determine state of lifter from raised/engaged switches
 * setValves       - control raise and lower valves
 * lifterInit      - initialise genSub record
 * lifterControl   - state machine to sequence lifter actions
 *
 * DEPENDENCIES
 * ------------
 *
 * LIMITATIONS
 * -----------
 *
 * BUGS
 * ----
 * 
 * AUTHORS
 * -------
 * Sean Prior  (srp@roe.ac.uk)
 * Steven Beard (smb@roe.ac.uk)
 * 
 * HISTORY
 * -------
 * 
 * 21-Jul-1999: Original.                                                   (srp)
 * 19-Nov-1999: Modified to work with real hardware and new solenoid.
 *              Original had active/inactive signals reversed.              (smb)
 * 25-Jan-2000: Modified to use correct EPICS watchdog/callback utilities
 *              rather than relying on a crude external sequence record.    (smb)
 *
 */
/* INDENT ON */
/* ===================================================================== */


#include <vxWorks.h>
#include <logLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <wdLib.h>
#include <callback.h>
#include <math.h>
#include <dbDefs.h>
#include <recSup.h>
#include <genSubRecord.h>
#include <string.h>
#include <stdio.h>
#include "lifter.h"        /* Contains GR_ symbolic constants */


/* define externals */

/* define globals */

struct callback {
    CALLBACK      callback;
    dbCommon      *pRecord;
    WDOG_ID       wd_id;
};

typedef struct
{
    SEM_ID      cmdLock;		/* Timer semaphore.		*/
    long        timeout;		/* Private timeout flag.	*/
    void        *timer;			/* Timer callback info.		*/
}  LIFTER_PRIVATE;

char *lifterDemandName[] =
{
    "OFF ", 
    "DOWN", 
    "UP  ", 
    "STOP",
    "EMERGENCY UP",
    NULL
};

char *lifterStateName[] =
{
    "ENGAGED", 
    "RAISED ", 
    "MOVING ", 
    "ERROR  ",
    "INIT   ",
    NULL
};

char *lifterActionName[] =
{
    "RAISING ", 
    "LOWERING", 
    "IDLE    ", 
    "EMERGENCY RAISE",
    NULL
};


/* Global variables */

int lifterDebug = LIFTER_DBUG_NONE;       /* Lifter debugging level. LIFTER_DBUG_NONE, MIN FULL or MAX. */

static long currentAction = GR_ACTION_IDLE;
static long previousState;
static long previousDmd;

/* Local function prototypes */

long        lifterState(long upSwitch, long downSwitch);
long        setValves (int demand, struct genSubRecord *pgsub);
long        lifterInit (struct genSubRecord * pgsub);
long        lifterControl (struct genSubRecord * pgsub);
static void lifterCallback( struct callback *);
void        callbackRequest();

/* ===================================================================== */
/* INDENT OFF */
/*
 * Function name:
 * lifterInit
 * lifterControl
 * 
 * Purpose:
 * Control the raising and lowering of the GMOS indexer turret
 *
 * Invocation:
 * struct genSubRecord *pgsub
 * status = lifterControl(struct genSubRecord *pgsub)
 * 
 * Parameters in:
 *		> pgsub->a	long	GR_LIFTER_UP/GR_LIFTER_DOWN/GR_LIFTER_OFF request
 *		> pgsub->b	long	turret raised microswitch (0 = active)
 *		> pgsub->c	long	turret lowered microswitch (0 = active)
 *		> pgsub->d	long	interlock active (0 = active)
 *		> pgsub->e	long	upstream pressure OK? (0 = ok)
 *		> pgsub->f	long	downstream pressure OK? (0 = ok)
 *              > pgsub->g	long	master enable (1 = enabled)
 *              > pgsub->h	long	simulation flag (1 = simulating)
 *              > pgsub->i	long	turret power status (1 = power on)
 * 
 * Parameters out:
 *		< pgsub->vala	long	command to activate lifter raise valve (0 = active)
 *		< pgsub->valb	long	command to activate lifter lower valve (0 = active)
 *		< pgsub->valc	long    state of lifter system
 *		< pgsub->vald	long    interlock for turret rotation
 *		< pgsub->vale	long    command reset output
 *		< pgsub->valf	string  message
 *
 * Return value:
 *		< status	long	OK or ERROR
 * 
 * Globals: 
 *	External functions:
 *	None
 * 
 *	External variables:
 *	        currentAction   long     GR_LIFTER_ENGAGED, etc...
 * 
 * Requirements:
 * It is assumed the VALE field of the record is wired back to the EPICS record
 * storing the command applied to the A field.
 * 
 * Author:
 * Sean Prior  (srp@roe.ac.uk)
 * Steven Beard (smb@roe.ac.uk)
 * 
 * History:
 * 21-Jul-1999: Original                                                       (srp)
 * 19-Nov-1999: Corrected active and inactive signals and corrected algorithm. (smb)
 * 22-Nov-1999: Only send interlock to turret when master enable is active,
 *              otherwise turret can never initialise.                         (smb)
 * 07-Mar-2000: Ignore the air pressure status in simulation mode.             (smb)
 * 
 */

/* INDENT ON */
/* ===================================================================== */

long lifterInit (struct genSubRecord * pgsub)
{

     long          status;
     LIFTER_PRIVATE *pPriv = NULL;
     struct callback 
                   *pcallback = NULL;      /* Callback structure ptr. */

     status = OK;

     if (lifterDebug >= LIFTER_DBUG_MAX)
     {
         logMsg("LifterInit: Entry\n", 0,0,0,0,0,0);
     }

/*
 * Create a private structure for this record.
 */

     pPriv = (LIFTER_PRIVATE *) malloc (sizeof(LIFTER_PRIVATE));
     if ( pPriv == NULL ) 
     {
        printf ("lifterInit: Failed to malloc device private structure.\n");
        status = ERROR;
        return( status );
     }
     pgsub->dpvt = (void *) pPriv;

/*
 * Initialise the private structure
 */

     pPriv->timeout = FALSE;
     pPriv->timer = NULL;

/*
 *  Create the MUTEX semaphore to protect the timeout in the
 *  private structure during asynchronous callback access.
 */
     
     if ( (pPriv->cmdLock = semBCreate( SEM_Q_FIFO, SEM_FULL) ) == NULL )
     {
        printf ("lifterInit: Failed to create cmdLock semaphore.\n");
        status = ERROR;
        return( status );
     }

/*
 *  Make room for the timer callback structure and set it up.
 */

     pcallback = (struct callback *)(calloc(1,sizeof(struct callback)));
     if ( pcallback == NULL )
     {
        printf ("lifterInit: Failed to calloc callback structure.\n");
        status = ERROR;
        return ( status );
     }

     pPriv->timer = (void *)pcallback;
  
     callbackSetCallback((CALLBACKFUNC)lifterCallback,&pcallback->callback);
     pcallback->pRecord = (struct dbCommon *)pgsub;

     if ( ( pcallback->wd_id = wdCreate() ) == NULL )
     {
         printf ("lifterInit: Failed to create watchdog.\n");
         status = ERROR;
         return( status );
     }
     callbackSetPriority( priorityLow, &pcallback->callback );


     /* Ensure the external demand is OFF on startup */

     previousDmd = GR_LIFTER_OFF;
     *(long *)pgsub->vale = GR_LIFTER_OFF;

     /* Ensure the solenoid valves are both OFF on startup */

     setValves (GR_LIFTER_OFF, pgsub);
     currentAction = GR_ACTION_IDLE;

     /* Initialise the output state and the previous state
      */

     previousState = GR_LIFTER_INIT;
     *(long *)pgsub->valc = GR_LIFTER_INIT;

     return (status);
}

/* --------------------------------------------------------------------- */

long lifterControl (struct genSubRecord * pgsub)
{
     long       lifterDmd;		/* Lifter demand.				*/
     long       upSwitch, downSwitch;	/* Status of up and down microswitches.		*/
     long       state;			/* Lifter state derived from microswitches.	*/
     long       interlock;		/* Interlock demanded flag.			*/
     long       upstreamOK, downstreamOK; /* Status of air pressure sensors.		*/
     long       masterEnable;		/* Master enable on flag.			*/
     long       simulate;		/* Simulation mode flag.			*/
     long       turretPower;		/* Turret power status.				*/
     LIFTER_PRIVATE 
                *pPriv;			/* Pointer to lifter device private structure.	*/
     struct callback
                *pCallback;		/* Structure passed to callback watchdog.	*/
     int        waitTime;		/* Time delay given to callback watchdog.	*/
     long       callbackFlag;		/* Callback required flag.			*/
     long       status;			/* Return status.				*/

     static int counter = 0;		/* Counter used to limit number of cycles.	*/

     if (lifterDebug >= LIFTER_DBUG_MAX)
     {
        logMsg("\nLifterControl: Entry\n", 0,0,0,0,0,0);
     }

    /*
     * Extract the address of the device private structure from the gensub record
     * structure, then extract the address of the callback structure from the device
     * private structure. Abort if either of these is NULL.
     */

     pPriv = (LIFTER_PRIVATE *) (pgsub->dpvt);
     if ( pPriv == NULL )
     {
         printf ("lifter: NULL pPriv pointer.\n");
         return (ERROR);
     }

     pCallback = (struct callback *)(pPriv->timer);
     if ( pCallback == NULL )
     {
         printf ("lifter: NULL pCallback pointer.\n");
         return (ERROR);
     }

     status = OK;
     callbackFlag = 0;


     /* Read the interlock state and lifter demand to local variables. */

     lifterDmd = *(long *) pgsub->a;
     interlock = *(long *) pgsub->d;

     /* check for interlock or stop demand */

     if ( (interlock != 0) || (lifterDmd == GR_LIFTER_STOP) )
     {
          /* Switch both valves off */
          setValves (GR_LIFTER_OFF, pgsub);
          currentAction = GR_ACTION_IDLE;

          /* Ensure the external demand is switched OFF */

          *(long *)pgsub->vale = GR_LIFTER_OFF;

          strncpy (pgsub->valf, "Emergency stop/Interlock", MAX_STRING_SIZE-1);

          if (lifterDebug >= LIFTER_DBUG_NONE)
          {
              logMsg("--- Lifter emergency stop activated ---\n", 0,0,0,0,0,0);
          }

          /* Cancel any outstanding callbacks. Ignore the status returned. */
          wdCancel( pCallback->wd_id );

          return (ERROR);
     }

     /* Read input switches to local variables */

     upSwitch     = *(long *) pgsub->b;
     downSwitch   = *(long *) pgsub->c;
     upstreamOK   = *(long *) pgsub->e;
     downstreamOK = *(long *) pgsub->f;
     masterEnable = *(long *) pgsub->g;
     simulate     = *(long *) pgsub->h;
     turretPower  = *(long *) pgsub->i;

     /* Obtain the current state, based on the UP and DOWN switches. */

     state = lifterState(upSwitch, downSwitch);

     if (lifterDebug >= LIFTER_DBUG_FULL)
     {
          logMsg("Lifter: state=%s, previousState=%s, action=%s, demand=%s, previousDmd=%s\n",
      	         (int) lifterStateName[state], (int) lifterStateName[previousState],
                 (int) lifterActionName[currentAction], (int) lifterDemandName[lifterDmd],
                 (int) lifterDemandName[previousDmd],0);

          if (lifterDebug >= LIFTER_DBUG_MAX)
          {
               logMsg("Lifter: upstreamOK=%d, downstreamOK=%d, masterEnable=%d\n",
                      (int) upstreamOK, (int) downstreamOK, (int) masterEnable, 0,0,0);
          }
     }

     /*
      * The lifter will not work unless the air pressure is within acceptable limits,
      * as sensed on the "upstream pressure OK" and "dowstream pressure OK" lines.
      */

     if ( (upstreamOK != GR_PRESSURE_OK ) || (downstreamOK != GR_PRESSURE_OK) )
     {
          /* Switch both valves off */
          setValves (GR_LIFTER_OFF, pgsub);
          currentAction = GR_ACTION_IDLE;

          strncpy (pgsub->valf, "Air pressure failure", MAX_STRING_SIZE-1);

          /* Ensure the external demand is switched OFF */

          *(long *)pgsub->vale = GR_LIFTER_OFF;

          if (lifterDebug >= LIFTER_DBUG_NONE)
          {
               logMsg("--- Air pressure problem - lifter disabled ---\n", 0,0,0,0,0,0);
          }

          /* Cancel any outstanding callbacks. Ignore the status returned. */
          wdCancel( pCallback->wd_id );

          return (ERROR);
     }

     /*
      * When not simulating and there is a lifter demand,
      * the lifter will not activate unless turret power is on.
      */

     if ( !simulate )
     {
          if ((turretPower != GR_TURRET_POWER_STAT_ON) && (lifterDmd != GR_LIFTER_OFF))
          {
               /* Switch both valves off */
               setValves (GR_LIFTER_OFF, pgsub);
               currentAction = GR_ACTION_IDLE;

               strncpy (pgsub->valf, "Turret power OFF or turret BUSY", MAX_STRING_SIZE-1);

               /* Ensure the external demand is switched OFF */

               *(long *)pgsub->vale = GR_LIFTER_OFF;

               if (lifterDebug >= LIFTER_DBUG_NONE)
               {
                   logMsg("--- Turret power OFF or turret BUSY - lifter action ignored ---\n", 0,0,0,0,0,0);
               }

               /* Cancel any outstanding callbacks. Ignore the status returned. */
               wdCancel( pCallback->wd_id );

               return (ERROR);
          }
     }
     else
     {
          if (lifterDebug >= LIFTER_DBUG_MIN)
          {
               logMsg("Lifter: Turret power check skipped in simulation mode\n", 0,0,0,0,0,0);
          }
     }

     /*
      * The lifter operates like a state machine. The way it responds to given demands
      * depends on the state it is in (apart from the emergency stop and interlock demands
      * which have already been dealt with).
      */

     switch(state)
     {
     case GR_LIFTER_ENGAGED:

          /*
           * The lifter is fully down. An UP demand will start the lifter raising.
           * A DOWN demand is ignored.
           */

          previousState = state;
          *(long *)pgsub->valc = GR_LIFTER_ENGAGED;

          counter = 0;
          if (lifterDebug >= LIFTER_DBUG_MAX)
          {
               logMsg("Lifter: counter=%d\n", counter, 0,0,0,0,0);
          }

          if (lifterDmd == GR_LIFTER_UP)
          {
               strncpy (pgsub->valf, "UP when engaged", MAX_STRING_SIZE-1);
               if (lifterDebug >= LIFTER_DBUG_MIN) {
                    logMsg("Lifter: UP demand when engaged\n", 0,0,0,0,0,0);
               }
               setValves (GR_LIFTER_UP, pgsub);
               currentAction = GR_ACTION_RAISING;

               /* Prevent the turret from moving */
               *(long *)pgsub->vald = 1;

               /* A callback is needed */
               callbackFlag = 1;
          }
          else if ( (lifterDmd == GR_LIFTER_OFF) && (previousDmd == GR_LIFTER_UP) &&
                    (previousState == GR_LIFTER_ENGAGED) )
          {
               /*
                * The lifter did not move at all. This could be caused by an air supply
                * failure not detected by the air pressure status switches or by a switch
                * failure.
                */

               strncpy (pgsub->valf, "Lifter did not move", MAX_STRING_SIZE-1);
               if (lifterDebug >= LIFTER_DBUG_MIN)
               {
                    logMsg("Lifter: still engaged after UP demand\n", 0,0,0,0,0,0);
               }
               setValves (GR_LIFTER_OFF, pgsub);
               currentAction = GR_ACTION_IDLE;

               /* Reset the external demand to OFF */
               *(long *)pgsub->vale = GR_LIFTER_OFF;

               /* Prevent the turret from moving */
               *(long *)pgsub->vald = 1;

               previousState = state;
               *(long *)pgsub->valc = GR_LIFTER_ERROR;
               status = ERROR;
          }
          else
          {
               strncpy (pgsub->valf, "IDLE", MAX_STRING_SIZE-1);
               if (lifterDebug >= LIFTER_DBUG_MIN)
               {
                    logMsg("Lifter: IDLE when engaged\n", 0,0,0,0,0,0);
               }
               setValves (GR_LIFTER_OFF, pgsub);
               currentAction = GR_ACTION_IDLE;

               /* Reset the external demand to OFF */
               *(long *)pgsub->vale = GR_LIFTER_OFF;

               /* Prevent the turret from moving */
               *(long *)pgsub->vald = 1;
          }
          break;

     case GR_LIFTER_RAISED:

          /*
           * The lifter is fully raised. A DOWN demand will start the lifter lowering.
           * An UP demand is ignored.
           */

          previousState = state;
          *(long *)pgsub->valc = GR_LIFTER_RAISED;

          counter = 0;
          if (lifterDebug >= LIFTER_DBUG_MAX)
          {
               logMsg("Lifter: counter=%d\n", counter, 0,0,0,0,0);
          }

          if (lifterDmd == GR_LIFTER_DOWN)
          {
               strncpy (pgsub->valf, "DOWN when raised", MAX_STRING_SIZE-1);
               if (lifterDebug >= LIFTER_DBUG_MIN)
               {
                    logMsg("Lifter: DOWN demand when raised\n", 0,0,0,0,0,0);
               }
               setValves (GR_LIFTER_DOWN, pgsub);
               currentAction = GR_ACTION_LOWERING;

               /* Prevent the turret from moving */
               *(long *)pgsub->vald = 1;

               /* A callback is needed */
               callbackFlag = 1;
          }
          else if ( (lifterDmd == GR_LIFTER_OFF) && (previousDmd == GR_LIFTER_DOWN) &&
                    (previousState == GR_LIFTER_RAISED) )
          {

               /*
                * The lifter did not move at all. This could be caused by an air supply
                * failure not detected by the air pressure status switches or by a switch
                * failure.
                */

               strncpy (pgsub->valf, "Lifter did not move", MAX_STRING_SIZE-1);
               if (lifterDebug >= LIFTER_DBUG_MIN)
               {
                    logMsg("Lifter: still raised after DOWN demand\n", 0,0,0,0,0,0);
               }
               setValves (GR_LIFTER_OFF, pgsub);
               currentAction = GR_ACTION_IDLE;

               /* Reset the external demand to OFF */
               *(long *)pgsub->vale = GR_LIFTER_OFF;

               /* Prevent the turret from moving */
               *(long *)pgsub->vald = 1;

               previousState = state;
               *(long *)pgsub->valc = GR_LIFTER_ERROR;
               status = ERROR;
          }
          else
          {
               strncpy (pgsub->valf, "IDLE", MAX_STRING_SIZE-1);
               if (lifterDebug >= LIFTER_DBUG_MIN)
               {
                    logMsg("Lifter: IDLE when raised\n", 0,0,0,0,0,0);
               }
               setValves (GR_LIFTER_OFF, pgsub);
               currentAction = GR_ACTION_IDLE;

               /* Reset the external demand to OFF */
               *(long *)pgsub->vale = GR_LIFTER_OFF;

               /* The turret can move */
               *(long *)pgsub->vald = 0;

               /* There should be an error if the lifter got to the RAISED state
                * through an emergency raise
                */

               if ( previousDmd == GR_LIFTER_EUP )
               {
                    strncpy (pgsub->valf, "IDLE after FAILURE", MAX_STRING_SIZE-1);
                    status = ERROR;
               }
          }
          break;

     case GR_LIFTER_MOVING:

          /*
           * The lifter is neither up nor down. This is assumed to be an intermediate state
           * that the lifter passes through while it is raising or lowering. Only a limited number
           * of passes through the moving state are allowed until the lifter times out.
           */

          counter++;
          if (lifterDebug >= LIFTER_DBUG_MAX)
          {
               logMsg("Lifter: counter=%d\n", counter, 0,0,0,0,0);
          }
          if ( counter > (GR_LIFTER_MAX_CYCLES-1) )
          {

               /*
                * The lifter has timed out. What happens next depends on the current demand
                * If the lifter has failed to raise it is stopped. If the lifter has failed to lower
                * it is automatically raised again, as it may have come to rest in a dangerous state
                * on the guard ring.
                */

               counter = 0;
               if (lifterDebug >= LIFTER_DBUG_NONE)
               {
                    logMsg("Lifter: Timeout, demand=%d, action=%d\n",
                           (int) lifterDmd, (int) currentAction, 0,0,0,0);
               }

               /*
                * Switch off the demand if the lifter was raising.
                * Reverse the direction and go back up if the lifter was lowering.
                */

               if ( (currentAction == GR_ACTION_RAISING) ||
                    (currentAction == GR_ACTION_EMERGENCY_RAISE) )
               {

                    /* Switch off the demand */
                    *(long *)pgsub->vale = GR_LIFTER_OFF;
                    lifterDmd = GR_LIFTER_OFF;
                    currentAction = GR_ACTION_IDLE;
               }
               else
               {
                    /* Emergency raise */
                    *(long *)pgsub->vale = GR_LIFTER_EUP;
                    lifterDmd = GR_LIFTER_EUP;
                    currentAction = GR_ACTION_EMERGENCY_RAISE;
               }

               status = ERROR;
          }


          if ((lifterDmd == GR_LIFTER_DOWN) ||
              ((lifterDmd == GR_LIFTER_OFF) && (currentAction == GR_ACTION_LOWERING)))
          {
               /* If the current action is raising the valves need to be switched
                * off briefly before reversing them. The valves will be set to
                * GR_LIFTER_DOWN at the next call back.
                */

               sprintf (pgsub->valf, "Moving DOWN - %d", counter);
               if (lifterDebug >= LIFTER_DBUG_MIN)
               {
                    logMsg("Lifter: DOWN when moving, demand=%d, action=%d\n", (int) lifterDmd,
                           (int) currentAction, 0,0,0,0);
               }

/* COMMENTED OUT - In an emergency the valves should be reversed immediately. */
#ifdef COMMENT
               if (currentAction == GR_ACTION_RAISING)
                    setValves (GR_LIFTER_OFF, pgsub);
               else
#endif
                    setValves (GR_LIFTER_DOWN, pgsub);

               currentAction = GR_ACTION_LOWERING;

               /* Prevent the turret from moving */
               *(long *)pgsub->vald = 1;

               previousState = state;
               *(long *)pgsub->valc = GR_LIFTER_MOVING;

               /* A callback is needed */
               callbackFlag = 1;
          }
          else if ((lifterDmd == GR_LIFTER_UP) ||
                   ((lifterDmd == GR_LIFTER_OFF) && currentAction == GR_ACTION_RAISING))
          {
               /* If the current action is lowering the valves need to be switched
                * off briefly before reversing them. The valves will be set to
                * GR_LIFTER_UP at the next call back.
                */

               sprintf (pgsub->valf, "Moving UP - %d", counter);
               if (lifterDebug >= LIFTER_DBUG_MIN)
               {
                    logMsg("Lifter: UP when moving, demand=%d, action=%d\n", (int) lifterDmd,
                           (int) currentAction, 0,0,0,0);
               }

/* COMMENTED OUT - In an emergency the valves should be reversed immediately. */
#ifdef COMMENT
               if (currentAction == GR_ACTION_LOWERING)
                    setValves (GR_LIFTER_OFF, pgsub);
               else
#endif
                    setValves (GR_LIFTER_UP, pgsub);

               currentAction = GR_ACTION_RAISING;

               /* Prevent the turret from moving */
               *(long *)pgsub->vald = 1;

               previousState = state;
               *(long *)pgsub->valc = GR_LIFTER_MOVING;

               /* A callback is needed */
               callbackFlag = 1;
          }
          else if ((lifterDmd == GR_LIFTER_EUP) ||
                   ((lifterDmd == GR_LIFTER_OFF) && currentAction == GR_ACTION_EMERGENCY_RAISE))
          {
               /* If the current action is lowering the valves need to be switched
                * off briefly before reversing them. The valves will be set to
                * GR_LIFTER_UP at the next call back.
                */

               sprintf (pgsub->valf, "Emergency UP - %d", counter);
               if (lifterDebug >= LIFTER_DBUG_MIN) {
                    logMsg("Lifter: EMERGENCY UP when moving, demand=%d, action=%d\n", (int) lifterDmd,
                           (int) currentAction, 0,0,0,0);
               }

/* COMMENTED OUT - In an emergency the valves should be reversed immediately. */
#ifdef COMMENT
               if (currentAction == GR_ACTION_LOWERING)
                    setValves (GR_LIFTER_OFF, pgsub);
               else
#endif
                    setValves (GR_LIFTER_UP, pgsub);

               currentAction = GR_ACTION_EMERGENCY_RAISE;

               /* Prevent the turret from moving */
               *(long *)pgsub->vald = 1;

               previousState = state;
               *(long *)pgsub->valc = GR_LIFTER_MOVING;

               /* A callback is needed */
               callbackFlag = 1;

               /* An emergency raise is still an error. */
               status = ERROR;
          }
          else
          {
               /*
                * The lifter is in an in-between state with no demand and no current action.
                * The lifter may have got stuck or one or more switches may have failed.
                */

               strncpy (pgsub->valf, "Lifter stuck or switch error", MAX_STRING_SIZE-1);
               if (lifterDebug >= LIFTER_DBUG_NONE)
               {
                    logMsg ("LIFTER ERROR: Lifter stopped neither raised nor lowered.\n", 0,0,0,0,0,0);
               }

               setValves (GR_LIFTER_OFF, pgsub);
               currentAction = GR_ACTION_IDLE;

               /* Prevent the turret from moving */
               *(long *)pgsub->vald = 1;

               previousState = state;
               *(long *)pgsub->valc = GR_LIFTER_ERROR;
               status = ERROR;
          }
          break;

     case GR_LIFTER_ERROR:

          /*
           * The lifter switches are suggesting an impossible state (e.g. up and down at the
           * same time. This is probably due to a switch error.
           */

          strncpy (pgsub->valf, "Invalid lifter state - switch error?", MAX_STRING_SIZE-1);
          if (lifterDebug >= LIFTER_DBUG_NONE)
          {
               logMsg ("LIFTER ERROR: Invalid lifter state; upSwitch=%d, downSwitch=%d.\n",
                       (int) upSwitch, (int) downSwitch, 0,0,0,0);
          }

          setValves (GR_LIFTER_OFF, pgsub);
          currentAction = GR_ACTION_IDLE;

          /* Prevent the turret from moving */
          *(long *)pgsub->vald = 1;

          previousState = state;
          *(long *)pgsub->valc = GR_LIFTER_ERROR;
          status = ERROR;

          break;

     default:

          /*
           * The software should never get here, but you never know...
           * Treat it as a lifter error state.
           */

          strncpy (pgsub->valf, "Undefined lifter state", MAX_STRING_SIZE-1);
          if (lifterDebug >= LIFTER_DBUG_NONE)
          {
               logMsg ("LIFTER ERROR: Undefined state, %d.\n", (int) state, 0,0,0,0,0);
          }

          setValves (GR_LIFTER_OFF, pgsub);
           currentAction = GR_ACTION_IDLE;

          /* Prevent the turret from moving */
          *(long *)pgsub->vald = 1;

          previousState = state;
          *(long *)pgsub->valc = GR_LIFTER_ERROR;
          status = ERROR;
     }

/*
 * Remember the previous demand when called back.
 */
     previousDmd = lifterDmd;

/*
 * If a callback is needed, start a timer to call back the record.
 */

     if ( callbackFlag != 0 )
     {
          waitTime = (int) (GR_LIFTER_CYCLE_TIME * sysClkRateGet() );

          if(  wdStart(pCallback->wd_id, waitTime, (FUNCPTR)callbackRequest, 
                       (int)pCallback) == ERROR )
          {
               logMsg ("LIFTER ERROR: Failed to start watchdog for callback.\n", 0,0,0,0,0,0);
               status = ERROR;
          }
     }

     return (status);
}

/* ===================================================================== */
/* INDENT OFF */
/*
 * Function name:
 * lifterState
 * 
 * Purpose:
 * Examine raised/lowered switches to determine current lifter state
 *
 * Invocation:
 * status = lifterState(int upSwitch, int downSwitch)
 * 
 * Parameters in:
 *		> upSwitch	int	status of turret raised switch (0 = active)
 *		> downSwitch	int	status of turret lowered switch (0 = active)
 *
 * Parameters out:
 *      None
 *
 * Return value:
 *		< status	int	OK or ERROR
 * 
 * Globals: 
 *	External functions:
 *	None
 * 
 *	External variables:
 *	None
 * 
 * Requirements:
 * None
 * 
 * Author:
 * Sean Prior  (srp@roe.ac.uk)
 * Steven Beard (smb@roe.ac.uk)
 * 
 * History:
 * 21-Jul-1999:	Original (srp)
 * 19-Nov-1999: Corrected active and inactive signals (smb)
 * 
 */

/* INDENT ON */
/* ===================================================================== */

long lifterState(long upSwitch, long downSwitch)
{
     if ((upSwitch == GR_SWITCH_INACTIVE) && (downSwitch == GR_SWITCH_INACTIVE))

          return (GR_LIFTER_MOVING);

     else if ((upSwitch == GR_SWITCH_INACTIVE) && (downSwitch == GR_SWITCH_ACTIVE))

          return (GR_LIFTER_ENGAGED);

     else if ((upSwitch == GR_SWITCH_ACTIVE)  && (downSwitch == GR_SWITCH_INACTIVE))

          return (GR_LIFTER_RAISED);

     else
          return (GR_LIFTER_ERROR);
}

/* ===================================================================== */
/* INDENT OFF */
/*
 * Function name:
 * setValves
 * 
 * Purpose:
 * Provide control signals to the pnuematic raise and lower valves
 *
 * Invocation:
 * status = setValves (int demand, struct genSubRecord *pgsub)
 * 
 * Parameters in:
 *		> demand	int	GR_LIFTER_UP/GR_LIFTER_DOWN/GR_LIFTER_OFF request
 *		> *pgsub        pointer	pointer to calling gensub
 * 
 * Parameters out:
 *      None
 *
 * Return value:
 *		< status	long	OK or ERROR
 * 
 * Globals: 
 *	External functions:
 *	None
 * 
 *	External variables:
 *	None
 * 
 * Requirements:
 * None
 * 
 * Author:
 * Sean Prior   (srp@roe.ac.uk)
 * Steven Beard (smb@roe.ac.uk)
 * 
 * History:
 * 21-Jul-1999:	Original (srp)
 * 19-Nov-1999: Corrected active and inactive signals (smb)
 * 
 */

/* INDENT ON */
/* ===================================================================== */

long setValves (int demand, struct genSubRecord *pgsub)
{
     if (demand == GR_LIFTER_UP)
     {
          *(long *) pgsub->vala = GR_VALVE_ACTIVE;
          *(long *) pgsub->valb = GR_VALVE_INACTIVE;
     }
     else if (demand == GR_LIFTER_DOWN)
     {
          *(long *) pgsub->vala = GR_VALVE_INACTIVE;
          *(long *) pgsub->valb = GR_VALVE_ACTIVE;
     }
     else /* Assume GR_LIFTER_OFF */
     {
          *(long *) pgsub->vala = GR_VALVE_INACTIVE;
          *(long *) pgsub->valb = GR_VALVE_INACTIVE;
     }
     return (OK);
}


/* ===================================================================== */
/* INDENT OFF */
/*
 * Function name:
 * swStateInit
 * swState
 * 
 * Purpose:
 * Put together all of the turret switches into one string
 *
 * Invocation:
 * struct genSubRecord *pgsub
 * status = swState(struct genSubRecord *pgsub)
 * 
 * Parameters in:
 *		> pgsub->a	long	turret in position 1
 *		> pgsub->b	long	turret in position 2
 *		> pgsub->c	long	turret in position 3
 *		> pgsub->d	long	turret in position 4
 * 
 * Parameters out:
 *		< pgsub->vala	string	string of combined status
 *              < pgsub->valb   long    grating in beam flag
 *
 * Return value:
 *		< status	long	OK or ERROR
 * 
 * Globals: 
 *	External functions:
 *	None
 * 
 *	External variables:
 *	None
 * 
 * Requirements:
 * The inputs are assumed to be logical switch states which are GR_SWITCH_ACTIVE or GR_SWITCH_INACTIVE
 * 
 * Author:
 * Sean Prior  (srp@roe.ac.uk)
 * Steven Beard (smb@roe.ac.uk)
 * 
 * History:
 * 2-Aug-1999:  Original                                                   (srp)
 * 19-Nov-1999: Initialise output in swStateInit. Declare scratchbuff
 *              a larger fixed size to avoid memory corruption.            (smb)
 * 26-Jun-2000: Output "grating in beam" flag.                             (smb)
 * 
 */

/* INDENT ON */
/* ===================================================================== */

long swStateInit (struct genSubRecord * pgsub)
{
     strncpy (pgsub->vala, "XXXX", MAX_STRING_SIZE-1);
     *(long *)pgsub->valb = 0;
     return (OK);
}

long swState (struct genSubRecord * pgsub)
{
     char scratchBuff[MAX_STRING_SIZE];
     long pos1, pos2, pos3, pos4;

     /* Read the "in position" switch states */ 

     pos1 = *(long *)pgsub->a;
     pos2 = *(long *)pgsub->b;
     pos3 = *(long *)pgsub->c;
     pos4 = *(long *)pgsub->d;

     sprintf (scratchBuff, "%d%d%d%d", (pos4==GR_SWITCH_ACTIVE), (pos3==GR_SWITCH_ACTIVE),
        (pos2==GR_SWITCH_ACTIVE), (pos1==GR_SWITCH_ACTIVE));

     strncpy (pgsub->vala, scratchBuff, MAX_STRING_SIZE-1);

     if ( (pos1 == GR_SWITCH_ACTIVE) || (pos2 == GR_SWITCH_ACTIVE) ||
          (pos3 == GR_SWITCH_ACTIVE) || (pos4 == GR_SWITCH_ACTIVE) )
     {
          *(long *)pgsub->valb = 1;
     }
     else
     {
          *(long *)pgsub->valb = 0;
     }

     return (OK);
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * lifterCallback
 *
 * INVOCATION:
 * lifterCallback ( pcallback);
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
 ************************************************************************
 */
static void lifterCallback(struct callback *pcallback      /* (in)  Ptr to callback struct.*/)
{
    struct genSubRecord
                *pGensub;	/* Ptr to gensub. rec.		*/
    LIFTER_PRIVATE 
                *pPriv;		/* Ptr to private.		*/
    int         waitTime;	/* Time to wait.		*/

    if (lifterDebug >= LIFTER_DBUG_MAX)
    {
        logMsg ("lifterCallback: Entry\n", 0,0,0,0,0,0);
    }

    /*
     * Extract the address of the gensub record structure from the callback structure,
     * then extract the address of the device private structure from the gensub structure.
     * Abort if either of these is NULL.
     */

    pGensub = (struct genSubRecord *) pcallback->pRecord;
    if ( pGensub == NULL )
    {
        logMsg ("lifterCallback: pGensub is NULL\n", 0,0,0,0,0,0);
        return;
    }

    pPriv = pGensub->dpvt;
    if ( pPriv == NULL )
    {
        logMsg ("lifterCallback: pPriv is NULL\n", 0,0,0,0,0,0);
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
            logMsg ("lifterCallback: timer is NULL\n", 0,0,0,0,0,0);
            return;
        }

        if (lifterDebug >= LIFTER_DBUG_MAX)
        {
            logMsg ("lifterCallback: Record busy - retry\n", 0,0,0,0,0,0);
        }

        waitTime = (int) (GR_LIFTER_SHORT_TIME * sysClkRateGet() );
        if( pcallback->wd_id == NULL ) 
        {
            if ( ( pcallback->wd_id = wdCreate() ) == NULL )
            {
                logMsg ("lifterCallback: No room for watchdog timer\n", 0,0,0,0,0,0);
                return;
            }
        }

        callbackSetPriority(priorityLow, &pcallback->callback);
        if(  wdStart(pcallback->wd_id, waitTime, (FUNCPTR)callbackRequest, 
                (int)pcallback) == ERROR )
        {
                logMsg ("lifterCallback: Failed to restart watchdog timer\n", 0,0,0,0,0,0);
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

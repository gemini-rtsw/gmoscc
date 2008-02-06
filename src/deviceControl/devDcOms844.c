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
 * devDcOms844.c
 *
 * PURPOSE:
 * EPICS OMS-8 and OMS-44 Device Support code for the deviceControl record.
 *
 * FUNCTION NAME(S)
 * configureDrive       Set motor velocity, acceleration and base velocity
 * controlMotion        Start, stop or abort a motion
 * controlPower         Enable or disable the motor power via OMS AUX bits
 * devBoInit            Initialize the binary output record support module
 * devBoInitRec         Initialize an instance of binary output record support
 * devBoControlPwer     Enable / disable the motor power via binaryOutput rec
 * devInit              Initialize the device control record support module
 * devInitRec           Initialize an instance of deviceControl record support
 * omsScanTask          Monitor device motion
 * setDelay             Re-process the calling record after a given interval
 * setPosition          Load the motor position counter with given value
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.5  2006/03/07 20:50:35  gemvx
 * implemented badRead to ignore spikes in encoder position caused by interference
 * which last for no more than one scan cycle.
 *
 * Revision 1.4  2006/01/04 01:09:29  gemvx
 * fixed motion timeout bug
 *
 * Revision 1.3  2004/12/17 03:43:31  gemvx
 * *** empty log message ***
 *
 * Revision 1.2  2002/04/24 21:09:58  ajf
 * Changes for the port to epics3.13.4GEM8.4
 *
 * Revision 1.1  2002/04/24 05:18:13  ajf
 * Changes for epics3.13.4GEM8.4.
 *
 * Revision 1.3  2002/03/25 19:44:49  mbec
 * epics 3.13 debugging
 *
 * Revision 1.2  2002/02/05 00:52:03  mbec
 * GCAL specifics ommited
 *
 * Revision 1.3  2001/07/12 19:11:00  gemvx
 * Re-enabled power check
 *
 * Revision 1.2  2001/04/23 18:23:09  smb
 * OIWFS will no longer time out after being moved manually a significant amount 
 * (bug 168).  HSWA field monitoring problem fixed (bug 218). Problem with setting 
 * up of simulation mode incorrectly if mode changed after INIT fixed (bug 258).
 *
 * Revision 1.25  2001/03/27 11:05:57  gmos
 * In omsScanTask(), allow spontaneous motion notification and
 * encoder updates even with no index or when in a limit switch.
 *
 * These changes allow MPOS to track any manual movements of
 * stages with encoders and will not not suffer from later
 * indexing timeouts.
 *
 * THESE CHANGES NEED TO BE THOROUGHLY TESTED ON REAL HARDWARE.
 *
 * Revision 1.24  2001/03/27 11:03:52  gmos
 * devDeviceControl.c
 * ... Committed by mistake with no log text again!
 *
 * Revision 1.23  2001/03/27 11:02:32  gmos
 * devDeviceControl.c
 * ... Committed by mistake with no log text.
 *
 * Revision 1.22  2001/03/20 14:50:33  gmos
 * ... incorporating Bob Wooff's simulation mode INDEX fix.
 *
 * Revision 1.20  2001/03/01 14:14:48  gmos
 * Switched to use device message levels defined in ddrMessageLevels.h.
 *
 * Revision 1.19  2000/12/04 11:05:47  gmos
 * Bob's new command string for centre home mode - add an extra movement to 
 * ensure the mode works regardless of whether the home switch is high or low 
 * when activated.
 *
 * Revision 1.18  2000/11/29 19:43:18  gmos
 * Added two more moves into indexing string for INDEX_UHSW (IALG=2).
 *
 * Revision 1.17  2000/11/10 00:13:48  gmos
 * Prevent display of "spontaneous encoder change" debug message in
 * omsScanTask() when simulating motion.
 *
 * Revision 1.16  2000/10/16 22:46:39  gmos
 * Spontaneous encoder changes print out encoder count change.
 *
 * Revision 1.15  2000/10/11 21:48:19  gmos
 * Allow callback due to change in encoder during IDLE periods (spontaneous
 * motion).
 *
 * Revision 1.14  2000/10/04 00:25:02  gmos
 * Tightened check for card type. Added check for maximum axes and for
 * duplicate axes.  Clear homeSwitch in omsScanTask when simulating.
 * Added more semaphore protection.  Removed one extra semaphore.
 * Corrected some comments.
 *
 * Revision 1.13  2000/08/02 13:30:49  gmos
 * Extra comment added to devBoControlPower
 *
 * Revision 1.12  2000/07/17 10:33:54  gmos
 * Doubled stack size for OMS scan task
 *
 * Revision 1.11  2000/07/12 16:56:44  gmos
 * Rearranged error checks and shortened strings to legal limit
 *
 * Revision 1.5  2000/07/06 17:25:59  gmos
 * Williams new code, tested and debugged
 *
 * Revision 1.3  2000/04/15 11:37:41  gmos
 * New device record with backlash compensation and binary out power control
 *
 * Revision 1.26  2000/03/31 18:16:07  wooff
 * Cast argument in devInit logMsg.
 *
 * Revision 1.25  2000/03/30 22:42:00  wooff
 * Change SET_ERR_MSG macro so that existing messages
 * are not overwritten.
 *
 * Revision 1.24  2000/03/28 19:55:23  wooff
 * Print out an error message if the call to drvOmsVmeInit fails
 * in devInit.
 *
 * Revision 1.23  2000/01/28 20:20:46  wooff
 * Added diagnostics to omsScanTask encoder/position
 * repeat check.  Also general code cleanup.
 *
 * Revision 1.22  1999/12/16 20:10:21  angelic
 * only perform check for compatible encoder and position values
 * if we are indexed, using encoders and are not in a limit
 *
 * Revision 1.21  1999/11/23 00:53:36  angelic
 * improved check for bad reads using eres and mres
 * bad reads result from card returning position rather than encoder counts
 *
 * Revision 1.20  1999/11/19 22:03:49  angelic
 * detected occasional bad reads from card and ignore them
 *
 * Revision 1.19  1999/11/15 22:23:49  angelic
 * added a bunch of semaphore locks to protect pDevice and pMotor
 *
 * Revision 1.18  1999/10/30 00:13:27  rambold
 * added checks for hardware physically present
 *
 * Revision 1.17  1999/10/25 18:32:55  rambold
 * changed min debugging messages
 *
 * Revision 1.16  1999/10/22 19:13:50  rambold
 * simplified task taking too long check and preset
 * position and limit variables before calling driver
 * to update with real values.
 *
 * Revision 1.15  1999/10/18 21:03:24  wooff
 * William commented out extra logMsg in OmsScanTask.
 *
 * Revision 1.14  1999/10/10 06:14:21  rambold
 * Removed center-home index mode
 *
 * Revision 1.13  1999/10/07 20:02:10  wooff
 * Added some initialization debugging.
 *
 * Revision 1.12  1999/09/02 20:14:35  angelic
 * minor debugging
 *
 * Revision 1.11  1999/08/13 19:21:10  angelic
 * for Vme 44 cards: index using an index encoder pulse
 *
 * Revision 1.10  1999/07/28 19:50:30  rambold
 * added driver layer error messages
 *
 * Revision 1.9  1999/07/28 17:02:28  angelic
 * cleaning up code
 *
 * Revision 1.8  1999/07/23 20:47:14  angelic
 * debugging: check limits and omsScanTask
 *
 * Revision 1.7  1999/06/22 21:28:48  angelic
 * cleaned up debugging statements
 *
 * Revision 1.6  1999/06/10 16:49:30  angelic
 * debugging
 *
 * Revision 1.5  1999/05/26 21:04:31  angelic
 * bug fixes
 *
 * Revision 1.4  1999/04/28 20:27:01  dunn
 * Bug fixes
 *
 * Revision 1.3  1999/03/19 23:12:59  dunn
 * Bug fixes.
 *
 * Revision 1.2  1999/02/26 20:42:04  rambold
 * beta version
 *
 * Revision 1.1  1998/11/20 03:14:21  rambold
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

#include    <stdioLib.h>
#include    <stdlib.h>
#include    <string.h>

#include    <semLib.h>
#include    <taskLib.h>
#include    <sysLib.h>
#include    <tickLib.h>
#include    <logLib.h>
#include    <link.h>
#include    <taskLib.h>

#include    <devSup.h>
#include    <recSup.h>
#include    <ellLib.h>
#include    <devLib.h>

#include    <deviceControlRecord.h>
#include    <boRecord.h>
#include    <devDcOms844.h>
#include    <drvOmsVme844.h>
#include    <ddrMessageLevels.h>      /* Device record message level definitions. */

/*
 *  Local Defines
 */

#define devReport                      NULL       /* No report provided           */
#define devGetIoIntInfo                NULL       /* No ioint info provided       */

#define DDR_OMS_SCAN_TASK_PRIORITY     80         /* OMS scan task priority       */
#define DDR_OMS_SCAN_TASK_STACK        0x2000     /* 8192 byte stack              */
#define DDR_OMS_SCAN_TASK_RATE         10          /* omsScanTask rate (scans/sec) */
#define DDR_OMS_SCAN_TASK_OPTIONS      VX_FP_TASK /* Allow floating point         */


/*
 *  Device support function prototypes
 */

static long configureDrive (DEVICE_CONTROL_PRIVATE *);
static long controlMotion (DEVICE_CONTROL_PRIVATE *, long);
static long controlPower (DEVICE_CONTROL_PRIVATE *, long);
static long devInit (unsigned);
static long devInitRec (DEVICE_CONTROL_RECORD *);
static long setDelay (DEVICE_CONTROL_PRIVATE *, long);
static long setPosition (DEVICE_CONTROL_PRIVATE *, long);

/*
 *  Binary output function prototypes
 */

static long devBoControlPower (struct boRecord *);
static long devBoInit (unsigned);
static long devBoInitRec (struct boRecord *);


/*
 * Internal function prototypes
 */
 
static int omsScanTask (int, int, int, int, int, int, int, int, int, int);


/*
 *  Create the device support interface structure for the deviceControl
 *  record.
 */

DEVICE_CONTROL_DSET devDcOms844 = {
    9,
    devReport,
    devInit,
    devInitRec,
    devGetIoIntInfo,
    configureDrive,
    controlPower,
    controlMotion,
    setDelay,
    setPosition,
    };


/*
 *  Create the device support interface structure for the binary output
 *  record. This structure is defined in devDeviceControl.h
 */

DEVICE_BO_OMS_DAO_DSET devBoOms844 = {
    5,
    NULL,
    devBoInit,
    devBoInitRec,
    NULL,
    devBoControlPower,
    };



static ELLLIST deviceControlScanList;      /* record scan list              */
static int scanTaskPeriod;                 /* system ticks between scans    */
                                           /*   (period of omsScanTask)     */

/*
 *  Define a macro to print debugging information.   If the current debug
 *  level is greater than or equal to the given debug interest level a 
 *  log message will be generated consisting of:
 *      - the value of the system tick counter
 *      - the card number generating the error
 *      - the axis number on the card generating the error
 *      - a user defined error string containing a single integer parameter.
 *        NOTE - does not support double or float parameters
 *
 * For example:
 *      DEBUG(DDR_MSG_FULL,
 *            "<%d> c:%d s:%d omsScanTask: new position: %d \n",
 *            position);
 *
 * may result in the following log message:
 *      <31265> c:1 s:3 omsScanTask: new position: 2532
 *
 */

#define DEBUG(l,FMT,V) if (l <= pDevice->debug)                    \
                                   printf ("%s: "FMT,              \
                                           taskName(0),            \
                                           tickGet(),              \
                                           pMotor->card,           \
                                           pMotor->axis,           \
                                           V);


/*
 *  Define a macro to save the first (root) error message generated
 *  since the last time the error message buffer was flushed.   This
 *  prevents the first message from being overwritten by other error
 *  messages generated as higher level functions fail due to the 
 *  original error.
 */

#define SET_ERR_MSG(MSG)                                           \
{                                                                  \
    if (!strlen (pDevice->errorMessage))                           \
        strncpy (pDevice->errorMessage, MSG, MAX_STRING_SIZE - 1); \
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * configureDrive
 *
 * INVOCATION:
 * status = configureDrive (pDevice);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pDevice  (DEVICE_CONTROL_PRIVATE *)    Record interface structure
 *
 * parameters accessed via the interface structure:
 *
 * (>) pPrivate     (DEV_CTL_OMS_PRIVATE *) motor control structure address
 * (>) simulation   (int)   current simulation mode
 * (>) baseVelocity (long)  starting / minimum velocity (steps/sec)
 * (>) velocity     (long)  maximum velocity (steps/sec)
 * (>) acceleration (long)  acceleration rate (steps/sec/sec)
 * 
 * FUNCTION VALUE:
 * (long) processing success code.
 *
 * PURPOSE:
 * Configure OMS motor driver motion parameters
 *
 * DESCRIPTION:
 * In simulation mode, set the internal simulation velocity.
 * In real mode, format an OMS command string to set the base velocity
 *   operating velocity and acceleration rate.
 * Write the string to the given oms card/axis.
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

static long configureDrive
(
    DEVICE_CONTROL_PRIVATE *pDevice   /* deviceControl rec interface struct */
)
{
    char    scratch[DRV_OMS_VME_MAX_MSG_LEN];        /* char scratch buffer */
    DEV_CTL_OMS_PRIVATE *pMotor = pDevice->pPrivate; /* axis control struct */
    long    status = 0;                              /* function status     */
  

    DEBUG(DDR_MSG_MAX, "<%ld> c:%d s:%d configureDrive:entry%c\n", ' ');

    /*
     *  In simulation mode just set the simulation velocity.
     */

    if ( pDevice->simulation )
    {
        semTake (pMotor->mutexSem, WAIT_FOREVER);
        pMotor->simVelocity = pDevice->velocity / DDR_OMS_SCAN_TASK_RATE;
        semGive (pMotor->mutexSem);
    }


    /*
     *  Otherwise set the axis the motion parameters
     */ 

    else
    {
        /*
         *  Confirm that the card and axis were found at init time.
         */

        if (!pMotor->exists)
        {
            SET_ERR_MSG("Motor hardware does not exist");
            return DRV_OMS_VME_S_CFG_ERROR;
        }
           
        /*
         *  The base velocity must be greater than zero
         */

        if ( pDevice->baseVelocity < 1 )
        {
            DEBUG(DDR_MSG_ERROR, "<%ld> c:%d s:%d configureDrive:velocity base cannot be less than 1: base velocity:%ld steps/sec\n", pDevice->baseVelocity);
            SET_ERR_MSG("VBAS must be greater than zero");
            return DRV_OMS_VME_S_PARAM_ERR;
        }

        /*
         *  The base velocity must be less than
         *  the programmed velocity (velocity or indexVelocity)
         */

        if ( pDevice->velocity <= pDevice->baseVelocity ||
                  pDevice->indexVelocity <= pDevice->baseVelocity )
        {
            DEBUG(DDR_MSG_ERROR, "<%ld> c:%d s:%d configureDrive:velocity base failure: base velocity:%ld steps/sec\n", pDevice->baseVelocity);
            DEBUG(DDR_MSG_MIN, "<%ld> c:%d s:%d configureDrive:velocity:%ld steps/sec\n", pDevice->velocity);
            DEBUG(DDR_MSG_MIN, "<%ld> c:%d s:%d configureDrive:index velocity:%ld steps/sec\n", pDevice->indexVelocity);
            SET_ERR_MSG("VBAS must be less than VELO and FIVL");
            return DRV_OMS_VME_S_PARAM_ERR;
        }

        /*
         *  The velocity must be faster than the omsScanTask rate
         */

        if ( pDevice->velocity < DDR_OMS_SCAN_TASK_RATE ||
             pDevice->indexVelocity < DDR_OMS_SCAN_TASK_RATE)
        {
            DEBUG(DDR_MSG_ERROR, "<%ld> c:%d s:%d configureDrive:velocity slower than scan rate, DDR_OMS_SCAN_TASK_RATE: %d scans/sec\n", DDR_OMS_SCAN_TASK_RATE);
            DEBUG(DDR_MSG_MIN, "<%ld> c:%d s:%d configureDrive:velocity: %ld steps/sec\n", pDevice->velocity);
            DEBUG(DDR_MSG_MIN, "<%ld> c:%d s:%d configureDrive:index velocity: %ld steps/sec\n", pDevice->indexVelocity);
            SET_ERR_MSG("Velocity too slow for scan task");
            return DRV_OMS_VME_S_PARAM_ERR;
        }

        /*
         *  The acceleration must be fast enough such that the position
         *  has changed within a single omsScanTask during the ramp up 
         */
	
        if ( pDevice->acceleration < (DDR_OMS_SCAN_TASK_RATE *
                   (DDR_OMS_SCAN_TASK_RATE - pDevice->baseVelocity)) )
        {
            DEBUG(DDR_MSG_ERROR, "<%ld> c:%d s:%d configureDrive:acceleration too slow for scan rate, DDR_OMS_SCAN_TASK_RATE: %d scans/sec\n", DDR_OMS_SCAN_TASK_RATE);
            DEBUG(DDR_MSG_MIN, "<%ld> c:%d s:%d configureDrive:base velocity: %ld steps/sec\n", pDevice->baseVelocity);
            DEBUG(DDR_MSG_MIN, "<%ld> c:%d s:%d configureDrive:acceleration: %ld steps/sec/sec\n", pDevice->acceleration);
            SET_ERR_MSG("Acceleration too slow for scan task");
            return DRV_OMS_VME_S_PARAM_ERR;
        }

        /*
         *  Generate an OMS command string to set the axis base velocity,
         *  operating velocity and acceleration rate.
         */

        semTake (pMotor->mutexSem, WAIT_FOREVER);

        sprintf (scratch,
                 "VB%ld VL%ld AC%ld ",
                 pDevice->baseVelocity,
                 pDevice->velocity,
                 pDevice->acceleration);


        /*
         *  Then write the string to the appropriate axis on the appropriate
         *  OMS card.
         */

        status = drvOmsVmeWriteMotor (pMotor->card,
                                      pMotor->axis,
                                      scratch);
        if (status)
        {
            drvOmsVmeGetErrorMessage (pDevice->errorMessage);
            DEBUG(DDR_MSG_ERROR, "<%ld> c:%d s:%d configureDrive: %s\n",
                                 pDevice->errorMessage);
        }
        semGive (pMotor->mutexSem);
    }
                                
    return ( status );                            
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * controlMotion
 *
 * INVOCATION:
 * status = controlMotion (pDevice, mode);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pDevice  (DEVICE_CONTROL_PRIVATE *)  record interface structure
 * (>) mode     (long)                      Motion control word.
 *
 * parameters accessed via the interface structure:
 *
 * (>) pPrivate     (DEV_CTL_OMS_PRIVATE *) motor control structure address
 * (>) simulation   (int)   current simulation mode
 * (>) position     (long)  current device position (steps)
 * (>) target       (long)  desired device position (steps)
 * (>) index        (int)   indexing algorithm to use
 * (>) velocity     (long)  maximum velocity (steps/sec) 
 * (>) indexVelocity (long) final approach to index switch vel (steps/sec)
 * (>) lowLimit     (int)   lower limit switch active flag
 * (>) highLimit    (int)   upper limit switch active flag
 *
 * FUNCTION VALUE:
 * (long) processing success code.
 *
 * PURPOSE:
 * Start/stop the currently configured motion
 *
 * DESCRIPTION:
 * Control the motion of the selected motor via the mode word.
 * Possible actions are:
 *  DDR_MOVE_START -> Move to the target given in the device structure
 *  DDR_MOVE_STOP  -> Bring the motor to a controlled stop
 *  DDR_MOVE_ABORT -> Bring the motor to a VERY QUICK stop (may be out of step)
 *
 * In each case an OMS command string is generated based on the indexing
 * algorithm, the type of OMS card used for this device and the state of
 * the motion limit switches.
 * The string is then written to the OMS card for execution.  All strings are
 * terminated with "ID" to cause an interrupt when motion has finished.
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
 * 1. Indexing algorithm assumes use of encoder index signal when the
 * OMS card type is 44 and index type is UHSW or LHSW. 
 * 2. Encoder indexing is not implemented for the CHSW index type.
 * 3. UHSW and LHSW both assume only one edge of the home switch can
 * be reached.  If this is not the case for your device, try CHSW.
 *-
 ************************************************************************
 */

static long controlMotion
(
    DEVICE_CONTROL_PRIVATE *pDevice,  /* deviceControl rec interface struct */
    long        mode                  /* motion control word                */
)
{
    DEV_CTL_OMS_PRIVATE *pMotor = pDevice->pPrivate; /* axis control struct */
    char    scratch[DRV_OMS_VME_MAX_MSG_LEN];        /* char scratch buffer */
    long    status = 0;                              /* function status     */


    DEBUG(DDR_MSG_FULL, "<%ld> c:%d s:%d controlMotion: mode=<%ld>\n", mode);


    /*
     *  In simulation mode set the target to be the current position to
     *  fake device motion when mode is GO.
     */
   
    if ( pDevice->simulation )
    {

        DEBUG(DDR_MSG_FULL, 
              "<%ld> c:%d s:%d controlMotion:simulating%c\n",' ');

        if ( mode != DDR_MOVE_GO )
        {
            semTake (pDevice->mutexSem, WAIT_FOREVER);
            pDevice->target = pDevice->position;
            semGive (pDevice->mutexSem);
        }
    }


    /*
     *  Otherwise send the appropriate motion command strings to the 
     *  OMS card.
     */

    else
    {
        /*
         *  Confirm that the requested card and axis were found at init time
         */

        if (!pMotor->exists)
        {
            SET_ERR_MSG("Motor hardware does not exist");
            return DRV_OMS_VME_S_CFG_ERROR;
        }

        /*
         *  Then generate OMS command strings according to the motion 
         *  control word and the contents of the interface structure.
         */
    
        switch ( mode )
        {

        /*
         *  In GO mode the OMS card is requested to re-position the device
         *  according to the prevailing index mode.
         */

        case DDR_MOVE_GO:
            switch (pDevice->index)
            {

            /*
             *  In INDEX_NONE mode (normal operation) the device is simply
             *  sent the target postion followed by the GO keyword to start
             *  the device in motion.
             */

            case DDR_INDEX_NONE:
                sprintf (scratch,
                         "MA%ld GO ID ",
                         pDevice->target);
                status = drvOmsVmeWriteMotor (pMotor->card,
                                              pMotor->axis,
                                              scratch);
                break;


            /*
             *  In INDEX_LHSW mode, the device will travel in the reverse
             *  direction and index on the high to low transition of the 
             *  home switch.  The OMS command string depends first on the
             *  motor card type and second on the state of the lower  
             *  soft limit.  
             */


            case DDR_INDEX_LHSW:
                DEBUG(DDR_MSG_FULL,
                      "<%ld> c:%d s:%d controlMotion: INDEX_LHSW%c\n", ' ');

                /*  
                 *  With the 44 type cards, if the limit IS set:
                 *           forward until home switch low, then ramp down 
                 *
                 *  If the limit is NOT set:
                 *           reverse until home switch low, then ramp down
                 *  
                 *  Either way, the stage should be stopped with the home 
                 *  switch in the low state and the command is the same
                 *  from here on: 
                 *           forward until home switch high, then ramp down
                 *           change to a slower index velocity
                 *           enable the index pulse
                 *           reverse until: home switch low and
                 *                          encoder index pulse high and
                 *                          encoder phase A high and
                 *                          encoder phase B low then ramp down
                 *           forward back to zero position
                 */

                if (pMotor->type == 44)
                {
                    DEBUG(DDR_MSG_FULL,
                          "<%ld> c:%d s:%d controlMotion: type 44%c\n",
                          ' ');

                    /*
                     *  Generate an OMS command string to implement this
                     *  algorithm.
                     */

                    if (pDevice->lowLimit)
                    {
                        sprintf (scratch,
                                 "HL HM0 HH HM0 VL%ld HE HR0 HS MA0 GO ID ",
                                 pDevice->indexVelocity);
                    }
                    else
                    {
                        sprintf (scratch,
                                 "HL HR0 HH HM0 VL%ld HE HR0 HS MA0 GO ID ",
                                 pDevice->indexVelocity);
                    }
                }

               /*
                 *  With card type 8, there is no index pulse.  If the limit
                 *  is NOT set:
                 *           reverse until home switch low, then ramp down
                 *
                 *  Then, limit or not, in both cases: 
                 *         # forward until home switch high, then ramp down
                 *         # reverse until home switch low, then ramp down
                 *           change to slower index velocity
                 *           forward until home switch high, then ramp down
                 *           reverse back to zero position
                 *
                 *  # This extra forward and reverse cycle is for the case 
                 *    where the index velocity must be very slow and the home  
                 *    switch is low for a long portion of its travel.  If the 
                 *    INDEX is asked for when the device is positioned at the 
                 *    opposite side of where the index point will be, it
                 *    eliminates the need to travel the distance at the 
                 *    slower index velocity.
                 */

                else
                {
                    /*
                     *  Generate an OMS command string to implement this
                     *  algorithm.
                     */

                    if (pDevice->lowLimit)
                    {
                        sprintf (scratch,
                                 "HH HM0 HL HR0 VL%ld HH HM0 MA0 GO ID ",
                                 pDevice->indexVelocity);
                    }
                    else
                    {
                        sprintf (scratch,
                                 "HL HR0 HH HM0 HL HR0 VL%ld HH HM0 MA0 GO ID ",
                                 pDevice->indexVelocity);
                    }
                }


                /*
                 *  Send the command string generated above to the
                 *  selected device.
                 */

                status = drvOmsVmeWriteMotor (pMotor->card,
                                              pMotor->axis,
                                              scratch);
                break;


            /*
             *  In INDEX_UHSW mode, the device will travel in the forward
             *  direction and index on the high to low transition of the 
             *  home switch.  The OMS command string depends first on the
             *  motor card type and second on the state of the lower  
             *  soft limit.  
             */

            case DDR_INDEX_UHSW:
                DEBUG(DDR_MSG_FULL,
                      "<%ld> c:%d s:%d controlMotion: INDEX_UHSW%c\n", ' ');

                /*  
                 *  With the 44 type cards, if limit IS set:
                 *           reverse until home low, then ramp down
                 *
                 *  If the limit is NOT set: 
                 *           forward until home low, then ramp down
                 *
                 *  Then carry on:
                 *           reverse until home high
                 *           change to slower indexing velocity
                 *           enable the index pulse
                 *           forward until: home switch low and
                 *                          encoder index pulse high and
                 *                          encoder phase A high and
                 *                          encoder phase B low then ramp down
                 *           forward back to zero position
                 */

                if (pMotor->type == 44)
                {
                    DEBUG(DDR_MSG_FULL,
                          "<%ld> c:%d s:%d controlMotion: type 44%c\n", ' ');

                    /*
                     *  Generate an OMS command string to implement this
                     *  algorithm.
                     */

                    if (pDevice->highLimit)
                    {
                        DEBUG(DDR_MSG_FULL,
                              "<%ld> c:%d s:%d controlMotion: hilim set%c\n",
                              ' ');
                        sprintf (scratch,
                                 "HL HR0 HH HR0 VL%ld HE HM0 HS MA0 GO ID ",
                                 pDevice->indexVelocity);
                    }
                    else
                    {
                        sprintf (scratch,
                                 "HL HM0 HH HR0 VL%ld HE HM0 HS MA0 GO ID ",
                                 pDevice->indexVelocity);
                    }
                }

                /*
                 *  With card type 8, there is no index pulse.  If the limit
                 *  is NOT set:
                 *           forward until home switch low, then ramp down
                 *
                 *  Then, limit or not, in both cases:
                 *         # reverse until home switch high, then ramp down
                 *         # forward until home switch low, then ramp down
                 *           change to slower index velocity
                 *           reverse until home switch high, then ramp down
                 *           forward back to zero position
                 *
                 *  # This extra forward and reverse cycle is for the case 
                 *    where the index velocity must be very slow and the home  
                 *    switch is low for a long portion of its travel.  If the 
                 *    INDEX is asked for when the device is positioned at the 
                 *    opposite side of where the index point will be, it
                 *    eliminates the need to travel the distance at the 
                 *    slower index velocity.
                 */

                else
                {
                    /*
                     *  Generate an OMS command string to implement this
                     *  algorithm.
                     */

                    if (pDevice->highLimit)
                    {
                        DEBUG(DDR_MSG_MAX,
                              "<%ld> c:%d s:%d controlMotion: hilim set%c\n",
                              ' ');
                        sprintf (scratch,
                                 "HH HR0 HL HM0 VL%ld HH HR0 MA0 GO ID ",
                                 pDevice->indexVelocity);
                    }
                    else
                    {
                        sprintf (scratch,
                                 "HL HM0 HH HR0 HL HM0 VL%ld HH HR0 MA0 GO ID ",
                                 pDevice->indexVelocity);
                    }           
                }


                /*
                 *  Send the command string generated above to the
                 *  selected device.
                 */

                status = drvOmsVmeWriteMotor (pMotor->card,
                                              pMotor->axis,
                                              scratch);
                break;
 

            /*
             *  The INDEX_CHSW mode is used for continuously rotating devices
             *  (i.e. filter wheels) that have a point contact switch trigger
             *  at some point in their travel.   The intent of this algorithm
             *  is to wind up close to the switch on the higher count side
             *  then back up slowly until the switch is triggered to define
             *  the index point. The algorithm includes extra moves designed
             *  to make the indexing work regardless of whether the centre
             *  home switch is high or low when active.
             *
             *  The device is instructed to:
             *           reverse until home switch low, then ramp down
             *           reverse again until home switch high, then ramp down
             *           forward until home switch high, then ramp down
             *           change to slower indexing velocity
             *           forward until home switch low, then ramp down
             *           reverse back to zero position
             */

            case DDR_INDEX_CHSW:
                DEBUG(DDR_MSG_FULL,
                      "<%ld> c:%d s:%d controlMotion: INDEX_CHSW%c\n", ' ');

                sprintf (scratch,
                         "HL HR0 HH HR0 HH HM0 VL%ld HL HM0 MA0 GO ID ",
                         pDevice->indexVelocity);
                status = drvOmsVmeWriteMotor (pMotor->card,
                                              pMotor->axis,
                                              scratch);
                break;


            /*
             *  In INDEX_LLSW mode the device is instructed to move in
             *  the reverse direction at full speed until the lower
             *  limit switch is triggered, which will cause the OMS card
             *  to ramp the device down to a controlled stop.  The calling 
             *  code can then reset the position counter to zero if it 
             *  wishes.  This mode is used for devices with no home switch
             *  and is accomplished by commanding a relative move to a very 
             *  large negative position.
             */

            case DDR_INDEX_LLSW:
                DEBUG(DDR_MSG_FULL,
                      "<%ld> c:%d s:%d controlMotion: INDEX_LLSW%c\n", ' ');
                status = drvOmsVmeWriteMotor (pMotor->card,
                                              pMotor->axis,
                                              "MR-1000000000 GO ID ");
                break;


            /*
             *  In INDEX_ULSW mode the device is instructed to move in
             *  the forward direction at full speed until the upper
             *  limit switch is triggered, which will cause the OMS card
             *  to ramp the device down to a controlled stop.  The calling 
             *  code can then reset the position counter to zero if it 
             *  wishes.  This mode is used for devices with no home switch
             *  and is accomplished by commanding a relative move to a very 
             *  large positive position.
             */

            case DDR_INDEX_ULSW:
                DEBUG(DDR_MSG_FULL,
                      "<%ld> c:%d s:%d controlMotion: INDEX_ULSW%c\n", ' ');
                status = drvOmsVmeWriteMotor (pMotor->card,
                                              pMotor->axis,
                                              "MR1000000000 GO ID ");
                break;


            /*
             *  Undefined indexing modes are rejected here. 
             */

            default:
                return -1;
                break;

            }

            DEBUG(DDR_MSG_MAX,
                  "<%ld> c:%d s:%d controlMotion: Done DDR_MOVE_GO%c\n", ' ');
            break;


        /*
         *  In STOP mode the device is commanded to stop the motion in
         *  progress.
         */

        case DDR_MOVE_STOP:
            status = drvOmsVmeWriteMotor (pMotor->card,
                                          pMotor->axis,
                                          "ST ID ");
            DEBUG(DDR_MSG_MAX,
                  "<%ld> c:%d s:%d controlMotion: Done DDR_MOVE_STOP%c\n",
                                     ' ');
            break;


        /*
         *  In ABORT mode the device is commanded to stop the motion in
         *  progress.   The difference between a STOP and an ABORT
         *  termination is that an ABORT first temporarily changes
         *  to a fast acceleration/deceleration.  This is done because
         *  there is a strong possibility that brakes will be applied
         *  at the same time.
         */

        case DDR_MOVE_ABORT:
            status = drvOmsVmeWriteMotor (pMotor->card,
                                          pMotor->axis,
                                          "AC2000000 ST ");
            DEBUG(DDR_MSG_MAX,
                  "<%ld> c:%d s:%d controlMotion: Done DDR_MOVE_ABORT%c\n",
                                     ' ');
            break;
        }
    }


    /*
     *  If the device driver code generated an error then recover the
     *  error message explaining what went wrong and save it in the
     *  interface structure.
     */

    if (status)
    {
        drvOmsVmeGetErrorMessage (pDevice->errorMessage);
        DEBUG(DDR_MSG_ERROR, 
              "<%ld> c:%d s:%d configure drive: %s\n",
              pDevice->errorMessage);
    }

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * controlPower
 *
 * INVOCATION:
 * status = controlPower (pDevice, power);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pDevice  (DEVICE_CONTROL_PRIVATE *)  Record interface structure.
 * (>) power    (long) Desired power state.
 *
 * parameters accessed via the interface structure:
 *
 * (>) pPrivate     (DEV_CTL_OMS_PRIVATE *) motor control structure address
 * (>) simulation   (int)   current simulation mode
 *
 * FUNCTION VALUE:
 * (long) processing success code.
 *
 * PURPOSE:
 * Control motor power via OMS drive hardware aux bit
 *
 * DESCRIPTION:
 * In simulation mode simply return otherwise set the state of the OMS 
 * card auxiliary bit for the given axis by sending a formatted command
 * string to the card.
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

static long controlPower
(
    DEVICE_CONTROL_PRIVATE *pDevice,  /* deviceControl rec interface struct */
    long power                        /* power enable/disable flag          */
)
{
    DEV_CTL_OMS_PRIVATE *pMotor = pDevice->pPrivate;
    long status = 0;

    DEBUG(DDR_MSG_FULL, "<%ld> c:%d s:%d controlPower:<%ld>\n", power);

    /*
     *  Only write to the hardware if we are not in simulation mode
     */

    if (!pDevice->simulation)
    {
        if (!pMotor->exists)
        {
            SET_ERR_MSG("Motor hardware does not exist");
            return DRV_OMS_VME_S_CFG_ERROR;
        }

        /*
         *  Request that the background task check the state of the
         *  limit switches on its next scan and then send a command 
         *  string to the OMS card.
         */
	/*
        semTake (pMotor->mutexSem, WAIT_FOREVER);
        pMotor->updateState = TRUE;
        semGive (pMotor->mutexSem);
	*/
        status = drvOmsVmeWriteMotor (pMotor->card,
                                      pMotor->axis,
                                      (power) ? "AN " : "AF ");
    }


    /*
     *  If the driver code reported an error then recover the error message
     *  and copy it into the interface structure.
     */

    if (status)
    {
        drvOmsVmeGetErrorMessage (pDevice->errorMessage);
        DEBUG(DDR_MSG_ERROR, 
              "<%ld> c:%d s:%d controlPower: failed: %s\n",
              pDevice->errorMessage);
    }

    return (status);
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * devBoInit
 *
 * INVOCATION:
 * status = devBoInit (pass);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pass  (unsigned) Pass number (called twice during initialization)
 *
 * FUNCTION VALUE:
 * (long) initialization pass success code.
 *
 * PURPOSE:
 * Initialize OMS support code for the Binary Output record
 *
 * DESCRIPTION:
 * Called before and after an EPICS database has been loaded.
 * First (before) initializes the drvOmsVme support code.
 * Second (after) invocation simply returns.
 *
 * Note that it is safe to call the drvOmsVme initialization function 
 * more than once, it will only do the initialization on the first 
 * invocation...
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


static long devBoInit 
(
    unsigned after              /* System initialization pass number    */
)
{
    long status = 0;

    if (after)
    {
        return status;
    }

    /*
     * Initialize OMS VME drive support
     */
   
    return drvOmsVmeInit();
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * devBoInitRec
 *
 * INVOCATION:
 * status = devBoInitRec (pbo);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pbo  (structBoRecord *) Pointer to Binary Output Record.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Initialize binaryOutput record-specific functions
 *
 * DESCRIPTION:
 * Just check to insure that the OUT field is of type VMEIO...
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

static long devBoInitRec
(
    struct boRecord  *pbo   /* binary output record structure           */
)
{
    long status = 0;        /* function return status                   */


    /*
     *  If the output field is defined as type VMEIO then mask off the
     *  bit corrisponding to the axis number on the card.
     */

    if (pbo->out.type == VME_IO)
    {
        pbo->mask = 1;
        pbo->mask <<= pbo->out.value.vmeio.signal;
    }


    /*
     *  Otherwise the wrong output field type has been specified and
     *  the driver will not operate.   Generate a system error message
     *  to warn the developer.
     */

    else
    {
        status = S_db_badField;
        recGblRecordError (status,
                           (void *)pbo,
                           "devBoOmsDao (devBoInitRec) Illegal OUT field");
    }

    return status;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * devBoControlPower
 *
 * INVOCATION:
 * status = devBoControlPower (pbo);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pbo  (structBoRecord *) Pointer to Binary Output Record.
 *
 * FUNCTION VALUE:
 * (long) write success code.
 *
 * PURPOSE:
 * Control selected motor power
 *
 * DESCRIPTION:
 * Send the motor power control strings to the selected motor.
 *
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 * It is assumed that the EPICS record support code will not call this
 * function in simulation mode, i.e. in simulation mode the SIMM field
 * must be set to "YES". Compare with the controlPower function above.
 *
 * SEE ALSO:
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long devBoControlPower
(
    struct boRecord  *pbo                   /* binary output record struct  */
)
{
    char errorMessage[MAX_STRING_SIZE];     /* error message buffer         */
    long status = 0;                        /* function return status       */


    /*
     *  Send an auxiliary bit control command string to the
     *  selected OMS card and axis.
     */

    status = drvOmsVmeWriteMotor (pbo->out.value.vmeio.card,
                                  pbo->out.value.vmeio.signal,
                                  (pbo->val) ? "AN " : "AF ");
 

    /*
     *  If the device driver reported an error then send a system
     *  error message and generate a system alarm to warn the operator 
     *  that this has happened.  The error message has to be read from
     *  the driver to clear it, even though it is not used here.
     */

    if (status != 0)
    {
        status = -1;
        drvOmsVmeGetErrorMessage (errorMessage);
        recGblRecordError (status,
                           (void *)pbo,
                           "devBoOmsDao (devBoControlPower) OMS write fault");
        recGblSetSevr(pbo,WRITE_ALARM,INVALID_ALARM);
    }

    return status;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * devInit
 *
 * INVOCATION:
 * status = devInit (pass);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pass  (unsigned) Pass number (called twice during initialization)
 *
 * FUNCTION VALUE:
 * (long) initialization pass success code.
 *
 * PURPOSE:
 * Initialize global doaDevice device support functions
 *
 * DESCRIPTION:
 * Called before and after an EPICS database has been loaded.
 * First (before) invocation creates the linked list for records to be 
 * processed by the scan task, creates the task and sets the scanning
 * rate.
 * Second (after) invocation simply returns.
 *
 * EXTERNAL VARIABLES:
 * deviceControlScanList (ELLLIST *) scan task linked list pointer saved here.
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



static long devInit 
(
    unsigned after                  /* System initialization pass number    */
)
{
    int deviceCtlTaskId;            /* control task ID                      */
    char errorMsg[MAX_STRING_SIZE]; /* returned error message               */
    long status = 0;                /* returned function status             */


    /*
     *  Everything is done on the first pass so simply ignore the second
     */

    if (after)
    {
        return status;
    }


    /*
     * Initialize OMS VME drive support
     */
   
    status = drvOmsVmeInit();
    if (status)
    {
        drvOmsVmeGetErrorMessage (errorMsg);
        logMsg ("\nError initializing drv: %s!\n", (int)errorMsg,0,0,0,0,0);
        return status;
    }


    /*
     * Create a linked list of records to be serviced every time the
     * scan task runs.
     */
    
    ellInit(&deviceControlScanList);


    /*
     * create the deviceControl record support OMS hardware scanning task
     */

    deviceCtlTaskId = taskSpawn (
        "tOmsVme",                     /* task name */
        DDR_OMS_SCAN_TASK_PRIORITY,    /* priority */
        DDR_OMS_SCAN_TASK_OPTIONS,     /* options */
        DDR_OMS_SCAN_TASK_STACK,       /* stack size */
        omsScanTask,                   /* task entry point */
        0,0,0,0,0,0,0,0,0,0);          /* invocation args 1 to 10 */
 
    if (!deviceCtlTaskId)
    {
        status = -1;
        logMsg ("\nCannot create DEV CTL OMS scan task!\n",0,0,0,0,0,0);
    }


    /*
     *  Set the period for omsScanTask (number of clock ticks per scan):
     *  sysClkRateGet returns number of clock ticks per second and
     *  DDR_OMS_SCAN_TASK_RATE is the number of scans per second 
     *  we want.
     */

    scanTaskPeriod = (int)sysClkRateGet() / DDR_OMS_SCAN_TASK_RATE;
    logMsg ("\nPeriod of omsScanTask = %d clock ticks\n", scanTaskPeriod,0,0,0,0,0);

    return status;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * devInitRec
 *
 * INVOCATION:
 * status = devInitRec (pdr);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pdr  (DEVICE_CONTROL_RECORD *) deviceControl record structure.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * parameters accessed via the record structure:
 *
 * (>) dpvt         (DEV_CTL_PRIVATE *) interface structure address
 *
 * parameters accessed via the interface structure:
 *
 * (<) pPrivate     (DEV_CTL_OMS_PRIVATE *) motor control structure address
 *
 * PURPOSE:
 * Initialize deviceControl record-specific functions
 *
 * DESCRIPTION:
 * Create device private structure and add to linked list of structures
 * to be processed by the OMS scan task.
 * Initialize critical device and private structure fields.
 *
 * EXTERNAL VARIABLES:
 * deviceControlScanList (ELLLIST *) private structure pointer
 * added to this list.
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

static long devInitRec
(
    DEVICE_CONTROL_RECORD    *pdr    /* deviceControl record structure      */
)
{
    DEVICE_CONTROL_PRIVATE
            *pDevice = (DEVICE_CONTROL_PRIVATE *) pdr->dpvt; /* ifc struct  */
    DEV_CTL_OMS_PRIVATE *pMotor = NULL;             /* motor control struct */
    DEV_CTL_OMS_PRIVATE *pAxis = NULL;              /* motor control struct */
    long    status = 0;                         /* function status return   */

    DEBUG(DDR_MSG_MAX, "<%ld> c:%d s:%d devInitRec: entry%c\n", ' ');


    /*
     *  Create a motor control structure for this record
     */

    pMotor = malloc (sizeof(DEV_CTL_OMS_PRIVATE));
    if ( pMotor == NULL )
    {
        status = -1;
        recGblRecordError(status, pdr, __FILE__ ":no room for device private");
        return status;
    }


    /*
     *  Save the motor control struct address in the record interface struct
     */
    
    pDevice->pPrivate = (void *) pMotor;


    /*
     *  Create the MUTEX semaphore to protect the record interface structure
     *  during asynchronous callback access
     */
   
    pMotor->mutexSem = NULL;
    if ((pMotor->mutexSem = semMCreate(SEM_Q_PRIORITY|SEM_INVERSION_SAFE)) ==
         NULL)
    {
        status = -1;
        recGblRecordError (status, pdr, __FILE__ ":no room for motor private");
        return status;
    }


    /*
     *  Save the address of the record structure and record interface 
     *  structure in the motor control structure.
     */ 

    semTake (pMotor->mutexSem, WAIT_FOREVER);
    
    pMotor->pDevice = pDevice;
    pMotor->pRecord = pdr;


    /*
     *  Save the card and axis for this motor in the motor control structure
     */
     
    pMotor->card = pdr->out.value.vmeio.card;
    pMotor->axis = pdr->out.value.vmeio.signal;


    /*
     *  Confirm that the interface card exists and get its type
     */

    pMotor->type = drvOmsVmeGetCardType (pMotor->card);


    /*
     *  Verify that the card type is valid
     */

    if ( pMotor->type != 44 && pMotor->type != 8 )
    {
        logMsg ("\ndevDcOms844: devInitRec: OMS motor card:%d invalid type=%d\n",
                 pMotor->card,pMotor->type,0,0,0,0);
        pMotor->exists = FALSE;
    }


    /*
     *  Verify that the axis number is valid for the card type
     */

    else if ( (pMotor->type == 44 && 
             (pMotor->axis >= DRV_OMS_VME_44_MAX_AXES || pMotor->axis < 0 )) ||
         (pMotor->type == 8 && 
             (pMotor->axis >= DRV_OMS_VME_8_MAX_AXES || pMotor->axis < 0 )) )
    {
        logMsg ("\ndevDcOms844: devInitRec: OMS motor card:%d axis:%d invalid\n",
                 pMotor->card,pMotor->axis,0,0,0,0);
        pMotor->exists = FALSE;
    }

    else
    {
        pMotor->exists = TRUE;
    }


    /*
     *  Initialize the rest of the control structure
     *
     */

    pMotor->updateState = TRUE;
    pMotor->earlyDone = FALSE;
    pMotor->simVelocity = 1;
    pMotor->scansLeft = 0;
    pMotor->stoppedCntr = 0;
    pMotor->status = 0;
    semGive (pMotor->mutexSem);


    /*
     * Check for duplicate motors on the scan list
     * Initialize with first entry in list
     */

    pAxis = (DEV_CTL_OMS_PRIVATE *) ellFirst (&deviceControlScanList);
        
    /*
     *  Compare with each non-NULL entry in list
     */

    while (pAxis)
    {  
        if ( (pAxis->card == pMotor->card) &&
             (pAxis->axis == pMotor->axis) )
        {
	   /*
            * Duplicate exists so prevent BOTH motors from getting used
            * except in simulation
            */
            pAxis->exists = FALSE;
            semTake (pMotor->mutexSem, WAIT_FOREVER);
            pMotor->exists = FALSE; 
            semGive (pMotor->mutexSem);
            DEBUG(DDR_MSG_FATAL, "\n<%ld> c:%d s:%d devInitRec: OMS motor duplicate%c\n", ' ');
        }

        /* get the next one off the list */
        pAxis = (DEV_CTL_OMS_PRIVATE *) ellNext (&pAxis->node);
       
    }   /* end of motor scanning loop */


    /*
     * And add it to the scan list
     */

    ellAdd (&deviceControlScanList, &pMotor->node);

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * omsScanTask
 *
 * INVOCATION:
 * status = omsScanTask ();
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *
 * FUNCTION VALUE:
 * (int) task success code (always 0).
 *
 * PURPOSE:
 * Scan all regestered motors
 *
 * DESCRIPTION:
 *
 * The scan task executes at a pre-determined rate.   Each time it processes
 * it runs through the list of motors that have deviceControl records 
 * controlling them and manages their operation using the following algorithm:
 *
 *  DO forever:
 *  {
 *      Get the first entry on the scan list
 *      While the entry is not NULL:
 *      {
 *          Recover the associated record and interface structures.
 *
 *          If we are in simulation mode:
 *          {
 *              Motor status is always good and encoder is always 0.
 *              On 1st pass only:
 *              {
 *                  Clear home and limit switches.
 *                  Set updateState flag.
 *              }
 *              Clear checkLimits flag.
 *              Simulate move by updating current position with the distance 
 *                the virtual motor would have gone between scans by adding or 
 *                subtracting the simulated velocity offset until the desired 
 *                target position is reached.
 *          }
 *
 *          Else if there is hardware attached to the device:
 *          {
 *              Read current position and encoder from the OMS card. 
 *              If error as a result of position/encoder update
 *              {
 *                  Recover the status error message from the driver level
 *                    and don't bother checking for encoder errors or QA.
 *              }
 *              Else 
 *              {
 * 
 *                  If either just starting the move OR just finished
 *                    (I.e. checkLimits flag is set or moving flag is set  
 *                     and position unchanged):
 *                  {
 *                      Set updateState flag.
 *                      Read current states from OMS card:
 *                        home switch
 *                        high and low limit switches
 *                        axisDone flag 
 *                      If error as a result of position/encoder update recover
 *                         the status error message from the driver level.
 *                      Clear checkLimits flag.
 *                  }
 *              }
 *          }
 *
 *          Else there is no hardware to control so simply save previous 
 *            position, encoder, limits & home state values locally.
 *
 *          Initialize local flags:
 *            Clear rescan flag.
 *            Set done flag.
 *
 *          If there is time remaining on the delay timer:
 *          {
 *              Decrement the delay timer.
 *              If the timer has reached zero set the timeout flag.
 *          }
 *
 *          If motor position & state were successfully read
 *          {
 *              If moving flag is set AND position is unchanged
 *              {
 *                  If status returned indicates either axisDone OR 
 *                    we're simulating, then we've stopped.
 *                    
 *                  {
 *                      Clear moving flag.
 *                      Set rescan flag.
 *                      Clear motion stopped scan counter 
 *                  }
 *                  Else status returned indicates either
 *                    highLimit OR lowLimit
 *                  {
 *                      If stopped and in limit for 5 scans
 *                      {
 *                          Issue "EF ID" string to set up interrupt when done
 *                      }
 *                      Else  Increment motion stopped counter
 * 
 *                      Set checkLimits flag
 *                  }
 *              }
 *              Else if position has changed
 *              {
 *                  If moving flag not set and not just indexing on limit
 *                  {
 *                      Set moving flag
 *                  }
 *                  Update position value.
 *                  Clear done flag.
 *                  Set rescan flag.
 *              }
 *
 *              If current encoder position has changed:
 *              {
 *                  Update encoder value.
 *                  If not moving and not simulating:
 *                  {
 *                       Indicate spontaneous motion.
 *                       Set rescan flag.
 *                  }
 *              }
 *
 *              If updateState flag set:
 *              {
 *                  If the home or either limit switch states has changed:
 *                  {
 *                      Update switch values.
 *                      Set rescan flag.
 *                  }
 *                  Clear updateState flag.
 *              }
 *          }
 * 
 *          If the motor status has changed:
 *          {
 *              Update motor status for record.
 *              Set rescan flag.
 *          }
 *
 *          If the timeout or rescan flags have been set:
 *          {
 *              Signal internal reprocessing by setting callback flag.
 *              Recover the address of the associated record's process func.
 *              Lock the record to prevent anyone else form processing it.
 *              Process the record by calling the process function.
 *              Unlock the record.
 *          }
 *
 *          Get the next entry on the scan list.
 *
 *      }
 *
 *      Calculate number of system ticks remaining until next scan starts.
 *      Put task to sleep for this number of ticks.
 *  }                          
 *
 *
 *
 * EXTERNAL VARIABLES:
 * deviceControlScanList (ELLLIST *)   linked list of attached devices.
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

static int omsScanTask 
(
    int a1, int a2, int a3, int a4, int a5,     /* invocation arguments     */
    int a6, int a7, int a8, int a9, int a10     /* none of which are used   */
)
{
    DEV_CTL_OMS_PRIVATE *pMotor = NULL; /* motor control structure          */
    DEVICE_CONTROL_PRIVATE *pDevice;    /* deviceControl rec interface str  */
    DEVICE_CONTROL_RECORD *pdr;         /* deviceControl record structure   */
    struct rset *pRset;                 /* deviceControl rec function str   */
    int rescan;                         /* record must be processed flag    */
    int done;                           /* motion finished flag             */
    int axisDone;                       /* axis done flag from hardware     */
    int lowLimit;                       /* lower limit switch state         */
    int highLimit;                      /* upper limit switch state         */
    int homeSwitch;                     /* home switch state                */
    long position;                      /* motor position counter value     */
    long encoder;                       /* encoder value                    */
    int startTick;                      /* system time at start of task     */
    int taskTime;                       /* time taken to execute task       */
    long status = 0;                    /* taskDelay return status          */


    /*
     *  The scan task operates as an infinite loop...
     */

    while (TRUE)
    { 
        startTick = (int)tickGet();


        /*
         *  Get the first entry on the scan list
         */
        
        pMotor = NULL;
        pMotor = (DEV_CTL_OMS_PRIVATE *) ellFirst (&deviceControlScanList);
        
        /*
         *  Process the entry if it not NULL
         */

        while (pMotor)
        {  
            /*
             *  Recover the addresses of the record and record interface
             *  structures associated with this motor.
             */

            pDevice = pMotor->pDevice;
            pdr = pMotor->pRecord;
                            

            /*
             *  In simulation there is no real hardware to control or 
             *  query so simply make it look like things are happening
             *  normally.
             */

            if (pDevice->simulation)
            {
                /*
                 *  Motor status is always good and encoder is always 0.
                 */

                semTake (pMotor->mutexSem, WAIT_FOREVER);
                pMotor->status = 0;
                semGive (pMotor->mutexSem);
                encoder = 0;

                /*
                 *  If the switch states haven't been cleared then it's
                 *  the first pass while simulating so clear the local
                 *  switch variables and set the updateState flag for later.
                 */

                if ( pDevice->lowLimit   != 0 || 
                     pDevice->highLimit  != 0 || 
                     pDevice->homeSwitch != 0   )
                {
                    lowLimit = 0;
                    highLimit = 0;
                    homeSwitch = 0;

                    semTake (pMotor->mutexSem, WAIT_FOREVER);
                    pMotor->updateState = TRUE;
                    semGive (pMotor->mutexSem);
                    DEBUG(DDR_MSG_FULL, "<%ld> c:%d s:%d omsScanTask:clearing limit and home switches for simulation mode: %c\n", ' ');

                }

                /*
                 *  Reset the checkLimits flag.
                 */

                semTake (pDevice->mutexSem, WAIT_FOREVER);
                pDevice->checkLimits = 0;
                semGive (pDevice->mutexSem);

                /*
                 *  Fake motion by running a virtual motor.  No acceleration
                 *  or deceleration is done but the motor takes a realistic
                 *  time to get where it is going.  Move the virtual motor
                 *  the distance the real motor would have gone between
                 *  scans by adding or subtracting the simulated velocity
                 *  offset until the desired target position is reached.
                 */

                if (pDevice->position < pDevice->target)
                {
                    position = pDevice->position + pMotor->simVelocity;
                }

                else if (pDevice->position > pDevice->target)
                {
                    position = pDevice->position - pMotor->simVelocity;
                }

                else
                {
                    position = pDevice->position;
                }


                /*
                 *  Getting within one offset step of the target position
                 *  is close enough.   Make the position equal the target.
                 */

                if (abs(pDevice->target - pDevice->position) <
                    pMotor->simVelocity)
                {
                    position = pDevice->target;
                }               
            }   /* end of simulation mode */


            /*
             *  Not simulating so check to see if the card and axis were found
             *  during system initialization.  
             */

            else if (pMotor->exists)
            {
                /* 
                 *  Update the motor position and encoder value with the
                 *  current values from the OMS card.
                 */

                semTake (pMotor->mutexSem, WAIT_FOREVER);

                pMotor->status = drvOmsVmeMotorPosition (pMotor->card,
                                                         pMotor->axis,
                                                         &position,
                                                         &encoder);
                semGive (pMotor->mutexSem);

                /*
                 *  If there was a problem reading the motor position recover 
                 *  the status message that describes what went wrong and
                 *  don't bother checking for encoder errors or reading the
                 *  motor state.
                 */

                if ( pMotor->status )
                {
                    drvOmsVmeGetErrorMessage (pDevice->errorMessage);
                    DEBUG(DDR_MSG_ERROR,
                            "<%ld> c:%d s:%d Get motor position status = %ld\n",
                            pMotor->status);
                }
                else if ( pDevice->checkLimits ||
                         (pDevice->moving && (position == pDevice->position)))
       	        {
                    /*  Otherwise carry on
                     *
                     *  Update the motor state information (limits, home state
                     *  and done flag) when the motor is either just starting 
                     *  the move OR just finished (I.e. checkLimits flag is set 
                     *  or moving flag is set and position unchanged).
                     *
                     *  These updates are only done when requested to eliminate 
                     *  unnecessary communication with the OMS card.
                     */

                    
                    DEBUG(DDR_MSG_FULL,
                        "<%ld> c:%d s:%d Get motor state (checkLimits:%d)\n",
                        pDevice->checkLimits);

                    semTake (pMotor->mutexSem, WAIT_FOREVER);
                    pMotor->updateState = TRUE;

                    pMotor->status = drvOmsVmeMotorState (pMotor->card,
                                                          pMotor->axis,
                                                          &lowLimit,
                                                          &highLimit,
                                                          &homeSwitch,
                                                          &axisDone);
                    semGive (pMotor->mutexSem);
		    /*
                    if ( pDevice->debug == DDR_MSG_FULL && 
                         pMotor->card == 0 && pMotor->axis == 1 )
                    {
                        printf("grp motor dn:ll:hl:hm %d%d%d%d\n", 
                               axisDone, lowLimit, highLimit, homeSwitch);
                    }

                    if ( pDevice->debug == DDR_MSG_FULL && axisDone )
                    {
                        printf("card:%d axis%d motion done\n", 
                               pMotor->card, pMotor->axis);
                    }
		    */
                    /*
                     *  If there was a problem reading the motor state, recover 
                     *  the status message that describes what went wrong.
                     */

                    if (pMotor->status)
                    {
                        drvOmsVmeGetErrorMessage (pDevice->errorMessage);
                        DEBUG(DDR_MSG_ERROR,
                            "<%ld> c:%d s:%d Get motor state status = %ld\n",
                            pMotor->status);
                    }

                    semTake (pDevice->mutexSem, WAIT_FOREVER);
                    pDevice->checkLimits = 0;
                    semGive (pDevice->mutexSem);               
                }

                /* 
                 *  This check detects invalid encoder counts and ignores 
		 *  them as long as the following read is successfull.  
		 *  Only perform this check if we know that we are indexed
		 *  and are not in the middle of an index.  Also, don't check
		 *  if we're in a limit.
		 */
		  
		if ( (pdr->ueip) && (pDevice->mode != DDR_MODE_INDEX) &&
		     (abs((position/pdr->mres - encoder/pdr->eres) / 
                        (position/pdr->mres)) > 0.1) &&
                     (position/pdr->mres != 0) )
		{
		    if (pDevice->badRead)
		    {
                        if (pdr->hpvl && !(pdr->lswa) )
			{
			    /*
			     *  Another bad encoder value read, dump the 
			     *  debug buffer.
			     */

		            DEBUG(DDR_MSG_MIN, 
				"<%ld> c:%d s:%d omsScanTask: another bad encoder \
				value ... not ignoring!%c\n",' ');
                            drvOmsVmeGetErrorMessage (pDevice->errorMessage);
                        }
		    } 
		    else
		    {
		        DEBUG(DDR_MSG_MIN, 
                            "<%ld> c:%d s:%d scanTask: bad encoder value%c\n",
                            ' ');
                        drvOmsVmeGetErrorMessage( pDevice->errorMessage );
		        DEBUG(DDR_MSG_FULL, 
                            "<%ld> c:%d s:%d scanTask: prev encoder: %ld \n", 
                            pDevice->encoder);
		        DEBUG(DDR_MSG_FULL, 
                            "<%ld> c:%d s:%d scanTask: prev position: %ld \n",
                            pDevice->position);
		        DEBUG(DDR_MSG_FULL, 
                            "<%ld> c:%d s:%d scanTask: new encoder: %ld \n",
                            encoder);
		        DEBUG(DDR_MSG_FULL, 
                            "<%ld> c:%d s:%d scanTask: new position: %ld \n",
                            position);
			encoder = pDevice->encoder;
			semTake (pDevice->mutexSem, WAIT_FOREVER);
			pDevice->badRead = TRUE;
			semGive (pDevice->mutexSem);                
		    }
		} 
		else 
		{
		    /*
		     *  Else it was a good read.
		     */

		    semTake (pDevice->mutexSem, WAIT_FOREVER);
		    pDevice->badRead = FALSE;
		    semGive (pDevice->mutexSem);                

		} /* end of test for bad read of position/encoders */


            }   /* end of things to do if the motor exists */


            /*
             *  Otherwise the motor does not exist so simply maintain
             *  the status quo.   This will prevent any action further
             *  on.
             */

            else
            {
                position = pDevice->position;
                encoder = pDevice->encoder;
                lowLimit = pDevice->lowLimit;
                highLimit = pDevice->highLimit;

                semTake (pDevice->mutexSem, WAIT_FOREVER);
                pDevice->status = pMotor->status;
                pDevice->moving = FALSE;
                semGive (pDevice->mutexSem);               
            }


            /*
             *  Control and monitoring is complete, now compare the
             *  current state to the starting state to determine if
             *  the associated record needs to be re-processed.
             */

            /*
             *  Initialize local flags
             */

            rescan = FALSE;
            done = TRUE;

            semTake (pDevice->mutexSem, WAIT_FOREVER);


            /*
             *  If there is time left on the delay timer then 
             *  decrement the counter here.  If this results in
             *  the timer reaching zero then set the timeout flag.
             *  Remember that the timer is counting the number of 
             *  times omsScanTask will run before the timeout occurs.
             */

	    semTake (pMotor->mutexSem, WAIT_FOREVER);

            if (pMotor->scansLeft)
            {
                pDevice->timeout = (--pMotor->scansLeft <= 0);
            }

	    semGive (pMotor->mutexSem);

            /*
             *  Check motor status to see if motor position and state  
             *  were successfully read.
             */

            if ( !pMotor->status )
            {

                /*  
                 *  Status is okay so check to see if the motor was moving
                 *  the last time it was checked.  If the position returned by the 
                 *  card has also not changed, then we may have stopped.
                 */

                if ( pDevice->moving == TRUE       &&
                     position == pDevice->position    )
                {

                    if ( (pMotor->earlyDone == TRUE) && (axisDone == 0) )         
                    {
                        DEBUG(DDR_MSG_FULL,
                           "<%ld> c:%d s:%d omsScanTask:motion & done flag detected in the same scan%c\n", ' ');
                        axisDone = 1;
                        pMotor->earlyDone = FALSE;
                    }
                    /*  
                     *  If the axis motion is done or axis is simulated, clear the
                     *  moving flag and leave done set.  Set local rescan flag
                     *  to indicate that the record needs to see this. Then clear 
                     *  the motion stopped counter for the next move.
                     */

                    if ( axisDone || pDevice->simulation )
                    {
                        DEBUG(DDR_MSG_MIN,
                           "<%ld> c:%d s:%d omsScanTask:device motion stopped%c\n",
                           ' ');
                        pDevice->moving = FALSE;
                        rescan = TRUE;
                        semTake (pMotor->mutexSem, WAIT_FOREVER);
                        pMotor->stoppedCntr = 0;
                        semGive (pMotor->mutexSem);
                    }

                    /*
                     *  If we're in a limit then we may not yet be stopped.
                     */

                    else if ( highLimit || lowLimit )
                    {
                        /*
                         *  The moving flag is still set, we're still in a limit
                         *  and the position remains unchanged.  Let it settle
                         *  in this condition for five scans.
                         */

                        if (pMotor->stoppedCntr > 5)
                        {
                            DEBUG(DDR_MSG_MIN,"<%ld> c:%d s:%d omsScanTask:device motion stopped in limit - issuing EF ID string%c\n",' ');

                            /*  
                             *  We've stopped and given the limits time to settle.
                             *  Now we need to send an "ID" command to set up
                             *  the interrupt when done flag. 
                             */

                            semTake (pMotor->mutexSem, WAIT_FOREVER);

                            pMotor->status = drvOmsVmeWriteMotor (pMotor->card,
                                                                  pMotor->axis,
                                                                  "EF ID ");

                            semGive (pMotor->mutexSem);

                            /*
                             *  If there was a problem reading the motor state  
                             *  recover the status message that describes what 
                             *  went wrong.
                             */

                            if (pMotor->status)
                            {
                                drvOmsVmeGetErrorMessage (pDevice->errorMessage);
                                DEBUG(DDR_MSG_ERROR,
                                  "<%ld> c:%d s:%d Write motor stop status = %ld\n",
                                  pMotor->status);
                            }
                        }
                        else
                        {
                            /*
                             *  Increment motion stopped counter.
                             */

                            semTake (pMotor->mutexSem, WAIT_FOREVER);
                            pMotor->stoppedCntr++;
                            semGive (pMotor->mutexSem);

                            /*
                             *  Set the checkLimits flag so that the motor state
                             *  is checked again next scan.
                             */
			    /*
                            pDevice->checkLimits = 1;
			    */
                        }
                    }
                    else
                    {
                        /*
                         *  This kludge has to stay until I figure out how to 
                         *  handle bootup initialization where the moving flag
                         *  gets set without an ID setting up the done interrupt.
                         *  The position gets initialized to 0 steps but if the
                         *  happens to have been left at anything non-zero (very
                         *  likely but not for sure if the IOC is rebooted but
                         *  not power cycled) then the change in position will
                         *  cause the moving flag to get set.  However there is no
                         *  done interrupt to cause moving to get cleared.
                         */

                        if (pMotor->stoppedCntr > 20)
                        {
 
                            DEBUG(DDR_MSG_WARNING,
                               "<%ld> c:%d s:%d omsScanTask:device motion stopped without done flag%c\n",' ');
                            pDevice->moving = FALSE;
                            rescan = TRUE;
                            semTake (pMotor->mutexSem, WAIT_FOREVER);
                            pMotor->stoppedCntr = 0;
                            semGive (pMotor->mutexSem);
                        }
                        else
                        {
                            /*
                             *  Increment motion stopped counter.
                             */

                            semTake (pMotor->mutexSem, WAIT_FOREVER);
                            pMotor->stoppedCntr++;
                            semGive (pMotor->mutexSem);
                        }
                    }
    
                }
                else if ( position != pDevice->position ) 
                {
                    /*  
                     *  The position has changed.  Have we just started a motion?
                     *  Check to see if axis was already moving and if not, set 
                     *  the moving flag.  But avoid getting fooled by the case 
                     *  where the position has changed because of setPosition.
                     */

                    if ( (pDevice->moving != TRUE)   && 
                          !(pDevice->index == DDR_INDEX_LLSW && 
                                 pDevice->lowLimit)  &&
                          !(pDevice->index == DDR_INDEX_ULSW && 
                                 pDevice->highLimit)     )
                    {
                        DEBUG(DDR_MSG_MIN, 
                            "<%ld> c:%d s:%d omsScanTask:device motion started%c\n",
                            ' ');
                        pDevice->moving = TRUE;
                        DEBUG(DDR_MSG_MAX, 
                            "<%ld> c:%d s:%d omsScanTask:new position:%ld\n", 
                            position);
                        DEBUG(DDR_MSG_MAX, 
                            "<%ld> c:%d s:%d omsScanTask:old position:%ld\n", 
                            pDevice->position);

                        if (axisDone)
                        {
                            /*
                             *  The motion starting was detected in the same
                             *  scan as the done flag (short move) so set the
                             *  earlyDone flag.
                             */

                            semTake (pMotor->mutexSem, WAIT_FOREVER);
                            pMotor->earlyDone = TRUE;
                            semGive (pMotor->mutexSem);
                        }

		    }    

                    /*
                     *  Update the structure position and clear the local done flag,
                     *  then set local rescan flag to indicate that the record 
                     *  needs to see this.
                     */

                    pDevice->position = position;
                    done = FALSE;
                    rescan = TRUE;
                    DEBUG(DDR_MSG_MAX, "<%ld> c:%d s:%d omsScanTask:device position change%c\n", ' ');
	        }


                /*
                 *  Update current encoder position regardless, otherwise we
                 *  get a stalled motor at startup. If the motor should not be 
                 *  moving  then you're seeing some spontaneous motion, so
                 *  update the encoder and set rescan flag.  Plus or minus a few 
                 *  counts is probably okay but make this determination at the 
                 *  rec level.
                 */

                if ( pdr->ueip && pDevice->encoder != encoder ) 
                { 
                    if ( !(pDevice->moving) && !(pDevice->simulation) )
                    {
                        DEBUG(DDR_MSG_MIN, "<%ld> c:%d s:%d omsScanTask:spontaneous encoder change:%ld counts\n", (pDevice->encoder - encoder));
                    }

                    pDevice->encoder = encoder;
                    rescan = TRUE;
                }


                /*
                 *  Look for changes in the home and limit switch states, if 
                 *  they have changed the record needs to know about it.
                 */

                if ( pMotor->updateState )
                {
                    if (lowLimit != pDevice->lowLimit)
                    {
                        DEBUG(DDR_MSG_FULL,
                          "<%ld> c:%d s:%d omsScanTask:low limit change:%d\n",
                          lowLimit);
                        pDevice->lowLimit = lowLimit;
                        rescan = TRUE;
                    }

                    if (highLimit != pDevice->highLimit)
                    {
                        DEBUG(DDR_MSG_FULL,
                          "<%ld> c:%d s:%d omsScanTask:high limit change:%d\n",
                          highLimit);
                        pDevice->highLimit = highLimit;
                        rescan = TRUE;
                    }

                    if (homeSwitch != pDevice->homeSwitch)
                    {
                        DEBUG(DDR_MSG_FULL,
                          "<%ld> c:%d s:%d omsScanTask:home switch change:%d\n",
                          homeSwitch);
                        pDevice->homeSwitch = homeSwitch;
                        rescan = TRUE;
                    }

                    semTake (pMotor->mutexSem, WAIT_FOREVER);
                    pMotor->updateState = FALSE;
                    semGive (pMotor->mutexSem);
                }
            }   /*  end of if ( !pMotor->status )... */

            /*
             *  Update the motor status if it's changed.
             */

            if (pMotor->status != pDevice->status)
            {
                DEBUG(DDR_MSG_MIN,
                 "<%ld> c:%d s:%d omsScanTask:motor status != device status%c\n",
                 ' ');
                pDevice->status = pMotor->status;
                rescan = TRUE;
            }

            semGive (pDevice->mutexSem);


            /*
             *  Check the timeout and rescan flags.   If either of them
             *  have been set then re-processes the associated record.
             */

            if (pDevice->timeout || (rescan && !pDevice->initializing))
            {
                if ( pDevice->debug == DDR_MSG_MAX )
                {
                    logMsg ("<%d> callback! <%x,%x,%ld,%ld,%d>\n",
                            (int)tickGet(),
                            (pMotor->card << 4) + pMotor->axis,
                            ((pDevice->highLimit << 12) +
                             (pDevice->lowLimit << 8) +
                             (pDevice->moving << 4) +
                             (pDevice->timeout)),
                            pDevice->position,
                            pDevice->encoder,
                            pDevice->status);
                }


                /*
                 *  Setting the callback flag tells the record processing
                 *  function that it has been called by the device support
                 */

                semTake (pDevice->mutexSem, WAIT_FOREVER);
                pDevice->callback = TRUE;
                semGive (pDevice->mutexSem);


                /*
                 *  Re-process the record by calling its process function
                 *  directly.   The processing function is recovered from the
                 *  record function structure.  Scan lock prevents anyone else
                 *  from accessing the record while it is being processed
                 *  from this task.
                 */

                pRset = (struct rset *) pdr->rset;
                dbScanLock ((struct dbCommon *) pdr);
                (*pRset->process) (pdr);
                dbScanUnlock ((struct dbCommon *) pdr);
            }
            
            
            /*
             * And move on to the next motor on the list
             */
            
            pMotor = (DEV_CTL_OMS_PRIVATE *) ellNext (&pMotor->node);
       
        }   /* end of motor scanning loop */


        /*
         * Subtract from the delay the amount of time spent processing this
         * loop.   This should allow the scan task rate to remain more stable
         * despite changes in CPU usage/speed ... AWE 07/99
         */

        taskTime = (int)tickGet() - startTick;

        if (taskTime < scanTaskPeriod)
        {
            status = taskDelay (scanTaskPeriod - taskTime);
            if (status < 0)    /* call to taskDelay failed */
            {
                SET_ERR_MSG("omsScanTask Warning: taskDelay failed ..timing off");
                DEBUG(DDR_MSG_WARNING, 
                      "<%ld> c:%d s:%d omsScanTask: taskDelay failed, so timing off - error:%ld\n", status);
            }
        }
        else
        {
            DEBUG(DDR_MSG_MIN,
                   "<%ld> c:%d s:%d omsScanTask:Slow by %d ticks!\n",taskTime);
        }

    }  /* end of main loop */

    return( -1 );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * setDelay
 *
 * INVOCATION:
 * status = setDelay (pDevice, delay);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pDevice  (DEVICE_CONTROL_PRIVATE *) Record interface structure
 * (>) delay    (long) Desired delay time in 0.1 second increments
 *
 * FUNCTION VALUE:
 * (long) processing success code.
 *
 * PURPOSE:
 * Re-process the calling record after a preset interval
 *
 * DESCRIPTION:
 * Load and enable the background delay timer.   Each time the scan task
 * executes the delay timer is decremented.   When this timer expires
 * the scan task will re-process the record.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * omsScanTask
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long setDelay
(
    DEVICE_CONTROL_PRIVATE *pDevice,  /* deviceControl rec interface struct */
    long        delay                 /* time to delay in 0.1 second units  */
                                      /*    before record is re-processed   */
)
{
    DEV_CTL_OMS_PRIVATE *pMotor = pDevice->pPrivate;


    /*
     *  Delay is time in 0.1 second units (I.e.  a value of 20 is
     *  2.0 seconds and 36 is 3.6 seconds.   Convert this time to the
     *  number of oms scans, so that the background task can count them
     *  down (scansLeft becomes a timer indicating the number of times
     *  omsScanTask will run before the specified timeout).   Setting 
     *  scansLeft to a non-zero number automatically starts the
     *  countdown timer.   Setting scansLeft to zero disables the
     *  timer. 
     */

    semTake (pMotor->mutexSem, WAIT_FOREVER);
    pMotor->scansLeft = (delay * DDR_OMS_SCAN_TASK_RATE) / 10;
    semGive (pMotor->mutexSem);

    DEBUG(DDR_MSG_MAX, 
          "<%ld> c:%d s:%d Set delay to <%ld>\n", pMotor->scansLeft);

    return( 0 );

}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * setPosition
 *
 * INVOCATION:
 * status = setPosition (pDevice, position);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pDevice  (DEVICE_CONTROL_PRIVATE *) Record interface structure.
 * (>) positon    (long) Desired position register value in steps.
 *
 * FUNCTION VALUE:
 * (long) processing success code.
 *
 * PURPOSE:
 * Update hardware position register
 *
 * DESCRIPTION:
 * In simulation mode simply updat the local position counter otherwise
 * load the given value into the drive electronics position counter.
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

static long setPosition
(
    DEVICE_CONTROL_PRIVATE *pDevice,  /* deviceControl record interface str */
    long             position         /* absolute position for this motor   */
)
{
    DEV_CTL_OMS_PRIVATE *pMotor = pDevice->pPrivate;  /* motor control str  */
    char scratch[DRV_OMS_VME_MAX_MSG_LEN];      /* message assembly buffer  */
    long status = 0;                        /* function return status       */

    DEBUG(DDR_MSG_FULL, 
          "<%ld> c:%d s:%d Set position to:%ld \n", position);


    /*
     *  In simulation mode we simply set the current position to the
     *  desired position.
     */

    if (pDevice->simulation)
    {
        semTake (pDevice->mutexSem, WAIT_FOREVER);
        pDevice->position = position;
        semGive (pDevice->mutexSem);
    }


    /*
     *  Otherwise send the position to the OMS card
     */

    else
    {
        /*
         *  Confirm that the card and axis were found at init time.
         */

        if (!pMotor->exists)
        {
            SET_ERR_MSG("Motor hardware does not exist");
            return DRV_OMS_VME_S_CFG_ERROR;
        }
           

        /*
         *  Use the Load Position command followed by the new
         *  position value to load.
         */

        sprintf (scratch, "LP%ld ", position);


        /*
         *  Then write the string to the appropriate axis on the appropriate
         *  OMS card.
         */

        status = drvOmsVmeWriteMotor (pMotor->card,
                                      pMotor->axis,
                                      scratch); 
        if (status)
        {
            drvOmsVmeGetErrorMessage (pDevice->errorMessage);
            DEBUG(DDR_MSG_ERROR, 
                  "<%ld> c:%d s:%d OMSscanTask: %s\n", pDevice->errorMessage);
        }
    }

    return status;
}

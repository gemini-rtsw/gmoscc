
/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) 1998.                        (c) 1998
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
 * recDeviceControl.h
 *
 * PURPOSE:
 * Publish epics deviceControl record public information for the
 * recDeviceControl module.
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.1  2001/08/10 14:01:49  ptaylor
 * Restructured src directory with 4 sub-directories, including pv and lut which were previously in pv as well as deviceControl and gcal previously in src
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.5  2000/12/19 13:30:49  gmos
 * Add DDR_DBUG_QUIET and DAR_DBUG_QUIET to the list of debugging modes.
 *
 * Revision 1.4  2000/11/10 00:16:45  gmos
 * Add rejectAck, rejectErrMess & actionErrMess to structure and
 * alphabetize members.
 *
 * Revision 1.3  2000/07/06 17:24:53  gmos
 * Williams new code, tested and debugged
 *
 * Revision 1.2  2000/04/14 10:00:39  gmos
 * Merged pre and post FP software
 *
 * Revision 1.1.1.1  2000/02/29 11:42:26  gmos
 * V1.00 release from HIA
 *
 * Revision 1.12  2000/01/27 21:26:22  dunn
 * Removed retry timeout, not used.  Also some alignment of lines.
 *
 * Revision 1.11  2000/01/27 19:19:44  angelic
 * added retryPowerTimeout to private structure
 *
 * Revision 1.10  1999/11/22 23:01:26  angelic
 * added badRead to the private structure
 *
 * Revision 1.9  1999/11/13 00:39:20  angelic
 * added move_while_moving to private structure
 *
 * Revision 1.8  1999/10/30 00:20:40  rambold
 * added private mode and fault entries to allow
 * pre-checking of commands without affecting command in progress
 *
 * Revision 1.7  1999/08/13 19:31:26  angelic
 * *** empty log message ***
 *
 * Revision 1.6  1999/07/30 18:27:59  angelic
 * added stalled_once flag to the private structure
 *
 * Revision 1.5  1999/07/28 19:52:09  rambold
 * added error message field to private structure
 *
 * Revision 1.4  1999/05/26 21:04:22  angelic
 * bug fixes
 *
 * Revision 1.3  1999/04/28 20:27:58  dunn
 * Bug fixes.
 *
 * Revision 1.2  1999/02/26 20:44:08  rambold
 * beta version
 *
 * Revision 1.1  1998/11/20 03:19:45  rambold
 * Initial revision
 *
 *INDENT-ON*
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/

#ifndef REC_DEVICE_CONTROL_INC
#define REC_DEVICE_CONTROL_INC

/*
 *  motor control mnemonics
 */

#define DDR_MOVE_GO             0       /* start moving to position         */
#define DDR_MOVE_STOP           1       /* bring motor to a stop            */
#define DDR_MOVE_ABORT          2       /* abort motion immediately         */

/*
 * define a mnemonic for the deviceControl record database structure
 */

typedef struct deviceControlRecord DEVICE_CONTROL_RECORD;


/*
 * define the generic device support interface and motion control structure
 */

typedef struct
{
    long        acceleration;       /* acceleration rate in steps/sec/sec   */
    int         backlashMotion;     /* anti-backlash motion in progress(T/F)*/
    int         badRead;            /* invalid encoder reading (T/F)        */
    long        baseVelocity;       /* ramp starting velocity in steps/sec  */
    int         callback;           /* callback processing (T/F) flag       */
    int         checkLimits;        /* re-check limits (T/F) flag           */
    int         debug;              /* current debugging mode               */
    long        encoder;            /* current motor position (from encoder)*/
    long        encoderDeadband;    /* encoder-reported position deadband   */
    char        errorMessage[MAX_STRING_SIZE]; /* root (intial) error msg   */
    int         fault;              /* interlock line active (T/F)          */
    int         faultChange;        /* flag to show interlock line changed
                                         from previous value  (T/F)         */
    int         highLimit;          /* upper limit switch is active (T/F)   */
    int         homeSwitch;         /* home switch is active (T/F)          */
    int         index;              /* current indexing mode                */
    long        indexVelocity;      /* final indexing velocity in steps/sec */
    int         initializing;       /* record is initializing               */
    int         insideDeadband;     /* motor is inside deadband (T/F)       */
    int         lowLimit;           /* lower limit switch is active (T/F)   */
    long        mode;               /* current operating mode               */
    int         move_while_busy;    /* new move requested during move (T/F) */
    int         moving;             /* motor is currently moving (T/F)      */
    SEM_ID      mutexSem;           /* mutual exclusion semaphore           */
    long        position;           /* current motor position (from card)   */
    void        *pPrivate;          /* internal device control struct ptr   */
    short       rejectAck;        /* command rejection acknowledge flag   */
    char        rejectErrMess[MAX_STRING_SIZE]; /* cmd rejection error msg  */
    char        actionErrMess[MAX_STRING_SIZE]; /* cmd action error msg     */
    int         simmChange;         /* simulation mode has changed          */
    short       simmHpvl;           /* index valid state entering simulation*/
    int         simulation;         /* current simulation mode              */
    int         stalled_times;      /* # of identical positions before stall*/
    long        status;             /* motion status                        */
    long        target;             /* target position in steps             */
    long        timeout;            /* timeout processing (T/F) flag        */
    long        velocity;           /* motion velocity in steps/sec         */
}  DEVICE_CONTROL_PRIVATE;

 
#endif /* REC_DEVICE_CONTROL_INC */

/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) 1998.                (c) 1998
 * National Research Council          Conseil national de recherches
 * Ottawa, Canada, K1A 0R6            Ottawa, Canada, K1A 0R6
 * All rights reserved                Tous droits reserves
 *                     
 * NRC disclaims any warranties,      Le CNRC denie toute garantie
 * expressed, implied, or statu-      enoncee, implicite ou legale,
 * tory, of any kind with respect     de quelque nature que se soit,
 * to the software, including         concernant le logiciel, y com-
 * without limitation any war-        pris sans restriction toute
 * ranty of merchantability or        garantie de valeur marchande
 * fitness for a particular pur-      ou de pertinence pour un usage
 * pose.  NRC shall not be liable     particulier.  Le CNRC ne
 * in any event for any damages,      pourra en aucun cas etre tenu
 * whether direct or indirect,        responsable de tout dommage,
 * special or general, consequen-     direct ou indirect, particul-
 * tial or incidental, arising        ier ou general, accessoire ou
 * from the use of the software.      fortuit, resultant de l'utili-
 *                                    sation du logiciel.
 *
 ************************************************************************
 *
 * FILENAME
 * recAssControl.h
 *
 * PURPOSE:
 * Publish assemblyControl record public information
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.5  2000/12/19 13:30:49  gmos
 * Add DDR_DBUG_QUIET and DAR_DBUG_QUIET to the list of debugging modes.
 *
 * Revision 1.4  2000/11/16 10:24:07  gmos
 * Added magic value to the data structures to allow a check for memory corruption.
 *
 * Revision 1.3  2000/08/08 13:18:03  cjt
 * Fixed MONITOR_VALX pointer bug.
 *
 * Revision 1.2  2000/05/30 16:29:11  gmos
 * New version commented by William Rambold. Master enable bug fixed.
 *
 * Revision 1.11  1999/11/06 01:14:07  angelic
 * added showBusy to private structure
 *
 * Revision 1.10  1999/10/30 00:23:11  rambold
 * added defines to allow device support to raise monitors on VALx fields.
 *
 * Revision 1.9  1999/10/27 00:23:06  rambold
 * added monitor support for VALx fields
 *
 * Revision 1.8  1999/10/25 18:15:12  angelic
 * added keepIndex to private structure
 *
 * Revision 1.7  1999/08/18 23:52:30  angelic
 * added failedPreset to the private structure
 *
 * Revision 1.6  1999/07/28 19:55:03  rambold
 * added MAX debugging level
 *
 * Revision 1.5  1999/06/25 17:47:54  angelic
 * gave error message in CHECKSTAT a debug level of DAR_DBUG_NONE
 *
 * Revision 1.4  1999/06/16 20:38:29  angelic
 * *** empty log message ***
 *
 * Revision 1.3  1999/05/26 18:05:01  dunn
 * Added external functions:
 * assAddErrorMessage, assAttachPrivateStruct, assCommandFinish, assCommandTimedOut,
 * assGetBusResponse, assGetPrivateStruct, assStopTimer, assStartTimer
 * assSimulateLevel.
 *
 * Revision 1.2  1999/05/07 17:12:25  dunn
 * Added a busChange flag and a special callback pointer.
 *
 * Revision 1.1  1999/04/28 21:05:00  dunn
 * Initial revision
 *
 *INDENT-ON*
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/

#ifndef    REC_ASS_CONTROL_INC
#define    REC_ASS_CONTROL_INC


/*
 * Definitions for Assembly Control Record recChoice field values
 */
                                             
/* DIRective field (matches Gemini CAD record) */
#define DAR_DIR_MARK            0
#define DAR_DIR_CLEAR           1
#define DAR_DIR_PRESET          2
#define DAR_DIR_START           3
#define DAR_DIR_STOP            4

/* MODE field */
#define DAR_MODE_INIT           0
#define DAR_MODE_MOVE           1
#define DAR_MODE_TRACK          2
#define DAR_MODE_INDEX          3
#define DAR_MODE_PARK           4
#define DAR_MODE_TEST           5
#define DAR_MODE_UPDATE         6

/* BUSY field (matches Gemini CAR record) */
#define DAR_BUSY_IDLE           0 
#define DAR_BUSY_BUSY           2
#define DAR_BUSY_ERROR          3

/* SIMulation field */
#define DAR_SIM_NONE            0
#define DAR_SIM_VSM             1
#define DAR_SIM_FAST            2
#define DAR_SIM_FULL            3

/* DeBUG field */
#define DAR_DBUG_QUIET          0
#define DAR_DBUG_NONE           1
#define DAR_DBUG_MIN            2
#define DAR_DBUG_FULL           3
#define DAR_DBUG_MAX            4

/* HeaLTH field */
#define DAR_HLTH_GOOD           0
#define DAR_HLTH_WARNING        1
#define DAR_HLTH_BAD            2

/* Assembly STAte field */
#define DAR_STATE_IDLE          0
#define DAR_STATE_INITIALIZING  1
#define DAR_STATE_MOVING        2
#define DAR_STATE_TRACKING      3
#define DAR_STATE_TESTING       4
#define DAR_STATE_INDEXING      5
#define DAR_STATE_UPDATING      6
#define DAR_STATE_STARTING      7
#define DAR_STATE_ERROR         8

/* ACKnowledge field */
#define DAR_ACK_VAL_ACCEPT      0
#define DAR_ACK_VAL_REJECT      -1


/*
 * Definitions for Assembly Control Record recChoice field values
 */
                                             
/* DIRective field*/
#define DAR_DEV_DIR_STOP        0
#define DAR_DEV_DIR_CHECK       1
#define DAR_DEV_DIR_GO          2

/* BUSY field (does not match CAR record) */
#define DAR_DEV_BUSY_IDLE       0 
#define DAR_DEV_BUSY_BUSY       1
#define DAR_DEV_BUSY_ERROR      2

/* ACKnowledge field */
#define DAR_DEV_VAL_ACCEPT      0
#define DAR_DEV_VAL_REJECT      1


/*
 * Function return status codes
 */ 

#define DAR_E_DEV_PTR           -16    /* Null device func ptr.      */
#define DAR_E_NULL_NAME         -15    /* Null name in cvtDbAddr.    */
#define DAR_E_NULL_STAT         -14    /* Null return status.        */
#define DAR_E_ATT               -13    /* Bad attribute.             */
#define DAR_E_TIMEDOUT          -12    /* Command timed out.         */
#define DAR_E_MODE              -11    /* Invalid Mode.              */
#define DAR_E_FLT_CLR           -10    /* Fault not cleared.         */
#define DAR_E_INV_DIR           -9     /* Invalid directive.         */
#define DAR_E_DEVICE            -8     /* Device failed.             */
#define DAR_E_BUSY              -7     /* Currently busy.            */
#define DAR_E_BAD_ATTR          -6     /* Attribute out of range.    */
#define DAR_E_TIMER_CAN         -5     /* Failed to cancel timer.    */
#define DAR_E_MALLOC            -4     /* Failed to malloc room.     */
#define DAR_E_STOP_TIMER        -3     /* Failed to stop timer.      */
#define DAR_E_TASK_SPAWN        -2     /* Failed to spawn a task.    */
#define DAR_FAILURE             -1     /* Generic bad news           */
#define DAR_S_SUCCESS           0      /* Success code               */   


/*
 *  Create shorter name for assembly control record structure
 */

typedef struct assemblyControlRecord ASSEMBLY_CONTROL_RECORD;


/*
 *  Define the local (private) control structure
 */

typedef struct
{
    int     bus1;               /* Set when BUS1 changes    */
    int     bus2;               /* Set when BUS2 changes    */
    int     bus3;               /* Set when BUS3 changes    */
    int     bus4;               /* Set when BUS4 changes    */
    int     bus5;               /* Set when BUS5 changes    */
    int     busChange;          /* Change in one of the above detected */
    int     cmdFinished;        /* Command processing has finished     */
    int     showBusy;           /* Command acknowledgement cycle flag  */
    int     cmdResponse;        /* Command was successfull flag        */
    int     keepIndex;          /* Keep index even if command fails    */
    int     cmdTimedOut;        /* Command has timed out flag          */
    int     debug;              /* Current debug mode                  */
    int     fault;              /* Interlock (ILCK) field set flag     */
    int     lastCommand;        /* Mode of last command executed       */
    int     currCommand;        /* Mode of currently executing command */
    void    *pPrivate;          /* Ptr to local control structure      */
    int     simulation;         /* Current simulation mode             */
    void    *special;           /* Special callback structure pointer  */
    long    timeout;            /* Command timedout flag               */
    void    *timer;             /* Timeout callback structure pointer  */
    long     magic;             /* Magic safety value                  */
}  ASS_CONTROL_PRIVATE;

/*
 *  Magic value to be inserted into the local (private) control structure
 */

#define DAR_MAGIC  0x20617373   /* Magic value for assembly record private structure */   


/*
 * CHECKSTAT and CHECKFNSTAT are now implemented as uppercase and
 * can rubustly be called from any part of a function without 
 * processing the "to be checked" routine more than once.
 * Thanks to SMB - 14 June 1999 for detecting this problem.
 *
 * ************************************************************************
 * NOTE - The CHECKSTAT macro assumes that a DEBUG macro has been
 *        declared elsewhere in the source code - see recAsscontrol.c for
 *        an example. The arguments of the DEBUG macro are always the same
 *        but the contents of the macro may vary.
 * ************************************************************************
 */


/*
 * Check the status return of a function.   If an error occurred then log
 * the status code and return as directed
 */

#define CHECKSTAT( s, ret )                                             \
{                                                                       \
    int inS = s;                                                        \
    if ( (inS) != DAR_S_SUCCESS )                                       \
    {                                                                   \
        DEBUG(DAR_DBUG_NONE, "<%d> %s:ERROR, status=%ld\n", inS);       \
        ret;                                                            \
    }                                                                   \
}
 

/*
 * Execute the given function (usually a system call) and return a 
 * generic staus code if the function fails.
 */

#define CHECKFNSTAT( fn, s, ret )                                       \
{                                                                       \
    int inS;                                                            \
    inS = fn;                                                           \
    if ( (inS) != DAR_S_SUCCESS )                                       \
    {                                                                   \
        s = DAR_FAILURE;                                                \
        ret;                                                            \
    }                                                                   \
}

/*
 *  External functions.
 */

extern void    assAddErrorMessage(ASSEMBLY_CONTROL_RECORD *, char * );
extern void    assAttachPrivateStruct(ASSEMBLY_CONTROL_RECORD *, void * );
extern void    assCommandFinish( ASSEMBLY_CONTROL_RECORD *, int, char * );
extern int     assCommandTimedOut( ASSEMBLY_CONTROL_RECORD * );
extern void    assGetBusResponse( ASSEMBLY_CONTROL_RECORD *, int *, int *, 
                                  int *, int *, int * );
extern void    *assGetPrivateStruct(ASSEMBLY_CONTROL_RECORD * );
extern void    assDisplayPrivateStruct(ASSEMBLY_CONTROL_RECORD * );
extern int     assSimulateLevel( ASSEMBLY_CONTROL_RECORD * );
extern void    assStartTimer( ASSEMBLY_CONTROL_RECORD *, long *, int );
extern void    assStopTimer( ASSEMBLY_CONTROL_RECORD *, long * );


/*
 * Define the maximum number of attributes, device records or sensors
 * that can be attached to the record.
 */

#define DAR_ARG_MAX        5


/* 
 *  Provide a way for device support to raise monitors on the VALx fields
 *  during processing.
 */

#define MONITOR_VALA      db_post_events(par, par->vala, DBE_VALUE);
#define MONITOR_VALB      db_post_events(par, par->valb, DBE_VALUE);
#define MONITOR_VALC      db_post_events(par, par->valc, DBE_VALUE);
#define MONITOR_VALD      db_post_events(par, par->vald, DBE_VALUE);
#define MONITOR_VALE      db_post_events(par, par->vale, DBE_VALUE);

#endif /* REC_ASS_CONTROL_INC */

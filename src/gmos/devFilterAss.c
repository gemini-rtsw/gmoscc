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
 * devFilterAss.c
 *
 * PURPOSE:
 * EPICS Assembly Device Support code for the assemblyControl record.
 *
 * FUNCTION NAME(S)
 * flAckReceived	A busy link activated.
 * flBuildList		Build a task list.
 * flCheckAttributes	Check input attributes.
 * flCheckBarcodeId	Check returned barcode id.
 * flCheckBus		Check change in BUS values.
 * flDoTask		Setup to execute a task on the task list
 * flEmptyList		Delete from the task List
 * flExecuteTask	Execute a task on the task list.
 * flIndexMode		Execute INDEX mode command.
 * flInitDeviceSupport	Initialize instance of record's device support code.
 * flInitMode		Execute INIT mode command.
 * flMoveMode		Execute MOVE or PARK mode command.
 * flStopDirective	Execute STOP directive to attached records.
 * flTaskCheck		Check that the task succeeded.
 * flTaskFinished	Determine if a task list is finished.
 * flTerminateTasks     Finish the current command
 * flTestMode		Execute the TEST mode.
 * flTrackMode		Execute the TRACK mode.
 * flTranslateDir	Translate dir directive for device record.
 * flUpdateMode		Execute UPDATE mode command.
 * flProcessFault	Tidy up after fault (interlock occurred)
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.1  2001/11/28 20:08:46  mbec
 * *** empty log message ***
 *
 * Revision 1.3  2001/04/27 21:33:16  smb
 * TEST command now passed on to filter and grating devices.
 *
 * Revision 1.2  2001/04/23 18:24:42  smb
 * DEBUG macro changed from logMsg to printf so it can display floating point values (bug 196)
 *
 * Revision 1.50  2001/03/20 13:40:26  gmos
 * Modified DEBUG macro. All files now use printf() rather than logMsg(). All also print the output from taskName(0).
 *
 * Revision 1.49  2001/02/28 17:24:09  gmos
 * Renamed gmMessageLevels.h to darMessageLevels.h.
 *
 * Revision 1.48  2001/02/26 16:30:56  gmos
 * Removed CHECK_NULL macro and replaced it with explicit error reporting code.
 *
 * Revision 1.47  2001/02/23 18:13:20  gmos
 * Commenting improved. Discard old commented out code (except for thermal correction code). Include a printf within FL_CHECK_NULL and GR_CHECK_NULL just in case.
 *
 * Revision 1.46  2001/02/08 13:35:38  gmos
 * Do not check in position switches after a move to a barcode position. Some really long lines wrapped at column 105.
 *
 * Revision 1.45  2001/01/26 17:02:09  gmos
 * Commented last revision.
 *
 * Revision 1.44  2001/01/26 16:44:36  gmos
 * Fixed flStopDirective, which was placing a STOP task on the task list,
 * even if no devices had to be stopped. Caused "busy with tasks error" on
 * next directive.
 *
 * Revision 1.43  2001/01/26 16:11:43  gmos
 * Revamped the QUICK update mode. Now looks for missing entries in the table,
 * rather than just bad barcodes. More efficient task sequence implemented.
 *
 * Revision 1.42  2001/01/24 18:25:15  gmos
 * Updated QUICK update mode - one shot only now.
 *
 * Revision 1.41  2000/12/12 10:27:18  gmos
 * Changed error message when on position switch not seen to emphanises an INDEX is needed.
 *
 * Revision 1.40  2000/12/12 09:51:43  gmos
 * Some tidying up. Check status from assStartTimer.
 *
 * Revision 1.39  2000/12/08 15:11:17  gmos
 * Protect all access to device private structures with mutex semaphore. flTerminateTasks function used to ensure timer is always stopped and task list always emptied.
 *
 * Revision 1.38  2000/12/08 09:24:21  gmos
 * Changed DEBUG statements to use new message type constants. flReadConfig transferred to task list in INIT, so file I/O failure does not stop devices being initialised. Empty stub file created if config file does not exist. Config database handling modified to use high water mark.
 *
 * Revision 1.37  2000/11/16 13:04:06  gmos
 * Magic number checks included.
 *
 * Revision 1.36  2000/11/16 10:43:31  gmos
 * Bug in initMode, in which data structure pointers were obtained in the wrong order, corrected. Checks on the return from assGetprivateStructure added throughout.
 *
 * Revision 1.35  2000/11/10 17:15:38  gmos
 * Improved clarity of device failure messages.
 *
 * Revision 1.34  2000/10/26 10:15:25  gmos
 * Initialise pFlPriv->position fields in INIT mode. Removed some out of date source comments.
 *
 * Revision 1.33  2000/10/17 13:56:39  gmos
 * Meaningless error messages demoted to DAR_DBUG_MIN.
 *
 * Revision 1.32  2000/10/05 14:40:46  gmos
 * Optimised order of barcode reading positions in flCharacterise. Should make
 * UPDATE slightly quicker.
 *
 * Revision 1.31  2000/09/04 09:30:52  gmos
 * Random tabs in the source code removed.
 *
 * Revision 1.30  2000/08/16 10:56:49  gmos
 * Implemented the ability to ignore in position switches
 *
 * Revision 1.29  2000/08/14 16:58:13  gmos
 * Add selectable load positions to filter assembly
 *
 * Revision 1.28  2000/07/18 10:56:18  cjt
 * Added check of wheel number in flReadConfig().
 *
 * Revision 1.27  2000/07/18 09:30:26  cjt
 * Added checking of keywords on first line of lookup tables.
 *
 * Revision 1.26  2000/07/14 16:00:55  cjt
 * Added checking of in-position switches during UPDATE.
 *
 * Revision 1.25  2000/07/14 14:22:53  cjt
 * Fixed the MOVE while MOVING bugs in xxCheckAttributes.
 *
 * Revision 1.24  2000/07/14 08:40:01  gmos
 * Removed recGblRecordError calls when there is a file I/O rather than a record error. Translation stage will now report when it cant open a LUT file.
 *
 * Revision 1.23  2000/07/10 16:33:39  cjt
 * Added error message in DoTask() for failure to WriteConfig().
 *
 * Revision 1.22  2000/07/10 14:16:45  gmos
 * I forgot to set keepIndex for a file read error. Fixed
 *
 * Revision 1.21  2000/07/10 13:52:01  cjt
 * Fixed StopDirective race condition bug.
 *
 * Revision 1.20  2000/07/10 13:26:46  gmos
 * Fixed problems reporting file I/O error messages. Set keepIndex to keep index after soft error.
 *
 * Revision 1.19  2000/07/06 17:27:16  gmos
 * strcpy replaced by strncpy. Additional status checks after reading barcode.
 *
 * Revision 1.18  2000/07/03 15:54:23  cjt
 * Added checks to return values of some recGbl calls.
 *
 * Revision 1.17  2000/06/26 13:33:34  gmos
 * DEBUG replaced by FLDEBUG
 *
 * Revision 1.16  2000/06/13 09:53:41  cjt
 * Fixed SAD barcode bug - minor typo.
 *
 * Revision 1.15  2000/06/08 10:45:56  cjt
 * Updated references to (ASSEMBLY_CONTROL_RECORD *) from "pdr" to "par".
 *
 * Revision 1.14  2000/06/07 12:51:09  gmos
 * Display name of backup LUT file on console
 *
 * Revision 1.13  2000/06/06 14:07:32  cjt
 * Fixed STOP directive "invalid task" bug.
 *
 * Revision 1.12  2000/06/06 11:19:50  gmos
 * Changed one or two debug messages
 *
 * Revision 1.11  2000/06/06 10:49:19  cjt
 * Added support for lookup table backups. And Quick Update in devFilterAss.c
 *
 * Revision 1.10  2000/05/18 17:00:42  gmos
 * Removed redundant devFilterAss.h devGratingAss.h and devTranslationAss.h files
 *
 * Revision 1.9  2000/05/11 15:01:59  gmos
 * Assembly name included in error messages
 *
 * Revision 1.8  2000/05/03 16:14:36  cjt
 * Improved in-position switch checking for filter and grating assemblies. Files
 * devGratingAss.c (.h), devFilterAss.c (.h) and filter.sch modified.
 *
 * Revision 1.7  2000/04/14 10:00:35  gmos
 * Merged pre and post FP software
 *
 * Revision 1.6  2000/04/03 15:43:04  cjt
 * Version 5 merged into old version 4 files. *NEW.* files removed. Correction to half-step mode support in post focal plane device records.
 *
 * Revision 1.4  2000/03/27 10:32:33  cjt
 * Copied grating tilt interpolation functionailty in gratingTilt.c to devGratingAssNEW.c
 * Removed gensub record from devGrating.sch, replaced by extra device support in assemblyControl record in grating.sch (above).
 * Increased size of some scratchBufs to 256 chars.
 *
 * Revision 1.3  2000/03/14 17:31:06  gmos
 * Philip Taylors more efficient version
 *
 * Revision 1.1  2000/03/03 11:41:00  pbt
 * With new DAO assembly record: added flProcessFault interlock handling
 *
 * Revision 1.5  2000/02/11 12:02:14  pbt
 *  Clear Task List on initialisation (in routine flInitMode)
 *
 * Revision 1.4  2000/02/11 11:40:34  pbt
 * Clear Task List on initialisation (in routine flInitMode)
 *
 * Revision 1.3  2000/01/13 12:39:10  gmos
 * Bugs and deficiencies noted
 *
 * Revision 1.2  1999/12/15 17:06:20  gmos
 * Fixed mistake in DEBUG macro format
 *
 * Revision 1.1  1999/12/10 14:33:41  gmos
 * Contents of assembly modules merged into gmoscc
 *
 * Revision 1.7  1999/11/30 14:03:11  gmos
 * UKATC banner added
 *
 * Revision 1.6  1999/11/24 12:17:38  gmos
 * Make sure all error messages are reported with DAR_DBUG_NONE to ensure thet are not lost
 *
 * Revision 1.5  1999/11/01 16:54:47  dlt
 * Implement empty filter barcode
 *
 * Revision 1.4  1999/10/21 15:57:05  dlt
 * Implement ID sad records
 *
 * Revision 1.3  1999/10/21 10:35:15  dlt
 * Move both wheels simultaniously when parking or updating
 *
 * Revision 1.2  1999/10/19 14:40:09  dlt
 * Implement update
 *
 * Revision 1.1  1999/10/19 08:08:48  dlt
 * Reimplement assembly support with tasks
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
#include        <taskLib.h>

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
#include        <tickLib.h>		/* Contains tickGet function used by DEBUG macro */
#include        <sysLib.h>		/* Contains sysClkRateGet.                       */
#include        <ellLib.h>		/* Contains ellList linked list stuff.           */

#include        <assemblyControlRecord.h> /* Assembly record general header.             */
#include        <recAssControl.h>	/* Assembly record record support header.        */
#include	<darMessageLevels.h>     /* GMOS message level definitions.               */

#include        <timeLib.h>		/* Gemini time library                           */

/*
 *  Local Defines
 */

#define FL_LUT_FIRST_STRING       "filter"	/* The first line of the filter lookup  */
						/* table file must contain this string. */

#define FL_BARCODE_MAX		  29999999	/* Maximum allowable filter barcode     */
#define FL_BARCODE_MIN		  20000000	/* Minimum allowable filter barcode     */
#define FL_BARCODE_MAX_SIM	  59999999	/* Maximum simulated barcode            */
#define FL_BARCODE_MIN_SIM	  50000000	/* Minimum simulated barcode            */
#define FL_NO_BARCODE		  -1		/* Code for undefined barcode.          */

#define FL_NUM_DEVICES		  2		/* Number of devices (filter wheels)	*/

#define FL_VELOCITY		  0.75		/* Filter velocity in EGU/sec	        */
#define FL_MAX_FILTERS		    24		/* Maximum number of filters altogether */
#define FL_COMMAND_TIMEOUT	   100		/* Normal command timeout in seconds.   */
#define FL_COMMAND_SHORT_TIMEOUT    10		/* Short command timeout in seconds.    */
#define FL_BARCODE_READ_DELAY        2		/* seconds to wait for barcode.         */

#define FL_ERROR_MESSAGE_SIZE      128		/* Size of error message buffer.	*/
#define FL_SCRATCH_BUFFER_SIZE     256		/* Size of scratch string buffer.	*/

/*
 * The following status codes supplement the DAR_ codes within recAssControl.h
 */

#define FL_LAST_TASK		 202		/* Return status - last task executed.		*/
#define FL_EMPTY_LIST		 203		/* Return status - all tasks complete.		*/

#define FL_BAD_STRUCTURE	-201		/* Return status - corrupted data structure.	*/
#define FL_BAD_DEV_CONNECT	-202		/* Return status - device not connected.	*/
#define FL_TASK_LIST_EMPTY	-203		/* Return status - task list empty.		*/
#define FL_TASK_LIST_NOT_EMPTY	-204		/* Return status - task list not empty.		*/
#define FL_INVALID_TASK		-205		/* Return status - invalid task number.		*/
#define FL_INVALID_LOCATION	-206		/* Return status - invalid filter location.	*/
#define FL_INVALID_BARCODE	-207		/* Return status - invalid barcode.		*/
#define FL_UNKNOWN_FILTER	-208		/* Return status - unknown filter.		*/
#define FL_ILLEGAL              -209		/* Rteunr status - illegal filter combination.	*/
#define FL_EMPTY_BARCODE_LIST	-210		/* Return status - empty barcode list.		*/
#define FL_TRIG_FAILED		-211		/* Return status - failed to trigger barcode.	*/
#define FL_FILT1_ERROR		-212		/* Return status - wheel 1 failed.		*/
#define FL_FILT2_ERROR		-213		/* Return status - wheel 2 failed.		*/
#define FL_FILTS_ERROR		-214		/* Return status - both wheels failed.		*/
#define FL_FILEIO_ERROR		-215		/* Return status - file open/read/write failed. */

/*
 *  Defines of the named positions in the device records.
 */

#define FL_NAME_POS_INDEX	"index"		/* VALS of index.	*/
#define FL_NAME_POS_PARK	"park"		/* VALS of park.	*/

/*
 *  Define device names.
 */

#define FLT1 0                                  /* Filter wheel 1.     */
#define FLT2 1                                  /* Filter wheel 2.     */

#define FL_DEV_NAME_FILTER1     "filter1"       /* filter1 device.     */
#define FL_DEV_NAME_FILTER2     "filter2"       /* filter2 device.     */

/*
 *  Define the attributes in and input sensors.
 */

#define FILT1_IN_POSITION_SW              par->sil    /* Ptr. to the filter 1 in-posn switch read. */
#define FILT1_BARCODE_READ_FIELD  (char *)par->sij    /* Ptr. to filter 1 barcode value read.      */
#define FILT1_BARCODE_TRIGGER             par->sor    /* Ptr. to filter 1 barcode link.            */

#define FILT2_IN_POSITION_SW              par->sim    /* Ptr. to the filter 2 in-posn switch read. */
#define FILT2_BARCODE_READ_FIELD  (char *)par->sik    /* Ptr. to filter 2 barcode value read.      */
#define FILT2_BARCODE_TRIGGER             par->sos    /* Ptr. to filter 2 barcode link.            */

/*
 * Data structure describing the state of the filter assembly record's links.
 */

typedef struct flDevInfo {
     struct link                *dirLink;	/* Dir's links.				*/
     struct link                *modLink;	/* Mod's links.				*/
     struct link                *posLink;	/* Pos's links.				*/
     struct link                *velLink;	/* Vel's links.				*/
     struct link                *ackLink;	/* Acks's links.			*/
} FL_DEV_INFO;

/*
 *  Task List structure.  Warning, static FL_TASK_LISTs below are dependent 
 *  on the order of variables in this structure.
 */

typedef struct taskList {
    int                 task;			/* Task to complete.			*/
    int                 fl1Response;		/* Response expected on BUS1 field.	*/
    int                 fl2Response;		/* Response expected on BUS2 field.	*/
} FL_TASK_LIST;

typedef struct flList {
    ELLNODE             node;			/* Ptr. to next task list node.		*/
    void                *item;			/* Ptr. to item list.			*/
} FL_LIST;


/*
 * Define the primitive tasks the assembly can be asked to do.
 * These tasks may be combined together to define how to respond
 * to particular commands.
 */

typedef enum
{
    FL_STOP_TASK = 0,		/* Stop everything. 				*/
    FL_INIT_ALL,		/* Initialise everything.			*/
    FL_TEST_ALL,		/* Test everything.				*/
    FL_INDEX_ALL,		/* Index all devices.				*/
    FL_MV_FILTER1,		/* Move filter wheel 1.				*/
    FL_MV_FILTER2,		/* Move filter wheel 2.				*/
    FL_MV_FILTERS,		/* Move both filter wheels simultaneously.	*/
    FL_PARK_ALL_DEFAULT,	/* Park all devices at default positions.	*/
    FL_PARK_ALL_LOAD,		/* Park all devices at specified load positions.*/
    FL_READ_BARCODE_1,		/* Read filter 1 barcode.			*/
    FL_READ_BARCODE_2,		/* Read filter 2 barcode.			*/
    FL_WRITE_CONFIG,		/* Write filter config file with backup.	*/
    FL_MV_FILT_BARCODE		/* Move to filter 0 barcode reading position.	*/

    /*
     * The next 11 values are used for moving to barcode positions such that
     * "FL_MV_FILT_BARCODE + N" moves to the barcode reading position for filter N
     */

} flTaskList;


/*
 *  Create static list of primitive tasks to perform for each high level operation.
 */

static FL_TASK_LIST  flInit[] = {			/* INIT */
     { FL_INIT_ALL,             TRUE,  TRUE  }
};
static FL_TASK_LIST  flTest[] = {			/* TEST */
     { FL_TEST_ALL,             TRUE,  TRUE  }
};
static FL_TASK_LIST  flIndexAll[] = {			/* INDEX */
     { FL_INDEX_ALL,            TRUE,  TRUE  } ,
     { FL_PARK_ALL_DEFAULT,     TRUE,  TRUE  }
};
static FL_TASK_LIST  flParkAll[] = {			/* PARK */
     { FL_PARK_ALL_LOAD,        TRUE,  TRUE  }
};
static FL_TASK_LIST  flMoveFilter[] = {			/* Select filters */
     { FL_MV_FILTERS,           TRUE,  TRUE  }
};
static FL_TASK_LIST  flCharacterise[] = {		/* Full UPDATE */
     { FL_MV_FILT_BARCODE + 6,  TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 7,  TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 8,  TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 9,  TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 10, TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 11, TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 0,  TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 1,  TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 2,  TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 3,  TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 4,  TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 5,  TRUE,  TRUE  } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_WRITE_CONFIG,         FALSE, FALSE }
};

/*
 * The following tasks *do not* get executed in this order
 * they're here to provide operations needed for the variable
 * action "quick characterise"
 */

static FL_TASK_LIST  flQuickCharacterise[] = {		/* Quick UPDATE */
     { FL_WRITE_CONFIG,          FALSE, FALSE } ,
     { FL_READ_BARCODE_1,       FALSE, FALSE } ,
     { FL_READ_BARCODE_2,       FALSE, FALSE } ,
     { FL_MV_FILT_BARCODE + 0,  TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 1,  TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 2,  TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 3,  TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 4,  TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 5,  TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 6,  TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 7,  TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 8,  TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 9,  TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 10, TRUE,  TRUE  } ,
     { FL_MV_FILT_BARCODE + 11, TRUE,  TRUE  }
};


/*
 *  Device support function prototypes
 */

static long flAckReceived( ASSEMBLY_CONTROL_RECORD * );
static long flCheckAttributes( ASSEMBLY_CONTROL_RECORD * );
static long flIndexMode (ASSEMBLY_CONTROL_RECORD * );
static long flInitDeviceSupport( ASSEMBLY_CONTROL_RECORD * );
static long flInitMode (ASSEMBLY_CONTROL_RECORD * );
static long flMoveMode (ASSEMBLY_CONTROL_RECORD * );
static long flStopDirective( ASSEMBLY_CONTROL_RECORD * );
static long flTestMode (ASSEMBLY_CONTROL_RECORD * );
static long flTrackMode (ASSEMBLY_CONTROL_RECORD * );
static long flUpdateMode (ASSEMBLY_CONTROL_RECORD * );
static long flProcessFault (ASSEMBLY_CONTROL_RECORD * );


/*
 * Internal function prototypes
 * (flShowConfig is not static because it can be used with care from the vxWorks console)
 */
  
static long flBuildList( ASSEMBLY_CONTROL_RECORD *, const int ); 
static long flCheckBarcodeId( ASSEMBLY_CONTROL_RECORD *, const int );
static long flCheckBus( ASSEMBLY_CONTROL_RECORD *, const int, int );
static long flCheckInPosSwitches(ASSEMBLY_CONTROL_RECORD *par);
static void flClearConfig(ASSEMBLY_CONTROL_RECORD *par);
static long flDoTask( ASSEMBLY_CONTROL_RECORD * );
static long flEmptyList( ASSEMBLY_CONTROL_RECORD *, const int );
static long flExecuteTask( ASSEMBLY_CONTROL_RECORD *, FL_TASK_LIST *);
static long flTranslateDir( unsigned short );
static long flTaskFinished( ASSEMBLY_CONTROL_RECORD * );
static long flTaskCheck( ASSEMBLY_CONTROL_RECORD * );
static void flTerminateTasks( ASSEMBLY_CONTROL_RECORD *, long, char *);
static long flReadConfig(ASSEMBLY_CONTROL_RECORD *par);
       long flShowConfig(ASSEMBLY_CONTROL_RECORD *par);
static long flWriteConfig(ASSEMBLY_CONTROL_RECORD *par);

/*
 *  Data Structures - device support entry table
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
    DEVSUPFUN       processFault;	/* Added 3/3/00 PBT */
} devFilterAssembly = {
    13,                                 /* Number                        */
    NULL,                               /* Report function               */
    NULL,                               /* Initialise                    */
    flInitDeviceSupport,                /* Initialise record             */
    NULL,				/* Get I/O interrupt information */
                                        /* User defined functions:       */
    flCheckAttributes,                  /*    Check attributes           */
    flStopDirective,                    /*    Process STOP directive     */
    flInitMode,                         /*    Process INIT mode          */
    flMoveMode,                         /*    Process MOVE and PARK modes*/
    flTrackMode,                        /*    Process TRACK mode         */
    flIndexMode,                        /*    Process INDEX mode         */
    flTestMode,                         /*    Process TEST mode          */
    flAckReceived,                      /*    Acknowledgement received   */
    flUpdateMode,                       /*    Process UPDATE mode        */
    flProcessFault                      /*    Process interlock          */
    };

/*
 *  Internal private device support structures.
 */

typedef struct devConfig {
                                        /* Database of barcodes, wheel numbers and          */
                                        /* positions, read from a config file or determined */
                                        /* from an UPDATE.                                  */
     int                barcode[FL_MAX_FILTERS];
     int                wheel[FL_MAX_FILTERS];
     char               pos[FL_MAX_FILTERS][MAX_STRING_SIZE];

                                        /* Array of pointers into database.                 */
     int                lutIndex[FL_MAX_FILTERS];

     int                highwater;      /* High water mark in configuration database        */
     int                badBarcode;	/* Database contains at least one null barcode?     */
     int                characterized;  /* Configuration database is valid?                 */
     long               magic;          /* magic value to guard against pointer corruption. */
} FL_DEV_CONFIG;

typedef struct {
     char               pos[FL_NUM_DEVICES][MAX_STRING_SIZE];
					/* Positions				*/
     long               barcode[FL_NUM_DEVICES];
					/* barcode of these positions		*/
     char               position[FL_NUM_DEVICES][MAX_STRING_SIZE];
					/* Positions to go to.			*/
     int                parkPosition;   /* Parking position.                    */
     double             velocity;	/* Velocity to move at.			*/
     int                currentCmd;	/* Current command executing.		*/
     int                askBarCode;	/* Asked for a barcode.			*/
     int                whichLutIndex;	/* where in the table to store barcode.	*/
     int                mode;		/* Mode to execute.			*/
     int                updateMode;     /* Update or quick update ? 		*/
     int                ignoreSwitches; /* Ignore in position switches ?        */
     int                fl1Finished;	/* Bus1 finished.			*/
     int                fl2Finished;	/* Bus2 finished.			*/
     int                cmdActive;	/* Command is activated.		*/
     SEM_ID             mutexSem;	/* mutual exclusion semaphore 		*/
     ASSEMBLY_CONTROL_RECORD *pRecord;	/* Pointer to calling record structure 	*/
     FL_DEV_INFO        fl1Device;	/* Filter1 info. structure		*/
     FL_DEV_INFO        fl2Device;	/* Filter2 info. structure		*/
     ELLLIST            taskList;	/* List of things to do.		*/
     FL_DEV_CONFIG      *pFilterPriv;   /* Ptr to private filter state structure*/
     long               magic;          /* magic value to guard against pointer corruption */
} FL_DEV_PRIVATE;

#define FL_MAGIC  0x2020666C            /* magic value for filter data structures.*/


/*
 *  Macros
 *
 *  FLDEBUG  - Logs a message if the debug level of the message is less than the current
 *             debugging mode. The time and record name are prepended.
 *
 *  FLDEBUG  - A filter-specific wrapper for the DEBUG macro which avoids repeating
 *             the "<%ld> %s:" format throughout the code.
 */

#define DEBUG(l,FMT,V)						\
{								\
    int k=l;							\
    if (k <= par->dbug)						\
    {								\
        printf ("%s: "FMT, taskName(0), tickGet(), par->name, V);		\
    }								\
}

#define FLDEBUG(l,FMT,V)					\
{								\
    DEBUG (l, "<%ld> %s:" FMT, V); \
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flAckReceived
 *
 * INVOCATION:
 * status = flAckReceived( par ); 
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
 * The function is also called whenever the watchdog timer expires after
 * waiting for a barcode read, for example. Handling this state change is
 * accomplished via the following algorithm:
 *
 *   if a command is active then
 *   	ignore this
 *   else if we asked for a barcode ID, then
 *      then this task has finished, so call flTaskFinished()
 *   else if we timed out, then
 *	indicate the command finished by calling flTerminateTasks(), supplying
 *	an error message.
 *   else if any of the BUSx fields have a response then
 *    	For any lines that are set, then call flCheckBus() to handle
 *	the change.
 *   otherwise an error
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * Assumes that BUS1=FL1, BUS2=FL2 
 *
 * DEFICIENCIES:
 * None known. 
 *-
 ************************************************************************
 */

static long flAckReceived(ASSEMBLY_CONTROL_RECORD *par)
{
     FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/
     FL_DEV_CONFIG  *pDevConfig;
     long status = DAR_S_SUCCESS;	/* Function status.		*/
     int bus1;				/* Response from bus1 field.	*/
     int bus2;				/* Response from bus2 field.	*/
     int bus3;				/* Response from bus3 field.	*/
     int bus4;				/* Response from bus4 field.	*/
     int bus5;				/* Response from bus5 field.	*/

     pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pFlPriv->pFilterPriv;

     FLDEBUG(DAR_MSG_MAX, "flAckReceived: entry, cmdActive=%d\n", pFlPriv->cmdActive );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL || pDevConfig == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flAckReceived: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC || pDevConfig->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flAckReceived: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Ask assembly control record which command states have changed.
     */

     assGetBusResponse( par, &bus1, &bus2, &bus3, &bus4, &bus5);

     /*
      * Find out why an acknowledgement has been received.
      */

     if ( pFlPriv->cmdActive == FALSE )
     {
          /*
           * No command is active, so no change in the BUS links was expected.
           * This can typically happen when someone is using the device level
           * engineering screen or the assembly record's special processing gets
           * called for a reason other than a device response. Ignore it.
           */

          FLDEBUG(DAR_MSG_MIN, "flAckReceived: unexpected device response ignored%c\n", ' ');
     }
     else if ( assCommandTimedOut( par ))
     {
          if ( pFlPriv->askBarCode )
          {
                /*
                 *  A command timeout with the askBarCode flag set means that the
                 *  barcode reading delay has expired.   Call taskFinished to complete
                 *  the reading task.
                 */

               FLDEBUG(DAR_MSG_MIN, "flAckReceived: asked for a Barcode%c\n", ' ');
               status = flTaskFinished( par );
          }
          else
          {
               /*
                *  Any other timeout means that something has gone seriously wrong
                *  with the last command and the system has stopped responding.
                *  Abort the command immediately.
                */
               
               status = DAR_E_TIMEDOUT;
               FLDEBUG(DAR_MSG_ERROR, "flAckReceived: filter command timed out%c\n", ' ');

               flTerminateTasks( par, status, "Filter command timed out" );
          }
     }
     else if ( bus1 == TRUE || bus2 == TRUE )
     {
          /*
           *  else response received from device(s), handle
           */

          if ( status == DAR_S_SUCCESS && bus1 == TRUE )
          {
               /*
                *  Filter wheel 1 response.
                */

               if ( par->bus1 == DAR_DEV_BUSY_IDLE )
               {
                    semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                    pFlPriv->fl1Finished = TRUE;
                    semGive (pFlPriv->mutexSem);
               }
               FLDEBUG(DAR_MSG_MIN, "flAckReceived, flt1 response (bus1)=%d\n", par->bus1 );
               status = flCheckBus( par, 1, par->bus1 );
          }
          if ( status == DAR_S_SUCCESS && bus2 == TRUE )
          {
               /*
                *  Filter wheel 2 response.
                */

               if ( par->bus2 == DAR_DEV_BUSY_IDLE )
               {
                    semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                    pFlPriv->fl2Finished = TRUE;
                    semGive (pFlPriv->mutexSem);
               }
               FLDEBUG(DAR_MSG_MIN, "flAckReceived, flt2 response (bus2)=%d\n", par->bus2 );
               status = flCheckBus( par, 2, par->bus2 );
          }
     }
     else
     {
          /*
           *  Received bus acknowledgment from something other than bus1 or bus2.
           */

          FLDEBUG(DAR_MSG_ERROR, "flAckReceived, response from unexpected device%c\n", ' ');
     }

     return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flBuildList
 *
 * INVOCATION:
 * status = flBuildList (par, mode); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *)	Pointer to assemblyControl record
 *					structure.
 * (>) mode (const int)			Mode to act on.
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Build a list of tasks to perform based on the requested operating mode
 * and the current configuration of the filter assembly.
 *
 * DESCRIPTION:
 * This function uses the requested operating mode, together with the tasks
 * defined as static FL_TASK_LIST data structures at the start of this module,
 * to build a linked list of tasks to be performed.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * Make sure the list is empty, this is an error otherwise.
 * There must be sufficient free memory to allow the task list to be
 * created.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flBuildList(ASSEMBLY_CONTROL_RECORD *par, const int mode)
{
     FL_LIST        *newTask = NULL;
     long            status = DAR_S_SUCCESS;    /* Return function status.	*/
     FL_DEV_PRIVATE *pFlPriv;                   /* Ptr to private dev. struct.	*/
     FL_DEV_CONFIG  *pDevConfig;
     int             i, j, k;                   /* Counters.			*/
     int             wheelPosition;
     BOOL            readWheel1, readWheel2;    /* Flags for QUICK update       */
     int             foundEntry1, foundEntry2;  /* QUICK update - existing lines in the file        */
     int             highwater;                 /* Where to start overwriting the database (update) */
     int             testPosition;              /* The filter (update) position */

     pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pFlPriv->pFilterPriv;

     FLDEBUG(DAR_MSG_MAX, "flBuildList: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL || pDevConfig == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flBuildList: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC || pDevConfig->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flBuildList: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

     /*
      *  Make sure the list is empty.
      */

     if ( ellCount( &(pFlPriv->taskList) ) != 0 )
     {
          FLDEBUG(DAR_MSG_ERROR, "flBuildList: filter task list not empty. ellCount=%d\n", 
                ellCount( &(pFlPriv->taskList) ) );

          status = FL_TASK_LIST_NOT_EMPTY;
          assAddErrorMessage( par, "Filter busy with outstanding tasks");
          return ( status );
     }

     newTask = ( FL_LIST *) ellFirst( &(pFlPriv->taskList) );

     switch( mode )
     {
     case DAR_MODE_INDEX:

          /* 
           * Index all devices at same time
           */

          FLDEBUG(DAR_MSG_FULL, "flBuildList: adding INDEX to task list%c\n", ' ');

          for ( i = 0; i < (int)(sizeof( flIndexAll)/ sizeof( FL_TASK_LIST )); i++ )
          {
                newTask = malloc( sizeof ( FL_LIST ) );
                if ( newTask == NULL )
                {
                   FLDEBUG(DAR_MSG_FATAL,
                           "flBuildList: Memory allocation failure for index task %d\n", i );

                   status = DAR_E_MALLOC;
                   assAddErrorMessage( par, "Filter, insufficient memory for new task");
                   return (status);
                }
                newTask->item = (void *) &(flIndexAll[i]);
                ellAdd( (ELLLIST *) &(pFlPriv->taskList), &(newTask->node) );
          }
          break;

     case DAR_MODE_MOVE:

          /* Move mode for filters */

          FLDEBUG(DAR_MSG_FULL, "flBuildList: Adding move filters to task list%c\n", ' ');

          for ( i = 0; i < (int)(sizeof( flMoveFilter)/ sizeof( FL_TASK_LIST )); i++ )
          {
                newTask = malloc( sizeof ( FL_LIST ) );
                if ( newTask == NULL )
                {
                   FLDEBUG(DAR_MSG_FATAL,
                           "flBuildList: Memory allocation failure for move task %d\n", i );

                   status = DAR_E_MALLOC;
                   assAddErrorMessage( par, "Filter, insufficient memory for new task");
                   return (status);
                }
                newTask->item = (void *) &(flMoveFilter[i]);
                ellAdd( (ELLLIST *) &(pFlPriv->taskList), &(newTask->node) );
          }
          break;

     case DAR_MODE_UPDATE:

          /* Update filter characterisation */

          /* Reset the bad barcode flag. */

          semTake (pFlPriv->mutexSem, WAIT_FOREVER);
          pDevConfig->badBarcode = FALSE;
          semGive (pFlPriv->mutexSem);

          if (!pFlPriv->updateMode)
          {

              /*
               * Clear the internal list of barcodes and define the LUT index numbers
               */

              flClearConfig( par );

              semTake (pFlPriv->mutexSem, WAIT_FOREVER);
              for (i = 0; i< FL_MAX_FILTERS; i++)
              {
                   pDevConfig->lutIndex[i] = i;
              }
              semGive (pFlPriv->mutexSem);


               FLDEBUG(DAR_MSG_LOG, "Filter UPDATE requested%c\n", ' ');
               FLDEBUG(DAR_MSG_FULL, "flBuildList: Adding UPDATE to task list%c\n", ' ');

               for ( i = 0; i < (int)(sizeof( flCharacterise)/ sizeof( FL_TASK_LIST )); i++ )
               {
                    newTask = malloc( sizeof ( FL_LIST ) );
                    if ( newTask == NULL )
                    {
                       FLDEBUG(DAR_MSG_FATAL,
                               "flBuildList: Memory allocation failure for update task %d\n", i );

                       status = DAR_E_MALLOC;
                       assAddErrorMessage( par, "Filter, insufficient memory for new task");
                       return (status);
                    }
                    newTask->item = (void *) &(flCharacterise[i]);
                    ellAdd( (ELLLIST *) &(pFlPriv->taskList), &(newTask->node) );
               }
          }
          else
          {

               /*
                * Issue a confirmation message in case the operator thinks a full update mode
                * is happening.
                */

               FLDEBUG(DAR_MSG_LOG, "Filter QUICK UPDATE requested%c\n", ' ');
               FLDEBUG(DAR_MSG_FULL, "flBuildList: Adding QUICK UPDATE to task list%c\n", ' ');

               /*
                * Search the configuration database and attempt to reread the barcode
                * for any position that has a valid wheel and position number but an
                * invalid barcode, or is missing.
                */

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);

	       /* Step through each available filter position */

	       highwater = pDevConfig->highwater;

	       for (k = 0, i = 0; i < (int)(FL_MAX_FILTERS / 2); i++)
	       {

		    testPosition = (i + 6) % (int)(FL_MAX_FILTERS / 2);
		    readWheel1 = readWheel2 = TRUE;
		    foundEntry1 = foundEntry2 = -1;

		    /* Now look for wheel1 and wheel2 barcodes, for this position, in the database */

		    for (j = 0;
                         (j < pDevConfig->highwater) && ((foundEntry1 < 0) || (foundEntry2 < 0));
                         j++)
		    {
			 if (pDevConfig->wheel[j] == 1 && (foundEntry1 < 0))
			 {
			      sscanf (pDevConfig->pos[j], "pos%d", &wheelPosition);
			      if (wheelPosition == testPosition)
			      {
				   foundEntry1 = j;
				   if (pDevConfig->barcode[j] != FL_NO_BARCODE)
				   {
					readWheel1 = FALSE;
				   }
			      }
			 }

			 if (pDevConfig->wheel[j] == 2 && (foundEntry2 < 0))
			 {
			      sscanf (pDevConfig->pos[j], "pos%d", &wheelPosition);
			      if (wheelPosition == testPosition)
			      {
				   foundEntry2 = j;
				   if (pDevConfig->barcode[j] != FL_NO_BARCODE)
				   {
					readWheel2 = FALSE;
				   }
			      }
			 }

		    }
		    
		    /* If either barcode missing, add a move to that location */
		    
		    if (readWheel1 || readWheel2)
		    {
                         newTask = malloc(sizeof (FL_LIST));
                         if ( newTask == NULL )
                         {
                            FLDEBUG(DAR_MSG_FATAL,
                                    "flBuildList: Memory allocation failure for barcode task%c\n", ' ' );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Filter, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(flQuickCharacterise[testPosition + 3]);
                         ellAdd((ELLLIST *) &(pFlPriv->taskList), &(newTask->node));
		    }
		    
		    /* Then read the appropriate barcode(s) */

		    if (readWheel1)
		    {
                         newTask = malloc(sizeof (FL_LIST));
                         if ( newTask == NULL )
                         {
                            FLDEBUG(DAR_MSG_FATAL,
                                    "flBuildList: Memory allocation failure for barcode task%c\n", ' ' );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Filter, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(flQuickCharacterise[1]);
                         ellAdd((ELLLIST *) &(pFlPriv->taskList), &(newTask->node));

			 if (!(foundEntry1 < 0))
			 {
			      pDevConfig->lutIndex[k++] = foundEntry1;
			 }
			 else
			 {
			      pDevConfig->lutIndex[k++] = highwater++;
			 }
		    }

		    if (readWheel2)
		    {
                         newTask = malloc(sizeof (FL_LIST));
                         if ( newTask == NULL )
                         {
                            FLDEBUG(DAR_MSG_FATAL,
                                    "flBuildList: Memory allocation failure for barcode task%c\n", ' ' );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Filter, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(flQuickCharacterise[2]);
                         ellAdd((ELLLIST *) &(pFlPriv->taskList), &(newTask->node));

			 if (!(foundEntry2 < 0))
			 {
			      pDevConfig->lutIndex[k++] = foundEntry2;
			 }
			 else
			 {
			      pDevConfig->lutIndex[k++] = highwater++;
			 }
		    }
	       }	       

	       /* Finally, add the task to write the config (lookup table) file */

               newTask = malloc(sizeof (FL_LIST));
               if ( newTask == NULL )
               {
                  FLDEBUG(DAR_MSG_FATAL,
                          "flBuildList: Memory allocation failure for config task%c\n", ' ' );

                  status = DAR_E_MALLOC;
                  assAddErrorMessage( par, "Filter, insufficient memory for new task");
                  return (status);
               }
               newTask->item = (void *) &(flQuickCharacterise[0]);
               ellAdd((ELLLIST *) &(pFlPriv->taskList), &(newTask->node));

               semGive (pFlPriv->mutexSem);
          }
          break;

     case DAR_MODE_INIT:

          /*
           *  Build the task list to initialise all devices at the same time.
           *  (The following statements assume INIT only has one task on the list).
           */

          FLDEBUG(DAR_MSG_FULL, "flBuildList: adding INIT to task list%c\n", ' ');
          newTask = malloc( sizeof ( FL_LIST ) );
          if ( newTask == NULL )
          {
             FLDEBUG(DAR_MSG_FATAL,
                     "flBuildList: Memory allocation failure for init task%c\n", ' ' );

             status = DAR_E_MALLOC;
             assAddErrorMessage( par, "Filter, insufficient memory for new task");
             return (status);
          }
          newTask->item = (void *)&(flInit[0]);
          ellAdd( (ELLLIST *) &(pFlPriv->taskList), &(newTask->node) );

          break;

     case DAR_MODE_TEST:

          /*
           *  Build the task list to test all devices at the same time.
           *  (The following statements assume TEST only has one task on the list).
           */

          FLDEBUG(DAR_MSG_FULL, "flBuildList: adding TEST to task list%c\n", ' ');
          newTask = malloc( sizeof ( FL_LIST ) );
          if ( newTask == NULL )
          {
             FLDEBUG(DAR_MSG_FATAL,
                     "flBuildList: Memory allocation failure for test task%c\n", ' ' );

             status = DAR_E_MALLOC;
             assAddErrorMessage( par, "Filter, insufficient memory for new task");
             return (status);
          }
          newTask->item = (void *)&(flTest[0]);
          ellAdd( (ELLLIST *) &(pFlPriv->taskList), &(newTask->node) );

          break;

     case DAR_MODE_PARK:

          /*
           *  Build the task list to park all the devices at the same time.
           */

          FLDEBUG(DAR_MSG_FULL, "flBuildList: adding PARK to task list%c\n", ' ');
          for ( i = 0; i < (int)(sizeof( flParkAll)/ sizeof( FL_TASK_LIST )); i++ )
          {
               newTask = malloc( sizeof ( FL_LIST ) );
               if ( newTask == NULL )
               {
                  FLDEBUG(DAR_MSG_FATAL, "flBuildList: Memory allocation failure for park task %d\n", i );

                  status = DAR_E_MALLOC;
                  assAddErrorMessage( par, "Filter, insufficient memory for new task");
                  return (status);
               }
               newTask->item = (void *) &(flParkAll[i]);
               ellAdd( (ELLLIST *) &(pFlPriv->taskList), &(newTask->node) );
          }

          break;

     default:

          /*
           *  Error, nothing else supported.
           */

          FLDEBUG(DAR_MSG_ERROR, "flBuildList: invalid mode=%d\n", mode );

          status = DAR_E_MODE;
          assAddErrorMessage( par, "Filter, Mode not implemented");

     }/* End of switch on MODE */

     return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flCheckAttributes
 *
 * INVOCATION:
 * status = flCheckAttributes( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *          > par->a    string   filter 1 barcode
 *          > par->b    string   filter 2 barcode
 *          > par->c    long     park position
 *          > par->d    long     update mode
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * To check the attributes in the assemblyControl record
 *
 * DESCRIPTION:
 * This function checks that all the attributes supplied to the
 * assembly control record are sensible and responds with a SUCCESS
 * or REJECT. For the filter assembly this involves checking that
 * the filters requested are loaded.
 * Executed during a PRESET prior to a START.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long      flCheckAttributes
(
    ASSEMBLY_CONTROL_RECORD *par		/* (in)  Ptr to ass rec.	*/
)
{
     FL_DEV_PRIVATE *pFlPriv;			/* Ptr to private dev. struct.	*/
     FL_DEV_CONFIG  *pDevConfig;		/* Ptr to device config struct.	*/
     long        status = DAR_S_SUCCESS;	/* Returned function status.	*/
     int         i;
     int         matchA[2];
     int         matchB[2];
     char *      positionA[2];
     char *      positionB[2];
     long        barcodeIdA;
     long        barcodeIdB;
     char dumpString[MAX_STRING_SIZE];

     FLDEBUG(DAR_MSG_MAX, "flCheckAttributes: entry%c\n", ' ');

     /*
      * Obtain the device private structure and device configuration structure pointers.
      */

     pFlPriv =  ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pFlPriv->pFilterPriv;

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL || pDevConfig == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flCheckAttributes: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC || pDevConfig->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flCheckAttributes: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Assembly must be idle before accepting new commands.
     */

    if ( par->busy == DAR_BUSY_BUSY)
    {
        FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: filter assembly is busy%c\n",  ' ');

        status = FL_TASK_LIST_NOT_EMPTY;
        assAddErrorMessage( par, "Error, filter assembly is BUSY");
        return status;
    }

     /*
      * The attributes needing to be checked depends on the mode.
      */

     if ( par->mode == DAR_MODE_MOVE )
     {

          /*
           *  In MOVE mode, return an error if attributes A and B are not both of STRING type,
           *  or the device has not been initialized or characterized. The barcode IDs given
           *  should both be contained in the LUT database and represent filters on different wheels.
           */

         if ( par->fta != DBF_STRING )
         {
              FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: Attr A is not a STRING%c\n", ' ');

              status = DAR_E_ATT;
              assAddErrorMessage( par, "Filter, Attr A is not STRING");
         }
         else if ( par->ftb != DBF_STRING )
         {
              FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: Attr b is not a STRING%c\n", ' ');

              status = DAR_E_ATT;
              assAddErrorMessage( par, "Filter, Attr B is not STRING");
         }
         else if ( pDevConfig->highwater <= 0 || pDevConfig->highwater > FL_MAX_FILTERS )
         {
              FLDEBUG(DAR_MSG_ERROR,
                   "flCheckAttributes: %d filters defined - INIT or UPDATE needed\n",
                   pDevConfig->highwater);

              status = FL_EMPTY_BARCODE_LIST;
              assAddErrorMessage( par, "Filter, INIT or UPDATE needed");
         }
         else
         {

              /*
               * Give a warning if the device has not been characterized
               * (i.e. there is no barcode database).
               */

              if ( pDevConfig->characterized == FALSE )
              {
                  FLDEBUG(DAR_MSG_WARNING,
                      "flCheckAttributes: Device not characterized - using existing database%c\n", ' ');
              }

              /*
               * Read in and check the first filter barcode. Allow for real barcode values and
               * the values generated in simulation mode.
               */

              if ( *(char *)par->a == '\0' )
              {
                  barcodeIdA = FL_NO_BARCODE;
              }
              else
              {
                  if (sscanf (par->a, "%ld%s", &barcodeIdA, dumpString) != 1)
                  {
                      status = DAR_E_ATT;
                      assAddErrorMessage (par, "Filter failed attr A conversion");
                      return(status);
                  }
              }

              if ( (barcodeIdA < FL_BARCODE_MIN || barcodeIdA > FL_BARCODE_MAX) &&
                   (barcodeIdA < FL_BARCODE_MIN_SIM || barcodeIdA > FL_BARCODE_MAX_SIM)
                 )
              {
                  status = DAR_E_BAD_ATTR;
                  assAddErrorMessage(par, "Filter 1 barcode out of range");
                  return(status);
              }


              /*
               * Check if the barcodes are present in the device configuration 
               * structure and determine the wheel number any matches. Set the 
               * position accordingly.
               */

              matchA[0] = matchA[1] = 0;
              for ( i=0; i < pDevConfig->highwater; i++ )
              {
                  if ( pDevConfig->barcode[i] == barcodeIdA )
                  {
                      matchA[pDevConfig->wheel[i]-1] = 1;
                      positionA[pDevConfig->wheel[i]-1] = pDevConfig->pos[i];
                  }
              }

              /*
               * Read in and check the second filter barcode. Allow for real barcode values and
               * the values generated in simulation mode.
               */

              if ( *(char *)par->b == '\0' )
              {
                  barcodeIdB = FL_NO_BARCODE;
              }
              else
              {
                  if (sscanf (par->b, "%ld%s", &barcodeIdB, dumpString) != 1)
                  {

                      status = DAR_E_ATT;
                      assAddErrorMessage (par, "Filter failed attr B conversion");
                      return(status);
                  }
              }
    
              if ( (barcodeIdB < FL_BARCODE_MIN || barcodeIdB > FL_BARCODE_MAX) &&
                   (barcodeIdB < FL_BARCODE_MIN_SIM || barcodeIdB > FL_BARCODE_MAX_SIM)
                 )
              {
                  status = FL_INVALID_BARCODE;
                  assAddErrorMessage(par, "Filter 2 barcode out of range");
                  return(status);
              }

              matchB[0] = matchB[1] = 0;
              for ( i=0; i < pDevConfig->highwater; i++ )
              {
                  if ( pDevConfig->barcode[i] == barcodeIdB )
                  {
                      matchB[pDevConfig->wheel[i]-1] = 1;
                      positionB[pDevConfig->wheel[i]-1] = pDevConfig->pos[i];
                  }
              }

              if ( matchA[0] == 0 && matchA[1] == 0 )
              {
                  FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: Filter A (%s) not found in database\n",
                     (char *) par->a);

                  status = FL_UNKNOWN_FILTER;
                  assAddErrorMessage( par, "Requested filter A not installed");
              }
              else if ( matchB[0] == 0 && matchB[1] == 0 )
              {
                  FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: Filter B (%s) not found in database\n",
                     (char *) par->b);

                  status = FL_UNKNOWN_FILTER;
                  assAddErrorMessage( par, "Requested filter B not installed");
              }

              /*
               * Check the wheel numbers are consistent.
               */

              else if ( ( matchA[0] == 0 && matchB[0] == 0 ) || 
                      ( matchA[1] == 0 && matchB[1] == 0 ) )
              {
                    FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: A and B on same wheel%c\n", ' ' );

                    status = FL_ILLEGAL;
                    assAddErrorMessage( par, "Filters A and B on same wheel");
              }
              else if ( matchA[0] && matchB[1] )
              {

                   /*
                    *  Set the positions and velocity and save the barcode ids.
                    */

                   semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                   strncpy( pFlPriv->pos[0], positionA[0], MAX_STRING_SIZE - 1 );
                   pFlPriv->barcode[0] = barcodeIdA;
                   strncpy( pFlPriv->pos[1], positionB[1], MAX_STRING_SIZE - 1 );
                   pFlPriv->barcode[1] = barcodeIdB;
                   pFlPriv->velocity    = FL_VELOCITY;
                   semGive (pFlPriv->mutexSem);
              }
              else
              {
                   semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                   strncpy( pFlPriv->pos[0], positionB[0], MAX_STRING_SIZE - 1 );
                   pFlPriv->barcode[0] = barcodeIdB;
                   strncpy( pFlPriv->pos[1], positionA[1], MAX_STRING_SIZE - 1 );
                   pFlPriv->barcode[1] = barcodeIdA;
                   pFlPriv->velocity    = FL_VELOCITY;
                   semGive (pFlPriv->mutexSem);
              }
         }
     }
     else if ( par->mode == DAR_MODE_PARK )
     {
         /*
          *  In PARK mode return an error if the parking position attribute (C) is not LONG
          *  or is out of range.
          */

         if ( par->ftc != DBF_LONG )
         {
              FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: Attr C is not a LONG%c\n", ' ');

              status = DAR_E_ATT;
              assAddErrorMessage( par, "Filter, Attr C is not LONG");
         }
         else if ( *(long *)par->c < 0 || *(long *)par->c > 11 )
         {
              FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: Park position %ld out of range\n",
                  *(long *)par->c);

              status = DAR_E_BAD_ATTR;
              assAddErrorMessage( par, "Filter park position out of range");
         }

         semTake (pFlPriv->mutexSem, WAIT_FOREVER);
         pFlPriv->parkPosition = (int) *(long *)par->c;
         pFlPriv->velocity     = FL_VELOCITY;
         semGive (pFlPriv->mutexSem);
     }
     else if ( par->mode == DAR_MODE_TRACK )
     {
         /*
          * TRACK mode is not appropriate for the filter assembly.
          */

         FLDEBUG(DAR_MSG_ERROR,
              "flCheckAttributes: TRACK mode not appropriate for filter assembly%c\n", ' ');

         status = DAR_E_MODE;
         assAddErrorMessage( par, "Filter, TRACK mode not appropriate");
     }
     else if ( par->mode == DAR_MODE_UPDATE )
     {
         /*
          * UPDATE just set velocity - position will be set later,
          * Then check that the update mode is valid.
          */

         semTake (pFlPriv->mutexSem, WAIT_FOREVER);

         pFlPriv->velocity = FL_VELOCITY;

         if ( par->ftd != DBF_LONG )
         {
              FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: Attr D is not a LONG%c\n", ' ');

              status = DAR_E_ATT;
              assAddErrorMessage( par, "Filter, Attr D is not LONG");
         }
         else if (*(long *)par->d != 0 && *(long *)par->d != 1)
         {
             FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: UPDATE mode (%ld) must be 0 or 1\n",
                *(long *)par->d);

             status = DAR_E_BAD_ATTR;
             assAddErrorMessage( par, "Invalid UPDATE mode");
         }

         /*
          * QUICK UPDATE mode is only appropriate if the assembly has already been
          * characterized in FULL UPDATE mode. Otherwise assume FULL UPDATE mode
          * and issue a warning. 
          */

         if ( (*(long *)par->d == 1) && !pDevConfig->characterized )
         {
             FLDEBUG(DAR_MSG_WARNING,
               "flCheckAttributes: QUICK UPDATE only valid when characterized - using FULL UPDATE%c\n",
               ' ');
	     *(long *)par->d = 0;
         }

	 pFlPriv->updateMode = (int) *(long *)par->d;

	 /* Quick update resets to full after execution */
	 
	 *(long *)par->d = 0;
	 db_post_events(par, par->d, DBE_VALUE);

         semGive (pFlPriv->mutexSem);

     }
     else
     {
         /*
          * In other modes set the positions to zero and the velocity to a suitable value.
          */

         semTake (pFlPriv->mutexSem, WAIT_FOREVER);
         strcpy( pFlPriv->pos[0], "0.0");
         strcpy( pFlPriv->pos[1], "0.0");
         pFlPriv->velocity    = FL_VELOCITY;
         semGive (pFlPriv->mutexSem);
     }

     /*
      * In all modes determine whether the in position switches are to be used
      */

     if ( status == DAR_S_SUCCESS )
     {
          if ( par->fte != DBF_LONG )
          {
               FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: Attr E is not a LONG%c\n", ' ');

               status = DAR_E_ATT;
               assAddErrorMessage( par, "Filter, Attr E is not LONG");
          }
          else if (*(long *)par->e != 0 && *(long *)par->e != 1)
          {
              FLDEBUG(DAR_MSG_ERROR, "flCheckAttributes: Switches flag (%ld) must be 0 or 1\n",
                 *(long *)par->e);

              status = DAR_E_BAD_ATTR;
              assAddErrorMessage( par, "Filter, invalid switches flag");
          } 

          semTake (pFlPriv->mutexSem, WAIT_FOREVER);
	  pFlPriv->ignoreSwitches = (int) *(long *)par->e;
          semGive (pFlPriv->mutexSem);
     }

     return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flCheckBarcodeId
 *
 * INVOCATION:
 * status = flCheckBarcodeId( par, wheel );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par	(ASSEMBLY_CONTROL_RECORD *)	Pointer to assemblyControl record
 *						structure.
 * (>) wheel	(const int)			Filter wheel number
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * To check the returned barcode id and update the barcode/filter position
 * table.
 *
 * DESCRIPTION:
 * To check the returned barcode id.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * Assumes the barcode reader has finished getting the barcode id.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flCheckBarcodeId(ASSEMBLY_CONTROL_RECORD *par, const int wheel)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt;	/* Private rec. support struct.			*/
    FL_DEV_PRIVATE      *pFlPriv;		/* Ptr to private dev. struct.			*/
    FL_DEV_CONFIG       *pDevConfig;		/* Ptr to device config struct.			*/
    long        status = DAR_S_SUCCESS;		/* Function status.				*/
    long        value;				/* Barcode value.				*/
    char        *end;				/* End string pointer.				*/
    long        lutIndex;			/* Where we're going to store the barcode	*/

    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
    pDevConfig = pFlPriv->pFilterPriv;

    FLDEBUG(DAR_MSG_MAX, "flCheckBarcodeId: entry%c\n",  ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL || pDevConfig == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flCheckBarcodeId: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC || pDevConfig->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flCheckBarcodeId: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     * Get the next available barcode index.
     */

    lutIndex = pFlPriv->whichLutIndex;


    /*
     * Decode the string obtained from the barcode reader into a barcode.
     * The reader responds with "NR" if the barcode could not be read.
     */

    FLDEBUG(DAR_MSG_MIN, "flCheckBarcodeId: Checking the barcode ID.%c\n", ' ');

    switch ( wheel )
    {
         case 1:
              if ( strncmp( FILT1_BARCODE_READ_FIELD, "NR", 2 ) != 0 )
              {
                   value = strtol(FILT1_BARCODE_READ_FIELD, &end, 10 );
          
                   if ( *end != '\0' )
                   {

                       /* The barcode read failed and generated a non-numeric string */

                       FLDEBUG(DAR_MSG_ERROR, "flCheckBarcodeId: Invalid wheel 1 barcode=%s\n",
                         FILT1_BARCODE_READ_FIELD );

                       /* A barcode read error should not affect the assembly index state */

                       pPriv->keepIndex = TRUE;

                       semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                       pDevConfig->badBarcode = TRUE;
                       semGive (pFlPriv->mutexSem);

                       status = FL_INVALID_BARCODE;
                   }
                   else if ( (value < FL_BARCODE_MIN || value > FL_BARCODE_MAX) &&
                             (value < FL_BARCODE_MIN_SIM || value > FL_BARCODE_MAX_SIM)
                           )
                   {
                       /* An out of range barcode is stored as a null value */

                       FLDEBUG(DAR_MSG_WARNING,
                          "flCheckBarcodeId: Barcode for wheel 1 at position %ld out of range\n",
                          lutIndex );
                       semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                       pDevConfig->badBarcode = TRUE;
                       semGive (pFlPriv->mutexSem);
                       value = FL_NO_BARCODE;
                   }
              }
              else
              {
                   /* An unreadable barcode is stored as a null value */

                   FLDEBUG(DAR_MSG_WARNING,
                      "flCheckBarcodeId: Unreadable barcode for wheel 1 at position %ld\n",
                      lutIndex );
                   semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                   pDevConfig->badBarcode = TRUE;
                   semGive (pFlPriv->mutexSem);
                   value = FL_NO_BARCODE;
              }
              break;

         case 2:
              if ( strncmp( FILT2_BARCODE_READ_FIELD, "NR", 2 ) != 0 )
              {
                   value = strtol(FILT2_BARCODE_READ_FIELD, &end, 10 );
          
                   if ( *end != '\0' )
                   {
                       /* The barcode read failed and generated a non-numeric string */

                       FLDEBUG(DAR_MSG_ERROR, "flCheckBarcodeId: Invalid wheel 2 barcode=%s\n",
                         FILT2_BARCODE_READ_FIELD );

                       /* A barcode read error should not affect the assembly index state */
                       pPriv->keepIndex = TRUE;

                       semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                       pDevConfig->badBarcode = TRUE;
                       semGive (pFlPriv->mutexSem);
                       status = FL_INVALID_BARCODE;
                   }
                   else if ( (value < FL_BARCODE_MIN || value > FL_BARCODE_MAX) &&
                             (value < FL_BARCODE_MIN_SIM || value > FL_BARCODE_MAX_SIM)
                           )
                   {
                       /* An out of range barcode is stored as a null value */

                       FLDEBUG(DAR_MSG_WARNING,
                          "flCheckBarcodeId: Barcode for wheel 2 at position %ld out of range\n",
                          lutIndex );
                       semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                       pDevConfig->badBarcode = TRUE;
                       semGive (pFlPriv->mutexSem);
                       value = FL_NO_BARCODE;
                   }
              }
              else
              {
                   /* An unreadable barcode is stored as a null value */

                   FLDEBUG(DAR_MSG_WARNING,
                      "flCheckBarcodeId: Unreadable barcode for wheel 2 at position %ld\n",
                      lutIndex );
                   semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                   pDevConfig->badBarcode = TRUE;
                   semGive (pFlPriv->mutexSem);
                   value = FL_NO_BARCODE;
              }
              break;
    }
    
    if (status == DAR_S_SUCCESS)
    {
         /*
          * Store away the barcode for this filter position, using a semaphore to
          * prevent simultaneous access.
          */
                  
         semTake (pFlPriv->mutexSem, WAIT_FOREVER);

         pDevConfig->barcode[pDevConfig->lutIndex[lutIndex]] = value;
         pDevConfig->wheel[pDevConfig->lutIndex[lutIndex]]   = wheel;

         *(pDevConfig->pos[pDevConfig->lutIndex[lutIndex]])  = '\0';
         strncat(pDevConfig->pos[pDevConfig->lutIndex[lutIndex]],
                 pFlPriv->position[wheel-1],
                 strlen(pFlPriv->position[wheel-1]) - 7);

         /*
          * Move the high water mark if the index is the largest found so far.
          */

         if ( (pDevConfig->lutIndex[lutIndex]+1) > pDevConfig->highwater )
         {
            pDevConfig->highwater = pDevConfig->lutIndex[lutIndex] + 1;
         }

         /* The database must now contain at least one entry. */
         pDevConfig->characterized=TRUE;

         semGive (pFlPriv->mutexSem);
    }

    /* Increment the table index */

    semTake (pFlPriv->mutexSem, WAIT_FOREVER);
    pFlPriv->whichLutIndex++;
    semGive (pFlPriv->mutexSem);

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flCheckBus
 *
 * INVOCATION:
 * status = flCheckBus( par, devnum, busVal );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 * (>) devnum (int)  Device number
 * (>) busVal (int)  Value of the changed busy (bus) field.
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
 *	Stop the timer, set an error message, empty the task list
 *	Set the cmdFinished flag to TRUE and cmdActive flag to FALSE.
 *    else if BUSY then
 *	Just ignore this change. The device hasn't finished yet.
 *    else if IDLE then
 *	That means something completed on that device so:
 *	a task has finished, call flTaskFinished()
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flCheckBus(ASSEMBLY_CONTROL_RECORD *par, const int devnum, int busVal)
{
    FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/

    long status = DAR_S_SUCCESS;	/* Function status.		*/
    char errMsg[FL_ERROR_MESSAGE_SIZE]; /* Error message.		*/

    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

    FLDEBUG(DAR_MSG_MAX, "flCheckBus: entry, bus=%d\n", busVal);

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flCheckBus: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flCheckBus: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
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

            FLDEBUG(DAR_MSG_FULL, "flCheckBus, BUSY to ERROR transition detected%c\n", ' ');

            status = DAR_E_DEVICE;
            sprintf (errMsg, "Filter wheel %d failed", devnum );
            flTerminateTasks( par, status, errMsg);
            break;

        case DAR_DEV_BUSY_BUSY:
            /*
             *  Ignore the busy change.
             */

            FLDEBUG(DAR_MSG_FULL, "flCheckBus, BUSY to BUSY transition detected - ignored%c\n",
               ' ');
            break;

        case DAR_DEV_BUSY_IDLE:
            /*
             *  Check to see if all devices are finished, 
             *  and if so then the command is finished, so stop the timer
             *  set the bus flags to false.
             */

            FLDEBUG(DAR_MSG_FULL, "flCheckBus, BUSY to IDLE transition detected%c\n",' ');
            status = flTaskFinished( par );
            break;

        default:
            FLDEBUG(DAR_MSG_ERROR, "flCheckBus, invalid device acknowledgement: %d\n",
                busVal );

            status = DAR_E_DEVICE;
            flTerminateTasks( par, status, "Filter, invalid device busy field");
            break;
    }
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flDoTask
 *
 * INVOCATION:
 * status = flDoTask (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Do the next task on the list
 *
 * DESCRIPTION:
 * This function executes the next task on the task list put together
 * by flBuildList, as follows:
 *    Make sure list is not empty and point to first one in list.
 *    Clear the receipt flags and make sure the timer is stopped.
 *    Do the Task requested by setting variables in the private
 *    structure: velocity, position, devicePointer, mode.
 *    If getting the task worked ok, then call flExecuteTask()
 *    to execute the command on the device(s).
 *    Start the timer.
 *    If anything fails here, then stop the timer, set the cmdActive
 *    flag to false, make sure the task list is empty and tell the
 *    record layer that the command is complete.
 *
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flDoTask(ASSEMBLY_CONTROL_RECORD *par)
{
     int            doAgain;		/* Redo flag.			*/
     char           errMsg[FL_ERROR_MESSAGE_SIZE];/* Error message.	*/
     long           nRequest = 1;	/* Number requested.		*/
     FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/
     FL_DEV_CONFIG  *pDevConfig;
     long status = DAR_S_SUCCESS;	/* Return function status.	*/
     FL_LIST        *taskRequested = NULL;	/* Ptr to task requested. */
     FL_TASK_LIST   *taskItem = NULL;	/* Ptr to item in task list.	*/
     long timerStatus = DAR_S_SUCCESS;  /* status word from assStopTimer  */
     int dbPutRet;                      /* debug status for dbPuts*/


     pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pFlPriv->pFilterPriv;

     FLDEBUG(DAR_MSG_MAX, "flDoTask: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL || pDevConfig == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flDoTask: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC || pDevConfig->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flDoTask: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

     /*
      *  Make sure list is not empty and point to first one in list.
      */

     if ( ellCount( &(pFlPriv->taskList) ) == 0 || 
          ( taskRequested = (FL_LIST *) ellFirst( &(pFlPriv->taskList) )) == NULL )
     {
          /*
           *  If there are no tasks then the filter assembly code has become confused.
           *  Abort the command.
           */

          FLDEBUG(DAR_MSG_ERROR, "flDoTask: taskList empty, ellCount=%d\n",
              ellCount( &(pFlPriv->taskList) ) );

          status = FL_LAST_TASK;
          flTerminateTasks( par, status, "Filter assembly confused, no tasks to do" );
          return ( status );
     }

     taskItem = ( FL_TASK_LIST *) taskRequested->item;

     FLDEBUG(DAR_MSG_MAX, "flDoTask: taskList size =%d\n", ellCount( &(pFlPriv->taskList) ) );
     FLDEBUG(DAR_MSG_MAX, "flDoTask: firsttask = %d\n", taskItem->task );

     /*
      *  Clear the receipt flags and make sure the timer is stopped.
      */

     semTake (pFlPriv->mutexSem, WAIT_FOREVER);
     pFlPriv->fl1Finished = FALSE; 
     pFlPriv->fl2Finished = FALSE; 
     semGive (pFlPriv->mutexSem);

     assStopTimer( par, &timerStatus );
     if (timerStatus != DAR_S_SUCCESS)
     {
         FLDEBUG(DAR_MSG_WARNING, "flDoTask: failed to stop watchdog timer. status=%ld\n",
            timerStatus);
     }
     status = DAR_S_SUCCESS;

     /*
      * Set default velocity.
      */

     semTake (pFlPriv->mutexSem, WAIT_FOREVER);
     pFlPriv->velocity = FL_VELOCITY;
     semGive (pFlPriv->mutexSem);

     /*
      *  Do the Task requested.
      */
 
     doAgain = 1;
     while ( doAgain )
     {
          doAgain = 0;
          switch( taskItem->task )
          {

          case FL_INDEX_ALL:
          
               FLDEBUG(DAR_MSG_MIN, "flDoTask: Index all devices%c\n", ' ');

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);
               (void) strncpy( pFlPriv->position[FLT1], FL_NAME_POS_INDEX, MAX_STRING_SIZE-1 );
               (void) strncpy( pFlPriv->position[FLT2], FL_NAME_POS_INDEX, MAX_STRING_SIZE-1 );
               pFlPriv->mode = DAR_MODE_INDEX;
               semGive (pFlPriv->mutexSem);
               break;

          case FL_INIT_ALL:
                 
               FLDEBUG(DAR_MSG_MIN, "flDoTask: Initialize all devices%c\n", ' ');

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);
               (void) strcpy( pFlPriv->position[FLT1], " " );
               (void) strcpy( pFlPriv->position[FLT2], " " );
               pFlPriv->mode = DAR_MODE_INIT;
               semGive (pFlPriv->mutexSem);
               break;

          case FL_TEST_ALL:
                 
               FLDEBUG(DAR_MSG_MIN, "flDoTask: Test all devices%c\n", ' ');

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);
               (void) strcpy( pFlPriv->position[FLT1], " " );
               (void) strcpy( pFlPriv->position[FLT2], " " );
               pFlPriv->mode = DAR_MODE_TEST;
               semGive (pFlPriv->mutexSem);
               break;

          case FL_READ_BARCODE_1:
                 
               FLDEBUG(DAR_MSG_MIN, "flDoTask: Read filter 1 barcode ID.%c\n", ' ');

               strncpy( FILT1_BARCODE_READ_FIELD, "WAITING", MAX_STRING_SIZE-1);
               /*if ( recGblPutLinkValue (&(FILT1_BARCODE_TRIGGER), (void *) par, DBR_LONG,
                      &nRequest, &nRequest) != DAR_S_SUCCESS )*/
                dbPutRet = dbPutLink(&(FILT1_BARCODE_TRIGGER), DBR_LONG, &nRequest, 0);
                if (dbPutRet != DAR_S_SUCCESS)
               {
                    printf("---------> dbPutRet = %d\n",dbPutRet);
                    status = FL_TRIG_FAILED;
                    strncpy( errMsg, "Trigger filter 1 barcode failed", MAX_STRING_SIZE-1);
               }
               else
               {
                    FLDEBUG(DAR_MSG_FULL, "flDoTask: trigger filter 1 barcode status OK%c\n", ' ');
               }
               break;

          case FL_READ_BARCODE_2:
                 
               FLDEBUG(DAR_MSG_MIN, "flDoTask: Read filter 2 barcode ID.%c\n", ' ');

               strncpy( FILT2_BARCODE_READ_FIELD, "WAITING", MAX_STRING_SIZE-1);
               /*if ( recGblPutLinkValue (&(FILT2_BARCODE_TRIGGER), (void *) par, DBR_LONG,
                      &nRequest, &nRequest) != DAR_S_SUCCESS )*/
                if (dbPutLink(&(FILT2_BARCODE_TRIGGER), DBR_LONG,&nRequest,nRequest)!=DAR_S_SUCCESS)
               {
                    status = FL_TRIG_FAILED;
                    strncpy( errMsg, "Trigger filter 2 barcode failed", MAX_STRING_SIZE-1);
               }
               else
               {
                    FLDEBUG(DAR_MSG_FULL, "flDoTask: trigger filter 2 barcode status OK%c\n", ' ');
               }
               break;

          case FL_WRITE_CONFIG:

               /*
                * Display the current filter barcode configuration.
                */

               flShowConfig( par );

               /*
                * Write the current filter barcode configuration to the specified file
                */

               if ((status = flWriteConfig(par)) != DAR_S_SUCCESS)
               {
                    strncpy (errMsg, "Failed to write config file", MAX_STRING_SIZE-1);
               }
               break;

          case FL_MV_FILTER1:

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);
               strncpy( pFlPriv->position[FLT1], pFlPriv->pos[0], MAX_STRING_SIZE-1);
               pFlPriv->mode = DAR_MODE_MOVE;
               semGive (pFlPriv->mutexSem);
               break;

          case FL_MV_FILTER2:

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);
               strncpy( pFlPriv->position[FLT2], pFlPriv->pos[1], MAX_STRING_SIZE-1);
               pFlPriv->mode = DAR_MODE_MOVE;
               semGive (pFlPriv->mutexSem);
               break;

          case FL_MV_FILTERS:

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);
               strncpy( pFlPriv->position[FLT1], pFlPriv->pos[0], MAX_STRING_SIZE-1);
               strncpy( pFlPriv->position[FLT2], pFlPriv->pos[1], MAX_STRING_SIZE-1);
               pFlPriv->mode = DAR_MODE_MOVE;
               semGive (pFlPriv->mutexSem);
               break;
               
          case FL_PARK_ALL_DEFAULT:

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);
               (void) strncpy( pFlPriv->position[FLT1], FL_NAME_POS_PARK, MAX_STRING_SIZE-1 );
               (void) strncpy( pFlPriv->position[FLT2], FL_NAME_POS_PARK, MAX_STRING_SIZE-1 );
               pFlPriv->mode = DAR_MODE_MOVE;
               semGive (pFlPriv->mutexSem);
               break;

          case FL_PARK_ALL_LOAD:

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);
               sprintf( pFlPriv->position[FLT1], "pos%dLoad", pFlPriv->parkPosition );
               sprintf( pFlPriv->position[FLT2], "pos%dLoad", pFlPriv->parkPosition );
               pFlPriv->mode = DAR_MODE_MOVE;
               semGive (pFlPriv->mutexSem);
               break;

          case FL_MV_FILT_BARCODE:
          case FL_MV_FILT_BARCODE + 1:
          case FL_MV_FILT_BARCODE + 2:
          case FL_MV_FILT_BARCODE + 3:
          case FL_MV_FILT_BARCODE + 4:
          case FL_MV_FILT_BARCODE + 5:
          case FL_MV_FILT_BARCODE + 6:
          case FL_MV_FILT_BARCODE + 7:
          case FL_MV_FILT_BARCODE + 8:
          case FL_MV_FILT_BARCODE + 9:
          case FL_MV_FILT_BARCODE + 10:
          case FL_MV_FILT_BARCODE + 11:

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);
               sprintf( pFlPriv->position[FLT1], "pos%dBarcode", taskItem->task - FL_MV_FILT_BARCODE );
               sprintf( pFlPriv->position[FLT2], "pos%dBarcode", taskItem->task - FL_MV_FILT_BARCODE );
               pFlPriv->velocity = FL_VELOCITY;
               pFlPriv->mode = DAR_MODE_MOVE;
               semGive (pFlPriv->mutexSem);
               break;

          default:
               FLDEBUG(DAR_MSG_ERROR, "flDoTask: Invalid task=%d\n", taskItem->task );

               status = FL_INVALID_TASK;
               strncpy( errMsg, "Filter, invalid task number", MAX_STRING_SIZE-1 );

          }
     }

     semTake (pFlPriv->mutexSem, WAIT_FOREVER);
     pFlPriv->askBarCode = FALSE;
     semGive (pFlPriv->mutexSem);

     if ( status == DAR_S_SUCCESS )
     {
          /*
           * If we are waiting for a barcode read then set a timer to check the barcode
           * after a short delay.
           */

          if ( taskItem->task == FL_READ_BARCODE_1 || taskItem->task == FL_READ_BARCODE_2 )
          {
               FLDEBUG(DAR_MSG_FULL, "flDoTask: Requested a barcode%c\n", ' ');

               semTake (pFlPriv->mutexSem, WAIT_FOREVER);
               pFlPriv->cmdActive = TRUE;
               pFlPriv->askBarCode = TRUE;
               semGive (pFlPriv->mutexSem);

               assStartTimer( par, &status, FL_BARCODE_READ_DELAY );
               if (status != DAR_S_SUCCESS)
               {
                   FLDEBUG(DAR_MSG_WARNING,
                      "flDoTask: failed to start watchdog timer for barcode read. status=%ld\n",
                      status);
               }
          }

          /*
           * FL_WRITE_CONFIG is always the last task of an UPDATE.
           */

          else if ( taskItem->task == FL_WRITE_CONFIG )
          {
               if ( flEmptyList( par, FALSE ) == FL_EMPTY_LIST )
               {
                    FLDEBUG(DAR_MSG_FULL, "flDoTask, all tasks done%c\n", ' ');

                    /*
                     * The command is finished. It is assumed the assembly record will
                     * keep the informational messages written to its message field.
                     * The assembly record private structure is fudged to make sure the
                     * message is not regarded as an error..
                     */

                    flTerminateTasks( par, status, par->mess );
               }
          }

          /*
           * Otherwise start the next task in the list.
           */

          else
          {
               if ( (status = flExecuteTask( par, taskItem))  == DAR_S_SUCCESS )
               {
                    FLDEBUG(DAR_MSG_FULL, "flDoTask: Started task =%d\n", taskItem->task );

                    assStartTimer( par, &status, FL_COMMAND_TIMEOUT );
                    if (status != DAR_S_SUCCESS)
                    {
                        FLDEBUG(DAR_MSG_WARNING,
                           "flDoTask: failed to start watchdog timer after flExecuteTask. status=%ld\n",
                           status);
                    }
               }
          }
     }
     else
     {
          /*
           *  This means the task failed.  To be safe, stop the timer, and
           *  make the cmd no longer active, empty the task list.
           */

          flTerminateTasks( par, status, errMsg );
     }

     return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flEmptyList
 *
 * INVOCATION:
 * status = flEmptyList (par, TRUE|FALSE); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 * (>) removeAll  (const int )		Remove all tasks from the list, or only 1.
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Empty the task list
 *
 * DESCRIPTION:
 *    Just return if list is empty.
 *    Point to beginning of list.
 *    while list is not empty
 *	delete the task from the list
 *	point to the next one.
 *	Stop if the list is empty
 *	Stop if we were only to delete one
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flEmptyList(ASSEMBLY_CONTROL_RECORD *par, const int removeAll)
{
    long         status = DAR_S_SUCCESS;	/* Return function status.	*/
    FL_DEV_PRIVATE *pFlPriv;			/* Ptr to private dev. struct.	*/
    FL_LIST        *pList;			/* Ptr to item in a list.	*/

    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

    FLDEBUG(DAR_MSG_MAX, "flEmptyList: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flEmptyList: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flEmptyList: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Just return if the list is already empty.
     */

    if ( ellCount( &(pFlPriv->taskList) ) == 0 )
    {
        FLDEBUG(DAR_MSG_MIN, "flEmptyList: task list is already empty, ellCount=%d\n", 
                    ellCount( &(pFlPriv->taskList) ) );

        status = FL_EMPTY_LIST;
        return ( status );
    }

    if ( !removeAll )
    {
        /* 
         *  Delete only the first one.
         */

        pList = (FL_LIST *) ellGet( &(pFlPriv->taskList) ); 
        pList->item = NULL;
        free( pList );
    }
    else
    {
        /* 
         *  Delete all items in the list.
         */

        while ( 1 )
        {
            if (  ( pList = (FL_LIST *) ellGet( &pFlPriv->taskList ) ) == NULL )
            {
                break;
            }
            pList->item = NULL;
            free( pList );
        }

        if ( ellCount( &(pFlPriv->taskList) ) != 0 )
        {
            FLDEBUG(DAR_MSG_WARNING, "flEmptyList: task list did not empty=%d\n",
                    ellCount( &(pFlPriv->taskList) ) );
        }
    }

    if ( ellCount( &(pFlPriv->taskList) ) == 0 )
    {
        status = FL_EMPTY_LIST;
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flExecuteTask
 *
 * INVOCATION:
 * status = flExecuteTask (par, taskItem); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 * (>) taskItem (FL_TASK_LIST *) Pointer to task list
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute a task on a device
 *
 * DESCRIPTION:
 * This function is called by flDoTask when the time comes to trigger
 * a device. It does the following:
 *    Initialize variables.
 *    Cycle through output links,
 *    they should be sequential, otherwise for only the link passed in.
 *	 Only do if the position link is connected.
 *	     Write the Position to the VALS, a string.
 *	     Write the Velocity.
 *	     Write the mode.
 *	     Write the directive.
 *	     Check the response from the device.
 *	     If it failed, then 
 *	     	set the cmdActive flag to FALSE and set an error message
 *	     otherwise
 *	     	set the cmdActive flag.
 * 
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flExecuteTask(ASSEMBLY_CONTROL_RECORD *par, FL_TASK_LIST *taskItem)
{
    unsigned short      dir;			/* Directive value.		*/
    unsigned short      mode;			/* Mode value.			*/
    double              vel;			/* Velocity.			*/
    char                errMsg[FL_ERROR_MESSAGE_SIZE];/* Error message.		*/
    long                nRequest = 1;		/* Number to send on link.	*/
    unsigned short      returnedAck;		/* Returned ack from device.	*/
    long                status = DAR_S_SUCCESS;	/* Function status.		*/
    int                 i;			/* Counter.			*/
    int                 useDevice[FL_NUM_DEVICES]; /* Use device flags		*/

    FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/
    struct link *dirLink;		/* Dir's links.			*/
    struct link *modLink;		/* Mod's links.			*/
    struct link *posLink;		/* Pos's links.			*/
    struct link *velLink;		/* Vel's links.			*/
    struct link *ackLink;		/* Acks's links.		*/

    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flExecuteTask: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flExecuteTask: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Set up variables.
     */

    dir  = flTranslateDir( DAR_DIR_START );
    mode = pFlPriv->mode;
    vel  = pFlPriv->velocity;

    dirLink = pFlPriv->fl1Device.dirLink;
    modLink = pFlPriv->fl1Device.modLink;
    posLink = pFlPriv->fl1Device.posLink;
    velLink = pFlPriv->fl1Device.velLink;
    ackLink = pFlPriv->fl1Device.ackLink;

    /*
     * Set up the useDevice array, reflecting the 'busy expected' fields
     * defined for this particular task.
     */

    useDevice[0] = taskItem->fl1Response;
    useDevice[1] = taskItem->fl2Response;

    /*
     *  Cycle through output links, only do if useDevice flag is set.
     *  They should be sequential.
     */

    for( i = 0 ; i < FL_NUM_DEVICES; i++, modLink++, dirLink++, posLink++, 
         velLink++, ackLink++ )
     {

      if (useDevice[i])        /* Use this device? */
      {
        /*
         *  Only do if the position link is connected.
         */

        if (posLink->type != CONSTANT)
        {

            /*
             *  Write the Position to the VALS, a string.
             */

            FLDEBUG(DAR_MSG_MIN, "flExecuteTask: Putting POS=%s\n", 
                    pFlPriv->position[i] );
            /*CHECKSTAT( (status = recGblPutLinkValue( posLink, 
                    (void *) par, DBR_STRING, pFlPriv->position[i], &nRequest)), 
                    return(status) );*/
	    CHECKSTAT(status = dbPutLink(posLink, DBR_STRING, pFlPriv->position[i],nRequest),return(status));
            /*
             *  Write the Velocity.
             */

            FLDEBUG(DAR_MSG_FULL, "flExecuteTask: Putting VEL=%f\n", vel );
            /*CHECKSTAT( (status = recGblPutLinkValue( velLink, (void *) par, 
                    DBR_DOUBLE, &vel, &nRequest)), 
                    return(status) );*/
             CHECKSTAT(status = dbPutLink(velLink, DBR_DOUBLE, &vel,nRequest),return(status));


            /*
             *  Write the mode.
             */

            FLDEBUG(DAR_MSG_FULL, "flExecuteTask: Putting MODE=%d\n", 
                    mode );
            /*CHECKSTAT( (status = recGblPutLinkValue( modLink, 
                    (void *) par, DBR_SHORT, &mode, &nRequest)), 
                    return(status) );*/
             CHECKSTAT ((status = dbPutLink(modLink,DBR_SHORT,&mode,nRequest)),return(status));


            /*
             *  Write the directive.
             */

            FLDEBUG(DAR_MSG_FULL, "flExecuteTask: Putting DIR=%d\n",
                        dir );
            /*CHECKSTAT( (status = recGblPutLinkValue( dirLink, 
                    (void *) par, DBR_SHORT, (void *) (&dir), &nRequest)), 
                    return(status) );*/
             CHECKSTAT(status =(dbPutLink(dirLink,DBR_SHORT,(void *) (&dir),nRequest)),return(status));

            /*
             *  Check the response from the device.
             */

            /*CHECKSTAT (status = recGblGetFastLink( ackLink, (void *) par, 
                    &returnedAck), return (status));*/
	    CHECKSTAT(status =(dbGetLink(ackLink,DBR_USHORT,&returnedAck,0,0)),return(status));
            FLDEBUG(DAR_MSG_FULL, "flExecuteTask: Got response: %d\n", 
                    returnedAck );

            if ( returnedAck != DAR_DEV_VAL_ACCEPT )
            {
                /*
                 *  Device failed, empty the list, stop the timer, set
                 *  an error message.  May be in the middle of a command
                 *  or initiallly.
                 */

                FLDEBUG(DAR_MSG_ERROR, "flExecuteTask: Filter wheel %d rejected command\n", 
                        i);

                status = DAR_E_DEVICE;
                sprintf (errMsg, "Filter wheel %d rejected command", i+1 );
                flTerminateTasks( par, status, errMsg );
                break;
            }
            else
            {
                semTake (pFlPriv->mutexSem, WAIT_FOREVER);
                pFlPriv->cmdActive = TRUE;
                semGive (pFlPriv->mutexSem);
            }

        }
        else
        {
            FLDEBUG(DAR_MSG_ERROR, "flExecuteTask: VALS link not connected%c\n", ' ' );
        }
      }
    }

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flIndexMode
 *
 * INVOCATION:
 * status = flIndexMode (par); 
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
 *    Build the task list by calling flBuildList()
 *    Start the task list by calling flDoTask()
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flIndexMode(ASSEMBLY_CONTROL_RECORD *par)
{
    FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/
    FL_DEV_CONFIG  *pDevConfig;
    long status = DAR_S_SUCCESS;

    FLDEBUG(DAR_MSG_MAX, "flIndexMode: entry%c\n", ' ');

    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
    pDevConfig = pFlPriv->pFilterPriv;

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL || pDevConfig == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flIndexMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flIndexMode: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Build the task list.
     */

    semTake (pFlPriv->mutexSem, WAIT_FOREVER);
    pFlPriv->currentCmd = par->mode;
    semGive (pFlPriv->mutexSem);

    if ( flBuildList( par, DAR_MODE_INDEX ) != DAR_S_SUCCESS )
    {
        FLDEBUG(DAR_MSG_ERROR, "flIndexMode: flbuildList failed. status=%ld\n", status);
    }
    else if ( ( status = flDoTask( par )) != DAR_S_SUCCESS )
    {

        FLDEBUG(DAR_MSG_ERROR, "flIndexMode: flDoTask failed. status=%ld\n", status);
    }
    
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flInitDeviceSupport
 *
 * INVOCATION:
 * status = flInitDeviceSupport (par); 
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
 *    Create private device support structure for this record.
 *    Link this structure to the record support private structure.
 *          (call assAttachPrivateStruct()).
 *    Create the MUTEX semaphore to protect the private structure
 *    during asynchronous callback access.
 *    Initialize private record variables.
 *    Check that the 2 deviceControl record POS[1-2] links
 *      are not CONSTANT.
 *    Create device info structure for both motors, relying on 
 *    	motor1=fl1, motor2=fl2.
 *    Initialize the task and barcode list.
 *    The task list is a list of function to perform, and
 *    the list barcodes is the characterized list.
 *
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flInitDeviceSupport(ASSEMBLY_CONTROL_RECORD *par)
{
     FL_DEV_PRIVATE *pFlPriv;            /* Private dev. struct         */
     FL_DEV_CONFIG  *pDevConfig;
     long status = DAR_S_SUCCESS;        /* Returned function status.	*/

     FLDEBUG(DAR_MSG_MAX, "flInitDeviceSupport: entry%c\n", ' ');

     /*
      *  Create private device support structure for this record.
      */

     pFlPriv = malloc (sizeof(FL_DEV_PRIVATE));
     if ( pFlPriv == NULL ) 
     {
          FLDEBUG(DAR_MSG_FATAL, "flInitDeviceSupport: malloc for FL_DEV_PRIVATE failed.%c\n", ' ');

          status = DAR_E_MALLOC;
          recGblRecordError (status, par, __FILE__ ":no room for device private");
          return( status );
     }


     /*
      *  Link this structure to the record support private structure.
      */
     
     assAttachPrivateStruct( par, (void *) pFlPriv ); 

     /*
      * Create the MUTEX semaphore to protect the private structure
      * during asynchronous callback access.
      */
     
     pFlPriv->mutexSem = NULL;
     if (  (pFlPriv->mutexSem = semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE) ) == NULL )
     {
          FLDEBUG(DAR_MSG_FATAL, "flInitDeviceSupport: semMCreate for pFlPriv->mutexSem failed.%c\n",
             ' ' );

          status = DAR_E_MALLOC;
          recGblRecordError (status, par, __FILE__ ":no room for mutex creation");
          return( status );
     }

     /*
      *  Create a configuration structure for the mechanism and write the pointer
      *  to the private device support structure.
      */

     pDevConfig = (FL_DEV_CONFIG *) malloc (sizeof(FL_DEV_CONFIG));

     if ( pDevConfig == NULL ) 
     {
          FLDEBUG(DAR_MSG_FATAL, "flInitDeviceSupport: malloc for FL_DEV_CONFIG failed.%c\n", ' ');

          status = DAR_E_MALLOC;
          recGblRecordError (status, par, __FILE__ ":no room for device config");
          return( status );
     }

     pFlPriv->pFilterPriv = pDevConfig;

     /*
      * Initialise the internal list of barcodes and the rest of the device
      * configuration structure.
      */

     flClearConfig( par );

     semTake (pFlPriv->mutexSem, WAIT_FOREVER);

     pDevConfig->magic = FL_MAGIC;

     /*
      * Initialize private record variables.
      */ 
     
     pFlPriv->pRecord     = par;
     pFlPriv->cmdActive   = FALSE; 
     pFlPriv->fl1Finished = FALSE; 
     pFlPriv->fl2Finished = FALSE; 
     pFlPriv->velocity    = 0.0;
     pFlPriv->mode        = -99;
     pFlPriv->askBarCode  = FALSE;
     pFlPriv->currentCmd  = -99;
     pFlPriv->magic       = FL_MAGIC;

     /*
      *  Create device info structure for 2 motors.
      *  NOTE: The two FL_DEV_INFO structures will have been created at the same
      *  time as the FL_DEV_PRIVATE structure above, since they are not pointers.
      */

     if (par->pos1.type == CONSTANT || par->pos2.type == CONSTANT )
     {
          FLDEBUG(DAR_MSG_FATAL,
             "flInitDeviceSupport: Not all device control records connected.%c\n", ' ' );

          status = FL_BAD_DEV_CONNECT;
          recGblRecordError (status, par, ":Not all deviceControl rec's connected");
          semGive (pFlPriv->mutexSem);
          return( status );
     }

     pFlPriv->fl1Device.dirLink = &par->odr1;
     pFlPriv->fl1Device.posLink = &par->pos1;
     pFlPriv->fl1Device.ackLink = &par->ack1;
     pFlPriv->fl1Device.velLink = &par->vel1;
     pFlPriv->fl1Device.modLink = &par->mod1;

     pFlPriv->fl2Device.dirLink = &par->odr2;
     pFlPriv->fl2Device.posLink = &par->pos2;
     pFlPriv->fl2Device.ackLink = &par->ack2;
     pFlPriv->fl2Device.velLink = &par->vel2;
     pFlPriv->fl2Device.modLink = &par->mod2;

     /* 
      *  Initialize the task and barcode list.
      */

     ellInit( &pFlPriv->taskList );

     semGive (pFlPriv->mutexSem);

     FLDEBUG(DAR_MSG_FULL, "flInitDeviceSupport: finished, status=%ld\n", status );

     return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flInitMode
 *
 * INVOCATION:
 * status = flInitMode (par); 
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
 *    Build the task list ( flBuildList() ).
 *    Start the task list ( flDoTask() ).
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flInitMode(ASSEMBLY_CONTROL_RECORD *par)
{
    FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/
    long status = DAR_S_SUCCESS;	/* Return function status.	*/

    FLDEBUG(DAR_MSG_MAX, "flInitMode: entry%c\n", ' ');


    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flInitMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flInitMode: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }


     /*
      * Read the current filter barcode configuration from the specified file
      */

     if ((status = flReadConfig( par )) != DAR_S_SUCCESS)
     {
          return( status );
     }

     /*
      * Display the current filter barcode configuration as a MIN debug message.
      */

     if ( par->dbug >= DAR_MSG_MIN )
     {
         flShowConfig( par );
     }

    /*
     *  Build the task list for initialising the filter devices.
     */

    semTake (pFlPriv->mutexSem, WAIT_FOREVER);
    pFlPriv->currentCmd = par->mode;
    semGive (pFlPriv->mutexSem);

    /*
     * Ensure the task list is empty before building the new list
     */

    flEmptyList( par, TRUE );

    if ( (status = flBuildList( par, DAR_MODE_INIT )) != DAR_S_SUCCESS )
    {
        FLDEBUG(DAR_MSG_ERROR, "flInitMode: flbuildList failed. status=%ld\n", status);
    }
    else if ( ( status = flDoTask( par )) != DAR_S_SUCCESS )
    {

        /*
         *  Start the task list, which will trigger all devices to init.
         */

        FLDEBUG(DAR_MSG_ERROR, "flInitMode: flDoTask failed. status=%ld\n", status);
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flMoveMode
 *
 * INVOCATION:
 * status = flMoveMode (par); 
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
 * Build the task list, which will first determine the current configuration,
 * then kick off the first task.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flMoveMode(ASSEMBLY_CONTROL_RECORD *par)
{
    FL_DEV_PRIVATE *pFlPriv;			/* Ptr to private dev. struct.	*/
    long           status = DAR_S_SUCCESS;	/* Return function status.	*/

    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

    FLDEBUG(DAR_MSG_MAX, "flMoveMode: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL  )
     {
         FLDEBUG(DAR_MSG_FATAL, "flMoveMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flMoveMode: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     *  Build the task list based on the mode.
     */

    semTake (pFlPriv->mutexSem, WAIT_FOREVER);
    pFlPriv->currentCmd = par->mode;
    semGive (pFlPriv->mutexSem);

    if ( par->mode == DAR_MODE_MOVE &&
            ( status = flBuildList( par, DAR_MODE_MOVE ))  != DAR_S_SUCCESS )
    {
        FLDEBUG(DAR_MSG_ERROR, "flMoveMode: flbuildList for move failed. status=%ld\n", status);
    }
    else if ( par->mode == DAR_MODE_PARK &&
            (status = flBuildList( par, DAR_MODE_PARK ) ) != DAR_S_SUCCESS )
    {
        FLDEBUG(DAR_MSG_ERROR, "flMoveMode: flbuildList for park failed. status=%ld\n", status);
    }

    /*
     *  Start the task list, as long as there is something to do.
     */

    else if ( status == DAR_S_SUCCESS &&
            ( status = flDoTask( par ) ) != DAR_S_SUCCESS )
    {
        FLDEBUG(DAR_MSG_ERROR, "flMoveMode: flDoTask failed. status=%ld\n", status);
    }
    
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flStopDirective
 *
 * INVOCATION:
 * status = flStopDirective (par); 
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
 *    If a command is currently active (cmdActive) and current command
 *    is INIT, MOVE, TRACK, TEST then ignore the directive as you want
 *    it to stop in a known state.
 *    else If a command is currently active (cmdActive) and the
 *    current command is INDEX or UPDATE then
 *    	Make sure the task list is not empty and point to the first one.
 *	Determine where to send stop to and write that into a
 *	temporary list node.
 *	Empty the list and attached the new one.
 *	Send the stops to the required devices.
 *	Start the timer.
 * 
 *  EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flStopDirective
(
    ASSEMBLY_CONTROL_RECORD  *par	/* (in)  Ptr to ass. rec.	*/
)
{
     FL_LIST        *taskRequested = NULL;	/* Ptr to task in task list.	*/
     FL_TASK_LIST   *newTask = NULL;	        /* Ptr to new task.		*/
     FL_TASK_LIST   *oldTask = NULL;	        /* Ptr to old task.		*/
     struct link    *dirLink;		        /* Dir's link.			*/
     long            status = DAR_S_SUCCESS;    /* Return function status.	*/
     long            writeStatus = DAR_S_SUCCESS;    /* Return function status.	*/
     FL_DEV_PRIVATE *pFlPriv;		        /* Private dev. support struc.	*/
     unsigned short  dir;			/* Directive to send.		*/
     long            nRequest = 1;       	/* Number of requested.		*/
     BOOL            writeFailed = FALSE;       /* A flag for the lut write op  */

     FLDEBUG(DAR_MSG_MAX, "flStopDirective: entry%c\n", ' ');

     pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

     /*
      * Trap any problem with the internal data structures
      */

     if ( pFlPriv == NULL )
     {
	  FLDEBUG(DAR_MSG_FATAL, "flStopDirective: Bad device private data structure%c\n", ' ' );
	  assDisplayPrivateStruct( par );

	  status = FL_BAD_STRUCTURE;
	  recGblRecordError (status, par, __FILE__ ":bad internal data structure");
	  return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
	  FLDEBUG(DAR_MSG_FATAL, "flStopDirective: No magic value seen in data structure%c\n", ' ' );

	  status = FL_BAD_STRUCTURE;
	  recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
	  return (status);
     }

     dirLink = &par->odr1;

     /*
      *  If a command is active and that command is move, park, test, init,
      *  do not stop the movement.
      */

     if ( pFlPriv->cmdActive && 
	  ( pFlPriv->currentCmd == DAR_MODE_MOVE || 
	    pFlPriv->currentCmd == DAR_MODE_INIT || 
	    pFlPriv->currentCmd == DAR_MODE_TEST ||
	    pFlPriv->currentCmd == DAR_MODE_PARK ) )
     {
	  FLDEBUG(DAR_MSG_LOG, "flStopDirective: ignoring STOP directive%c\n", ' ');
	  return( status );
     }
     else if ( pFlPriv->cmdActive )
     {
	  /* 
	   * Otherwise, stop between tasks - for both INDEX or UPDATE.
	   */

	  /*
	   *  Make sure the task list is not empty.
	   */

	  dir = flTranslateDir( DAR_DIR_STOP );
	  if ( ellCount( &(pFlPriv->taskList) ) == 0 || 
	       ( taskRequested = 
		 (FL_LIST *) ellFirst( &(pFlPriv->taskList) )) == NULL )
	  {
	       FLDEBUG(DAR_MSG_ERROR, "flStopDirective: task list empty. ellCount=%d\n", 
		       ellCount( &(pFlPriv->taskList) ) );

	       status = FL_TASK_LIST_EMPTY;
	       flTerminateTasks( par, status, "Filter task list is empty" );
	       return ( status );
	  }
	  oldTask = ( FL_TASK_LIST *) taskRequested->item;


	  /*
	   *  Determine where to send stops to, and write that into
	   *  the newTask list that we will add later.
	   */

	  newTask = malloc( sizeof ( FL_TASK_LIST ) );
          if ( newTask == NULL )
	  {
	       FLDEBUG(DAR_MSG_FATAL, "flStopDirective: failed to malloc new task list%c\n", ' ' );

	       status = DAR_E_MALLOC;
	       flTerminateTasks( par, status, "Filter, insufficient memory for new task");
	       return ( status );
	  }

	  newTask->fl1Response = ( oldTask->fl1Response && 
				   !pFlPriv->fl1Finished )? TRUE : FALSE;
	  newTask->fl2Response = ( oldTask->fl2Response && 
				   !pFlPriv->fl2Finished )? TRUE : FALSE;
	  newTask->task = FL_STOP_TASK;

	  semTake (pFlPriv->mutexSem, WAIT_FOREVER);
	  if (!newTask->fl1Response) pFlPriv->fl1Finished = FALSE;
	  if (!newTask->fl2Response) pFlPriv->fl2Finished = FALSE;
	  semGive (pFlPriv->mutexSem);

	  /*
	   *  Empty the list and attached the new one.
	   */

	  flEmptyList( par, TRUE );

	  /* Add the stop task only if a device needs stopping */

	  if (newTask->fl1Response || newTask->fl2Response)
	  {
	       taskRequested = malloc( sizeof ( FL_LIST ) );
               if ( taskRequested == NULL )
               {
                  FLDEBUG(DAR_MSG_FATAL, "flStopDirective: Memory allocation failure for stop task%c\n", ' ' );

                  status = DAR_E_MALLOC;
	          flTerminateTasks(par, status, "Filter, insufficient memory for new task" );
                  return (status);
               }
	       taskRequested->item = (void *) newTask;
	       ellAdd( (ELLLIST *) &(pFlPriv->taskList), &(taskRequested->node) );
	       taskRequested = ( FL_LIST *) ellNext( &(taskRequested->node) );
	  }

	  /* If there is a partially completed UPDATE then... */

	  if (pFlPriv->currentCmd == DAR_MODE_UPDATE)
	  {
	       /*
		* Display the current filter barcode configuration.
		*/

	       flShowConfig( par );

	       /*
		* Write the current filter barcode configuration to the specified file
		*/

	       if ((writeStatus = flWriteConfig(par)) != DAR_S_SUCCESS)
	       {
		    writeFailed = TRUE;
	       }
	  }

	  /*
	   *  Send the stops to the required devices.
	   */

	  if ( newTask->fl1Response && !pFlPriv->fl1Finished)
	  {
	       /*
		*  Send a stop to Filter 1.
		*/

	       FLDEBUG(DAR_MSG_FULL, "flStopDirective: sending STOP to filter wheel 1%c\n",
		       ' ');
	       /*status = recGblPutLinkValue( pFlPriv->fl1Device.dirLink,
					    (void *) par, DBR_SHORT, (void *) (&dir), &nRequest);*/
                 status = dbPutLink(pFlPriv->fl1Device.dirLink,DBR_SHORT,(void *) (&dir),nRequest);
	  }

	  if ( status == DAR_S_SUCCESS && newTask->fl2Response && !pFlPriv->fl2Finished )
	  {
	       /*
		*  Send a stop to filter2.
		*/

	       FLDEBUG(DAR_MSG_FULL, "flStopDirective: sending STOP to filter wheel 2%c\n",
		       ' ');
	       /*status = recGblPutLinkValue( pFlPriv->fl2Device.dirLink,
					    (void *) par, DBR_SHORT, (void *) (&dir), &nRequest);*/
                status = dbPutLink(pFlPriv->fl2Device.dirLink,DBR_SHORT,(void *) (&dir),nRequest);
	  }

	  /*
	   *  Start the timer.
	   */

	  if ( status == DAR_S_SUCCESS )
	  {
	       assStartTimer( par, &status, FL_COMMAND_SHORT_TIMEOUT );
	       if (status != DAR_S_SUCCESS)
	       {
		    FLDEBUG(DAR_MSG_WARNING,
                            "flStopDirective: failed to start watchdog timer. status=%ld\n",
			    status);
	       }
	  }
	  else
	  {
	       flTerminateTasks(par, status, "Filter, failed to write STOP directive" );
	  }

     }
     else
     {
	  /*
	   *  No command currently active, just tell them we finished fine.
	   */

	  FLDEBUG(DAR_MSG_WARNING, "flStopDirective: no command active - STOP directive ignored%c\n",
		  ' ');
	  flTerminateTasks( par, status, "Filter, no command active" );
     }

     if (writeFailed)
     {
	  flTerminateTasks (par, writeStatus, "Failed to write config file");
     }

     return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flTaskCheck
 *
 * INVOCATION:
 * status = flTaskCheck (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Determine if the current task has finished correctly
 *
 * DESCRIPTION:
 * Determine if the current task has finished correctly by calling
 * the appropriate mechanism checking routine.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flTaskCheck(ASSEMBLY_CONTROL_RECORD *par)
{
     long status = DAR_S_SUCCESS;
     FL_LIST *pList = NULL;		/* Ptr to list.			*/
     FL_TASK_LIST *taskPerformed = NULL;/* Task performed.		*/
     FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/
     FL_DEV_CONFIG  *pDevConfig;
     long barcode = FL_NO_BARCODE;
     long nRequest = 1;
     long minusOne = -1;
     long switches;

     pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pFlPriv->pFilterPriv;

     FLDEBUG(DAR_MSG_MAX, "flTaskCheck: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL || pDevConfig == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flTaskCheck: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC || pDevConfig->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flTaskCheck: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

     /*
      *  Depending on the task performed, check different sensor words.
      */
    
     if ( ellCount( &(pFlPriv->taskList) ) == 0 || 
          ( pList = (FL_LIST *) ellFirst( &(pFlPriv->taskList) )) == NULL )
     {
          FLDEBUG(DAR_MSG_MIN, "flTaskCheck: filter task list empty, ellCount=%d\n", 
                ellCount( &(pFlPriv->taskList) ) );
          status = FL_LAST_TASK;

          return ( status );
     }

     taskPerformed = ( FL_TASK_LIST *) pList->item;

     switch( taskPerformed->task )
     {

     case FL_STOP_TASK:
     case FL_INIT_ALL:

          /*
           * Set the barcodes to "unknown" after initialisation or stopping a movement.
           */

          barcode = FL_NO_BARCODE;
          /*CHECKSTAT ((status = recGblPutLinkValue (&(par->sot), (void *) par, DBR_LONG,
            &barcode, &nRequest)), return (status));*/
          CHECKSTAT ((status =dbPutLink(&(par->sot),DBR_LONG,&barcode,nRequest)),return(status));
          /*CHECKSTAT ((status = recGblPutLinkValue (&(par->sou), (void *) par, DBR_LONG,
            &barcode, &nRequest)), return (status));*/
          CHECKSTAT ((status =dbPutLink(&(par->sou),DBR_LONG,&barcode,nRequest)),return(status));
          break;

     case FL_TEST_ALL:

          /* This command doesn't need any checks */

          break;

     case FL_INDEX_ALL:
     case FL_PARK_ALL_DEFAULT:
     case FL_PARK_ALL_LOAD:

          /*
           * Set the current barcodes after any movement.
           */

          /*CHECKSTAT ((status = recGblPutLinkValue (&(par->sot), (void *) par, DBR_LONG,
            &barcode, &nRequest)), return (status));*/
          CHECKSTAT ((status =dbPutLink(&(par->sot),DBR_LONG,&barcode,nRequest)),return(status));

          /*CHECKSTAT ((status = recGblPutLinkValue (&(par->sou), (void *) par, DBR_LONG,
            &barcode, &nRequest)), return (status));*/
          CHECKSTAT ((status =dbPutLink(&(par->sou),DBR_LONG,&barcode,nRequest)),return(status));
          break;

     case FL_MV_FILT_BARCODE:
     case FL_MV_FILT_BARCODE + 1:
     case FL_MV_FILT_BARCODE + 2:
     case FL_MV_FILT_BARCODE + 3:
     case FL_MV_FILT_BARCODE + 4:
     case FL_MV_FILT_BARCODE + 5:
     case FL_MV_FILT_BARCODE + 6:
     case FL_MV_FILT_BARCODE + 7:
     case FL_MV_FILT_BARCODE + 8:
     case FL_MV_FILT_BARCODE + 9:
     case FL_MV_FILT_BARCODE + 10:
     case FL_MV_FILT_BARCODE + 11:

          /*
           * Set the current barcodes after any movement.
           */

          /*CHECKSTAT ((status = recGblPutLinkValue (&(par->sot), (void *) par, DBR_LONG,
            &barcode, &nRequest)), return (status));*/
          CHECKSTAT ((status =dbPutLink(&(par->sot),DBR_LONG,&barcode,nRequest)),return(status));
          /*CHECKSTAT ((status = recGblPutLinkValue (&(par->sou), (void *) par, DBR_LONG,
            &barcode, &nRequest)), return (status));*/
           CHECKSTAT ((status =dbPutLink(&(par->sou),DBR_LONG,&barcode,nRequest)),return(status));
          /*
           * There is no need to check the status of the in position switches.
           * The important thing after a movement to a barcode reading position
           * is that the barcodes are readable.
           */

          break;

     case FL_WRITE_CONFIG:

          /* This command doesn't need any checks */

          break;

     case FL_MV_FILTER1:

          /*
           * Check the status of the in position switches
           * Clear the barcode ID output if not in position.
           */

          /*CHECKSTAT ((status = recGblPutLinkValue (&(par->sot), (void *) par, DBR_LONG,
            &(pFlPriv->barcode[0]), &nRequest)), return (status));*/
          CHECKSTAT ((status = dbPutLink(&(par->sot),DBR_LONG, &(pFlPriv->barcode[0]),nRequest)),return(status));

          if ( pFlPriv->ignoreSwitches == 0 )
          {
              switches = flCheckInPosSwitches (par);

              if (switches == 0 || switches == 2)
              {
                   status = FL_FILT1_ERROR;
                   /*CHECKSTAT (recGblPutLinkValue (&(par->sot), (void *) par, DBR_LONG, &minusOne,
                     &nRequest), return (DAR_FAILURE));*/
                     CHECKSTAT ((status = dbPutLink(&(par->sot),DBR_LONG, &minusOne,nRequest)),return(DAR_FAILURE));
              }
          }
          break;

     case FL_MV_FILTER2:

          /*
           * Check the status of the in position switches
           * Clear the barcode ID output if not in position.
           */

          /*CHECKSTAT ((status = recGblPutLinkValue (&(par->sou), (void *) par, DBR_LONG,
            &(pFlPriv->barcode[1]), &nRequest)), return (status));*/
            CHECKSTAT ((status = dbPutLink(&(par->sou),DBR_LONG,  &(pFlPriv->barcode[1]),nRequest)),return(status));

          if ( pFlPriv->ignoreSwitches == 0 )
          {
              switches = flCheckInPosSwitches (par);

              if (switches < 2)
              {
                   status = FL_FILT2_ERROR;
                   /*CHECKSTAT (recGblPutLinkValue (&(par->sou), (void *) par, DBR_LONG, &minusOne,
                     &nRequest), return (DAR_FAILURE));*/
                   CHECKSTAT ((status = dbPutLink(&(par->sou),DBR_LONG, &minusOne,nRequest)),return(DAR_FAILURE));
              }
          }
          break;

     case FL_MV_FILTERS:

          /*
           * Check the status of the in position switches
           * Clear the barcode ID outputs if not in position.
           */

          /*CHECKSTAT ((status = recGblPutLinkValue (&(par->sot), (void *) par, DBR_LONG,
            &(pFlPriv->barcode[0]), &nRequest)), return (status));*/

	 CHECKSTAT ((status = dbPutLink(&(par->sot),DBR_LONG,&(pFlPriv->barcode[0]),nRequest)),return(status));


          /*CHECKSTAT ((status = recGblPutLinkValue (&(par->sou), (void *) par, DBR_LONG,
            &(pFlPriv->barcode[1]), &nRequest)), return (status));*/
          CHECKSTAT ((status = dbPutLink(&(par->sou),DBR_LONG,&(pFlPriv->barcode[1]),nRequest)),return(status));

          if ( pFlPriv->ignoreSwitches == 0 )
          {
              switches = flCheckInPosSwitches (par);

              if (switches == 2)
              {
                   status = FL_FILT1_ERROR;
                   /*CHECKSTAT (recGblPutLinkValue (&(par->sot), (void *) par, DBR_LONG, &minusOne,
                      &nRequest), return (DAR_FAILURE));*/
                    CHECKSTAT ((dbPutLink(&(par->sot),DBR_LONG,&minusOne,nRequest)),return(DAR_FAILURE));
              }
              else if (switches == 1)
              {
                   status = FL_FILT2_ERROR;
                   /*CHECKSTAT (recGblPutLinkValue (&(par->sou), (void *) par, DBR_LONG, &minusOne,
                     &nRequest), return (DAR_FAILURE));*/
             CHECKSTAT ((dbPutLink(&(par->sou),DBR_LONG,&minusOne,nRequest)),return(DAR_FAILURE)); 
	      }
              else if (!switches)
              {
                   status = FL_FILTS_ERROR;
                   /*CHECKSTAT (recGblPutLinkValue (&(par->sot), (void *) par, DBR_LONG, &minusOne,
                     &nRequest), return (DAR_FAILURE));*/
                   CHECKSTAT ((dbPutLink(&(par->sot),DBR_LONG,&minusOne,nRequest)),return(DAR_FAILURE));
                   /*CHECKSTAT (recGblPutLinkValue (&(par->sou), (void *) par, DBR_LONG, &minusOne,
                     &nRequest), return (DAR_FAILURE));*/
                   CHECKSTAT ((dbPutLink(&(par->sou),DBR_LONG,&minusOne,nRequest)),return(DAR_FAILURE));
              }
          }
          break;


     case FL_READ_BARCODE_1:

          status = flCheckBarcodeId(par, 1);
          break;

     case FL_READ_BARCODE_2:

          status = flCheckBarcodeId(par, 2);
          break;

     default:
          FLDEBUG(DAR_MSG_ERROR, "flTaskCheck: Invalid task number=%d\n", taskPerformed->task );
          status = FL_INVALID_TASK;
     }

     return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flTaskFinished
 *
 * INVOCATION:
 * status = flTaskFinished (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Determine if the current task is finished
 *
 * DESCRIPTION:
 *    Make sure the list is not empty.  This is an error, if it occurs.
 *    Point to current one in the list.
 *    Make sure every device has responded for this task and if it has:
 *    	then stop the timer
 *	Delete the top task off, as that is the one that just finished.
 *	If the list is now empty then
 *		set the cmdFinished to TRUE,
 *		set cmdActive to FALSE
 *	otherwise there are more tasks to do.
 *	    call flDoTask() to execute the task.
 *	    and start the timer again.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * Assumes that BUS1=FLT1, BUS2=FLT2
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flTaskFinished(ASSEMBLY_CONTROL_RECORD *par)
{
     long           status = DAR_S_SUCCESS;
     FL_LIST        *pTask = NULL;		/* Ptr to task list.              */
     FL_TASK_LIST   *currentTask = NULL;	/* Ptr to current task.           */
     FL_DEV_PRIVATE *pFlPriv;			/* Ptr to private dev. struct.	  */
     int            numCompleted;		/* Number of completed tasks.	  */
     long timerStatus = DAR_S_SUCCESS;          /* status word from assStopTimer  */

     pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

     FLDEBUG(DAR_MSG_MAX, "flTaskFinished: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flTaskFinished: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flTaskFinished: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

     /*
      *  Make sure the list is not empty.  This is an error, if it occurs.
      */

     if ( ellCount( &(pFlPriv->taskList) ) == 0 )
     {
          FLDEBUG(DAR_MSG_ERROR, "flTaskFinished: filter task list empty, ellCount=%d\n",
                  ellCount( &(pFlPriv->taskList) ) );

          status = DAR_E_DEVICE;
          flTerminateTasks( par, status, "Filter task list empty" );
          return (status);
     }

     /* 
      *  Point to current one in the list.
      */

     pTask = (FL_LIST *) ellFirst( &(pFlPriv->taskList) );
     currentTask = ( FL_TASK_LIST *) pTask->item;

     FLDEBUG(DAR_MSG_FULL, "flTaskFinished: current task=%d\n", currentTask->task);
     numCompleted = currentTask->fl1Response + currentTask->fl2Response;

     FLDEBUG(DAR_MSG_MAX, "flTaskFinished: expect=%d\n", numCompleted );
     numCompleted = pFlPriv->fl1Finished + pFlPriv->fl2Finished;

     FLDEBUG(DAR_MSG_MAX, "flTaskFinished: completed=%d\n", numCompleted);

     if ( (  ( currentTask->fl1Response ==  pFlPriv->fl1Finished ) &&
             ( currentTask->fl2Response == pFlPriv->fl2Finished ) ) ||
          ( pFlPriv->askBarCode == TRUE ) )
     {
          /*
           *  Task is finished, stop the timer, check any necessary sensors
           *  based on the task that was performed & then see if there is 
           *  another one.
           */

          FLDEBUG(DAR_MSG_MIN, "flTaskFinished: Task finished%c\n", ' ');

          assStopTimer( par, &timerStatus );
          if (timerStatus != DAR_S_SUCCESS)
          {
              FLDEBUG(DAR_MSG_WARNING, "flTaskFinished: failed to stop watchdog timer. status=%ld\n",
                 timerStatus);
          }

          status = flTaskCheck( par );

          if (status == FL_LAST_TASK)
          {
               FLDEBUG(DAR_MSG_WARNING, "flTaskFinished, FL_LAST_TASK reported%c\n", ' ');
               flTerminateTasks( par, status, "Filter task check reported FL_LAST_TASK");
          }
          else if (status == FL_FILT1_ERROR)
          {
               FLDEBUG(DAR_MSG_ERROR, "flTaskFinished, error detected in filter wheel 1%c\n", ' ');
               flTerminateTasks( par, status, "Filter 1 not in position - INDEX needed");
          }
          else if (status == FL_FILT2_ERROR)
          {
               FLDEBUG(DAR_MSG_ERROR, "flTaskFinished, error detected in filter wheel 1%c\n", ' ');
               flTerminateTasks( par, status, "Filter 2 not in position - INDEX needed");
          }
          else if (status == FL_FILTS_ERROR)
          {
               FLDEBUG(DAR_MSG_ERROR, "flTaskFinished, error detected in both filter wheels%c\n", ' ');
               flTerminateTasks( par, status, "Filters 1+2 not in position - INDEX needed");
          }
          else if (status == FL_INVALID_BARCODE)
          {
               FLDEBUG(DAR_MSG_ERROR, "flTaskFinished, failed to read barcode%c\n", ' ');
               flTerminateTasks( par, status, "Filter barcode read failure");
          }
          else if (status != DAR_S_SUCCESS)
          {
               FLDEBUG(DAR_MSG_ERROR, "flTaskFinished, task check failed. status=%ld\n", status);
               flTerminateTasks( par, status, "Filter task check failed");
          }
          else if ( flEmptyList( par, FALSE ) == FL_EMPTY_LIST )
          {
               FLDEBUG(DAR_MSG_MIN, "flTaskFinished, all tasks done%c\n", ' ');
               flTerminateTasks( par, status, NULL );
          }
          else
          {
               /* There are more tasks, so kick them off */

               if ( ( status = flDoTask( par )) != DAR_S_SUCCESS && status != FL_LAST_TASK )
               {
                    FLDEBUG(DAR_MSG_ERROR, "flTaskFinished: flDoTask failed. status=%ld\n", status);
               }
          }
     }
     else
     {
          FLDEBUG(DAR_MSG_MIN, "flTaskFinished: Task not finished yet%c\n", ' ');
     }

     return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flTestMode
 *
 * INVOCATION:
 * status = flTestMode (par); 
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
 * Test that all links to the 2 motors are not CONSTANT.
 * Test that the task list is empty.
 * Forward a TEST command to the underlying devices.
 *
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 * Assumes that deviceControl records are wired up with
 * 1=FLT1, 2=FLT2
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flTestMode
(
    ASSEMBLY_CONTROL_RECORD *par	/* (in)  Ptr to ass. rec.	*/
)
{
    long      status = DAR_S_SUCCESS;	/* Returned function status.	*/
    FL_DEV_PRIVATE 
              *pFlPriv;			/* Ptr to private dev. struct.	*/


    FLDEBUG(DAR_MSG_MAX, "flTestMode: entry%c\n", ' ');

    /*
     * Check the integrity of the device private structures
     */

    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

    if ( pFlPriv == NULL )
    {
        FLDEBUG(DAR_MSG_FATAL, "flTestMode: Bad device private data structure%c\n", ' ' );
        assDisplayPrivateStruct( par );

        status = FL_BAD_STRUCTURE;
        assCommandFinish( par, status, "Bad internal data structures");
        return (status);
    }
    else if ( pFlPriv->magic != FL_MAGIC )
    {
        FLDEBUG(DAR_MSG_FATAL, "flDoTask: No magic value seen in data structure%c\n", ' ' );
        assDisplayPrivateStruct( par );

        status = FL_BAD_STRUCTURE;
        flTerminateTasks( par, status, "Bad internal data structures");
        return (status);
    }
    else if ( par->dbug >= DAR_MSG_MAX )
    {
        /* Display the private structure in MAX debug mode. */

        assDisplayPrivateStruct( par );
    }

    semTake (pFlPriv->mutexSem, WAIT_FOREVER);
    pFlPriv->currentCmd = par->mode;
    semGive (pFlPriv->mutexSem);

    /*
     *  Check that all position, mode, directive, ack links
     *  are NOT CONSTANT.
     */

    if ( par->pos1.type == CONSTANT || par->ack1.type == CONSTANT || 
         par->mod1.type == CONSTANT || par->odr1.type == CONSTANT ||
         par->vel1.type == CONSTANT )
    {
        /*
         *  Check the Filter 1 links.
         */

        FLDEBUG(DAR_MSG_ERROR, "flTestMode: Filter 1 device links not connected.%c\n", ' ' );

        status = FL_BAD_DEV_CONNECT;
        flTerminateTasks( par, status, "Filter 1 links not connected");
    }
    else if ( par->pos2.type == CONSTANT || par->ack2.type == CONSTANT || 
              par->mod2.type == CONSTANT || par->odr2.type == CONSTANT ||
              par->vel2.type == CONSTANT )
    {
        /*
         *  Check the Filter 2 links.
         */

        FLDEBUG(DAR_MSG_ERROR, "flTestMode: Filter 2 device links not connected.%c\n", ' ' );

        status = FL_BAD_DEV_CONNECT;
        flTerminateTasks( par, status, "Filter 2 links not connected");
    }

    else if ( ellCount( &(pFlPriv->taskList) ) != 0 )
    {
        /*
         *  Error, task list is not empty.
         */

        FLDEBUG(DAR_MSG_ERROR, "flTestMode: Task list not empty.%c\n", ' ' );

        status = FL_TASK_LIST_NOT_EMPTY;
        flTerminateTasks( par, status, "Filter task list not empty");
    }
    else
    {
        /* 
         *  All assembly tests pass successfully.
         *  Build the task list for testing the filter devices.
         */

        semTake (pFlPriv->mutexSem, WAIT_FOREVER);
        pFlPriv->currentCmd = par->mode;
        semGive (pFlPriv->mutexSem);

        if ( (status = flBuildList( par, DAR_MODE_TEST )) != DAR_S_SUCCESS )
        {
            FLDEBUG(DAR_MSG_ERROR, "flTestMode: flbuildList failed. status=%ld\n", status);
        }
        else if ( ( status = flDoTask( par )) != DAR_S_SUCCESS )
        {

            /*
             *  Start the task list, which will trigger all devices to test.
             */

            FLDEBUG(DAR_MSG_ERROR, "flTestMode: flDoTask failed. status=%ld\n", status);
        }
    }

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flTrackMode
 *
 * INVOCATION:
 * status = flTrackMode (par); 
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
 * Do nothing for now.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flTrackMode(ASSEMBLY_CONTROL_RECORD *par)
{
    long status = DAR_S_SUCCESS;	/* Returned function status.	*/

    FLDEBUG(DAR_MSG_LOG, "flTrackMode: TRACK mode ignored%c\n", ' ');

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flTranslateDir
 *
 * INVOCATION:
 * status = flTranslateDir (par); 
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
 * DEFICIENCIES:
 * Only translates START and STOP directives.
 *-
 ************************************************************************
 */

static long flTranslateDir(unsigned short inDir)
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

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flUpdateMode
 *
 * INVOCATION:
 * status = flUpdateMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization code.
 *
 * PURPOSE:
 * Execute a MODE of UPDATE
 *
 * DESCRIPTION:
 * If the barcode list is not already empty, then empty it.
 * Build the task list.
 * Kick off the first task.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flUpdateMode(ASSEMBLY_CONTROL_RECORD *par)
{
    FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/

    long status = DAR_S_SUCCESS;	/* Return function status.	*/

    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

    FLDEBUG(DAR_MSG_MAX, "flUpdateMode: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL  )
     {
         FLDEBUG(DAR_MSG_FATAL, "flUpdateMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flUpdateMode: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    semTake (pFlPriv->mutexSem, WAIT_FOREVER);
    pFlPriv->currentCmd = par->mode;

    /* Reset the barcode index */
    pFlPriv->whichLutIndex = 0;

    semGive (pFlPriv->mutexSem);

    /* Build the task list. */

    if ( (status = flBuildList( par, DAR_MODE_UPDATE ) ) != DAR_S_SUCCESS )
    {
        FLDEBUG(DAR_MSG_ERROR, "flUpdateMode: flBuildList failed. status=%ld\n", status);
    }

    /* Start the task list */

    else if ( ( status = flDoTask( par )) != DAR_S_SUCCESS )
    {
        FLDEBUG(DAR_MSG_ERROR, "flUpdateMode: flDoTask failed. status=%ld\n", status);
    }
    
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flReadConfig
 *
 * INVOCATION:
 * status = flReadConfig (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 * Read the current filter barcode configuration file from a file
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flReadConfig(ASSEMBLY_CONTROL_RECORD *par)
{
     ASS_CONTROL_PRIVATE *pPriv = par->dpvt;	/* Private rec. support struct.	*/
     FL_DEV_PRIVATE *pFlPriv;			/* Ptr to private dev. struct.	*/
     FL_DEV_CONFIG  *pDevConfig;
     long status = DAR_S_SUCCESS;		/* Return function status.	*/
     char scratchBuf[FL_SCRATCH_BUFFER_SIZE];
     char pos[MAX_STRING_SIZE];
     int wheel;
     long barcode = FL_NO_BARCODE;
     FILE *fp;
     int i = 0;

     pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pFlPriv->pFilterPriv;

     FLDEBUG(DAR_MSG_MAX, "flReadConfig: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flReadConfig: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flReadConfig: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

     /*
      * Clear the internal barcode list.
      */

     flClearConfig( par );

     /*
      * Assemble the name of the barcode file and attempt to open it. If the file cannot be
      * opened for reading just assume it has not been written yet. This is NOT an error,
      * since the file will be created on the first successful UPDATE. A file stub is created
      * to verify that a new file can be created. If this fails then there is a problem.
      */

     i = 0;

     *scratchBuf = '\0';
     strncat( scratchBuf, par->tdir, MAX_STRING_SIZE );
     strcat( scratchBuf, "/" );
     strncat(scratchBuf, par->tfil, MAX_STRING_SIZE );

     if ( (fp = fopen(scratchBuf, "r")) == NULL)
     {
        if ( (fp = fopen(scratchBuf, "w")) == NULL)
        {
           FLDEBUG(DAR_MSG_ERROR,
              "flReadConfig: barcode file, %.40s, does not exist and cannot be created.\n",
              scratchBuf);

           /* A file open error should not affect the index status */
           pPriv->keepIndex = TRUE;

           status = FL_FILEIO_ERROR;
           flTerminateTasks(par, status, "Failed to open barcode file");

           return (status);
        }
        else
        {
           FLDEBUG(DAR_MSG_WARNING,
              "flReadConfig: Barcode file, %.40s, did not exist - created file stub.\n",
              scratchBuf);

           /* Warning message commented out - caused assembly record to ignore next command,
            * since the assembly record regards a command being accepted with a message outstanding
            * as a failure. */

           /* assAddErrorMessage( par, "No barcode file - UPDATE needed"); */

           /*
            * Write the FL_LUT_FIRST_STRING string, close the file and return.
            * There is no point in attempting to read an empty file.
            */

           fprintf(fp, "%s\n", FL_LUT_FIRST_STRING);
           fclose (fp);  
           return (status);
        }
     }

     /* The first line should contain the string "filter". Check this first */

     if (fgets(scratchBuf, sizeof scratchBuf, fp) == NULL) 
     {
          FLDEBUG(DAR_MSG_ERROR, "flReadConfig: failed to read barcode file.%c\n", ' ');

          /* A file read error should not affect the index status */
          pPriv->keepIndex = TRUE;

          status = FL_FILEIO_ERROR;
          flTerminateTasks(par, status, "Failed to read barcode file");
     }
     else if (strncmp (scratchBuf, FL_LUT_FIRST_STRING, 6))
     {
          FLDEBUG(DAR_MSG_ERROR, "flReadConfig: could not find \"%s\" string\n",
             FL_LUT_FIRST_STRING);

          /* A file read error should not affect the index status */
          pPriv->keepIndex = TRUE;

          status = FL_FILEIO_ERROR;
          flTerminateTasks(par, status, "Incorrect barcode file type");
     }
     else
     {
          /* Process one entry at a time */
                          
          while (fgets(scratchBuf, sizeof scratchBuf, fp) != NULL) 
          {
               FLDEBUG(DAR_MSG_MAX, "flReadConfig: got a line: %.60s\n",  scratchBuf);

               if (*scratchBuf != '#' && *scratchBuf != '\n')     /* a comment or empty line, discard it */
               {
                    /*
                     * Read the configuration file into the local structure.
                     */

                    if (sscanf(scratchBuf, "%ld %d %s", &barcode, &wheel, pos) != 3)
                    {
                         FLDEBUG(DAR_MSG_ERROR, "flReadConfig: parse failed: %.60s\n", scratchBuf);

                         /* A file read error should not affect the index status */
                         pPriv->keepIndex = TRUE;

                         status = FL_FILEIO_ERROR;
                         flTerminateTasks(par, status, "Failed to read barcode file");
                         break;
                    }
                    else
                    {
                         /* read line was successful, populate internal store - after these checks. */

                         if (i > FL_MAX_FILTERS)
                         {
                              FLDEBUG(DAR_MSG_ERROR,
                                      "flReadConfig: File contains too many filters, %d\n ", i);

                              /* A file read error should not affect the index status */
                              pPriv->keepIndex = TRUE;

                              status = FL_FILEIO_ERROR;
                              flTerminateTasks(par, status, "Too many filters in barcode file");
                         }
                         else if ((wheel > FL_NUM_DEVICES) || (wheel <= 0))
                         {
                              FLDEBUG(DAR_MSG_ERROR,
                                      "flReadConfig: Invalid wheel # in barcode file, %d\n ", i);

                              /* A file read error should not affect the index status */
                              pPriv->keepIndex = TRUE;

                              status = FL_FILEIO_ERROR;
                              flTerminateTasks(par, status, "Invalid wheel # in barcode file");
                         }
                         else
                         {

                              /*
                               * Add a new entry to the database, using a semaphore to prevent
                               * simultaneous access.
                               */

                              semTake (pFlPriv->mutexSem, WAIT_FOREVER);

                              pDevConfig->barcode[i] = barcode;
                              pDevConfig->wheel[i] = wheel;
                              strncpy(pDevConfig->pos[i], pos, MAX_STRING_SIZE - 1);
                              pDevConfig->characterized = TRUE;

                              semGive (pFlPriv->mutexSem);
                              i++;
                         }
                    }
               }
          }

          /* Set the high water mark to the number of entries read */

          semTake (pFlPriv->mutexSem, WAIT_FOREVER);
          pDevConfig->highwater = i;
          semGive (pFlPriv->mutexSem);

          /* Report an error if no valid entries have been read */

          if (!pDevConfig->characterized)
          {
               FLDEBUG(DAR_MSG_WARNING,
                  "flReadConfig: Barcode file contains no entries.%c\n", ' ');

               /* Warning message commented out - caused assembly record to ignore next command,
                * since the assembly record regards a command being accepted with a message outstanding
                * as a failure. */

               /* assAddErrorMessage( par, "No barcode entries - UPDATE needed"); */
          }
     }

     fclose (fp);  
     return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flWriteConfig
 *
 * INVOCATION:
 * status = flWriteConfig (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 * Write the current filter barcode configuration database to file
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long flWriteConfig(ASSEMBLY_CONTROL_RECORD *par)
{
     ASS_CONTROL_PRIVATE *pPriv = par->dpvt;	/* Private rec. support struct.	*/
     FL_DEV_PRIVATE *pFlPriv;			/* Ptr to private dev. struct.	*/
     FL_DEV_CONFIG  *pDevConfig;
     char scratchBuf[FL_SCRATCH_BUFFER_SIZE];
     char scratchBuf2[FL_SCRATCH_BUFFER_SIZE];
     long status = DAR_S_SUCCESS;		/* Return function status.	*/
     int i, j, c[7];
     FILE *fp;
     char *month[] = { "000", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

     pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pFlPriv->pFilterPriv;

     FLDEBUG(DAR_MSG_MAX, "flWriteConfig: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL || pDevConfig == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flWriteConfig: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC || pDevConfig->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flWriteConfig: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

     /* Get a time stamp for the lookup tables */

     j = timeNowC (UTC, 3, c);

     /* First save the default lookup table file */

     *scratchBuf = '\0';
     strncat (scratchBuf, par->tdir, MAX_STRING_SIZE );
     strcat (scratchBuf, "/" );
     strncat (scratchBuf, par->tfil, MAX_STRING_SIZE );

     if (!(fp = fopen(scratchBuf, "w")))
     {
          FLDEBUG(DAR_MSG_ERROR, "flWriteConfig: failed to create barcode file, %.60s\n", scratchBuf);

          /* A file open error should not affect the index status */
          pPriv->keepIndex = TRUE;

          status = FL_FILEIO_ERROR;
          flTerminateTasks(par, status, "Failed to create new barcode file");

          return (status);
     }

     fprintf(fp, "%s\n", FL_LUT_FIRST_STRING);
     fprintf(fp, "%s\n", "#\n# Database of current GMOS filter installation\n");
     if ( j == 0 )
     {
          fprintf (fp, "# Time : %d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d.%3.3d (UTC)\n",
                   c[0], c[1], c[2], c[3], c[4], c[5], c[6]);
     }
     fprintf(fp, "%s\n", "# Barcode  Wheel Pos\n");

     for(i = 0; i < pDevConfig->highwater; i++)
     {
          fprintf(fp, "%10d %2d %s\n", pDevConfig->barcode[i], pDevConfig->wheel[i],
             pDevConfig->pos[i]);
     }

     fclose (fp);


     /* Then save the backup lookup table file */

     if (j == 0)
     {
          *scratchBuf  = '\0';
          strncat (scratchBuf, par->tdir, MAX_STRING_SIZE );
          strcat (scratchBuf, "/" );

          *scratchBuf2 = '\0';
          sprintf (scratchBuf2, "%s.%02d%s%4d_%02d%02dUTC", par->tfil, c[2], month[c[1]],
                   c[0], c[3], c[4]);

          strncat (scratchBuf, scratchBuf2, MAX_STRING_SIZE );

          if (!(fp = fopen(scratchBuf, "w")))
          {
               FLDEBUG(DAR_MSG_ERROR, "flWriteConfig: Failed to create backup file, %.60s\n",
                  scratchBuf);

               /* A file open error should not affect the index status */
               pPriv->keepIndex = TRUE;

               status = FL_FILEIO_ERROR;
               flTerminateTasks(par, status, "Failed to create backup file");

               return(status);
          }

          fprintf (fp, "%s\n", FL_LUT_FIRST_STRING);
          fprintf (fp, "%s\n", "#\n# Database of current GMOS filter installation\n");

          fprintf (fp, "# Time : %d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d.%3.3d (UTC)\n",
                   c[0], c[1], c[2], c[3], c[4], c[5], c[6]);

          fprintf (fp, "%s\n", "# Barcode  Wheel Pos\n");

          for(i = 0; i < pDevConfig->highwater; i++)
          {
               fprintf(fp, "%10d %2d %s\n", pDevConfig->barcode[i], pDevConfig->wheel[i],
                       pDevConfig->pos[i]);
          }

          sprintf (scratchBuf, "File %.28s saved", scratchBuf2);
          assAddErrorMessage(par, scratchBuf);
          FLDEBUG(DAR_MSG_LOG, "flWriteConfig: backup file is %.60s\n", scratchBuf);
          fclose (fp);  
     }
     else
     {
          FLDEBUG(DAR_MSG_WARNING,
             "flWriteConfig: no TCS time system, backup file not saved%c\n", ' ');
          assAddErrorMessage(par, "No Backup file saved");
     }

     /*
      * Report if the database contains one or more bad barcodes. If needed, this will
      * overwrite the informational message about backup files.
      */

     if ( pDevConfig->badBarcode )
     {
          FLDEBUG(DAR_MSG_WARNING, "flWriteConfig: One or more barcode read failures%c\n", ' ');
          assAddErrorMessage(par, "Barcode failures - try QUICK UPDATE");
     }

     return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flClearConfig
 *
 * INVOCATION:
 * flClearConfig (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Clear the current filter barcode configuration database
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the validity of "par" and the internal data structures has
 * already been verified.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static void flClearConfig(ASSEMBLY_CONTROL_RECORD *par)
{
     FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/
     FL_DEV_CONFIG  *pDevConfig;
     int i = 0;

     pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pFlPriv->pFilterPriv;

     FLDEBUG(DAR_MSG_MAX, "flClearConfig: entry%c\n", ' ');

     /*
      * Clear the internal list of barcodes, using a semaphore to prevent simultaneous access.
      */

     semTake (pFlPriv->mutexSem, WAIT_FOREVER);

     for ( i=0; i > FL_MAX_FILTERS; i++)
     {
        pDevConfig->barcode[i] = FL_NO_BARCODE;
        pDevConfig->wheel[i] = 0;
        strcpy(pDevConfig->pos[i], "posX");
     }
     pDevConfig->characterized = FALSE;
     pDevConfig->highwater     = 0;
     pDevConfig->badBarcode    = FALSE;

     semGive (pFlPriv->mutexSem);

     return;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * flShowConfig
 *
 * INVOCATION:
 * status = flShowConfig (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 * Display the current filter barcode configuration database
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

long flShowConfig(ASSEMBLY_CONTROL_RECORD *par)
{
     FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/
     FL_DEV_CONFIG  *pDevConfig;
     long status = DAR_S_SUCCESS;	/* Return function status.	*/
     int i = 0;

     pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pFlPriv->pFilterPriv;

     FLDEBUG(DAR_MSG_MAX, "flShowConfig: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL || pDevConfig == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flShowConfig: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC || pDevConfig->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flShowConfig: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");
         return (status);
     }

    /*
     * Display the contents of the internal lookup table.
     */

     printf("Filter wheel configuration database");

     if (pDevConfig->highwater > 0)
     {
          if (par->mode == DAR_MODE_UPDATE && pFlPriv->updateMode)
          {
               printf(" - from QUICK update mode");
          }
          printf("\n");
          printf("--------------------------------------\n");

          for(i = 0; i < pDevConfig->highwater; i++)
          {
               printf("%10d %2d %s\n", pDevConfig->barcode[i], pDevConfig->wheel[i],
                  pDevConfig->pos[i]);
          }
     }
     else
     {
          printf(" is EMPTY.\n");
     }

     printf ("Database filled to %d entries. characterized flag is ", pDevConfig->highwater);
     if ( pDevConfig->characterized )
          printf("TRUE\n");
     else
          printf("FALSE\n");

     return(status);

}


/* Following function added by PBT 3/3/00                               */
/*
************************************************************************
 *+
 * FUNCTION NAME:
 * flProcessFault
 *
 * INVOCATION:
 * status = flProcessFault( par );
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
 * If command is active, empty the task list and finish command
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 * 
 *-
 ************************************************************************
 */

static long flProcessFault(ASSEMBLY_CONTROL_RECORD *par)
{
    FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/
    long status = DAR_S_SUCCESS;	/* Function status.		*/
    
    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flProcessFault: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flProcessFault: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");

         /* Don't return. Carry on and attempt the interlock. */
     }

/* Is a command active? If not, do nothing */

     if (pFlPriv->cmdActive)
     {

         /* Finish the command and empty the task list */

         FLDEBUG(DAR_MSG_ERROR, "flProcessFault: filter interlock occurred - aborting%c\n",' ');

         status = DAR_E_FLT_CLR;
         flTerminateTasks( par, status, "Filter interlock occurred" );
     }
     
     return ( status );
}


/* Following function added by CJT 28/4/00                               */
/*
************************************************************************
 *+
 * FUNCTION NAME:
 * flCheckInPosSwitches
 *
 * INVOCATION:
 * switchState = flCheckInPosSwitches( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) current switch state as follows
 *
 * Value    Filter 1 in position   Filter 2 in position
 * -----    --------------------   --------------------
 * 0x0      NO                     NO
 * 0x1      YES                    NO
 * 0x2      NO                     YES
 * 0x3      YES                    YES
 *
 * PURPOSE:
 * Read status of the two filter wheel in position switches
 *
 * DESCRIPTION:
 * Read status of the two filter wheel in position switches
 * and return the result as an encoded bit mask.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 * 
 *-
 ************************************************************************
 */

static long flCheckInPosSwitches(ASSEMBLY_CONTROL_RECORD *par)
{
     long switchState;

     switchState = (~((*(long *)FILT1_IN_POSITION_SW) | (*(long *)FILT2_IN_POSITION_SW << 1))) & 0x3;

     return (switchState); 
}


/* Following function added by SMB 8/12/00                               */
/*
************************************************************************
 *+
 * FUNCTION NAME:
 * flTerminateTasks
 *
 * INVOCATION:
 * flTerminateTasks( par, status, errMessage );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par        (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					      structure.
 * (>) status     (long)                      Completion status
 * (>) errMessage (char *)                    Error message
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * If command is active, empty the task list and finish command
 *
 * DESCRIPTION:
 * If command is active, empty the task list and finish command
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * DEFICIENCIES:
 * None known.
 * 
 *-
 ************************************************************************
 */

static void      flTerminateTasks
(
    ASSEMBLY_CONTROL_RECORD *par,		/* (in)  Ptr to ass rec.	*/
    long                    status,             /* Final status.                */
    char                    *errMessage         /* Error message.               */
)
{
    FL_DEV_PRIVATE *pFlPriv;		/* Ptr to private dev. struct.	*/
    long timerStatus = DAR_S_SUCCESS;   /* status word from assStopTimer  */

    pFlPriv = ( FL_DEV_PRIVATE *) assGetPrivateStruct( par );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pFlPriv == NULL )
     {
         FLDEBUG(DAR_MSG_FATAL, "flTerminateTasks: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return;
     }
     else if ( pFlPriv->magic != FL_MAGIC )
     {
         FLDEBUG(DAR_MSG_FATAL, "flTerminateTasks: No magic value seen in data structure%c\n", ' ' );

         status = FL_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");

         /* Don't return. Carry on and attempt to terminate the tasks. */
     }

     /*
      * Empty the filter task list.
      */

     flEmptyList( par, TRUE );

     /*
      * Stop the timer to prevent any further processing of the assembly record.
      * (Use a temporary status to avoid overwriting the final command status.)
      */

     assStopTimer( par, &timerStatus );
     if (timerStatus != DAR_S_SUCCESS)
     {
         FLDEBUG(DAR_MSG_WARNING, "flTerminateTasks: failed to stop watchdog timer. status=%ld\n",
            timerStatus);
     }

     /*
      * Set the flags in the device private structure to indicate completion of the command
      */

     semTake (pFlPriv->mutexSem, WAIT_FOREVER);
     pFlPriv->cmdActive = FALSE;
     semGive (pFlPriv->mutexSem);

     /*
      * Finally, tell the assembly record the command has finished, supplying the final
      * status and error message.
      */

     assCommandFinish( par, status, errMessage );

     return;
}

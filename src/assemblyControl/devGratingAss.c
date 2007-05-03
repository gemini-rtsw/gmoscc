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
 * devGratingAss.c
 *
 * PURPOSE:
 * EPICS Assembly Device Support code for the grating assemblyControl record.
 *
 * FUNCTION NAME(S)
 * grAckReceived	A busy link activated.
 * grBuildList		Build a task list.
 * grCheckAttributes	Check input attributes.
 * grCheckBarcodeId	Check returned barcode id.
 * grCheckBus		Check change in BUS values.
 * grDoTask		Setup to execute a task on the task list
 * grEmptyList		Delete from the task List
 * grExecuteTask	Execute a task on the task list.
 * grIndexMode		Execute INDEX mode command.
 * grInitDeviceSupport	Initialize instance of record's device support code.
 * grInitMode		Execute INIT mode command.
 * grMoveMode		Execute MOVE or PARK mode command.
 * grStopDirective	Execute STOP directive to attached records.
 * grTaskCheck		Check that the task succeeded.
 * grTaskFinished	Determine if a task list is finished.
 * grTestMode		Execute the TEST mode.
 * grTrackMode		Execute the TRACK mode.
 * grTranslateDir	Translate dir directive for device record.
 * grUpdateMode		Execute UPDATE mode command.
 * grProcessFault	Tidy up after fault (interlock occurred)
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.9  2005/02/23 01:50:03  gemvx
 * *** empty log message ***
 *
 * Revision 1.8  2004/12/17 03:42:20  gemvx
 * *** empty log message ***
 *
 * Revision 1.7  2004/06/07 19:55:35  gemvx
 * V5-1: bug fixes and grStep keyword added
 *
 * Revision 1.4  2003/07/21 21:37:20  gemvx
 * V4-2 follow in z only version
 *
 * Revision 1.3  2003/05/27 21:16:51  gemvx
 * Version 4-1
 *
 * Revision 1.2  2003/01/31 14:12:46  gemvx
 * Merged gmos-south
 *
 * Revision 1.1  2002/04/24 05:24:56  ajf
 * New directory for port to epics3.13.4GEM8.4.
 *
 * Revision 1.1  2001/11/28 20:08:46  mbec
 * *** empty log message ***
 *
 * Revision 1.3  2001/04/27 21:33:15  smb
 * TEST command now passed on to filter and grating devices.
 *
 * Revision 1.2  2001/04/23 18:24:42  smb
 * DEBUG macro changed from logMsg to printf so it can display floating point values (bug 196)
 *
 * Revision 1.58  2001/03/20 13:40:27  gmos
 * Modified DEBUG macro. All files now use printf() rather than logMsg(). All also print the output from taskName(0).
 *
 * Revision 1.57  2001/02/28 17:24:10  gmos
 * Renamed gmMessageLevels.h to darMessageLevels.h.
 *
 * Revision 1.56  2001/02/26 16:30:57  gmos
 * Removed CHECK_NULL macro and replaced it with explicit error reporting code.
 *
 * Revision 1.55  2001/02/23 18:13:21  gmos
 * Commenting improved. Discard old commented out code (except for thermal correction code). Include a printf within FL_CHECK_NULL and GR_CHECK_NULL just in case.
 *
 * Revision 1.54  2001/02/23 15:16:42  gmos
 * Increased GR_TILT_LOW_LIMIT to stop 0.0 degree tilt demands. Device was hitting limit switch.
 *
 * Revision 1.53  2001/02/08 13:36:09  gmos
 * Unexpected device acknowledgement is no longer a warning. Some really long lines wrapped at column 105.
 *
 * Revision 1.52  2001/02/08 09:52:29  gmos
 * Modified INDEX_VELOCITY for turret.
 *
 * Revision 1.51  2001/01/23 15:40:43  gmos
 * Added indexing velocity for grating turret. DoTask() now writes the turret velocity for every turret move.
 *
 * Revision 1.50  2000/12/13 17:50:47  gmos
 * Corrections to upper limit checking, select only flag checking and SAD writing by assembly.
 *
 * Revision 1.49  2000/12/13 15:06:08  gmos
 * Completed forwardlash implementation.
 *
 * Revision 1.48  2000/12/12 09:24:11  gmos
 * INIT modified so a non-existent barcode file is not regarded as a fatal error. I few more comments added. Status from assStartTimer checked.
 *
 * Revision 1.47  2000/12/08 16:20:00  gmos
 * Protect every write to device private data structure with a semaphore. grTerminateTasks function added to ensure timer is always stoppedand the task list is always emptied when a command finishes.
 *
 * Revision 1.46  2000/12/08 11:10:10  gmos
 * Switched over to use new message level scheme.
 *
 * Revision 1.45  2000/12/06 16:39:26  gmos
 * Half implemented forwardlash correction at grating assembly (tilt A only) and config parameters hard-coded into device support.
 *
 * Revision 1.44  2000/12/04 15:37:59  gmos
 * Tidied up function status return codes. Removed obsolete codes and task numbers. Added comments. Invalid barcode value changed from 0 to -1 and defined as a constant.
 *
 * Revision 1.43  2000/11/16 13:04:06  gmos
 * Magic number checks included.
 *
 * Revision 1.42  2000/11/16 10:43:31  gmos
 * Bug in initMode, in which data structure pointers were obtained in the wrong order, corrected. Checks on the return from assGetprivateStructure added throughout.
 *
 * Revision 1.41  2000/11/10 17:14:55  gmos
 * Fixed bug in device rejection code and improved clarity of device failure messages.
 *
 * Revision 1.40  2000/10/26 10:15:41  gmos
 * Removed some out of date source comments.
 *
 * Revision 1.39  2000/10/25 15:33:16  gmos
 * Save the grating database after every grating move.
 *
 * Revision 1.38  2000/10/11 14:16:40  gmos
 * Use named load positions from the turret lookup table.
 *
 * Revision 1.37  2000/09/26 15:26:14  gmos
 * Default turret velocity changed from 3.75 to 2.5 teeth per second.
 *
 * Revision 1.36  2000/09/19 14:12:41  cjt
 * Added support to PARK the grating assembly at grating loading/unloading
 * positions.
 *
 * Revision 1.35  2000/09/05 12:06:33  gmos
 * Inconsistent tabs removed from source code.
 *
 * Revision 1.34  2000/08/16 10:56:29  gmos
 * Fixed a few typos. Replaced the last srp logMsg with a GRDEBUG message
 *
 * Revision 1.33  2000/07/18 13:32:44  cjt
 * Spelling mistake.
 *
 * Revision 1.32  2000/07/18 10:59:43  gmos
 * Grating PARK position changed from grating A to grating B to reflect new turret configuration.
 *
 * Revision 1.31  2000/07/18 09:30:27  cjt
 * Added checking of keywords on first line of lookup tables.
 *
 * Revision 1.30  2000/07/14 14:22:54  cjt
 * Fixed the MOVE while MOVING bugs in xxCheckAttributes.
 *
 * Revision 1.29  2000/07/14 08:40:02  gmos
 * Removed recGblRecordError calls when there is a file I/O rather than a record error. Translation stage will now report when it cant open a LUT file.
 *
 * Revision 1.28  2000/07/10 16:33:38  cjt
 * Added error message in DoTask() for failure to WriteConfig().
 *
 * Revision 1.27  2000/07/10 14:12:24  gmos
 * Fixed typo. GRDEBUG rather than FLDEBUG
 *
 * Revision 1.26  2000/07/10 14:04:13  gmos
 * Fixed problems reporting file I/O error messages. Set keepIndex to keep index after soft error.
 *
 * Revision 1.25  2000/07/10 13:25:00  cjt
 * Fixed stopDirective race condition bug.
 *
 * Revision 1.24  2000/07/06 17:27:04  gmos
 * strcpy replaced by strncpy. Additional status checks after reading barcode. Serious lack of check for lifter status fixed.
 *
 * Revision 1.23  2000/07/03 15:54:22  cjt
 * Added checks to return values of some recGbl calls.
 *
 * Revision 1.22  2000/06/28 14:06:33  gmos
 * Changed output of grating tilt to SAD from assembly VALC to SOV link.
 *
 * Revision 1.21  2000/06/26 13:34:59  gmos
 * Current grating tilt/ID zeroed on INIT, INDEX or PARK. DEBUG replaced by GRDEBUG and logMsg removed from DEBUG macro.
 *
 * Revision 1.20  2000/06/15 14:34:30  cjt
 * Corrected incorrect debug message.
 *
 * Revision 1.19  2000/06/13 10:15:23  cjt
 * Removed some commented printf()s.
 *
 * Revision 1.18  2000/06/08 10:45:56  cjt
 * Updated references to (ASSEMBLY_CONTROL_RECORD *) from "pdr" to "par".
 *
 * Revision 1.17  2000/06/07 12:51:42  gmos
 * Display name of backup LUT file on console. Also corrected turret position expected after PARK.
 *
 * Revision 1.16  2000/06/06 14:07:31  cjt
 * Fixed STOP directive "invalid task" bug.
 *
 * Revision 1.15  2000/06/06 10:49:20  cjt
 * Added support for lookup table backups. And Quick Update in devFilterAss.c
 *
 * Revision 1.14  2000/06/05 16:39:00  gmos
 * Go back to triggering the barcode even in simulation mode. The barcode software itself can now simulate
 *
 * Revision 1.13  2000/05/18 17:00:42  gmos
 * Removed redundant devFilterAss.h devGratingAss.h and devTranslationAss.h files
 *
 * Revision 1.12  2000/05/18 09:01:11  gmos
 * GR_ prefix added to some symbolic constants
 *
 * Revision 1.11  2000/05/15 16:49:05  gmos
 * Lookup table code tidied up and now assumes arrays start from index
 *
 * Revision 1.10  2000/05/11 12:55:56  gmos
 * gratingLib renamed gmosLutLib
 *
 * Revision 1.9  2000/05/10 08:16:59  gmos
 * Grating lookup tables filled from index 1. Check lookup tables are monotonic before using them.
 *
 * Revision 1.8  2000/05/03 16:14:36  cjt
 * Improved in-position switch checking for filter and grating assemblies. Files
 * devGratingAss.c (.h), devFilterAss.c (.h) and filter.sch modified.
 *
 * Revision 1.7  2000/04/14 10:00:36  gmos
 * Merged pre and post FP software
 *
 * Revision 1.6  2000/04/03 16:37:53  cjt
 * Changed name of second grating assy lut from gr.lut_aux to aux_gr.lut. Updated grating assy device support.
 *
 * Revision 1.5  2000/04/03 15:43:05  cjt
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
 * Revision 1.4  2000/02/24 16:48:22  gmos
 * Fixed bug in grating UPDATE mode. Incorrect turret barcode reading positions were being selected.
 *
 * Revision 1.3  2000/02/11 12:25:44  gmos
 * Ensure the task list is empty before building the list during INIT
 *
 * Revision 1.2  2000/01/13 12:41:15  gmos
 * List of remaining bugs added. Do not lower turret after characterising. Use simulated barcodes in simulation mode. Attempt to rename LUT file before overwriting it. Set characterized flag when barcode obtained.
 *
 * Revision 1.1  1999/12/10 14:33:42  gmos
 * Contents of assembly modules merged into gmoscc
 *
 * Revision 1.22  1999/12/02 15:42:55  gmos
 * Modified so velocity is that tilts and turret do not have their velocities reset on INDEX or INIT mode.
 *
 * Revision 1.21  1999/11/30 14:05:48  gmos
 * *** empty log message ***
 *
 * Revision 1.20  1999/11/30 13:46:30  gmos
 * Added UKATC banner
 *
 * Revision 1.19  1999/11/24 12:17:24  gmos
 * Make sure all error messages are reported with DAR_DEBUG_NONE to ensure thet are not lost
 *
 * Revision 1.18  1999/11/23 16:20:48  gmos
 * Modified to use symbolic constants in lifter.h. Turret drives to position A after indexing rather than D. Turret timeout increased.
 *
 * Revision 1.17  1999/10/28 11:32:16  dlt
 * Lower turret after all movements
 *
 * Revision 1.16  1999/10/28 11:23:25  dlt
 * Change grating tilts to degrees
 *
 * Revision 1.15  1999/10/22 13:23:57  dlt
 * Implement tilt SAD records
 *
 * Revision 1.14  1999/10/21 12:58:18  dlt
 * remove redundant code
 *
 * Revision 1.13  1999/10/12 15:25:38  dlt
 * Tidy up prints
 *
 * Revision 1.12  1999/10/12 12:28:53  dlt
 * Keep track of turret position
 *
 * Revision 1.11  1999/10/11 14:21:53  dlt
 * Rename gratings to A..D
 *
 * Revision 1.10  1999/10/08 14:00:45  dlt
 * Use demand position instead of switches for barcode position
 *
 * Revision 1.9  1999/10/07 15:17:10  dlt
 * Set correct velocity for turret
 *
 * Revision 1.8  1999/10/04 13:39:31  dlt
 * Output current grating ID
 *
 * Revision 1.5  1999/09/28 12:49:09  dlt
 * Complete UPDATE code
 *
 * Revision 1.4  1999/09/23 11:10:53  srp
 * Modifications to include the characterisation functions
 *
 * Revision 1.3  1999/07/23 15:02:22  srp
 * Add support for barcode reader and pneumatic lifter. Add preliminary characterisation sequencing
 *
 * Revision 1.1  1999/06/23 13:08:33  scs
 * Cloned from mask assembly device support with modification to support preliminary grating sequencing
 *
 * Revision 1.1  1999/06/01 13:03:41  gmos
 * First version received from DAO
 *
 * Revision 1.2  1999/05/07 17:10:23  dunn
 * Init and Index working.
 *
 * Revision 1.1  1999/05/03 22:34:36  dunn
 * Initial revision
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
#include        <stdio.h>
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
#include        <tickLib.h>		/* Contains tickGet function used by DEBUG macro */
#include        <sysLib.h>		/* Contains sysClkRateGet.                       */
#include        <ellLib.h>		/* Contains ellList linked list stuff.           */

#include        <assemblyControlRecord.h> /* Assembly record general header.             */
#include        <assemblyControl.h>	  /* Assembly record record support header.        */
#include	<darMessageLevels.h>      /* GMOS message level definitions.               */


#include        <timeLib.h>		/* Gemini time library                           */
#include        <lifter.h>		/* Grating lifter device codes.                  */
#include        <gmosLutLib.h>          /* GMOS lookup table library.                    */


/*
 *  Local Defines
 */
#define GR_LUT_FIRST_STRING     "grating"	/* The first line of the grating lookup		*/
						/* table file must contain this string.		*/
#define GR_AUX_LUT_FIRST_STRING "aux_grating"	/* The first line of the auxilliary lookup	*/
						/* table file must contain this string.		*/

#define GR_SELECT_ONLY		1		/* Option for selecting a grating only		*/
#define GR_SELECT_AND_MOVE	0		/* Option for selecting and tilting		*/
#define GR_TILT_LO_LIMIT	0.5		/* Minimum grating tilt in degrees		*/
#define GR_TILT_HI_LIMIT	89.5		/* Maximum grating tilt in degrees		*/

#define GR_BARCODE_MAX		39999999        /* Maximum allowable grating barcode		*/
#define GR_BARCODE_MIN		30000000	/* Minimum allowable grating barcode		*/
#define GR_BARCODE_MAX_SIM	59999999	/* Maximum simulated barcode			*/
#define GR_BARCODE_MIN_SIM	50000000	/* Minimum simulated barcode			*/
#define GR_NO_BARCODE		-1		/* Code for undefined barcode.			*/

#define GR_NUM_GRATINGS		4               /* Number of gratings				*/
#define GR_NUM_DEVICES		5		/* Number of devices (gratings plus turret)	*/

#define GR_VELOCITY_GRATING	500.0		/* Grating velocity in steps/sec.		*/
#define GR_VELOCITY_TURRET	  5.0		/* Turret velocity in teeth/sec.		*/
#define GR_VELOCITY_TURRET_INDEX  5.0		/* Turret indexing velocity in teeth/sec.	*/
#define GR_VELOCITY_NONE         -1.0           /* Flag which means don't change the velocity	*/
#define GR_COMMAND_TIMEOUT	  100		/* Normal command timeout in seconds.		*/
#define GR_COMMAND_SHORT_TIMEOUT   10		/* Short command timeout in seconds.		*/
#define GR_POWER_TIMEOUT	    1		/* Turret power on/off timeout.			*/
#define GR_BARCODE_READ_DELAY       2		/* seconds to wait for barcode.                 */

#define GR_ERROR_MESSAGE_SIZE     128		/* Size of error message buffer  		*/
#define GR_SCRATCH_BUFFER_SIZE    256		/* Size of scratch string buffer 		*/

/*
 * The following status codes supplement the DAR_ codes within recAssControl.h
 */

#define GR_LAST_TASK		 302		/* Return status - last task executed.		*/
#define GR_EMPTY_LIST		 303		/* Return status - all tasks complete.		*/

#define GR_BAD_STRUCTURE        -301            /* Return status - corrupted data structure	*/
#define GR_BAD_DEV_CONNECT	-302		/* Return status - device not connected.	*/
#define GR_TASK_LIST_EMPTY	-303		/* Return status - task list empty.		*/
#define GR_TASK_LIST_NOT_EMPTY	-304		/* Return status - task list not empty.		*/
#define GR_INVALID_TASK		-305		/* Return status - invalid task number.		*/
#define GR_INVALID_LOCATION	-306		/* Return status - invalid grating location.	*/
#define GR_INVALID_BARCODE	-307		/* Return status - invalid barcode.		*/
#define GR_UNKNOWN_GRATING	-308		/* Return status - unknown grating.		*/
#define GR_EMPTY_BARCODE_LIST	-309		/* Return status - empty barcode list.		*/
#define GR_TRIG_FAILED		-310		/* Return status - failed to trigger device.	*/
#define GR_LIFTER_FAILED        -311		/* Return status - lifter operation failed.	*/
#define GR_POWER_FAILED         -312		/* Return status - turret power control failed.	*/
#define GR_TURRET_ERROR         -313		/* Return status - turret positioning error	*/
#define GR_FILEIO_ERROR         -314		/* Return status - file open/read/write failed.	*/
#define GR_LUT_ERROR            -315            /* Return status - LUT calibration failed.      */

#define GR_ENGAGED              0               /* turret safely lowered and engaged		*/
#define GR_DISENGAGED           1               /* turret safely raised				*/
#define GR_TURRET_INDETERMINATE 2               /* turret not in defined position		*/

/*
 *  Defines of the named positions in the device records.
 */

#define GR_NAME_POS_INDEX	"index"		/* VALS of index.	*/
#define GR_NAME_POS_PARK	"park"		/* VALS of park.	*/
#define GR_NAME_POS_BC  	"barcode"	/* VALS of grating tilt barcode reading position. */
#define GR_NAME_POS_POSA	"posA"		/* VALS of position A.	*/
#define GR_NAME_POS_POSB	"posB"		/* VALS of position B.	*/
#define GR_NAME_POS_POSC	"posC"		/* VALS of position C.	*/
#define GR_NAME_POS_POSD	"posD"		/* VALS of position D.	*/
#define GR_NAME_POS_POSBCA	"posABarcode"	/* VALS of barcode reading position A.	*/
#define GR_NAME_POS_POSBCB	"posBBarcode"	/* VALS of barcode reading position B.	*/
#define GR_NAME_POS_POSBCC	"posCBarcode"	/* VALS of barcode reading position C.	*/
#define GR_NAME_POS_POSBCD	"posDBarcode"	/* VALS of barcode reading position D.	*/
#define GR_NAME_POS_POSLDA	"posALoad"	/* VALS of loading position A.	*/
#define GR_NAME_POS_POSLDB	"posBLoad"	/* VALS of loading position B.	*/
#define GR_NAME_POS_POSLDC	"posCLoad"	/* VALS of loading position C.	*/
#define GR_NAME_POS_POSLDD	"posDLoad"	/* VALS of loading position D.	*/

/*
 *  Define device names.
 */

#define GRA 0
#define GRB 1
#define GRC 2
#define GRD 3
#define TRT 4

#define GR_DEV_NAME_GRA         "gratingA"      /* grating1 device.     */
#define GR_DEV_NAME_GRB         "gratingB"      /* grating2 device.     */
#define GR_DEV_NAME_GRC         "gratingC"      /* grating3 device.     */
#define GR_DEV_NAME_GRD         "gratingD"      /* grating4 device.     */
#define GR_DEV_NAME_TRT         "turret"        /* turret device.       */

/*
 *  Define the attributes in and input/output sensors.
 */

#define GRAT_BARCODE_READ_FIELD (char *)par->sij    /* Ptr. to grating barcode value read.    */
#define GRAT_BARCODE_TRIGGER    par->sor            /* Ptr. to grating barcode link.          */

#define GR_LIFTER_STATE         par->sik            /* Ptr. to lifter state port.             */
#define GR_LIFTER_CMD           par->sos            /* Ptr. to lifter raise command port.     */

#define GR_TURRET_POWER         par->sou            /* Ptr. to turret power port.             */
#define GR_TURRET_POWER_STATE   par->sim            /* Ptr. to turret power input port.       */
#define GR_SITE_CODE		par->sin	    /* Ptr. to site code MK=0, CP=1 */

#define GR_TILT_VALUE           par->sov            /* Ptr. to grating tilt output - for SAD. */
#define GR_STEP_VALUE           *(double *)par->valc    /* Ptr. to grating tilt output - for SAD. */

#define GR_TURRET_POWER_ON_VAL  1
#define GR_TURRET_POWER_OFF_VAL 0
#define GR_TURRET_POWER_IS_ON   1
#define GR_TURRET_POWER_IS_OFF  0

#define GR_MAX_NAMES_PER_DEVICE 3

/*
 * Device control record link structure.  Keeps all of the links for each
 * attached device in one place for ease of reference.
 */

typedef struct grDevInfo {
     struct link		*dirLink;		/* Dir's links.			*/
     struct link		*modLink;		/* Mod's links.			*/
     struct link		*posLink;		/* Pos's links.			*/
     struct link		*velLink;		/* Vel's links.			*/
     struct link		*ackLink;		/* Acks's links.		*/
} GR_DEV_INFO;

/*
 *  Task List structure.  Warning, static GR_TASK_LISTs below are dependent 
 *  on the order of variables in this structure.
 */

typedef struct taskList {
    int			task;			/* Task to complete.			*/
    int			grAResponse;		/* Response expected on BUS1 field.	*/
    int			grBResponse;		/* Response expected on BUS2 field.	*/
    int			grCResponse;		/* Response expected on BUS3 field.	*/
    int			grDResponse;		/* Response expected on BUS4 field.	*/
    int			trtResponse;		/* Response expected on BUS5 field.	*/
} GR_TASK_LIST;

/*
 *  Task list node structure.  A node is added to the end of the list for each
 *  task to perform for the current command and removed from the head of the
 *  list when it is completed during execution of the command.
 */

typedef struct grList {
    ELLNODE		node;			/* Ptr. to next task list node.	*/
    void		*item;			/* Ptr. to item list.		*/
} GR_LIST;


/*
 * Define the primitive tasks the assembly can be asked to do.
 * These tasks may be combined together to define how to respond
 * to particular commands.
 */

typedef enum
{
    GR_STOP_TASK = 0,		/* Stop everything. 				*/
    GR_INIT_ALL,		/* Initialise everything.			*/
    GR_TEST_ALL,		/* Test everything.				*/
    GR_INDEX_ALL,		/* Index all devices.				*/
    GR_PARK_ALL,		/* Park all devices.				*/
    GR_MV_GRATING_ALL,		/* Move all gratings simultaneously.		*/
    GR_BC_GRATING_ALL,		/* Move all gratings to barcode position.	*/
    GR_MV_GRATING_A_1,		/* Tilt grating A only - first part of move.	*/
    GR_MV_GRATING_A_2,		/* Tilt grating A only - second part of move.	*/
    GR_MV_GRATING_A_3,		/* Tilt grating A only - third part of move.	*/
    GR_BC_GRATING_A,		/* Tilt grating A to barcode read position.	*/
    GR_PARK_GRATING_A,		/* Tilt grating A to park position.		*/
    GR_MV_GRATING_B_1,		/* Tilt grating B only - first part of move.    */
    GR_MV_GRATING_B_2,		/* Tilt grating B only - second part of move.   */
    GR_MV_GRATING_B_3,		/* Tilt grating B only - third part of move.    */
    GR_BC_GRATING_B,		/* Tilt grating B to barcode read position.	*/
    GR_PARK_GRATING_B,		/* Tilt grating B to park position.		*/
    GR_MV_GRATING_C_1,		/* Tilt grating C only - first part of move.    */
    GR_MV_GRATING_C_2,		/* Tilt grating C only - second part of move.   */
    GR_MV_GRATING_C_3,		/* Tilt grating C only - third part of move.    */
    GR_BC_GRATING_C,		/* Tilt grating C to barcode read position.	*/
    GR_PARK_GRATING_C,		/* Tilt grating C to park position.		*/
    GR_MV_GRATING_D_1,		/* Tilt grating D only - first part of move.    */
    GR_MV_GRATING_D_2,		/* Tilt grating D only - second part of move.   */
    GR_MV_GRATING_D_3,		/* Tilt grating D only - third part of move.    */
    GR_BC_GRATING_D,		/* Tilt grating D to barcode read position.	*/
    GR_PARK_GRATING_D,		/* Tilt grating D to park position.		*/
    GR_MV_TURRET_A,		/* Move turret to grating A position.		*/
    GR_MV_TURRET_B,		/* Move turret to grating B position.		*/
    GR_MV_TURRET_C,		/* Move turret to grating C position.		*/
    GR_MV_TURRET_D,		/* Move turret to grating D position.		*/
    GR_MV_TURRET_BCA,		/* Move turret to read grating A's barcode.	*/
    GR_MV_TURRET_BCB,		/* Move turret to read grating B's barcode.	*/
    GR_MV_TURRET_BCC,		/* Move turret to read grating C's barcode.	*/
    GR_MV_TURRET_BCD,		/* Move turret to read grating D's barcode.	*/
    GR_PARK_TURRET,		/* Move turret to park position.		*/
    GR_TURRET_UP,		/* Raise the lifter and release the turret.	*/
    GR_TURRET_DOWN,		/* Lower the lifter and engage the turret.	*/
    GR_READ_BARCODE,		/* Read the grating barcode.			*/
    GR_WRITE_CONFIG_BAK,	/* Write grating config file with backup.	*/
    GR_WRITE_CONFIG,		/* Write grating config file without backup.	*/
    GR_INDEX_TURRET,		/* Index the grating turret only.		*/
    GR_INDEX_GRATING_A,		/* Index grating A only.			*/
    GR_INDEX_GRATING_B,		/* Index grating B only.			*/
    GR_INDEX_GRATING_C,		/* Index grating C only.			*/
    GR_INDEX_GRATING_D,		/* Index grating D only.			*/
    GR_TURRET_POWER_OFF,	/* Switch turret power off.			*/
    GR_TURRET_POWER_ON		/* Switch turret power on,			*/
} tGrTaskList;


/*
 *  Create static list of primitive tasks to perform for each high level operation.
 */

static GR_TASK_LIST  grInit[] = {					/* INIT */
    { GR_INIT_ALL,           TRUE,  TRUE,  TRUE,  TRUE,  TRUE } ,
};
static GR_TASK_LIST  grTest[] = {					/* TEST */
    { GR_TEST_ALL,           TRUE,  TRUE,  TRUE,  TRUE,  TRUE } ,
};
static GR_TASK_LIST  grIndexAll[] = {					/* INDEX */
     { GR_TURRET_POWER_ON,   FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_UP,         FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_INDEX_ALL,         TRUE,  TRUE,  TRUE,  TRUE,  TRUE  } ,
     { GR_MV_TURRET_B,       FALSE, FALSE, FALSE, FALSE, TRUE  } ,
};
static GR_TASK_LIST  grParkAll[] = {					/* PARK */
     { GR_TURRET_POWER_ON,   FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_UP,         FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_PARK_ALL,          TRUE,  TRUE,  TRUE,  TRUE,  TRUE  } ,
     { GR_TURRET_DOWN,       FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_POWER_OFF,  FALSE, FALSE, FALSE, FALSE, FALSE } ,
};
static GR_TASK_LIST  grMoveGratA[] = {					/* Tilt A */
     { GR_MV_GRATING_A_1,    TRUE,  FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_A_2,    TRUE,  FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_A_3,    TRUE,  FALSE, FALSE, FALSE, FALSE } ,
};
static GR_TASK_LIST  grMoveGratB[] = {					/* Tilt B */
     { GR_MV_GRATING_B_1,    FALSE, TRUE,  FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_B_2,    FALSE, TRUE,  FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_B_3,    FALSE, TRUE,  FALSE, FALSE, FALSE } ,
};
static GR_TASK_LIST  grMoveGratC[] = {					/* Tilt C */
     { GR_MV_GRATING_C_1,    FALSE, FALSE, TRUE,  FALSE, FALSE } ,
     { GR_MV_GRATING_C_2,    FALSE, FALSE, TRUE,  FALSE, FALSE } ,
     { GR_MV_GRATING_C_3,    FALSE, FALSE, TRUE,  FALSE, FALSE } ,
};
static GR_TASK_LIST  grMoveGratD[] = {					/* Tilt D */
     { GR_MV_GRATING_D_1,    FALSE, FALSE, FALSE, TRUE,  FALSE } ,
     { GR_MV_GRATING_D_2,    FALSE, FALSE, FALSE, TRUE,  FALSE } ,
     { GR_MV_GRATING_D_3,    FALSE, FALSE, FALSE, TRUE,  FALSE } ,
};
static GR_TASK_LIST  grMovePosA[] = {					/* Select and tilt A */
     { GR_TURRET_POWER_ON,   FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_UP,         FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_TURRET_A,       FALSE, FALSE, FALSE, FALSE, TRUE  } ,
     { GR_TURRET_DOWN,       FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_POWER_OFF,  FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_A_1,    TRUE,  FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_A_2,    TRUE,  FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_A_3,    TRUE,  FALSE, FALSE, FALSE, FALSE } ,
     { GR_WRITE_CONFIG,      FALSE, FALSE, FALSE, FALSE, FALSE } ,
};
static GR_TASK_LIST  grMovePosB[] = {					/* Select and tilt B */
     { GR_TURRET_POWER_ON,   FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_UP,         FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_TURRET_B,       FALSE, FALSE, FALSE, FALSE, TRUE  } ,
     { GR_TURRET_DOWN,       FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_POWER_OFF,  FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_B_1,    FALSE, TRUE,  FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_B_2,    FALSE, TRUE,  FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_B_3,    FALSE, TRUE,  FALSE, FALSE, FALSE } ,
     { GR_WRITE_CONFIG,      FALSE, FALSE, FALSE, FALSE, FALSE } ,
};
static GR_TASK_LIST  grMovePosC[] = {					/* Select and tilt C */
     { GR_TURRET_POWER_ON,   FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_UP,         FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_TURRET_C,       FALSE, FALSE, FALSE, FALSE, TRUE  } ,
     { GR_TURRET_DOWN,       FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_POWER_OFF,  FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_C_1,    FALSE, FALSE, TRUE,  FALSE, FALSE } ,
     { GR_MV_GRATING_C_2,    FALSE, FALSE, TRUE,  FALSE, FALSE } ,
     { GR_MV_GRATING_C_3,    FALSE, FALSE, TRUE,  FALSE, FALSE } ,
     { GR_WRITE_CONFIG,      FALSE, FALSE, FALSE, FALSE, FALSE } ,
};
static GR_TASK_LIST  grMovePosD[] = {					/* Select and tilt D */
     { GR_TURRET_POWER_ON,   FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_UP,         FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_TURRET_D,       FALSE, FALSE, FALSE, FALSE, TRUE  } ,
     { GR_TURRET_DOWN,       FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_POWER_OFF,  FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_GRATING_D_1,    FALSE, FALSE, FALSE, TRUE,  FALSE } ,
     { GR_MV_GRATING_D_2,    FALSE, FALSE, FALSE, TRUE,  FALSE } ,
     { GR_MV_GRATING_D_3,    FALSE, FALSE, FALSE, TRUE,  FALSE } ,
     { GR_WRITE_CONFIG,      FALSE, FALSE, FALSE, FALSE, FALSE } ,
};
static GR_TASK_LIST  grCharacterise[] = {				/* UPDATE */
     { GR_TURRET_POWER_ON,   FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_TURRET_UP,         FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_BC_GRATING_ALL,    TRUE,  TRUE,  TRUE,  TRUE,  FALSE } ,
     { GR_MV_TURRET_BCA,     FALSE, FALSE, FALSE, FALSE, TRUE  } ,
     { GR_READ_BARCODE,      FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_TURRET_BCB,     FALSE, FALSE, FALSE, FALSE, TRUE  } ,
     { GR_READ_BARCODE,      FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_TURRET_BCC,     FALSE, FALSE, FALSE, FALSE, TRUE  } ,
     { GR_READ_BARCODE,      FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_MV_TURRET_BCD,     FALSE, FALSE, FALSE, FALSE, TRUE  } ,
     { GR_READ_BARCODE,      FALSE, FALSE, FALSE, FALSE, FALSE } ,
     { GR_WRITE_CONFIG_BAK,  FALSE, FALSE, FALSE, FALSE, FALSE } ,
};

/*
 *  Device support function prototypes
 */

static long grAckReceived( ASSEMBLY_CONTROL_RECORD * );
static long grCheckAttributes( ASSEMBLY_CONTROL_RECORD * );
static long grIndexMode (ASSEMBLY_CONTROL_RECORD * );
static long grInitDeviceSupport( ASSEMBLY_CONTROL_RECORD * );
static long grInitMode (ASSEMBLY_CONTROL_RECORD * );
static long grMoveMode (ASSEMBLY_CONTROL_RECORD * );
static long grStopDirective( ASSEMBLY_CONTROL_RECORD * );
static long grTestMode (ASSEMBLY_CONTROL_RECORD * );
static long grTrackMode (ASSEMBLY_CONTROL_RECORD * );
static long grUpdateMode (ASSEMBLY_CONTROL_RECORD * );
static long grProcessFault (ASSEMBLY_CONTROL_RECORD * );


/*
 * Internal function prototypes
 */
  
static long grBuildList( ASSEMBLY_CONTROL_RECORD *, int ); 
static long grCheckBarcodeId( ASSEMBLY_CONTROL_RECORD * );
static long grCheckBus( ASSEMBLY_CONTROL_RECORD *, const int, int );
static void grClearBarcodes(ASSEMBLY_CONTROL_RECORD *par);
static void grReadZPC(ASSEMBLY_CONTROL_RECORD *par);
static long grDoTask( ASSEMBLY_CONTROL_RECORD * );
static long grEmptyList( ASSEMBLY_CONTROL_RECORD *, int );
static long grExecuteTask( ASSEMBLY_CONTROL_RECORD *, GR_TASK_LIST *);
static long grTranslateDir( unsigned short );
static long grTaskFinished( ASSEMBLY_CONTROL_RECORD * );
static long grTaskCheck( ASSEMBLY_CONTROL_RECORD * );
static void grTerminateTasks( ASSEMBLY_CONTROL_RECORD *, long, char *);
static long grReadConfig(ASSEMBLY_CONTROL_RECORD *par);
static long grShowConfig(ASSEMBLY_CONTROL_RECORD *par);
static long grReadSwitches(ASSEMBLY_CONTROL_RECORD *par);
static long grWriteConfig(ASSEMBLY_CONTROL_RECORD *par, const int savebackup);

/*
 *  Data Structures - device support entry table.
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
    DEVSUPFUN       processFault;         /* Added 1/3/00 PBT */
} devGratingAssembly = {
    14,
    NULL,
    NULL,
    grInitDeviceSupport,
    NULL,
    grCheckAttributes,
    grStopDirective,
    grInitMode,
    grMoveMode,
    grTrackMode,
    grIndexMode,
    grTestMode,
    grAckReceived,
    grUpdateMode,
    grProcessFault                        /* Added 1/3/00 PBT */
    };  

/*
 *  Internal private device support structures.
 */

typedef struct devConfig {
                                        /* Array of barcodes for each grating.                  */
     int                barcode[GR_NUM_GRATINGS];

                                        /* Array of old grating tilts.                          */
     float              oldGratTilt[GR_NUM_GRATINGS];

                                        /* Array of new grating tilts.                          */
     float              newGratTilt[GR_NUM_GRATINGS];
     int                characterized;  /* Configuration database is valid?                     */
     int                selectOnlyFlag; /* Grating is being selected without changing tilt?     */
     int                newTurretPos;   /* New turret position.                                 */
     int                currentTurretPos; /* Current turret position.                           */
     double             tiltScale;      /* Degrees per motor step when no LUT available.        */
     double             backlash;       /* Amount of grating tilt backlash.                     */
     double             forwardlash;    /* Movement required to release contact with worm gear. */
     double             zpc[GR_NUM_GRATINGS];            /* Zero point correction in motorsteps */
     double             numberGrating;  /* numberGrating                                        */
     long               magic;          /* magic value to guard against pointer corruption      */
} GR_DEV_CONFIG;

typedef struct {
     char		position[GR_NUM_DEVICES][MAX_STRING_SIZE]; /* Positions to go to.	*/
     double		velocity[GR_NUM_DEVICES];     /* Velocities to move at.			*/
     int 		currentCmd;	/* Current command executing.				*/
     double		askBarCode;	/* Asked for a barcode.					*/
     int		raiseTurret;	/* Asked to raise/lower turret  			*/
     int		turretPowerChange;	/* Asked to change the turret motor power	*/
     int                parkPosition;   /* Which grating do we want to load ?                   */
     int		mode;		/* Mode to execute.					*/
     int		busyMode;	/* Current busy mode.					*/
     int		grAFinished;	/* Device 1 finished.					*/
     int		grBFinished;	/* Device 2 finished.					*/
     int		grCFinished;	/* Device 3 finished.					*/
     int		grDFinished;	/* Device 4 finished.					*/
     int		trtFinished;	/* Device 5 finished.					*/
     int		cmdActive;	/* Command is activated.				*/
     SEM_ID             mutexSem;	/* mutual exclusion semaphore				*/
     ASSEMBLY_CONTROL_RECORD *pRecord;	/* Pointer to calling record structure			*/
     GR_DEV_INFO	grADevice;	/* Grating1 info. structure				*/
     GR_DEV_INFO	grBDevice;	/* Grating2 info. structure				*/
     GR_DEV_INFO	grCDevice;	/* Grating3 info. structure				*/
     GR_DEV_INFO	grDDevice;	/* Grating4 info. structure				*/
     GR_DEV_INFO	trtDevice;	/* Turret info.	structure				*/
     ELLLIST		taskList;	/* List of things to do.				*/
     GR_DEV_CONFIG	*pGratingPriv;  /* Pointer to private grating state structure		*/
     gmosLookupTable	tilt2StepsLut[GR_NUM_GRATINGS];
                                        /* Lookup tables for the tilt to step count translation */
     long               magic;          /* magic value to guard against pointer corruption      */
} GR_DEV_PRIVATE;

#define GR_MAGIC  0x20206772            /* magic value for grating data structures.             */

/*
 * Site names
 */
#define	SITE_MK		0		/* Mauna Kea */
#define SITE_CP		1		/* Cerro Pachon */


/*
 *  Macros
 *
 *  DEBUG   - Logs a message if the debug level of the message is less than the current
 *            debugging mode. The time and record name are prepended.
 *
 *  GRDEBUG - A grating-specific wrapper for the DEBUG macro which avoids repeating
 *            the "<%ld> %s:" format throughout the code.
 */

#define DEBUG(l,FMT,V)						\
{								\
    int k=l;							\
    if (k <= par->dbug)						\
    {								\
        printf ("%s: "FMT, taskName(0), tickGet(), par->name, (V));		\
    }								\
}

#define GRDEBUG(l,FMT,V)					\
{								\
    DEBUG (l, "<%ld> %s:" FMT, V); \
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grAckReceived
 *
 * INVOCATION:
 * status = grAckReceived( par ); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Handle change in device control record state or watchdog callback.
 *
 * DESCRIPTION:
 * This function is called when the Assembly Control Record detects
 * that an attached device control record has either started or
 * completed a command, through a change to one of the BUS1-BUS5 fields.
 * The function is also called whenever the watchdog timer expires after
 * waiting for a lifter move or a barcode read, for example. Handling this
 * state change is accomplished via the following algorithm:
 *
 *   if a command is active then
 *   	ignore this
 *   else if we asked for a barcode ID, then
 *      then this task has finished, so call grTaskFinished()
 *   else if we asked for a raise/lower operation, then
 *      then this task has finished, so call grTaskFinished()
 *   else if we timed out, then
 *	indicate the command finished by called grTerminateTasks(), supplying
 *	an error message.
 *   else if any of the BUSx fields have a response then
 *    	For any lines that are set, then call grCheckBus() to handle
 *	the change.
 *   otherwise an error
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * Assumes that BUS1=GR1, BUS2=GR2, BUS3=GR3, BUS4=GR4, BUS5=TRT
 *
 * DEFICIENCIES:
 * Completion of a barcode read or a turret raise/lower operation is
 * assumed after a timeout. It would be nicer if the completion of the
 * lifter operation could be signalled through a BUSY/IDLE signal in
 * the same way as the motor devices. SMB - 23 Nov 1999.
 *-
 ************************************************************************
 */

static long grAckReceived(ASSEMBLY_CONTROL_RECORD *par)
{
     GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/
     GR_DEV_CONFIG  *pDevConfig;
     long status = DAR_S_SUCCESS;	/* Function status.		*/
     int bus1;				/* Grating A command state.	*/
     int bus2;				/* Grating B command state.	*/
     int bus3;				/* Grating C command state.	*/
     int bus4;				/* Grating D command state.	*/
     int bus5;				/* Grating turret command state.*/

     pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pGrPriv->pGratingPriv;

     GRDEBUG(DAR_MSG_MAX, "grAckReceived: entry, cmdActive=%d\n", pGrPriv->cmdActive );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL || pDevConfig == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grAckReceived: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grAckReceived: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

    /*
     *  Ask assembly control record which command states have changed.
     */

     assGetBusResponse( par, &bus1, &bus2, &bus3, &bus4, &bus5);

     if ( pGrPriv->cmdActive == FALSE )
     {
          /*
           * No command is active, so no change in the BUS links was expected.
           * This can typically happen when someone is using the device level
           * engineering screen or the assembly record's special processing gets
           * called for a reason other than a device response. Ignore it.
           */

          GRDEBUG(DAR_MSG_MIN, "grAckReceived: unexpected device response ignored%c\n", ' ');
     }
     else if ( assCommandTimedOut( par ))
     {
          if ( pGrPriv->askBarCode )
          {
                /*
                 *  A command timeout with the askBarCode flag set means that the
                 *  barcode reading delay has expired.   Call taskFinished to complete
                 *  the reading task.
                 */

               GRDEBUG(DAR_MSG_MIN, "grAckReceived: asked for a Barcode%c\n", ' ');
               status = grTaskFinished( par );
          }
          else if ( pGrPriv->raiseTurret || pGrPriv->turretPowerChange )
          {
               /*
                *  Raise/lower turret operation (or change of turret power) should be finished
                */

               GRDEBUG(DAR_MSG_MIN, "grAckReceived: raise/lower/power turret requested%c\n", ' ');
               status = grTaskFinished( par );
          }
          else
          {
               /*
                *  Any other timeout means that something has gone seriously wrong
                *  with the last command and the system has stopped responding.
                *  Abort the command immediately.
                */
               
               status = DAR_E_TIMEDOUT;
               GRDEBUG(DAR_MSG_ERROR, "grAckReceived: grating command timed out%c\n", ' ');

               grTerminateTasks( par, status, "Grating command timed out" );
          }
     }
     else if ( bus1 == TRUE || bus2 == TRUE || bus3 == TRUE || bus4 == TRUE || bus5 == TRUE)
     {
          /*
           *  else response received from device(s), handle
           */

          if ( status == DAR_S_SUCCESS && bus1 == TRUE )
          {
               /*
                *  GratingA response.
                */

               if ( par->bus1 == DAR_DEV_BUSY_IDLE )
               {
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pGrPriv->grAFinished = TRUE;
                    semGive (pGrPriv->mutexSem);
               }
               GRDEBUG(DAR_MSG_MIN, "grAckReceived, grA response (bus1)=%d\n", par->bus1 );
               status = grCheckBus( par, 1, par->bus1 );
               if ( status == DAR_S_SUCCESS )
               {
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pDevConfig->oldGratTilt[0] = pDevConfig->newGratTilt[0];
                    semGive (pGrPriv->mutexSem);
               }
          }
          if (status == DAR_S_SUCCESS && bus2 == TRUE )
          {
               /*
                *  GratingB response.
                */

               if ( par->bus2 == DAR_DEV_BUSY_IDLE )
               {
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pGrPriv->grBFinished = TRUE;
                    semGive (pGrPriv->mutexSem);
               }
               GRDEBUG(DAR_MSG_MIN, "grAckReceived, grB response (bus2)=%d\n", par->bus2 );
               status = grCheckBus( par, 2, par->bus2 );
               if ( status == DAR_S_SUCCESS )
               {
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pDevConfig->oldGratTilt[1] = pDevConfig->newGratTilt[1];
                    semGive (pGrPriv->mutexSem);
               }
          }
          if ( status == DAR_S_SUCCESS && bus3 == TRUE )
          {
               /*
                *  GratingC response.
                */

               if ( par->bus3 == DAR_DEV_BUSY_IDLE )
               {
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pGrPriv->grCFinished = TRUE;
                    semGive (pGrPriv->mutexSem);
               }
               GRDEBUG(DAR_MSG_MIN, "grAckReceived, grC response (bus3)=%d\n", par->bus3 );
               status = grCheckBus( par, 3, par->bus3 );
               if ( status == DAR_S_SUCCESS )
               {
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pDevConfig->oldGratTilt[2] = pDevConfig->newGratTilt[2];
                    semGive (pGrPriv->mutexSem);
               }
          }
          if ( status == DAR_S_SUCCESS && bus4 == TRUE )
          {
               /*
                *  GratingD response.
                */

               if ( par->bus4 == DAR_DEV_BUSY_IDLE )
               {
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pGrPriv->grDFinished = TRUE;
                    semGive (pGrPriv->mutexSem);
               }
               GRDEBUG(DAR_MSG_MIN, "grAckReceived, grD response (bus4)=%d\n", par->bus4 );
               status = grCheckBus( par, 4, par->bus4 );
               if ( status == DAR_S_SUCCESS )
               {
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pDevConfig->oldGratTilt[3] = pDevConfig->newGratTilt[3];
                    semGive (pGrPriv->mutexSem);
               }
          }
          if ( status == DAR_S_SUCCESS && bus5 == TRUE )
          {
               /*
                *  Turret response.
                */

               if ( par->bus5 == DAR_DEV_BUSY_IDLE )
               {
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pGrPriv->trtFinished = TRUE;
                    semGive (pGrPriv->mutexSem);
               }
               GRDEBUG(DAR_MSG_MIN, "grAckReceived, turret response (bus5)=%d\n", par->bus5 );
               status = grCheckBus( par, 5, par->bus5 );
               if ( status == DAR_S_SUCCESS )
               {
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pDevConfig->currentTurretPos = pDevConfig->newTurretPos;
                    semGive (pGrPriv->mutexSem);
               }
          }
     }
     else
     {
          /*
           *  Received bus acknowledgment from something other than bus1 to bus5.
           */

          GRDEBUG(DAR_MSG_ERROR, "grAckReceived, response from unexpected device%c\n", ' ');
     }

     return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grBuildList
 *
 * INVOCATION:
 * status = grBuildList (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 * 					structure.
 * (>) mode (int) Mode to act on.
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Build a list of tasks to perform based on the requested operating mode
 * and the current configuration of the grating assembly.
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

static long grBuildList(ASSEMBLY_CONTROL_RECORD *par, int mode)
{
     GR_LIST       *newTask = NULL;
     long          status = DAR_S_SUCCESS;      /* Return function status.	*/
     GR_DEV_PRIVATE *pGrPriv;                   /* Ptr to private dev. struct.	*/
     GR_DEV_CONFIG  *pDevConfig;
     int i;                                     /* Counter.			*/

     pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pGrPriv->pGratingPriv;

     GRDEBUG(DAR_MSG_MAX, "grBuildList: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL || pDevConfig == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grBuildList: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grBuildList: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

     /*
      *  Make sure the list is empty.
      */

     if ( ellCount( &(pGrPriv->taskList) ) != 0 )
     {
          GRDEBUG(DAR_MSG_ERROR, "grBuildList: taskList not empty, ellCount=%d\n", 
                ellCount( &(pGrPriv->taskList) ) );

          status = GR_TASK_LIST_NOT_EMPTY;
          assAddErrorMessage( par, "Grating, busy with outstanding tasks");
          return ( status );
     }

     newTask = ( GR_LIST *) ellFirst( &(pGrPriv->taskList) );

     switch( mode )
     {
     case DAR_MODE_INDEX:

          /* 
           * Index all devices at same time
           */

          for ( i = 0; i < (int)(sizeof( grIndexAll)/ sizeof( GR_TASK_LIST )); i++ )
          {
               newTask = malloc( sizeof ( GR_LIST ) );
               if ( newTask == NULL )
               {
                  GRDEBUG(DAR_MSG_FATAL,
                          "grBuildList: Memory allocation failure for index task %d\n", i );

                  status = DAR_E_MALLOC;
                  assAddErrorMessage( par, "Grating, insufficient memory for new task");
                  return (status);
               }
               newTask->item = (void *) &(grIndexAll[i]);
               ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
          }
          break;

     case DAR_MODE_MOVE:

          /*
           * Select a grating/tilt combination
           */

          GRDEBUG(DAR_MSG_FULL, "grBuildList: Adding move grating to task list%c\n", ' ');

          /* a move has been selected but what to do depends on the current configuration */

          semTake (pGrPriv->mutexSem, WAIT_FOREVER);
          pDevConfig->currentTurretPos = grReadSwitches (par);
          semGive (pGrPriv->mutexSem);

          if (pDevConfig->currentTurretPos == pDevConfig->newTurretPos )
          {
               /* Only need to move grating */

               switch(pDevConfig->newTurretPos)
               {
               case 0:
                    for ( i = 0; i < (int)(sizeof( grMoveGratA)/ sizeof( GR_TASK_LIST )); i++ )
                    {
                         newTask = malloc( sizeof ( GR_LIST ) );
                         if ( newTask == NULL )
                         {
                            GRDEBUG(DAR_MSG_FATAL,
                               "grBuildList: Memory allocation failure for turret move task %d\n", i );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Grating, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(grMoveGratA[i]);
                         ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
                    }
                    break;

               case 1:
                    for ( i = 0; i < (int)(sizeof( grMoveGratB)/ sizeof( GR_TASK_LIST )); i++ )
                    {
                         newTask = malloc( sizeof ( GR_LIST ) );
                         if ( newTask == NULL )
                         {
                            GRDEBUG(DAR_MSG_FATAL,
                               "grBuildList: Memory allocation failure for turret move task %d\n", i );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Grating, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(grMoveGratB[i]);
                         ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
                    }
                    break;

               case 2:
                    for ( i = 0; i < (int)(sizeof( grMoveGratC)/ sizeof( GR_TASK_LIST )); i++ )
                    {
                         newTask = malloc( sizeof ( GR_LIST ) );
                         if ( newTask == NULL )
                         {
                            GRDEBUG(DAR_MSG_FATAL,
                               "grBuildList: Memory allocation failure for turret move task %d\n", i );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Grating, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(grMoveGratC[i]);
                         ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
                    }
                    break;

               case 3:
                    for ( i = 0; i < (int)(sizeof( grMoveGratD)/ sizeof( GR_TASK_LIST )); i++ )
                    {
                         newTask = malloc( sizeof ( GR_LIST ) );
                         if ( newTask == NULL )
                         {
                            GRDEBUG(DAR_MSG_FATAL,
                               "grBuildList: Memory allocation failure for turret move task %d\n", i );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Grating, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(grMoveGratD[i]);
                         ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
                    }
                    break;

               default:
                    GRDEBUG(DAR_MSG_ERROR, "grBuildList: invalid turret position=%d\n",
                       pDevConfig->newTurretPos );

                    status = GR_INVALID_LOCATION;
                    assAddErrorMessage( par, "Grating turret position not recognised");
               }
          }
          else
          {
               /* Move turret and grating */

               switch(pDevConfig->newTurretPos)
               {
               case 0:
                    for ( i = 0; i < (int)(sizeof( grMovePosA)/ sizeof( GR_TASK_LIST )); i++ )
                    {
                         newTask = malloc( sizeof ( GR_LIST ) );
                         if ( newTask == NULL )
                         {
                            GRDEBUG(DAR_MSG_FATAL,
                               "grBuildList: Memory allocation failure for move task %d\n", i );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Grating, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(grMovePosA[i]);
                         ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
                    }
                    break;

               case 1:
                    for ( i = 0; i < (int)(sizeof( grMovePosB)/ sizeof( GR_TASK_LIST )); i++ )
                    {
                         newTask = malloc( sizeof ( GR_LIST ) );
                         if ( newTask == NULL )
                         {
                            GRDEBUG(DAR_MSG_FATAL,
                               "grBuildList: Memory allocation failure for move task %d\n", i );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Grating, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(grMovePosB[i]);
                         ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
                    }
                    break;

               case 2:
                    for ( i = 0; i < (int)(sizeof( grMovePosC)/ sizeof( GR_TASK_LIST )); i++ )
                    {
                         newTask = malloc( sizeof ( GR_LIST ) );
                         if ( newTask == NULL )
                         {
                            GRDEBUG(DAR_MSG_FATAL,
                               "grBuildList: Memory allocation failure for move task %d\n", i );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Grating, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(grMovePosC[i]);
                         ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
                    }
                    break;

               case 3:
                    for ( i = 0; i < (int)(sizeof( grMovePosD)/ sizeof( GR_TASK_LIST )); i++ )
                    {
                         newTask = malloc( sizeof ( GR_LIST ) );
                         if ( newTask == NULL )
                         {
                            GRDEBUG(DAR_MSG_FATAL,
                               "grBuildList: Memory allocation failure for move task %d\n", i );

                            status = DAR_E_MALLOC;
                            assAddErrorMessage( par, "Grating, insufficient memory for new task");
                            return (status);
                         }
                         newTask->item = (void *) &(grMovePosD[i]);
                         ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
                    }
                    break;

               default:
                    GRDEBUG(DAR_MSG_ERROR, "grBuildList: invalid turret position=%d\n",
                       pDevConfig->newTurretPos );

                    status = GR_INVALID_LOCATION;
                    assAddErrorMessage( par, "Grating turret position not recognised");
               }
          }

          break;

     case DAR_MODE_UPDATE:

         /*
          * Clear the internal list of barcodes.
          */

          grClearBarcodes( par );

          /* Update grating characterisation */

          GRDEBUG(DAR_MSG_LOG, "Grating UPDATE requested%c\n", ' ');
          GRDEBUG(DAR_MSG_FULL, "grBuildList: Adding UPDATE to task list%c\n", ' ');

          for ( i = 0; i < (int)(sizeof( grCharacterise)/ sizeof( GR_TASK_LIST )); i++ )
          {
               newTask = malloc( sizeof ( GR_LIST ) );
               if ( newTask == NULL )
               {
                  GRDEBUG(DAR_MSG_FATAL,
                     "grBuildList: Memory allocation failure for update task %d\n", i );

                  status = DAR_E_MALLOC;
                  assAddErrorMessage( par, "Grating, insufficient memory for new task");
                  return (status);
               }
               newTask->item = (void *) &(grCharacterise[i]);
               ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
          }
          break;

     case DAR_MODE_INIT:

          /*
           *  Build the task list to initialise all devices at the same time.
           *  (The following statements assume INIT only has ont task on the list).
           */

          GRDEBUG(DAR_MSG_FULL, "grBuildList: adding INIT to task list%c\n", ' ');

          newTask = malloc( sizeof ( GR_LIST ) );
          if ( newTask == NULL )
          {
             GRDEBUG(DAR_MSG_FATAL,
                "grBuildList: Memory allocation failure for init task%c\n", ' ' );

             status = DAR_E_MALLOC;
             assAddErrorMessage( par, "Grating, insufficient memory for new task");
             return (status);
          }
          newTask->item = (void *)&(grInit[0]);
          ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );

          break;

     case DAR_MODE_TEST:

          /*
           *  Build the task list to test all devices at the same time.
           *  (The following statements assume TEST only has ont task on the list).
           */

          GRDEBUG(DAR_MSG_FULL, "grBuildList: adding TEST to task list%c\n", ' ');

          newTask = malloc( sizeof ( GR_LIST ) );
          if ( newTask == NULL )
          {
             GRDEBUG(DAR_MSG_FATAL,
                "grBuildList: Memory allocation failure for test task%c\n", ' ' );

             status = DAR_E_MALLOC;
             assAddErrorMessage( par, "Grating, insufficient memory for new task");
             return (status);
          }
          newTask->item = (void *)&(grTest[0]);
          ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );

          break;

     case DAR_MODE_PARK:

          /*
           *  Build the task list to park all the devices at the same time.
           */

          GRDEBUG(DAR_MSG_FULL, "grBuildList: adding PARK to task list%c\n", ' ');

          for ( i = 0; i < (int)(sizeof( grParkAll)/ sizeof( GR_TASK_LIST )); i++ )
          {
               newTask = malloc( sizeof ( GR_LIST ) );
               if ( newTask == NULL )
               {
                  GRDEBUG(DAR_MSG_FATAL,
                     "grBuildList: Memory allocation failure for park task %d\n", i );

                  status = DAR_E_MALLOC;
                  assAddErrorMessage( par, "Grating, insufficient memory for new task");
                  return (status);
               }
               newTask->item = (void *) &(grParkAll[i]);
               ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(newTask->node) );
          }

          break;

     default:

          /*
           *  Error, nothing else supported.
           */

          GRDEBUG(DAR_MSG_ERROR, "grBuildList: invalid mode=%d\n", mode );

          status = DAR_E_MODE;
          assAddErrorMessage( par, "Grating, Mode not implemented");

     }/* End of switch on MODE */

     return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grCheckAttributes
 *
 * INVOCATION:
 * status = grCheckAttributes( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *          > par->a    long     select only flag {1 = grating only}
 *          > par->b    string   gratingId {barcode number string}
 *          > par->c    double   grating tilt angle {radians}
 *          > par->d    string   backlash/forwardlash/zpc parameters
 *          > par->e    long     park position
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
 * or REJECT. For the grating assembly this involves checking that
 * the requested grating is loaded and the tilt angle is achievable.
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

static long grCheckAttributes(ASSEMBLY_CONTROL_RECORD *par)
{
     long           status = DAR_S_SUCCESS;	/* Returned function status.	*/
     GR_DEV_PRIVATE *pGrPriv;                   /* Ptr to private dev. struct.	*/
     GR_DEV_CONFIG  *pDevConfig;
     int            i = 0;
     long           barcodeId;
     double         tiltAngle;
     int            match = FALSE;                   /* boolean search flag         */
     char           tempString[4][MAX_STRING_SIZE];  /* temp string buffers         */
     double         tempDouble[3];                   /* temp doubles                */
     long           tempLong[4];                        /* temp long                   */
     BOOL           gotTscale;                       /* TRUE when tscale allocated  */
     BOOL           gotFlash;                        /* TRUE when flash  allocated  */
     BOOL           gotBlash;                        /* TRUE when blash allocated  */
     BOOL           gotZpca;                          /* TRUE when zerp point correction allocated */
     BOOL           gotZpcb;                          /* TRUE when zerp point correction allocated */
     BOOL           gotZpcc;                          /* TRUE when zerp point correction allocated */
     BOOL           gotZpcd;                          /* TRUE when zerp point correction allocated */
     long         pixZpca;                         /* zpc in pixels */
     long         pixZpcb;                         /* zpc in pixels */
     long         pixZpcc;                         /* zpc in pixels */
     long         pixZpcd;                         /* zpc in pixels */


     GRDEBUG(DAR_MSG_MAX, "grCheckAttributes: entry, sim=%d\n", assSimulateLevel(par) );

     /*
      * Obtain the device private structure and device configuration structure pointers.
      */

     pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pGrPriv->pGratingPriv;

     /*
      * Trap any problem with the internal data structures
      */

     if ( pGrPriv == NULL || pDevConfig == NULL )
     {
	  GRDEBUG(DAR_MSG_FATAL, "grCheckAttributes: Bad device private data structure%c\n", ' ' );
	  assDisplayPrivateStruct( par );

	  status = GR_BAD_STRUCTURE;
	  recGblRecordError (status, par, __FILE__ ":bad internal data structure");
	  return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
     {
	  GRDEBUG(DAR_MSG_FATAL, "grCheckAttributes: No magic value seen in data structure%c\n", ' ' );

	  status = GR_BAD_STRUCTURE;
	  recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
	  return (status);
     }

     /*
      *  Assembly must be idle before accepting new commands.
      */

     if ( par->busy == DAR_BUSY_BUSY)
     {
	  GRDEBUG(DAR_MSG_ERROR, "grCheckAttributes: grating assembly is busy%c\n",  ' ');

	  status = GR_TASK_LIST_NOT_EMPTY;
	  assAddErrorMessage (par, "Error, grating assembly is BUSY");
	  return status;
     }

     /* If DIR/MODE is START|PRESET/MOVE, then  */

     if ( ( par->dir == DAR_DIR_PRESET || par->dir == DAR_DIR_START ) && par->mode == DAR_MODE_MOVE )
     {
          if ( par->fta != DBF_LONG || par->ftb != DBF_STRING || par->ftc != DBF_DOUBLE ||
               par->ftd != DBF_STRING || par->fte != DBF_LONG)
          {

	       /* Error, The field types of attribute a or b or c or d or e are not correct */

               GRDEBUG(DAR_MSG_ERROR,
               "grCheckAttributes: data types for attr a,b,c,d,e should be long, string, double, string, long%c\n",
		       ' ' );
               status = DAR_E_ATT;
               assAddErrorMessage( par, "Grating, Invalid attribute data types");
          }

          /* read in and range check attributes */

          semTake (pGrPriv->mutexSem, WAIT_FOREVER);
          if ( *(long *) par->a == 1 )
          {
               pDevConfig->selectOnlyFlag =  GR_SELECT_ONLY;
          }
          else if ( *(long *) par->a == 0 )
          {
               pDevConfig->selectOnlyFlag =  GR_SELECT_AND_MOVE;
          }
	  else
	  {
               GRDEBUG(DAR_MSG_ERROR, "grCheckAttributes: expect attr a to be 0/1 only %c\n", ' ' );
               status = DAR_E_ATT;
               assAddErrorMessage( par, "Select Only flag (att A) invalid");
	       semGive (pGrPriv->mutexSem);
	       return (status);
	  }

          semGive (pGrPriv->mutexSem);

          /* read in and check barcode */

          if (sscanf (par->b, "%ld%s", &barcodeId, &tempString[0][0]) != 1)
          {
               assAddErrorMessage (par, "Grating failed attr B conversion");
               status = DAR_E_ATT;
               return(status);
          }

          if ( (barcodeId < GR_BARCODE_MIN || barcodeId > GR_BARCODE_MAX) &&
               (barcodeId < GR_BARCODE_MIN_SIM || barcodeId > GR_BARCODE_MAX_SIM)
	       )
          {
               assAddErrorMessage(par, "Grating barcode out of range");
               status = GR_INVALID_BARCODE;
               return(status);
          }

	  /*
           * Read in the tilt device configuration string - expect to store tilt scale,
           * backlash and forwardlash numbers
           */

	  semTake (pGrPriv->mutexSem, WAIT_FOREVER);

	  gotTscale = FALSE;
	  gotBlash  = FALSE;
	  gotFlash  = FALSE;
          gotZpca    = FALSE;
          gotZpcb    = FALSE;
          gotZpcc    = FALSE;
          gotZpcd    = FALSE;

	  if (sscanf((char *)par->d, "%lf %lf %lf %ld %ld %ld %ld",
		     &tempDouble[0],
		     &tempDouble[1],
		     &tempDouble[2],
                     &tempLong[0], 
                     &tempLong[1], 
                     &tempLong[2], 
                     &tempLong[3]) != 7)
	  {
	       GRDEBUG(DAR_MSG_ERROR,
		       "grCheckAttributes: wrong string read from input D%c\n",
		       ' ' );
	       status = DAR_E_ATT;
	       assAddErrorMessage( par, "Invalid config string in att D");
	       semGive (pGrPriv->mutexSem);
	       return (status);
	  }
	  else
	  {
			 pDevConfig->tiltScale = tempDouble[0];
			 gotTscale = TRUE;
			 pDevConfig->backlash = tempDouble[1];
			 gotBlash = TRUE;
			 pDevConfig->forwardlash = tempDouble[2];
			 gotFlash = TRUE;
                         pixZpca = tempLong[0]; 
                         pixZpcb = tempLong[1]; 
                         pixZpcc = tempLong[2]; 
                         pixZpcd = tempLong[3]; 

                         gotZpca = TRUE; 
			 gotZpcb = TRUE;
			 gotZpcc = TRUE;
			 gotZpcd = TRUE;

	  }

	  if (!gotTscale || !gotBlash || !gotFlash || !gotZpca || !gotZpcb || !gotZpcc || !gotZpcd)
	  {
	       GRDEBUG(DAR_MSG_ERROR,
		       "grCheckAttributes: incomplete string read from input D%c\n",
		       ' ' );
	       status = DAR_E_ATT;
	       assAddErrorMessage( par, "Incomplete config string in att D");
	       semGive (pGrPriv->mutexSem);
	       return (status);
	  }

	  semGive (pGrPriv->mutexSem);

          /* read in and check tilt angle request */

          if ( pDevConfig->selectOnlyFlag ==  GR_SELECT_AND_MOVE )
          {
               tiltAngle = *(double *) par->c;
               if (tiltAngle < GR_TILT_LO_LIMIT || (tiltAngle + pDevConfig->backlash) > GR_TILT_HI_LIMIT)
               {
                    assAddErrorMessage (par, "Grating tilt angle out of range");
                    status = DAR_E_BAD_ATTR;
                    return(status);
               }
          }

          /*
           * Give a warning if the device has not been characterized
           * (i.e. there is no barcode database).
           */

          if ( pDevConfig->characterized == FALSE)
          {
	       GRDEBUG(DAR_MSG_WARNING,
		       "grCheckAttributes: Device not characterized - using existing database%c\n", ' ');
          }

          /* check whether the named position is in the internal database */

          match = FALSE;

          for (i=0; i < GR_NUM_GRATINGS; i++)
          {
               if (pDevConfig->barcode[i] == barcodeId)
               {
                    match = TRUE;
                    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                    pDevConfig->newTurretPos = i;
                    if ( pDevConfig->selectOnlyFlag ==  GR_SELECT_AND_MOVE ) 
                    {
                         pDevConfig->newGratTilt[i] = tiltAngle;
                    }
                    semGive (pGrPriv->mutexSem);
               }
          }

          /* if not found, exit */

          if (match == FALSE)
          {
               GRDEBUG(DAR_MSG_ERROR, "grCheckAttributes: grating with ID %ld not installed\n",
		       barcodeId);

               status = GR_UNKNOWN_GRATING;
               assAddErrorMessage(par, "Requested grating not installed");

               return(status);
          }
     }

     /* If DIR/MODE is START|PRESET/PARK, then  */

     if ( ( par->dir == DAR_DIR_PRESET || par->dir == DAR_DIR_START ) && par->mode == DAR_MODE_PARK )
     {
          if ( par->fte != DBF_LONG  )
          {

	       /* Error, The field types of attribute e is not correct */

               GRDEBUG(DAR_MSG_ERROR,
		       "grCheckAttributes: data type for attr e should be long%c\n",
		       ' ' );

               status = DAR_E_ATT;
               assAddErrorMessage( par, "Grating, Invalid attribute e data type");
               return(status);
          }

          /* read in and range check attributes */

          semTake (pGrPriv->mutexSem, WAIT_FOREVER);
	  pGrPriv->parkPosition = *(long *) par->e; 
          semGive (pGrPriv->mutexSem);

	  if (pGrPriv->parkPosition < 0 || pGrPriv->parkPosition > 3)
	  {

	       /* Error, An invalid grating number */

               GRDEBUG(DAR_MSG_ERROR,
		       "grCheckAttributes: Invalid grating selected for PARK%c\n",
		       ' ' );
               status = DAR_E_BAD_ATTR;
               assAddErrorMessage( par, "Grating, Invalid turret PARK position");
               return(status);
	  }
     }


     /* If DIR/MODE is START|PRESET/UPDATE, then  */

     if ( ( par->dir == DAR_DIR_PRESET || par->dir == DAR_DIR_START ) && par->mode == DAR_MODE_UPDATE )
     {
          /* Set demand grating tilts. */

          semTake (pGrPriv->mutexSem, WAIT_FOREVER);
          for ( i = 0; i < GR_NUM_GRATINGS; i++ )
          {
               pDevConfig->newGratTilt[i] = pDevConfig->oldGratTilt[i];
          }
          semGive (pGrPriv->mutexSem);
     }

     return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grCheckBarcodeId
 *
 * INVOCATION:
 * status = grCheckBarcodeId( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * To check the returned barcode id and update the barcode/turret position
 * table.
 *
 * DESCRIPTION:
 * To check the returned barcode id.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the barcode reader has finished getting the barcode id.
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long grCheckBarcodeId(ASSEMBLY_CONTROL_RECORD *par)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt;	/* Private rec. support struct.	*/
    GR_DEV_PRIVATE *pGrPriv;			/* Ptr to private dev. struct.	*/
    GR_DEV_CONFIG  *pDevConfig;		        /* Ptr to device config struct.	*/
    long        status = DAR_S_SUCCESS;		/* Function status.		*/
    long        value;				/* Barcode value.		*/
    char        *end;				/* End string pointer.		*/

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
    pDevConfig = pGrPriv->pGratingPriv;

    GRDEBUG(DAR_MSG_MAX, "grCheckBarcodeId: entry%c\n",  ' ');

   /*
    * Trap any problem with the internal data structures
    */

    if ( pGrPriv == NULL || pDevConfig == NULL )
    {
        GRDEBUG(DAR_MSG_FATAL, "grCheckBarcodeId: Bad device private data structure%c\n", ' ' );
        assDisplayPrivateStruct( par );

        status = GR_BAD_STRUCTURE;
        recGblRecordError (status, par, __FILE__ ":bad internal data structure");
        return (status);
    }
    else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
    {
        GRDEBUG(DAR_MSG_FATAL, "grCheckBarcodeId: No magic value seen in data structure%c\n", ' ' );

        status = GR_BAD_STRUCTURE;
        recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
        return (status);
    }

    GRDEBUG(DAR_MSG_MIN, "grCheckBarcodeId: Checking the barcode ID.%c\n", ' ');

    /* Check the barcode read. */

    if ( strncmp( GRAT_BARCODE_READ_FIELD, "NR", 2 ) != 0 )
    {
         value = strtol(GRAT_BARCODE_READ_FIELD, &end, 10 );

         if ( *end != '\0' )
         {
             GRDEBUG(DAR_MSG_ERROR,"grCheckBarcodeId: Invalid barcode=%s\n",
                GRAT_BARCODE_READ_FIELD );

             /* A barcode read error should not affect the index status */
             pPriv->keepIndex = TRUE;

             status = GR_INVALID_BARCODE;
             value = 0;
        }
        else if ( (value < GR_BARCODE_MIN || value > GR_BARCODE_MAX) &&
                  (value < GR_BARCODE_MIN_SIM || value > GR_BARCODE_MAX_SIM)
                )
        {
             /* An out of range barcode is stored as a null value */

             GRDEBUG(DAR_MSG_WARNING,
                "grCheckBarcodeId: Barcode at turret position %d out of range\n",
                pDevConfig->currentTurretPos );

             value = GR_NO_BARCODE;
        }
    }
    else
    {
         /* An unreadable barcode is stored as a null value */

         GRDEBUG(DAR_MSG_WARNING,
            "grCheckBarcodeId: Unreadable barcode at turret position %d\n",
            pDevConfig->currentTurretPos );

         value = GR_NO_BARCODE;
    }
    
    if ( status == DAR_S_SUCCESS )
    {
        /* store away the barcode for this grating position. The barcode reader is on
           the opposite side of the turret from the grating position. */

         if (pDevConfig->currentTurretPos >= 0 && pDevConfig->currentTurretPos < GR_NUM_GRATINGS)
         {
              semTake (pGrPriv->mutexSem, WAIT_FOREVER);
              switch (pDevConfig->currentTurretPos) 
              {
              case 0: 
                   pDevConfig->barcode[0] = value; break;	/* Was [2] */
              case 1: 
                   pDevConfig->barcode[1] = value; break;	/* Was [3] */
              case 2: 
                   pDevConfig->barcode[2] = value; break;	/* Was [0] */
              case 3: 
                   pDevConfig->barcode[3] = value; break;	/* Was [1] */
              }
              pDevConfig->characterized = TRUE;
              semGive (pGrPriv->mutexSem);
         }
    }

    return ( status );
}
/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grCheckLifter
 *
 * INVOCATION:
 * status = grCheckLifter( par, expectedState );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Check turret lifter status after a raise/lower operation
 *
 * DESCRIPTION:
 * Status returned from lifter mechanism is examined
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

static long grCheckLifter( ASSEMBLY_CONTROL_RECORD *par, const long expectedState )
{
    long status = DAR_S_SUCCESS;	/* Function status.		*/
    long lifterState;

    GRDEBUG(DAR_MSG_FULL, "grCheckLifter: Read lifter status - expect %ld\n", expectedState);

    lifterState = *(long *)par->sik;

    if (lifterState != expectedState)
    {
         status = GR_LIFTER_FAILED;
    }
    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grCheckTurretPower
 *
 * INVOCATION:
 * status = grCheckTurretPower( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Check turret motor power after a power on/off operation
 *
 * DESCRIPTION:
 * Status returned from turret power examined
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

static long grCheckTurretPower(ASSEMBLY_CONTROL_RECORD *par)
{

    GRDEBUG(DAR_MSG_FULL, "grCheckTurretPower: Read turret power status.%c\n", ' ');

    return (*(long *)GR_TURRET_POWER_STATE);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grCheckBus
 *
 * INVOCATION:
 * status = grCheckBus( par, devnum, busVal );
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
 *     if ERROR then
 *	The command failed on that device.
 *	Stop the timer, set an error message, empty the task list
 *	Set the cmdFinished flag to TRUE and cmdActive flag to FALSE.
 *    else if BUSY then
 *	Just ignore this change. The device hasn't finished yet.
 *    else if IDLE then
 *	That means something completed on that device so:
 *	a task has finished, call grTaskFinished()
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

static long grCheckBus(ASSEMBLY_CONTROL_RECORD *par, const int devnum, int busVal)
{
    GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/

    long status = DAR_S_SUCCESS;	/* Function status.		*/
    char errMsg[GR_ERROR_MESSAGE_SIZE]; /* Error message.		*/

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    GRDEBUG(DAR_MSG_MAX, "grCheckBus: entry, bus=%d\n", busVal);

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grCheckBus: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grCheckBus: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
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

            GRDEBUG(DAR_MSG_MIN, "grCheckBus, BUSY to ERROR transition detected%c\n", ' ');

            status = DAR_E_DEVICE;
            sprintf (errMsg, "Grating %s failed",
               (devnum==1 ? "A" : (devnum==2 ? "B" : (devnum==3 ? "C" : (devnum==4 ? "D" : "turret")))));
            grTerminateTasks( par, status, errMsg);
            break;

        case DAR_DEV_BUSY_BUSY:
            /*
             *  Ignore the busy change.
             */

            GRDEBUG(DAR_MSG_FULL, "grCheckBus, BUSY to BUSY transition detected%c\n", ' ');
            break;

        case DAR_DEV_BUSY_IDLE:
            /*
             *  Check to see if all devices are finished, 
             *  and if so then the command is finished, so stop the timer
             *  set the bus flags to false.
             */

            GRDEBUG(DAR_MSG_FULL, "grCheckBus, BUSY to IDLE transition detected%c\n",' ');
            status = grTaskFinished( par );
            break;

        default:
            GRDEBUG(DAR_MSG_ERROR, "grCheckBus, invalid device acknowledgement: %d\n",
                busVal );

            status = DAR_E_DEVICE;
            grTerminateTasks( par, status, "Grating, invalid device busy field");
            break;
    }
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grDoTask
 *
 * INVOCATION:
 * status = grDoTask (par); 
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
 * by grBuildList, as follows:
 *    Make sure list is not empty and point to first one in list.
 *    Clear the receipt flags and make sure the timer is stopped.
 *    Do the Task requested by setting variables in the private
 *    structure: velocity, position, devicePointer, mode.
 *    If getting the task worked ok, then call grExecuteTask()
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

static long grDoTask(ASSEMBLY_CONTROL_RECORD *par)
{
     ASS_CONTROL_PRIVATE *assPriv = par->dpvt;	/* Private rec. support struct.	*/
     int i;		                /* Count through devices 	*/
     int doAgain;		        /* Redo flag.			*/
     char errMsg[GR_ERROR_MESSAGE_SIZE];/* Error message.		*/
     long nRequest = 1;		        /* Number requested.		*/
     GR_DEV_PRIVATE *pGrPriv;           /* Ptr to private dev. struct.	*/
     GR_DEV_CONFIG *pDevConfig;
     long status = DAR_S_SUCCESS;	/* Return function status.	*/
     GR_LIST *taskRequested = NULL;	/* Ptr to task requested.	*/
     GR_TASK_LIST *taskItem = NULL;	/* Ptr to item in task list.	*/

     long timerStatus = DAR_S_SUCCESS;  /* status word from assStopTimer  */

     int lifterUp       = GR_LIFTER_UP;
     int lifterDown     = GR_LIFTER_DOWN;
     int turretPowerOn  = GR_TURRET_POWER_ON_VAL;
     int turretPowerOff = GR_TURRET_POWER_OFF_VAL;
     int lifterWaitTime = 1 + (int) ( (float) GR_LIFTER_CYCLE_TIME * 1.5);

     double fsteps;

     int site;

     pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pGrPriv->pGratingPriv;

     GRDEBUG(DAR_MSG_MAX, "grDoTask: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL || pDevConfig == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grDoTask: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grDoTask: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

     /*
      *  Make sure list is not empty and point to first one in list.
      */

     if ( ellCount( &(pGrPriv->taskList) ) == 0 || 
          ( taskRequested = (GR_LIST *) ellFirst( &(pGrPriv->taskList) )) == NULL )
     {
          /*
           *  If there are no tasks then the grating assembly code has become confused.
           *  Abort the command.
           */

          GRDEBUG(DAR_MSG_ERROR, "grDoTask: taskList empty, ellCount=%d\n",
              ellCount( &(pGrPriv->taskList) ) );

          status = GR_LAST_TASK;
          grTerminateTasks( par, status, "Grating assembly confused, no tasks to do" );
          return ( status );
     }

     taskItem = ( GR_TASK_LIST *) taskRequested->item;

     GRDEBUG(DAR_MSG_MAX, "grDoTask: taskList size=%d\n", ellCount( &(pGrPriv->taskList) ) );
     GRDEBUG(DAR_MSG_MAX, "grDoTask: firsttask=%d\n", taskItem->task );

     /*
      *  Clear the receipt flags and make sure the timer is stopped.
      */

     semTake (pGrPriv->mutexSem, WAIT_FOREVER);
     pGrPriv->grAFinished = FALSE; 
     pGrPriv->grBFinished = FALSE; 
     pGrPriv->grCFinished = FALSE; 
     pGrPriv->grDFinished = FALSE; 
     pGrPriv->trtFinished = FALSE; 
     semGive (pGrPriv->mutexSem);

     assStopTimer( par, &timerStatus );
     if (timerStatus != DAR_S_SUCCESS)
     {
         GRDEBUG(DAR_MSG_WARNING, "grDoTask: failed to stop watchdog timer. status=%ld\n",
            timerStatus);
     }
     status = DAR_S_SUCCESS;


     /*
      * Set default velocities.
      */

     semTake (pGrPriv->mutexSem, WAIT_FOREVER);

     for (i = GRA; i < TRT; i++)
          pGrPriv->velocity[i] = GR_VELOCITY_GRATING;
     pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET;

     semGive (pGrPriv->mutexSem);

     /*
      *  Do the Task requested.
      */
 
     doAgain = 1;
     while ( doAgain )
     {
          doAgain = 0;
          switch( taskItem->task )
          {

          case GR_INDEX_ALL:

               GRDEBUG(DAR_MSG_MIN, "grDoTask: Index all devices%c\n", ' ');

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               for (i=0; i < GR_NUM_DEVICES; i++)
               {
                    (void) strncpy( pGrPriv->position[i], GR_NAME_POS_INDEX, MAX_STRING_SIZE-1 );
                    pGrPriv->velocity[i] = GR_VELOCITY_NONE;     /* Do not change the velocity... */
               }
	       pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET_INDEX;     /* ...except the turret velocity */
               pGrPriv->mode = DAR_MODE_INDEX;
               pDevConfig->newTurretPos = -1;
               semGive (pGrPriv->mutexSem);
               break;
                        
          case GR_INIT_ALL:
                 
               GRDEBUG(DAR_MSG_MIN, "grDoTask: Initialize all devices%c\n", ' ');

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               for (i=0; i < GR_NUM_DEVICES; i++)
               {
                    (void) strcpy( pGrPriv->position[i], " " );
                    pGrPriv->velocity[i] = GR_VELOCITY_NONE;     /* Do not change the velocity */
               }
               pGrPriv->mode = DAR_MODE_INIT;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_TEST_ALL:
                 
               GRDEBUG(DAR_MSG_MIN, "grDoTask: Test all devices%c\n", ' ');

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               for (i=0; i < GR_NUM_DEVICES; i++)
               {
                    (void) strcpy( pGrPriv->position[i], " " );
                    pGrPriv->velocity[i] = GR_VELOCITY_NONE;     /* Do not change the velocity */
               }
               pGrPriv->mode = DAR_MODE_TEST;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_READ_BARCODE:
                 
               GRDEBUG(DAR_MSG_MIN, "grDoTask: Read grating barcode ID%c\n", ' ');

               strncpy( GRAT_BARCODE_READ_FIELD, "WAITING", MAX_STRING_SIZE-1);
                 if((dbPutLink(&(GRAT_BARCODE_TRIGGER),DBR_LONG,&nRequest,nRequest))!= DAR_S_SUCCESS)
               {
                    status = GR_TRIG_FAILED;
                    strncpy( errMsg, "Trigger grating barcode failed", MAX_STRING_SIZE-1);
               }
               else
               {
                    GRDEBUG(DAR_MSG_FULL, "grDoTask: trigger grating barcode status OK%c\n", ' ');
               }
               break;

          case GR_TURRET_UP:

               /* set lifter control to up */

                 if(dbPutLink(&(GR_LIFTER_CMD),DBR_LONG,&lifterUp,nRequest)!= DAR_S_SUCCESS)
               {
                    status = GR_TRIG_FAILED;
               }

               break;

          case GR_TURRET_DOWN:
                 
               /* set lifter control to down */

                 if(
 			dbPutLink(&(GR_LIFTER_CMD),DBR_LONG,&lifterDown,nRequest)!= DAR_S_SUCCESS
                   )
               {
                    status = GR_TRIG_FAILED;
               }

               break;

          case GR_WRITE_CONFIG_BAK:

               /* Display the current configuration */

               grShowConfig( par );
  
               /* write current grating configuration (with new barcodes) to file (with backup) */

               if ((status = grWriteConfig(par, 1)) != DAR_S_SUCCESS)
               {
                    strncpy(errMsg, "Failed to write config file", MAX_STRING_SIZE-1);
               }

               break;

          case GR_WRITE_CONFIG:
                 
               /* write current grating configuration (with new tilts) to file (without backup) */

               if ((status = grWriteConfig(par, 0)) != DAR_S_SUCCESS)
               {
                    strncpy(errMsg, "Failed to write config file", MAX_STRING_SIZE-1);
               }

               break;

          case GR_MV_TURRET_A:

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSA, MAX_STRING_SIZE-1 );
               pGrPriv->mode = DAR_MODE_MOVE;
	       pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET;     /* Correct turret move velocity */
               pDevConfig->newTurretPos = 0;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_TURRET_B:

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSB, MAX_STRING_SIZE-1 );
               pGrPriv->mode = DAR_MODE_MOVE;
	       pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET;     /* Correct turret move velocity */
               pDevConfig->newTurretPos = 1;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_TURRET_C:

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSC, MAX_STRING_SIZE-1 );
               pGrPriv->mode = DAR_MODE_MOVE;
	       pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET;     /* Correct turret move velocity */
               pDevConfig->newTurretPos = 2;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_TURRET_D:

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSD, MAX_STRING_SIZE-1 );
               pGrPriv->mode = DAR_MODE_MOVE;
	       pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET;     /* Correct turret move velocity */
               pDevConfig->newTurretPos = 3;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_TURRET_BCA:

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSBCA, MAX_STRING_SIZE-1 );
               pGrPriv->mode = DAR_MODE_MOVE;
	       pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET;     /* Correct turret move velocity */
               pDevConfig->newTurretPos = 0;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_TURRET_BCB:

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSBCB, MAX_STRING_SIZE-1 );
               pGrPriv->mode = DAR_MODE_MOVE;
	       pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET;     /* Correct turret move velocity */
               pDevConfig->newTurretPos = 1;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_TURRET_BCC:

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSBCC, MAX_STRING_SIZE-1 );
               pGrPriv->mode = DAR_MODE_MOVE;
	       pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET;     /* Correct turret move velocity */
               pDevConfig->newTurretPos = 2;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_TURRET_BCD:

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSBCD, MAX_STRING_SIZE-1 );
               pGrPriv->mode = DAR_MODE_MOVE;
	       pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET;     /* Correct turret move velocity */
               pDevConfig->newTurretPos = 3;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_A_1:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRA].numberOfEntries < 2 )
               {
                    printf("*** applying scale factor from the D input field ***\n");
                    fsteps = (pDevConfig->newGratTilt[GRA] + pDevConfig->backlash) *
                             pDevConfig->tiltScale;
               }
               else
               {
                    printf("*** succeeded loading lookup table, checking lut\n");
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRA]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating A: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */
                    /*printf(" *** translating via lut: pDevConfig->newGratTilt[GRA] = %f\n", pDevConfig->newGratTilt[GRA]); */
                    /*printf(" *** translating via lut: pDevConfig->backlash = %f\n",pDevConfig->backlash);*/ 
                    /*printf(" GR_MV_GRATING_A_1 *** steps from lut: &fsteps = %f\n", fsteps); */
                    
	            status = gmosLutApply( (pDevConfig->newGratTilt[GRA] + pDevConfig->backlash),
                                           &(pGrPriv->tilt2StepsLut[GRA]), &fsteps );

                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating A: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }
               }


               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRA], "%f", fsteps);
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_A_2:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRA].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRA]) * pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRA]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating A: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */
                    /*printf(" 2 translating via lut: pDevConfig->newGratTilt[GRA] = %f\n", pDevConfig->newGratTilt[GRA]); */
                    /*printf(" 2 translating via lut: pDevConfig->backlash = %f\n", pDevConfig->backlash); */
                    /*printf(" 2 translating via lut: &fsteps = %f\n", fsteps);*/

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRA]),
                                           &(pGrPriv->tilt2StepsLut[GRA]), &fsteps );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating A: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }
               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRA], "%f", fsteps);
               GR_STEP_VALUE = fsteps;
               printf(" GRATING A MOTORSTEPS : %f\n", fsteps);
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_A_3:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRA].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRA] + pDevConfig->forwardlash) *
                             pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRA]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating A: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */
                    /* Translate via the lut */
                    /*printf(" 3 translating via lut: pDevConfig->newGratTilt[GRA] = %f\n", pDevConfig->newGratTilt[GRA]); */
                    /*printf(" 3 translating via lut: pDevConfig->backlash = %f\n", pDevConfig->backlash);*/ 
                    /*printf(" 3 translating via lut: &fsteps = %f\n", fsteps);*/ 

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRA] + pDevConfig->forwardlash),
                                           &(pGrPriv->tilt2StepsLut[GRA]), &fsteps );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating A: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }

               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRA], "%f", fsteps);
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_B_1:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRB].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRB] + pDevConfig->backlash) *
                              pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRB]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating B: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */
                    /* Translate via the lut */
                    /*printf(" 4 translating via lut: pDevConfig->newGratTilt[GRB] = %f\n", pDevConfig->newGratTilt[GRB]); */
                    /*printf(" 4 translating via lut: pDevConfig->backlash = %f\n", pDevConfig->backlash);*/ 
                    /*printf(" 4 translating via lut: &fsteps = %f\n", fsteps);*/

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRB] + pDevConfig->backlash),
                                           &(pGrPriv->tilt2StepsLut[GRB]), &fsteps );

                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating B: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }
               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRB], "%f", fsteps);
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_B_2:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRB].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRB]) * pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRB]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating B: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */
                   /*printf(" 5 translating via lut: pDevConfig->newGratTilt[GRB] = %f\n", pDevConfig->newGratTilt[GRB]); */
                    /*printf(" 5 translating via lut: pDevConfig->backlash = %f\n", pDevConfig->backlash); */
                    /*printf(" 5 translating via lut: &fsteps = %f\n", fsteps);*/

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRB]),
                                            &(pGrPriv->tilt2StepsLut[GRB]), &fsteps );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating B: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }

               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRB], "%f", fsteps);
               printf(" GRATING B MOTORSTEPS : %f\n", fsteps);
               GR_STEP_VALUE = fsteps;
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_B_3:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRB].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRB] + pDevConfig->forwardlash) *
                              pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRB]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating B: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */
                   /*printf(" 5 translating via lut: pDevConfig->newGratTilt[GRB] = %f\n", pDevConfig->newGratTilt[GRB]); */
                    /*printf(" 5 translating via lut: pDevConfig->backlash = %f\n", pDevConfig->backlash);*/ 
                    /*printf(" 5 translating via lut: &fsteps = %f\n", fsteps); */

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRB] + pDevConfig->forwardlash),
                                           &(pGrPriv->tilt2StepsLut[GRB]), &fsteps );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating B: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }

               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRB], "%f", fsteps);
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_C_1:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRC].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRC] + pDevConfig->backlash) *
                             pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRC]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating C: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRC] + pDevConfig->backlash),
                                           &(pGrPriv->tilt2StepsLut[GRC]), &fsteps );

                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating C: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }

               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRC], "%f", fsteps);
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_C_2:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRC].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRC]) * pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRC]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating C: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRC]),
                                            &(pGrPriv->tilt2StepsLut[GRC]), &fsteps );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating C: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }

               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRC], "%f", fsteps);
               printf(" GRATING C MOTORSTEPS : %f\n", fsteps);
               GR_STEP_VALUE = fsteps;
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_C_3:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRC].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRC] + pDevConfig->forwardlash) * pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRC]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating C: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRC] + pDevConfig->forwardlash),
                                           &(pGrPriv->tilt2StepsLut[GRC]), &fsteps );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating C: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }

               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRC], "%f", fsteps);
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_D_1:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRD].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRD] + pDevConfig->backlash) *
                             pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRD]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating D: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRD] + pDevConfig->backlash),
                                           &(pGrPriv->tilt2StepsLut[GRD]), &fsteps );

                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating D: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }

               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRD], "%f", fsteps);
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_D_2:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRD].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRD]) * pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRD]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating D: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRD]),
                                           &(pGrPriv->tilt2StepsLut[GRD]), &fsteps );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating D: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }
                    
               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRD], "%f", fsteps);
               printf(" GRATING D MOTORSTEPS : %f\n", fsteps);
               GR_STEP_VALUE = fsteps;
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_MV_GRATING_D_3:

               /* If we havent succeeded in loading a lookup table then just 
                  apply the scale factor from the D input field */

               if ( pGrPriv->tilt2StepsLut[GRD].numberOfEntries < 2 )
               {
                    fsteps = (pDevConfig->newGratTilt[GRD] + pDevConfig->forwardlash) *
                             pDevConfig->tiltScale;
               }
               else
               {
                    /* Check the LUT */

                    status = gmosLutCheck( &(pGrPriv->tilt2StepsLut[GRD]) );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating D: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                        break;
                    }

                    /* Translate via the lut */

                    status = gmosLutApply( (pDevConfig->newGratTilt[GRD] + pDevConfig->forwardlash),
                                           &(pGrPriv->tilt2StepsLut[GRD]), &fsteps );
                    if ( status == GMOSLUT_S_OK )
                    {
                        status = DAR_S_SUCCESS;
                    }
                    else
                    {
                        gmosLutMessage( status, GR_ERROR_MESSAGE_SIZE, errMsg );
                        GRDEBUG(DAR_MSG_ERROR, "grDoTask: Grating D: %s\n", errMsg);
			assPriv->keepIndex = TRUE;
                        status = GR_LUT_ERROR;
                    }

               }

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               sprintf( pGrPriv->position[GRD], "%f", fsteps);
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_BC_GRATING_ALL:

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               for (i=0; i < TRT; i++)
                    (void) strncpy( pGrPriv->position[i], GR_NAME_POS_BC, MAX_STRING_SIZE-1 );
               pGrPriv->mode = DAR_MODE_MOVE;
               semGive (pGrPriv->mutexSem);
               break;

          case GR_PARK_ALL:

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);

               for (i=0; i < TRT; i++)
                    (void) strncpy( pGrPriv->position[i], GR_NAME_POS_PARK, MAX_STRING_SIZE-1 );

	       pGrPriv->mode = DAR_MODE_MOVE;
	       pGrPriv->velocity[TRT] = GR_VELOCITY_TURRET;     /* Correct turret move velocity */

	       site = *((long *) GR_SITE_CODE);
	       if (site != SITE_MK && site != SITE_CP) {
printErr ("illegal site=%d\n", site);
                    GRDEBUG(DAR_MSG_WARNING,
			"grDoTask: Invalid site value=%d. Using MK.\n",
			site);
		    site = SITE_MK;
	       }
printErr ("site=%d\n", site);

	       switch (pGrPriv->parkPosition)
	       {
	       case (GRA):
                    (void) strncpy( pGrPriv->position[TRT],
				GR_NAME_POS_POSLDA, MAX_STRING_SIZE-1 );
		    if (site == SITE_MK)
		        pDevConfig->newTurretPos = GRB;
		    else
		        pDevConfig->newTurretPos = GRD;
		    break;

	       case (GRB):
                    (void) strncpy( pGrPriv->position[TRT],
				GR_NAME_POS_POSLDB, MAX_STRING_SIZE-1 );
		    if (site == SITE_MK)
		        pDevConfig->newTurretPos = GRC;
		    else
		        pDevConfig->newTurretPos = GRA;
		    break;

	       case (GRC):
                    (void) strncpy( pGrPriv->position[TRT],
				GR_NAME_POS_POSLDC, MAX_STRING_SIZE-1 );
		    if (site == SITE_MK)
		        pDevConfig->newTurretPos = GRD;
		    else
		        pDevConfig->newTurretPos = GRB;
		    break;

	       case (GRD):
                    (void) strncpy( pGrPriv->position[TRT],
				GR_NAME_POS_POSLDD, MAX_STRING_SIZE-1 );
		    if (site == SITE_MK)
		        pDevConfig->newTurretPos = GRA;
		    else
		        pDevConfig->newTurretPos = GRC;
		    break;

	       default:
                    /* If the grating number is not recognised use
		     * the default parking position
		     */
                    GRDEBUG(DAR_MSG_WARNING,
		    "grDoTask: Invalid parking position=%d. Using default.\n",
                     pGrPriv->parkPosition );
                    (void) strncpy( pGrPriv->position[TRT],
			GR_NAME_POS_PARK, MAX_STRING_SIZE-1 );
                    pDevConfig->newTurretPos = GRB;     /* Default is B */
                    break;
	       }
#if 0
#ifdef MK 
	       switch (pGrPriv->parkPosition)
	       {
	       case (GRA):
                    (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSLDA, MAX_STRING_SIZE-1 );
		    pDevConfig->newTurretPos = GRB;
		    break;

	       case (GRB):
                    (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSLDB, MAX_STRING_SIZE-1 );
		    pDevConfig->newTurretPos = GRC;
		    break;

	       case (GRC):
                    (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSLDC, MAX_STRING_SIZE-1 );
		    pDevConfig->newTurretPos = GRD;
		    break;

	       case (GRD):
                    (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSLDD, MAX_STRING_SIZE-1 );
		    pDevConfig->newTurretPos = GRA;
		    break;

	       default:
                    /* If the grating number is not recognised use the default parking position */

                    GRDEBUG(DAR_MSG_WARNING, "grDoTask: Invalid parking position=%d. Using default.\n",
                       pGrPriv->parkPosition );
                    (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_PARK, MAX_STRING_SIZE-1 );
                    pDevConfig->newTurretPos = GRB;     /* Default position is assumed to be B */
                    break;

	       }
#else

switch (pGrPriv->parkPosition)
               {
               case (GRA):
                    (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSLDA, MAX_STRING_SIZE-1 );
                    pDevConfig->newTurretPos = GRD;
                    break;

               case (GRB):
                    (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSLDB, MAX_STRING_SIZE-1 );
                    pDevConfig->newTurretPos = GRA;
                    break;

               case (GRC):
                    (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSLDC, MAX_STRING_SIZE-1 );
                    pDevConfig->newTurretPos = GRB;
                    break;

               case (GRD):
                    (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_POSLDD, MAX_STRING_SIZE-1 );
                    pDevConfig->newTurretPos = GRC;
                    break;

               default:
                    /* If the grating number is not recognised use the default parking position */

                    GRDEBUG(DAR_MSG_WARNING, "grDoTask: Invalid parking position=%d. Using default.\n",
                       pGrPriv->parkPosition );
                    (void) strncpy( pGrPriv->position[TRT], GR_NAME_POS_PARK, MAX_STRING_SIZE-1 );
                    pDevConfig->newTurretPos = GRB;     /* Default position is assumed to be B */
                    break;
               }
#endif
#endif

               semGive (pGrPriv->mutexSem);
               break;

          case GR_TURRET_POWER_OFF:
                 if(
			(dbPutLink(&(GR_TURRET_POWER),DBR_LONG,&turretPowerOff,nRequest))!=DAR_S_SUCCESS
 		   )
               {
                    status = GR_TRIG_FAILED;
               }
               break;

          case GR_TURRET_POWER_ON:
                 if(
			(dbPutLink(&(GR_TURRET_POWER),DBR_LONG,&turretPowerOn,nRequest))!=DAR_S_SUCCESS	
		   )
               {
                    status = GR_TRIG_FAILED;
               }
               break;

          default:
               GRDEBUG(DAR_MSG_ERROR, "grDoTask: Invalid task=%d\n", taskItem->task );
               status = GR_INVALID_TASK;
               strncpy( errMsg, "Invalid task number", MAX_STRING_SIZE-1 );

          }
     }

     pGrPriv->askBarCode = FALSE;
     pGrPriv->raiseTurret = FALSE;
     pGrPriv->turretPowerChange = FALSE;

     if ( status == DAR_S_SUCCESS )
     {
          /* first check whether we're waiting for a barcode read */

          if ( taskItem->task == GR_READ_BARCODE )
          {
               GRDEBUG(DAR_MSG_FULL, "grDoTask: Requested a barcode%c\n", ' ');

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               pGrPriv->cmdActive = TRUE;
               pGrPriv->askBarCode = TRUE;
               semGive (pGrPriv->mutexSem);

               assStartTimer( par, &status, GR_BARCODE_READ_DELAY );
               if (status != DAR_S_SUCCESS)
               {
                   GRDEBUG(DAR_MSG_WARNING,
                      "grDoTask: failed to start watchdog timer for barcode read. status=%ld\n",
                      status);
               }
          }
          else if ( taskItem->task == GR_TURRET_UP || taskItem->task == GR_TURRET_DOWN)
          {
               GRDEBUG(DAR_MSG_FULL, "grDoTask: Lifter %s requested.\n",
                  (taskItem->task == GR_TURRET_UP ? "UP" : "DOWN"));

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               pGrPriv->cmdActive = TRUE;
               pGrPriv->raiseTurret = TRUE;
               semGive (pGrPriv->mutexSem);

               assStartTimer( par, &status, lifterWaitTime );
               if (status != DAR_S_SUCCESS)
               {
                   GRDEBUG(DAR_MSG_WARNING,
                      "grDoTask: failed to start watchdog timer for lifter. status=%ld\n",
                      status);
               }
          }
          else if ( (taskItem->task == GR_WRITE_CONFIG_BAK) || (taskItem->task == GR_WRITE_CONFIG) )
          {
               if ( grEmptyList( par, FALSE ) == GR_EMPTY_LIST )
               {
                    GRDEBUG(DAR_MSG_FULL, "grDoTask, all tasks done%c\n", ' ');

                    /*
                     * The command is finished. It is assumed the assembly record will
                     * keep the informational messages written to its message field.
                     * The assembly record private structure is fudged to make sure the
                     * message is not regarded as an error..
                     */

                    grTerminateTasks(par, status, par->mess);
               }
          }
          else if (taskItem->task == GR_TURRET_POWER_OFF || taskItem->task == GR_TURRET_POWER_ON)
          {
               GRDEBUG(DAR_MSG_FULL, "grDoTask: Turret power %s requested.\n",
                 (taskItem->task == GR_TURRET_POWER_ON ? "ON" : "OFF"));

               semTake (pGrPriv->mutexSem, WAIT_FOREVER);
               pGrPriv->cmdActive = TRUE;
               pGrPriv->turretPowerChange = TRUE;
               semGive (pGrPriv->mutexSem);

               assStartTimer( par, &status, GR_POWER_TIMEOUT );
               if (status != DAR_S_SUCCESS)
               {
                   GRDEBUG(DAR_MSG_WARNING,
                      "grDoTask: failed to start watchdog timer for turret power. status=%ld\n",
                      status);
               }
          }
          else
          {
               if ( (status = grExecuteTask( par, taskItem))  == DAR_S_SUCCESS )
               {
                    GRDEBUG(DAR_MSG_FULL, "grDoTask: Started task=%d\n", taskItem->task );
                    assStartTimer( par, &status, GR_COMMAND_TIMEOUT );
                    if (status != DAR_S_SUCCESS)
                    {
                        GRDEBUG(DAR_MSG_WARNING,
                           "grDoTask: failed to start watchdog timer after grExecuteTask. status=%ld\n",
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
           
          grTerminateTasks( par, status, errMsg );
     }

     return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grEmptyList
 *
 * INVOCATION:
 * status = grEmptyList (par, TRUE|FALSE); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 * (>) removeAll  (int ) Remove all tasks from the list, or only 1.
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

static long grEmptyList(ASSEMBLY_CONTROL_RECORD *par, int removeAll)
{
    long           status = DAR_S_SUCCESS;	/* Return function status.	*/
    GR_DEV_PRIVATE *pGrPriv;			/* Ptr to private dev. struct.	*/
    GR_LIST        *pList;			/* Ptr to item in a list.	*/

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    GRDEBUG(DAR_MSG_MAX, "grEmptyList: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grEmptyList: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grEmptyList: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

    /*
     *  Just return if list is empty.
     */

    if ( ellCount( &(pGrPriv->taskList) ) == 0 )
    {
        GRDEBUG(DAR_MSG_MIN, "grEmptyList: task list is already empty, ellCount=%d\n", 
                    ellCount( &(pGrPriv->taskList) ) );
        status = GR_EMPTY_LIST;
        return ( status );
    }

    if ( !removeAll )
    {
        /* 
         *  Delete only the first one.
         */

        pList = (GR_LIST *) ellGet( &(pGrPriv->taskList) ); 
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
            if (  ( pList = (GR_LIST *) ellGet( &pGrPriv->taskList ) ) == NULL )
            {
                break;
            }
            pList->item = NULL;
            free( pList );
        }

        if ( ellCount( &(pGrPriv->taskList) ) != 0 )
        {
            GRDEBUG(DAR_MSG_WARNING, "grEmptyList: task list did not empty, ellCount=%d\n",
                    ellCount( &(pGrPriv->taskList) ) );
        }
    }

    if ( ellCount( &(pGrPriv->taskList) ) == 0 )
    {
        status = GR_EMPTY_LIST;
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grExecuteTask
 *
 * INVOCATION:
 * status = grExecuteTask (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Execute a task on a device
 *
 * DESCRIPTION:
 * This function is called by grDoTask when the time comes to trigger
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

static long grExecuteTask(ASSEMBLY_CONTROL_RECORD *par, GR_TASK_LIST *taskItem)
{
    unsigned short      dir;			/* Directive value.		*/
    unsigned short      mode;			/* Mode value.			*/
    double              vel;			/* Velocity.			*/
    char                errMsg[GR_ERROR_MESSAGE_SIZE];/* Error message.		*/
    long                nRequest = 1;		/* Number to send on link.	*/
    unsigned short      returnedAck;		/* Returned ack from device.	*/
    long                status = DAR_S_SUCCESS;	/* Function status.		*/
    int                 i;			/* Counter			*/
    int                 useDevice[GR_NUM_DEVICES];	/* Use device flags     */

    GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/
    struct link    *dirLink;		/* Dir's links.			*/
    struct link    *modLink;		/* Mod's links.			*/
    struct link    *posLink;		/* Pos's links.			*/
    struct link    *velLink;		/* Vel's links.			*/
    struct link    *ackLink;		/* Acks's links.		*/

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grExecuteTask: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grExecuteTask: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

    /*
     *  Set up variables.
     */

    dir = grTranslateDir( DAR_DIR_START );
    mode = pGrPriv->mode;
    
    dirLink = pGrPriv->grADevice.dirLink;
    modLink = pGrPriv->grADevice.modLink;
    posLink = pGrPriv->grADevice.posLink;
    velLink = pGrPriv->grADevice.velLink;
    ackLink = pGrPriv->grADevice.ackLink;


    /*
     * Set up the useDevice array, reflecting the 'busy expected' fields
     * defined for this particular task.
     */

    useDevice[0] = taskItem->grAResponse;
    useDevice[1] = taskItem->grBResponse;
    useDevice[2] = taskItem->grCResponse;
    useDevice[3] = taskItem->grDResponse;
    useDevice[4] = taskItem->trtResponse;    
    
    /*
     *  Cycle through output links, only do if useDevice flag is set.
     *  They should be sequential.
     */

    for ( i = 0 ; i < GR_NUM_DEVICES; 
         i++, modLink++, dirLink++, posLink++, velLink++, ackLink++ )
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

            GRDEBUG(DAR_MSG_MIN, "grExecuteTask: Putting POS=%s\n", pGrPriv->position[i] );
             CHECKSTAT((status = dbPutLink(posLink,DBR_STRING,pGrPriv->position[i],nRequest)),return(status));


            /*
             *  Write the Velocity if needed.
             */

            vel = pGrPriv->velocity[i];
            if ( vel > 0.0 )
            {

               GRDEBUG(DAR_MSG_FULL, "grExecuteTask: Putting VEL=%f\n", vel );
	         CHECKSTAT((status = dbPutLink(velLink,DBR_DOUBLE,&vel,nRequest)),return(status));

            }


            /*
             *  Write the mode.
             */

            GRDEBUG(DAR_MSG_FULL, "grExecuteTask: Putting MODE=%d\n", mode );
            CHECKSTAT((status = dbPutLink(modLink,DBR_SHORT,&mode,nRequest)),return(status));


            /*
             *  Write the directive.
             */

            GRDEBUG(DAR_MSG_FULL, "grExecuteTask: Putting DIR=%d\n", dir );
             CHECKSTAT((status = dbPutLink(dirLink,DBR_SHORT,(void *) (&dir),nRequest)),return(status));


            /*
             *  Check the response from the device.
             */

            /*CHECKSTAT((status = recGblGetFastLink(ackLink, (void *) par, 
                    &returnedAck)), return (status));*/
            CHECKSTAT((status = dbGetLink(ackLink,DBR_USHORT,&returnedAck,0,0)),return(status));
            GRDEBUG(DAR_MSG_FULL, "grExecuteTask: Got response: %d\n", returnedAck );

            if ( returnedAck != DAR_DEV_VAL_ACCEPT )
            {
                /*
                 *  Device rejected command. Empty the list, stop the timer, set
                 *  an error message.  May be in the middle of a command
                 *  or initially.
                 */

                GRDEBUG(DAR_MSG_ERROR, "grExecuteTask: Device %d rejected command\n", i);

                status = DAR_E_DEVICE;
                sprintf (errMsg, "Grating %s rejected command",
                   (i==0 ? "A" : (i==1 ? "B" : (i==2 ? "C" : (i==3 ? "D" : "turret")))));
                grTerminateTasks( par, status, errMsg );
                break;
            }
            else
            {
                semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                pGrPriv->cmdActive = TRUE;
                semGive (pGrPriv->mutexSem);
            }
        }
        else
        {
            GRDEBUG(DAR_MSG_ERROR, "grExecuteTask: VALS link not connected%c\n", ' ');
        }
      }
    }

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grIndexMode
 *
 * INVOCATION:
 * status = grIndexMode (par); 
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
 *    Build the task list by calling grBuildList()
 *    Start the task list by calling grDoTask()
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

static long grIndexMode(ASSEMBLY_CONTROL_RECORD *par)
{
    GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/
    GR_DEV_CONFIG  *pDevConfig;
    long status = DAR_S_SUCCESS;

    GRDEBUG(DAR_MSG_MAX, "grIndexMode: entry%c\n", ' ');

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
    pDevConfig = pGrPriv->pGratingPriv;

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL || pDevConfig == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grIndexMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grIndexMode: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

    /*
     *  Build the task list.
     */

    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
    pGrPriv->currentCmd = par->mode;
    semGive (pGrPriv->mutexSem);

    if ( grBuildList( par, DAR_MODE_INDEX ) != DAR_S_SUCCESS )
    {
        GRDEBUG(DAR_MSG_ERROR, "grIndexMode: grbuildList failed. status=%ld\n", status);
    }
    else if ( ( status = grDoTask( par ))  != DAR_S_SUCCESS )
    {

       /*
        *  Index task failed - turret position is now unknown
        */

        semTake (pGrPriv->mutexSem, WAIT_FOREVER);
        pDevConfig->currentTurretPos = -1;
        semGive (pGrPriv->mutexSem);

        GRDEBUG(DAR_MSG_ERROR, "grIndexMode: grDoTask failed. status=%ld\n", status);
    }
    
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grInitDeviceSupport
 *
 * INVOCATION:
 * status = grInitDeviceSupport (par); 
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
 *    Check that all of the 5 deviceControl record POS[1-5] links
 *      are not CONSTANT.
 *    Create device info structure for all 5 motors, relying on 
 *    	motor1=grA, motor2=grB, motor3=grC, motor4=grD, motor5=trt.
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

static long grInitDeviceSupport(ASSEMBLY_CONTROL_RECORD *par)
{
     GR_DEV_PRIVATE *pGrPriv;            /* Private dev. struct         */
     GR_DEV_CONFIG  *pDevConfig;
     int i;
     long status = DAR_S_SUCCESS;        /* Returned function status.	*/

     GRDEBUG(DAR_MSG_MAX, "grInitDeviceSupport: entry%c\n", ' ');

     /*
      *  Create private device support structure for this record.
      */

     pGrPriv = malloc (sizeof(GR_DEV_PRIVATE));
     if ( pGrPriv == NULL ) 
     {
          GRDEBUG(DAR_MSG_FATAL, "grInitDeviceSupport: malloc for GR_DEV_PRIVATE failed.%c\n", ' ');

          status = DAR_E_MALLOC;
          recGblRecordError (status, par, __FILE__ ":no room for device private");
          return( status );
     }

     /*
      *  Link this structure to the record support private structure.
      */
     
     assAttachPrivateStruct( par, (void *) pGrPriv ); 

     /*
      * Create the MUTEX semaphore to protect the private structure
      * during asynchronous callback access.
      */
     
     pGrPriv->mutexSem = NULL;
     if (  (pGrPriv->mutexSem = semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE) ) == NULL )
     {
          GRDEBUG(DAR_MSG_FATAL, "grInitDeviceSupport: semMCreate for pGrPriv->mutexSem failed.%c\n",
             ' ');

          status = DAR_E_MALLOC;
          recGblRecordError (status, par, __FILE__ ":no room for mutex creation");
          return( status );
     }

     /*
      *  Create a configuration structure for the mechanism and write the pointer
      *  to the private device support structure.
      */

     pDevConfig = (GR_DEV_CONFIG *) malloc (sizeof(GR_DEV_CONFIG));

     if ( pDevConfig == NULL ) 
     {
          GRDEBUG(DAR_MSG_FATAL, "grInitDeviceSupport: malloc for GR_DEV_CONFIG failed.%c\n", ' ');

          status = DAR_E_MALLOC;
          recGblRecordError (status, par, __FILE__ ":no room for device config");
          return( status );
     }

     pGrPriv->pGratingPriv = pDevConfig;

     /*
      * Initialize the barcode list
      */

     grClearBarcodes (par);

     semTake (pGrPriv->mutexSem, WAIT_FOREVER);

     /*
      * Intialise the rest of the device configuration structure.
      */

     pDevConfig->currentTurretPos = -1;
     pDevConfig->magic            = GR_MAGIC;

     /*
      * Initialize private record variables.
      */  
     
     pGrPriv->pRecord     = par;
     pGrPriv->cmdActive   = FALSE; 
     pGrPriv->grDFinished = FALSE; 
     pGrPriv->grAFinished = FALSE; 
     pGrPriv->grBFinished = FALSE; 
     pGrPriv->grCFinished = FALSE; 
     pGrPriv->trtFinished = FALSE;

     for (i=0; i < GR_NUM_DEVICES; i++)
       pGrPriv->velocity[i] = 0.0;

     pGrPriv->mode        = -99;
     pGrPriv->busyMode    = -99;
     pGrPriv->askBarCode  = FALSE;
     pGrPriv->raiseTurret = FALSE;
     pGrPriv->turretPowerChange = FALSE;
     pGrPriv->currentCmd  = -99;
     pGrPriv->magic       = GR_MAGIC;

     /*
      *  Create device info structure for all 4 devices.
      */

     if (par->pos1.type == CONSTANT || par->pos2.type == CONSTANT || 
        par->pos3.type == CONSTANT || par->pos4.type == CONSTANT ||
        par->pos5.type == CONSTANT )
     {
          GRDEBUG(DAR_MSG_FATAL,
             "grInitDeviceSupport: Not all device control records connected.%c\n", ' ' );

          status = GR_BAD_DEV_CONNECT;
          recGblRecordError (status, par, ":Not all deviceControl rec's connected");
          semGive (pGrPriv->mutexSem);
          return( status );
     }

     pGrPriv->grADevice.dirLink = &par->odr1;
     pGrPriv->grADevice.posLink = &par->pos1;
     pGrPriv->grADevice.ackLink = &par->ack1;
     pGrPriv->grADevice.velLink = &par->vel1;
     pGrPriv->grADevice.modLink = &par->mod1;

     pGrPriv->grBDevice.dirLink = &par->odr2;
     pGrPriv->grBDevice.posLink = &par->pos2;
     pGrPriv->grBDevice.ackLink = &par->ack2;
     pGrPriv->grBDevice.velLink = &par->vel2;
     pGrPriv->grBDevice.modLink = &par->mod2;

     pGrPriv->grCDevice.dirLink = &par->odr3;
     pGrPriv->grCDevice.posLink = &par->pos3;
     pGrPriv->grCDevice.ackLink = &par->ack3;
     pGrPriv->grCDevice.velLink = &par->vel3;
     pGrPriv->grCDevice.modLink = &par->mod3;

     pGrPriv->grDDevice.dirLink = &par->odr4;
     pGrPriv->grDDevice.posLink = &par->pos4;
     pGrPriv->grDDevice.ackLink = &par->ack4;
     pGrPriv->grDDevice.velLink = &par->vel4;
     pGrPriv->grDDevice.modLink = &par->mod4;

     pGrPriv->trtDevice.dirLink = &par->odr5;
     pGrPriv->trtDevice.posLink = &par->pos5;
     pGrPriv->trtDevice.ackLink = &par->ack5;
     pGrPriv->trtDevice.velLink = &par->vel5;
     pGrPriv->trtDevice.modLink = &par->mod5;


     /* 
      *  Initialize the task list.
      */

     ellInit( &pGrPriv->taskList );

     semGive (pGrPriv->mutexSem);

     GRDEBUG(DAR_MSG_FULL, "grInitDeviceSupport: finished, status=%ld\n", status );

     return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grInitMode
 *
 * INVOCATION:
 * status = grInitMode (par); 
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
 *    Build the task list ( grBuildList() ).
 *    Start the task list ( grDoTask() ).
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

static long grInitMode(ASSEMBLY_CONTROL_RECORD *par)
{
    GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/
    long status = DAR_S_SUCCESS;	/* Return function status.	*/

    GRDEBUG(DAR_MSG_MAX, "grInitMode: entry%c\n", ' ');

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL  )
     {
         GRDEBUG(DAR_MSG_FATAL, "grInitMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grInitMode: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

    /* Read in the current grating configuration files */

    if ((status = grReadConfig( par )) != DAR_S_SUCCESS)
    {
         return( status );
    }

     /*
      * Display the current grating barcode configuration as a MIN debug message.
      */

     if ( par->dbug >= DAR_MSG_MIN )
     {
         grShowConfig( par );
     }

    /*
     *  Build the task list for initialising the grating devices.
     */

    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
    pGrPriv->currentCmd = par->mode;
    semGive (pGrPriv->mutexSem);

    /*
     * Ensure the task list is empty before building the new list
     */

    grEmptyList( par, TRUE );

    if ( (status = grBuildList( par, DAR_MODE_INIT )) != DAR_S_SUCCESS )
    {
        GRDEBUG(DAR_MSG_ERROR, "grInitMode: grbuildList failed. status=%ld\n", status);
    }
    else if ( ( status = grDoTask( par )) != DAR_S_SUCCESS )
    {
        /*
         *  Start the task list, which will trigger all devices to init.
         */

        GRDEBUG(DAR_MSG_ERROR, "grInitMode: grDoTask failed. status=%ld\n", status);
    }
    
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grMoveMode
 *
 * INVOCATION:
 * status = grMoveMode (par); 
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

static long grMoveMode(ASSEMBLY_CONTROL_RECORD *par)
{
    GR_DEV_PRIVATE *pGrPriv;			/* Ptr to private dev. struct.	*/
    long           status = DAR_S_SUCCESS;	/* Return function status.	*/

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    GRDEBUG(DAR_MSG_MAX, "grMoveMode: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL  )
     {
         GRDEBUG(DAR_MSG_FATAL, "grMoveMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grMoveMode: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }


    /*
     *  Build the task list based on the mode & position of the grating1.
     */

    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
    pGrPriv->currentCmd = par->mode;
    semGive (pGrPriv->mutexSem);

    if ( par->mode == DAR_MODE_MOVE &&
            ( status = grBuildList( par, DAR_MODE_MOVE ))  != DAR_S_SUCCESS )
    {
        GRDEBUG(DAR_MSG_ERROR, "grMoveMode: grbuildList for move failed. status=%ld\n", status);
    }
    else if ( par->mode == DAR_MODE_PARK &&
            (status = grBuildList( par, DAR_MODE_PARK ) ) != DAR_S_SUCCESS )
    {
        GRDEBUG(DAR_MSG_ERROR, "grMoveMode: grbuildList for park failed. status=%ld\n", status);
    }

    /*
     *  Start the task list, as long as there is something more to do.
     */

    else if ( status == DAR_S_SUCCESS &&
            ( status = grDoTask( par ) ) != DAR_S_SUCCESS )
    {
        GRDEBUG(DAR_MSG_ERROR, "grMoveMode: grDoTask failed. status=%ld\n", status);
    }
    
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grStopDirective
 *
 * INVOCATION:
 * status = grStopDirective (par); 
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

static long grStopDirective
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
    GR_LIST      *taskRequested = NULL;	/* Ptr to task in task list.	*/
    GR_TASK_LIST *newTask = NULL;	/* Ptr to new task.		*/
    GR_TASK_LIST *oldTask = NULL;	/* Ptr to old task.		*/
    struct link  *dirLink;		/* Dir's link.			*/
    long         status = DAR_S_SUCCESS;	/* Return function status.	*/
    GR_DEV_PRIVATE *pGrPriv;		/* Private dev. support struc.	*/
    unsigned short dir;			/* Directive to send.		*/
    long         nRequest = 1;		/* Number of requested.		*/

    GRDEBUG(DAR_MSG_MAX, "grStopDirective: entry%c\n", ' ');

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grStopDirective: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grStopDirective: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

    dirLink = &par->odr1;

    /*
     *  If a command is active and that command is move, park, test, init,
     *  do not stop the movement.
     */

    if ( pGrPriv->cmdActive && 
        ( pGrPriv->currentCmd == DAR_MODE_MOVE || 
          pGrPriv->currentCmd == DAR_MODE_INIT || 
          pGrPriv->currentCmd == DAR_MODE_TEST ||
          pGrPriv->currentCmd == DAR_MODE_PARK ) )
    {
        GRDEBUG(DAR_MSG_LOG, "grStopDirective: ignoring STOP directive%c\n", ' ');
        return( status );
    }
    else if ( pGrPriv->cmdActive )
    {
        /* 
         * Otherwise, stop between tasks - for both INDEX or UPDATE.
         */

        /*
         *  Make sure the task list is not empty.
         */

        dir = grTranslateDir( DAR_DIR_STOP );
        if ( ellCount( &(pGrPriv->taskList) ) == 0 || 
                    ( taskRequested = 
                    (GR_LIST *) ellFirst( &(pGrPriv->taskList) )) == NULL )
        {
            GRDEBUG(DAR_MSG_ERROR, "grStopDirective: taskList empty, ellCount=%d\n", 
                        ellCount( &(pGrPriv->taskList) ) );

            status = GR_TASK_LIST_EMPTY;
            grTerminateTasks( par, status, "Grating task list is empty" );
            return ( status );
        }
        oldTask = ( GR_TASK_LIST *) taskRequested->item;


        /*
         *  Determine where to send stops to, and write that into
         *  the newTask list that we will add later.
         */

        newTask = malloc( sizeof ( GR_TASK_LIST ) );
        if ( newTask == NULL )
        {
            GRDEBUG(DAR_MSG_FATAL, "grStopDirective: failed to malloc new task list%c\n", ' ' );

            status = DAR_E_MALLOC;
            grTerminateTasks( par, status, "Grating, insufficient memory for new task");
            return ( status );
        }

        newTask->grAResponse = ( oldTask->grAResponse && 
                !pGrPriv->grAFinished )? TRUE : FALSE;
        newTask->grDResponse = ( oldTask->grDResponse && 
                !pGrPriv->grDFinished )? TRUE : FALSE;
        newTask->grBResponse = ( oldTask->grBResponse && 
                !pGrPriv->grBFinished )? TRUE : FALSE;
        newTask->grCResponse = ( oldTask->grCResponse && 
                !pGrPriv->grCFinished )? TRUE : FALSE;
        newTask->trtResponse = ( oldTask->trtResponse && 
                !pGrPriv->trtFinished )? TRUE : FALSE;
        newTask->task = GR_STOP_TASK;

        semTake (pGrPriv->mutexSem, WAIT_FOREVER);
        if (!newTask->grAResponse) pGrPriv->grAFinished = FALSE;
        if (!newTask->grBResponse) pGrPriv->grBFinished = FALSE;
        if (!newTask->grCResponse) pGrPriv->grCFinished = FALSE;
        if (!newTask->grDResponse) pGrPriv->grDFinished = FALSE;
        if (!newTask->trtResponse) pGrPriv->trtFinished = FALSE;
        semGive (pGrPriv->mutexSem);

        /*
         *  Empty the list and attach the new one.
         */

        grEmptyList( par, TRUE );
        taskRequested = malloc( sizeof ( GR_LIST ) );
        if ( taskRequested == NULL )
        {
           GRDEBUG(DAR_MSG_FATAL,
              "grStopDirective: Memory allocation failure for stop task%c\n", ' ' );

           status = DAR_E_MALLOC;
           grTerminateTasks( par, status, "Grating, insufficient memory for new task");
           return (status);
        }
        taskRequested->item = (void *) newTask;
        ellAdd( (ELLLIST *) &(pGrPriv->taskList), &(taskRequested->node) );
        taskRequested = ( GR_LIST *) ellNext( &(taskRequested->node) );


        /*
         *  Send the stops to the required devices.
         */

        if ( newTask->grAResponse && !pGrPriv->grAFinished)
        {
            /*
             *  Send a stop to GratingA.
             */

            GRDEBUG(DAR_MSG_FULL, "grStopDirective: sending STOP to grating A%c\n",
                            ' ');
              status = dbPutLink(pGrPriv->grADevice.dirLink,DBR_SHORT,(void *) (&dir),nRequest);
        }
        if ( status == DAR_S_SUCCESS && newTask->grDResponse && !pGrPriv->grDFinished )
        {
            /*
             *  Send a stop to GratingD.
             */

            GRDEBUG(DAR_MSG_FULL, "grStopDirective: sending STOP to grating D%c\n",
                            ' ');
             status = dbPutLink(pGrPriv->grDDevice.dirLink,DBR_SHORT,(void *) (&dir),nRequest); 
        }
        if (  status == DAR_S_SUCCESS && newTask->grBResponse  
                && !pGrPriv->grBFinished )
        {
            /*
             *  Send a stop to GratingB.
             */

            GRDEBUG(DAR_MSG_FULL, "grStopDirective: sending STOP to grating B%c\n",
                            ' ');
              status = dbPutLink(pGrPriv->grBDevice.dirLink,DBR_SHORT,(void *) (&dir),nRequest);
        }
        if (  status == DAR_S_SUCCESS && newTask->grCResponse 
                && !pGrPriv->grCFinished)
        {
            /*
             *  Send a stop to GratingC.
             */

            GRDEBUG(DAR_MSG_FULL, "grStopDirective: sending STOP to grating D%c\n",
                            ' ');
              status = dbPutLink(pGrPriv->grCDevice.dirLink,DBR_SHORT,(void *) (&dir),nRequest);
        }
        if (  status == DAR_S_SUCCESS && newTask->trtResponse 
                && !pGrPriv->trtFinished)
        {
            /*
             *  Send a stop to the turret.
             */

            GRDEBUG(DAR_MSG_FULL, "grStopDirective: sending STOP to turret%c\n",
                            ' ');
              status = dbPutLink(pGrPriv->trtDevice.dirLink,DBR_SHORT,(void *) (&dir),nRequest);
        }

        /*
         *  Start the timer.
         */

        if ( status == DAR_S_SUCCESS )
        {
            assStartTimer( par, &status, GR_COMMAND_SHORT_TIMEOUT );
            if (status != DAR_S_SUCCESS)
            {
                GRDEBUG(DAR_MSG_WARNING,
                   "grStopDirective: failed to start watchdog timer. status=%ld\n",
                   status);
            }
        }
        else
        {
            grTerminateTasks( par, status, "Grating, failed to write STOP directive" );
        }

    }
    else
    {
        /*
         *  No command currently active, just tell them we finished fine.
         */

        GRDEBUG(DAR_MSG_WARNING, "grStopDirective: no command active - STOP directive ignored%c\n",
                    ' ');
        grTerminateTasks( par, status, "Grating, no command active" );
    }

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grTaskCheck
 *
 * INVOCATION:
 * status = grTaskCheck (par); 
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

static long grTaskCheck(ASSEMBLY_CONTROL_RECORD *par)
{
     long status = DAR_S_SUCCESS;
     GR_LIST *pList = NULL;		/* Ptr to list.			*/
     GR_TASK_LIST *taskPerformed = NULL;/* Task performed.		*/
     GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/
     GR_DEV_CONFIG  *pDevConfig;
     long        nRequest = 1;		/* Number to send on link.	*/
     long barcode = GR_NO_BARCODE;
     long turretPosition;
     float tiltAngle;
     float zeroPointZero = 0.0;

     pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pGrPriv->pGratingPriv;

     GRDEBUG(DAR_MSG_MAX, "grTaskCheck: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL || pDevConfig == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grTaskCheck: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grTaskCheck: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

     /*
      *  Depending on the task performed, check different sensor words.
      */
    
     if ( ellCount( &(pGrPriv->taskList) ) == 0 || 
          ( pList = (GR_LIST *) ellFirst( &(pGrPriv->taskList) )) == NULL )
     {
          GRDEBUG(DAR_MSG_MIN, "grTaskCheck: grating task list empty, ellCount=%d\n", 
                ellCount( &(pGrPriv->taskList) ) );
          status = GR_LAST_TASK;

          return ( status );
     }

     taskPerformed = ( GR_TASK_LIST *) pList->item;

     switch( taskPerformed->task )
     {

     case GR_STOP_TASK:
     case GR_INIT_ALL:
     case GR_INDEX_ALL:
     case GR_INDEX_TURRET:
     case GR_INDEX_GRATING_A:
     case GR_INDEX_GRATING_B:
     case GR_INDEX_GRATING_C:
     case GR_INDEX_GRATING_D:

          /*
           * Set the current grating tilt and ID to "unknown" after initialisation
           * or stopping a movement.
           */

          barcode = GR_NO_BARCODE;
            if ((status = dbPutLink(&(par->sot),DBR_LONG,&barcode,nRequest)))
          {
                 dbPutLink(&(GR_TILT_VALUE),DBR_FLOAT,&zeroPointZero,nRequest);
          }
          else 
          {
                 status = dbPutLink (&(GR_TILT_VALUE),DBR_FLOAT,&zeroPointZero,nRequest);
          }

          break;

     case GR_TEST_ALL:
     case GR_BC_GRATING_ALL:
     case GR_MV_TURRET_A:
     case GR_MV_TURRET_B:
     case GR_MV_TURRET_C:
     case GR_MV_TURRET_D:
     case GR_MV_TURRET_BCA:
     case GR_MV_TURRET_BCB:
     case GR_MV_TURRET_BCC:
     case GR_MV_TURRET_BCD:
     case GR_WRITE_CONFIG_BAK:
     case GR_WRITE_CONFIG:

          /* These commands don't need any checks */

          break;

     case GR_PARK_ALL:
     case GR_PARK_TURRET:
     case GR_PARK_GRATING_A:
     case GR_PARK_GRATING_B:
     case GR_PARK_GRATING_C:
     case GR_PARK_GRATING_D:

          /* Set the current grating tilt to zero after parking */

            status = dbPutLink(&(GR_TILT_VALUE),DBR_FLOAT, &zeroPointZero,nRequest);
          break;

     case GR_MV_GRATING_A_1:

          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_A_2:

          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_A_3:

	  tiltAngle = pDevConfig->newGratTilt[GRA];
            status = dbPutLink(&(GR_TILT_VALUE),DBR_FLOAT,&tiltAngle,nRequest);
          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_B_1:

          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_B_2:

          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_B_3:

	  tiltAngle = pDevConfig->newGratTilt[GRB];
          status = dbPutLink(&(GR_TILT_VALUE),DBR_FLOAT,&tiltAngle,nRequest);
          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_C_1:

          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_C_2:

          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_C_3:

	  tiltAngle = pDevConfig->newGratTilt[GRC];
          status = dbPutLink(&(GR_TILT_VALUE),DBR_FLOAT,&tiltAngle,nRequest);
          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_D_1:

          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_D_2:

          turretPosition = grReadSwitches( par);
          break;

     case GR_MV_GRATING_D_3:

	  tiltAngle = pDevConfig->newGratTilt[GRD];
          status = dbPutLink(&(GR_TILT_VALUE),DBR_FLOAT,&tiltAngle,nRequest);
          turretPosition = grReadSwitches( par);
          break;

     case GR_READ_BARCODE:

          /*
           * An invalid barcode error is ignored so a complete database can be built up
           * even if some barcodes fail.
           */

          status = grCheckBarcodeId(par);
          break;

     case GR_TURRET_UP:

          status = grCheckLifter(par, GR_LIFTER_RAISED);
          if ( status != DAR_S_SUCCESS )
          {
               GRDEBUG(DAR_MSG_ERROR, "grTaskCheck: Lifter did not raise properly%c\n", ' ');
               return (status);
          }

          /*
           * Set the grating ID to "unknown" and grating tilt to zero each time
           * the turret is raised
           */

          barcode = GR_NO_BARCODE;


          if ((status = dbPutLink(&(par->sot),DBR_LONG,&barcode,nRequest)))
	  {
	       dbPutLink(&(GR_TILT_VALUE),DBR_FLOAT,&zeroPointZero,nRequest);
          }
	  else
	  {
	       status = dbPutLink(&(GR_TILT_VALUE),DBR_FLOAT,&zeroPointZero,nRequest);
          }
          break;

     case GR_TURRET_DOWN:

          status = grCheckLifter(par, GR_LIFTER_ENGAGED);
          if ( status != DAR_S_SUCCESS )
          {
               GRDEBUG(DAR_MSG_ERROR, "grTaskCheck: Lifter did not lower properly%c\n", ' ');
               return (status);
          }

          barcode = GR_NO_BARCODE;
          if ( status == DAR_S_SUCCESS )
          {
               turretPosition = grReadSwitches( par);

               if (turretPosition >= 0 && turretPosition <= (GR_NUM_GRATINGS - 1) &&
                  (turretPosition == pDevConfig->newTurretPos && pDevConfig->newTurretPos >= 0))
               {
                    barcode = pDevConfig->barcode[turretPosition];
               }
               else
               {
                    barcode = GR_NO_BARCODE;
                    status = GR_TURRET_ERROR;
               }

               if ( status == DAR_S_SUCCESS )
               {
                      status = dbPutLink(&(par->sot),DBR_LONG,&barcode,nRequest);
               }
               else
               {
                      dbPutLink(&(par->sot),DBR_LONG,&barcode,nRequest);
               }

               GRDEBUG(DAR_MSG_MIN, "grTaskCheck: current barcode=%ld\n", barcode );
          }
          break;

     case GR_TURRET_POWER_ON:

          if (par->simm != DAR_SIM_NONE)
          {
               GRDEBUG(DAR_MSG_MIN, "grTaskCheck: simulation, ignoring turret power%c\n", ' ' );
               status = DAR_S_SUCCESS;               
          }
          else if (grCheckTurretPower (par) == GR_TURRET_POWER_IS_ON)
          {
               status = DAR_S_SUCCESS;
          }
          else
          {
               GRDEBUG(DAR_MSG_ERROR, "grTaskCheck: turret power not on, task=%d\n",
                 taskPerformed->task );
               status = GR_POWER_FAILED;
          }
          break;

     case GR_TURRET_POWER_OFF:

          if (par->simm != DAR_SIM_NONE)
          {
               GRDEBUG(DAR_MSG_MIN, "grTaskCheck: simulation, ignoring turret power%c\n", ' ' );
               status = DAR_S_SUCCESS;               
          }
          else if (grCheckTurretPower (par) == GR_TURRET_POWER_IS_OFF)
          {
               status = DAR_S_SUCCESS;
          }
          else
          {
               GRDEBUG(DAR_MSG_ERROR, "grTaskCheck: turret power not off, task=%d\n",
                 taskPerformed->task );
               status = GR_POWER_FAILED;
          }
          break;

     default:

          GRDEBUG(DAR_MSG_ERROR, "grTaskCheck: Invalid task=%d\n", taskPerformed->task );
          status = GR_INVALID_TASK;
     }

     return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grTaskFinished
 *
 * INVOCATION:
 * status = grTaskFinished (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Determine if the current task has finished correctly.
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
 *	    call grDoTask() to execute the task.
 *	    and start the timer again.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 * Assumes that BUS1=GR1, BUS2=GR2, BUS3=GR3, BUS4=GR4, BUS5=TRT
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long grTaskFinished(ASSEMBLY_CONTROL_RECORD *par)
{
    long           status = DAR_S_SUCCESS;
    GR_LIST        *pTask = NULL;	/* Ptr to task list.		*/
    GR_TASK_LIST   *currentTask = NULL;	/* Ptr to current task.		*/
    GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/
    int            numCompleted;	/* Number of completed tasks.	*/
    long timerStatus = DAR_S_SUCCESS;   /* status word from assStopTimer  */

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    GRDEBUG(DAR_MSG_MAX, "grTaskFinished: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grTaskFinished: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grTaskFinished: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

    /*
     *  Make sure the list is not empty.  This is an error, if it occurs.
     */

    if ( ellCount( &(pGrPriv->taskList) ) == 0 )
    {
        GRDEBUG(DAR_MSG_ERROR, "grTaskFinished: grating task list empty, ellCount=%d\n",
            ellCount( &(pGrPriv->taskList) ) );

        status = DAR_E_DEVICE;
        grTerminateTasks( par, status, "Grating task list empty" );
    }

    /* 
     *  Point to current one in the list.
     */

    pTask = (GR_LIST *) ellFirst( &(pGrPriv->taskList) );
    currentTask = ( GR_TASK_LIST *) pTask->item;

    GRDEBUG(DAR_MSG_FULL, "grTaskFinished: current task=%d\n", currentTask->task);

    numCompleted = currentTask->grAResponse + currentTask->grBResponse +
            currentTask->grCResponse + currentTask->grDResponse + currentTask->trtResponse;

    GRDEBUG(DAR_MSG_MAX, "grTaskFinished: expect numCompleted=%d\n", numCompleted );

    numCompleted = pGrPriv->grAFinished + pGrPriv->grBFinished +
            pGrPriv->grCFinished + pGrPriv->grDFinished + pGrPriv->trtFinished; 

    GRDEBUG(DAR_MSG_MAX, "grTaskFinished: actual numCompleted=%d\n", numCompleted );

    if ( (  ( currentTask->grAResponse ==  pGrPriv->grAFinished ) &&
            ( currentTask->grBResponse == pGrPriv->grBFinished ) &&
            ( currentTask->grCResponse == pGrPriv->grCFinished ) &&
            ( currentTask->grDResponse == pGrPriv->grDFinished ) &&
            ( currentTask->trtResponse == pGrPriv->trtFinished ) ) ||
            ( pGrPriv->askBarCode == TRUE ) ||
            ( pGrPriv->turretPowerChange == TRUE ) ||
            ( pGrPriv->raiseTurret == TRUE ) )
    {
        /*
         *  Task is finished, stop the timer, check any necessary sensors
         *  based on the task that was performed & then see if there is 
         *  another one.
         */

        GRDEBUG(DAR_MSG_MIN, "grTaskFinished: Task finished%c\n", ' ');

        assStopTimer( par, &timerStatus );
        if (timerStatus != DAR_S_SUCCESS)
        {
            GRDEBUG(DAR_MSG_WARNING, "grTaskFinished: failed to stop watchdog timer. status=%ld\n",
               timerStatus);
        }

        status = grTaskCheck( par );

        if (status == GR_LAST_TASK)
        {
            GRDEBUG(DAR_MSG_WARNING, "grTaskFinished, GR_LAST_TASK%c\n", ' ');
            grTerminateTasks( par, status, "Grating task check reported GR_LAST_TASK");
        }
        else if (status == GR_LIFTER_FAILED)
        {
             GRDEBUG(DAR_MSG_ERROR, "grCheckLifter reported error%c\n", ' ');
             grTerminateTasks( par, status, "Grating lifter operation failed");
        }
        else if (status == GR_POWER_FAILED)
        {
             GRDEBUG(DAR_MSG_ERROR, "grCheckTurretPower reported error%c\n", ' ');
             grTerminateTasks( par, status, "Grating turret power change failed");
        }
        else if (status == GR_TURRET_ERROR)
        {
             GRDEBUG(DAR_MSG_ERROR, "Grating turret not at demand location%c\n", ' ');
             grTerminateTasks( par, status, "Grating turret not at demand location");
        }
        else if (status == GR_INVALID_BARCODE)
        {
             GRDEBUG(DAR_MSG_ERROR, "grTaskFinished, GR_INVALID_BARCODE%c\n", ' ');
             grTerminateTasks( par, status, "Grating barcode read failure");
        }
        else if (status != DAR_S_SUCCESS)
        {
            GRDEBUG(DAR_MSG_ERROR, "grTaskFinished, task check failed. status=%ld\n", status);
            grTerminateTasks( par, status, "Grating task check failed");
        }
        else if ( grEmptyList( par, FALSE ) == GR_EMPTY_LIST )
        {
            GRDEBUG(DAR_MSG_MIN, "grTaskFinished, all tasks done%c\n", ' ');
            grTerminateTasks( par, status, NULL );
        }
        else
        {
            /* There are more tasks, so kick them off */

            if ( ( status = grDoTask( par )) != DAR_S_SUCCESS && status != GR_LAST_TASK )
            {
                GRDEBUG(DAR_MSG_ERROR, "grTaskFinished: grDoTask failed. status=%ld\n", status);
            }
        }
    }
    else
    {
        GRDEBUG(DAR_MSG_MIN, "grTaskFinished: Task not finished yet%c\n", ' ');
    }

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grTestMode
 *
 * INVOCATION:
 * status = grTestMode (par); 
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
 * Test that all links to the 5 motors are not CONSTANT.
 * Test that the task list is empty.
 * Forward a TEST command to the underlying devices.
 *
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 * Assumes that deviceControl records are wired up with
 * 1=GR1, 2=GR2, 3=GR3, 4=GR4, 5=TRT
 *
 * DEFICIENCIES:
 * None known.
 *-
 ************************************************************************
 */

static long grTestMode
(
    ASSEMBLY_CONTROL_RECORD 
    			*par		/* (in)  Ptr to ass. rec.	*/
)
{
    long         status = DAR_S_SUCCESS;	/* Returned function status.	*/
    GR_DEV_PRIVATE 
                 *pGrPriv;			/* Ptr to private dev. struct.	*/


    GRDEBUG(DAR_MSG_MAX, "grTestMode: entry%c\n", ' ');

    /*
     * Check the integrity of the device private structures
     */

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    if ( pGrPriv == NULL )
    {
        GRDEBUG(DAR_MSG_FATAL, "grTestMode: Bad device private data structure%c\n", ' ' );
        assDisplayPrivateStruct( par );

        status = GR_BAD_STRUCTURE;
        assCommandFinish( par, status, "Bad internal data structures");
        return (status);
    }
    else if ( pGrPriv->magic != GR_MAGIC )
    {
        GRDEBUG(DAR_MSG_FATAL, "grTestMode: No magic value seen in data structure%c\n", ' ' );
        assDisplayPrivateStruct( par );

        status = GR_BAD_STRUCTURE;
        grTerminateTasks( par, status, "Bad internal data structures");
        return (status);
    }
    else if ( par->dbug >= DAR_MSG_MAX )
    {
        /* Display the private structure in MAX debug mode. */

        assDisplayPrivateStruct( par );
    }

    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
    pGrPriv->currentCmd = par->mode;
    semGive (pGrPriv->mutexSem);


    /*
     *  Check that all position, mode, directive, ack links
     *  are NOT CONSTANT.
     */

    if ( par->pos1.type == CONSTANT || par->ack1.type == CONSTANT || 
         par->mod1.type == CONSTANT || par->odr1.type == CONSTANT ||
         par->vel1.type == CONSTANT )
    {
        /*
         *  Check the Grating A links.
         */

        GRDEBUG(DAR_MSG_ERROR, "grTestMode: Grating A links not connected%c\n", ' ' );
        status = GR_BAD_DEV_CONNECT;
        grTerminateTasks( par, status, "Grating A links not connected");
    }
    else if ( par->pos2.type == CONSTANT || par->ack2.type == CONSTANT || 
              par->mod2.type == CONSTANT || par->odr2.type == CONSTANT ||
              par->vel2.type == CONSTANT )
    {
	/*
	 *  Check the Grating B links.
	 */

	GRDEBUG(DAR_MSG_ERROR, "grTestMode: Grating B links not connected%c\n", ' ' );
        status = GR_BAD_DEV_CONNECT;
	grTerminateTasks( par, status, "Grating B links not connected");
    }
    else if ( par->pos3.type == CONSTANT || par->ack3.type == CONSTANT || 
              par->mod3.type == CONSTANT || par->odr3.type == CONSTANT ||
              par->vel3.type == CONSTANT )
    {
        /*
         *  Check the Grating C links.
         */

        GRDEBUG(DAR_MSG_ERROR, "grTestMode: Grating C links not connected.%c\n", ' ' );
        status = GR_BAD_DEV_CONNECT;
        grTerminateTasks( par, status, "Grating C links not connected");
    }

    else if ( par->pos4.type == CONSTANT || par->ack4.type == CONSTANT || 
              par->mod4.type == CONSTANT || par->odr4.type == CONSTANT ||
              par->vel4.type == CONSTANT )
    {
        /*
         *  Check the Grating D links.
         */

        GRDEBUG(DAR_MSG_ERROR, "grTestMode: Grating D links not connected%c\n", ' ' );
        status = GR_BAD_DEV_CONNECT;
        grTerminateTasks( par, status, "Grating D links not connected");
    }
    else if ( par->pos5.type == CONSTANT || par->ack5.type == CONSTANT || 
              par->mod5.type == CONSTANT || par->odr5.type == CONSTANT ||
              par->vel5.type == CONSTANT )
    {
        /*
         *  Check the Turret.
         */

        GRDEBUG(DAR_MSG_ERROR, "grTestMode: Turret links not connected%c\n", ' ' );
        status = GR_BAD_DEV_CONNECT;
        grTerminateTasks( par, status, "Grating turret links not connected");
    }

    else if ( ellCount( &(pGrPriv->taskList) ) != 0 )
    {
        /*
         *  Error, task list is not empty.
         */

        GRDEBUG(DAR_MSG_ERROR, "grTestMode: Task list not empty%c\n", ' ' );
        status = GR_TASK_LIST_NOT_EMPTY;
        grTerminateTasks( par, status, "Grating task list not empty");
    }
    else
    {
        /* 
         *  All assembly tests pass successfully.
         *  Build the task list for testing the grating devices.
         */

        semTake (pGrPriv->mutexSem, WAIT_FOREVER);
        pGrPriv->currentCmd = par->mode;
        semGive (pGrPriv->mutexSem);

        if ( (status = grBuildList( par, DAR_MODE_TEST )) != DAR_S_SUCCESS )
        {
            GRDEBUG(DAR_MSG_ERROR, "grTestMode: grbuildList failed. status=%ld\n", status);
        }
        else if ( ( status = grDoTask( par )) != DAR_S_SUCCESS )
        {

            /*
             *  Start the task list, which will trigger all devices to test.
             */

            GRDEBUG(DAR_MSG_ERROR, "grTestMode: grDoTask failed. status=%ld\n", status);
        }
    }

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grTrackMode
 *
 * INVOCATION:
 * status = grTrackMode (par); 
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

static long grTrackMode(ASSEMBLY_CONTROL_RECORD *par)
{
    long status = DAR_S_SUCCESS;	/* Returned function status.	*/

    GRDEBUG(DAR_MSG_LOG, "grTrackMode: TRACK mode ignored%c\n", ' ');

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grTranslateDir
 *
 * INVOCATION:
 * status = grTranslateDir (par); 
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

static long grTranslateDir(unsigned short inDir)
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
 * grUpdateMode
 *
 * INVOCATION:
 * status = grUpdateMode (par); 
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

static long grUpdateMode(ASSEMBLY_CONTROL_RECORD *par)
{
    GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/
    long status = DAR_S_SUCCESS;	/* Return function status.	*/

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    GRDEBUG(DAR_MSG_MAX, "grUpdateMode: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grUpdateMode: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grUpdateMode: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

    semTake (pGrPriv->mutexSem, WAIT_FOREVER);
    pGrPriv->currentCmd = par->mode;
    semGive (pGrPriv->mutexSem);

    /* Build the task list based on position of the grating. */

    if ( (status = grBuildList( par, DAR_MODE_UPDATE ) ) != DAR_S_SUCCESS )
    {
        GRDEBUG(DAR_MSG_ERROR, "grUpdateMode: grbuildList failed. status=%ld\n", status);
    }

    /* Start the task list */

    else if ( ( status = grDoTask( par )) != DAR_S_SUCCESS )
    {
        GRDEBUG(DAR_MSG_ERROR, "grUpdateMode: grDoTask failed. status=%ld\n", status);
    }
    
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grReadConfig
 *
 * INVOCATION:
 * status = grReadConfig (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 * Read the current grating configuration file
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

static long grReadConfig(ASSEMBLY_CONTROL_RECORD *par)
{
     ASS_CONTROL_PRIVATE *pPriv = par->dpvt;	/* Private rec. support struct.	*/
     GR_DEV_PRIVATE *pGrPriv;			/* Ptr to private dev. struct.	*/
     GR_DEV_CONFIG  *pDevConfig;
     long status = DAR_S_SUCCESS;		/* Return function status.	*/
     char scratchBuf[256];
     char testBuf[256];
     int gratPos = 0;
     int gratingPos =0;
     float gratTilt = 0.0;
     long barcode = GR_NO_BARCODE;
     FILE *fp;
     int grind;
     double tilt;
     double steps;
     int n[GR_NUM_GRATINGS];

     pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pGrPriv->pGratingPriv;

     GRDEBUG(DAR_MSG_MAX, "grReadConfig: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL || pDevConfig == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grReadConfig: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grReadConfig: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }


     /*
      * Clear the internal barcode list.
      */

     grClearBarcodes( par );

     /* Read in zero point correction values */

     grReadZPC( par);


     /*
      * Assemble the name of the barcode file and attempt to open it. If the file cannot be
      * opened for reading just assume it has not been written yet. This is NOT an error,
      * since the file will be created on the first successful UPDATE. A file stub is created
      * to verify that a new file can be created. If this fails then there is a problem.
      */

     *scratchBuf = '\0';
     strncat( scratchBuf, par->tdir, MAX_STRING_SIZE );
     strcat( scratchBuf, "/" );
     strncat(scratchBuf, par->tfil, MAX_STRING_SIZE );

     if ( (fp = fopen(scratchBuf, "r")) == NULL)
     {
        if ( (fp = fopen(scratchBuf, "w")) == NULL)
        {
           GRDEBUG(DAR_MSG_ERROR,
              "grReadConfig: barcode file, %.40s, does not exist and cannot be created.\n",
              scratchBuf);

           /* A file open error should not affect the index status */
           pPriv->keepIndex = TRUE;

           status = GR_FILEIO_ERROR;
           grTerminateTasks(par, status, "Failed to open barcode file");

           return (status);
        }
        else
        {

           /*
            * Write the GR_LUT_FIRST_STRING string, close the file and return.
            * There is no point in attempting to read an empty file.
            */

           fprintf(fp, "%s\n", GR_LUT_FIRST_STRING);
           fclose (fp);

           GRDEBUG(DAR_MSG_WARNING,
              "grReadConfig: LUT file, %.40s, did not exist - created file stub.\n",
              scratchBuf);

           /* Warning message commented out - caused assembly record to ignore next assStartTimer. */
           /* assAddErrorMessage( par, "No barcode file - UPDATE needed"); */

           return (status);
        }
     }


     /* The first line should contain the string "grating". Check this first */

     if (fgets(scratchBuf, sizeof scratchBuf, fp) == NULL) 
     {
          GRDEBUG(DAR_MSG_ERROR, "grReadConfig: failed to read barcode file.%c\n", ' ');

          /* A file read error should not affect the index status */
          pPriv->keepIndex = TRUE;

          status = GR_FILEIO_ERROR;
          grTerminateTasks(par, status, "Failed to read barcode file");
     }
     else if (strncmp (scratchBuf, GR_LUT_FIRST_STRING, 7))
     {
          GRDEBUG(DAR_MSG_ERROR, "grReadConfig: could not find \"%s\" string\n",
             GR_LUT_FIRST_STRING);

          /* A file read error should not affect the index status */
          pPriv->keepIndex = TRUE;

          status = GR_FILEIO_ERROR;
          grTerminateTasks(par, status, "Incorrect barcode file type");
     }
     else
     {
          /* Process one entry at a time */
                          
          while (fgets(scratchBuf, sizeof scratchBuf, fp) != NULL) 
          {
               GRDEBUG(DAR_MSG_MAX, "grReadConfig:got a line: %.60s\n",  scratchBuf );

               if (*scratchBuf != '#' && *scratchBuf != '\n')     /* a comment or empty line, discard it */
               {
                    /*
                     * Read the configuration file into the local structure.
                     */

                    if (sscanf(scratchBuf, "%ld %d %f", &barcode, &gratPos, &gratTilt) != 3)
                    {
                         GRDEBUG(DAR_MSG_ERROR, "grReadConfig: LUT parse failed: %.60s\n",
                            scratchBuf);

                         /* A file read error should not affect the index status */
                         pPriv->keepIndex = TRUE;

                         status = GR_FILEIO_ERROR;
                         grTerminateTasks(par, status, "Failed to read barcode file");
                         break;
                    }
                    else if ( gratPos >= GR_NUM_GRATINGS )
                    {
                         GRDEBUG(DAR_MSG_ERROR, "grReadConfig: grating position out of range: %.60s\n",
                                 scratchBuf);

                         /* A file read error should not affect the index status */
                         pPriv->keepIndex = TRUE;

                         status = GR_FILEIO_ERROR;
                         grTerminateTasks(par, status, "Invalid grating pos in barcode file");
                         break;
                    }
                    else
                    {
                         /* Read line was successful. Add a new entry to the database,
                          * using a semaphore to prevent simultaneous access.
                          */

                         semTake (pGrPriv->mutexSem, WAIT_FOREVER);

                         pDevConfig->barcode[gratPos] = barcode;
                         pDevConfig->newGratTilt[gratPos] = gratTilt;
                         pDevConfig->characterized = TRUE;

                         semGive (pGrPriv->mutexSem);
                    }
               }
          }

          /* Report an error if no valid entries have been read */

          if (!pDevConfig->characterized)
          {
               GRDEBUG(DAR_MSG_WARNING,
                  "flReadConfig: Barcode file contains no entries.%c\n", ' ');
               /* Warning message commented out - caused assembly record to ignore next assStartTimer. */
               /* assAddErrorMessage( par, "No barcode entries - UPDATE needed"); */
          }
     }

     fclose (fp);

     /* Bail out of the status is bad at this point */

     if ( status != DAR_S_SUCCESS ) return (status);

     /*
      * Read in the auxiliary lookup table. This time an empty or non-existent file IS regarded
      * as an error.
      */
     n[GRA] = 0;
     n[GRB] = 0;
     n[GRC] = 0;
     n[GRD] = 0;

     for (gratingPos = 0; gratingPos < GR_NUM_GRATINGS; gratingPos++)
     {
     GRDEBUG(DAR_MSG_MIN,"TEST DEBUG: gratingPos = %d \n", gratingPos);
     *testBuf = '\0';
     sprintf(testBuf, "%d", pDevConfig->barcode[gratingPos]); 
     GRDEBUG(DAR_MSG_MIN,"TEST DEBUG: testBuf: %s \n ", testBuf);

     *scratchBuf = '\0';
     strncat( scratchBuf, par->tdir, MAX_STRING_SIZE );
     strcat( scratchBuf, "/aux_" );
     strncat( scratchBuf, testBuf, 9);
     strncat(scratchBuf, par->tfil, MAX_STRING_SIZE );
     GRDEBUG(DAR_MSG_MIN,"TEST DEBUG: scratchBuf :  %s  \n ", scratchBuf);
    
 
     if (!(fp = fopen(scratchBuf, "r")))
     {
          /* A file open error should not affect the index status */
          pPriv->keepIndex = TRUE;

          status = GR_FILEIO_ERROR;
          grTerminateTasks(par, status, "Failed to open aux LUT file");

          return(status);
     }

     /* The first line should contain the string "aux_grating". Check this first */

     if (fgets(scratchBuf, sizeof scratchBuf, fp) == NULL) 
     {
          GRDEBUG(DAR_MSG_ERROR, "grReadConfig: read failed%c\n", '.');

          /* A file read error should not affect the index status */
          pPriv->keepIndex = TRUE;

          status = GR_FILEIO_ERROR;
          grTerminateTasks(par, status, "Failed to read aux LUT file");
     }
     else if (strncmp (scratchBuf, GR_AUX_LUT_FIRST_STRING, 11))
     {
          GRDEBUG(DAR_MSG_ERROR, "grReadConfig: could not find \"%s\" string\n",
             GR_AUX_LUT_FIRST_STRING);

          /* A file read error should not affect the index status */
          pPriv->keepIndex = TRUE;

          status = GR_FILEIO_ERROR;
          grTerminateTasks(par, status, "Incorrect barcode file type");
     }
     else
     {

          while (fgets(scratchBuf, sizeof scratchBuf, fp) != NULL)
          {
               if (*scratchBuf != '#' && *scratchBuf != '\n') 
               {
                    if (sscanf(scratchBuf, "%d %lf %lf", &grind, &tilt, &steps) != 3)
                    {
                         GRDEBUG(DAR_MSG_ERROR, "grReadConfig: aux LUT parse failed: %.60s\n",
                            scratchBuf);

                         /* A file read error should not affect the index status */
                         pPriv->keepIndex = TRUE;

                         status = GR_FILEIO_ERROR;
                         grTerminateTasks(par, status, "Failed to read aux LUT file");

                         break;
                    }
                    else
                    {
                         /* read line was successful, populate internal store - if we have space left */


                         if (grind == (gratingPos+1)) {
                         if (n[grind - 1] <= MAX_GMOS_LUT_ENTRIES)
                         {
                              semTake (pGrPriv->mutexSem, WAIT_FOREVER);
                              pGrPriv->tilt2StepsLut[grind -1].input[n[grind -1]]  = tilt;
                              pGrPriv->tilt2StepsLut[grind -1].measured[n[grind -1]] = steps + pDevConfig->zpc[grind-1];
                              if ( (grind-1) == GRA )
                              {
				
                              printf("TEST DEBUG: STEPS = %f ZPC = %f \n", steps, pDevConfig->zpc[grind-1]);
                              printf("Test Debug: measured =  %f \n",pGrPriv->tilt2StepsLut[grind -1].measured[n[grind -1]]);
   				}
                              semGive (pGrPriv->mutexSem);
                              n[grind - 1]++;
                         }
                         else
                         {
                              /* A file read error should not affect the index status */
                              pPriv->keepIndex = TRUE;

                              status = GR_FILEIO_ERROR;
                              grTerminateTasks(par, status, "Too many entries in aux LUT file");

                              break;
                         }/*if*/
                      }/* if grind == gratingPos */


                    }
               }
          }
        }
    fclose (fp);
     }

     semTake (pGrPriv->mutexSem, WAIT_FOREVER);
     pGrPriv->tilt2StepsLut[GRA].numberOfEntries = n[GRA];
     GRDEBUG(DAR_MSG_MAX,"*** n[GRA] = %d\n",n[GRA]);
     pGrPriv->tilt2StepsLut[GRB].numberOfEntries = n[GRB];
     GRDEBUG(DAR_MSG_MAX,"*** n[GRA] = %d\n",n[GRB]);
     pGrPriv->tilt2StepsLut[GRC].numberOfEntries = n[GRC];
     GRDEBUG(DAR_MSG_MAX,"*** n[GRA] = %d\n",n[GRC]);
     pGrPriv->tilt2StepsLut[GRD].numberOfEntries = n[GRD];
     GRDEBUG(DAR_MSG_MAX,"*** n[GRA] = %d\n",n[GRC]);
     semGive (pGrPriv->mutexSem);

     return(status);
}
/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grReadZPC
 *
 * INVOCATION:
 * grReadZPC (par);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *                                      structure.
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Read in the zero point correction values from the tilt configuration string 
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

static void grReadZPC(ASSEMBLY_CONTROL_RECORD *par)
{
     GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.  */
     GR_DEV_CONFIG  *pDevConfig;
     long status = DAR_S_SUCCESS;               /* Return function status.      */
     double         tempDouble[3];                   /* temp doubles                */
     long           tempLong[4];                        /* temp long                   */
     BOOL           gotZpca;                          /* TRUE when zerp point correction allocated */
     BOOL           gotZpcb;                          /* TRUE when zerp point correction allocated */
     BOOL           gotZpcc;                          /* TRUE when zerp point correction allocated */
     BOOL           gotZpcd;                          /* TRUE when zerp point correction allocated */
     long         pixZpca;                         /* zpc in pixels */
     long         pixZpcb;                         /* zpc in pixels */
     long         pixZpcc;                         /* zpc in pixels */
     long         pixZpcd;                         /* zpc in pixels */
     int	site;
        

GRDEBUG(DAR_MSG_MAX, "grReadZPC: entry, sim=%d\n", assSimulateLevel(par) );	

     /*
      * Obtain the device private structure and device configuration structure pointers.
      */

     pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pGrPriv->pGratingPriv;

     /*
      * Trap any problem with the internal data structures
      */

     if ( pGrPriv == NULL || pDevConfig == NULL )
     {
          GRDEBUG(DAR_MSG_FATAL, "grReadZPC: Bad device private data structure%c\n", ' ' );
          assDisplayPrivateStruct( par );

          status = GR_BAD_STRUCTURE;
          recGblRecordError (status, par, __FILE__ ":bad internal data structure");
          return ;
     }
     else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
     {
          GRDEBUG(DAR_MSG_FATAL, "grReadZPC: No magic value seen in data structure%c\n", ' ' );

          status = GR_BAD_STRUCTURE;
          recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
          return ;
     }

          /*
           * Read in the tilt device configuration string 
           */

          semTake (pGrPriv->mutexSem, WAIT_FOREVER);

          gotZpca    = FALSE;
          gotZpcb    = FALSE;
          gotZpcc    = FALSE;
          gotZpcd    = FALSE;

          if (sscanf((char *)par->d, "%lf %lf %lf %ld %ld %ld %ld",
                     &tempDouble[0],
                     &tempDouble[1],
                     &tempDouble[2],
                     &tempLong[0],
                     &tempLong[1],
                     &tempLong[2],
                     &tempLong[3]) != 7)
          {
               GRDEBUG(DAR_MSG_ERROR,
                       "grReadZOC wrong string read from input D%c\n",
                       ' ' );
               status = DAR_E_ATT;
               assAddErrorMessage( par, "Invalid config string in att D");
               semGive (pGrPriv->mutexSem);
               return ;
          }
          else
          {
		pixZpca = tempLong[0];
		pixZpcb = tempLong[1];
		pixZpcc = tempLong[2];
		pixZpcd = tempLong[3];

		printf("*** zero point correction in pixels received : %ld %ld %ld %ld ***\n", pixZpca,pixZpcb,pixZpcc,pixZpcd);

		 site = *((long *) GR_SITE_CODE);
		 if (site != SITE_MK && site != SITE_CP) {
printErr ("illegal site=%d\n", site);
			GRDEBUG(DAR_MSG_WARNING,
			    "grDoTask: Invalid site value=%d. Using MK.\n",
			    site);
			site = SITE_MK;
		 }
printErr ("site=%d\n", site);

		 if (site == SITE_MK)
		 {
		     if (pixZpca >  0 ) {
			    pDevConfig->zpc[GRA] = (pixZpca - 190.5)/12.0;
		     }
		     else {
			    pDevConfig->zpc[GRA] = 0.0;
		     }
		     if (pixZpcb >  0) {
			    pDevConfig->zpc[GRB] = (pixZpcb - 182.5)/12.0;
		     }
		     else {
			    pDevConfig->zpc[GRB] = 0.0;
		     }
		     if (pixZpcc >  0) {
			    pDevConfig->zpc[GRC] = (pixZpcc - 182.5)/12.0;
		     }
		     else {
			    pDevConfig->zpc[GRC]= 0.0;
		     }
		     if (pixZpcd >  0) {
			    pDevConfig->zpc[GRD] = (pixZpcd - 182.5)/12.0;
		     }
		     else {
			    pDevConfig->zpc[GRD] = 0.0;
		     }
		 } else {
		     if (pixZpca >  0) {
			    pDevConfig->zpc[GRA] = (pixZpca - 151.0)/10.5;
		     }
		     else {
			    pDevConfig->zpc[GRA] = 0.0;
		     }
		     if (pixZpcb >  0) {
			    pDevConfig->zpc[GRB] = (pixZpcb - 151.0)/10.5;
		     }
		     else {
			    pDevConfig->zpc[GRB] = 0.0;
		     }
		     if (pixZpcc >  0) {
			    pDevConfig->zpc[GRC] = (pixZpcc - 151.0)/10.5;
		     }
		     else {
			    pDevConfig->zpc[GRC] = 0.0;
		     }
		     if (pixZpcd >  0) {
			    pDevConfig->zpc[GRD] = (pixZpcd - 151.0)/10.5;
		     }
		     else {
			    pDevConfig->zpc[GRD] = 0.0;
		     }
		 }

#if 0
#ifdef MK
			 if (pixZpca >  0 ) {
				pDevConfig->zpc[GRA] = (pixZpca - 190.5)/12.0;
			 }
			 else {
				pDevConfig->zpc[GRA] = 0.0;
			 }
			 if (pixZpcb >  0) {
				pDevConfig->zpc[GRB] = (pixZpcb - 182.5)/12.0;
			 }
			 else {
				pDevConfig->zpc[GRB] = 0.0;
			 }
			 if (pixZpcc >  0) {
				pDevConfig->zpc[GRC] = (pixZpcc - 182.5)/12.0;
			 }
			 else {
				pDevConfig->zpc[GRC]= 0.0;
			 }
			 if (pixZpcd >  0) {
				pDevConfig->zpc[GRD] = (pixZpcd - 182.5)/12.0;
			 }
			 else {
				pDevConfig->zpc[GRD] = 0.0;
			 }
#else

                         if (pixZpca >  0) {
                                pDevConfig->zpc[GRA] = (pixZpca - 151.0)/10.5;
                         }
                         else {
                                pDevConfig->zpc[GRA] = 0.0;
                         }
                         if (pixZpcb >  0) {
                                pDevConfig->zpc[GRB] = (pixZpcb - 151.0)/10.5;
                         }
                         else {
                                pDevConfig->zpc[GRB] = 0.0;
                         }
                         if (pixZpcc >  0) {
                                pDevConfig->zpc[GRC] = (pixZpcc - 151.0)/10.5;
                         }
                         else {
                                pDevConfig->zpc[GRC] = 0.0;
                         }
                         if (pixZpcd >  0) {
                                pDevConfig->zpc[GRD] = (pixZpcd - 151.0)/10.5;
                         }
                         else {
                                pDevConfig->zpc[GRD] = 0.0;
                         }
#endif
#endif

                        printf(" ******** populating configuration structure : \n");
                        printf(" ******** pDevConfig->zpc[GRA] = %f \n", pDevConfig->zpc[GRA]);
   
          }
/*
          if (!gotZpca || !gotZpcb || !gotZpcc || !gotZpcd)
          {
               GRDEBUG(DAR_MSG_ERROR,
                       "grReadZPC: incomplete string read from input D%c\n",
                       ' ' );
               status = DAR_E_ATT;
               assAddErrorMessage( par, "Incomplete config string in att D");
               semGive (pGrPriv->mutexSem);
               return ;
          }
*/
        semGive (pGrPriv->mutexSem);
 	return;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grClearBarcodes
 *
 * INVOCATION:
 * grClearBarcodes (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Clear the current grating barcode list. Does not clear auxilliary lookup table.
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

static void grClearBarcodes(ASSEMBLY_CONTROL_RECORD *par)
{
     GR_DEV_PRIVATE *pGrPriv;			/* Ptr to private dev. struct.	*/
     GR_DEV_CONFIG  *pDevConfig;
     int            i;

     pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pGrPriv->pGratingPriv;

     GRDEBUG(DAR_MSG_MAX, "grClearBarcodes: entry%c\n", ' ');

     /*
      * Clear the internal list of barcodes, using a semaphore to prevent simultaneous access.
      */

     semTake (pGrPriv->mutexSem, WAIT_FOREVER);

     for (i=0; i < GR_NUM_GRATINGS; i++)
     {
         pDevConfig->barcode[i] = GR_NO_BARCODE;
     }
     pDevConfig->characterized = FALSE;

     semGive (pGrPriv->mutexSem);

     /*
      * Do not clear the auxiliary lookup table
      */

     return;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grWriteConfig
 *
 * INVOCATION:
 * status = grWriteConfig (par, savebackup); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *					structure.
 * (>) savebackup (const int)           Save backup flag
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 * Write the current turret configuration to file
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

static long grWriteConfig(ASSEMBLY_CONTROL_RECORD *par, const int savebackup)
{
     ASS_CONTROL_PRIVATE *pPriv = par->dpvt;	/* Private rec. support struct.	*/
     GR_DEV_PRIVATE *pGrPriv;			/* Ptr to private dev. struct.	*/
     GR_DEV_CONFIG  *pDevConfig;
     char scratchBuf[256];
     char scratchBuf2[256];
     long status = DAR_S_SUCCESS;	/* Return function status.	*/
     int i, j, c[7];
     FILE *fp;
     char *month[] = { "000", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

     pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pGrPriv->pGratingPriv;

     GRDEBUG(DAR_MSG_MAX, "grWriteConfig: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL || pDevConfig == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grWriteConfig: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grWriteConfig: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

     /* Get a time stamp for the lookup tables */

     j = timeNowC (UTC, 3, c);

     /* First save the default lookup table file */

     *scratchBuf = '\0';
     strncat (scratchBuf, par->tdir, MAX_STRING_SIZE );
     strcat  (scratchBuf, "/" );
     strncat (scratchBuf, par->tfil, MAX_STRING_SIZE );

     if (!(fp = fopen(scratchBuf, "w")))
     {
          GRDEBUG(DAR_MSG_ERROR, "grWriteConfig: failed to create %.60s\n", scratchBuf);

          /* A file open error should not affect the index status */
          pPriv->keepIndex = TRUE;

          status = GR_FILEIO_ERROR;
          grTerminateTasks(par, status, "Failed to create new LUT file");

	  return(status);
     }

     fprintf(fp, "%s\n", GR_LUT_FIRST_STRING);
     fprintf(fp, "%s\n", "#\n# Database of current GMOS grating installation\n");
     if ( j == 0 )
     {
          fprintf (fp, "# Time : %d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d.%3.3d (UTC)\n",
                   c[0], c[1], c[2], c[3], c[4], c[5], c[6]);
     }

     fprintf(fp, "%s\n", "# Barcode  Turret Pos  Tilt\n");

     for (i = 0; i < GR_NUM_GRATINGS; i++)
     {
          fprintf(fp, "%10d%10d%8.2f\n", pDevConfig->barcode[i], i, pDevConfig->oldGratTilt[i]);
     };

     fclose (fp);  

     /* Then save the backup lookup table file */

     if ((savebackup) && (j == 0))
     {
          *scratchBuf  = '\0';
          strncat (scratchBuf, par->tdir, MAX_STRING_SIZE );
          strcat (scratchBuf, "/" );

          *scratchBuf2 = '\0';
          sprintf (scratchBuf2, "%s.%02d%s%4d_%02d%02dUTC", par->tfil, c[2], month[c[1]],
                   c[0], c[3], c[4]);

          strncat (scratchBuf, scratchBuf2, MAX_STRING_SIZE);

          if (!(fp = fopen(scratchBuf, "w")))
          {
               GRDEBUG(DAR_MSG_ERROR, "grWriteConfig: failed to create %.60s\n", scratchBuf);

               /* A file open error should not affect the index status */
               pPriv->keepIndex = TRUE;

               status = GR_FILEIO_ERROR;
               grTerminateTasks(par, status, "Failed to create backup LUT file");

               return(status);
          }

          fprintf (fp, "%s\n", GR_LUT_FIRST_STRING);
          fprintf (fp, "%s\n", "#\n# Database of current GMOS grating installation\n");

          fprintf (fp, "# Time : %d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d.%3.3d (UTC)\n",
                   c[0], c[1], c[2], c[3], c[4], c[5], c[6]);

          fprintf(fp, "%s\n", "# Barcode  Turret Pos  Tilt\n");

          for (i = 0; i < GR_NUM_GRATINGS; i++)
          {
               fprintf(fp, "%10d%10d%8.2f\n", pDevConfig->barcode[i], i, pDevConfig->oldGratTilt[i]);
          };

          sprintf (scratchBuf, "File %.28s saved", scratchBuf2);
          assAddErrorMessage(par, scratchBuf);
          GRDEBUG(DAR_MSG_LOG, "grWriteConfig: backup lookup table is %.60s\n", scratchBuf);
          fclose (fp);  
     }
     else if (j != 0)
     {
          GRDEBUG(DAR_MSG_WARNING, "grWriteConfig: no TCS time system, backup file not saved%c\n", ' ');
          assAddErrorMessage(par, "No Backup file saved");
     }

     return (status);
}

/* ============================================================================= */

static long grShowConfig(ASSEMBLY_CONTROL_RECORD *par)
{
     GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/
     GR_DEV_CONFIG  *pDevConfig;
     long status = DAR_S_SUCCESS;	/* Return function status.	*/
     int i = 0;

     static char turretPositions[GR_NUM_GRATINGS] = "ABCD";

     pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );
     pDevConfig = pGrPriv->pGratingPriv;

     GRDEBUG(DAR_MSG_MAX, "grShowConfig: entry%c\n", ' ');

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL || pDevConfig == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grShowConfig: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC || pDevConfig->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grShowConfig: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");
         return (status);
     }

    /*
     * Display the contents of the internal lookup table.
     */

     if (pDevConfig->characterized == TRUE)
     {
          printf("Internal grating configuration database\n");
          printf("---------------------------------------\n");

          for (i = 0; i < GR_NUM_GRATINGS; i++)
          {
               printf("turretPos = %c, barcode = %d tilt = %f\n", turretPositions[i],
                   pDevConfig->barcode[i], pDevConfig->oldGratTilt[i]);
          }
     }
     else
     {
          printf("Grating assembly not characterized\n");
     }

     return(status);
}

/* ============================================================================= */

static long grReadSwitches(ASSEMBLY_CONTROL_RECORD *par)
{
     if (!strncmp(par->sil, "0000", MAX_STRING_SIZE))
          return(4);
     else if (!strncmp(par->sil, "0001", MAX_STRING_SIZE))
          return(0);
     else if (!strncmp(par->sil, "0010", MAX_STRING_SIZE))
          return(1);
     else if (!strncmp(par->sil, "0100", MAX_STRING_SIZE))
          return(2);
     else if (!strncmp(par->sil, "1000", MAX_STRING_SIZE))
          return(3);
     else
          return(GR_TURRET_ERROR);
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * grProcessFault
 *
 * INVOCATION:
 * status = grProcessFault( par );
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

static long grProcessFault(ASSEMBLY_CONTROL_RECORD *par)
{
    GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/
    long status = DAR_S_SUCCESS;	/* Function status.		*/
    
    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grProcessFault: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return (status);
     }
     else if ( pGrPriv->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grProcessFault: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value in data structure");

         /* Don't return. Carry on and attempt the interlock. */
     }

/* Is a command active? If not, do nothing */

    if (pGrPriv->cmdActive)
    {

/* Finish the command and empty the task list */

       GRDEBUG(DAR_MSG_ERROR, "grProcessFault: interlock occurred%c\n",' ');

       status = DAR_E_FLT_CLR;
       grTerminateTasks( par, status, "Grating interlock occurred" );
    }
     
    return ( status );
      
}

/*
************************************************************************
 *+
 * FUNCTION NAME:
 * grTerminateTasks
 *
 * INVOCATION:
 * grTerminateTasks( par, status, errMessage );
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

static void      grTerminateTasks
(
    ASSEMBLY_CONTROL_RECORD *par,		/* (in)  Ptr to ass rec.	*/
    long                    status,             /* Final status.                */
    char                    *errMessage         /* Error message.               */
)
{
    GR_DEV_PRIVATE *pGrPriv;		/* Ptr to private dev. struct.	*/
    long timerStatus = DAR_S_SUCCESS;   /* status word from assStopTimer  */

    pGrPriv = ( GR_DEV_PRIVATE *) assGetPrivateStruct( par );

    /*
     * Trap any problem with the internal data structures
     */

     if ( pGrPriv == NULL )
     {
         GRDEBUG(DAR_MSG_FATAL, "grTerminateTasks: Bad device private data structure%c\n", ' ' );
         assDisplayPrivateStruct( par );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":bad internal data structure");
         return;
     }
     else if ( pGrPriv->magic != GR_MAGIC )
     {
         GRDEBUG(DAR_MSG_FATAL, "grTerminateTasks: No magic value seen in data structure%c\n", ' ' );

         status = GR_BAD_STRUCTURE;
         recGblRecordError (status, par, __FILE__ ":no magic value seen in data structure");

         /* Don't return. Carry on and attempt to terminate the tasks. */
     }

     /*
      * Empty the grating task list.
      */

     grEmptyList( par, TRUE );

     /*
      * Stop the timer to prevent any further processing of the assembly record.
      * (Use a temporary status to avoid overwriting the final command status.)
      */

     assStopTimer( par, &timerStatus );
     if (timerStatus != DAR_S_SUCCESS)
     {
         GRDEBUG(DAR_MSG_WARNING, "grTerminateTasks: failed to stop watchdog timer. status=%ld\n",
            timerStatus);
     }

     /*
      * Set the flags in the device private structure to indicate completion of the command
      */

     semTake (pGrPriv->mutexSem, WAIT_FOREVER);
     pGrPriv->cmdActive = FALSE;
     semGive (pGrPriv->mutexSem);

     /*
      * Finally, tell the assembly record the command has finished, supplying the final
      * status and error message.
      */

     assCommandFinish( par, status, errMessage );

     return;
}


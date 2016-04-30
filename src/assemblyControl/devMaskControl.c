/* 
*************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) <1999>                       (c) <1999>
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
 * devMaskControl.c
 *
 * PURPOSE:
 * EPICS Assembly Device Support code for the mask assemblyControl record.
 
 * FUNCTION NAME(S)
 * mkAckReceived        Device control record changed state.
 * mkBuildList          Build a task list.
 * mkCheckAttributes    Check input attributes.
 * mkCheckBarcodeId     Check returned barcode id.
 * mkCheckCassette      Check cassette.
 * mkCheckFocalPlane    Create tasks based on focal plane state.
 * mkClearBarList       Empties barcode list.
 * mkCommandDevice      Execute a task on the task list.
 * mkDoTask             Setup to execute a task on the task list
 * mkEmptyList          Delete from the task List
 * mkIndexMode          Execute INDEX mode command.
 * mkInitDeviceSupport  Initialize instance of record's device support code.
 * mkInitMode           Execute INIT mode command.
 * mkMoveConfig         Determine configuration for a MOVE command.
 * mkMoveMode           Execute MOVE or PARK mode command.
 * mkReadBarList        Read the barcode list from the file. 
 * mkProcessFault       Handle an external fault or interlock.
 * mkStopDirective      Execute STOP directive to attached records.
 * mkTaskFinished       Determine if a task list is finished.
 * mkTaskPostCheck      Determine if a task finished correctly.
 * mkTaskPreCheck       Determine if a task can be executed.
 * mkTerminateTasks     Terminate the current command.
 * mkTestMode           Execute the TEST mode.
 * mkTestSensors        Check that sensor and extractor words make sense.
 * mkTrackMode          Execute the TRACK mode.
 * mkTranslateDir       Translate dir directive for device record.
 * mkUpdateMode         Execute UPDATE mode command.
 * mkWriteBarList       Write the barcode list to a file.
 *  
 *
 *
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.10  2015/03/11 13:05:27  rrojas
 * Optical sensors SCAN attr. changed to "I/O intr". plus some pv's and lut's files
 *
 * Revision 1.9  2014/08/18 15:37:21  rrojas
 * Added a 2 seconds delay to read the Mask switch.
 *
 * Revision 1.8  2013/01/10 01:48:42  gemvx
 * implement Single Mask Update, redefine the extractor velocities to be configured using global variables on the console
 *
 * Revision 1.7  2011/08/27 16:29:40  gemvx
 * changed error message for a failure of the mask in slot sensor to:
 * Mask not in current slot, use INIT or UPDATE
 *
 * Revision 1.6  2008/01/04 23:12:23  gemvx
 * changed the barcode read timeout to 10 seconds (see FR 7484 and CR 1254)
 *
 * Revision 1.5  2006/04/12 02:51:48  gemvx
 * make the ifu barcode available on SOV to be writtin to the ifuSelection record
 *
 * Revision 1.4  2005/02/23 01:50:52  gemvx
 * *** empty log message ***
 *
 * Revision 1.3  2003/11/18 18:53:55  gemvx
 * *** empty log message ***
 *
 * Revision 1.2  2003/05/27 21:16:51  gemvx
 * Version 4-1
 *
 * Revision 1.1  2002/04/24 05:24:56  ajf
 * New directory for port to epics3.13.4GEM8.4.
 *
 * Revision 1.1  2001/11/28 20:08:46  mbec
 * *** empty log message ***
 *
 * Revision 1.2  2001/04/23 18:24:42  smb
 * DEBUG macro changed from logMsg to printf so it can display floating point values (bug 196)
 *
 * Revision 1.27  2001/03/20 13:40:28  gmos
 * Modified DEBUG macro. All files now use printf() rather than logMsg(). All also print the output from taskName(0).
 *
 * Revision 1.26  2001/03/01 21:22:51  gmos
 * Removed all references to the MK_NO_TASKS status.  In MOVE mode commands,
 * a debug message will appear to say nothing needs to be done when the
 * requested configuration is the same as the current configuration.  Removed
 * unused defines and changed some MAX level debug statements to be FULL.
 *
 * Revision 1.25  2001/02/28 17:24:11  gmos
 * Renamed gmMessageLevels.h to darMessageLevels.h.
 *
 * Revision 1.24  2001/02/26 14:45:20  gmos
 * Subtracted 100 from all the status codes to remove any overlap with those 
 * defined in recAssControl.h. Use MK_MALLOC_ERROR throughout for a memory 
 * allocation error.
 *
 * Revision 1.23  2001/02/26 14:24:15  gmos
 * Improved the console message reported in MK_CHECK_NULL. Modified 
 * mkTerminateTasks to ensure the MESS field gets an error message whenever 
 * there is a malloc error.
 *
 * Revision 1.22  2001/02/23 18:11:23  gmos
 * Include printf within MK_CHECK_NULL, just in case. It should never normally 
 * be executed.
 *
 * Revision 1.21  2001/02/23 15:43:24  gmos
 * Converted to same messaging scheme as the filter, grating and translation 
 * assemblies. Messages of type DAR_DBUG_MIN changed to DAR_MSG_FATAL, ERROR, 
 * WARNING or MIN as appropriate.
 *
 * Revision 1.20  2001/01/22 23:43:22  gmos
 * -Added error codes to differentiate between invalid barcodes, duplicate
 *  barcodes and wrong barcodes.
 * -Added lookup table code for defining invalid barcode.
 * -Added barcode range checks when asking for a new mask and when reading
 *  barcode lists.
 * -Bad barcodes always end in an error but the commands carry on except in
 *  MOVE when the barcode is valid but not the same as the target and in
 *  UPDATE when the barcode is a duplicate of one read already.
 * -Added quick update attribute to sensor input M (SIM) and check for it
 *  in mkBuildNewList (sets quickUpdate flag if UPDATE and SIM == 1).
 * -The QUICK update attribute is cleared as soon as an UPDATE command
 *  acknowledges it preventing it from accidently getting used again.
 * -In UPDATE command, the default is FULL update which first clears the old
 *  list, then checks every slot.  The operator can choose a QUICK update
 *  which does not clear the old list and will only attempt to read barcodes
 *  on masks it finds if there is no valid entry already specified (skip the
 *  slot unless the entry says invalid or no barcode).
 * -Added debug warnings about stopping an unfinished UPDATE commmand.
 * -Changed the way barcode lists are cleared.  Now clears during device
 *  initialization, in UPDATE command if FULL UPDATE (a warning about
 *  NOT clearing if QUICK UPDATE), in INIT command if the file can be read
 *  and also if only a partial INIT was possible.
 * -Changed debug messages to indicate cassette and slot number when appropriate.
 * -In simulation, allow command to be stopped after moving cassette to slot.
 *
 * Revision 1.19  2001/01/18 02:03:22  gmos
 * Removed barcode simulator code from mkCheckBarcodeId (devBarcode already
 * simulates barcodes and errors).  Change so that barcode related failures
 * at any time do not cause the index to be lost.  Ensure the barcode list is
 * cleared at start of UPDATE and INIT.  Change the mask ID output for SAD
 * (VALA) so that it is unknown (-99) when a mask has been loaded following a
 * barcode reader failure.  Clarified some error messages and comments.
 *
 * Revision 1.18  2001/01/16 11:45:21  gmos
 * Arguments of strcpy were reversed. Fixed.
 *
 * Revision 1.17  2001/01/16 00:21:00  gmos
 * Change mkCheckBarcodeId() so that it copies the barcode reader string
 * into a local variable (so that it's always evaluating the same string).
 *
 * Revision 1.16  2001/01/15 21:39:08  gmos
 * Change mkCheckBarcodeId so that a mask is still placed in the focalplane
 * when the barcode reader fails (if it's a wrong mask or a duplicate the
 * mask is put back into its cassette).
 *
 * Revision 1.15  2000/11/18 01:09:48  gmos
 * Ensured all failures due to barcode readers and lookup table file
 * opening problems did not result in loss of index.  Cleaned up status
 * returns from calls to assStartTimer and assStopTimer.
 *
 * Revision 1.14  2000/11/17 00:30:42  gmos
 * Add file open check in mkUpdateMode so that time isn't wasted updating
 * if the file cannot be opened.  Update now fails if directory and/or
 * filename are missing.  Cleaned up some comments and debug messages.
 *
 * Revision 1.13  2000/11/16 01:32:09  gmos
 * Function mkReadBarList now clears internal barcode list every time.
 * Added check for duplicate barcode IDs (if found causes INIT to fail).
 * Added comments and debugging.  Added DEBUG3 macro.
 *
 * Revision 1.12  2000/09/08 11:39:45  gmos
 * Check simulation level against DAR_SIM_NONE rather than 0.
 *
 * Revision 1.11  2000/09/06 08:16:13  gmos
 * Call mkBuildNewList in simulation mode.
 *
 * Revision 1.9  2000/08/18 16:58:21  gmos
 * Added more temporary debugging to mkBuildList to help determine
 * optimum opto sensor power up delay.
 *
 * Revision 1.8  2000/08/16 19:01:35  gmos
 * Added taskDelay following optoSensor powerup before reading switches.
 * Delay parameter is temporarily supplied via SIM sensor input.
 *
 * Revision 1.7  2000/08/15 18:55:19  gmos
 * Added debugging for opto switch control.
 *
 * Revision 1.6  2000/07/28 15:16:20  gmos
 * Disable opto sensors when mask assembly idle
 *
 * Revision 1.5  2000/05/30 16:29:07  gmos
 * New version commented by William Rambold. Master enable bug fixed.
 *
 * Revision 1.33  2000/03/28 23:18:29  wooff
 * Rearranged mkTaskFinished so that the error would not be lost
 * when mkWriteBarList failed.
 *
 * Revision 1.32  2000/02/10 23:32:26  wooff
 * Changed mkCheckFocalplane to check for any of the
 * three focal plane switches.
 *
 * Revision 1.31  2000/02/10 21:34:30  angelic
 * added NULL function pointer for new DSET function processFault
 *
 * Revision 1.30  2000/02/08 21:25:54  wooff
 * Removed Pre Check for reading barcode (no longer checks for ext ID position.
 *
 * Revision 1.29  2000/01/14 00:01:45  wooff
 * Cleaned up error messages and made certain every error
 * message was accompanied by a MIN debug statement.
 *
 * Revision 1.28  2000/01/13 00:25:27  wooff
 * Fixed write to mask_loc in post check.
 *
 * Revision 1.27  1999/12/16 23:25:32  wooff
 * When PreCheck fails to find a mask after moving ext to grip,
 * the index is kept using keepIndex flag (also used for barcoder).
 *
 * Revision 1.26  1999/12/16 22:42:59  wooff
 * Added casParked flag to prevent cassette movement following a
 * cassette load if the cassette isn't completely inserted.
 *
 * Revision 1.25  1999/12/16 20:53:33  wooff
 * Changed MK_EXT_TO_MASK post check to fail unless in_focal_plane and
 * only mask_in or ifu_in (not both or none).
 *
 * Revision 1.24  1999/12/16 20:02:28  wooff
 * Fixed extractor indexing in mkBuildList. Now indexes on home switch
 * or lower soft limit based on zone 5 status and aligned/occupied switches.
 *
 * Revision 1.23  1999/12/15 20:56:53  angelic
 * improved simulation mode in pre and post checks
 *
 * Revision 1.22  1999/11/16 22:11:57  angelic
 * added semaphore calls to protect the private structure
 *
 * Revision 1.21  1999/11/04 23:39:43  angelic
 * prevent new move commands while busy
 *
 * Revision 1.20  1999/11/01 22:25:24  rambold
 * Fixed bug where error messages were being lost by
 * moving CLR_ERR_MSG to checAttributes from terminateTasks...
 *
 * Revision 1.19  1999/10/30 17:06:29  rambold
 * Passed test command down to devices.
 *
 * Revision 1.18  1999/10/30 00:24:44  rambold
 * moved MK_CAS_TO_C1S1 entry point in postCheck
 *
 * Revision 1.17  1999/10/28 17:33:15  rambold
 * Added terminate tasks if a mode command fails immediately.
 *
 * Revision 1.16  1999/10/20 17:13:00  rambold
 * added stop sequences to shorten aborted motion times
 * and created a proper mask rejection sequence.
 *
 * Revision 1.15  1999/10/19 20:41:50  rambold
 * added stop checking to grip and cassette motions.
 *
 * Revision 1.14  1999/10/19 19:40:35  angelic
 * added keepIndex flag in order to keep index when a barcode read fails
 *
 * Revision 1.13  1999/10/15 18:32:42  angelic
 * fixed bug by unsetting the askBarCode flag
 * added debug statement to writeBarCode
 *
 * Revision 1.12  1999/10/04 23:48:29  wooff
 * Major rework of mkTestSensors and addition of MK_CAS_TO_C1S1.
 *
 * Revision 1.11  1999/09/10 22:21:34  wooff
 * Changed mkMoveMode so that it passed the PARK mode to mkBuildList when 
 * appropriate instead of always passing MOVE.  Also cleaned up some 
 * spelling mistakes in error messages and added some MAX debugging to 
 * mkMoveConfig function.
 *
 * Revision 1.10  1999/08/24 17:59:22  angelic
 * added simulation of barcode scanning
 *
 * Revision 1.9  1999/07/30 18:37:30  rambold
 * updated error messages, added pre and post task
 * checks and generally reworked the tasking structure
 *
 * Revision 1.8  1999/07/06 17:47:13  rambold
 * bug fixes .. last save before major change.
 *
 * Revision 1.7  1999/06/12 01:59:38  rambold
 * Cleaned up update feature
 *
 * Revision 1.6  1999/06/08 21:32:33  dunn
 * Added reading of a lookup table.
 *
 * Revision 1.5  1999/06/07 23:35:02  dunn
 * Added reading in of lookup table for characterization.
 *
 * Revision 1.4  1999/06/06 21:31:11  dunn
 * Added fixes for park.
 *
 * Revision 1.3  1999/06/05 00:11:03  dunn
 * Lots of stuff works find now.
 *
 * Revision 1.2  1999/05/07 17:10:23  dunn
 * Init and Index working.
 *
 * Revision 1.1  1999/05/03 22:34:36  dunn
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
#include        <taskLib.h>
#include        <tickLib.h>
#include        <sysLib.h> 
#include        <ellLib.h>
#include        <dbEvent.h>

#include        <assemblyControlRecord.h>
#include        <assemblyControl.h>
#include	<darMessageLevels.h>     /* GMOS message level definitions.               */


/*
 *  Local Defines
 */

#define mkDevInit               NULL     /* no function.                */

#define MK_NO_ENTRY           -99        /* No entry in list            */
#define MK_COMMAND_TIMEOUT    120        /* 120 seconds.                */
#define MK_CMD_INBEAM           0        /* Move mask inbeam            */
#define MK_CMD_OUTBEAM          1        /* Move mask outbeam           */
#define MK_NUM_CASSETTES        3        /* Num of cassettes.           */
#define MK_NUM_SLOTS            9        /* Slots per cassette.         */
#define MK_MAX_SLOTS           27        /* Total slots available       */
#define MK_NUM_DEVICES          4        /* Number of attached devices  */
#define MK_MAX_NUM_RETRY_BC     5        /* Num of retries for BC       */
#define MK_MAX_LUT_NAME        16        /* max length of name          */
#define MK_BARCODE_MAX     19999999      /* Maximum allowable barcode   */
#define MK_BARCODE_MIN     00000000      /* Minimum allowable barcode   */
#define MK_BARCODE_MAX_SIM 59999999      /* Maximum simulated barcode   */
#define MK_BARCODE_MIN_SIM 50000000      /* Minimum simulated barcode   */
#define MK_NO_BARCODE          -1        /* Code for undefined barcode. */
#define MK_BARCODE_READ_DELAY  10        /* seconds to wait for barcode */
#define MK_SENSOR_DELAY         1        /* seconds to wait for power-up*/
#define MK_VELOCITY_DISABLE     0.0      /* use this for zero velocities*/
                 /* either as a placeholder for devices that don't use  */
                 /* their VEL link or to disable devices that do use it */

/*
 *  Move/Update configurations.
 */

#define MK_NOTHING              1        /* Move nothing.               */
#define MK_WRONG_MASK_IN_FP     2        /* Wrong mask in fp.           */
#define MK_MASK_OUT_FP          3        /* Move mask out of fp.        */
#define MK_NEW_FP_IN            4        /* Move mask in of fp.         */


/*
 * The following status codes supplement the DAR_ codes within recAssControl.h
 */

#define MK_EMPTY_LIST           3        /* Barcode list is empty       */
#define MK_LAST_TASK            2        /* Return status.              */
#define MK_MALLOC_ERROR         -103     /* Malloc call failed          */
#define MK_TASK_LIST_EMPTY      -104     /* No tasks left on list       */
#define MK_INVALID_LOCATION     -105     /* Invalid mask destination    */
#define MK_EMPTY_BARCODE_LIST   -106     /* Barcode list is empty       */
#define MK_MOVE_CONFIG_ERR      -107     /* Bad instrument configuration*/
#define MK_TASK_FAILED          -108     /* Task failed during execution*/
#define MK_INVALID_TASK         -109     /* Task number not recognized  */
#define MK_BAD_DEV_CONNECT      -110     /* Can not connect to device   */
#define MK_TASK_LIST_NOT_EMPTY  -111     /* Unexpected tasks on list    */
#define MK_INVALID_MODE         -112     /* Invalid mode received       */
#define MK_FP_CONFIG_ERROR      -113     /* Bad FP configuration        */
#define MK_INVALID_BARCODE      -114     /* Invalid barcode read        */
#define MK_INVALID_MASK         -115     /* Invalid mask ID entered     */
#define MK_TRIG_FAILED          -116     /* Can not trigger barcode read*/
#define MK_BAD_EXT_POS          -117     /* Invalid extractor position  */
#define MK_BAD_SENSORS          -118     /* Invalid sensor data         */
#define MK_SLOT_ERROR           -119     /* Slot not aligned            */
#define MK_CAS_ERROR            -120     /* Cassette not installed      */
#define MK_BAD_BAR_NODE         -121     /* Invalid barcode list number */
#define MK_BAD_PARK_POSITION    -122     /* Invalid park position entered   */
#define MK_FILE_OPEN_ERROR      -123     /* Can not open barcode file   */
#define MK_FILE_FORMAT_ERROR    -124     /* Can not parse barcode file  */
#define MK_BAD_WRITE            -125     /* Can't write to barcode file */
#define MK_NO_POWER             -126     /* Can't turn on/off opto sensors  */
#define MK_INVALID              -127     /* Misc checkAttribute failure */
#define MK_BAR_READ_FAIL        -128     /* Reader returned bad/no id   */
#define MK_DUPLICATE_BARCODE    -129     /* ID same as another mask     */
#define MK_WRONG_BARCODE        -130     /* ID not same as target       */
#define MK_UPDATE_INCOMPLETE    -131     /* UPDATE stopped before done  */
/*
 *  Defines of the named positions in the device records.
 */

#define MK_NAME_POS_CLEAR       "clear"         /* VALS of clear        */
#define MK_NAME_POS_LOCK        "lock"          /* VALS of lock         */
#define MK_NAME_POS_REL         "release"       /* VALS of release.     */
#define MK_NAME_POS_ID          "id"            /* VALS of id.          */
#define MK_NAME_POS_GRP         "grip"          /* VALS of grip.        */
#define MK_NAME_POS_MASK        "mask"          /* VALS of mask.        */
#define MK_NAME_POS_IFU         "ifu"           /* VALS of ifu.         */
#define MK_NAME_POS_INDEX       "index"         /* VALS of index.       */
#define MK_NAME_POS_I4          "i4"            /* VALS of i4.          */
#define MK_NAME_POS_I4_REL      "i4release"     /* VALS of i4release.   */
#define MK_NAME_POS_I4_GRP      "i4gripper"     /* VALS of i4gripper.   */
#define MK_NAME_POS_PARK        "park"          /* VALS of park         */



/*
 *  Define device names.
 */

#define MK_DEV_NAME_EXT        "extractor"      /* extractor device.    */
#define MK_DEV_NAME_GRP        "gripper"        /* gripper device.      */
#define MK_DEV_NAME_REL        "release"        /* release device.      */
#define MK_DEV_NAME_CAS        "cassette"       /* cassette device.     */


/*
 *  Define the attributes in and input sensors.
 */

#define MK_BARCODE_ID_IN        *(long *)  par->a   /* Mask id.         */
#define MK_MASK_IFU_LOC         *(long *)  par->b   /* Mask location.   */
#define MK_PARK_POSITION_IN     *(long *)  par->c   /* Park mode.       */
#define MK_UPDATE_SINGLE_C      *(long *)  par->d   /* Single Update Cassette Number */
#define MK_UPDATE_SINGLE_S      *(long *)  par->e   /* Single Update Slot Number */

#define MK_MASK_IN_BEAM         *(long *) par->vala /* In-Beam Mask ID  */
#define MK_EXT_MASK_POS         *(long *) par->valb /* Current Mask Posn*/

#define MK_SENSOR_WORD          *(long *) par->sij  /* Ptr. to Monitor word.*/
#define MK_EXT_POSITION         *(long *) par->sik  /* Ptr. to extractor pos*/
#define MK_BARCODE_READ         (char *)  par->sil  /* Ptr. to barcode read.*/
#define MK_QUICK_UPDATE         *(long *) par->sim  /* Quick or Single Update Flag (0: normal, 1: Quick, -1: Single) */
#define MK_IGNORE_BARCODE       *(long *) par->sin  /* Ignore barcode reader failures Flag */

#define MK_BARCODE_TRIGGER      par->sot            /* Ptr. to barcode link.*/
#define MK_SENSOR_POWER         par->sor     /* Ptr. to control sensor power*/
#define MK_IFU_BARCODE          par->sov     /* Expected IFU barcode from lut */

/*
 * Change the extractor fast and slow velocities into global variables rather than inputs
 * these weren't available as inputs on a dm screen and haven't been changed in a long time.
 * This allows the values to remain dynamic while freeing up Assembly inputs for a new update mode.
 */

long  MK_EXT_VEL_FAST = 35.0;         /* Running velocity. */
long  MK_EXT_VEL_SLOW = 15.0;         /* Indexing velocity */


/*
 *  Define mask location status output names
 */

#define MK_MASK_LOC_UNKNOWN        -1
#define MK_MASK_LOC_IN_BEAM         0
#define MK_MASK_LOC_IN_CASSETTE     1


/*
 * Bitmap mnemonics for Extractor Positions. If gmMskMisc.c changes, then
 * must make associated changed here.
 *
 * Extractor Stage Zones by number:
 *
 * SoftLimit   Release  Grip   Lvdt   ID  Clear  Mask  Ifu  SoftLimit
 *     |         | |     | |     |     |    |      |    |       |
 *     |<---4--->|0|<-2->|1|<-5->|                               
 *     |                         |                               
 *     |<-----------3----------->|                               
 *     |                         |                               
 *
 *
 * Within the code zones are also sometimes referred to by letter:
 *
 * SoftLimit   Release  Grip   Lvdt  ID  Clear  Mask  Ifu  SoftLimit
 *     |         | |     | |     |     |    |      |    |       |
 *     |         |         |                                    |
 *     |<---A--->|<---B--->|<----------------C----------------->|
 *     |                                                        |
 */

#define MK_EXT_AT_RELEASE       0x00000001    /* Zone 0.    */
#define MK_EXT_AT_GRIPPER       0x00000002    /* Zone 1.    */
#define MK_EXT_BTW_R_G          0x00000004    /* Zone 2.    */
#define MK_EXT_IN_LVDT          0x00000008    /* Zone 3.    */
#define MK_EXT_ATPAST_REL       0x00000010    /* Zone 4.    */
#define MK_EXT_BEFR_GRP         0x00000020    /* Zone 5.    */


/*
 * Bitmap mnemonics for Monitor Sensors. If gmMskMisc.c changes, then
 * must make associated changed here.
 */

#define MK_MON_RELNOTLCK    0x00000001    /* Release not in locked position. */
#define MK_MON_RELNOTREL    0x00000002    /* Release not in rel. position.   */
#define MK_MON_GRPNOTREL    0x00000004    /* Gripper not in rel. position.   */
#define MK_MON_GRPNOTGRP    0x00000008    /* Gripper not in grip position.   */
#define MK_MON_NOTALIGNED   0x00000010    /* Mask slot NOT aligned with Ext. */
#define MK_MON_NOTOCCUPIED  0x00000020    /* Aligned slot not occupied.      */
#define MK_MON_NOTINSTALLED 0x00000040    /* Cassette NOT installed          */
#define MK_MON_MASKNOTIN    0x00000080    /* Mask NOT in focal plane         */
#define MK_MON_IFUNOTIN     0x00000100    /* IFU NOT in focal plane          */
#define MK_MON_NOTINFP      0x00000200    /* Mask or IFU NOT in focal plane  */


/*
 * Device control record link structure.  Keeps all of the links for each
 * attached device in one place for ease of reference.
 */

typedef struct mkDevInfo {
    char                *name;          /* Device name.                 */
    struct link        *dirLink;        /* Directive link.              */
    struct link        *modLink;        /* Mode link.                   */
    struct link        *posLink;        /* Target position links.       */
    struct link        *velLink;        /* Velocity link.               */
    struct link        *ackLink;        /* Command acknowledge link.    */
} MK_DEV_INFO;


/*
 *  Barcode List node structure.  One of these is created for each cassette
 *  slot position to keep track of which masks are installed. 
 */

typedef struct barList {
    char        barCode[MAX_STRING_SIZE];   /* Bar code id strg.    */
    long        barCodeNum;                 /* Bar code id number.  */
    int         casNum;                     /* Cassette number.     */
    int         slotNum;                    /* Slot number.         */
    char        namedPos[64];               /* Named Position.      */
} MK_BAR_LIST;                      


/*
 * Create the default barcode list.  This will be filled in with the
 * actual masks installed when an initialize or update mode command is 
 * received.  Note that it assumes three cassettes of nine slots each....
 */

MK_BAR_LIST  mkBarList[] = { 
    { " ", MK_NO_ENTRY, 1, 1, "c1s1" }, 
    { " ", MK_NO_ENTRY, 1, 2, "c1s2" }, 
    { " ", MK_NO_ENTRY, 1, 3, "c1s3" }, 
    { " ", MK_NO_ENTRY, 1, 4, "c1s4" }, 
    { " ", MK_NO_ENTRY, 1, 5, "c1s5" }, 
    { " ", MK_NO_ENTRY, 1, 6, "c1s6" }, 
    { " ", MK_NO_ENTRY, 1, 7, "c1s7" }, 
    { " ", MK_NO_ENTRY, 1, 8, "c1s8" }, 
    { " ", MK_NO_ENTRY, 1, 9, "c1s9" }, 
    { " ", MK_NO_ENTRY, 2, 1, "c2s1" }, 
    { " ", MK_NO_ENTRY, 2, 2, "c2s2" }, 
    { " ", MK_NO_ENTRY, 2, 3, "c2s3" }, 
    { " ", MK_NO_ENTRY, 2, 4, "c2s4" }, 
    { " ", MK_NO_ENTRY, 2, 5, "c2s5" }, 
    { " ", MK_NO_ENTRY, 2, 6, "c2s6" }, 
    { " ", MK_NO_ENTRY, 2, 7, "c2s7" }, 
    { " ", MK_NO_ENTRY, 2, 8, "c2s8" }, 
    { " ", MK_NO_ENTRY, 2, 9, "c2s9" }, 
    { " ", MK_NO_ENTRY, 3, 1, "c3s1" }, 
    { " ", MK_NO_ENTRY, 3, 2, "c3s2" }, 
    { " ", MK_NO_ENTRY, 3, 3, "c3s3" }, 
    { " ", MK_NO_ENTRY, 3, 4, "c3s4" }, 
    { " ", MK_NO_ENTRY, 3, 5, "c3s5" }, 
    { " ", MK_NO_ENTRY, 3, 6, "c3s6" }, 
    { " ", MK_NO_ENTRY, 3, 7, "c3s7" }, 
    { " ", MK_NO_ENTRY, 3, 8, "c3s8" }, 
    { " ", MK_NO_ENTRY, 3, 9, "c3s9" }
};


/*
 *  Named Park/Unload positions.
 */

static char  *mkParkPositions[] = { 
     "cl1" ,
     "cl2" ,
     "cl3" ,
};


/*
 *  Device action list for each task.   The devices involved in the
 *  given task are identified by setting the appropriate flags so that
 *  the system knows which ones will be moving when executing the task.
 */

typedef struct taskList {
    int            task;                /* Task identifier.         */
    int            extResponse;         /* Extractor will move.     */
    int            grpResponse;         /* Grip will move.          */
    int            casResponse;         /* Cassette will move.      */
    int            relResponse;         /* Release will moe.        */
} MK_TASK_LIST;


/*
 *  Task list node structure.  A node is added to the end of the list for each
 *  task to perform for the current command and removed from the head of the
 *  list when it is completed during execution of the command.
 */

typedef struct mkList {
    ELLNODE         node;           /* Ptr. to next one.     */
    void            *item;          /* Ptr. to item list.    */
} MK_LIST;


/*
 *  Identification numbers for the pre-defined tasks that can be used
 *  to create command execution sequences.
 */

typedef enum
{
    MK_INDEX_REL            = 1,        /* Re-index the release mechanism   */
    MK_REL_TO_RELEASE       = 2,        /* Remove the locking pin           */
    MK_REL_TO_LOCK          = 3,        /* Insert the locking pin           */
    MK_INDEX_EXT            = 4,        /* Re-index the extractor stage     */
    MK_EXT_TO_I4            = 5,        /* Move ext to cassette end soft lim*/
    MK_EXT_TO_I4_GRP        = 6,        /* Offset from soft lim to grip pos */
    MK_EXT_TO_I4_REL        = 7,        /* Offset from soft lim to rel pos  */
    MK_EXT_TO_REL           = 8,        /* Move ext to release position     */
    MK_EXT_TO_GRP           = 9,        /* Move ext to grip position        */
    MK_EXT_TO_ID            = 10,       /* Move ext to read barcode position*/
    MK_EXT_TO_CLEAR         = 11,       /* Move ext to be clear of cassette */
    MK_EXT_TO_MASK          = 12,       /* Move ext to mask in-beam position*/
    MK_EXT_TO_IFU           = 13,       /* Move ext to IFU in-beam position */
    MK_INDEX_GRP            = 14,       /* Re-index the gripper mechanism   */
    MK_GRP_TO_RELEASE       = 15,       /* Move grip to the release posiiton*/
    MK_GRP_TO_GRIP          = 16,       /* Move grip to the gripped posiiton*/
    MK_INDEX_CAS            = 17,       /* Re-index cassette stage          */
    MK_CAS_TO_SLOT          = 18,       /* Move cas to a given slot number  */
    MK_CAS_TO_PARK          = 19,       /* Move cas to a given park position*/
    MK_ASKFOR_BARCODE_ID    = 20,       /* Trigger barcode reader           */
    MK_CHECK_BARCODE        = 21,       /* Decode barcode reader return     */
    MK_CHECK_CASSETTE       = 22,       /* Check cassette slot state        */
    MK_CK_FOCAL_PLANE       = 23,       /* Check focal plane state          */
    MK_INIT_ALL             = 24,       /* Initialize all devices at once   */
    MK_CAS_TO_C1S1          = 25,       /* Move cassette to first slot      */
    MK_TEST_ALL             = 26,       /* Test all devices at once         */
    MK_TURN_ON_OPTO         = 27        /* Turn on opto sensors & build list*/ 
} tMkTaskList;


/*
 *  The pre-defined sequences and sequence segments used to create
 *  dynamic command lists are defined below.
 */


/*
 * Re-index the mask assembly when absolute encoder sez the extractor is 
 * somewhere in the mask storage area.   Since it is not safe to index
 * the extractor in the normal manner at this point it is moved into
 * the cassette end soft limit first and then the mask replacement
 * sequence is performed using extractor offsets from this limit switch to
 * free up the extractor which can then be indexed in the normal
 * manner.
 */

static MK_TASK_LIST  mkIndexAreaAB[] = { 
    { MK_EXT_TO_I4,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_EXT_TO_I4_REL,  TRUE,  FALSE, FALSE, FALSE } ,
    { MK_INDEX_REL,      FALSE, FALSE, FALSE, TRUE  } ,
    { MK_EXT_TO_I4_GRP,  TRUE,  FALSE, FALSE, FALSE } ,
    { MK_INDEX_GRP,      FALSE, TRUE,  FALSE, FALSE } ,
    { MK_INDEX_EXT,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_EXT_TO_CLEAR,   TRUE,  FALSE, FALSE, FALSE } ,
    { MK_INDEX_CAS,      FALSE, FALSE, TRUE,  FALSE } ,
    { MK_CAS_TO_C1S1,    FALSE, FALSE, TRUE,  FALSE }
};


/*
 * Re-index the mask assembly when the absolute encoder sez the extractor
 * is outside of the mask storage area.   Since it is safe to index the
 * extractor in the normal manner even if it has a mask this is done first.
 * The check focal plane task selects one of the two following sequences
 * to complete the index depending on if a mask is detected in the focal
 * plane after the extractor has been indexed.
 */

static MK_TASK_LIST  mkIndexAreaC[] = {
    { MK_INDEX_EXT,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_CK_FOCAL_PLANE, FALSE, FALSE, FALSE, FALSE }
};


/*
 * Indexing sequence to be executed when a mask was detected in the
 * focal plane after indexing the extractor (see above).
 */

static MK_TASK_LIST  mkInFocalPlane[] = {
    { MK_EXT_TO_REL,     TRUE,  FALSE, FALSE, FALSE } ,
    { MK_INDEX_REL,      FALSE, FALSE, FALSE, TRUE  } ,
    { MK_EXT_TO_GRP,     TRUE,  FALSE, FALSE, FALSE } ,
    { MK_INDEX_GRP,      FALSE, TRUE,  FALSE, FALSE } ,
    { MK_EXT_TO_CLEAR,   TRUE,  FALSE, FALSE, FALSE } ,
    { MK_INDEX_CAS,      FALSE, FALSE, TRUE,  FALSE } ,
    { MK_CAS_TO_C1S1,    FALSE, FALSE, TRUE,  FALSE }
};


/*
 * Indexing sequence to be executed when the focal plane is empty after
 * indexing the extractor (see above).
 */

static MK_TASK_LIST  mkOutFocalPlane[] = {
    { MK_INDEX_GRP,      FALSE,  TRUE,  FALSE, FALSE } ,
    { MK_INDEX_REL,      FALSE, FALSE, FALSE, TRUE  } ,
    { MK_EXT_TO_CLEAR,   TRUE,  FALSE, FALSE, FALSE } ,
    { MK_INDEX_CAS,      FALSE, FALSE, TRUE,  FALSE } ,
    { MK_CAS_TO_C1S1,    FALSE, FALSE, TRUE,  FALSE }
};


/*
 * Take the mask or IFU that is currently in the focal plane and
 * put it back into the cassette.   Once the mask is safely
 * locked away in the cassette bring the extractor back
 * to a safe position so that the cassette can be moved.
 */

static MK_TASK_LIST  mkMoveConfig1[] = {
    { MK_EXT_TO_REL,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_REL_TO_LOCK,     FALSE, FALSE, FALSE, TRUE  } ,
    { MK_EXT_TO_GRP,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_GRP_TO_RELEASE,  FALSE, TRUE,  FALSE, FALSE } ,
    { MK_EXT_TO_CLEAR,    TRUE,  FALSE, FALSE, FALSE }
};


/*
 * Make sure that the extractor is well clear of the cassette area
 * then move the cassette to select the requested mask.   Grab
 * the mask, unlock it and bring it into the barcode reading posiiton.
 * Read the barcode.... if we have the correct mask then put it
 * into the focal plane.   If we have the wrong mask then the
 * barcode reading task will automaticially put it back into the
 * cassette via mkRejectMask below and generate an error.
 */

static MK_TASK_LIST  mkMoveConfig2[] = {
    { MK_EXT_TO_CLEAR,   TRUE,  FALSE, FALSE, FALSE } ,
    { MK_CAS_TO_SLOT,     FALSE, FALSE, TRUE,  FALSE } ,
    { MK_EXT_TO_GRP,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_GRP_TO_GRIP,      FALSE, TRUE,  FALSE, FALSE } ,
    { MK_EXT_TO_REL,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_REL_TO_RELEASE,  FALSE, FALSE, FALSE, TRUE  } ,
    { MK_EXT_TO_ID,       TRUE,  FALSE, FALSE, FALSE } ,
    { MK_ASKFOR_BARCODE_ID,  FALSE, FALSE, FALSE, FALSE } ,
    { MK_EXT_TO_MASK,    TRUE,  FALSE, FALSE, FALSE }
};


/*
 * React to reading the wrong mask ID when loading a mask into
 * the focal plane by putting it back into the cassette and
 * moving the extractor to a safe position.
 */

static MK_TASK_LIST  mkRejectMask[] = {
    { MK_ASKFOR_BARCODE_ID,  FALSE, FALSE, FALSE, FALSE } ,
    { MK_EXT_TO_REL,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_REL_TO_LOCK,     FALSE, FALSE, FALSE, TRUE  } ,
    { MK_EXT_TO_GRP,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_GRP_TO_RELEASE,  FALSE, TRUE,  FALSE, FALSE } ,
    { MK_EXT_TO_CLEAR,    TRUE,  FALSE, FALSE, FALSE }
};

/*
 * Move the extractor to the IFU position ... this is done when
 * the mask arrives in the mask position and the switches indicate
 * that it is really an IFU.
 */

static MK_TASK_LIST  mkMoveConfig3[] = {
    { MK_EXT_TO_IFU,     TRUE, FALSE, FALSE,  FALSE } 
};


/*
 * Park the cassette stage by moving it to the selected park position.
 */

static MK_TASK_LIST  mkParkCassette[] = {
    { MK_CAS_TO_PARK,   FALSE,  FALSE, TRUE, FALSE }
};


/*
 * Bring the extractor to a safe position in response to a STOP
 * command putting the mask back into the cassette.   This is detected 
 * during the GRIP_TO_RELEASE task.  Put the GRIP_TO_RELEASE task back on 
 * the list and pull the extractor back to a safe posiiton.
 */

static MK_TASK_LIST  mkStopAfterRelease[] = {
    { MK_GRP_TO_RELEASE,  FALSE, TRUE,  FALSE, FALSE } ,
    { MK_EXT_TO_CLEAR,    TRUE,  FALSE, FALSE, FALSE }
};


/*
 * Respond to a STOP command that was received after a mask
 * was gripped but before the mask was moved.   Put the GRIP_TO_GRIP
 * task back onto the task list then release the mask and pull the 
 * extractor back to a safe posiiton.
 */

static MK_TASK_LIST  mkStopAfterGrip[] = {
    { MK_GRP_TO_GRIP,      FALSE, TRUE,  FALSE, FALSE } ,
    { MK_GRP_TO_RELEASE,  FALSE, TRUE,  FALSE, FALSE } ,
    { MK_EXT_TO_CLEAR,    TRUE,  FALSE, FALSE, FALSE }
};


/*
 * Respond to a STOP command that was recieved during the cassette
 * motion to select a new mask.  Simply put the move cassette
 * task back onto the list.
 */

static MK_TASK_LIST  mkStopAtCassetteMotion[] = {
    { MK_CAS_TO_SLOT,     FALSE,  FALSE, TRUE, FALSE }
};


/*
 * Send the INIT command to all of the device control records.
 */

static MK_TASK_LIST  mkInit[] = {
    { MK_INIT_ALL,           TRUE,  TRUE,  TRUE,  TRUE  }
};


/*
 * Send the TEST command to all of the device control records.
 */

static MK_TASK_LIST  mkTest[] = {
    { MK_TEST_ALL,           TRUE,  TRUE,  TRUE,  TRUE  }
};


/*
 * Perform an update sequence.  Move the cassette to a given
 * slot and then check the cassette.   CHECK_CASSETTE will
 * either read the barcode via the next seqence or will
 * select the next mask by re-running this sequence.
 */

static MK_TASK_LIST  mkUpdateSeq[] = {
    { MK_CAS_TO_SLOT,        FALSE,  FALSE, TRUE, FALSE } ,
    { MK_CHECK_CASSETTE,     FALSE,  FALSE, FALSE, FALSE }
};


/*
 * Read the barcode on the currently selected mask as part of
 * the update sequence above.   Unlock the mask and move it to
 * the barcode reading position then read the barcode and put
 * it back in the cassette.
 */

static MK_TASK_LIST  mkReadBarCode[] = {
    { MK_EXT_TO_GRP,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_GRP_TO_GRIP,      FALSE, TRUE,  FALSE, FALSE } ,
    { MK_EXT_TO_REL,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_REL_TO_RELEASE,  FALSE, FALSE, FALSE, TRUE  } ,
    { MK_EXT_TO_ID,       TRUE,  FALSE, FALSE, FALSE } ,
    { MK_ASKFOR_BARCODE_ID,  FALSE, FALSE, FALSE, FALSE } ,
    { MK_EXT_TO_REL,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_REL_TO_LOCK,     FALSE, FALSE, FALSE, TRUE  } ,
    { MK_EXT_TO_GRP,      TRUE,  FALSE, FALSE, FALSE } ,
    { MK_GRP_TO_RELEASE,  FALSE, TRUE,  FALSE, FALSE } ,
    { MK_EXT_TO_CLEAR,    TRUE,  FALSE, FALSE, FALSE }
};


/*
 * Turn the opto sensors on then start the timer to ensure
 * sensors are not read before they are powered.
 */

static MK_TASK_LIST  mkOptoSensorsOn[] = {
    { MK_TURN_ON_OPTO,    FALSE,  FALSE, FALSE, FALSE }
};

/*
 *  Define some codes for which devices are active.
 */

typedef enum
{
    MK_ALL = 1, 
    MK_EXT = 2, 
    MK_REL = 4,
    MK_GRP = 5,
    MK_CAS = 6
} tMkAct;


/*
 * Create an error message buffer.
 */

static char errorMessage[MAX_STRING_SIZE];


/*
 *  Device support function prototypes
 */

static long mkAckReceived( ASSEMBLY_CONTROL_RECORD * );
static long mkCheckAttributes( ASSEMBLY_CONTROL_RECORD * );
static long mkIndexMode (ASSEMBLY_CONTROL_RECORD * );
static long mkInitDeviceSupport( ASSEMBLY_CONTROL_RECORD * );
static long mkInitMode (ASSEMBLY_CONTROL_RECORD * );
static long mkProcessFault ( ASSEMBLY_CONTROL_RECORD * );
static long mkMoveMode (ASSEMBLY_CONTROL_RECORD * );
static long mkStopDirective( ASSEMBLY_CONTROL_RECORD * );
static long mkTestMode (ASSEMBLY_CONTROL_RECORD * );
static long mkTrackMode (ASSEMBLY_CONTROL_RECORD * );
static long mkUpdateMode (ASSEMBLY_CONTROL_RECORD * );


/*
 * Internal function prototypes
 */
  
static long mkBuildList( ASSEMBLY_CONTROL_RECORD *, int ); 
static long mkBuildNewList( ASSEMBLY_CONTROL_RECORD *, int ); 
static long mkCheckBarcodeId( ASSEMBLY_CONTROL_RECORD * );
static long mkCheckCassette( ASSEMBLY_CONTROL_RECORD * );
static long mkCheckFocalPlane( ASSEMBLY_CONTROL_RECORD * );
static void mkClearBarList( ASSEMBLY_CONTROL_RECORD * );
static long mkCommandDevice( ASSEMBLY_CONTROL_RECORD *, MK_DEV_INFO *,
        unsigned short, char *, double );
static long mkDoTask( ASSEMBLY_CONTROL_RECORD * );
static long mkEmptyList( ASSEMBLY_CONTROL_RECORD *, int );
static long mkMoveConfig (ASSEMBLY_CONTROL_RECORD * , int, int *);
static long mkReadBarList( ASSEMBLY_CONTROL_RECORD * );
static long mkTaskFinished( ASSEMBLY_CONTROL_RECORD * );
static long mkTaskPostCheck( ASSEMBLY_CONTROL_RECORD * );
static long mkTaskPreCheck( ASSEMBLY_CONTROL_RECORD * );
static void mkTerminateTasks( ASSEMBLY_CONTROL_RECORD *, long );
static long mkTestSensors( ASSEMBLY_CONTROL_RECORD * );
static long mkTranslateDir( unsigned short );
static long mkWriteBarList( ASSEMBLY_CONTROL_RECORD * );
   

/*
 *  Create the Assembly Control Record device support code definition
 *  structure.  This will be used by the Assembly Control Record
 *  to access mask assembly device support code functions.
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
} devMaskAssembly = {
    14,
    NULL,
    mkDevInit,
    mkInitDeviceSupport,
    NULL,
    mkCheckAttributes,
    mkStopDirective,
    mkInitMode,
    mkMoveMode,
    mkTrackMode,
    mkIndexMode,
    mkTestMode,
    mkAckReceived,
    mkUpdateMode,
    mkProcessFault
    };


/*
 *  Define the internal control structure used to keep status
 *  information and the like for the device support code.
 */

typedef struct {
    double      velocity;           /* Velocity to move at.            */
    double      veloExtFast;        /* Normal Velocity for Extractor   */
    double      veloExtSlow;        /* Indexing Velocity for Extractor */
    int         barNodeNum;         /* Barcode node number.            */
    long        currentCode;        /* Current barcode number.         */
    int         currentCmd;         /* Current command executing.      */
    int         currentLoc;         /* Requested mask location.        */
    long        currentBC;          /* Requested mask barcode.         */
    int         currentParkPos;     /* Requested park position.        */
    int         optoDelayOn;        /* Waiting for optosensor powerup  */
    int         optoDelayMode;      /* Mode following sensor powerup   */
    long        quickUpdate;        /* Quick Update Flag               */
    long        singleUpdate;       /* Single Mask Update Flag         */
    int         askBarCode;         /* Asked for a barcode.            */
    int         mode;               /* Mode to execute.                */
    int         busyMode;           /* Current busy mode.              */
    int         extFinished;        /* Extractor motion complete.      */
    int         grpFinished;        /* Grip motion complete.           */
    int         casFinished;        /* Cassette motion complete.       */
    int         relFinished;        /* Release motion complete.        */
    int         cmdActive;          /* Device is executing a command   */
    int         stopRequested;      /* Stop command received.          */
    int         casParked;          /* Cassette is in a load zone      */
    long        status;             /* Saved status word               */
    SEM_ID      mutexSem;           /* mutual exclusion semaphore      */
    ASSEMBLY_CONTROL_RECORD   
                *pRecord;           /* calling record structure        */
    MK_DEV_INFO relDevice;          /* Ptr to release info.            */
    MK_DEV_INFO extDevice;          /* Ptr to extractor info.          */
    MK_DEV_INFO grpDevice;          /* Ptr to gripper info.            */
    MK_DEV_INFO casDevice;          /* Ptr to casette info.            */
    ELLLIST     taskList;           /* List of things to do.           */
    MK_BAR_LIST *barCodeList;       /* List of barcodes.               */
    int         barListEmpty;       /* Flag for barcode list.          */
} MK_DEV_PRIVATE;


/*
 *  Macro Definitions
 */

/*
 * Send debug information to the logging task if the appropriate
 * debug level is set.
 */

#define DEBUG(l,FMT,V)                                                  \
{                                                                       \
    int k=l;                                                            \
    if (k <= par->dbug)                                                 \
    printf ("%s: "FMT, taskName(0), tickGet(), par->name, V);                              \
}

#define DEBUG4(l,FMT,V1,V2,V3,V4)                                       \
{                                                                       \
    int k=l;                                                            \
    if (k <= par->dbug)                                                 \
    printf  ("%s: "FMT, taskName(0), tickGet(), par->name,                                 \
            V1, V2, V3, V4);                                            \
}

/*
 * Macro to return an error code if the function returns a null
 */

#define MK_CHECK_NULL(fn, ret)                                          \
{                                                                       \
    if ( (fn) == NULL )                                                 \
    {                                                                   \
        printf ("devMaskControl: Null return from system function at line %d\n", __LINE__);  \
        return ret;                                                     \
    }                                                                   \
}


/*
 * Save the first error message received.   This will prevent subsequent
 * error messages from overwriting the root cause of the problem.
 */

#define SET_ERR_MSG(MSG)                                                \
{                                                                       \
    if (!strlen (errorMessage))                                         \
    strncpy (errorMessage, MSG, MAX_STRING_SIZE - 1);                   \
}


/*
 * Clear out the saved error message after it has been dealt with.  This
 * allows the next error message to be saved....
 */

#define CLEAR_ERR_MSG                                                   \
{                                                                       \
    errorMessage[0] = '\0';                                             \
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkAckReceived
 *
 * INVOCATION:
 * status = mkAckReceived( par ); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record.
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Handle a change in device control record command execution state.
 *
 * DESCRIPTION: 
 * This function is called when the Assembly Control Record detects
 * that an attached device control record has either started or
 * completed a command.   Handling this state change is accomplished
 * via the following algorithm:
 *
 *   If no tasks are executing then return immediately and ignore the call.
 *   Else if timeout and reading barcode ID complete task processing.
 *   Else if timeout and waiting for opto sensors to power-up,
 *      complete task processing.
 *   Else if command has timed out then abort the command immediately.
 *   Else if any of the device records have started or stopped
 *      Check each record, if the busy field has changed
 *          if it is now IDLE then complete task processing.
 *          if it is now BUSY then ignore this call.
 *          if it is now ERR then set status.
 *   Ignore any other calls to this function. 
 *   If status has been set abort the command immediately.
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
 * Assumes that BUS1=Ext, BUS2=GRP, BUS3=CAS, BUS4=REL
 *-
 ************************************************************************
 */

static long mkAckReceived
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.       */
)
{
    MK_DEV_PRIVATE *pMkPriv;            /* Internal control structure.  */
    long    status = DAR_S_SUCCESS;     /* Function status.             */
    int        bus1;                    /* Extractor command state.     */
    int        bus2;                    /* Grip command state.          */
    int        bus3;                    /* Cassette command state.      */
    int        bus4;                    /* Release command state        */


    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );

    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkAckReceived: entry, cmdActive=%d\n", 
            pMkPriv->cmdActive );


    /*
     *  Ask assembly control record which command states have changed.
     */

    assGetBusResponse( par, &bus1, &bus2, &bus3, &bus4, NULL );


    /*
     * If we have not issued any commands and the device control 
     * record command states are changing then someone is mucking around
     * from an engineering screen.   It is safe to ignore these....
     */

    if ( pMkPriv->cmdActive == FALSE )
    {
        DEBUG(DAR_MSG_FULL, "<%ld> %s:mkAckReceived:unexpected cmd change%c\n",
              ' ');
    }


    /*
     *  A command timeout with the askBarCode flag set means that the
     *  barcode reading delay has expired.   Call taskFinished to complete
     *  the reading task.
     */

    else if ( pMkPriv->askBarCode && assCommandTimedOut( par ) )
    {
        DEBUG(DAR_MSG_FULL, "<%ld> %s:mkAckReceived:barcode read timeout%c\n", 
              ' ');
        status = mkTaskFinished( par );
    }


    /*
     *  A command timeout with the optoDelayOn flag set means that the opto
     *  sensor power on delay has expired and it should be safe to read the
     *  sensors.  Call mkTaskFinished to complete the task. 
     */

    else if ( pMkPriv->optoDelayOn && assCommandTimedOut( par ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkAckReceived:optoDelayOn timeout%c\n", 
              ' ');
        status = mkTaskFinished( par );
    }


    /*
     *  Any other timeout means that something has gone seriously wrong
     *  with the last command and the system has stopped responding.
     *  Abort the command immeditely.
     */

    else if ( assCommandTimedOut( par ) )
    {
        SET_ERR_MSG("Mask command timed out");
        status = DAR_E_TIMEDOUT;
        DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkAckReceived: command timeout%c\n", ' ');
        mkTerminateTasks (par, status);
    }


    /*
     *  If one of the busy flags are set then a device control record
     *  command has started or stopped.   Analyze each record in turn.
     */

    else if ( bus1 || bus2 || bus3 || bus4 )
    {
        /*
         * If the extractor command has completed (change to IDLE) then set
         * the extractor finished flag and call taskFinished to see
         * if there is anything else to do. 
         */

        if (bus1) switch (par->bus1)
        {
            case DAR_DEV_BUSY_IDLE:
                DEBUG(DAR_MSG_MIN, "<%ld> %s:mkAckReceived:Ext finished%c\n",
                      ' ');
                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->extFinished = TRUE;
                semGive (pMkPriv->mutexSem);
                status =  mkTaskFinished( par );
                break;

            /*
             * If the extractor command has started (change to BUSY) there is 
             * nothing to do.   This is for information only.
             */

            case DAR_DEV_BUSY_BUSY:
                DEBUG(DAR_MSG_MIN, "<%ld> %s:mkAckReceived:Ext started%c\n",
                    ' ');
                break;

            /*
             * If the extractor command has failed (change to ERR) then 
             * generate an error message and status code.
             */

            case DAR_DEV_BUSY_ERROR:
                DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkAckReceived: Ext failed%c\n",
                      ' ');
                SET_ERR_MSG("Extractor Action Failed");
                status =  DAR_E_DEVICE;
                break;
        }


        /*
         *  Check gripper action state
         */

        if (!status && bus2) switch (par->bus2)
        {
            /*
             * If the gripper command has completed (change to IDLE) then set
             * the gripper finished flag and call taskFinished to see
             * if there is anything else to do. 
             */

            case DAR_DEV_BUSY_IDLE:
                DEBUG(DAR_MSG_MIN, "<%ld> %s:mkAckReceived:Grp finished%c\n", 
                      ' ');
                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->grpFinished = TRUE;
                semGive (pMkPriv->mutexSem);
                status =  mkTaskFinished( par );
                break;

            /*
             * If the gripper command has started (change to BUSY) there is 
             * nothing to do.   This is for information only.
             */

            case DAR_DEV_BUSY_BUSY:
                DEBUG(DAR_MSG_MIN, "<%ld> %s:mkAckReceived: Grp started%c\n",
                  ' ');
                break;

            /*
             * If the gripper command has failed (change to ERR) then generate 
             * an error message and status code.
             */

            case DAR_DEV_BUSY_ERROR:
                DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkAckReceived: Grp failed%c\n",
                      ' ');
                SET_ERR_MSG("Gripper Action Failed");
                status = DAR_E_DEVICE;
                break;
        }


        /*
         *  Check cassette action state
         */

        if (!status && bus3) switch (par->bus3)
        {
            /*
             * If the cassette command has completed (change to IDLE) then set
             * the cassette finished flag and call taskFinished to see
             * if there is anything else to do. 
             */

            case DAR_DEV_BUSY_IDLE:
                DEBUG(DAR_MSG_MIN, "<%ld> %s:mkAckReceived: Cas finished%c\n",
                      ' ');
                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->casFinished = TRUE;
                semGive (pMkPriv->mutexSem);
                status =  mkTaskFinished( par );
                break;

            /*
             * If the cassette command has started (change to BUSY) there is 
             * nothing to do.   This is for information only.
             */

            case DAR_DEV_BUSY_BUSY:
                DEBUG(DAR_MSG_MIN, "<%ld> %s:mkAckReceived: Cas started%c\n",
                      ' ');
                break;

            /*
             * If the cassette command has failed (change to ERR) then generate 
             * an error message and status code.
             */

            case DAR_DEV_BUSY_ERROR:
                DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkAckReceived: Cas failed%c\n",
                      ' ');
                SET_ERR_MSG("Cassette Action Failed");
                status = DAR_E_DEVICE;
                break;
        }


        /*
         *  Check release action state
         */

        if (!status && bus4) switch (par->bus4)
        {
            /*
             * If the release has completed (change to IDLE) then set
             * the release finished flag and call taskFinished to see
             * if there is anything else to do. 
             */

            case DAR_DEV_BUSY_IDLE:
                DEBUG(DAR_MSG_MIN, "<%ld> %s:mkAckReceived: Rel finished%c\n", 
                      ' ');
                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->relFinished = TRUE;
                semGive (pMkPriv->mutexSem);
                status =  mkTaskFinished( par );
                break;


            /*
             * If the release command has started (change to BUSY) there is 
             * nothing to do.   This is for information only.
             */

            case DAR_DEV_BUSY_BUSY:
                DEBUG(DAR_MSG_MIN, "<%ld> %s:mkAckReceived: Rel started%c\n",
                  ' ');
                break;

            /*
             * If the release command has failed (change to ERR) then generate 
             * an error message and status code.
             */

            case DAR_DEV_BUSY_ERROR:
                DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkAckReceived: Release failed%c\n",
                      ' ');
                SET_ERR_MSG("Release Action Failed");
                status = DAR_E_DEVICE;
                break;
        }
    }


    /*
     * Any other reason for calling this function is ignored...
     */

    else
    {
        DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkAckReceived, invalid bus response %c\n",
              ' ');
    } 


    /*
     * Any errors detected above will have set the error message.  Call
     * terminateTasks to abort the command if this is the case.
     */

    if (status) 
    {
        mkTerminateTasks (par, status);
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkBuildList
 *
 * INVOCATION:
 * status = mkBuildList (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 * (>) mode (int) Command modifier.
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Build a list of tasks to perform based on the requested operating mode
 * and the current configuration of the mask assembly.
 *
 * DESCRIPTION:
 *
 * Create a list of tasks that will be executed sequentially to implement
 * the given command.   The following algorithm is used to generate the
 * list:
 *
 *      If the task list is not empty then abort the command immediately.
 *      Start a new task list with the first entry then build a list 
 *      with a single entry to turn on the opto sensors (saving the
 *      desired mode for use later in mkBuildNewList()..
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

static long mkBuildList
(
    ASSEMBLY_CONTROL_RECORD *par,       /* (in)  Ptr to assembly recrod */
    int         mode                    /* (in)  Operating mode.        */
)
{
    MK_LIST  *newTask = NULL;           /* Generic task pointer         */
    long     status = DAR_S_SUCCESS;    /* Return function status.      */
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct.  */
    int      i;                         /* Generic counter variable.    */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkBuildList: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  The task list must be empty (nothing left to do) before 
     *  creating a list based on the current configuration.   If some
     *  tasks remain then return an error immediately.
     */

    if ( ellCount( &(pMkPriv->taskList) ) != 0 )
    {
        DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkBuildList: taskList not empty =%d\n", 
              ellCount( &(pMkPriv->taskList) ) );
        status = MK_TASK_LIST_NOT_EMPTY;
        SET_ERR_MSG("Cannot start command while tasks remain");
        return ( status );
    }

    /*
     * Start a new task list with the first entry then build a list 
     * with a single entry to turn on the opto sensors.
     */

    newTask = ( MK_LIST *) ellFirst( &(pMkPriv->taskList) );

    DEBUG(DAR_MSG_MIN, 
          "<%ld> %s:mkBuildList: put opto sensor power up on list%c\n", ' ');

    for (i=0; i<(sizeof(mkOptoSensorsOn)/sizeof(MK_TASK_LIST )); i++)
    {
        MK_CHECK_NULL(newTask = malloc( sizeof ( MK_LIST ) ), 
                      MK_MALLOC_ERROR );
        newTask->item = (void *) &(mkOptoSensorsOn[i]);
        ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node) );
    }
    /* save the desired mode for later in mkBuildNewList() */
    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
    pMkPriv->optoDelayMode = mode;
    semGive (pMkPriv->mutexSem);

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkBuildNewList
 *
 * INVOCATION:
 * status = mkBuildNewList (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 * (>) mode (int) Command modifier.
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Build a list of tasks to perform based on the requested operating mode
 * and the current configuration of the mask assembly.
 *
 * DESCRIPTION:
 *
 * Create a list of tasks that will be executed sequentially to implement
 * the given command.   The following algorithm is used to generate the
 * list:
 *
 *      If the task list is not empty then abort the command immediately.
 *      Add to the existing (one task) list.
 *      Switch on the requested operating mode:
 *          INDEX mode:
 *              Check the position of the extractor.
 *                  If the extractor is outside of the mask 
 *                      loading/unloading area then generate a simple index
 *                      sequence using the extractor home switch.
 *                  If the extractor is inside of the mask loading/unloading
 *                      area then generate a more complex index sequence
 *                      involving a temporary index of the extractor on the
 *                      uper limit switch. This mode is always selected when
 *                      in simulation mode.
 *                  If the assembly is found in a state that can not be
 *                      covered by either of the above then abort the command
 *                      immediately.
 *
 *          MOVE mode or PARK mode:
 *              Check the state of all assembly sensors.
 *                  If the sensors do not indicate that the system is idle
 *                      with either a mask in the beam or the focal plane clear
 *                      then abort the command immediately.
 *                  If the desired mask is not in the focal plane or the
 *                      focal plane is to be cleared then remove
 *                      the current mask and put it back into the cassette.
 *                  If a new mask has been requested then select it and
 *                      put it into the focal plane.
 *                  If this is a park request then park the cassette in the
 *                      requested position.
 *
 *          INIT mode:
 *              Initialize all of the attached device control records.
 *
 *          TEST mode:
 *              Test all sensors then test all attached device control records.
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
 * None.
 *-
z ************************************************************************
 */

static long mkBuildNewList
(
    ASSEMBLY_CONTROL_RECORD *par,       /* (in)  Ptr to assembly recrod */
    int         mode                    /* (in)  Operating mode.        */
)
{
    MK_TASK_LIST *tmp;
    MK_LIST  *newTask = NULL;           /* Generic task pointer         */
    long     status = DAR_S_SUCCESS;    /* Return function status.      */
    int      numConfig;                 /* Deduced assy configuration.  */
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct.  */
    int      i;                         /* Generic counter variable.    */
    int      nodeToUpdate;              /* Single Mask Node to update   */
    int      barCount;                  /* Counter into bar list.        */

    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkBuildNewList: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  Find the place to add tasks.
     */

    if ( ( ellCount( &(pMkPriv->taskList) ) == 0 ) )
    {
        newTask = ( MK_LIST *) ellFirst( &(pMkPriv->taskList) );
    }
    else
    {
        newTask = ( MK_LIST *) ellLast( &(pMkPriv->taskList) );
    }


    /*
     *  Build a list based on the requested operating mode.
     */

    switch( mode )
    {

        /*
         *  In INDEX mode we must create a sequence to re-index all 
         *  the attached devices in a safe manner.
         */

        case DAR_MODE_INDEX:

        /*
         *  Start by logging the current configuration if requested. 
         */

        if ( (MK_EXT_POSITION & MK_EXT_BEFR_GRP ) )
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkBuildNewList: Ext not grip/release area%c\n", ' ');
        }
        else
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkBuildNewList: Ext in grip/release area%c\n", ' ');
        }

        if ( !( MK_SENSOR_WORD & MK_MON_NOTINSTALLED ) )
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkBuildNewList: Cassette installed.%c\n", ' ');
        }

        if ( !( MK_SENSOR_WORD & MK_MON_NOTALIGNED ) )
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkBuildNewList: Mask slot aligned.%c\n", ' ');
        }

        if ( !( MK_SENSOR_WORD & MK_MON_NOTOCCUPIED ) )
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkBuildNewList: Slot occupied.%c\n", ' ');
        } 
           

        /*
         *  Then check to see if the mask is clear of the loading/unloading
         *  area (before the grip position).   If so use the simple indexing
         *  sequence mkIndexAreaC (see definition for details). 
         */

        if ((assSimulateLevel (par) == DAR_SIM_NONE) &&
            ((MK_EXT_POSITION & MK_EXT_BEFR_GRP) ||
             (!(MK_EXT_POSITION & MK_EXT_BEFR_GRP) &&
             (MK_SENSOR_WORD & MK_MON_NOTOCCUPIED))))
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkBuildNewList: index on home switch%c\n", ' ');

            for (i=0; i<(sizeof(mkIndexAreaC)/sizeof(MK_TASK_LIST )); i++)
            {
                MK_CHECK_NULL(newTask = malloc( sizeof ( MK_LIST ) ), 
                              MK_MALLOC_ERROR );
                newTask->item = (void *) &(mkIndexAreaC[i]);
                ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node) );
            }
        }

        /*
         * The extractor stage was found to be in the mask handling
         * area.   Since we do not know anything else about the state
         * of the other devices we must assume the worst (half way
         * through a mask loading/unloading sequence) and do the more
         * complicated indexing sequence mkIndexAreaAB (see definition
         * for details) involving a temporary index of the extractor
         * on the cassette end soft limit switch.
         */

        else if ((assSimulateLevel (par) != DAR_SIM_NONE) || 
                 (!(MK_EXT_POSITION & MK_EXT_BEFR_GRP) &&
                  !(MK_SENSOR_WORD & MK_MON_NOTALIGNED) &&
                  !(MK_SENSOR_WORD & MK_MON_NOTOCCUPIED)))
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkBuildNewList: index on low limit switch%c\n", ' '); 

            for ( i = 0; i < (sizeof( mkIndexAreaAB )/ sizeof( MK_TASK_LIST ));
                i++ )
            {
                MK_CHECK_NULL(newTask = malloc  (sizeof (MK_LIST)), 
                              MK_MALLOC_ERROR);
                newTask->item = (void *) &(mkIndexAreaAB[i]);
                tmp = &(mkIndexAreaAB[i]);
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkBuildNewList: Adding task ==>%d\n", tmp->task );
                ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node) );
            }
        }


        /*
         * The assembly was found to be in a state where it is not
         * possible to determine a safe indexing sequence.   Manual
         * intervention is required at this point so abort the command
         * with an appropriate error message.
         */

        else
         {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkBuildNewList: Can't make safe index seq.%c\n", ' ');
            SET_ERR_MSG("Invalid pos., try index at device level");
            status = MK_BAD_EXT_POS;
        }
        break;
        

        /* 
         * In MOVE mode we must generate a sequence to select the requested
         * mask and put it into the focal plane.   We may also be asked
         * to remove the current mask and put it away.
         *
         * In PARK mode we must put the mask away if it is in the focal plane
         * and move all devices to  safe storage positions.
         */

        case DAR_MODE_MOVE:
        case DAR_MODE_PARK:

        /*
         *  Start by selecting a motion sequence based on the current
         *  position of the devices and the state of the sensors.   If 
         *  a sequence can not be automatically generated to execute the
         *  requested function because of a strange combination of position 
         *  and status information abort the command here.
         */

        if ((status = mkMoveConfig (par, mode, &numConfig)) < 0 )
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkBuildNewList: Can't determine config. %c\n", ' ');
            SET_ERR_MSG("Mask assembly position invalid, help!");
            break;
        }


        /*
         *  The mask in the focal plane either needs to be replaced or is not
         *  the one requested.   Add a sequence to store the current mask
         *  in the cassette.  See the definition of mkMoveConfig1 for details.
         */

        if (numConfig ==  MK_WRONG_MASK_IN_FP ||
            numConfig == MK_MASK_OUT_FP ) 
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkBuildNewList: Move out mask%c\n", ' ');
            for (i = 0; 
                 i < (sizeof(mkMoveConfig1) / sizeof (MK_TASK_LIST));
                 i++ )
            {
                MK_CHECK_NULL (newTask = malloc (sizeof (MK_LIST)), 
                               MK_MALLOC_ERROR );
                newTask->item = (void *) &(mkMoveConfig1[i]);
                ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node));
            }
        }


        /*
         *  We have been asked to put a different mask into the focal
         *  plane so add a sequence to select a new mask and insert it.
         *  See definition of mkMoveConfig2 for details.
         */

        if (numConfig ==  MK_WRONG_MASK_IN_FP ||
            numConfig == MK_NEW_FP_IN ) 
        {
            DEBUG(DAR_MSG_MIN,
                  "<%ld> %s:mkBuildNewList: Moving into fp%c\n", ' ');
            for (i = 0; 
                 i < (sizeof (mkMoveConfig2)/ sizeof (MK_TASK_LIST));
                 i++ )
            {
                MK_CHECK_NULL (newTask = malloc (sizeof ( MK_LIST )), 
                               MK_MALLOC_ERROR);
                newTask->item = (void *) &(mkMoveConfig2[i]);
                ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node) );
            }
        }


        if ( numConfig ==  MK_NOTHING && mode == DAR_MODE_MOVE )
        {

            /*
             *  Turns out that the current configuraton exactly matches the
             *  requested state of the assembly so there is nothing to do.
             */

            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkBuildNewList: Nothing to do%c\n", ' ');
        }

        else if ( mode == DAR_MODE_PARK )
        {

            /*
             *  In park mode we always finish off by moving the cassette to
             *  the requested parking position.
             */

            MK_CHECK_NULL( newTask = malloc( sizeof ( MK_LIST ) ), 
                          MK_MALLOC_ERROR );
            newTask->item = (void *) &(mkParkCassette[0]);
            ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node) );
        }

        break;


        /*
         *  In update mode we need to generate a sequence to cycle through
         *  all of the possible mask storage slots in the three cassettes
         *  and determine which mask, if any, is in each. 
         */

        case DAR_MODE_UPDATE:

        /*
         *  Start by selecting a motion sequence based on the current
         *  position of the devices and the state of the sensors.   If 
         *  a sequence can not be automatically generated to execute the
         *  requested function because of a strange combination of position 
         *  and status information abort the command here.
         */

        if (( status = mkMoveConfig( par, mode, &numConfig )) < 0 )
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkBuildNewList: Invalid assy configuration%c\n", ' ');
            SET_ERR_MSG("Mask assembly position invalid, help!");
            break;
        }


        /*
         *  If there is already something in the focal plane it will
         *  be necessary to put it away before starting the update 
         *  sequence.   See definition of mkMoveConfig1 for details
         *  of the tasks that are added here.
         */

        if ( numConfig == MK_MASK_OUT_FP ) 
        {
            DEBUG(DAR_MSG_MIN, 
                  "<%ld> %s:mkBuildNewList: Move out mask%c\n", ' ');
            for ( i = 0; 
                  i < (sizeof( mkMoveConfig1)/ sizeof( MK_TASK_LIST ));
                  i++ )
            {
                MK_CHECK_NULL( newTask = malloc( sizeof ( MK_LIST ) ), 
                              MK_MALLOC_ERROR );
                newTask->item = (void *) &(mkMoveConfig1[i]);
                ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node));
            }
        }

	/*
	 *  Check the Update Mode.  Could be Quick or Single
	 */
	if (MK_QUICK_UPDATE == TRUE)
	  {
	    /* Set the quick update flag */
	    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
	    pMkPriv->singleUpdate = FALSE;
	    pMkPriv->quickUpdate = TRUE;
	    semGive (pMkPriv->mutexSem);
	    
	    DEBUG(DAR_MSG_WARNING, 
		  "<%ld> %s:mkBuildNewList: WARNING! Performing QUICK UPDATE - this does NOT clear the existing barcode list!%c\n", ' ');

	    /*
	     *  Initialize the barcode list node number to zero so that we
	     *  overwrite the current list.
	     */
	    
	    pMkPriv->barNodeNum = 0;
	  }
	else if (MK_QUICK_UPDATE == -1)
	  {
	    /* Set the Single Update flag, Clear the quick update flag */
	    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
	    pMkPriv->singleUpdate = TRUE;
	    pMkPriv->quickUpdate = FALSE;
	    semGive (pMkPriv->mutexSem);
	    

	    DEBUG(DAR_MSG_WARNING, 
		  "<%ld> %s:mkBuildNewList: WARNING! Performing SINGLE UPDATE - this does NOT clear the existing barcode list!%c\n", ' ');

	    /* figure out the node to use for the given cassette and slot */
	    nodeToUpdate = -1;
	    for ( barCount = 0; barCount < MK_MAX_SLOTS; barCount++ )
	      {
		if ((pMkPriv->barCodeList[barCount].casNum == MK_UPDATE_SINGLE_C)&&(pMkPriv->barCodeList[barCount].slotNum == MK_UPDATE_SINGLE_S))
		  {
		    nodeToUpdate=barCount;
		    break;
		  }
	      }

	    if (nodeToUpdate > 0) 
	      {
		semTake (pMkPriv->mutexSem, WAIT_FOREVER);
		pMkPriv->barNodeNum = nodeToUpdate;
		semGive (pMkPriv->mutexSem);
		
		DEBUG4(DAR_MSG_LOG, 
		      "<%ld> %s:mkBuildNewList: SINGLE UPDATE C:%i S:%i using node: %i%c\n", pMkPriv->barCodeList[nodeToUpdate].casNum, pMkPriv->barCodeList[nodeToUpdate].slotNum, nodeToUpdate, ' ');
	      }
	    else 
	      {
		/* Node not found .. raise an error. Shouldn't happen because of limit checking on C and S inputs. */
		DEBUG(DAR_MSG_ERROR, 
		      "<%ld> %s:mkBuildNewList: Single Update, Node not found%c\n", ' ');
		SET_ERR_MSG("Single Update failed, (C,S) not found!");
		break;
	      } 
	  }
	else
	  {
	    /* Clear the quick/single update flags */
	    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
	    pMkPriv->singleUpdate = FALSE;
	    pMkPriv->quickUpdate = FALSE;
	    semGive (pMkPriv->mutexSem);
	    
	    /* Clear the barcode list if it's a normal UPDATE (not a quickie or single). */
	    mkClearBarList( par );
	    DEBUG(DAR_MSG_LOG, 
		  "<%ld> %s:mkBuildNewList: FULL UPDATE - old list cleared.%c\n", ' ');

	    /*
	     *  Initialize the barcode list node number to zero so that we
	     *  overwrite the current list.
	     */
	    
	    pMkPriv->barNodeNum = 0;

	  }

	/*
	 *  Now add the sequence to cycle through the storage slots
	 *  and record whatever is found.   See the definition of
	 *  updateSeq for details of the tasks added to do this.
	 *  For the Single Update mode, this will go to the configured node.
	 */
	
	for ( i = 0; 
	      i < (sizeof( mkUpdateSeq)/ sizeof( MK_TASK_LIST ));
	      i++ )
	  {     
	    MK_CHECK_NULL( newTask = malloc( sizeof ( MK_LIST ) ), 
			       MK_MALLOC_ERROR );
	    newTask->item = (void *) &(mkUpdateSeq[i]);
	    ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node));
	  }
	
        break;


        /*
         * For INIT mode we add tasks to initialize all of the
         * attached device control records at once.
         */

        case DAR_MODE_INIT:

        DEBUG(DAR_MSG_FULL, "<%ld> %s:mkBuildNewList: adding mkInit task list %c\n", ' ');
        MK_CHECK_NULL( newTask = malloc( sizeof ( MK_LIST ) ), 
                       MK_MALLOC_ERROR );
        newTask->item = (void *)&(mkInit[0]);
        ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node) );
       
        break;


        /*
         *  For TEST mode we add tasks to test the attached switches
         *  and sensors then test all of the attached device control 
         *  records at once. 
         */ 

        case DAR_MODE_TEST:

        DEBUG(DAR_MSG_FULL, "<%ld> %s:mkBuildNewList: adding mkTest task list %c\n", ' ');
        MK_CHECK_NULL( newTask = malloc( sizeof ( MK_LIST ) ), 
                       MK_MALLOC_ERROR );
        newTask->item = (void *)&(mkTest[0]);
        ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node) );

        break;


        /*
         *  Unsupported modes get trapped here and will cause an immediate
         *  command abort.
         */    

        default:

        DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkBuildNewList: invalid mode=%d\n", mode );
        status = MK_INVALID_MODE;
        SET_ERR_MSG("Invalid Mask assembly mode");

        break;

    }  /* End of switch on MODE */

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkCheckAttributes
 *
 * INVOCATION:
 * status = mkCheckAttributes( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * To check to insure that the requested command can be implemented with
 * the given arguments.
 *
 * DESCRIPTION:
 * Reject invalid commands using the following algorithm:
 *
 *      Reject track mode requests.
 *      Reject all commands while the assembly is busy.
 *      Reject mask requests if the assembly has not been updated.
 *      Reject mask requests if the requested mask is not in the list
 *      Reject park requests if the position does not correspond to one
 *         of the cassettes.
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
 * None.
 *-
 ************************************************************************
 */

static long mkCheckAttributes
(
    ASSEMBLY_CONTROL_RECORD *par       /* (in)  Ptr to ass rec.         */
)
{
    long    status = DAR_S_SUCCESS;    /* Returned function status.     */
    MK_DEV_PRIVATE *pMkPriv;           /* Ptr to private dev. struct.   */
    int     barCount;                  /* Counter into bar list.        */
    long   idTarget;                   /* Target barcode id             */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkCheckAttributes: entry, sim=%d\n", 
            assSimulateLevel(par) );

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     * Since this is a new command request start by clearing out any
     * old error messages.
     */
        
    CLEAR_ERR_MSG;


    /* 
     *  Reject a track mode request immediately since it is not supported
     *  by the mask assembly.
     */

    if (par->mode == DAR_MODE_TRACK)
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkCheckAttributes: track mode is not valid for mask assembly %c\n",  ' ' );
        assAddErrorMessage (par, "Track mode not valid for mask assembly");
        status = MK_INVALID_MODE;
        return status;
    }


    /*
     *  Assembly must be idle before accepting new commands.
     */

    if( par->busy == DAR_BUSY_BUSY)
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkCheckAttributes: assembly is busy %c\n",  ' ' );
        status = MK_INVALID;
        assAddErrorMessage (par, "Error, mask assembly is BUSY");
        return status;
    }
    
    /*
     *  Check requests to insert or remove masks.
     */

    if (( par->dir == DAR_DIR_PRESET || par->dir == DAR_DIR_START ) &&
        par->mode == DAR_MODE_MOVE )
    {
        /*
         * Make sure that no one has switched data types on us.
         * This is a little excessive.....
         */

        if ( par->fta != DBF_LONG || par->ftb != DBF_LONG )
        {
            DEBUG(DAR_MSG_ERROR,
                  "<%ld> %s:mkCheckAttributes:invalid data types%c\n",' ' );
            status = MK_INVALID;
            assAddErrorMessage( par, "Error, Invalid attributes types");
            return status;
        }

        /*
         * A request to insert a mask.   Check to insure that the 
         * desired mask exists in the list of installed masks.
         */        

        else if (MK_MASK_IFU_LOC == MK_CMD_INBEAM)
        {

            idTarget = MK_BARCODE_ID_IN;

            /*
             * Must be within Mask ID ranges.
             */

            if ( (idTarget < MK_BARCODE_MIN || 
                  idTarget > MK_BARCODE_MAX)      &&
                 (idTarget < MK_BARCODE_MIN_SIM || 
                  idTarget > MK_BARCODE_MAX_SIM)   )
            {
                DEBUG(DAR_MSG_ERROR,"<%ld> %s:mkCheckAttributes: Target mask ID out of range, ID=%ld\n",
                      idTarget );
                status = MK_INVALID_BARCODE;
                assAddErrorMessage( par, "Barcode invalid - out of range");
                return status;
            }
 
            /*
             * There must be a barcode list before accepting requests
             * to put a mask into the beam!
             */
        
            if ( pMkPriv->barListEmpty )
            {
                DEBUG(DAR_MSG_ERROR,
                      "<%ld> %s:mkCheckAttributes:No barcodes%c\n",' ');
                status = MK_EMPTY_BARCODE_LIST;
                assAddErrorMessage( par, "Mask set unknown, update");
                return status;
            }


            /*
             * A list exists, now scan the list of installed masks looking
             * for a match with the requested mask.
             */

            else
            {
                for ( barCount = 0; barCount < MK_MAX_SLOTS; barCount++ )
                {
                    if (pMkPriv->barCodeList[barCount].barCodeNum != 
                         MK_NO_ENTRY )
                    {
                        DEBUG(DAR_MSG_MAX, 
                              "<%ld> %s:mkCheckAttributes: checking: %ld\n",
                              pMkPriv->barCodeList[barCount].barCodeNum );
                    }
            

                    /*
                     * If the codes match then save the slot location and
                     * exit the scanning loop.
                     */
 
                    if (pMkPriv->barCodeList[barCount].barCodeNum == 
                        idTarget )
                    {
                        DEBUG(DAR_MSG_MAX, 
                              "<%ld> %s:mkCheckAttributes: found at: %d\n",
                              barCount );
                        semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                        pMkPriv->barNodeNum = barCount;
                        semGive (pMkPriv->mutexSem);
                        break;
                    }   
                }
              
                /*
                 * Not found in the list, reject the command.
                 */

                if (barCount == MK_MAX_SLOTS)
                {
                    status = MK_INVALID_MASK;
                    assAddErrorMessage(par, "This mask is not installed");
                    return (status);
                }
            } /* If barcode list is empty else */
        } /* If asked to move inbeam */

        
        /*
         *  If not a request to insert a mask then must be a request
         *  to remove a mask otherwise the request is invalid.
         */ 
        
        else if (MK_MASK_IFU_LOC != MK_CMD_OUTBEAM) 
        {

            DEBUG(DAR_MSG_ERROR,
                  "<%ld> %s:mkCheckAttributes: invalid location%c\n", ' ');
            status = MK_INVALID_LOCATION;
            assAddErrorMessage( par, "Invalid mask location [0 | 1]");
            return status;
        }
    } /* If inserting or removing a mask */


    /*
     *  Request to park all devices.  Make sure park location is valid.
     */

    else if ((par->dir == DAR_DIR_PRESET || par->dir == DAR_DIR_START) &&
                  par->mode == DAR_MODE_PARK )
    {
        /*
         *  Check the field type.  Again, a little excessive.....
         */

        if ( par->ftc != DBF_LONG )
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkCheckAttributes: invalid park position data type%c\n",' ');
            status = MK_BAD_PARK_POSITION;
            assAddErrorMessage( par, "Park position must be of type LONG");
            return status;
        }

        /*
         *  Only one park position for each cassette.
         */

        else if ( MK_PARK_POSITION_IN < 0 ||
                  MK_PARK_POSITION_IN > MK_NUM_CASSETTES )
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkCheckAttributes:bad park position%c\n",' ');
            status = MK_BAD_PARK_POSITION;
            assAddErrorMessage (par, "Invalid park position");
            return status;
        }
    }/* else if Park */

    /*
     *  Request to update.  Make sure Cassette and Slot are valid in case this is a Single Mask Update.
     */

    else if ((par->dir == DAR_DIR_PRESET || par->dir == DAR_DIR_START) &&
                  par->mode == DAR_MODE_UPDATE )
    {

        if ( MK_UPDATE_SINGLE_C < 1 || MK_UPDATE_SINGLE_C > 3 )
        {
            DEBUG(DAR_MSG_ERROR,
                  "<%ld> %s:mkCheckAttributes: MK_UPDATE_SINGLE_C out of range%c\n",
                  ' ' );
            status = MK_INVALID;
            assAddErrorMessage( par, "Global var MK_UPDATE_SINGLE_C out of range(1:3)");
            return status;
        }

        if ( MK_UPDATE_SINGLE_S < 1 || MK_UPDATE_SINGLE_S > 9 )
        {
            DEBUG(DAR_MSG_ERROR,
                  "<%ld> %s:mkCheckAttributes:MK_UPDATE_SINGLE_S out of range(1:9)%c\n",
                  ' ' );
            status = MK_INVALID;
            assAddErrorMessage( par, "Global var MK_UPDATE_SINGLE_S out of range");
            return status;
        }

        
    }/* else if Update */


    /*
     *  Get the extractor fast/slow velocities.  These are checked regardless
     *  of MODE since these attributes don't form part of the command interface.
     *  These used to be assemby input and are now global variables.
     */

    if (par->dir == DAR_DIR_PRESET || par->dir == DAR_DIR_START)
    {

        /*
         *  Ensure that extractor velocities are within attribute limits.
         */

        if ( MK_EXT_VEL_FAST < 20.0 || MK_EXT_VEL_FAST > 40.0 )
        {
            DEBUG(DAR_MSG_ERROR,
                  "<%ld> %s:mkCheckAttributes: MK_EXT_VEL_FAST out of range (normal extractor velocity)%c\n",
                  ' ' );
            status = MK_INVALID;
            assAddErrorMessage( par, "Global var MK_EXT_VEL_FAST out of range");
            return status;
        }

        if ( MK_EXT_VEL_SLOW < 5.0 || MK_EXT_VEL_SLOW > 20.0 )
        {
            DEBUG(DAR_MSG_ERROR,
                  "<%ld> %s:mkCheckAttributes:MK_EXT_VEL_SLOW out of range (extractor indexing velocity)%c\n",
                  ' ' );
            status = MK_INVALID;
            assAddErrorMessage( par, "Global var MK_EXT_VEL_SLOW out of range");
            return status;
        }

        /*
         *  All is well, so save extractor velocities in private structure
         *  to use when calling mkCommandDevice later.  
         */

        semTake (pMkPriv->mutexSem, WAIT_FOREVER);
        pMkPriv->veloExtFast = (double)MK_EXT_VEL_FAST;
        pMkPriv->veloExtSlow = (double)MK_EXT_VEL_SLOW;
        semGive (pMkPriv->mutexSem);

    } /* else if PRESET or START */

    return( status );
}      

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkCheckBarcodeId
 *
 * INVOCATION:
 * status = mkCheckBarcodeId( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Read and analyze a mask barcode identifier.
 *
 * DESCRIPTION:
 * Read the string returned by the barcode reader.  Analyze and store this
 * information using the following algorithm:
 *
 *      In simulation mode fake the read by writing the slot number to the
 *          reader string input location.
 *
 *      If the reader returned an error string indicating that it could
 *          not read the code return an error.
 *
 *      If the barcode can not be converted to a long integer return an error.
 *
 *      If this read was a check before inserting a mask and the numbers do not
 *          agree then put the mask back and return an error otherwise write 
 *          this code to the output status word as the in-beam mask.
 *
 *      If this read was part of an update sequence check for duplicate
 *          numbers then add tasks put the mask back and select the next one.
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
 * DEFICIENCIES:
 *
 *-
 ************************************************************************
 */

static long mkCheckBarcodeId
(
    ASSEMBLY_CONTROL_RECORD 
                *par        /* (in)  Ptr to ass rec.    */
)
{
    MK_DEV_PRIVATE *pMkPriv;                /* Ptr to private dev. struct. */
    long    status = DAR_S_SUCCESS;         /* Function status.            */
    long    value;                          /* Barcode value.              */
    char    *end;                           /* End string pointer.         */
    MK_LIST     *newTask = NULL;            /* Generic task pointer.       */
    int         i;                          /* Generic counter.            */
    char    idString[MAX_STRING_SIZE];      /* Barcode Reader string       */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkCheckBarcodeId: entry%c\n",  ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  Copy the barcode reader ID string into a local variable.
     */

      strcpy ( idString, MK_BARCODE_READ );

    /*
     *  Is the ID okay?
     */

    if ( !strlen ( idString ) ||
         (strncmp ( idString, "NR", 2 ) == 0) ||
         (strncmp ( idString, "ERROR", 5 ) == 0) )
    {
        DEBUG(DAR_MSG_ERROR,"<%ld> %s:mkCheckBarcodeId: Barcode Reader failure? Returned string=%s\n",
              idString );
       /* Barcode reader failure ack only if ignore barcode flag is set to false */
       /* TRUE = 1 FALSE = 0 */

       if ( MK_IGNORE_BARCODE == FALSE)
	{
        printf("TEST DEBUG: ID string is not OK, MK_IGNORE_BARCODE = FALSE");
       	status = MK_BAR_READ_FAIL;
       	}
       else
      	{
        /* if flag is TRUE, just set the barcode to 000000000 and continue
        status will remain DAR_S_SUCCESS */
        printf("TEST DEBUG: ID string is not OK, MK_IGNORE_BARCODE = TRUE");
       	*idString = '\0';
        strcat(idString,"000000000"); 
	}       
        
    }

    /*
     * Reader was happy.  Ensure that the string is valid.
     */

    else
    {
        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:mkCheckBarcodeId: Read the barcode id.%c\n",' ');

        /*
         * String must convert to a long integer.
         */

        value = strtol(idString, &end, 10 );
        if ( *end != '\0' )
        {
            DEBUG(DAR_MSG_ERROR,"<%ld> %s:mkCheckBarcodeId: Invalid barcode - not an integer, string=%s\n",
                  idString );
            status = MK_INVALID_BARCODE;
        }

        /*
         * Long integer must be within Mask ID ranges.
         */

        else if ( (value < MK_BARCODE_MIN     || value > MK_BARCODE_MAX)   &&
                  (value < MK_BARCODE_MIN_SIM || value > MK_BARCODE_MAX_SIM) )
        {
            DEBUG(DAR_MSG_ERROR,"<%ld> %s:mkCheckBarcodeId: Barcode ID out of range, ID=%ld\n", value );
            status = MK_INVALID_BARCODE;
        }
    }


    /*
     * The rest depends on command mode
     */

    if ( pMkPriv->currentCmd == DAR_MODE_MOVE )
    {
        if (status != DAR_S_SUCCESS)
        {
            DEBUG(DAR_MSG_ERROR,"<%ld> %s:mkCheckBarcodeId: Barcode ID could not be verified (ID=%ld), but move into focalplane anyway\n", value );
            SET_ERR_MSG("Mask in FP but couldn't verify barcode");
        }

        /*
         * Compare the ID read with what the target expected if ignoreFlag is set to zero 
         */
        else if ( value != pMkPriv->currentBC  && MK_IGNORE_BARCODE == FALSE) 
        {
            /* ID and target are different */
            printf("TEST DEBUG: Compare ID with target expected: MK_IGNORE_BARCODE is FALSE");
            DEBUG4(DAR_MSG_ERROR,
                  "<%ld> %s:mkCheckBarcodeId:Wrong mask found! Expected %ld but found %ld%c%c\n", pMkPriv->currentBC, value,' ' ,' ' );
            SET_ERR_MSG("Wrong mask found in this slot");
            status = MK_WRONG_BARCODE;
        }

        if (status != MK_WRONG_BARCODE)
        {

            /* ID is same as target so make it the current ID code */
            DEBUG(DAR_MSG_FULL,
                  "<%ld> %s:mkCheckBarcodeId: Valid ID=%s\n", idString );
            semTake (pMkPriv->mutexSem, WAIT_FOREVER);
            pMkPriv->currentCode = pMkPriv->currentBC;
            semGive (pMkPriv->mutexSem);
        }
    }

    /*
     * In update we will read the mask and save the number.
     */

    else if ( pMkPriv->currentCmd == DAR_MODE_UPDATE )
    {

        if (status != DAR_S_SUCCESS)
        {
            /* flag the barcode as invalid but carry on */
            value = MK_NO_BARCODE;
            DEBUG(DAR_MSG_WARNING,"<%ld> %s:mkCheckBarcodeId: Flagging current mask as unidentifiable, node=%d\n", pMkPriv->barNodeNum-1 );
            SET_ERR_MSG("At least one barcode was not readable");
        }

        /*
         * Scan the masks read already to trap duplicate masks.
         */

        else if ( pMkPriv->barNodeNum > 1 )
        {
            for (i=0; i < pMkPriv->barNodeNum-2; i++)
            {
                if (pMkPriv->barCodeList[i].barCodeNum == value)
                {
                    DEBUG(DAR_MSG_ERROR,
                        "<%ld> %s:mkCheckBarcodeId: Duplicate%c\n",' ');
                    SET_ERR_MSG("Duplicate mask found in this slot");
                    status = MK_DUPLICATE_BARCODE;

                    break; /* no need to check any more */
                }
            }
        }

        if (status != MK_DUPLICATE_BARCODE)
        {
            /*
             *  Then save the read barcode ID to the 
             *  barcode list.  The node number has been
             *  pointed to the next slot in checkCassette so
             *  take this into consideration.  Set the empty
             *  flag to FALSE.
             */

            semTake (pMkPriv->mutexSem, WAIT_FOREVER);
            pMkPriv->barCodeList[pMkPriv->barNodeNum-1].barCodeNum = value;
            strncpy (pMkPriv->barCodeList[pMkPriv->barNodeNum-1].barCode,
                     idString, MAX_STRING_SIZE - 1 );
            pMkPriv->barListEmpty = FALSE;
            semGive (pMkPriv->mutexSem);
            if (value != MK_NO_BARCODE)
            {
                DEBUG(DAR_MSG_FULL,
                     "<%ld> %s:mkCheckBarcodeId: Valid ID=%ld\n",
                     pMkPriv->barCodeList[pMkPriv->barNodeNum-1].barCodeNum );
            }
            else 
            {
                DEBUG(DAR_MSG_FULL,
                     "<%ld> %s:mkCheckBarcodeId: Dummy ID=%ld\n",
                     pMkPriv->barCodeList[pMkPriv->barNodeNum-1].barCodeNum );
            }
        }
    }
    else
    {
        DEBUG(DAR_MSG_ERROR,"<%ld> %s:mkCheckBarcodeId: Should not be reading barcodes in this command mode:%d\n", pMkPriv->currentCmd );
        SET_ERR_MSG("This command mode can't use barcodes");
        status = MK_INVALID_MODE;
    }


    /*
     * If the barcode read was not the same as the target ID (MOVE mode) 
     * or if it is a duplicate (UPDATE mode) or if the mode was invalid
     * then replace the mask and abort the command.  Note that any other
     * failures will cause the task list to continue on.  So if the mode
     * is MOVE it will place the mask in the focalplane and if the mode
     * is UPDATE it will go on to check the next slot.  Any bad status
     * will still cause the command to end in an error.
     */

    if ( status == MK_WRONG_BARCODE     || 
         status == MK_DUPLICATE_BARCODE ||
         status == MK_INVALID_MODE        )
    {
        /* 
         *  Kill remaining tasks in the command sequence.
         */

        mkEmptyList (par, TRUE);
        newTask = (MK_LIST *) ellFirst (&pMkPriv->taskList);

       
        /* 
         * And replace them with the reject mask sequence.
         */

        for (i = 0; i < (sizeof (mkRejectMask)/sizeof (MK_TASK_LIST)); i++)
        {
            MK_CHECK_NULL(newTask = malloc(sizeof(MK_LIST)), MK_MALLOC_ERROR);
            newTask->item = (void *) &(mkRejectMask[i]);
            ellAdd ((ELLLIST *) &(pMkPriv->taskList), &(newTask->node));
        }
    }


    /*
     * Copy any bad status into private structure to indicate 
     * barcode related failure so that the index flag is kept.
     */

    if (status != DAR_S_SUCCESS)
    {
        semTake (pMkPriv->mutexSem, WAIT_FOREVER);
        pMkPriv->status = status;
        semGive (pMkPriv->mutexSem);

        /* 
         * Restore status to allow task to complete.
         */

        status = DAR_S_SUCCESS;
    }

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkCheckCassette
 *
 * INVOCATION:
 * status = mkCheckCassette( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Check to see if a cassette and/or mask is installed in the given slot.
 *
 * DESCRIPTION:
 * This function is called as part of the updating sequence after the
 * cassette stage has been moved to a new mask position.  The following
 * algorithm is used to decide what to do next:
 * 
 *      In simulation mode we always ask that a mask be read.
 *      Otherwise analyze the cassette sensors:
 *          Look for a cassette:
 *              If not there then skip to the next one unless it is the
 *                  last one that is missing.   In that case you are done.
 *              If not there for any but the first slot this indicates a
 *                  sensor failure so abort the command immediately.
 *              If it is there then look for a mask:
 *                  If there is a mask then request that it be read.
 *                  If there is no mask request that we skip to the next slot.
 *
 *      
 *      If we requested that a mask be read then add tasks to remove it, read
 *          the code and replace it again.
 *
 *      If we requested a mask be read or a move to the next slot then add 
 *          tasks to move to the next slot.
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
 * DEFICIENCIES:
 * 
 *-
 ************************************************************************
 */

static long    mkCheckCassette
(
    ASSEMBLY_CONTROL_RECORD 
                *par        /* (in)  Ptr to ass rec.    */
)
{
    int        casNum;                  /* Cassette number.             */
    int        i;                       /* Generic counter.             */
    int        goToNextSlot = FALSE;    /* Go to next slot task list.   */
    MK_LIST    *newTask = NULL;         /* Generic task pointer         */
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct.  */
    int        readTheBarCode = FALSE;  /* Read barcode flag.           */
    int        slotNum;                 /* Slot number.                 */
    long       barCodeNum;              /* mask ID number.              */
    long    status = DAR_S_SUCCESS;     /* Function status.             */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkCheckCassette: entry%c\n",  ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  The full names are way to long to work with so...
     */

    casNum       = pMkPriv->barCodeList[pMkPriv->barNodeNum].casNum;
    slotNum      = pMkPriv->barCodeList[pMkPriv->barNodeNum].slotNum;
    barCodeNum   = pMkPriv->barCodeList[pMkPriv->barNodeNum].barCodeNum;


    /* 
     *  In simulation mode assume there is a cassette installed and a slot
     *  occupied.
     */

    if ( assSimulateLevel(par) != DAR_SIM_NONE )
    {
        /*
         *  Set the flag to read the barcode unless we're in QUICk UPDATE
         *  mode and there is a valid entry for this slot.
         */

        if ( barCodeNum != MK_NO_BARCODE && 
             barCodeNum != MK_NO_ENTRY && 
             pMkPriv->quickUpdate == TRUE )
        {
            DEBUG4(DAR_MSG_MIN, 
                  "<%ld> %s:mkCheckCassette:Quick Update - skipping C%d_S%d  Barcode:%ld%c\n", casNum, slotNum, barCodeNum, ' ');
            goToNextSlot = TRUE;
        }
        else
        {
            DEBUG(DAR_MSG_MAX, 
                  "<%ld> %s:mkCheckCassette:Setting barcode read flag%c\n",' ');
            readTheBarCode = TRUE;
        }
    }


    /*
     * Otherwise check to see if a cassette is installed.   If not then
     * skip to the next cassette.
     */

    else if (  ( MK_SENSOR_WORD & MK_MON_NOTINSTALLED ) )
    {

        /*
         *  If the cassette was missing for any slot except the first then
         *  the sensor is not working properly.   Abort the command.
         */

        if ( slotNum != 1 )
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkCheckCassette: Cassette disappeared!%c\n",' ');
            status = MK_CAS_ERROR;
            SET_ERR_MSG ("Mask Cassette Missing!!");
        }

        /*
         * If it was the last cassette that was missing then no point in
         * going on since there won't be any more slots to check anyway.
         */

        else if ( casNum == MK_NUM_CASSETTES ) 
        {
            DEBUG(DAR_MSG_MAX, 
                  "<%ld> %s:mkCheckCassette: Last cas empty, done%c\n",' ');
        
        }
	else if (pMkPriv->singleUpdate == TRUE)
	{
	  /* We're done, even if the cassette wasn't found */
	  DEBUG(DAR_MSG_ERROR, 
		"<%ld> %s:mkCheckCassette: Single Update failed, Cassette not found!%c\n",' ');  
	}

        /*
         * Add tasks to skip to the first slot of the next cassette.
         */

        else
        {
            semTake (pMkPriv->mutexSem, WAIT_FOREVER);
            pMkPriv->barNodeNum += MK_NUM_SLOTS - 1; /* Below we increment.*/
            goToNextSlot = TRUE;
            semGive (pMkPriv->mutexSem);
        }
    } /* End of If the Cassettte is not installed. */

    
    /*
     * Cassette is installed,check to see if it is occupied.
     */

    else
    {
        /*
         * If it is occupied then the slot must be aligned and the locking pin
         * inserted before we can remove it to read the barcode.
         */

        if ( !( MK_SENSOR_WORD & MK_MON_NOTOCCUPIED ) &&
            ( ( MK_SENSOR_WORD & MK_MON_NOTALIGNED ) ||
            ( MK_SENSOR_WORD & MK_MON_RELNOTLCK ) ) )
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkCheckCassette: Not safe to remove mask%c\n",' ');
            status = MK_SLOT_ERROR;
            SET_ERR_MSG("Cassette misalignment .. reindex" );
        }

        else
        {
            /*
             * If there is a mask in the slot request that tasks be added to
             * remove it and read the barcode.
             */

            if ( !( MK_SENSOR_WORD & MK_MON_NOTOCCUPIED ) )
            {
                if ( barCodeNum != MK_NO_BARCODE && 
                     barCodeNum != MK_NO_ENTRY   && 
                     pMkPriv->quickUpdate == TRUE   )
                {
                    DEBUG4(DAR_MSG_MIN, 
                          "<%ld> %s:mkCheckCassette:Quick Update - skipping C%d_S%d  Barcode:%ld%c\n", casNum, slotNum, barCodeNum, ' ');
                    goToNextSlot = TRUE;
                }
                else
                {
                    DEBUG(DAR_MSG_MAX, 
                          "<%ld> %s:mkCheckCassette:Setting barcode read flag%c\n",' ');
                    readTheBarCode = TRUE;
                }

            }

	    else if (pMkPriv->singleUpdate == TRUE)
	      {
		/* We're done, even if the slot was empty .. need to clear the old barcode for this slot (if any) */
		DEBUG(DAR_MSG_LOG, 
		      "<%ld> %s:mkCheckCassette: Single Update, Slot empty! clearing barcode%c\n",' ');

		semTake (pMkPriv->mutexSem, WAIT_FOREVER);
		pMkPriv->barCodeList[ pMkPriv->barNodeNum].barCodeNum = MK_NO_ENTRY;
		pMkPriv->barCodeList[ pMkPriv->barNodeNum].barCode[0] = '\0';
		semGive (pMkPriv->mutexSem);		
	      }


            /*
             * Otherwise request that we just go onto the next slot.
             */

            else
            {   
                goToNextSlot = TRUE;
                DEBUG(DAR_MSG_MAX, 
                      "<%ld> %s:mkCheckCassette: Slot empty%c\n", ' ');
            }
        }

    }/* End of else Cassette is installed. */


    /*
     *  If requested to add more tasks to the list do so here.
     */

    if ( readTheBarCode || goToNextSlot )
    {
        /*
         *  Find the place to add tasks.
         */

        if ( ( ellCount( &(pMkPriv->taskList) ) == 0 ) )
        {
            newTask = ( MK_LIST *) ellFirst( &(pMkPriv->taskList) );
        }
        else
        {
            newTask = ( MK_LIST *) ellLast( &(pMkPriv->taskList) );
        }


        /*
         * If the barcode is to be read then add tasks to remove the
         * mask, read the barcode and replace the mask again.
         */

        if ( readTheBarCode )
        {
            for ( i = 0; 
                  i < (sizeof(mkReadBarCode)/ sizeof(MK_TASK_LIST ));
                  i++ )
            {   
                MK_CHECK_NULL( newTask = malloc( sizeof ( MK_LIST ) ), 
                               MK_MALLOC_ERROR );
                newTask->item = (void *) &(mkReadBarCode[i]);
                ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node) );
            }
        }

        /*
         * Then add tasks to select the next slot, except when performing a single mask update.
         */

        if ( pMkPriv->barNodeNum < ( MK_MAX_SLOTS - 1 ) && (pMkPriv->singleUpdate != TRUE))
        {
            for ( i = 0; 
                  i < (sizeof( mkUpdateSeq)/ sizeof( MK_TASK_LIST ));
                  i++ )
            {
                MK_CHECK_NULL(newTask = malloc( sizeof ( MK_LIST ) ), 
                              MK_MALLOC_ERROR );
                newTask->item = (void *) &(mkUpdateSeq[i]);
                ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node));
            }
        }
    }

    /*
     * Increment the barcode node number so that the ID will be
     * written into the next slot.
     */
    
    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
    pMkPriv->barNodeNum++;
    semGive (pMkPriv->mutexSem);
    
    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkCheckFocalPlane
 *
 * INVOCATION:
 * status = mkCheckFocalPlane( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * To detect a mask in the focal plane during an index sequence.
 *
 * DESCRIPTION:
 * This function is called during the assembly indexing sequence after
 * the extractor has been indexed using the home switch.   
 *
 * If there is a mask in the focal plane tasks are added to replace it in the
 * cassette, which will re-index the grip and release in the process, then
 * index the cassette stage. 
 *
 * If the focal plane is empty then add tasks to index the grip and release
 * immediately then index the cassette stage.
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
 * Always returns successfully.
 *-
 ************************************************************************
 */

static long    mkCheckFocalPlane
(
    ASSEMBLY_CONTROL_RECORD 
                *par        /* (in)  Ptr to ass rec.    */
)
{
    MK_DEV_PRIVATE *pMkPriv;           /* Ptr to private dev. struct.    */
    long    status = DAR_S_SUCCESS;    /* Function return status.        */
    int        i;                      /* Generic counter.               */
    MK_LIST *newTask = NULL;           /* Generic task pointer.          */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkCheckFocalPlane: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /* 
     *  If simulating there is nothing to do so just return.
     */

    if ( assSimulateLevel(par ) != DAR_SIM_NONE )
    {
        DEBUG(DAR_MSG_MAX, 
              "<%ld> %s:mkCheckFocalPlane: simulating, return%c\n", ' ');
        return( status );
    }


    /*
     *  Find the place to start adding tasks.
     */

    if ( ( ellCount( &(pMkPriv->taskList) ) == 0 ) )
    {
        newTask = ( MK_LIST *) ellFirst( &(pMkPriv->taskList) );
    }
    else
    {
        newTask = ( MK_LIST *) ellLast( &(pMkPriv->taskList) );
    }


    /*
     *  If Mask or IFU in focal plane then put it back
     *  Check for mask and ifu as well as in_fp switch 
     *  in case the latter is bad.  See mkInFocalPlane for details.
     */

    if ( !( MK_SENSOR_WORD & MK_MON_NOTINFP ) ||
         !( MK_SENSOR_WORD & MK_MON_MASKNOTIN ) ||  
         !( MK_SENSOR_WORD & MK_MON_IFUNOTIN )  )
    {
        DEBUG(DAR_MSG_MIN, 
              "<%ld> %s:mkCheckFocalPlane:Something in focal plane%c\n",' ');
        for ( i = 0; 
              i < (sizeof( mkInFocalPlane)/ sizeof( MK_TASK_LIST ));
              i++ )
        {
            MK_CHECK_NULL( newTask = malloc( sizeof ( MK_LIST ) ), 
                           MK_MALLOC_ERROR );
            newTask->item = (void *) &(mkInFocalPlane[i]);
            ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node) );
        }
    }


    /*
     *  If the focal plane is empty then add tasks to index grip and release 
     *  immediately.  See mkOutFocalPlane definition for details.
     */

    else
    {
        DEBUG(DAR_MSG_MIN, 
              "<%ld> %s:mkCheckFocalPlane: Nothing in focal plane%c\n",' ');
        for ( i = 0; 
              i < (sizeof( mkOutFocalPlane)/ sizeof( MK_TASK_LIST ));
              i++ )
        {
            MK_CHECK_NULL( newTask = malloc( sizeof ( MK_LIST ) ), 
                           MK_MALLOC_ERROR );
            newTask->item = (void *) &(mkOutFocalPlane[i]);
            ellAdd( (ELLLIST *) &(pMkPriv->taskList), &(newTask->node) );
        }
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkClearBarList
 *
 * INVOCATION:
 * mkClearBarList( par );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * To erase the current barcode list.
 *
 * DESCRIPTION:
 * Remove all entries on the barcode list ready for a new update.
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
 * None.
 *-
 ************************************************************************
 */

static void    mkClearBarList
(
    ASSEMBLY_CONTROL_RECORD *par    /* (in)  Ptr to ass rec.    */
)
{
    MK_DEV_PRIVATE *pMkPriv;        /* Ptr to private dev. struct.    */
    int        barCount;            /* Counter into bar list.    */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkClearBarList: entry, sim=%d\n", 
            assSimulateLevel(par) );

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     * Fill the entire list with the NO_ENTRY code and erase all ID strings.   
     * Set the empty flag when done.
     */

    semTake (pMkPriv->mutexSem, WAIT_FOREVER);

    for ( barCount = 0; barCount < MK_MAX_SLOTS; barCount++ )
    {
        pMkPriv->barCodeList[barCount].barCodeNum = MK_NO_ENTRY;
        pMkPriv->barCodeList[barCount].barCode[0] = '\0';
    }

    pMkPriv->barListEmpty = TRUE;
    semGive (pMkPriv->mutexSem);

}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkCommandDevice
 *
 * INVOCATION:
 * status = mkCommandDevice (par, &deviceInfo, mode, position, velocity); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 * (>) deviceInfo (MK_DEV_INFO *)   Link definitions for the device.
 * (>) mode (unsigned short)        Mode word to be sent to device.
 * (>) position (char *)            Position string to be sent to the device.
 * (>) velocity (double)            Velocity to be sent to the device.
 *                                  Velocity link is not often used and when
 *                                  that is the case, this parameter will be set
 *                                  to zero.  Also set to zero when the device
 *                                  should not move.
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Send a command to an attached device control record.
 *
 * DESCRIPTION:
 * Send the given command to the selected device using the following
 * algorithm:
 *
 *      Recover communication link references for this device.
 *      Setup command to the device by:
 *          - Writing operating mode
 *          - Writing target position
 *          - Writing motion velocity (if required)
 *      Execute command by:
 *          - Writing the GO directive
 *          - Checking the acknowledge field to see if command accepted.
 *      If command was accepted start command timeout timer.
 *      If command was rejected abort the assembly command.
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
 * Calls assStartTimer without checking to see if the command timeout
 * timer has already been set.
 *-
 ************************************************************************
 */

static long mkCommandDevice
(
    ASSEMBLY_CONTROL_RECORD *par,       /* (in)  Ptr to ass. rec.          */
    MK_DEV_INFO        *deviceInfo,     /* (in)  Device record to command. */
    unsigned short      mode,           /* Mode value.                     */
    char *              pos,            /* Position value.                 */
    double              vel             /* Velocity value.                 */
)
{
    unsigned short  dir;                    /* Translated directive          */
    long            nRequest = 1;           /* Number of data words to send. */
    unsigned short  returnedAck;            /* Returned ack from device.     */
    long            status = DAR_S_SUCCESS; /* Function status.              */
    MK_DEV_PRIVATE  *pMkPriv;               /* Ptr to private dev. struct.   */
    struct link     *dirLink;               /* Where to write directive      */
    struct link     *modLink;               /* Where to write mode           */
    struct link     *posLink;               /* Where to write position       */
    struct link     *velLink;               /* Where to write velocity       */
    struct link     *ackLink;               /* Where to read acknowledgement */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkCommandDevice: entry%c\n", ' ' );

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );

   
    /*
     *  Translate directive into device directive and recover links.
     */

    dir = mkTranslateDir( DAR_DIR_START );
    dirLink = deviceInfo->dirLink;
    modLink = deviceInfo->modLink;
    posLink = deviceInfo->posLink;
    velLink = deviceInfo->velLink;
    ackLink = deviceInfo->ackLink;


    /*
     *  Write the mode if there is something attached to the link.
     */

    if (modLink->type != CONSTANT)
    {
        DEBUG(DAR_MSG_MAX, 
              "<%ld> %s:mkCommandDevice: Putting MOD, %d\n", mode );
        /*CHECKSTAT( (status = recGblPutLinkValue( modLink, 
                   (void *) par, DBR_SHORT, &mode, &nRequest)), 
                   return(status) );*/
         CHECKSTAT((status =(dbPutLink(modLink, DBR_SHORT,&mode,nRequest))),return(status));
    }


    /*
     *  Write the position if there is something attached to the link.
     *
     */

    if (posLink->type != CONSTANT)
    {
        /*CHECKSTAT( (status = recGblPutLinkValue( posLink, 
                   (void *) par, DBR_STRING, pos, &nRequest)), 
                   return(status) );*/
         CHECKSTAT((status =(dbPutLink(posLink, DBR_STRING,pos,nRequest))),return(status));
    }


    /*
     *  Write the Velocity if there is something attached to the link.
     */

    if (velLink->type != CONSTANT)
    {

        DEBUG(DAR_MSG_MAX, 
              "<%ld> %s:mkCommandDevice: Putting VEL, %f\n", vel );
        /*
         CHECKSTAT( (status = recGblPutLinkValue( velLink, (void *) par, 
                   DBR_DOUBLE, &vel, &nRequest)), 
                   return(status) );
         */
         CHECKSTAT((status =(dbPutLink(velLink, DBR_DOUBLE,&vel,nRequest))),return(status));
    }


    /*
     *  Write the directive.
     */

    if (dirLink->type != CONSTANT)
    {
        DEBUG(DAR_MSG_MAX, 
              "<%ld> %s:mkCommandDevice: Putting DIR, %d\n", dir );
        /*CHECKSTAT( (status = recGblPutLinkValue( dirLink, 
                  (void *) par, DBR_SHORT, (void *) (&dir), &nRequest)), 
                  return(status) );*/
          CHECKSTAT((status =(dbPutLink(dirLink, DBR_SHORT,(void *) (&dir) ,nRequest))),return(status));
    }


    /*
     *  Check the response from the device.
     */
    
    /*CHECKSTAT (status = recGblGetFastLink( ackLink, 
              (void *) par, &returnedAck), return (status));*/
     CHECKSTAT((status = dbGetLink(ackLink,DBR_USHORT,&returnedAck,0,0 )),return(status));
    DEBUG(DAR_MSG_MAX, 
          "<%ld> %s:mkCommandDevice: Got response: %d\n", returnedAck );

    /*
     * Motion started successfully, set the command active
     * flag and start the command timer
     */

    if (returnedAck == DAR_DEV_VAL_ACCEPT )
    {
        semTake (pMkPriv->mutexSem, WAIT_FOREVER);
        pMkPriv->cmdActive = TRUE;
        semGive (pMkPriv->mutexSem);
        assStartTimer( par, &status, MK_COMMAND_TIMEOUT );
        if (status != DAR_S_SUCCESS)
        {
            DEBUG(DAR_MSG_WARNING, 
                  "<%ld> %s:mkCommandDevice:failed to start watchdog timer - status:%ld\n", status);
            status = DAR_S_SUCCESS;
        }
    }


    /*
     *  Otherwise device rejected command, abort the assembly command.
     */

    else
    {
      DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkCommandDevice: %s failed\n", 
               deviceInfo->name);

        status = DAR_E_DEVICE;
        SET_ERR_MSG("Mask device rejected command");
        mkTerminateTasks (par, status);
    }
    
    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkDoTask
 *
 * INVOCATION:
 * status = mkDoTask (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Execute the given task.
 *
 * DESCRIPTION:
 * Pull the task off the top of the task list and do whatever is necessary
 * to execute it.   Each task is described in detail in the following code.
 * Some tasks request that the next task be executed immediately....
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

static long mkDoTask
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.    */
)
{
    int      moreTasks = TRUE;          /* Execute next task immediately    */
    long     nRequest = 1;              /* Number requested.                */
    long     sensor = 0;                /* Value to turn on sensor power */
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct.      */
    long     status = DAR_S_SUCCESS;    /* Return function status.          */
    MK_LIST *taskRequested = NULL;      /* Current task pointer             */
    MK_LIST *newTask = NULL;            /* Generic task pointer             */
    MK_TASK_LIST *taskItem = NULL;      /* Ptr to item in task list.        */
                                  

    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkDoTask: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  Get the task from the top of the list.  If there are no tasks
     *  then abort the command since we are totally confused.
     */

    if (ellCount( &(pMkPriv->taskList) ) == 0 || 
        (taskRequested = (MK_LIST *) ellFirst (&(pMkPriv->taskList))) == NULL)
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkDoTask: taskList empty =%d\n", 
              ellCount( &(pMkPriv->taskList) ) );
        SET_ERR_MSG("Mask assembly confused, no tasks to do");
        status = MK_LAST_TASK;
        mkTerminateTasks (par, status);
        return ( status );
    }

    taskItem = ( MK_TASK_LIST *) taskRequested->item;

    DEBUG(DAR_MSG_MAX, 
          "<%ld> %s:mkDoTask: taskList size =%d\n", 
          ellCount( &(pMkPriv->taskList) ) );
    DEBUG(DAR_MSG_MAX, 
          "<%ld> %s:mkDoTask: firsttask = %d\n", 
          taskItem->task );


    /*
     *  Clear command finished flags and stop the timer.
     */

    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
    pMkPriv->relFinished = FALSE; 
    pMkPriv->extFinished = FALSE; 
    pMkPriv->grpFinished = FALSE; 
    pMkPriv->casFinished = FALSE; 
    semGive (pMkPriv->mutexSem);
    assStopTimer( par, &status );
    if (status != DAR_S_SUCCESS)
    {
        DEBUG(DAR_MSG_WARNING, 
              "<%ld> %s:mkDoTask:failed to stop watchdog timer - status:%ld\n", status);
        status = DAR_S_SUCCESS;
    }


    /*
     *  Then do whatever is required to execute the given task.
     */
 
 
    while ( moreTasks && status == DAR_S_SUCCESS )
    {
        moreTasks = FALSE;


        /*
         * Before executing a task check to insure that it is safe to
         * execute the task at this time.   If so, do it.
         */

        if ((status = mkTaskPreCheck(par)) == DAR_S_SUCCESS) 
            switch(taskItem->task )
        {

            /*
             *  Index the locking mechanism and leave it locked.
             */

            case MK_INDEX_REL:         
                DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkDoTask:** Index locking  mechanism%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->relDevice), 
                                          DAR_MODE_INDEX, 
                                          MK_NAME_POS_INDEX,
                                          MK_VELOCITY_DISABLE);
                break;

            /*
             *  Remove the locking pin.
             */

            case MK_REL_TO_RELEASE:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Release locking pin%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->relDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_REL,
                                          MK_VELOCITY_DISABLE);
                break;

            /*
             *  Insert the locking pin.
             */

            case MK_REL_TO_LOCK:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask: Insert locking pin%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->relDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_LOCK, 
                                          MK_VELOCITY_DISABLE);
                break;

            /*
             *  Index the extractor stage on the home switch and leave it at 
             *  the index poisition.
             */

            case MK_INDEX_EXT:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Index Extractor%c\n", ' ' );
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_INDEX, 
                                          MK_NAME_POS_INDEX,
                                          pMkPriv->veloExtSlow);
                break;

            /*
             *  Move extractor at a very slow speed until the cassette
             *  end soft limit is reached.  Do not generate an error but
             *  leave the extractor there (see below for why).  The position
             *  counter will be set to zero due to the indexing mode used.
             */

            case MK_EXT_TO_I4:
                DEBUG(DAR_MSG_FULL,
                      "<%ld> %s:mkDoTask:** Pre-index ext to I4%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_I4,
                                          pMkPriv->veloExtSlow);
                break;

            /* 
             *  Offset the extractor stage from the soft limit position
             *  above to a position where the gripper can be rotated.
             *  This motion is used when replacing a mask as part of the
             *  index sequence where normal extractor indexing is not possible.
             */
         
            case MK_EXT_TO_I4_GRP:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Pre-index ext to grip%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_I4_GRP,
                                          pMkPriv->veloExtSlow);
                break;


            /* 
             *  Offset the extractor stage from the soft limit position
             *  above to a position where the lock can be inserted.
             *  This motion is used when replacing a mask as part of the
             *  index sequence where normal extractor indexing is not possible.
             */
         
            case MK_EXT_TO_I4_REL:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Pre-index ext to rel%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_I4_REL,
                                          pMkPriv->veloExtSlow);
                break;

            /*
             *  Move the extractor stage to a position where the locking pin
             *  can be moved.
             */

            case MK_EXT_TO_REL:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Move extr release%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_REL,
                                          pMkPriv->veloExtFast);
                break;


            /*
             *  Move the extractor stage to a position where the gripper
             *  can be rotated.
             */

            case MK_EXT_TO_GRP:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Move extr gripper%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_GRP,
                                          pMkPriv->veloExtFast);
                break;

            /*
             *  Move the extractor stage to a position where the mask barcode
             *  can be read.
             */

            case MK_EXT_TO_ID:
                DEBUG(DAR_MSG_FULL,
                      "<%ld> %s:mkDoTask:** Move extr ID%c\n", ' ' );
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_ID,
                                          pMkPriv->veloExtFast);
                break;

            /*
             *  Move the extractor stage to a position where it is completely
             *  clear of the cassette so that the cassette can be moved.
             */

            case MK_EXT_TO_CLEAR:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Move extr clear%c\n", ' ' );
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_CLEAR,
                                          pMkPriv->veloExtFast);
                break;

            /*
             *  Move the extractor stage to a position where it holding
             *  a mask in the beam with the correct pre-load tension.
             */

            case MK_EXT_TO_MASK:
                DEBUG(DAR_MSG_MIN, 
                      "<%ld> %s:mkDoTask:** Move extr to mask posn%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_MASK,
                                          pMkPriv->veloExtFast);
                break;

            /*
             *  Move the extractor stage to a position where it holding
             *  an IFU in the beam with the correct pre-load tension.
             */

            case MK_EXT_TO_IFU:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Move extr to ifu posn%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_IFU,
                                          pMkPriv->veloExtFast);
                break;                        

            /*
             *  Index the gripper mechanism and leave it in the mask released 
             *  position.
             */

            case MK_INDEX_GRP:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Index gripper%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->grpDevice), 
                                          DAR_MODE_INDEX, 
                                          MK_NAME_POS_INDEX,
                                          MK_VELOCITY_DISABLE);
                break;

            /*
             *  Rotate the gripper mechanism to release a mask.
             */

            case MK_GRP_TO_RELEASE:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Release gripper%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->grpDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_REL,
                                          MK_VELOCITY_DISABLE);
                break;

            /*
             *  Rotate the gripper mechanism to grip a mask.
             */

            case MK_GRP_TO_GRIP:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Engage gripper%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->grpDevice), 
                                          DAR_MODE_MOVE, 
                                          MK_NAME_POS_GRP,
                                          MK_VELOCITY_DISABLE);
                break;

            /* 
             *  Index the cassette stage and leave at the index point.
             */
         
            case MK_INDEX_CAS:
                DEBUG(DAR_MSG_FULL,
                      "<%ld> %s:mkDoTask:** Index cassette%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->casDevice), 
                                          DAR_MODE_INDEX, 
                                          MK_NAME_POS_INDEX,
                                          MK_VELOCITY_DISABLE);
                break;


            /* 
             *  Move the cassette stage to the slot number specified
             *  in the current barcode list entry.
             */
         
            case MK_CAS_TO_SLOT:
                DEBUG4(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Move cassette to C%d_S%d%c%c\n", 
                      pMkPriv->barCodeList[pMkPriv->barNodeNum].casNum,
                      pMkPriv->barCodeList[pMkPriv->barNodeNum].slotNum,' ',' ' );
                status = mkCommandDevice (par, 
                           &(pMkPriv->casDevice), 
                           DAR_MODE_MOVE, 
                           pMkPriv->barCodeList[pMkPriv->barNodeNum].namedPos,
                           MK_VELOCITY_DISABLE);
                break;

            /* 
             *  Move the cassette stage to the first slot in the fist cassette.
             */
         
            case MK_CAS_TO_C1S1:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Move cas to first slot%c\n", ' ' );
                status = mkCommandDevice (par, 
                                          &(pMkPriv->casDevice), 
                                          DAR_MODE_MOVE, 
                                          "c1s1",
                                          MK_VELOCITY_DISABLE);
                break;

            /* 
             *  Move the cassette stage to either the default park position (
             *  position = 0) or one of the three loading park positions.
             */
         
            case MK_CAS_TO_PARK:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Park cassette:%d\n", 
                      pMkPriv->currentParkPos );
                if ( pMkPriv->currentParkPos == 0 )
                {
                    status = mkCommandDevice (par, 
                                              &(pMkPriv->casDevice), 
                                              DAR_MODE_MOVE, 
                                              MK_NAME_POS_PARK,
                                              MK_VELOCITY_DISABLE);
                }
                else
                {
                    status = mkCommandDevice (par, 
                                  &(pMkPriv->casDevice), 
                                  DAR_MODE_MOVE, 
                                  mkParkPositions[pMkPriv->currentParkPos-1],
                                  MK_VELOCITY_DISABLE);
                }
                break;


            /* 
             *  Check the focal plane state as a part of the indexing sequence.
             *  This process will add more tasks to the list to complete the
             *  sequence.
             */
         
            case MK_CK_FOCAL_PLANE:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Check focal plane%c\n", ' ');

                /*
                 * Start by dropping this task from the list so that the
                 * first added task can be executed immediately.
                 */

                mkEmptyList( par, FALSE );

                /*
                 *  Then check the focal plane which will add the next
                 *  set of tasks to the list.  If the check failed for
                 *  some reason bail out here and the command will be
                 *  aborted
                 */
 
                if ( (status = mkCheckFocalPlane( par )) != DAR_S_SUCCESS )
                {
                    DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkDoTask: mkCheckFocalPlane() failed%c\n", ' ');
                    break;
                }


                /*
                 *  If no tasks left then you're done.
                 */
 
                else if (ellCount ( &(pMkPriv->taskList)) == 0 || 
                         (taskRequested = 
                          (MK_LIST *) ellFirst( &(pMkPriv->taskList))) == NULL)
                {
                    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkDoTask: last task%c\n", ' ');
                    status = MK_LAST_TASK;
                    break;
                }


                /*
                 *  There are tasks left - check that task item is valid
                 */

                else
                {
                    if ( (taskItem = 
                         (MK_TASK_LIST *) taskRequested->item) == NULL )
                    {
                        DEBUG(DAR_MSG_ERROR, 
                              "<%ld> %s:mkDoTask:task item for MK_CK_FOCAL_PLANE invalid:%d\n",
                              ellCount( &(pMkPriv->taskList) ) );
                        SET_ERR_MSG("Task invalid for MK_CK_FOCAL_PLANE");
                        status = MK_LAST_TASK;
                        mkTerminateTasks (par, status);
                    }

                    /*
                     * There are tasks on the list so set the moreTasks flag
                     * to execute the next one when we loop back.
                     */

                    else
                    {
                        moreTasks = TRUE;
                    }
                }
                break;


            /*
             * Check the state of the cassette sensors as a part of the
             * update sequence.   This will add whatever tasks are necessary
             * for the next step in the sequence.
             */

            case MK_CHECK_CASSETTE:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Check Cassette%c\n", ' ');
                /*
                 * Start by dropping this task from the list so that the
                 * first added task can be executed immediately.
                 */

                mkEmptyList( par, FALSE );


                /*
                 *  Then check the cassette which will add the next
                 *  set of tasks to the list.  If the check failed for
                 *  some reason bail out here and the command will be
                 *  aborted
                 */
 
                if ((status = mkCheckCassette (par)) != DAR_S_SUCCESS)
                {
                    DEBUG(DAR_MSG_ERROR, 
                          "<%ld> %s:mkDoTask: mkCheckCassette() failed%c\n", ' ' );
                    break;
                }


                /*
                 *  If there are no tasks left then the last of the
                 *  slots have been read and the command has finished.
                 */

                else if (ellCount( &(pMkPriv->taskList) ) == 0 || 
                        (taskRequested = 
                         (MK_LIST *) ellFirst( &(pMkPriv->taskList) )) == NULL)
                {
                    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkDoTask: last task%c\n", ' ');

                    /*
                     * Put MK_CHECK_CASSETTE back onto task list so that it 
                     * can be removed by mkTaskFinished.
       	             */
		  
                    newTask = ( MK_LIST *) ellFirst( &(pMkPriv->taskList) );
	           
                    MK_CHECK_NULL( newTask = malloc( sizeof ( MK_LIST ) ), 
				                   MK_MALLOC_ERROR );
                    newTask->item = (void *) &(mkUpdateSeq[1]);
                    ellAdd( (ELLLIST *) &(pMkPriv->taskList), 
                            &(newTask->node) );

                    DEBUG(DAR_MSG_MAX, 
                          "<%ld> %s:mkDoTask: replace check cassette%c\n", ' ');

                    /* 
                     * Call mkTaskFinished here because there won't be another
                     * busy/idle transition to cause it to be called in the 
                     * usual way by mkAckReceived.
                     */

                    status =  mkTaskFinished( par );            
                }


                /*
                 *  There are tasks left - check that task item is valid
                 */

                else
                {
                    if ( (taskItem = 
                         ( MK_TASK_LIST *) taskRequested->item) == NULL )
                    {
                        DEBUG(DAR_MSG_ERROR, 
                              "<%ld> %s:mkDoTask: task item for MK_CHECK_CASSETTE invalid:%d\n", 
                              ellCount( &(pMkPriv->taskList) ) );
                        SET_ERR_MSG("Task invalid for MK_CHECK_CASSETTE");
                        status = MK_LAST_TASK;
                        mkTerminateTasks (par, status);
                    }

                    /*
                     * There are tasks on the list so set the moreTasks flag
                     * to execute the next one when we loop back.
                     */

                    else
                    {
                        moreTasks = TRUE;
                    }
                }

                break;
         

            /* 
             *  Trigger the barcode reader by writing to the
             *  appropriate sensor output link.
             */
         
            case MK_ASKFOR_BARCODE_ID:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Ask for barcode id.%c\n", ' ');

                /*
                 *  Write to the link and check the status.
                 *  If the write fails set status to abort the command.
                 */

                  if (
                     (dbPutLink(&(MK_BARCODE_TRIGGER), DBR_LONG,&nRequest,nRequest))
                    !=DAR_S_SUCCESS
		     )
                {
                    status = MK_TRIG_FAILED;
                    DEBUG(DAR_MSG_ERROR, 
                          "<%ld> %s:mkDoTask:Barcode trigger failed%c\n",' ');
                    SET_ERR_MSG("Mask barcode reader trigger failed");
                    break;
                }

                /*
                 *  Clear out the previous string and set the command
                 *  active and ask barcode flags.   Start the read
                 *  delay timer which will re-preocess the record to
                 *  analyze the input string.
                 */
        
                strcpy( MK_BARCODE_READ, "");
                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->cmdActive = TRUE;
                pMkPriv->askBarCode = TRUE;
                semGive (pMkPriv->mutexSem);
                assStartTimer( par, &status, MK_BARCODE_READ_DELAY );
                if (status != DAR_S_SUCCESS)
                {
                    DEBUG(DAR_MSG_WARNING, 
                          "<%ld> %s:mkDoTask:MK_ASK_FOR_BARCODE_ID: failed to start watchdog timer - status:%ld\n", status);
                    status = DAR_S_SUCCESS;
                }

                break;


            /*
             *  Initialize all of the attached device control records
             *  in parallel since nothing moves it is safe to do this.
             *  Initialize device velocities to zero
             */

            case MK_INIT_ALL:
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Initialize all devices%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_INIT, 
                                          "",
                                          MK_VELOCITY_DISABLE);

                if (!status) status = mkCommandDevice (par, 
                                                       &(pMkPriv->casDevice), 
                                                       DAR_MODE_INIT, 
                                                       "",
                                                       MK_VELOCITY_DISABLE);

                if (!status) status = mkCommandDevice (par, 
                                                       &(pMkPriv->grpDevice), 
                                                       DAR_MODE_INIT, 
                                                       "",
                                                       MK_VELOCITY_DISABLE);

                if (!status) status = mkCommandDevice (par, 
                                                       &(pMkPriv->relDevice), 
                                                       DAR_MODE_INIT, 
                                                       "",
                                                       MK_VELOCITY_DISABLE);

                break;


            /*
             *  Test all of the attached device control records
             *  in parallel since nothing moves it is safe to do this.
             *  Set device velocities to zero for the TEST mode.
             */

            case MK_TEST_ALL:         
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkDoTask:** Test all devices%c\n", ' ');
                status = mkCommandDevice (par, 
                                          &(pMkPriv->extDevice), 
                                          DAR_MODE_TEST, 
                                          "",
                                          MK_VELOCITY_DISABLE);

                if (!status) status = mkCommandDevice (par, 
                                                       &(pMkPriv->casDevice), 
                                                       DAR_MODE_TEST, 
                                                       "",
                                                       MK_VELOCITY_DISABLE);

                if (!status) status = mkCommandDevice (par, 
                                                       &(pMkPriv->grpDevice), 
                                                       DAR_MODE_TEST, 
                                                       "",
                                                       MK_VELOCITY_DISABLE);

                if (!status) status = mkCommandDevice (par, 
                                                       &(pMkPriv->relDevice), 
                                                       DAR_MODE_TEST, 
                                                       "",
                                                       MK_VELOCITY_DISABLE);

                break;

            /* 
             *  Trigger the barcode reader by writing to the
             *  appropriate sensor output link.
             */
         
            case MK_TURN_ON_OPTO:
                DEBUG(DAR_MSG_MAX, 
                 "<%ld> %s:mkDoTask:** Turn on opto sensors if necessary.%c\n",' ');

                /*
                 *  Set flags regardless of whether opto sensors are
                 *  turned on or not to ensure this task can finish
                 */

                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->cmdActive = TRUE;
                pMkPriv->optoDelayOn = TRUE;
                semGive (pMkPriv->mutexSem);

                if ( status == DAR_S_SUCCESS && 
                       par->dir == DAR_DIR_START &&
                       par->mode != DAR_MODE_INIT )
                {
                    /*
                     *  Write to the link and check the status.
                     *  If the write fails set status to abort the command.
                     */

                    DEBUG(DAR_MSG_FULL, 
                         "<%ld> %s:mkDoTask:** turning opto sensors ON %c\n", ' ' );

                    /*if ( recGblPutLinkValue (&(MK_SENSOR_POWER), 
                           (void *) par, DBR_LONG, &sensor, &nRequest) != 
			   DAR_S_SUCCESS )*/
		    /* AWE: if simulating, the link to optsensorEnable is invalid (no hardware) resulting in an error here .. working around it */

		    if ((assSimulateLevel(par) == DAR_SIM_NONE) && dbPutLink(&(MK_SENSOR_POWER),DBR_LONG,&sensor,nRequest) != DAR_S_SUCCESS)
		      {
                        status = MK_NO_POWER;
                        DEBUG(DAR_MSG_ERROR, 
                         "<%ld> %s:mkDoTask:** Sensor power control failed%c\n",' ');
                        SET_ERR_MSG("Could not power up mask sensors");
                        break;
                    }

                    /*
                     *  Delay here to allow sensors to power up before reading them.
                     */

                    DEBUG(DAR_MSG_MAX, 
                             "<%ld> %s:mkDoTask:** opto sensors delay=%d secs\n", 
                             MK_SENSOR_DELAY );

                    assStartTimer( par, &status, MK_SENSOR_DELAY );
                    if (status != DAR_S_SUCCESS)
                    {
                        DEBUG(DAR_MSG_WARNING, 
                            "<%ld> %s:mkDoTask:MK_TURN_ON_OPTO:ON failed to start watchdog timer - status:%ld\n", status);
                        status = DAR_S_SUCCESS;
                    }
                }
                else 
                {

                    /*
                     *  Start timer but with no delay.
                     */

                    DEBUG(DAR_MSG_FULL, 
                       "<%ld> %s:mkDoTask:** opto sensors not required %c\n", ' ' );
                    assStartTimer( par, &status, 0 );
                    if (status != DAR_S_SUCCESS)
                    {
                        DEBUG(DAR_MSG_WARNING, 
                            "<%ld> %s:mkDoTask:MK_TURN_ON_OPTO:OFF failed to start watchdog timer - status:%ld\n", status);
                        status = DAR_S_SUCCESS;
                    }
                }

                break;

            /*
             *  Any unsupported task requests are trapped here....
             */    

            default:
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkDoTask:** Invalid task=%d\n", 
                      taskItem->task );
                status = MK_INVALID_TASK;
                SET_ERR_MSG("Mask assembly confused, invalid task");

        } /* Switch on task. */
    } /* while moreTasks */

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkEmptyList
 *
 * INVOCATION:
 * status = mkEmptyList (par, removeAll); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 * (>) removeAll  (bool) Remove all tasks from list (TRUE), or only 1 (FALSE).
 *
 * FUNCTION VALUE:
 * (long) return function status.
 *
 * PURPOSE:
 * Empty the task list
 *
 * DESCRIPTION:
 * Erase either all tasks (removeAll = TRUE) on the task list or just
 * the last task (removeAll = FALSE);   Return MK_EMPTY_LIST if the
 * list is empty after removing the task(s).
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
 * Does not return an error message if the task list has not been
 * emptied.  Only sends a MAX debug message!
 *-
 ************************************************************************
 */

static long mkEmptyList
(
    ASSEMBLY_CONTROL_RECORD *par,       /* (in)  Ptr to ass. rec.     */
    int            removeAll            /* (in)  Delete all tasks?    */
)
{
    long     status = DAR_S_SUCCESS;    /* Return function status.    */
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct.*/
    MK_LIST    *pList;                  /* Ptr to item in a list.     */

 
    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkEmptyList: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  If the task list is already empty then there is nothing to do.
     */

    if ( ellCount( &(pMkPriv->taskList) ) == 0 )
    {
        DEBUG(DAR_MSG_MAX, 
              "<%ld> %s:mkEmptyList: taskList is empty%c\n",' ' );
        status = MK_EMPTY_LIST;
        return ( status );
    }


    /*
     *  If the removeAll flag is FALSE then just remove the next task
     *  on the list.
     */

    if ( !removeAll )
    {
        pList = (MK_LIST *) ellGet( &(pMkPriv->taskList) ); 
        pList->item = NULL;
        free( pList );
    }


    /*
     *  Otherwise keep removing tasks from the list until there are none left.
     */

    else
    {
    while ( 1 )
    {
        if (( pList = (MK_LIST *) ellGet( &pMkPriv->taskList )) == NULL)
        {
            break;
        }
        pList->item = NULL;
        free( pList );
    }

    if ( ellCount( &(pMkPriv->taskList) ) != 0 )
    {
        DEBUG(DAR_MSG_MAX, 
            "<%ld> %s:mkEmptyList: did not empty=%d\n",
            ellCount( &(pMkPriv->taskList) ) );
    }
    }

    if ( ellCount( &(pMkPriv->taskList) ) == 0 )
    {
        status = MK_EMPTY_LIST;
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkIndexMode
 *
 * INVOCATION:
 * status = mkIndexMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Re-index all assembly devices
 *
 * DESCRIPTION:
 *    Build the task list based on the current position of the extractor
 *    by calling mkBuildList()
 *    Start the task list by calling mkDoTask()
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

static long mkIndexMode
(
    ASSEMBLY_CONTROL_RECORD *par    /* (in)  Ptr to ass. rec.     */
)
{
    MK_DEV_PRIVATE *pMkPriv;        /* Ptr to private dev. struct. */
    long status = DAR_S_SUCCESS;    /* Function return status      */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkIndexMode: entry%c\n", ' ');
 
    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  Save the current mode and then build an indexing task list based on
     *  the current position of the extractor.
     */

    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
    pMkPriv->currentCmd = par->mode;
    semGive (pMkPriv->mutexSem);

    if ( ( status = mkBuildList( par, DAR_MODE_INDEX ) ) != DAR_S_SUCCESS )
    {

        /*
         * An error here means that it was not possible to build an
         * automatic indexing sequence with the assembly in its current
         * state and human intervention is required. Abort the command.
         */

        DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkIndexMode: mkbuildList failed%c\n", ' ');
        mkTerminateTasks (par, status);
    }


    /*
     * The task list is ready, start the sequence by executing the
     * first task.  Abort the command if this task fails to execute.
     */

    else if ( ( status = mkDoTask( par ))  != DAR_S_SUCCESS )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkIndexMode: mkDoTask failed%c\n", ' ');
        mkTerminateTasks (par, status);
    }
    
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkInitDeviceSupport
 *
 * INVOCATION:
 * status = mkInitDeviceSupport (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Initialize device support-specific functions
 *
 * DESCRIPTION:
 *    Create private device support structure for this record.
 *    Link this structure to the record support private structure.
 *          (call assAttachPrivateStruct()).
 *    Create the MUTEX semaphore to protect the private structure
 *          during asynchronous callback access.
 *    Initialize private record variables.
 *    Check that all of the 4 deviceControl record POS[1-4] links
 *          are not CONSTANT.
 *    Create device info structure for all 4 motors, relying on 
 *          motor1=ext, motor2=grp, motor3=cas, motor4=rel.
 *    Initialize the task and barcode lists.
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
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long mkInitDeviceSupport
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.      */
)
{
    MK_DEV_PRIVATE *pMkPriv;            /* Private dev. struct.        */
    long     status = DAR_S_SUCCESS;    /* Returned function status.   */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkInitDeviceSupport: entry%c\n", ' ');


    /*
     *  Create an internal control (private) structure for this record.
     */

    pMkPriv = malloc (sizeof(MK_DEV_PRIVATE));
    if ( pMkPriv == NULL ) 
    {
        DEBUG(DAR_MSG_FATAL,
              "<%ld> %s:mkInitDeviceSupport: malloc failed.%c\n", ' ');
        status = MK_MALLOC_ERROR;
        recGblRecordError (status, par, __FILE__ ":no room for device private");
        return( status );
    }


    /*
     *  Save the pointer to the internal control structure in the
     *  assembly control record's private structure field.
     */
     
    assAttachPrivateStruct( par, (void *) pMkPriv ); 


    /*
     * Create a mutual exclusion semaphore to protect the private structure
     * during asynchronous callback access.
     */
     
    pMkPriv->mutexSem = NULL;
    if (  (pMkPriv->mutexSem = 
        semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE) ) == NULL )
    {
        DEBUG(DAR_MSG_FATAL, 
              "<%ld> %s:mkInitDeviceSupport: mutex failed.%c\n", ' ' );
        status = MK_MALLOC_ERROR;
        recGblRecordError (status, par, __FILE__ ":no room for mutex creation");
        return( status );
    }


    /*
     * Initialize critical control structure elements.
     */  

    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
     
    pMkPriv->pRecord = par;                /* Back link to record struct */
    pMkPriv->cmdActive = FALSE;            /* Clear device command flag  */
    pMkPriv->relFinished = FALSE;          /* Release is idle            */
    pMkPriv->extFinished = FALSE;          /* Extractor is idle          */ 
    pMkPriv->grpFinished = FALSE;          /* Grip is idle               */ 
    pMkPriv->casFinished = FALSE;          /* Cassette is idle           */ 
    pMkPriv->stopRequested = FALSE;        /* Clear stop command flag    */
    pMkPriv->casParked = FALSE;            /* Clear cassette parked flag */
    pMkPriv->status = DAR_S_SUCCESS;       /* Preset status to success   */
    pMkPriv->velocity = 0.0;               /* Preset velocity to 0       */
    pMkPriv->mode = MK_NO_ENTRY;           /* Clear operating mode       */
    pMkPriv->busyMode = MK_NO_ENTRY;       /* Clear busy mode            */
    pMkPriv->barNodeNum = 0;               /* Reset barcode index        */
    pMkPriv->askBarCode = FALSE;           /* Clear barcode request flag */
    pMkPriv->optoDelayOn = FALSE;          /* Clear opto pwr delay flag  */
    pMkPriv->currentCode = MK_NO_ENTRY;    /* Clear current barcode      */
    pMkPriv->currentCmd = MK_NO_ENTRY;     /* Clear current command      */
    pMkPriv->currentLoc = MK_NO_ENTRY;     /* Clear requested location   */
    pMkPriv->currentBC = MK_NO_ENTRY;      /* Clear requested mask code  */
    pMkPriv->barCodeList = mkBarList;      /* Save the barcode list ptr  */
    pMkPriv->barListEmpty = TRUE;          /* Set the list empty flag    */
    pMkPriv->quickUpdate = FALSE;          /* Configure for normal UPDATE*/
    pMkPriv->singleUpdate = FALSE;         /* Configure for normal UPDATE*/
    pMkPriv->veloExtFast = 35.0;           /* Normal Extractor Velocity  */
    pMkPriv->veloExtSlow = 15.0;           /* Index Extractor Velocity   */


    /*
     *  Confirm that all four device control records have been attached.
     */

    if ( par->pos1.type == CONSTANT || par->pos2.type == CONSTANT || 
        par->pos3.type == CONSTANT || par->pos4.type == CONSTANT )
    {
        DEBUG(DAR_MSG_FATAL, 
              "<%ld> %s:mkInitDeviceSupport:One or more devices not connected.%c\n",' ');
        status = MK_BAD_DEV_CONNECT;
        recGblRecordError (status, par, ":Not all deviceControl rec's connected");
        semGive (pMkPriv->mutexSem);
        return( status );
    }

    /*
     *  Confirm that the barcode reader is connected.
     */

    if ( par->sot.type == CONSTANT )
    {
        DEBUG(DAR_MSG_FATAL, 
              "<%ld> %s:mkInitDeviceSupport: Barcode reader not connected.%c\n",' ' );
        status = MK_BAD_DEV_CONNECT;
        recGblRecordError (status, par, ":Barcode link not connected");
        semGive (pMkPriv->mutexSem);
        return( status );
    }


    /*
     * Initialize the device control record link structure.
     */

    pMkPriv->extDevice.name    = "Extractor";
    pMkPriv->extDevice.dirLink = &par->odr1;
    pMkPriv->extDevice.posLink = &par->pos1;
    pMkPriv->extDevice.ackLink = &par->ack1;
    pMkPriv->extDevice.velLink = &par->vel1;
    pMkPriv->extDevice.modLink = &par->mod1;

    pMkPriv->grpDevice.name    = "Grip";
    pMkPriv->grpDevice.dirLink = &par->odr2;
    pMkPriv->grpDevice.posLink = &par->pos2;
    pMkPriv->grpDevice.ackLink = &par->ack2;
    pMkPriv->grpDevice.velLink = &par->vel2;
    pMkPriv->grpDevice.modLink = &par->mod2;

    pMkPriv->casDevice.name    = "Cassette";
    pMkPriv->casDevice.dirLink = &par->odr3;
    pMkPriv->casDevice.posLink = &par->pos3;
    pMkPriv->casDevice.ackLink = &par->ack3;
    pMkPriv->casDevice.velLink = &par->vel3;
    pMkPriv->casDevice.modLink = &par->mod3;

    pMkPriv->relDevice.name    = "Release";
    pMkPriv->relDevice.dirLink = &par->odr4;
    pMkPriv->relDevice.posLink = &par->pos4;
    pMkPriv->relDevice.ackLink = &par->ack4;
    pMkPriv->relDevice.velLink = &par->vel4;
    pMkPriv->relDevice.modLink = &par->mod4;

    semGive (pMkPriv->mutexSem);


    /* 
     *  Initialize the task and barcode list.  Cannot read barcode list
     *  file here because must initialize all records to determine
     *  lockset.
     */

    ellInit( &pMkPriv->taskList );
    mkClearBarList( par );
    errorMessage[0] = '\0';


    /*
     *  Set the current mask position and ID to unknown
     */                     

    MK_MASK_IN_BEAM = MK_NO_ENTRY;
    MONITOR_VALA;
    MK_EXT_MASK_POS = MK_MASK_LOC_UNKNOWN;
    MONITOR_VALB;


    /*
     *  Ensure that a full UPDATE is the default.
     */

    MK_QUICK_UPDATE = FALSE;
    db_post_events(par, par->sim, DBE_VALUE);


    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkInitDeviceSupport: finished,status=%ld\n",  
        status );

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkInitMode
 *
 * INVOCATION:
 * status = mkInitMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Initialize all fo the attached device control records.
 *
 * DESCRIPTION:
 * Recover the barcode list from the file given in the assembly record
 * attributes.  Add tasks to the task list to send the INIT command to all 
 * of the attached devce control records then start the sequence by executing
 * the first task.
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

static long mkInitMode
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.      */
)
{
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct.  */
    long     status = DAR_S_SUCCESS;    /* Return function status.      */


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkInitMode: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );

    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
    pMkPriv->currentCmd = par->mode;
    semGive (pMkPriv->mutexSem);


    /*
     *  Task list must be empty before starting command.
     */

    if ( ellCount( &(pMkPriv->taskList) ) != 0 )
    {
        DEBUG(DAR_MSG_MAX, "<%ld> %s:mkInitMode: empty taskList =%d\n", 
              ellCount( &(pMkPriv->taskList) ) );
        mkEmptyList( par, TRUE );
    }


    /*
     *  Re-read the barcode list.
     */

    if ( (status = mkReadBarList( par ) ) != DAR_S_SUCCESS )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkInitMode: failed to read lookup table%c\n", ' ' );
        mkTerminateTasks (par, status);
        return( status );
    }


    /*
     * Then build a task list to initialize all of the devices based
     * on the current state of the assembly.
     */   

    if ( (status = mkBuildList( par, DAR_MODE_INIT )) != DAR_S_SUCCESS )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkInitMode: mkbuildList failed%c\n", ' ');
        mkTerminateTasks (par, status);
    }


    /*
     * If a list was successfully built then start the sequence by executing
     * the first task on the list.   If this task fails to execute then
     * abort the command immediately.
     */                              

    else if ( ( status = mkDoTask( par )) != DAR_S_SUCCESS )
    {
        mkTerminateTasks (par, status);
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkInitMode: mkDoTask failed%c\n", ' ');
    }
    
    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkMoveConfig
 *
 * INVOCATION:
 * status = mkMoveConfig (par, mode, &numConfig); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 * (>) mode  (int) Move or Update mode.
 * (>) numConfig  (int *) Configuration number.
 *
 * FUNCTION VALUE:
 * (long) return function value.
 *
 * PURPOSE:
 * Decide what needs to be done based on the current command and 
 * assembly configuration.
 *
 * DESCRIPTION: 
 * Look at the current mask request and the state of the focal plane
 * and generate a code to indicate what needs to be done to execute
 * the given command.   The codes are generated using the following
 * algorithm:
 *
 *      If in simulation mode look at the requested mask location:
 *          return MK_NEW_FP_IN if the request is to put a mask in.
 *          return MK_MASK_OUT_FP if the request is to remove the mask.
 *
 *      If there is a mask in the focal plane:
 *          If it matches the requested in-beam mask return MK_NOTHING
 *          If a different mask has been requested return MK_WRONG_MASK_IN_FP
 *          If the request is to remove the mask return MK_MASK_OUT_FP
 *
 *      If there is no mask in the focal plane:
 *          If a mask has been requested return MK_NEW_FP_IN
 *          Otherwise return MK_NOTHING
 *
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

static long mkMoveConfig
(
    ASSEMBLY_CONTROL_RECORD *par,   /* (in)  Ptr to ass. rec.         */
    int            mode,            /* (in)  Current mode request.    */
    int            *numConfig       /* (out) Configuration number.    */
)
{
    MK_DEV_PRIVATE  *pMkPriv;       /* Ptr to private dev. struct.    */
    long status = DAR_S_SUCCESS;    /* Return function status.        */


    DEBUG(DAR_MSG_MIN , "<%ld> %s:mkMoveConfig: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );

    *numConfig = 0;


    /*
     * In simulation mode return the code to inset or remove mask
     * depending on the requested position of the mask.
     */

    if ( assSimulateLevel(par ) != DAR_SIM_NONE  )
    {
        if ( mode == DAR_MODE_MOVE && pMkPriv->currentLoc == MK_CMD_INBEAM )
        {
            *numConfig = MK_NEW_FP_IN;
        }
        else
        {
            *numConfig = MK_MASK_OUT_FP;
        }
        return( status );
    }


    /*
     *  First check to see if there is a mask or IFU in the focal plane.
     */

    if ( !( MK_SENSOR_WORD & MK_MON_NOTINFP ) )
    {
        DEBUG(DAR_MSG_MIN, 
              "<%ld> %s:mkMoveConfig: mask or IFU is in FP%c\n", ' ');

        /*
         * Yes there is, is the current request to put a mask into the beam
         */

        if ( mode == DAR_MODE_MOVE && pMkPriv->currentLoc == MK_CMD_INBEAM )
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkMoveConfig: mode MOVE with target IN%c\n",' ');

            /*
             *  Yes, but if the codes match then it is already there.
             */

            if ( pMkPriv->currentCode != MK_NO_ENTRY &&
                 MK_BARCODE_ID_IN == pMkPriv->currentCode )
            {
                DEBUG(DAR_MSG_FULL, 
                "<%ld> %s:mkMoveConfig: correct mask already there%c\n", ' ');
                *numConfig = MK_NOTHING;
            }

            /* 
             *  If the codes don't match then request a new mask be inserted.
             */

            else
            {
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkMoveConfig: wrong mask in%c\n",' ');
                *numConfig = MK_WRONG_MASK_IN_FP;
            }
        } 

         /*
          * Request is to clear the FP so just remove current mask.
          */

        else
        {
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkMoveConfig: mask is in, want it out%c\n", ' ');
            *numConfig = MK_MASK_OUT_FP;
        }
    } 

    /*
     *  If there is no mask in the focal plane do we need to put one in?
     */

    else
    {
        DEBUG(DAR_MSG_MIN, 
              "<%ld> %s:mkMoveConfig: No mask in FP%c\n", ' ');

        /*
         *  A mask has been requested so ask that it be put into the fp.
         */

        if ( mode == DAR_MODE_MOVE && pMkPriv->currentLoc == MK_CMD_INBEAM )
        {
            DEBUG(DAR_MSG_MIN, 
                  "<%ld> %s:mkMoveConfig: no mask in, one is needed%c\n", ' ');
            *numConfig = MK_NEW_FP_IN;
        }

        /*
         *  No mask needed so there is nothing more to do.
         */

        else
        {
            DEBUG(DAR_MSG_MIN, 
                  "<%ld> %s:mkMoveConfig: No mask in, none needed%c\n", ' ');
                 *numConfig = MK_NOTHING;
        }
    }

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkMoveMode
 *
 * INVOCATION:
 * status = mkMoveMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Insert/remove a mask or park the assembly.
 *
 * DESCRIPTION:
 * Save the input arguments in the private control structure then 
 * try to build a task list to execute the given command.   If a list
 * can not be built because the machine is in a weird state then abort
 * the command otherwise start the sequence by executing the first task.
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
 * None.
 *-
 ************************************************************************
 */

static long mkMoveMode
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.       */
)
{
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct.  */
    long     status = DAR_S_SUCCESS;    /* Return function status.      */


    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkMoveMode: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  Save the input arguments in the private control structure
     *  for future reference.
     */

    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
    pMkPriv->currentCmd = par->mode;
    pMkPriv->currentLoc = MK_MASK_IFU_LOC;
    pMkPriv->currentParkPos = MK_PARK_POSITION_IN;
    pMkPriv->currentBC = MK_BARCODE_ID_IN;
   
    /*  If outbeam  requested when a mask is inbeam 
        or update requested, return the ignore
        barcode failures flag to false */
   /*  TEST removing for tests */
   /*     if ( pMkPriv->currentCmd == DAR_MODE_MOVE && 
              pMkPriv->currentLoc == MK_CMD_INBEAM )
        {
		MK_IGNORE_BARCODE = FALSE;
                 printf("TEST DEBUG: MK_IGNORE_BARCODE set to FALSE due to an out-of-beam, update request ");
        }
   */ 
    /* 
     *  If an out-of-beam request or update is requested, then clear
     *  the current command code.
     */

    if ( pMkPriv->currentCmd == DAR_MODE_UPDATE ||
        ( pMkPriv->currentCmd == DAR_MODE_MOVE && 
        pMkPriv->currentLoc == MK_CMD_OUTBEAM ) )
    {
        pMkPriv->currentCmd = MK_NO_ENTRY;
        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:mkMoveMode: clearing currentCmd%c\n", ' ');
    }

    semGive (pMkPriv->mutexSem);


    /*
     *  Build a command list based on the current state of the
     *  assembly and the type of move requested.
     */

    if ( par->mode == DAR_MODE_MOVE || par->mode == DAR_MODE_PARK )
    {
          /*TEST printf("TEST DEBUG: MK_IGNORE_BARCODE set to FALSE due to an out-of-beam  or update request");*/
        if ( (status = mkBuildList( par, par->mode )) != DAR_S_SUCCESS )
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkMoveMode: mkbuildList failed%c\n", ' ');
        }
    }
    else
    {
        status = MK_MOVE_CONFIG_ERR;
        SET_ERR_MSG("Invalid Mask move mode received");
        DEBUG(DAR_MSG_ERROR,
              "<%ld> %s:mkMoveMode: invalid move mode%c\n", ' ');
    }

    /*
     *  A bad status at this point means that we can not automatically
     *  generate a sequence to execute the command.  Abort the
     *  command immediately.  It will rarely reach this code -
     *  only a failure in mkBuildList() or an invalid mode.
     */

    if (status)
    {
        mkTerminateTasks (par, status);
    }


    /*
     *  We know how to do this and have created a task list to execute
     *  the command.   Start the sequence by executing the first task
     *  on the list.  It should never reach this code.
     */

    else 
    {
        status = mkDoTask (par);
        if (status)
        {   
            DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkMoveMode: mkDoTask failed%c\n", ' ');
            mkTerminateTasks (par, status);
        }
    }

    return status;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkProcessFault
 *
 * INVOCATION:
 * status = mkProcessFault (par); 
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
 * If a command is in progress abort it immedaitely by calling terminateTasks.
 * If the record is idle then there is nothing for device support to do so
 * simply return.
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

static long mkProcessFault
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.      */
)
{
    long     status = DAR_S_SUCCESS;    /* Return function status.     */

    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkProcessFault: entry%c\n", ' ');


    /*
     * If there is a command in progress kill it immediately
     */

    if (par->busy == DAR_BUSY_BUSY)
    {
        SET_ERR_MSG ( "External interlock detected!" );
        status = DAR_E_FLT_CLR;
        mkTerminateTasks (par, status);
    } 

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkReadBarList
 *
 * INVOCATION:
 * status = mkReadBarList (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Load the barcode list with the last configuration information.
 *
 * DESCRIPTION:
 * Read in the barcode list.
 * Format of the file is:  CASSETTE# SLOT# BARCODE#
 * 
 * Try to open the file with the given name in the given directory.
 *
 * If the file does not exist it is not considered to be an error although
 * the list will remain empty.
 *
 * Do a sanity check on the data read from the file and if it looks good
 * then load it into the barcode list and clear then empty flag to say
 * that we now have valid barcode information.
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
 * 
 *-
 ************************************************************************
 */

static long mkReadBarList
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.    */
)
{
    long     status = DAR_S_SUCCESS;    /* Returned function status.   */
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct. */
    char     scratchBuf[256];           /* Scratchpad character buffer.*/
    int        casNum;                  /* Cassette number.            */
    int        slotNum;                 /* Slot number.                */
    long       nRequest = 1;              /* Number requested.                */
    long       barCode;                 /* Barcode.                    */
    int        barCount;                /* Barcode counter (0-26)      */
    int         i;                      /* Generic counter.            */
    FILE       *fp;                     /* Translation file pointer.   */


    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkReadBarList: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     * Read and load the Mask Barcode configuration table if
     * both a file name and directory have been given.
     */
     
    if (strlen(par->tfil) && strlen( par->tdir ) )
    {   
        /*
         * Open the translation file.  If we cannot open it, just assume
         * it hasn't been written yet.  This is NOT an error.  The file
         * should exist after the first successful UPDATE.
         */
         
        sprintf (scratchBuf, "%s/%s", par->tdir, par->tfil);
        if (( fp = fopen( scratchBuf, "r")) == NULL )
        {
            if ( (fp = fopen(scratchBuf, "w")) == NULL )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkReadBarList: File: %s does not exist and cannot be created\n",
                      scratchBuf);
                SET_ERR_MSG("Cannot open mask barcode file");
                status = MK_FILE_OPEN_ERROR;
                return ( status );
            }
            else
            {
                DEBUG(DAR_MSG_WARNING, 
                      "<%ld> %s:mkReadBarList: Warning! %s did not exist - created file stub\n",
                      scratchBuf);
            }
        }

        /*
         * The file exists and is open - clear the existing internal barcode list.
         */
                          
        mkClearBarList( par );
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkReadBarList: Old list cleared.%c\n", ' ');

        /*
         * Check the lookup table one line at a time
         */

        while ( fgets( scratchBuf, sizeof( scratchBuf ), fp ) != NULL )
        {
            DEBUG(DAR_MSG_MAX, "<%ld> %s:mkReadBarList:got a line: %s\n", 
            scratchBuf ); 

            /*
             * Discard lines with comments and carriage returns.
             */

            if (*scratchBuf == '#' || *scratchBuf == '\n' )
            {  
                 continue;
            }

            /*
             * Then read the data .. expect three fields (cassette number,
             * slot number and barcode ID) separated by white spaces.
             */
 
            else        
            {
                if (sscanf( scratchBuf, "%d %d %ld", &casNum, &slotNum, 
                    &barCode) != 3 )
                {
                    DEBUG(DAR_MSG_ERROR, 
                          "<%ld> %s:mkReadBarList: Bad line in mask lookup table:%s\n", 
                          scratchBuf );
                    SET_ERR_MSG("Mask barcode file corrupted, use UPDATE");
                    status = MK_FILE_FORMAT_ERROR;
                    break;
                }

                /*
                 * Sanity check ... the cassette and slot numbers must
                 * be within the physical configuration of the instrument.
                 */

                else if ( casNum < 1 || casNum > MK_NUM_CASSETTES ||
                          slotNum < 1 || slotNum > MK_NUM_SLOTS )
                {
                    DEBUG(DAR_MSG_ERROR, 
                          "<%ld> %s:mkReadBarList: Slot or cassette number out of range in LUT line:%s\n",
                          scratchBuf);
                    SET_ERR_MSG("Mask barcode file corrupted, use UPDATE");
                    status = MK_FILE_FORMAT_ERROR;
                    break;
                }

                /*
                 *  Looks okay so far.
                 */

                else
                {
		    /*
		     * If this is the IFU (cas 1, slot 7) make the IFU barcode available on sov
		     */
		    if ((casNum == 1) && (slotNum == 7))
		      {
			dbPutLink(&(MK_IFU_BARCODE),DBR_LONG,&barCode,nRequest);
		      }
		    
                    barCount = (casNum - 1) * MK_NUM_SLOTS + (slotNum - 1);

                    /*
                     *  Scan the masks read thus far and check for duplicates.
                     *  Skip the check if it was flagged as unidentifiable.
                     */

                    if (barCount > 0 && barCode != MK_NO_BARCODE)
                    {
                        for (i=0; i < barCount; i++)
                        {
                            if (pMkPriv->barCodeList[i].barCodeNum == barCode)
                            {
                                DEBUG4(DAR_MSG_ERROR,
                                      "<%ld> %s:mkReadBarList: Found duplicate mask ID for C%d_S%d ID:%ld%c\n",
                                      casNum, slotNum, barCode, ' ' );
                                SET_ERR_MSG("Duplicate mask ID in lookup table");
                                status = MK_FILE_FORMAT_ERROR;
                                break;
                            }
                        }
                    }

                    if (status == DAR_S_SUCCESS)
                    {   
                        /*  
                         *  Looks good, so save this barcode entry
                         *  and set the empty flag to false. 
                         */
                        semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                        pMkPriv->barCodeList[barCount].barCodeNum = barCode;
                        pMkPriv->barListEmpty = FALSE;
                        semGive (pMkPriv->mutexSem);;
                        DEBUG4(DAR_MSG_FULL,
       	                      "<%ld> %s:mkReadBarList: Creating entry for mask in C%d_S%d ID:%ld%c\n", 
                              casNum, slotNum, barCode, ' ' );
                    }
                    else 
                    { 
                        break;
                    }
                }
            }                  
        }/* End of while reading the buffer. */
        
        fclose (fp);


        /*
         * Clear the list if the read failed after partially writing it, 
         * Otherwise indicate that we have valid barcode information.
         */

        if ( status != DAR_S_SUCCESS )
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkReadBarList: Failed to read barcode list:%c\n",' ');
            if ( pMkPriv->barListEmpty == FALSE )
            {
                mkClearBarList( par );
                DEBUG(DAR_MSG_MIN, "<%ld> %s:mkReadBarList: Partial list cleared.%c\n", ' ');
            }
        }
        else
        {
            DEBUG4(DAR_MSG_MIN, 
                  "<%ld> %s:mkReadBarList: Saved contents of %s/%s to internal barcode list%c%c\n",
                  par->tdir, par->tfil,' ', ' ');
        }        
    }
    else
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkReadBarList: Mask barcode directory/filename not specified%c\n",  ' ');
        SET_ERR_MSG("Mask lookup table dir/file unspecified");
        status = MK_FILE_OPEN_ERROR;
    }


    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkStopDirective
 *
 * INVOCATION:
 * status = mkStopDirective (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Stop a currently executing command as soon as possible.
 *
 * DESCRIPTION:
 * If a command is active then set the stopRequested flag so that
 * the task list handling system can stop the sequence at a place where
 * the assembly can easily accept another command.
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

static long mkStopDirective
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.      */
)
{
    long     status = DAR_S_SUCCESS;    /* Return function status.     */
    MK_DEV_PRIVATE *pMkPriv;            /* Private dev. support struc. */


    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkStopDirective: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  If a command is active, request that it be stopped as soon
     *  as it is safe to do so.  This is really only valid for UPDATE,
     *  MOVE or PARK commands.  Refer to mkTaskPostCheck function to see 
     *  when it is safe to stop the command.
     */

    if (pMkPriv->cmdActive)
    {
        DEBUG(DAR_MSG_LOG, 
              "<%ld> %s:mkStopDirective:Will abort command when safe%c\n",' ');
        semTake (pMkPriv->mutexSem, WAIT_FOREVER);
        pMkPriv->stopRequested = TRUE;
        semGive (pMkPriv->mutexSem);
    }

    /*
     *  Otherwise there is nothing to do, send command completion.
     */

    else
    {
        DEBUG(DAR_MSG_MIN, 
              "<%ld> %s:mkStopDirective:no cmd active%c\n",' ');
        mkTerminateTasks (par, status);
    }

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkTaskFinished
 *
 * INVOCATION:
 * status = mkTaskFinished (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) initialization success code.
 *
 * PURPOSE:
 * Check for task and/or command completion.
 *
 * DESCRIPTION:
 * This function is called whenever a device control record finishes
 * executing a command.   If it was the only device active then the
 * command has completed, otherwise acknowledge the device completion
 * and return since we will be called again as the others complete.
 * Use the following algorithm to sort all this out:
 *
 *      Check to make sure that there is a task left to complete and
 *        abort the command if the list is empty.
 *      Log the number of devices expected to finish and the number
 *        that have finished.
 *      If all devices are finished:
 *          Clear the barcode reading flag.
 *          Clear the opto switch delay flag.
 *          Call postCheck to see if the task completed successfully.
 *          Remove this task from the list, are there any more?
 *              No more, the command has finished, so:
 *                  If this was an update write the barcode list.
 *                  If a mask went into the beam record the fact.
 *              Otherwise execute the next task in the list.
 *      If all devices are not finished then return and expect to
 *        be called again when the next one finishes.
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
 * Assumes that BUS1=Ext, BUS2=GRP, BUS3=CAS, BUS4=REL
 *-
 ************************************************************************
 */

static long mkTaskFinished
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.     */
)
{
    ASS_CONTROL_PRIVATE 
                    *pPriv = par->dpvt; /* Private rec. support struc.*/
    MK_TASK_LIST *currentTask = NULL;   /* Ptr to current task.       */
    int        numCompleted;            /* Number of completed tasks. */
    MK_LIST *pTask = NULL;              /* Ptr to task list.          */
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct.*/
    long     status = DAR_S_SUCCESS;
    long     lutStatus = DAR_S_SUCCESS; /* Status from file writing   */


    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkTaskFinished: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  There must be at least one task (the one we are supposed to
     *  finish) on the task list.   If not then there has been an
     *  error somewhere.   Abort the command immediately.
     */

    if ( ellCount( &(pMkPriv->taskList) ) == 0 )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTaskFinished: task list is already empty!%c\n", ' ');
        SET_ERR_MSG("Mask assembly confused, task list empty");
        status = DAR_E_DEVICE;
        mkTerminateTasks (par, status);
        return status;
    }


    /* 
     *  Recover the currently executing command.
     */

    pTask = (MK_LIST *) ellFirst( &(pMkPriv->taskList) );
    currentTask = ( MK_TASK_LIST *) pTask->item;
    DEBUG(DAR_MSG_FULL, 
          "<%ld> %s:mkTaskFinished: current task = %d\n", 
          currentTask->task);

    /*
     *  Compare the number of device commands issued for this task
     *  to the number of device commands that have completed.  Note
     *  that this is for debugging only, no use is made of this information!
     */

    numCompleted = currentTask->extResponse +
                   currentTask->relResponse +
                   currentTask->casResponse + 
                   currentTask->grpResponse;
    DEBUG(DAR_MSG_MAX, 
          "<%ld> %s:mkTaskFinished: expect= %d\n", numCompleted );

    numCompleted = pMkPriv->extFinished + 
                   pMkPriv->relFinished +
                   pMkPriv->grpFinished + 
                   pMkPriv->casFinished;
    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkTaskFinished: completed= %d\n", 
          numCompleted );


    /*
     *  If all of the devices have finished executing then the
     *  task is complete.
     */

    if ((( currentTask->extResponse == pMkPriv->extFinished ) &&
         ( currentTask->relResponse == pMkPriv->relFinished ) &&
         ( currentTask->casResponse == pMkPriv->casFinished ) &&
         ( currentTask->grpResponse == pMkPriv->grpFinished )) ||
         ( pMkPriv->askBarCode == TRUE ) ||
         ( pMkPriv->optoDelayOn == TRUE ) )
    { 

        /*
         * Clear the askBarCode & optoDelayOn flags 
         * and stop the command timer
         */

        semTake (pMkPriv->mutexSem, WAIT_FOREVER);
        pMkPriv->askBarCode = FALSE;
        pMkPriv->optoDelayOn = FALSE;
        semGive (pMkPriv->mutexSem);

        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:mkTaskFinished: Task finished: %d\n", 
              currentTask->task);
        assStopTimer( par, &status );
        if (status != DAR_S_SUCCESS)
        {
            DEBUG(DAR_MSG_WARNING, 
                  "<%ld> %s:mkTaskFinished:failed to stop watchdog timer - status:%ld\n", status);
            status = DAR_S_SUCCESS;
        }

        /*
         * Check the state of the assembly to see if the sensors
         * agree with where things should be after this task has
         * finished.
         */

        if ( ( status = mkTaskPostCheck( par ) ) != DAR_S_SUCCESS )
        {
            DEBUG(DAR_MSG_ERROR, 
                "<%ld> %s:mkTaskFinished, post check failed%c\n", ' ');
        }


        /*
         * Everything checks out so remove the task from the list
         * and see if there is anything more to do.
         */

        else if ( ( mkEmptyList( par, FALSE ) == MK_EMPTY_LIST ) )
        {
            DEBUG(DAR_MSG_MIN, 
                  "<%ld> %s:mkTaskFinished, all tasks done%c\n", ' ');
            semTake (pMkPriv->mutexSem, WAIT_FOREVER);

            /*
             * If an update command has just completed then write the
             * barcode list to the given file.
             */

            if ( pMkPriv->currentCmd == DAR_MODE_UPDATE )
            {
                if (pMkPriv->status != DAR_S_SUCCESS) 
                {
                    DEBUG(DAR_MSG_WARNING, 
                          "<%ld> %s:mkTaskFinished:barcode reader failed, will still write partial list (status: %ld)\n", 
                          pMkPriv->status);
                }

                /* 
                 * Try writing any full or partial list to the lookup table.
                 */ 

                if (pMkPriv->barListEmpty == FALSE)
                {
                    if ( (lutStatus = mkWriteBarList( par )) != DAR_S_SUCCESS )
                    {
                        /* 
                         * Lookup table file writing failure
                         */

                        DEBUG(DAR_MSG_ERROR, 
                              "<%ld> %s:mkTaskFinished:write fail status: %ld\n", 
                              lutStatus);
                    }
                }


                if ( pMkPriv->status != DAR_S_SUCCESS || 
                           lutStatus != DAR_S_SUCCESS    ) 
                {
                    /* 
                     * UPDATE failed due to barcode reader
                     * or file writing.
                     */

                    DEBUG4(DAR_MSG_ERROR, 
                          "<%ld> %s:mkTaskFinished, UPDATE failed, reader status:%ld, file write status: %ld%c%c\n",
                          pMkPriv->status, lutStatus,' ',' ');

                    /* The assembly record shouldn't lose it's index */
                    pPriv->keepIndex = TRUE;
                }
            }

            /*
             * If an insert mask command has just completed then update
             * the status output fields to indicate which mask is there.
             */

            else if ( pMkPriv->currentCmd == DAR_MODE_MOVE &&
                      pMkPriv->currentLoc == MK_CMD_INBEAM )
            {
                pMkPriv->currentCode = MK_BARCODE_ID_IN;
                if (pMkPriv->status != DAR_S_SUCCESS)
                {
                    /* 
                     * On second thought - we don't know for certain 
                     * because the reader failed.
                     */
                    pMkPriv->currentCode = MK_NO_ENTRY;

                    /* Make sure the assembly record doesn't lose it's index */
                    pPriv->keepIndex = TRUE;
                }
            }

            semGive (pMkPriv->mutexSem);

            /*
             *  Signal command completion.
             */

            mkTerminateTasks (par, status);

            semTake (pMkPriv->mutexSem, WAIT_FOREVER);
            pMkPriv->status = DAR_S_SUCCESS;
            semGive (pMkPriv->mutexSem);
        }


        /*
         *  The command has not completed since there are more
         *  tasks to do.  Execute the next one on the list.
         */

        else
        {
            if ((status = mkDoTask( par )) != DAR_S_SUCCESS &&
                status != MK_LAST_TASK )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskFinished: mkDoTask failed%c\n", ' ');
            }
        }
    }


    /* 
     * This task has not completed since some attached devices are
     * still busy.   Exit here but we will be called again as each one
     * finishes.
     */

    else
    {
        DEBUG(DAR_MSG_FULL, "<%ld> %s:mkTaskFinished: Task not done%c\n", ' ');
    }

    return ( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkTaskPostCheck
 *
 * INVOCATION:
 * status = mkTaskPostCheck (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Check the state of the assembly after a task has completed.
 *
 * DESCRIPTION:
 * When a task completes it is necessary to check the configuration
 * of the instrument to confirm that the action completed successfully.
 * Depending on the current task look for signs that a moving device
 * reached its target.   The main reason for this is to insure that it
 * is safe to continue with a sequence by trapping mechanical problems
 * before the next motion.   Any failures detected here will cause
 * the command to abort immediately.
 *
 * Another function of the post-check is to detect when it is possible
 * to stop a sequence (mask is safely back in the cassette) and do so
 * if requested.
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
 * Assumes that BUS1=Ext, BUS2=GRP, BUS3=CAS, BUS4=REL
 *-
 ************************************************************************
 */

static long mkTaskPostCheck
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.    */
)
{
    long     status = DAR_S_SUCCESS;
    int         i;
    MK_LIST *newTask = NULL;                /* Generic task pointer     */
    MK_LIST *pList = NULL;                  /* Task list                */
    MK_TASK_LIST *taskPerformed = NULL;     /* Current task             */
    MK_DEV_PRIVATE *pMkPriv;                /* Internal control struct. */


    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );

    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkTaskPostCheck: entry%c\n", ' ');


    /*
     *  There must be a task to check or something has gone seriously wrong.
     */
    
    if ( ellCount( &(pMkPriv->taskList) ) == 0 || 
        ( pList = (MK_LIST *) ellFirst( &(pMkPriv->taskList) )) == NULL )
    {
        DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkTaskPostCheck: taskList empty =%d\n", 
              ellCount( &(pMkPriv->taskList) ) );
        status = MK_LAST_TASK;
        SET_ERR_MSG("Mask post check task list empty");
        return ( status );
    }

    
    /*
     * Recover the currently executing task.
     */

    taskPerformed = ( MK_TASK_LIST *) pList->item;

    DEBUG(DAR_MSG_MAX, 
          "<%ld> %s:mkTaskPostCheck:%d\n", taskPerformed->task);

    /* 
     *  In simulation mode the mechanics always work perfectly.  Depending
     *  on the task either read the barcode ID, set the current mask location
     *  or stop a command sequence.
     */

    if ( assSimulateLevel(par ) != DAR_SIM_NONE )
    {
        if (taskPerformed->task == MK_ASKFOR_BARCODE_ID)
        {
            status = mkCheckBarcodeId( par );
            return (status);
        }
        else if (taskPerformed->task == MK_TURN_ON_OPTO)
        {
            status = mkBuildNewList( par, pMkPriv->optoDelayMode );
            return (status);
        }
        else if (taskPerformed->task == MK_REL_TO_RELEASE)
        {
            MK_MASK_IN_BEAM = MK_NO_ENTRY;
            MONITOR_VALA;
            MK_EXT_MASK_POS = MK_MASK_LOC_UNKNOWN;
            MONITOR_VALB;
            DEBUG(DAR_MSG_MIN, 
                  "<%ld> %s:mkTaskPostCheck: set MASK_LOC to: %ld\n", 
                  MK_EXT_MASK_POS);
        } 
        else if (taskPerformed->task == MK_REL_TO_LOCK)
        {
            MK_EXT_MASK_POS = MK_MASK_LOC_IN_CASSETTE;
            MONITOR_VALB;
            DEBUG(DAR_MSG_MIN, 
                  "<%ld> %s:mkTaskPostCheck: set MASK_LOC to: %ld\n", 
                  MK_EXT_MASK_POS);
        }
        else if (taskPerformed->task == MK_EXT_TO_MASK)
        {
            if (pMkPriv->status != DAR_S_SUCCESS)
            {
                /* Barcode reader failed so no guarantee what the ID is. */
                MK_MASK_IN_BEAM = MK_NO_ENTRY;
            }
            else
            {
                MK_MASK_IN_BEAM = MK_BARCODE_ID_IN;
            }
            MONITOR_VALA;
            MK_EXT_MASK_POS = MK_MASK_LOC_IN_BEAM;
            MONITOR_VALB;
            DEBUG(DAR_MSG_MIN, 
                  "<%ld> %s:mkTaskPostCheck: set MASK_LOC to: %ld\n", 
                  MK_EXT_MASK_POS);
        }
        else if ((taskPerformed->task == MK_GRP_TO_RELEASE) || 
                 (taskPerformed->task == MK_GRP_TO_GRIP) ||
                 (taskPerformed->task == MK_CAS_TO_SLOT) ||
                 (taskPerformed->task == MK_CAS_TO_C1S1) )
        {
            if (pMkPriv->currentCmd != DAR_MODE_INDEX && pMkPriv->stopRequested)
            {
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkTaskPostCheck: Stopping simulated motion after task:%d\n",taskPerformed->task);

                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->stopRequested = FALSE;
                semGive (pMkPriv->mutexSem);

                /* kill remaining tasks in the move sequence */
                mkEmptyList (par, TRUE);
                newTask = (MK_LIST *) ellFirst (&pMkPriv->taskList);
       
                /* and replace with the stop at grip motion sequence*/
                for (i=0; 
                     i < (sizeof (mkStopAfterRelease)/sizeof (MK_TASK_LIST)); 
                     i++)
                {
                    MK_CHECK_NULL(newTask = malloc(sizeof(MK_LIST)), 
                                  MK_MALLOC_ERROR);
                    newTask->item = (void *) &(mkStopAfterRelease[i]);
                    ellAdd ((ELLLIST *) &(pMkPriv->taskList),&(newTask->node));
                }

                /*
                 * If the command is UPDATE then warn 
                 * the operator that it wasn't completed.
                 */

                if (pMkPriv->currentCmd == DAR_MODE_UPDATE)
                {
                    DEBUG4(DAR_MSG_WARNING,
                            "<%ld> %s:mkTaskPostCheck: WARNING! UPDATE aborted at C%d_S%d!%c%c\n", 
                            pMkPriv->barCodeList[pMkPriv->barNodeNum].casNum, 
                            pMkPriv->barCodeList[pMkPriv->barNodeNum].slotNum, 
                            ' ', ' ');
                }
            }
        }

        /* 
         *  There are no faults in simulation land so return success.
         */

        DEBUG(DAR_MSG_MAX,
              "<%ld> %s:mkTaskPostCheck: simulating, return%c\n", ' ');

        return( status );
    }


    /*
     * Otherwise we are in the real world where things can go wrong....
     * Check the sensor information to check each action.
     */

    switch( taskPerformed->task )
    {
        /* 
         *  Index of the release mechanism, locking pin must be all of the 
         *  way in.
         */
         
        case MK_INDEX_REL:
            if ( MK_SENSOR_WORD & MK_MON_RELNOTLCK )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: Locking pin not in%c\n",' ');
                SET_ERR_MSG("Mask locking pin not fully inserted");
                status = MK_TASK_FAILED;
            }
            break;

        /* 
         *  Removed locking pin, locking pin must be all of the way out.
         *  IF that was successfull then we can say that the mask is no
         *  longer safely in the cassette.
         */
         
        case MK_REL_TO_RELEASE:
            if ( MK_SENSOR_WORD & MK_MON_RELNOTREL )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: Locking pin not out%c\n", ' ');
                SET_ERR_MSG("Mask locking pin not fully removed");
                status = MK_TASK_FAILED;
            }

            if (status == DAR_S_SUCCESS)
            {
                MK_MASK_IN_BEAM = MK_NO_ENTRY;
                MONITOR_VALA;
                MK_EXT_MASK_POS = MK_MASK_LOC_UNKNOWN;
                MONITOR_VALB;
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkTaskPostCheck:Mask pos unknown%c\n", ' ');
            }
            break;
            

        /* 
         *  Inserted locking pin, locking pin must be all of the way in.
         *  If that was done successfully we can say that the mask has been
         *  put back into the cassette.
         */
         
        case MK_REL_TO_LOCK:
            if ( MK_SENSOR_WORD & MK_MON_RELNOTLCK )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: Locking pin not in%c\n", ' ');
                SET_ERR_MSG("Mask locking pin not fully inserted");
                status = MK_TASK_FAILED;
            }

            if ((status == DAR_S_SUCCESS) && (MK_SENSOR_WORD & MK_MON_NOTINFP))
            {
                MK_EXT_MASK_POS = MK_MASK_LOC_IN_CASSETTE;
                MONITOR_VALB;
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkTaskPostCheck: Mask is in cassette%c\n",' ');
            }
            break;
         

        /* 
         *  Indexed the extractor stage.  No sensors can help on this one.
         */
         
        case MK_INDEX_EXT:
            break;


        /* 
         *  Extractor moved into cassette end soft limit.   Check that it
         *  made it.
         */
         
        case MK_EXT_TO_I4:
            if ( !(MK_EXT_POSITION & MK_EXT_ATPAST_REL ) )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: extractor not in limit%c\n", 
                ' ');
                SET_ERR_MSG("Extractor didn't make it to soft limit");
                status = MK_TASK_FAILED;
            }
            break;


        /* 
         *  Extractor moved to the gripping position.   LVDT must show that
         *  we are within the safe gripping range.  If no mask is detected
         *  then the grip mechanism is not aligned properly and has pushed
         *  the mask back into the cassette.
         */
         
        case MK_EXT_TO_I4_GRP:
        case MK_EXT_TO_GRP:
            if ( !(MK_EXT_POSITION & MK_EXT_AT_GRIPPER ) )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: extractor not in grip position%c\n",' ');
                SET_ERR_MSG("Extractor not at grip position");
                status = MK_TASK_FAILED;
            }
	    /* AWE in single update mode, this could happen deliberately, allow it */
            if ( (MK_SENSOR_WORD & MK_MON_NOTOCCUPIED) && !(pMkPriv->currentCmd == DAR_MODE_UPDATE && pMkPriv->singleUpdate == TRUE ))
            {
                DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkTaskPostCheck: No mask in slot after MK_EXT_TO_I4_GRP or MK_EXT_TO_GRP%c\n",  ' ');
                SET_ERR_MSG("No mask found in current cassette slot");
                status = MK_TASK_FAILED;
            }
            break;


        /* 
         *  Extractor moved to the locking/unlocking position.   The LVDT
         *  must show that we are in the valid locking range.
         */
         
        case MK_EXT_TO_I4_REL:
        case MK_EXT_TO_REL:
            if ( !(MK_EXT_POSITION & MK_EXT_AT_RELEASE) )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: Extractor not in release position%c\n",' ');
                SET_ERR_MSG("Extractor not at release position");
                status = MK_TASK_FAILED;
            }
        break;


        /* 
         *  Extractor has moved to the barcode reading or clear position.  No
         *  sensors to help us here.
         */
         
        case MK_EXT_TO_ID:
        case MK_EXT_TO_CLEAR:
            break;


        /* 
         *  Extractor has moved to the mask in-beam position.  Confirm that
         *  there is something in the focal plane.   If so, check the
         *  encoding switches to see if it is actually an IFU and, if so,
         *  move the extractor to the IFU in-beam position.  If sucessful
         *  indicate that the mask is safely in the beam.
         */

        case MK_EXT_TO_MASK:

            if ( MK_SENSOR_WORD & MK_MON_NOTINFP )
            {
                DEBUG(DAR_MSG_ERROR,
                      "<%ld> %s:mkTaskPostCheck: Nothing in FP%c\n", ' ');
                SET_ERR_MSG("Mask/IFU did not make it to focal plane");
                status = MK_TASK_FAILED;
            }

            else if ( !(MK_SENSOR_WORD & MK_MON_IFUNOTIN) )
            {
                if ( !(MK_SENSOR_WORD & MK_MON_MASKNOTIN) )
                {
                    DEBUG(DAR_MSG_ERROR, 
                          "<%ld> %s:mkTaskPostCheck:Mask switch fault - cannot be both IFU and Mask%c\n",' ');
                    SET_ERR_MSG("Mask type switch failure (1)");
                    status = MK_TASK_FAILED;
                }
                else
                {
                    newTask = ( MK_LIST *) ellLast( &(pMkPriv->taskList) );
                    MK_CHECK_NULL( newTask = malloc( sizeof ( MK_LIST ) ), 
                         MK_MALLOC_ERROR );
                    newTask->item = (void *) &(mkMoveConfig3[0]);
                    ellAdd((ELLLIST *) &(pMkPriv->taskList), &(newTask->node));
                }
            }

            else 
	    { 
                DEBUG(DAR_MSG_MIN, 
                     "<%ld> %s:mkTaskPostCheck: wait for Mask switch%c\n", ' ');
	        taskDelay(sysClkRateGet() * 2.0);       /* wait for 2.0 seconds */

		if (MK_SENSOR_WORD & MK_MON_MASKNOTIN)
                {
                   DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck:Mask switch fault - must be either IFU or mask%c\n", ' ');
                   SET_ERR_MSG("Mask type switch failure (2)");
                   status = MK_TASK_FAILED;
                 }

                 if (status == DAR_S_SUCCESS)
                 {
                   MK_MASK_IN_BEAM = pMkPriv->currentCode;
                   MONITOR_VALA;
                   MK_EXT_MASK_POS = MK_MASK_LOC_IN_BEAM;
                   MONITOR_VALB;
                   DEBUG(DAR_MSG_MIN, 
                        "<%ld> %s:mkTaskPostCheck: Mask in beam%c\n", ' ');
                 }
            }
            break;


        /* 
         *  Move Extractor to the IFU in-beam position.  Check coding 
         *  switches to make sure we read an IFU.   If successful indicate
         *  that we now have a mask safely in the beam.
         */

        case MK_EXT_TO_IFU:
            if ( MK_SENSOR_WORD & MK_MON_IFUNOTIN )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: No IFU in FP%c\n", ' ');
                SET_ERR_MSG("No IFU in focal plane");
                status = MK_TASK_FAILED;
            }
            else if (status == DAR_S_SUCCESS)
            {
                MK_MASK_IN_BEAM = pMkPriv->currentCode;
                MONITOR_VALA;
                MK_EXT_MASK_POS = MK_MASK_LOC_IN_BEAM;
                MONITOR_VALB;
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkTaskPostCheck: IFU in beam %c\n", ' ');
            }
            break;


        /*
         *  Gripper mechanism was indexed.   This should leave it in
         *  the released position.
         */

        case MK_INDEX_GRP:
            if ( MK_SENSOR_WORD & MK_MON_GRPNOTREL )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: grip not released%c\n", ' ');
                SET_ERR_MSG("Mask gripper not fully released");
                status = MK_TASK_FAILED;
            }
            break;


        /*
         *  A mask has been released.   This should leave the gripper in
         *  the released position.   If a stop request has been issued
         *  replace the remaining tasks with one to pull the extractor
         *  clear of the cassette. 
         */

        case MK_GRP_TO_RELEASE:
            if ( MK_SENSOR_WORD & MK_MON_GRPNOTREL )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: Grip not released%c\n", ' ');
                SET_ERR_MSG("Mask gripper not fully released");
                status = MK_TASK_FAILED;
            }

            if (pMkPriv->currentCmd != DAR_MODE_INDEX 
                && pMkPriv->stopRequested)
            {
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkTaskPostCheck: Stop current command after moving grip to release position%c\n", ' ');
                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->stopRequested = FALSE;
                semGive (pMkPriv->mutexSem);

                mkEmptyList (par, TRUE);
                newTask = (MK_LIST *) ellFirst (&pMkPriv->taskList);
       
                for (i=0; 
                     i < (sizeof (mkStopAfterRelease)/sizeof (MK_TASK_LIST)); 
                     i++)
                {
                    MK_CHECK_NULL(newTask = malloc(sizeof(MK_LIST)), 
                                  MK_MALLOC_ERROR);
                    newTask->item = (void *) &(mkStopAfterRelease[i]);
                    ellAdd ((ELLLIST *) &(pMkPriv->taskList),&(newTask->node));
                }

                /*
                 * If the command is UPDATE then warn 
                 * the operator that it wasn't completed.
                 */

                if (pMkPriv->currentCmd == DAR_MODE_UPDATE)
                {
                    DEBUG4(DAR_MSG_WARNING,
                            "<%ld> %s:mkTaskPostCheck: WARNING! UPDATE aborted at C%d_S%d!%c%c\n", 
                            pMkPriv->barCodeList[pMkPriv->barNodeNum].casNum, 
                            pMkPriv->barCodeList[pMkPriv->barNodeNum].slotNum, 
                            ' ', ' ');
                }
            }
            break;


        /*
         *  A mask has been gripped.   This should leave the gripper in
         *  the gripped position.   If a stop request has been issued
         *  replace the remaining tasks with ones to release the mask
         *  again and pull the extractor clear of the cassette. 
         */

        case MK_GRP_TO_GRIP:
            if ( MK_SENSOR_WORD & MK_MON_GRPNOTGRP )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: Grip not gripped%c\n", ' ');
                SET_ERR_MSG("Mask gripper not fully gripped");
                status = MK_TASK_FAILED;
                }

            if (pMkPriv->currentCmd != DAR_MODE_INDEX && pMkPriv->stopRequested)
            {
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkTaskPostCheck:Stop current command after moving grip to grip position%c\n",' ');
                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->stopRequested = FALSE;
                semGive (pMkPriv->mutexSem);

                mkEmptyList (par, TRUE);
                newTask = (MK_LIST *) ellFirst (&pMkPriv->taskList);
       
                for (i=0; i < (sizeof (mkStopAfterGrip)/sizeof (MK_TASK_LIST)); i++)
                {
                    MK_CHECK_NULL(newTask = malloc(sizeof(MK_LIST)),
                                  MK_MALLOC_ERROR);
                    newTask->item = (void *) &(mkStopAfterGrip[i]);
                    ellAdd((ELLLIST *) &(pMkPriv->taskList), &(newTask->node));
                }

                /*
                 * If the command is UPDATE then warn 
                 * the operator that it wasn't completed.
                 */

                if (pMkPriv->currentCmd == DAR_MODE_UPDATE)
                {
                    DEBUG4(DAR_MSG_WARNING,
                            "<%ld> %s:mkTaskPostCheck: WARNING! UPDATE aborted at C%d_S%d!%c%c\n", 
                            pMkPriv->barCodeList[pMkPriv->barNodeNum].casNum, 
                            pMkPriv->barCodeList[pMkPriv->barNodeNum].slotNum, 
                            ' ', ' ');
                }
            }
            break;


        /* 
         *  Indexed or parked cassette, no sensors to check.
         */
         
        case MK_INDEX_CAS:
            break;


        /* 
         *  Move a cassette to a park position, no checking required
         *  but set the flag to indicate the cassette is in a
         *  loading zone.
         */

        case MK_CAS_TO_PARK:
            DEBUG(DAR_MSG_FULL,
                  "<%ld> %s:mkTaskPostCheck: setting casParked%c\n", ' ');
            semTake (pMkPriv->mutexSem, WAIT_FOREVER);
            pMkPriv->casParked = TRUE;
            semGive (pMkPriv->mutexSem);     
            break;

        /* 
         *  Moved cassette to a selected slot.  Slot must be aligned when 
         *  motion finished.  This is not the case in update mode because
         *  some slot alignment pins will be missing when we check an IFU
         *  cassette.    If the cassette was moved to the first cassette
         *  position (beginning of an update sequence) check for a stop
         *  request.   If someone cancelled the update replace the remaining
         *  tasks with the stop sequence.
         */

        case MK_CAS_TO_SLOT:
            if ( pMkPriv->currentCmd != DAR_MODE_UPDATE &&
                 (MK_SENSOR_WORD & MK_MON_NOTALIGNED) )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPostCheck: Slot not aligned%c\n", ' ');
                SET_ERR_MSG("Cassette slot not aligned");
                status = MK_TASK_FAILED;
            }
         
        case MK_CAS_TO_C1S1:
            if (pMkPriv->currentCmd != DAR_MODE_INDEX && pMkPriv->stopRequested)
            {
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkTaskPostCheck: Stop current command after moving cassette to C1S1 position%c\n", ' ');
                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->stopRequested = FALSE;
                semGive (pMkPriv->mutexSem);

                mkEmptyList (par, TRUE);
                newTask = (MK_LIST *) ellFirst (&pMkPriv->taskList);
       
                for (i=0; i < (sizeof (mkStopAtCassetteMotion)/sizeof (MK_TASK_LIST)); i++)
                {
                    MK_CHECK_NULL(newTask = malloc(sizeof(MK_LIST)), 
                                  MK_MALLOC_ERROR);
                    newTask->item = (void *) &(mkStopAtCassetteMotion[i]);
                    ellAdd((ELLLIST *) &(pMkPriv->taskList), &(newTask->node));
                }

                /*
                 * If the command is UPDATE then warn 
                 * the operator that it wasn't completed.
                 */

                if (pMkPriv->currentCmd == DAR_MODE_UPDATE)
                {
                    DEBUG4(DAR_MSG_WARNING,
                            "<%ld> %s:mkTaskPostCheck: WARNING! UPDATE aborted at C%d_S%d!%c%c\n", 
                            pMkPriv->barCodeList[pMkPriv->barNodeNum].casNum, 
                            pMkPriv->barCodeList[pMkPriv->barNodeNum].slotNum, 
                            ' ', ' ');
                }
            }
            break;


        /* 
         *  Configuration of the focal plane or cassette has been checked.
         *  NO sensors to check for these actions.
         */

        case MK_CK_FOCAL_PLANE:
        case MK_CHECK_CASSETTE:
            break;

        /* 
         *  Initialize all devices, no checking required
         */
         
        case MK_INIT_ALL:
            break;

        /* 
         *  Test all devices. Test assembly sensors now that the
         *  opto sensor power is on.
         */
         
        case MK_TEST_ALL:

            /*
             *  Do a sanity check on the sensor return information.
             */

            status = mkTestSensors (par);
            break;

        /* 
         *  We were asked for the barcode id.  Read and analyze the
         *  barcode string.
         */

        case MK_ASKFOR_BARCODE_ID:   
            status = mkCheckBarcodeId( par );
            break;


        /* 
         *  The opto sensors are powered up, so it's safe to read the 
         *  sensors and build a task list based on the command mode.
         */

        case MK_TURN_ON_OPTO:        
            status = mkBuildNewList( par, pMkPriv->optoDelayMode );
            break;


        /*
         *  The task just completed is not supported by the post-check
         *  function or the list has been corrupted.  Abort the command.
         */

        default:
            DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkTaskPostCheck: Invalid task=%d\n", 
                  taskPerformed->task );
            SET_ERR_MSG("Invalid task in Mask post check");
            status = MK_INVALID_TASK;
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkTaskPreCheck
 *
 * INVOCATION:
 * status = mkTaskPreCheck (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Insure that it is safe to execute the given task.
 *
 * DESCRIPTION:
 * Determine if the current task can be executed based on the state of
 * MK_SENSOR_WORD and MK_EXT_POSITION.  If simulating just return happy.
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
 * Assumes that BUS1=Ext, BUS2=GRP, BUS3=CAS, BUS4=REL
 *-
 ************************************************************************
 */

static long mkTaskPreCheck
(
    ASSEMBLY_CONTROL_RECORD *par            /* (in)  Ptr to ass. rec.    */
)
{
    ASS_CONTROL_PRIVATE *pPriv = par->dpvt; /* Private rec. support struc. */

    long     status = DAR_S_SUCCESS;
    MK_LIST *pList = NULL;                  /* Ptr to list.                */
    MK_TASK_LIST *taskPerformed = NULL;     /* Task performed.             */
    MK_DEV_PRIVATE *pMkPriv;                /* Ptr to private dev. struct. */


    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkTaskPreCheck: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  Make sure that there is a task to execute....
     */
    
    if ( ellCount( &(pMkPriv->taskList) ) == 0 || 
       ( pList = (MK_LIST *) ellFirst( &(pMkPriv->taskList) )) == NULL )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTaskPreCheck: taskList empty =%d\n", 
              ellCount( &(pMkPriv->taskList) ) );
        status = MK_LAST_TASK;
        SET_ERR_MSG("Mask pre check task list empty");
        return ( status );
    }


    /* 
     *  Nothing can go wrong in simulation land however it is still
     *  necessary to update the mask posiiton if we are simulating
     *  the mask motion.
     */

    if ( assSimulateLevel(par ) != DAR_SIM_NONE )
    {
        if (taskPerformed->task == MK_EXT_TO_REL)
        {
            MK_MASK_IN_BEAM = MK_NO_ENTRY;
            MONITOR_VALA;
            MK_EXT_MASK_POS = MK_MASK_LOC_UNKNOWN;
            MONITOR_VALB;
            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkTaskPreCheck: Faking mask motion%c\n",' ');
        }

        DEBUG(DAR_MSG_MAX, 
              "<%ld> %s:mkTaskPreCheck: simulating, return%c\n", ' ');
        return( status );
    }


    /*
     *  Recover the task about to be performed and check the appropriate
     *  status information before allowing it to be executed.
     */

    taskPerformed = ( MK_TASK_LIST *) pList->item;

    DEBUG(DAR_MSG_MAX, 
          "<%ld> %s:mkTaskPreCheck:%d\n", taskPerformed->task);

    switch( taskPerformed->task )
    {
        /* 
         *  Indexing motions can be done at any time.....
         */

        case MK_INDEX_REL:
        case MK_INDEX_EXT:
        case MK_EXT_TO_I4:
        case MK_EXT_TO_I4_GRP:
        case MK_EXT_TO_I4_REL:
        case MK_INDEX_GRP:
            break;


        /* 
         *  Remove the locking pin.  Extractor must be in the safe mask
         *  release zone before allowing this operation.
         */

        case MK_REL_TO_RELEASE:

            if ( !(MK_EXT_POSITION & MK_EXT_AT_RELEASE ) )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: Extractor not in release position%c\n",' ');
                SET_ERR_MSG("Extractor not in release position");
                status = MK_TASK_FAILED;
            }

            break;
 

        /* 
         *  Inserting the locking pin.  Extractor must be in the safe mask
         *  release zone and the slot must be aligned before allowing this 
         *  operation.
         */
         
        case MK_REL_TO_LOCK:

            if ( !(MK_EXT_POSITION & MK_EXT_AT_RELEASE ) )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: Extractor not in release position%c\n",' ');
                SET_ERR_MSG("Extractor not in release position");
                status = MK_TASK_FAILED;
            }

            break;



        /* 
         *  Moving extractor to the mask lock/unlock position.  
         *  Slot must be aligned before doing this.   Except for
         *  the case when moving from the grip to the release
         *  positions while unlocking a mask the locking pin must be
         *  removed before moving.
         */
         
        case MK_EXT_TO_REL:

            if ( MK_SENSOR_WORD & MK_MON_NOTALIGNED )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: Slot not aligned %c\n",' ');
                SET_ERR_MSG("Cassette slot not aligned");
                status = MK_TASK_FAILED;
            }

            if (!(MK_EXT_POSITION & MK_EXT_AT_GRIPPER))
            {
                if ( MK_SENSOR_WORD & MK_MON_RELNOTREL )
                {
                    DEBUG(DAR_MSG_ERROR, 
                          "<%ld> %s:mkTaskPreCheck: Locking pin in%c\n",' ');
                    SET_ERR_MSG("Lockikng pin is in the way");
                    status = MK_TASK_FAILED;
                }
            }

            if (status == DAR_S_SUCCESS)
            {
                MK_MASK_IN_BEAM = MK_NO_ENTRY;
                MONITOR_VALA;
                MK_EXT_MASK_POS = MK_MASK_LOC_UNKNOWN;
                MONITOR_VALB;
                DEBUG(DAR_MSG_FULL, 
                      "<%ld> %s:mkTaskPreCheck: mask loc unknown%c\n", ' ');
            }

            break;


        /* 
         *  Moving extractor to the grip position.   Slot must be aligned
         *  before moving. Except when moving from the release to the grip 
         *  posiiton there must be a mask in the slot and the gripper must 
         *  be in the released position.
         */
         
        case MK_EXT_TO_GRP:

            if ( MK_SENSOR_WORD & MK_MON_NOTALIGNED )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: Slot not aligned%c\n", ' ');
                SET_ERR_MSG("Cassette slot not aligned");
                status = MK_TASK_FAILED;
            }

            if (!(MK_EXT_POSITION & MK_EXT_AT_RELEASE))
            {
                if ( MK_SENSOR_WORD & MK_MON_GRPNOTREL )
                {
                    DEBUG(DAR_MSG_ERROR, 
                          "<%ld> %s:mkTaskPreCheck:grip not released%c\n", ' ');
                    SET_ERR_MSG("Mask gripper must be released first");
                    status = MK_TASK_FAILED;
                }
         
                if ( MK_SENSOR_WORD & MK_MON_NOTOCCUPIED )
                {
                    DEBUG(DAR_MSG_ERROR,
                          "<%ld> %s:mkTaskPreCheck: No mask in slot before MK_EXT_TO_GRP%c\n", ' ');
                    SET_ERR_MSG("Mask not in current slot, use INIT or UPDATE");
                    status = MK_TASK_FAILED;

                    /* make sure the assembly record doesn't lose it's index */
                    pPriv->keepIndex = TRUE;
                }
            }

            break;


        /* 
         *  Removing a mask from the cassette.  The locking
         *  pin must be out and the gripper in the grip position before
         *  starting the motion. In addition the mask switches must
         *  read the IFU code before making the last IFU motion.
         */
         
        case MK_EXT_TO_IFU:

            if ((MK_SENSOR_WORD & MK_MON_IFUNOTIN ) )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: Not an IFU!%c\n", ' ');
                SET_ERR_MSG("Mask is not coded as an IFU");
                status = MK_TASK_FAILED;
            }

        case MK_EXT_TO_ID:
        case MK_EXT_TO_MASK:

            if ( MK_SENSOR_WORD & MK_MON_RELNOTREL )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: locking pin not removed%c\n", ' ');
                SET_ERR_MSG("Locking pin has not been removed");
                status = MK_TASK_FAILED;
            }

            if ( MK_SENSOR_WORD & MK_MON_GRPNOTGRP )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: not gripping mask%c\n", ' ');
                SET_ERR_MSG("Mask has not been gripped");
                status = MK_TASK_FAILED;
            }
         
            break;


        /* 
         *  Moving the extractor to the clear position.  The locking
         *  pin must be in and the gripper in the release position before
         *  starting the motion.
         */
         
        case MK_EXT_TO_CLEAR:
        
            if ( MK_SENSOR_WORD & MK_MON_RELNOTLCK )
            {
                DEBUG(DAR_MSG_ERROR,
                      "<%ld> %s:mkTaskPreCheck: locking pin not inserted%c\n", ' ');
                SET_ERR_MSG("Locking pin has not been inserted");
                status = MK_TASK_FAILED;
            }

            if ( MK_SENSOR_WORD & MK_MON_GRPNOTREL )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: Grip is not released%c\n", ' ');
                SET_ERR_MSG("Grip has not been released");
                status = MK_TASK_FAILED;
            }
         
            break;


        /* 
         *  About to release a mask.  Extractor must be in the valid
         *  grip range before doing so.
         */

        case MK_GRP_TO_RELEASE:

            if ( !(MK_EXT_POSITION & MK_EXT_AT_GRIPPER ) )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: Extractor not at grip pos%c\n",  ' ');
                SET_ERR_MSG("Extractor not at grip position");
                status = MK_TASK_FAILED;
            }

            break;


        /* 
         *  About to grip a mask.  There must be a mask in the slot and the
         *  extractor must be in the valid grip range before doing so.
         */

        case MK_GRP_TO_GRIP:

            if ( !( MK_EXT_POSITION & MK_EXT_AT_GRIPPER ) )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: Extractor not at grip pos%c\n", ' ');
                SET_ERR_MSG("Extractor not at grip position");
                status = MK_TASK_FAILED;
            }

            if ( MK_SENSOR_WORD & MK_MON_NOTOCCUPIED )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: No mask in the slot%c\n", ' ');
                SET_ERR_MSG("Can not get mask from empty slot");
                status = MK_TASK_FAILED;
            }

            break;

        /* 
         *  About to move the cassette.   If the cassette was in one of the
         *  park positions then check the slot aligned bit to insure that
         *  the cassette was fully inserted.   In all cases the the extractor
         *  must be clear of the cassette, the locking pin installed and
         *  the grip released before moving. 
         */

    case MK_INDEX_CAS:
    case MK_CAS_TO_PARK:
    case MK_CAS_TO_C1S1:
    case MK_CAS_TO_SLOT:

            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkTaskPreCheck: casParked=%d\n", 
                   pMkPriv->casParked);

            if (pMkPriv->casParked && 
                (MK_SENSOR_WORD & MK_MON_NOTALIGNED) &&
                 !(MK_SENSOR_WORD & MK_MON_NOTINSTALLED) )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck:Cassette not fully inserted%c\n", ' ');
                SET_ERR_MSG("Cassette not completely inserted");
                status = MK_TASK_FAILED;
            }
    
            else
            {
                semTake (pMkPriv->mutexSem, WAIT_FOREVER);
                pMkPriv->casParked = FALSE;
                semGive (pMkPriv->mutexSem);
            }

            if ( MK_EXT_POSITION & MK_EXT_IN_LVDT )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck:Extractor not clear of cassette%c\n", ' ');
                SET_ERR_MSG("Extractor not clear of cassette");
                status = MK_TASK_FAILED;
            }
        
            if ( MK_SENSOR_WORD & MK_MON_RELNOTLCK )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: Locking pin not inserted%c\n", ' ');
                SET_ERR_MSG("Mask lock must be locked first");
                status = MK_TASK_FAILED;
            }

            if ( MK_SENSOR_WORD & MK_MON_GRPNOTREL )
            {
                DEBUG(DAR_MSG_ERROR, 
                      "<%ld> %s:mkTaskPreCheck: Grip not released%c\n", ' ');
                SET_ERR_MSG("Mask gripper must be released first");
                status = MK_TASK_FAILED;
            }
                  
            break;


        /*
         *  Tasks that do not involve motion do not need to be checked first.
         */

        case MK_CK_FOCAL_PLANE:
        case MK_CHECK_CASSETTE:
        case MK_ASKFOR_BARCODE_ID:
        case MK_INIT_ALL:
        case MK_TEST_ALL:
        case MK_TURN_ON_OPTO:
            break;

        /*
         *  Trap unsupported tasks here.  Abort the command immediately
         */

        default: 

            DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkTaskPreCheck: Invalid task=%d\n", 
                  taskPerformed->task );
            SET_ERR_MSG("Invalid task in Mask pre check");
            status = MK_INVALID_TASK;

            break;
    }

    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkTerminateTasks
 *
 * INVOCATION:
 * mkTerminateTasks (par, long); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 * (>) status (long)  Status word passed in
 *
 * FUNCTION VALUE:
 * none.
 *
 * PURPOSE:
 * Signal completion of the current command.
 *
 * DESCRIPTION:
 * This function handles the command completion tasks that must be
 * performed for both successful and aborted command.
 *
 * Clear all remaining tasks off of the task list, clean up the
 * internal flags and stop the timeout timer.   Recover any saved
 * error codes and tell the assembly record that the command is finished.
 *
 * 
 * EXTERNAL VARIABLES:
 * pMkPriv->status  is status from a barcode related failure (mkWriteBarList
 *                  or mkCheckBarcodeId 
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

static void mkTerminateTasks
(
    ASSEMBLY_CONTROL_RECORD *par,        /* (in)  Ptr to ass. rec.    */
    long status                          /* (in)  status word         */
)
{
    MK_DEV_PRIVATE *pMkPriv;            /* Private dev. support struc.    */
    long timerStatus = DAR_S_SUCCESS;   /* status word from assStopTimer  */
    long nRequest = 1;                  /* Number of data words to send.  */
    long sensor = 1;                    /* Value to turn off sensor power */

    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkTerminateTasks: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  Clean up command flags and timer
     */

    mkEmptyList (par, TRUE);
    assStopTimer( par, &timerStatus );
    if (timerStatus != DAR_S_SUCCESS)
    {
        DEBUG(DAR_MSG_WARNING, 
                  "<%ld> %s:mkTerminateTasks:failed to stop watchdog timer - status:%ld\n", timerStatus);
    }
    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
    pMkPriv->cmdActive = FALSE;
    pMkPriv->stopRequested = FALSE; 
    semGive (pMkPriv->mutexSem);

    /*
     *  Disable the opto switch sensors. Don't overwrite any bad status code
     *  passed into terminateTasks.  Oh and don't overwrite that private bad
     *  status code that barcode related failures create.  
     */

     /*
      *  Write to the link and check the status.
      *  If the write fails set status to abort the command.
      */

     DEBUG(DAR_MSG_FULL, 
            "<%ld> %s:mkTerminateTasks: turning opto sensors OFF %c\n",  ' ' );

     /*if ( recGblPutLinkValue (&(MK_SENSOR_POWER), 
             (void *) par, DBR_LONG, &sensor, &nRequest) != 
             DAR_S_SUCCESS )*/

     /* AWE: if simulating, the link to optsensorEnable is invalid (no hardware) resulting in an error here .. working around it */

     if ((assSimulateLevel(par) == DAR_SIM_NONE) && dbPutLink(&(MK_SENSOR_POWER),DBR_LONG,&sensor,nRequest) != DAR_S_SUCCESS)
     {
         /*  Don't overwite any existing error status codes  */
         if (status == DAR_S_SUCCESS && pMkPriv->status == DAR_S_SUCCESS)
         {
             status = MK_NO_POWER;
         }
         DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkTerminateTasks:Sensor power control failed%c\n",' ');
         SET_ERR_MSG("Could not power down mask sensors");
     }

    /*
     *  If an error message does not exist and the status passed to this
     *  function is MK_MALLOC_ERROR, then there has been a NULL return from
     *  a memory allocation function, trapped by MK_CHECK_NULL.
     *  Report an appropriate error message.
     */

    if (!strlen(errorMessage) && status == MK_MALLOC_ERROR)
    {
        SET_ERR_MSG( "Memory allocation failure" );
    }

    /*
     *  If an error message exists but the status passed to this
     *  function was SUCCESS, then use the private status which 
     *  must mean there was a barcode related failure (barcode
     *  reader or file writing).
     */

    if (strlen(errorMessage) && status == DAR_S_SUCCESS)
    {
        status = pMkPriv->status;
    }


    /*
     *  And tell the assembly record what happened
     */

    assCommandFinish( par, status, errorMessage );
 
    return;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkTestMode
 *
 * INVOCATION:
 * status = mkTestMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Test the entire Mask assembly.
 *
 * DESCRIPTION:
 * Test the mask assembly using the following algorithm.
 *
 *      If debugging is enabled print out the contents of the barcode list.
 *      Fail immediately if the task list is not empty.
 *      Fail immediately if the sensors return conflicting information.
 *      Build a task list to test all of the device control records. 
 *      Start the sequence by executing the first task.
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
 * Assumes that deviceControl records are wired up with
 * 1=Ext, 2=GRP, 3=CAS, 4=REL
 *-
 ************************************************************************
 */

static long mkTestMode
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.    */
)
{
    long status = DAR_S_SUCCESS;    /* Returned function status.    */
    MK_DEV_PRIVATE *pMkPriv;        /* Ptr to private dev. struct.  */
    int        barCount;            /* Barcode ID counter.          */


    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkTestMode: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );

    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
    pMkPriv->currentCmd = par->mode;
    semGive (pMkPriv->mutexSem);

    if ( par->dbug >= DAR_MSG_MAX )
    {
        /* Display the private structure in MAX debug mode. */

        assDisplayPrivateStruct( par );
    }

    /*
     *  Print out the contents of the barcode list if debugging is enabled.
     */

    for ( barCount = 0; barCount < MK_MAX_SLOTS; barCount++ )
    {
        if ( pMkPriv->barCodeList[barCount].barCodeNum != MK_NO_ENTRY )
        {
            DEBUG4(DAR_MSG_MIN, "<%ld> %s:mkTestMode: C%d_S%d barcode: %ld%c\n",
            pMkPriv->barCodeList[barCount].casNum, 
            pMkPriv->barCodeList[barCount].slotNum, 
            pMkPriv->barCodeList[barCount].barCodeNum, ' ' );
        }
        else
        {
            DEBUG4(DAR_MSG_MIN, "<%ld> %s:mkTestMode: C%d_S%d no barcode entry%c%c\n",
            pMkPriv->barCodeList[barCount].casNum, 
            pMkPriv->barCodeList[barCount].slotNum, ' ', ' ' );
        }
    }


    /* 
     * There should not be anything on the task list at this point.
     */

    if ( ellCount( &(pMkPriv->taskList) ) != 0 )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestMode: Task list not empty.%c\n", ' ' );
        SET_ERR_MSG ("Mask record code corruption detected");
        status = MK_TASK_LIST_NOT_EMPTY;
        mkTerminateTasks (par, status); 
        return (status);
    }

    
    /*
     *  Build a task list to test all of the
     *  attached device control records. Start the sequence by executing
     *  the first task on the list.
     */

    if ( (status = mkBuildList( par, DAR_MODE_TEST )) != DAR_S_SUCCESS )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestMode: mkbuildList failed%c\n", ' ');
        mkTerminateTasks (par, status);
        return status;
    }
    else if ( ( status = mkDoTask( par )) != DAR_S_SUCCESS )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestMode: mkDoTask failed%c\n", ' ');
        mkTerminateTasks (par, status);
        return status;
    }
    
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkTestSensors
 *
 * INVOCATION:
 * status = mkTestSensors (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Test extractor and sensor words for consistancy
 *
 * DESCRIPTION:
 * If debugging is enabled print out the assembly configuration.
 * Test that the extractor position work makes sense.
 * Test that the sensor word makes sense.
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
 * 
 *-
 ************************************************************************
 */

static long mkTestSensors
(
    ASSEMBLY_CONTROL_RECORD 
                *par        /* (in)  Ptr to ass. rec.    */
)
{
    long     status = DAR_S_SUCCESS;    /* Returned function status.    */
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct.  */
                                     

    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkTestSensors: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    /*
     *  Print out Extractor Word if debugging is enabled.
     */

    DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Extractor word is:0x%lx\n", MK_EXT_POSITION );
    if ( (MK_EXT_POSITION & MK_EXT_AT_RELEASE ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Ext in release position%c\n", ' ');
    }
    if ( (MK_EXT_POSITION & MK_EXT_AT_GRIPPER ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Ext in grip position%c\n", ' ');
    }
    if ( (MK_EXT_POSITION & MK_EXT_BTW_R_G ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Ext between Release & Grip%c\n", ' ');
    }
    if ( (MK_EXT_POSITION & MK_EXT_IN_LVDT ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Ext in LVDT area%c\n", ' ');
    }
    if ( (MK_EXT_POSITION & MK_EXT_ATPAST_REL ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Ext at or Past Release position%c\n", ' ');
    }
    if ( (MK_EXT_POSITION & MK_EXT_BEFR_GRP ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Ext before grip position%c\n", ' ');
    }

    /*
     *  Do a sanity check on the above information.
     */

    /*
     *  Must be either before or after grip position, not both.
     */

    if ((MK_EXT_POSITION & MK_EXT_BEFR_GRP ) &&
        (MK_EXT_POSITION & 
        (MK_EXT_AT_RELEASE | MK_EXT_AT_GRIPPER | 
         MK_EXT_BTW_R_G | MK_EXT_ATPAST_REL ) ) )
    {
        DEBUG(DAR_MSG_ERROR,
              "<%ld> %s:mkTestSensors:Ext reads before and after grip%c\n",' ');
        SET_ERR_MSG ("Invalid extractor position");
        status = MK_BAD_SENSORS;
    }


    /*
     *  Must be either before or after release position, not both.
     */

    else if ((MK_EXT_POSITION & MK_EXT_ATPAST_REL ) &&
             (MK_EXT_POSITION & 
             (MK_EXT_AT_GRIPPER | MK_EXT_BTW_R_G | MK_EXT_BEFR_GRP )) )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestSensors:Ext reads before and after release%c\n", ' ');
        SET_ERR_MSG ("Invalid extractor position");
        status = MK_BAD_SENSORS;
    }


    /*
     *  Must be either inside or outside LVDT range, not both.
     */

    else if ((MK_EXT_POSITION & MK_EXT_IN_LVDT  ) &&
             (MK_EXT_POSITION & 
             (MK_EXT_AT_GRIPPER | MK_EXT_BTW_R_G | MK_EXT_ATPAST_REL ) )  && 
             ( MK_EXT_POSITION & MK_EXT_BEFR_GRP ) )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestSensors:Ext reads in and out of LVDT%c\n", ' ');
        SET_ERR_MSG ("Invalid extractor position");
        status = MK_BAD_SENSORS;
    }


    /*
     *  Must be between release and grip or outside this area, not both
     */

    else if ((MK_EXT_POSITION & MK_EXT_BTW_R_G  ) &&
             (MK_EXT_POSITION & 
             (MK_EXT_AT_RELEASE | MK_EXT_AT_GRIPPER | 
              MK_EXT_ATPAST_REL | MK_EXT_BEFR_GRP ) ) )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestSensors: Ext LVDT position conflict%c\n", ' ');
        SET_ERR_MSG ("Invalid extractor position");
        status = MK_BAD_SENSORS;
    }


    /*
     *  Now print and check the sensing switches information.
     */

    DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Sensor word is:0x%lx\n", MK_SENSOR_WORD );


    /*
     * Check the release switches.
     */

    if ( !( MK_SENSOR_WORD & MK_MON_RELNOTLCK ) &&
          ( MK_SENSOR_WORD & MK_MON_RELNOTREL ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Release in lock position%c\n", ' ');
    }
    else if ( !( MK_SENSOR_WORD & MK_MON_RELNOTREL ) &&
               ( MK_SENSOR_WORD & MK_MON_RELNOTLCK ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Release in release position%c\n", ' ');
    }
    else if ( ( MK_SENSOR_WORD & MK_MON_RELNOTLCK ) &&
              ( MK_SENSOR_WORD & MK_MON_RELNOTREL ) )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestSensors: Release between positions%c\n", ' ');
        SET_ERR_MSG ("Release between positions?");
        status = MK_BAD_SENSORS;
    }
    else
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestSensors: Mask release switch failure%c\n", ' ');
        SET_ERR_MSG ("Suspect release switch failure");
        status = MK_BAD_SENSORS;
    }


    /*
     * Check the gripper switches.
     */

    if ( !( MK_SENSOR_WORD & MK_MON_GRPNOTREL ) &&
          ( MK_SENSOR_WORD & MK_MON_GRPNOTGRP ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Gripper in release position%c\n", ' ');
    }
    else if ( !( MK_SENSOR_WORD & MK_MON_GRPNOTGRP ) &&
               ( MK_SENSOR_WORD & MK_MON_GRPNOTREL ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Gripper in grip position%c\n", ' ');
    }
    else if ( ( MK_SENSOR_WORD & MK_MON_GRPNOTGRP ) &&
              ( MK_SENSOR_WORD & MK_MON_GRPNOTREL ) )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestSensors: Gripper between positions%c\n", ' ');
        SET_ERR_MSG ("Gripper between positions?");
        status = MK_BAD_SENSORS;
    }
    else
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestSensors: Mask gripper switch failure%c\n", ' ');
        SET_ERR_MSG ("Suspect gripper switch failure");
        status = MK_BAD_SENSORS;
    }


    /*
     * Check the cassette area switches.
     */

    if ( !( MK_SENSOR_WORD & MK_MON_NOTINSTALLED ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Cassette installed.%c\n", ' ');
    }
    if ( !( MK_SENSOR_WORD & MK_MON_NOTALIGNED ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Mask slot aligned .%c\n", ' ');
    }
    if ( !( MK_SENSOR_WORD & MK_MON_NOTOCCUPIED ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Slot occupied.%c\n", ' ');
    }
    if ( !( MK_SENSOR_WORD & MK_MON_NOTALIGNED ) &&
          ( MK_SENSOR_WORD & MK_MON_NOTINSTALLED ) )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestSensors:Slot aligned without a cassette?%c\n",' ');
        SET_ERR_MSG ("Slot cannot be aligned without cassette");
        status = MK_BAD_SENSORS;
    }
    if ( !( MK_SENSOR_WORD & MK_MON_NOTOCCUPIED ) &&
          ( MK_SENSOR_WORD & MK_MON_NOTALIGNED ) )
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestSensors: slot occupied yet not aligned?%c\n",' ');
        SET_ERR_MSG ("Slot cannot be occupied and not aligned");
        status = MK_BAD_SENSORS;
    }


    /*
     * Check the focal plane area switches.
     */

    if ( !( MK_SENSOR_WORD & MK_MON_NOTINFP ) &&
         !( MK_SENSOR_WORD & MK_MON_MASKNOTIN ) &&
          ( MK_SENSOR_WORD & MK_MON_IFUNOTIN ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Mask In focal plane.%c\n", ' ');
    }

    else if ( !( MK_SENSOR_WORD & MK_MON_NOTINFP ) &&
              !( MK_SENSOR_WORD & MK_MON_IFUNOTIN ) &&
               ( MK_SENSOR_WORD & MK_MON_MASKNOTIN ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:IFU In focal plane.%c\n", ' ');
    }

    else if ( ( MK_SENSOR_WORD & MK_MON_NOTINFP ) &&
              ( MK_SENSOR_WORD & MK_MON_MASKNOTIN ) &&
              ( MK_SENSOR_WORD & MK_MON_IFUNOTIN ) )
    {
        DEBUG(DAR_MSG_MIN, "<%ld> %s:mkTestSensors:Focal plane is empty.%c\n", ' ');
    }

    else 
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkTestSensors:Conflicting FP switch info%c\n",' ');
        SET_ERR_MSG ("Focal plane switch failure(s)?");
        status = MK_BAD_SENSORS;
    }


    return( status );
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkTrackMode
 *
 * INVOCATION:
 * status = mkTrackMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Trap and ignore track mode requests.
 *
 * DESCRIPTION:
 * Track mode is not supported for the mask since there is nothing to
 * track.   Ignore all tracking requests.
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

static long mkTrackMode
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.      */
)
{
    long  status = DAR_S_SUCCESS;       /* Returned function status.    */


    /*
     * Tell the assembly record that the command has finished.
     */

    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkTrackMode: nothing to track%c\n", ' ');
    assCommandFinish( par, status, NULL );

    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkTranslateDir
 *
 * INVOCATION:
 * directive = mkTranslateDir (par); 
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

static long mkTranslateDir
(
    unsigned short    inDir        /* (in)  Dir to translate.    */
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

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkUpdateMode
 *
 * INVOCATION:
 * status = mkUpdateMode (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) initialization code.
 *
 * PURPOSE:
 * Save a mask configuration.
 *
 * DESCRIPTION:
 * Clear out the old barcode list and create a sequence to cycle
 * through all the mask positions and record what is found there.
 * Start the sequence by executing the first task.
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

static long mkUpdateMode
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.      */
)
{
    ASS_CONTROL_PRIVATE 
                    *pPriv = par->dpvt; /* Private rec. support struct */
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct. */
    long     status = DAR_S_SUCCESS;    /* Return function status.     */
    char    scratchBuf[256];            /* Scratchpad character buffer.*/
    FILE    *fp;                        /* File pointer to write to.   */


    DEBUG(DAR_MSG_MAX, "<%ld> %s:mkUpdateMode: entry%c\n", ' ');

    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );

    semTake (pMkPriv->mutexSem, WAIT_FOREVER);
    pMkPriv->currentCmd = par->mode;
    semGive (pMkPriv->mutexSem);


    /*
     *  Before going to the trouble of an update, ensure that
     *  the lookup table can be opened.
     * 
     *  first, bail unless the directory and filename are specified.
     */
     
    if (strlen(par->tfil) && strlen( par->tdir ) )
    {   
        /*
         *  Check to see if the lookup table can be opened.
         */
         
        sprintf (scratchBuf, "%s/%s", par->tdir, par->tfil);
        DEBUG(DAR_MSG_MIN, 
              "<%ld> %s:mkUpdateMode:opening file: %s\n", par->tfil);
        DEBUG(DAR_MSG_MIN, 
              "<%ld> %s:mkUpdateMode:opening file: %s\n", scratchBuf);

        if ( (fp = fopen(scratchBuf, "a")) == NULL)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkUpdateMode: cannot open file %s\n", scratchBuf);
            SET_ERR_MSG("Cannot open mask barcode file");
            status = MK_FILE_OPEN_ERROR;
        }
        else
        {

            /*
             *  Success - close it for now and open it again at the end 
             *  of the update to write the new list.
             */

            DEBUG(DAR_MSG_FULL, 
                  "<%ld> %s:mkUpdateMode:Closing file: %s\n", par->tfil);
            fclose (fp);
        }
    }
    else
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkUpdateMode: Mask barcode directory/filename not specified%c\n",  ' ');
        SET_ERR_MSG("Barcode directory/filename unspecified");
        status = MK_FILE_OPEN_ERROR;
    }

    if (status != DAR_S_SUCCESS)
    {
        /* 
         *  The lookup table file couldn't be opened so abort the update,
         *  but ensure the Mask Assembly doesn't lose it's index.
         */
        DEBUG(DAR_MSG_MIN, 
              "<%ld> %s:mkUpdateMode:opening file failed: %s\n", par->tfil);
        pPriv->keepIndex = TRUE;

        mkTerminateTasks (par, status);
        return ( status );
    }

    /*
     *  Build a task list to cycle through each mask position
     *  and record what is found there.
     */

    status = mkBuildList( par, DAR_MODE_UPDATE );
    if (status)
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkUpdateMode: mkbuildList failed%c\n", ' ');
        mkTerminateTasks (par, status);
        return status;
    }

    /*
     *  Start the sequence by executing the first task.
     */

    status = mkDoTask( par );
    if (status)
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkUpdateMode: mkDoTask failed%c\n", ' ');
        mkTerminateTasks (par, status);
    }
     
    return (status);
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkWriteBarList
 *
 * INVOCATION:
 * status = mkWriteBarList (par); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) par  (ASSEMBLY_CONTROL_RECORD *) Pointer to assemblyControl record
 *
 * FUNCTION VALUE:
 * (long) returned function status.
 *
 * PURPOSE:
 * Write the barcode list to the given file.
 *
 * DESCRIPTION:
 * Write out the barcode list to the file named in the assembly record.
 * Format of the file is:  Cassette# Slot# Barcode#
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
 *  Warning, rely on there being 3 cassettes and 9 slots.
 *-
 ************************************************************************
 */

static long mkWriteBarList
(
    ASSEMBLY_CONTROL_RECORD *par        /* (in)  Ptr to ass. rec.       */
)                                                                    
{
    long     status = DAR_S_SUCCESS;    /* Returned function status.    */
    MK_DEV_PRIVATE *pMkPriv;            /* Ptr to private dev. struct.  */
    char    scratchBuf[256];            /* Scratchpad character buffer. */
    int        barCount;                /* Slot counter                 */
    FILE    *fp;                        /* File pointer to write to.    */


    pMkPriv = ( MK_DEV_PRIVATE *) assGetPrivateStruct( par );


    DEBUG(DAR_MSG_FULL, "<%ld> %s:mkWriteBarList: entry%c\n", ' ');


    /*
     * Write only if the directory and file name are listed in the
     * record.
     */
     
    if (strlen(par->tfil) && strlen( par->tdir ) )
    {   
        /*
         * Open the translation file
         */
         
        sprintf (scratchBuf, "%s/%s", par->tdir, par->tfil);
        DEBUG(DAR_MSG_MIN, 
              "<%ld> %s:mkWriteBarList:Opening file: %s\n", par->tfil);

        if ( (fp = fopen(scratchBuf, "w")) == NULL)
        {
            DEBUG(DAR_MSG_ERROR, 
                  "<%ld> %s:mkWriteBarList: File open failed%c\n", ' ');
            SET_ERR_MSG("Cannot open mask barcode file");
            status = MK_FILE_OPEN_ERROR;
            return ( status );
        }

        
        /*
         * Process one entry at a time
         */
                          
        for ( barCount = 0; barCount < MK_MAX_SLOTS; barCount++ )
        {
            if ( pMkPriv->barCodeList[barCount].barCodeNum != MK_NO_ENTRY )
            {

                sprintf( scratchBuf, "%d\t%d\t%ld\n", 
                        pMkPriv->barCodeList[barCount].casNum, 
                        pMkPriv->barCodeList[barCount].slotNum,
                        pMkPriv->barCodeList[barCount].barCodeNum );

                if ( fputs( scratchBuf, fp ) == EOF )
                {
                     status = MK_BAD_WRITE;
                }
                else
                {
                     DEBUG(DAR_MSG_MAX, "<%ld> %s:mkWriteBarList: Wrote line: %s", scratchBuf);
                }
            }
            if (status == MK_BAD_WRITE)
            {
                SET_ERR_MSG("Mask barcode file write failed");
                DEBUG(DAR_MSG_ERROR, "<%ld> %s:mkWriteBarList: Write failed%c\n",' ');
                break;
            }

        }     /* End of writing the buffer. */

        DEBUG(DAR_MSG_FULL, 
              "<%ld> %s:mkWriteBarList:Closing file: %s\n", par->tfil);
        fclose (fp);
        
    }
    else
    {
        DEBUG(DAR_MSG_ERROR, 
              "<%ld> %s:mkWriteBarList: Mask barcode directory/filename not specified%c\n",  ' ');
        SET_ERR_MSG("Mask lookup table dir/file unspecified");
        status = MK_FILE_OPEN_ERROR;
    }

    return ( status );
}

/*
 * from UK:
 * static char rcsid[] = "$Id$";
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) <year>                       (c) <year>
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
 * devBarcode.c
 *
 * PURPOSE:
 * Device support for stringin records to access barcode scanners.
 *
 * FUNCTION NAME(S)
 * init_chan            - record support initialization
 * devBarcodeConfig     - debug level configure channel
 * trigger_scan         - trigger scan
 * monitor_port         - task to monitor port
 * return_scan          - Task to re-process the string in record
 * open_switch          - Open box connection
 * close_switch         - Close box connection
 * OkToRead             - Check there is something to read
 * WriteScanner         - function to write to the scanner
 * ReadScanner          - function to read to the scanner
 *
 * Note that I2of5 and D2of5 lengths >8 are not accepted by the scanner.
 *
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.1  2001/11/28 20:08:46  mbec
 * *** empty log message ***
 *
 * Revision 1.2  2001/04/23 18:24:42  smb
 * DEBUG macro changed from logMsg to printf so it can display floating point values (bug 196)
 *
 * Revision 1.9  2001/03/20 13:40:26  gmos
 * Modified DEBUG macro. All files now use printf() rather than logMsg(). All also print the output from taskName(0).
 *
 * Revision 1.8  2001/03/01 11:23:04  gmos
 * Introduced devBarcodeSimValue global variable, so simulated barcode readings can be requested on demand from the VxWorks console.
 *
 * Revision 1.7  2000/07/17 10:34:18  gmos
 * Extra checks included to guard against memory corruption.
 *
 * Revision 1.6  2000/07/06 17:22:36  gmos
 * Williams new code merged with mine
 *
 *
 *
 * Revision UK1.5  2000/06/05 16:39:31  gmos
 * Improved simulation mode. Will now generate a different barcode each time and can simulate an error by setting devBarcodeError.
 *
 * Revision UK1.4  2000/05/15 16:50:21  gmos
 * Rearranged header so entire history is in chronological order
 *
 * Revision UK1.3  2000/04/15 11:37:58  gmos
 * Williams new barcode driver
 *
 * Revision UK1.1  2000/04/10 09:19:58  gmos
 * gmtools and gmoscc merged
 * *
 * Revision UK1.2  2000/03/08 11:04:48  gmos
 * New release from DAO - makes V5 - saved temporarily as NEW files
 *
 * Revision 1.4  2000/02/09 05:59:41  dunn
 * Fine tooth comb changes from UK, and headers included.
 *
 * Revision UK1.7  1999/12/01 10:03:45  gmos
 * Split DEBUG macro into DEBUG/DEBUGBAR. Tidied indentation and corrected some
 * of the error message logic. Make chan long throughout. Added more diagnostics
 * Replaced strcpy with strncpy.
 *
 * Revision UK1.6  1999/11/26 13:45:19  gmos
 * Corrected unsafe use of DEBUG macro
 *
 * Revision UK1.5  1999/11/25 13:24:11  gmos
 * Change default debug mode so that error messages are printed.
 *
 * Revision 1.3  1999/08/19 23:02:07  angelic
 * made global vars static
 *
 * Revision 1.2  1999/06/16 20:36:50  angelic
 * *** empty log message ***
 *
 * Revision 1.1  1999/03/30 21:51:22  angelic
 * Initial revision
 *
 * Revision 1.1 1999/02/17  AWE
 * Initial Revision
 *
 *
 *INDENT-ON*
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/


/*
 *  Includes
 */

#include  <vxWorks.h>
#include  <types.h>
#include  <stdioLib.h>
#include  <ioLib.h>
#include  <sysLib.h>
#include  <taskLib.h>
#include  <string.h>
#include  <ctype.h>
#include  <msgQLib.h>
#include  <logLib.h>

#include  <alarm.h>
#include  <dbDefs.h>
#include  <dbAccess.h>
#include  <recSup.h>
#include  <devSup.h>
#include  <module_types.h>
#define GEN_SIZE_OFFSET
#include  <stringinRecord.h>
#undef  GEN_SIZE_OFFSET

/*
 *  Debug and simulation levels
 */

#define BAR_SIM_NONE    0       /* No simulation, do the real thing    */
#define BAR_SIM_FULL    3       /* Simulate barcode reader action      */

#define BAR_DBUG_NONE   0       /* Print nothing except error messages */
#define BAR_DBUG_MIN    1       /* Print warnings and minimal messages */
#define BAR_DBUG_FULL   2       /* Print running commentary            */
#define BAR_DBUG_MAX    3       /* Print everything                    */


/*
 *  Miscellaneous constants
 */

#define TRUE            1       /* Of course.                          */
#define FALSE           0       /* By definition.                      */
#define NUM_CHAN        8       /* Maximum Number of Channels          */
#define MAX_STRING      160     /* String buffer size.                 */
#define BARCODE_LENGTH  40      /* Maximum barcode length.             */
#define DEVICE_LENGTH   10      /* Maximum device string length.       */
#define MESSAGE_LENGTH  5       /* Size of string for message queue.   */
#define READ_TIMEOUT    1       /* Number of seconds before giving up. */
#define MAX_RESCANS     1       /* Maximum number of scan attempts.    */

#define BAR_SIM_BASE	50000000 /* Base number for simulated barcode.  */

/*
 * Code Operated Switch arming and channel character definitions
 */

#define ARMC            "\x23"  /* arming character to activate switch */
#define CH0             '\x30'  /* channel 0                           */
#define CH1             '\x31'  /* channel 1                           */
#define CH2             '\x32'  /* channel 2                           */
#define CH3             '\x33'  /* channel 3                           */
#define CH4             '\x34'  /* channel 4                           */
#define CH5             '\x35'  /* channel 5                           */
#define CH6             '\x36'  /* channel 6                           */
#define CH7             '\x37'  /* channel 7                           */

/*
 * Barcode scanner codes 
 */

#define STX             "\x02"  /* start transmission     */
#define ETX             "\x03"  /* end transmission       */
#define ACK             '\x06'  /* acknowledge            */
#define PREFIX          '\x02'  /* prefix (STX)           */
#define SUFFIX1         '\x03'  /* first suffix (ETX)     */
#define SUFFIX2         '\x00'  /* second suffix (unused) */


/*
 * Barcode scanner setup string segments used to build setup commands below.
 * Note that there is an error in the scanner documentation - the values for
 * "Host triggering" and "Constant scan and report" are reversed.
 */ 

#define CODE_TYPES      ">313"                  /* enable all but EAN128     */
#define DECODE_OPTIONS  "0<24040000000004"
#define RS232_OPTIONS   "060401080000"          /* 9600 N81, no handshaking  */
#define LASER_CNTRL     "000101"                /* level trigger, timeouts   */
#define CODE_OPTIONS    "0:01010008000800"
#define PRE_SUF         "800=0:"                /* no prefix, suffix = CR,LF */
#define I2OF5           "00400100"              /* decode I2of5 only         */
#define XMIT_NR         "0301"                  /* xmit "NR" if decode fails */
#define HOST_TRIG       "1605"                  /* host trigger mode         */
#define ON_TIME         "1700"                  /* Laser on for .5 seconds   */
#define I2OF5_LEN       "2308"                  /* set I2of5 length to 8     */
#define PRE_STX         "2702"                  /* prefix = STX              */
#define SUF_ETX         "28032980"              /* suffix = ETX              */


/*
 * Barcode scanner setup strings.
 */

#define SET_DEFAULTS    "STIG" CODE_TYPES DECODE_OPTIONS RS232_OPTIONS        \
                               LASER_CNTRL CODE_OPTIONS PRE_SUF
#define INIT_PARM       "STIH" I2OF5 XMIT_NR HOST_TRIG I2OF5_LEN ON_TIME PRE_STX SUF_ETX
#define LASER_ON        "STIE"


/*
 *  Device support function prototypes
 */

static long init_chan(struct stringinRecord *);
static long trigger_scan(struct stringinRecord *);


/*
 * Public engineering function prototype.
 */

long devBarcodeConfig(char *, long);


/*
 * Strictly local function prototypes
 */

static void monitor_port ();
static void return_scan ();
static int  OkToRead (int, int);
static long WriteScanner (int, char *);
static long ReadScanner (int, char *);
static long open_switch (int,long);
static long close_switch (int,long);


/* 
 * Create the DSET for use by the Stringin record
 */

struct{
    long  number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write_stringin;
}devBarcode={
    5,
    NULL,
    NULL,
    init_chan,
    NULL,
    trigger_scan
};


/* 
 *  Internal control structure used for each copy of the record. 
 */

typedef struct {
   long                   chan;                     /* channel on switchbox */
   char                   device[DEVICE_LENGTH];    /* serial port address  */
   struct stringinRecord  *psi;                     /* pointer to parent    */
   char                   barcode[BARCODE_LENGTH];  /* the resulting barcode*/
}BarcodePrivate;


/*
 *  Private control variables
 */

static BarcodePrivate pbar[NUM_CHAN];               /* array of private strs*/
static char      channel[NUM_CHAN];                 /* channel identifiers  */
static MSG_Q_ID  Qin;                               /* incoming message Q   */
static MSG_Q_ID  Qout;                              /* outgoing message Q   */
static int       return_id;                         /* read task ID         */
static int       monitor_id;                        /* write task ID        */


/*
 *  Public control variables
 */

int              devBarcodeSimulate = BAR_SIM_NONE; /* simulation level     */
long             devBarcodeSimValue = 0;            /* Simulated barcode value  */
int              devBarcodeError = 0;               /* Set to trigger simulated barcode read error */
int              devBarcodeDebug = BAR_DBUG_NONE;   /* debugging level      */
int              devBarcodePortInitialized = 0;     /* initialization done  */


/* 
 * DEBUGBAR writes a string containing the device name and one integer value
 * if the current debug level is greater than the specified level.
 *
 * NOTE: This macro assumes that the pbar data structure has been defined
 *       and initialized and that argument V is a valid channel number.
 */

#define DEBUGBAR(L,FMT,V)                                        \
if (L <= devBarcodeDebug)                                        \
{                                                                \
    printf ("%s: "FMT, taskName(0), pbar[V].device, V);          \
};

/* 
 * DEBUG writes a string containing one integer value
 * if the current debug level is greater than the specified level.
 */

#define DEBUG(L,FMT,V)                                           \
if (L <= devBarcodeDebug)                                        \
{                                                                \
    printf ("%s: "FMT, taskName(0), V);                          \
};



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * init_chan
 *
 * INVOCATION:
 * status = init_chan ( psi );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) psi (struct stringinRecord *)  Pointer to record structure.
 *
 * FUNCTION VALUE:
 * (static long) function status return.
 *
 * PURPOSE:
 * Initialize a copy of the barcode reading device support.
 *
 * DESCRIPTION:
 * Initialize the serial port and barcode reader using the following
 * algorithm.   Note that this function is called each time a stringin
 * record is created with barcode device support but the serial port and
 * device support initialization functions are only executed once ....
 *
 *      Recover serial port device and channel number from record.
 *      Reject invalid port and channel requests.
 *      The first time this function is called do the following:
 *          Initialize common internal structures.
 *          Create message queues.
 *          Spawn background tasks.
 *          Set up the serial port communications parameters.
 *      Setup individual internal structures.
 *      Set up the barcode scanner for this record.
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
 * Does not reject multiple serial port assignments...
 *-
 ************************************************************************
 */

static long init_chan
(
    struct stringinRecord *psi      /* (in)  Parent function structure  */
)
{

    long chan;                      /* switch channel number            */ 
    long status;                    /* status return                    */
    int  x;                         /* generic index thing              */              
    int  scanfd;                    /* file descriptor for scanner port */
    char device[DEVICE_LENGTH];     /* serial port device name          */
    char input[BARCODE_LENGTH];     /* input string buffer              */
    char sep[3]=",";                /* token seperator string           */

    /*
     * Sanity check on the psi structure pointer
     */

    if (psi == NULL)
    {
       DEBUG(BAR_DBUG_NONE,"Barcode init_chan: NULL psi pointer%c\n",' ');
       return(ERROR);
    }

    /*
     *  Get a local copy of the record OUT field value and extract the
     *  serial port device and switch channel information.  OUT format
     *  is @device,channel (i.e. @/tyCo/1,3   for port 1 channel 3).
     */

    strncpy (input, psi->inp.value.instio.string, BARCODE_LENGTH-1);

    strncpy (device, strtok(input,sep),DEVICE_LENGTH-1);
    chan = atol(strtok(NULL,sep));


    /*
     * Sanity check on device and channel values
     */

    if (device[0]=='\0')
    {
       DEBUG(BAR_DBUG_NONE,"Barcode init_chan: Invalid OUT field: %s Use@device,channel\n",input);
       return(ERROR);
    }

    if ((chan < 0)||(chan >= NUM_CHAN))
    {
       DEBUG(BAR_DBUG_NONE,"Barcode init_chan: Invalid barcode channel number: %ld\n", chan);
       return(ERROR);
    }


    /*
     * Initialize device support features.   This should really be in
     * the init() function to be called by the record at IOC init time
     * but it isn't.   Uses the devBarcodePortInitialized flag to insure
     * that the following is only executed by the first record to call
     * this function.
     */ 

    if (!devBarcodePortInitialized)
    {
       devBarcodePortInitialized = 1;

       /* 
        * initialize the internal control structure
        */

       for (x=0; x<NUM_CHAN; x++)
       {
          pbar[x].chan = -1;
          strncpy(pbar[x].device, device, DEVICE_LENGTH-1);
          strcpy(pbar[x].barcode,"\0");
       }

       DEBUGBAR(BAR_DBUG_FULL,"Initializing barcode device:%s chan:%ld\n",chan);

 
       /* 
        * initialize the array of channel selectors 
        */

       channel[0] = CH0;
       channel[1] = CH1;
       channel[2] = CH2;
       channel[3] = CH3;
       channel[4] = CH4;
       channel[5] = CH5;
       channel[6] = CH6;
       channel[7] = CH7;

       /* 
        * Create the send and receive message Queues 
        */ 

       Qin = msgQCreate(20,5,MSG_Q_FIFO);
       if (!Qin)
       {
          DEBUGBAR(BAR_DBUG_NONE,
                   "Barcode d:%s c:%ld Can't create input queue\n",chan);
          return(ERROR);
       }
 
       Qout = msgQCreate(20,5,MSG_Q_FIFO);
       if (!Qout)
       {
          DEBUGBAR(BAR_DBUG_NONE,
                   "Barcode d:%s c:%ld Can't create output queue\n",chan);
          return(ERROR);
       }
 

       /* 
        * Spawn the port control and record re-processing tasks.
        */

       DEBUGBAR(BAR_DBUG_FULL,
                "Barcode d:%s c:%ld Spawning monitor_port...\n",chan);
       monitor_id = taskSpawn("monitor_port", 
                              100, 
                              VX_FP_TASK, 
                              2000,
                              (FUNCPTR)monitor_port,
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

       if ( monitor_id == ERROR )
       {
          DEBUGBAR(BAR_DBUG_NONE,
                   "Barcode d:%s c:%ld Can't create monitor_port task\n",chan);
          return(ERROR);
       }
 
       DEBUGBAR(BAR_DBUG_FULL,
                "Barcode d:%s c:%ld Spawning return_scan...\n",chan);
       return_id = taskSpawn("return_scan", 
                             100, 
                             VX_FP_TASK, 
                             2000,
                             (FUNCPTR)return_scan,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
       if ( return_id == ERROR )
       {
          DEBUGBAR(BAR_DBUG_NONE,
                   "Barcode d:%s c:%ld Can't create monitor_port task\n",chan);
          return(ERROR);
       }

       /* 
        *  Setup communications port.   Don't do this in simulation mode! 
        */

       DEBUGBAR(BAR_DBUG_FULL,
                "Barcode d:%s c:%ld Setting port parameters\n",chan);
 
       if (!devBarcodeSimulate)
       {

          /*
           * Open the serial port 
           */

          if ((scanfd =  open(device, O_RDWR | O_NONBLOCK,0644)) == ERROR)
          {
             DEBUGBAR(BAR_DBUG_NONE,
                      "Barcode d:%s c:%ld Serial port open fail\n",chan);
             return(ERROR);
          }


          /* 
           *  Set the communications parameters for the port
           */ 

          status = ioctl (scanfd, FIOSETOPTIONS, OPT_RAW);       
          if ( status != ERROR ) status = ioctl (scanfd, FIOBAUDRATE, 9600);
          if ( status != ERROR ) status = ioctl (scanfd, FIOFLUSH, 0);          
          if ( status == ERROR )
          {
             DEBUGBAR(BAR_DBUG_NONE,
                      "Barcode d:%s c:%ld Can't configure port.\n",chan);
             close (scanfd);
             return(ERROR);
          }

          close (scanfd);
       }
    }


    /*
     *  After the first barcode reader has been installed the global 
     *  initialization will have been done... note this.
     */

    else
    {
       DEBUGBAR(BAR_DBUG_FULL,
                "Barcode d:%s c:%ld already initialized.\n",chan);
    }


    /*
     *  Now do the channel-by-channel initialization stuff.   Start
     *  by checking for duplicate channels and inconsistant device names.
     */

    if (pbar[chan].chan == -1)
    {
       pbar[chan].chan = chan;
    }
    else
    {
       DEBUGBAR(BAR_DBUG_NONE,
                "Barcode d:%s c:%ld  Duplicate channel.\n",chan);
       return(ERROR);
    }

    if (strcmp(pbar[chan].device,device)!=0)
    {
       DEBUGBAR(BAR_DBUG_NONE,
                "Barcode d:%s c:%ld This device not configured\n",chan);
       strncpy(pbar[chan].device, device, DEVICE_LENGTH-1);
    }

    /*
     *  Save the parent record pointer for access to its fields and for
     *  callback.
     */

    pbar[chan].psi = psi;
    psi->dpvt = (void *)&pbar[chan];


    /*
     *  Now configure the scanner that is attached to this channel on
     *  the switch.   If we are simulating then there is nothing to
     *  configure so just return here....
     */

    if (devBarcodeSimulate) return (0);

    DEBUGBAR(BAR_DBUG_FULL,
             "Barcode device:%s  Setting up Scanner on channel %ld...\n",chan);

    /* 
     *  Open the serial port 
     */ 

    if ((scanfd =  open(pbar[chan].device, O_RDWR | O_NONBLOCK,0644)) == ERROR)
    {
       DEBUGBAR(BAR_DBUG_NONE,
                "Barcode d:%s c:%ld Can't open serial port.\n",chan);
       return(ERROR);
    }


    /* 
     *   Select the serial switch channel for this barcode reader.
     */

    if (open_switch(scanfd,chan) == ERROR)
    {
       DEBUGBAR(BAR_DBUG_NONE,
                "Barcode d:%s c:%ld Can't connect to switch.\n",chan);
       close(scanfd);
       return(ERROR);
    }

    /*
     *  Send the configuration strings
     */

    DEBUG(BAR_DBUG_MAX,
          "Sending scanner: " SET_DEFAULTS "%c\n", ' ');

    if (WriteScanner (scanfd, SET_DEFAULTS) == ERROR)
    {
       DEBUGBAR(BAR_DBUG_NONE,
                "Barcode d:%s c:%ld SET_DEFAULTS failed.\n",chan);
       close_switch(scanfd,chan);
       close(scanfd);
       return (ERROR);
    }
   
    taskDelay(sysClkRateGet() * 0.5);       /* wait for 0.5 seconds */
   
    DEBUG(BAR_DBUG_MAX,
          "Sending scanner: " INIT_PARM "%c\n",' ');

    if (WriteScanner (scanfd, INIT_PARM) == ERROR)
    {
       DEBUGBAR(BAR_DBUG_NONE,
                "Barcode d:%s c:%ld INIT_PARM failed.\n",chan);
       close_switch(scanfd,chan);
       close(scanfd);
       return (ERROR);
    }
   
    taskDelay(sysClkRateGet() * 0.5);       /* wait for 0.5 seconds */

    close_switch(scanfd,chan);
    close(scanfd);

    return (0);

}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * devBarcodeConfig
 *
 * INVOCATION:
 * status = devBarcodeConfig( device, chan );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) device (char *)      Device Name
 * (>) chan (long )         Code operated switch channel number
 *
 * FUNCTION VALUE:
 * (long) function status
 *
 * PURPOSE:
 * Engineering function used to reconfigure a barcode reader from the VxWorks
 * console.
 *
 * DESCRIPTION:
 * Engineering function used to reconfigure a barcode reader from the VxWorks
 * console.
 * This allows a duff barcode reader to be replaced and reconfigured without
 * rebooting.
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


long devBarcodeConfig
(
    char *device,               /* (in)     Device name                     */
    long chan                   /* (in)     Channel number                  */
)    
{
    int  scanfd;                /* file descriptor for scanner port         */

    /* 
     *  Reject invalid input parameters 
     */              

    if ( device == NULL )
    {
       DEBUG(BAR_DBUG_NONE,
             "devBarcodeConfig: NULL device name.%c\n",' ');
       return(ERROR);
    }

    if ((chan < 0)||(chan >= NUM_CHAN))
    {
       DEBUG(BAR_DBUG_NONE,
             "devBarcodeConfig: Bad channel number, %ld\n",chan);
       return(ERROR);
    }

    /* 
     *  Open the serial port and select requested the code operated switch
     *  channel.
     */ 

    if ((scanfd =  open(device, O_RDWR | O_NONBLOCK,0644)) == ERROR)
    {
       DEBUG(BAR_DBUG_NONE,
             "devBarcodeConfig: Cannot open serial port on %s.\n",device);
       return(ERROR);
    }

    if (open_switch(scanfd,chan) == ERROR)
    {
       DEBUG(BAR_DBUG_NONE,
             "devBarcodeConfig: Cannot connect to switch channel %ld.\n",chan);
       close(scanfd);
       return(ERROR);
    }


    /* 
     *  Then set up the barcode scanner by sending the configuration strings.
     */

    DEBUG(BAR_DBUG_MIN,
          "Setting up barcode scanner on channel %ld... \n", chan);
    DEBUG(BAR_DBUG_MAX,
          "Sending: " SET_DEFAULTS "%c\n", ' ');

    if (WriteScanner (scanfd, SET_DEFAULTS) == ERROR)
    {
       DEBUG(BAR_DBUG_NONE,
             "devBarcodeConfig: Scanner SET_DEFAULTS failed.%c\n",' ');
       close_switch(scanfd,chan);
       close(scanfd);
       return (ERROR);
    }
   
    taskDelay(sysClkRateGet() * 0.5);       /* wait for 0.5 seconds */
   
    DEBUG(BAR_DBUG_MAX,
          "Sending: " INIT_PARM "%c\n",' ');

    if (WriteScanner (scanfd, INIT_PARM) == ERROR)
    {
       DEBUG(BAR_DBUG_NONE,
             "devBarcodeConfig: Scanner INIT_PARM failed.%c\n",' ');
       close_switch(scanfd,chan);
       close(scanfd);
       return (ERROR);
    }
   
    taskDelay(sysClkRateGet() * 0.5);       /* wait for 0.5 seconds */

    close_switch(scanfd,chan);
    close(scanfd);

    DEBUG(BAR_DBUG_FULL,
          "devBarcodeConfig: Scanner reconfigured.%c\n", ' ');

    return (0);
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * trigger_scan
 *
 * INVOCATION:
 * status = trigger_scan ( psi );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) psi ( struct stringinRecord *)   Pointer to record structure.
 *
 * FUNCTION VALUE:
 * (long ) function status.
 *
 * PURPOSE:
 * To request that a barcode be read.
 *
 * DESCRIPTION:
 * This function is called directly by the parent record when it
 * wants to input a barcode.   Processing is done asynchronously
 * resulting in two calls for each read as follows:
 *
 * A request for barcode input causes the record to process which 
 * results in this function being called.   This function places
 * the channel number of the reader to use on the outgoing message
 * queue and sets the record's PACT (processing active) field to
 * TRUE to prevent the record from being called again until the
 * data is ready.  The function then returns.
 *
 * When the background tasks have finished reading the information
 * from the barcode reader they re-process the record directly.
 * This causes this function to be called again.  The fact that the
 * PACT field is set means that the reading has finished. This
 * function now sets the PACT field to FALSE and returns thereby
 * telling the record that new data is available and it can receive
 * requests for more reads.....
 *
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
 * None.
 *-
 ************************************************************************
 */

static long trigger_scan
(
    struct stringinRecord *psi      /* pointer to record structure  */
)
{
    long           chan;                /* serial switch channel to use     */
    long           status;              /* function status.                 */
    char           message[MESSAGE_LENGTH];
					/* Formatted channel # to go on Q   */
    BarcodePrivate *tpbar;  		/* private struct */

    /*
     * Sanity check on the psi structure pointer
     */

    if (psi == NULL)
    {
       DEBUG(BAR_DBUG_NONE,"Barcode trigger_scan: NULL psi pointer%c\n",' ');
       return(ERROR);
    }

    /*
     * Define the private structure pointer and make sure it is not NULL.
     */

    tpbar = (BarcodePrivate *)psi->dpvt;
 
    if (tpbar == NULL)
    {
       DEBUG(BAR_DBUG_NONE,"Barcode trigger_scan: NULL tpbar pointer%c\n",' ');
       return(ERROR);
    }

    /*
     *  Recover the channel number from the private control structure that was
     *  saved in the calling record's structure and check it is within range.
     */

    chan = tpbar->chan;

    if ((chan < 0)||(chan >= NUM_CHAN))
    {
       DEBUG(BAR_DBUG_NONE,"Barcode trigger_scan: Invalid barcode channel number: %ld\n", chan);
       return(ERROR);
    }


    /*
     *  Check the PACT field... if it is TRUE then this is a callback and
     *  there is noting for us to do.
     */

    if (psi->pact)
    {
       DEBUGBAR(BAR_DBUG_FULL,
                "Barcode device:%s chan:%ld Finishing processing.\n",chan);
       return(0);
    }


    /*
     *  It is not a callback but a read request. Set the processing active 
     *  field to indicate that a barcode will be coming and to prevent any one 
     *  else from processing this record until the code has been returned.
     */

    psi->pact = TRUE;


    /*
     *  Then place the code operated switch channel number onto the outgoing
     *  queue.   This will wake up the barcode reader communication task
     *  which will then read the barcode.
     */

    sprintf(message,"%ld",chan);
    DEBUGBAR(BAR_DBUG_FULL,
             "Barcode device:%s Placing channel %ld into outgoing Q.\n",chan);

    status = msgQSend(Qout, message, sizeof(message), 10, MSG_PRI_NORMAL);
    if (status != 0)
    {
       DEBUGBAR(BAR_DBUG_NONE,
                "Barcode device:%s Message Qout write for %ld failed.\n",chan);
    }

    return(0);
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * monitor_port
 *
 * INVOCATION:
 * monitor_port();
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None.
 *
 * FUNCTION VALUE:
 * void
 *
 * PURPOSE:
 * To trigger a scan and read the resulting code
 *
 * DESCRIPTION:
 * Handle barcode reader communications using the following algorithm:
 *
 *      Repeat forever:
 *          Wait for a message (channel number) on the outgoing queue.....
 *          When a channel number arrives make sure that it is valid. 
 *          If we are in simulation mode fake a barcode number.
 *          Otherwise read one as follows:
 *              Open the serial port.
 *              Select reader by sending select code to switch.
 *              Send the "laser on" command which will read the code.
 *              Read and analyze the return string.
 *              Close the switch and serial port.
 *          Write the channel number to the incoming Queue to indicate
 *            that the read is complete.
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
 * None.
 *-
 ************************************************************************
 */
static void monitor_port ()
{
    int     scanfd;                     /* file descriptor for scanner port             */
    char    cmd_string[MAX_STRING];     /* scratchpad character buffer for commands     */
    char    rpy_string[MAX_STRING];     /* scratchpad character buffer for responses    */
    char    message[MESSAGE_LENGTH];    /* message received/sent on queue               */
    long    chan;                       /* channel number                               */
    long    rescan;                     /* rescan counter                               */
    long    error = 0;                  /* processing error flag                        */
    long    status;			/* Function status from msgQSend                */

    for (;;)
    {
       /* 
        *  Suspend the task until a message arrives on the outgoing message
        *  queue.
        */

        msgQReceive(Qout, message, MESSAGE_LENGTH, WAIT_FOREVER);

        DEBUG(BAR_DBUG_MAX,
              "Barcode message received from Qout = %s\n", message);

        /*
         *  Insure that the channel request is valid...
         */

        chan = atol(message);
        if ( (chan < 0) || (chan >= NUM_CHAN) )
        {
            DEBUG(BAR_DBUG_NONE,
                  "Barcode monitor_port: Invalid channel number from Qout, %ld\n",chan);
            error = 1;
        }


        /*
         *  In simulation mode make up a bogus barcode, incrementing the value 
         *  each time a new barcode is requested. The devBarcodeSimValue variable
         *  can be altered from the VxWorks console to control the simulated barcode
         *  generated. The value will wrap at 1000 making the simulated barcode range
         *  50000000 to 50000999.
         */

        else if (devBarcodeSimulate)
        {
            if ( devBarcodeError == 0 )
            {
                DEBUGBAR(BAR_DBUG_FULL,
                         "Barcode device:%s chan:%ld Faking barcode....\n",chan);
                sprintf(pbar[chan].barcode,"%8ld", BAR_SIM_BASE + devBarcodeSimValue);
                devBarcodeSimValue++;
                if ( devBarcodeSimValue > 1000 ) devBarcodeSimValue = 0;
            }
            else if ( devBarcodeError == 1 )
            {
                DEBUGBAR(BAR_DBUG_FULL,
                         "Barcode device:%s chan:%ld Faking barcode NR....\n",chan);
                strcpy(pbar[chan].barcode, "NR");
            }
            else
            {
                DEBUGBAR(BAR_DBUG_FULL,
                         "Barcode device:%s chan:%ld Faking barcode ERROR....\n",chan);
                strncpy(pbar[chan].barcode, "ERROR - simulated",
                        BARCODE_LENGTH-1);
            }
        }


        /*
         *  Otherwise trigger a read and analyze the results....
         */
        
        else if (!error)
        {
            /* 
             *  Open the serial port 
             */ 

            if ((scanfd = open(pbar[chan].device,O_RDWR|O_NONBLOCK,0644))==-1)
            {
                DEBUGBAR(BAR_DBUG_NONE,
                         "Barcode d:%s c:%ld Open port failed.\n",chan);
                strncpy (pbar[chan].barcode,"ERROR - open port fail on read",
                         BARCODE_LENGTH-1);
                error = 1;
            }

     
            /* 
             *  connect to correct scanner through switch
             */

            if (!error && open_switch(scanfd, chan) == ERROR)
            {
                DEBUGBAR(BAR_DBUG_NONE,
                         "Barcode device:%s chan:%ld Switch failed.\n",chan);
                close (scanfd);
                strncpy(pbar[chan].barcode,"ERROR - open switch failed on read", 
                        BARCODE_LENGTH-1);
                error = 1;
            }
     
            /* 
             *  Trigger barcode read by sending LASER_ON code
             */

            for (rescan = 0; !error && rescan < MAX_RESCANS; rescan++)
            {

                strncpy(cmd_string,LASER_ON,MAX_STRING-1);
     
                if (!error && WriteScanner(scanfd, cmd_string) == ERROR)
                {
                    DEBUGBAR(BAR_DBUG_NONE,
                         "Barcode d:%s c:%ld No ack on write.\n",chan);
                    close_switch(scanfd,chan);
                    close (scanfd);   
                    strncpy (pbar[chan].barcode,"ERROR - no ack on write",
                             BARCODE_LENGTH-1);
                    error = 1;
                }


                /*
                 *  Then wait for the return string...
                 */

                if (!error)
                {
                    DEBUGBAR(BAR_DBUG_MIN,
                             "Barcode d:%s c:%ld Waiting...\n",chan);
     
                    if (ReadScanner(scanfd, rpy_string) == ERROR)
                    {
                        DEBUGBAR(BAR_DBUG_NONE,
                                 "Barcode d:%s c:%ld Read failed, retry\n",
                                 chan);
                        ioctl (scanfd,FIOFLUSH,0);
                        strncpy (pbar[chan].barcode,"ERROR - read failure", 
                                 BARCODE_LENGTH-1);
                     }

                    else if (strcmp(rpy_string, "NR") == 0)
                    {
                        strncpy (pbar[chan].barcode, rpy_string, BARCODE_LENGTH-1);
                        DEBUGBAR(BAR_DBUG_MIN,
                                 "Barcode d:%s c:%ld Bad barcode.\n",chan);
                        break;
                    }

                    else
                    {
                        strncpy (pbar[chan].barcode, rpy_string, BARCODE_LENGTH-1);
                        DEBUGBAR(BAR_DBUG_FULL,
                                 "Barcode d:%s c:%ld Read Success.\n",chan);
                        break;
                    }
                }
            }

            /*
             *  We are finished for whatever reason... trigger completion
             *  by returning the message to the incoming queue.
             */

            close_switch(scanfd,chan);
            close (scanfd);   
        }

        /* DEBUGBAR cannot be used because the channel number can be invalid at this point */
        DEBUG(BAR_DBUG_MAX,
              "Barcode monitor_port: Sending message %s to Qin.\n", message);

        status = msgQSend(Qin, message, sizeof(message), 10, MSG_PRI_NORMAL);
        if (status != 0)
        {
             DEBUG(BAR_DBUG_NONE,
                "Barcode monitor_port: Message Qin write for channel %ld failed.\n",chan);
        }
        error = 0;
    }
}




/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * return_scan
 *
 * INVOCATION:
 * return_scan();
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None.
 *
 * FUNCTION VALUE:
 * void
 *
 * PURPOSE:
 * Reprocess the appropriate stringin record when a barcode is received
 *
 * DESCRIPTION:
 * This function waits for a message from the monitor_port task above and
 * then re-processes the record associated with the given channel number.
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

static void return_scan()
{
    char         message[MESSAGE_LENGTH];            /* message on the message queue     */
    long         chan;                  /* code operated switch channel     */

    for (;;)
    {
        /* 
         *  Wait for a message to arrive on the incoming queue.
         */

       msgQReceive(Qin, message, MESSAGE_LENGTH, WAIT_FOREVER);


        /*
         *  The message will be the channel number of the barcode reader that
         *  has just returned a string.  If the channel number is invalid
         *  all we can do is to log an error message.
         */

       chan = atol(message);
       if ((chan < 0)||(chan >= NUM_CHAN))
       {
          /* 
           *  can't return an error here with DEBUGBAR macro because there's no 
           *  chan to access the private structure
           */

          DEBUG(BAR_DBUG_NONE,
                "Barcode return_scan: Invalid channel from Qin, %ld.\n",chan);
       }
       else
       {

           /* 
            *  Use the channel number to access the internal control structure
            *  for this record.   Copy the string read from the barcode reader
            *  to the record VAL field and then process the record to make this
            *  string available to the world.   Re-processing also sets the
            *  PACT field to false to indicate that the asynchronous processing
            *  has finished.
            */

          strncpy(pbar[chan].psi->val, pbar[chan].barcode, BARCODE_LENGTH-1);

          dbScanLock((struct dbCommon *)pbar[chan].psi);
          (*((struct rset *) pbar[chan].psi->rset)->process)(pbar[chan].psi);
          dbScanUnlock((struct dbCommon *)pbar[chan].psi);
       }
    }
}




/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * open_switch
 *
 * INVOCATION:
 * status = open_switch( fd, chan );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) fd (int)         serial device file descriptor.
 * (>) chan (long)      code operated switch channel number to open
 *
 * FUNCTION VALUE:
 * (long) function status.
 *
 * PURPOSE:
 * Establish switch box connection
 *
 * DESCRIPTION:
 * Establish switch box connection by arming the switch and sending
 * the desired channel code.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * A serial connection should already have been opened and a file descriptor
 * provided in fd.
 * The channel number, chan, should already have been checked to be within range.
 *
 * SEE ALSO:
 * None.
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long open_switch
(
    int fd,                 /* (in) serial device file descriptor           */
    long chan               /* (in) Channel number                          */
)
{
    char cmd_string[MAX_STRING];    /* character scratch buffer             */
    char chan_string[2];            /* channel select buffer                */


    /* 
     *  Send the arming character followed by the channel select character
     */

    chan_string[0] = channel[chan];
    chan_string[1] = '\0';

    DEBUGBAR(BAR_DBUG_FULL,
         "Barcode device:%s chan:%ld Opening switchbox connection.\n",chan);

    strncpy(cmd_string, (char *) ARMC, MAX_STRING-1);
    strcat(cmd_string, chan_string);

    DEBUG(BAR_DBUG_MAX, "Sending switchbox command string <%s>", cmd_string);

    if (write(fd,cmd_string,strlen(cmd_string)) != strlen(cmd_string))
    {
       DEBUGBAR(BAR_DBUG_NONE,
                "Barcode d:%s c:%ld Failed to establish switch connection.\n",
                chan);
       return (ERROR);
    }
    return (0);
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * close_switch
 *
 * INVOCATION:
 * status = close_switch( fd, chan );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) fd (int)             serial device file descriptor
 * (>) chan (long)          code operated switch channel number
 *
 * FUNCTION VALUE:
 * (long) function status.
 *
 * PURPOSE:
 * Close switch box connection.
 *
 * DESCRIPTION:
 * Close switch box connection.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * A serial connection should already have been opened and a file descriptor
 * provided in fd.
 * The channel number, chan, should already have been checked to be within range.
 *
 * SEE ALSO:
 * None.
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */


static long close_switch
(
    int fd,             /* (in) File descriptor for the serial device      */
    long chan           /* (in) Channel number to be closed                */
)
{
    char cmd_string[MAX_STRING];

    DEBUGBAR(BAR_DBUG_FULL,
             "Barcode d:%s c:%ld Closing switchboxconnection.\n",chan);

    /* 
     *  Close any open connections by sending the arming character
     */

    strncpy(cmd_string, (char *) ARMC, MAX_STRING-1);
   
    if (write(fd,cmd_string,strlen(cmd_string)) != strlen(cmd_string))
    {
       DEBUGBAR(BAR_DBUG_NONE,
                "Barcode d:%s c:%ld Failed to closeswitch box connection.\n",
                chan);
       return (ERROR);
    }
    return (0);
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * OkToRead
 *
 * INVOCATION:
 * stat = OkToRead( fd, timeout );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) fd (int)                 Serial device file descriptor
 * (>) timeout (int )           maximum time to wait for a character
 *
 * FUNCTION VALUE:
 * (int) Returns # of bytes on read buffer
 *
 * PURPOSE:
 * Check there is something to read
 *
 * DESCRIPTION:
 * Wait up to the specified timeout period for characters to arrive
 * on the selected serial device.
 * Returns # of bytes that have arrived so far.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * A serial connection should already have been opened and a file descriptor
 * provided in fd.
 *
 * SEE ALSO:
 * None.
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static int OkToRead
(
    int     fd,             /* serial device file descriptor                */
    int     timeout         /* maximum time (in seconds) to wait for input  */
)
{
    int i;                  /* generic index thing                          */
    int maxWaitTicks;       /* timeout in system ticks                      */
    int bytesReceived = 0;  /* number of characters in the buffer           */
    int status;             /* function return status                       */

    maxWaitTicks = timeout * sysClkRateGet();

    for (i = 0; i < maxWaitTicks; i++)
    {

        /*
         * Check to see if there are characters ready in the
         * receive buffer.
         */

        status = ioctl (fd, FIONREAD, (int)&bytesReceived);
        if ( status == ERROR )
        {
            DEBUG(BAR_DBUG_FULL,
          "Barcode: OkToRead: ioctl failed, status = %d\n", status);
        }

        if (bytesReceived)
        {
            break;
        }

        taskDelay(1);
    }

    if (!bytesReceived)
    {
        DEBUG(BAR_DBUG_NONE,
              "Timeout waiting for barcode characters%c\n", ' ');
    }

    DEBUG(BAR_DBUG_MAX,
      "Barcode: OkToRead: bytes received = %d\n", bytesReceived);

    return bytesReceived;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * WriteScanner
 *
 * INVOCATION:
 * status = WriteScanner (fd, scanout);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) fd (int)                  File descriptor for serial device
 * (>) scanout (char *)          String to write
 *
 * FUNCTION VALUE:
 * (long) function status returned.
 *
 * PURPOSE:
 * Write a string to a barcode reader
 *
 * DESCRIPTION:
 * WriteScanner writes a string to the scanner and checks for
 * acknowledgement.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * A serial connection should already have been opened and a file descriptor
 * provided in fd.
 * The scanout string must be shorter than MAX_STRING-4 characters
 *
 * SEE ALSO:
 * None.
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

static long WriteScanner
(
    int     fd,                     /*  file descriptor for serial device   */
    char    *scanout                /*  string to write                     */
)
{
    int     i;                      /*  generic index thing                 */
    char    cmd_string[MAX_STRING]; /*  returned string                     */
    char    lrc[2] = "\0";          /*  calculated checksum character       */
    char    c;                      /*  generic character thing             */


    /* 
     *  add end-of-transmission (ETX) character to end of the given string.
     */

    strncpy(cmd_string, STX, MAX_STRING-4);
    strncat(cmd_string, scanout, MAX_STRING-4);
    strcat(cmd_string, ETX);


    /* 
     *  compute a checksum character for the string and add it to
     *  the end of the string.
     */

    for (i = 1; i < strlen(cmd_string); i++)
    {
       lrc[0] = lrc[0] ^ cmd_string[i];
    }

    strcat(cmd_string, lrc);


    /* 
     *  write the resulting string to the given serial device.
     */

    if (write(fd,cmd_string,strlen(cmd_string)) != strlen(cmd_string))
    {
       DEBUG(BAR_DBUG_NONE, 
             "Barcode scanner error: command write failed.%c\n", ' ');
       return(ERROR);
    }
 
    DEBUG(BAR_DBUG_FULL,"checking for ack%c\n",' '); 


    /*
     *  Then look for the acknowledge character from the barcode reader
     *  which indicates that it received and understood the command.
     */

    if (OkToRead(fd,READ_TIMEOUT))
    {
       if (read(fd, &c, 1) == 1)
       {
          if (c == ACK)
          {
             return(0);
          }

          else
          {
             DEBUG(BAR_DBUG_NONE,
                   "Barcode scanner error: bad acknowledgement,%c.\n",c);
             return(ERROR);
          }
       }
       DEBUG(BAR_DBUG_NONE,
             "Barcode scanner error: failed to readresponse.%c\n",' ');
       return(ERROR);
    }
    else
    {
       DEBUG(BAR_DBUG_NONE,
             "Barcode scanner error: no response.%c\n",' ');
       return(ERROR);
    }
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * ReadScanner
 *
 * INVOCATION:
 * status = ReadScanner (fd, barcode);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) fd  (int)                Serial device file descriptor
 * (<) barcode (char *)         String returned from the barcode reader
 *
 * FUNCTION VALUE:
 * (long) function status returned.
 *
 * PURPOSE:
 * Read a string from the scanner
 *
 * DESCRIPTION:
 * ReadScanner reads a string from the selected barcode scanner.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * A serial connection should already have been opened and a file descriptor
 * provided in fd.
 * The barcode string must be long enough to receive up to MAX_STRING characters
 *
 * SEE ALSO:
 * None.
 *
 * DEFICIENCIES:
 * A string that is too long for the return buffer will overwrite the
 * the buffer ... and only give a warning after the fact.
 *-
 ************************************************************************
 */

static long ReadScanner
(
    int     fd,                 /* serial device file descriptor            */
    char    *barcode            /* barcode returned from the scanner        */
)
{
    char    c[2] = "\0";        /* character buffer                         */
    int     i;                  /* generic index thing                      */
    int    found = FALSE;       /* desired character found flag             */
    int    goodscan = TRUE;     /* scan successful flag                     */


    /*
     *  Clear out the returned barcode buffer
     */

    barcode[0] = '\0';


    /*
     *  If a prefix character has been defined then read stuff from the 
     *  scanner until the prefix character is found (this indicates the start 
     *  of a barcode response).  Normally the prefix is the first character to 
     *  be returned.
     */

    if (PREFIX)
    {
        while (OkToRead(fd,READ_TIMEOUT) && !found)
        {
            if (read(fd, c, 1) == 1)
            {
                if (c[0] == PREFIX)
                {
                    found = TRUE;
                }
                else
                {
                    DEBUG(BAR_DBUG_MIN,
                          "Barcode scanner read %c waiting for prefix.\n",
                          c[0]);
                }
            }
            else
            {
                DEBUG(BAR_DBUG_NONE,
                      "Barcode: read error waiting for prefix.%c\n",' ');
            }
        }

        if (!found)
        {
            DEBUG(BAR_DBUG_NONE,
                  "Barcode scanner: failed to find scan prefix.%c\n",' ');
            goodscan = FALSE;
        }
    }     
   

    /* 
     *  Read characters from the barcode scanner until the manditory
     *  suffix code is detected.
     */

    for (i = 0; goodscan && (i < MAX_STRING) && (c[0] != SUFFIX1); i++)
    {
        if (OkToRead(fd,READ_TIMEOUT))
        {
            if (read(fd, c, 1) == 1)
            {
                if (c[0] != SUFFIX1)
                {
                    strcat(barcode, c);
                    goodscan = TRUE;
                }
            }
            else
            {
                DEBUG(BAR_DBUG_NONE,
                      "Barcode scanner error: failed to read scan.%c\n",' ');
                goodscan = FALSE;
            }
        }
        else
        {
            DEBUG(BAR_DBUG_NONE,
                  "Barcode error: timeout reading scan string.%c\n",' ');
            goodscan = FALSE;
        }
    }

    /* 
     *  String must be less than the return buffer size
     *  [The above loop will ensure this will never happen - SMB]
     */

    if (i >= MAX_STRING)
    {
        DEBUG(BAR_DBUG_NONE,
              "Barcode scanner error: scan string too long, %d.\n",i);
        goodscan = FALSE;
    }
   

    /* 
     *  If a second suffix has been defined then wait for it
     */ 

    if (goodscan && SUFFIX2)
    {
        if (OkToRead(fd,READ_TIMEOUT))
        {
            if (read(fd, c, 1) == 1)
            {
                if (c[0] != SUFFIX2)
                {
                    strcat(barcode, c);
                    goodscan = TRUE;
                }
                else
                {
                    DEBUG(BAR_DBUG_MIN,
                          "Barcode warning: invalid suffix2 %c.\n", c[0]);
                }
            }
            else
            {
                 DEBUG(BAR_DBUG_NONE,
                       "Barcode error: failed to read suffix2.%c\n",' ');
                goodscan = FALSE;
            }
        }
         else
        {
          DEBUG(BAR_DBUG_NONE,
                "Barcode scanner error: timeout reading suffix2.%c\n",' ');
          goodscan = FALSE;
        }
    }
 
    if ( goodscan )
    {
        return (0);
    }
    else
    {
        return (ERROR);
    }
}

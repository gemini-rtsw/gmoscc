static char rcsid[] = "$Id$";
/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) <year>                        (c) <year>
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
 * drvOmsVme
 *               
 * PURPOSE:
 * Handle communications with OMS VME motion control cards
 *
 * FUNCTION NAME(S)
 * drvOmsVmeGetCardType         - return the OMS card type
 * drvOmsVmeGetErrorMessage     - return root (initial) error message
 * drvOmsVmeInit                - initialize driver code and hardware
 * drvOmsVmeIsr                 - handle OMS card interrupts
 * drvOmsVmeMotorPosition       - return motor position
 * drvOmsVmeMotorState          - return state of limit switches
 * drvOmsVmeReadCard            - read a message from an OMS card
 * drvOmsVmeWriteCard           - write a message to an OMS card
 * drvOmsVmeWriteMotor          - write a message to a single axis
 * dumpDebugBuffer              - print recent message transcript
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.6  2000/10/16 22:36:31  gmos
 * Checks for home switch active ('\H') now instead of not active ('\N').
 * Added more status checks, some debugging and more comments.
 *
 * Revision 1.5  2000/10/04 00:30:53  gmos
 * Added more initialization debugging.  Moved max axes definitions
 * to drvOmsVme.h.
 *
 * Revision 1.4  2000/07/17 10:34:19  gmos
 * Extra checks included to guard against memory corruption.
 *
 * Revision 1.3  2000/07/06 17:25:35  gmos
 * Williams new code, tested and debugged
 *
 * Revision 1.2  2000/04/14 10:00:38  gmos
 * Merged pre and post FP software
 *
 * Revision 1.1.1.1  2000/02/29 11:42:26  gmos
 * V1.00 release from HIA
 *
 * Revision 1.10  2000/01/28 20:18:37  wooff
 * Changes to error handling in Isr, MotorPosition, MotorState,
 * WriteCard and WriteMotor.  Also general code cleanup.
 *
 * Revision 1.9  2000/01/12 00:36:58  angelic
 * replaced debugBuffer with readDebugBuffer and writeDebugBuffer
 * so that the information is still in memory when logmessage displays 
 * the contents
 *
 * Revision 1.8  2000/01/10 18:03:30  angelic
 * moved version 1.6.2.1.2.1 to trunk here
 *
 * Revision 1.6.2.1.2.1  1999/10/30 00:18:35  rambold
 * added cleanup for when limit switch clears command buffer
 * for an axis while a command is being sent to the axis.   Also
 * tested for pCard = NULL to determine if card exists.
 *
 * Revision 1.6.2.1  1999/10/07 20:00:53  wooff
 * Added some initialization debugging and cleaned up some spelling mistakes.
 *
 * Revision 1.6.1.1  1999/08/13 19:20:30  angelic
 * added routine to return card type for a given card number
 *
 * Revision 1.6  1999/07/30 18:25:00  angelic
 * changed DRV_OMS_VME_MAX_REPLY_TIME from 2 to 3 clock ticks
 *
 * Revision 1.5  1999/07/28 19:50:51  rambold
 * added error messages
 *
 * Revision 1.4  1999/06/25 17:45:37  angelic
 * added bug fixed by Chris Tierney in drvOmsVmeInit
 *
 * Revision 1.3  1999/06/10 16:49:42  angelic
 * make print statements dependant on debug set to true
 *
 * Revision 1.2  1999/04/28 20:27:01  dunn
 * Bug fixes
 *
 * Revision 1.1  1999/02/26 20:42:04  rambold
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
#include    <sysLib.h>

#include    <vxWorks.h>
#include    <vme.h>
#include    <logLib.h> 
#include    <msgQLib.h>
#include    <rngLib.h>
#include    <semLib.h>
#include    <tickLib.h>

#include    <types.h>
#include    <ctype.h>
#include    <string.h>

#include    <devSup.h>
#include    <devLib.h> 
#include    <errMdef.h>

#include    <devLib.h>

#include    <drvOmsVme.h>


/*
 *  OMS card status register bitmap
 */

#define DRV_OMS_VME_STAT_IRQ               0x80     /* interrupt req active */
#define DRV_OMS_VME_STAT_TRANS_BUF_EMPTY   0x40     /* transmit buffer empty*/
#define DRV_OMS_VME_STAT_INPUT_BUF_FULL    0x20     /* receive buffer full  */
#define DRV_OMS_VME_STAT_DONE              0x10     /* motion is finished   */
#define DRV_OMS_VME_STAT_OVERTRAVEL        0x08     /* limit switch active  */
#define DRV_OMS_VME_STAT_ENCODER           0x04     /* encoder read fault   */
#define DRV_OMS_VME_STAT_INIT              0x02     /* card is initializing */
#define DRV_OMS_VME_STAT_ERROR             0x01     /* unrecognized command */


/*
 *  OMS card control register bitmap
 */

#define DRV_OMS_VME_IRQ_ENABLE             0x80     /* enable interrupts    */
#define DRV_OMS_VME_IRQ_TRANS_BUF          0x40     /* enable xmit interrupt*/
#define DRV_OMS_VME_IRQ_INPUT_BUF          0x20     /* enable read interrupt*/
#define DRV_OMS_VME_IRQ_DONE               0x10     /* enable error/done int*/


/*
 *  System configuration definitions
 */

#define DRV_OMS_VME_ADDRS_TYPE              atVMEA16/* vme addressing type  */
#define DRV_OMS_VME_INTERRUPT_TYPE          intVME  /* interupt source type */
#define DRV_OMS_VME_MEM_SPACE               0x10    /* memory space flag    */   


/*
 *  Setup and communications parameters
 */

#define DRV_OMS_VME_MAX_REPLY_TIME          4       /* read timeout..       */
#define DRV_OMS_VME_MAX_REQUESTS            3       /* max synch attempts   */
#define DRV_OMS_VME_MAX_MESSAGES            16      /* size of message queue*/
#define DRV_OMS_VME_MAX_CARDS               8       /* max cards in a system*/
#define DRV_OMS_VME_CTL_X                   0x18    /* define control-X     */
#define DRV_OMS_VME_CTL_Y                   0x19    /* define control-Y     */


/*
 *  Define the OMS card register addresses
 */

typedef struct
{
    uint8_t unused0;            /* byte alignment                           */
    uint8_t data;               /* data input/output register               */
    uint8_t unused1;            /* byte alignment                           */
    uint8_t done;               /* motion complete and error flag register  */
    uint8_t unused2;            /* byte alignment                           */
    uint8_t control;            /* card control register                    */
    uint8_t unused3;            /* byte alignment                           */
    uint8_t status;             /* card status register                     */
    uint8_t unused4;            /* byte alignment                           */
    uint8_t vector;             /* interrupt vector register                */
    uint8_t unused5[6];         /* byte alignment                           */
} DRV_OMS_VME_REGISTERS;



/*
 *  Define the internal OMS card control structure
 */

typedef struct
{
    long        status;                             /* card status          */
    int         axes;                               /* number of axes       */
    int         type;                               /* card type (8 or 44)  */
    DRV_OMS_VME_REGISTERS *pRegisters;              /* register base address*/
    MSG_Q_ID    readQId;                            /* receive message queue*/
    char        readBuf[DRV_OMS_VME_MAX_MSG_LEN];   /* input buffer         */
    int         charactersRead;                     /* chars read from card */
    char        readDebug[DRV_OMS_VME_MAX_MSG_LEN]; /* read debugging buffer*/
    int         pReadDebug;                         /* read debug buf index */
    SEM_ID      writeMutex;                         /* ring buffer mutex sem*/
    RING_ID     writeBuf;                           /* write msg ring buffer*/
    char        writeDebug[DRV_OMS_VME_MAX_MSG_LEN];/* write debug buffer   */
    int         pWriteDebug;                        /* write debug buf index*/
    } DRV_OMS_VME_CARD;
    
static int lastCard = 0;                            /* number of cards found*/
static DRV_OMS_VME_CARD *pCards[DRV_OMS_VME_MAX_CARDS];     /* one per card */
static char axisName[] = {'X', 'Y', 'Z', 'T', 'U', 'V', 'R', 'S'}; /* prefix*/
static char errorMessage[MAX_STRING_SIZE];          /* error message buffer */
static char readDebugBuffer[DRV_OMS_VME_MAX_MSG_LEN+1];   /* read debugging 
                                      bufffer (with space for null at end)  */
static char writeDebugBuffer[DRV_OMS_VME_MAX_MSG_LEN+1];  /* write debugging 
                                       bufffer (with space for null at end) */


/*
 *  Public debug control word
 */

int drvOmsVmeDebug = 0;             /* enable debug messages (T/F)          */

    
/*
 *  Internal Function Prototypes
 */

static void drvOmsVmeIsr (void *);
static void dumpDebugBuffer (DRV_OMS_VME_CARD *);


/*
 * Macro to save the first (root) error message
 */

#define SET_ERR_MSG(MSG)                                      \
{                                                             \
    if (!strlen (errorMessage))                               \
    strncpy (errorMessage, MSG, MAX_STRING_SIZE - 1);         \
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * drvOmsVmeGetCardType
 *
 * INVOCATION:
 * type = drvOmsVmeGetCardType (int card);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) card (int) OMS card type ( 8 or 44) returned by card
 *
 * FUNCTION VALUE:
 * (int) card type ( or 44 ) or zero if card does not exist.
 *
 * PURPOSE:
 * Return the selected OMS motor driver card type
 *
 * DESCRIPTION:
 * Confirm that card number refers to an existing card
 * and then return the type field of the appropriate card
 * control structure.
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

int drvOmsVmeGetCardType 
(
    int card                /* OMS card number to access                    */
)
{   
    /*
     *  Ensure that the request is valid
     */
     
    if ( (card < 0) || (card >= DRV_OMS_VME_MAX_CARDS) || (pCards[card] == NULL))
    {
        logMsg("drvOmsVme:drvOmsVmeGetCardType: Card type requested from invalid OMS Card #:%d\n", card,0,0,0,0,0);
        SET_ERR_MSG ("OMS type requested from invalid card");
        return 0;
    }


    /*
     *  Return the card type from the card control structure.
     */

    return pCards[card]->type;
}     

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * drvOmsVmeGetErrorMessage
 *
 * INVOCATION:
 * drvOmsVmeGetErrorMessage (message);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (<) message (char *) Error message
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Recover current error message
 *
 * DESCRIPTION:
 * Copy the contents of the error message buffer into the output buffer and
 * then empty the error message buffer.   This allows the next error
 * message generated to be saved in the error message buffer.
 *
 * If debugging is enabled then dump the contents of all communications
 * debugging buffers.
 *
 * Note that the SET_ERR_MSG macro will save the first (root) error message
 * received after the error message buffer is cleared and will keep that
 * message in the buffer until the getErrorMessage function is called to
 * recover it.
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

void drvOmsVmeGetErrorMessage
(
    char *message                   /* error message gets copied here       */
)
{
    int card;                       /* card number for dumping debug buffer */

    /*
     *  Copy the error message into the given output buffer then clear the
     *  error message buffer.
     */ 

    strncpy (message, errorMessage, MAX_STRING_SIZE-1);
    errorMessage[0] = '\0';


    /*
     *  If debugging is enabled then dump the contents of all communications
     *  transcript buffers.
     */

    if (drvOmsVmeDebug)
    {
        for (card = 0; card < lastCard; card++)
        {
            dumpDebugBuffer(pCards[card]);
        }
    }
      
    return;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * drvOmsVmeInit
 *
 * INVOCATION:
 * status = drvOmsVmeInit ()
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * 
 * FUNCTION VALUE:
 * (long) Initialization success code.
 *
 * PURPOSE:
 * Locate and initialize all OMS Vme boards
 *
 * DESCRIPTION:
 * Initialize the OMS vme card driver module using the following algorithm:
 * 
 *      If function has already been called
 *      {
 *          Return immediately since setup has already been done.
 *      }
 *
 *      Initialize the card control pointer structure.
 *
 *      Starting at the first card base address:
 *      While ( something exists at this address )
 *      {
 *          Insure that the address space is free and claim it.
 *          Create a card control struct and save in the control ptr struct.
 *          Create a read message queue.
 *          Create a write ring buffer and protection mutex semaphore.
 *          Initialize critical elements in control structure.
 *          Configure system interrupt handler functions.
 *          Load interrupt vector into the card.
 *          Enable the desired interrupt sources.
 *          Query board type by sending a "WY" string.
 *          Read response, decode and save board type.
 *          Update card type specific control structure elements.
 *          Increment card number and point to next vector and base address.
 *      }
 *
 *      Save the number of cards found.
 *    
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


long drvOmsVmeInit ()
{
    void *base = (void *) DRV_OMS_VME_BASE;     /* base address of card     */ 
    unsigned vector = DRV_OMS_VME_VECTOR;       /* interrupt vector of card */
    void *localAddress;                         /* translated VME address   */
    DRV_OMS_VME_CARD *pCard;                    /* card control structure   */
    DRV_OMS_VME_REGISTERS *pRegisters;          /* register access structure*/ 
    char buffer[DRV_OMS_VME_MAX_MSG_LEN];       /* scratchpad char buffer   */
    char *pFirst;                               /* start of parse string    */
    char *pLast;                                /* end of parse string      */
    long status = 0;                            /* function return status   */


    /* 
     * Insure that this function is only executed once by checking to see
     * if one or more cards have already been initialized.   If so  then
     * there is nothing more to be done.
     */

    errorMessage[MAX_STRING_SIZE-1] = '\0';

    if (lastCard != 0) 
    {
        return status;
    }


    /*
     * Otherwise this is the first invocation ... get on with it
     *
     * Start by initializing the card control structure
     */
      
    for (lastCard=0; lastCard < DRV_OMS_VME_MAX_CARDS; lastCard++)
    {
        pCards[lastCard] = NULL;
    }
                        
    lastCard = 0;


    /* 
     * Then locate all VME boards and create support for each
     */
   
    while (locationProbe(DRV_OMS_VME_ADDRS_TYPE, base) == S_dev_addressOverlap)
    {

        logMsg("drvOmsVme:drvOmsVmeInit: Initializing OMS Card #: %d\n", 
                lastCard,0,0,0,0,0);

        if (lastCard >= DRV_OMS_VME_MAX_CARDS)
        {
            logMsg("drvOmsVme:drvOmsVmeInit: OMS Card #:%d exceeds maximum\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("too many cards found!");
            return (DRV_OMS_VME_S_CFG_ERROR);
        }

                  
        /*
         * Check that the address space is free and claim it
         */
         
        status = devRegisterAddress(__FILE__,
                                    DRV_OMS_VME_ADDRS_TYPE,
                                    base,
                                    DRV_OMS_VME_MEM_SPACE,
                                    &localAddress);
        if (status)
        {
            logMsg("drvOmsVme:drvOmsVmeInit: OMS Card #:%d cannot be registered\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("Cannot register OMS card");
            return (DRV_OMS_VME_S_SYS_ERROR);
        }
              
              
        /*
         * Create and save the interface card control structure
         */

        pCard = (DRV_OMS_VME_CARD *) malloc (sizeof (DRV_OMS_VME_CARD));
        if (!pCard)
        {
            logMsg("drvOmsVme:drvOmsVmeInit: Cannot create card control structure for OMS Card #:%d\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("Cannot create OMS card control structure");
            return (DRV_OMS_VME_S_SYS_ERROR);
        }
            
        pCards[lastCard] = pCard;
                           
                           
        /*
         * Create a message queue to receive strings from the card
         */

        pCard->readQId = msgQCreate (DRV_OMS_VME_MAX_MESSAGES,
                                     DRV_OMS_VME_MAX_MSG_LEN,
                                     MSG_Q_PRIORITY); 
        if (pCard->readQId == NULL)
        {
            logMsg("drvOmsVme:drvOmsVmeInit: Cannot create message queue for OMS Card #:%d\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("Cannot create OMS read queue");
            return (DRV_OMS_VME_S_SYS_ERROR);
        }
        

        /*
         *  Create ring buffer to send strings to the card and a mutual
         *  exclusion semaphore to protect it from asynchronous access.
         */

        pCard->writeBuf = 
               rngCreate (DRV_OMS_VME_MAX_MSG_LEN * DRV_OMS_VME_MAX_MESSAGES);
        if (pCard->writeBuf == NULL)
        {
            logMsg("drvOmsVme:drvOmsVmeInit: Cannot create write buffer for OMS Card #:%d\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("Cannot create OMS write buffer");
            return (DRV_OMS_VME_S_SYS_ERROR);
        }

        pCard->writeMutex = semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
        if (pCard->writeMutex == NULL)
        {
            logMsg("drvOmsVme:drvOmsVmeInit: Cannot create write semaphore for OMS Card #:%d\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("Cannot create OMS write semaphore");
            return (DRV_OMS_VME_S_SYS_ERROR);
        }
       
        
        /*
         * Intialize the interface control structure
         */
         
        pRegisters = (DRV_OMS_VME_REGISTERS *) localAddress;
        pRegisters->control = 0;        
        pCard->pRegisters = pRegisters;
        pCard->charactersRead = 0; 
        pCard->axes = 0;
        
        
        /* 
         * Set up interrupt handler and vector stuff
         */

        status = devConnectInterrupt(DRV_OMS_VME_INTERRUPT_TYPE,
                                     vector,
                                     &drvOmsVmeIsr,
                                     (void *) pCard);

        if (status)
        {
            logMsg("drvOmsVme:drvOmsVmeInit: Cannot connect interrupt for OMS Card #:%d\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("Cannot connect OMS interrupt");
            pRegisters->control = 0;
            return (DRV_OMS_VME_S_SYS_ERROR);
        }

        status = devEnableInterruptLevel (DRV_OMS_VME_INTERRUPT_TYPE,
                                          DRV_OMS_VME_INTERRUPT);
        if (status)
        {
            logMsg("drvOmsVme:drvOmsVmeInit: Cannot enable interrupts for OMS Card #:%d\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("Cannot enable OMS interrupts");
            pRegisters->control = 0;
            return (DRV_OMS_VME_S_SYS_ERROR);
        }


        /*
         * Set the interrupt vector.
         */
         
        pRegisters->vector = vector;


        /* 
         * Enable interrupt-when-done and input-buffer-full interrupts. 
         */
         
        pRegisters->control = DRV_OMS_VME_IRQ_ENABLE |  
                              DRV_OMS_VME_IRQ_INPUT_BUF |
                              DRV_OMS_VME_IRQ_DONE;

        
        /*
         *  Clear out any pending messages by reading the card 
         *  and discarding messages until its buffer is empty.
         */

        while (drvOmsVmeReadCard (lastCard, buffer, NO_WAIT) > 0)
        {
            ;
        }


        /*
         *  Ensure echo is off and query the board for its type.  This will
         *  also confirm that the setup was done correctly and we can
         *  communicate with it.
         */
         
        status = drvOmsVmeWriteCard (lastCard, "EFWY");
        if (status)
        {
            logMsg("drvOmsVme:drvOmsVmeInit: Initialization write failure for OMS Card #:%d\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("OMS initialization write failure");
            pRegisters->control = 0;
            return (DRV_OMS_VME_S_SYS_ERROR);
        }


        /*
         *  Read the return string and try to parse the card type from
         *  the type return message.
         */

        while ((status = drvOmsVmeReadCard (lastCard, 
                                            buffer, 
                                            DRV_OMS_VME_MAX_REPLY_TIME)) > 0)
        {
            /*
             *  Scan return string for first numeric character
             */

            for (pFirst = buffer; 
                 !isdigit(*pFirst) && *pFirst != '\0'; 
                 pFirst++)
            {
                ;
            }


            /* 
             *  If found this should be the card type number
             */

            if (isdigit(*pFirst))
            {        
                pCard->type = strtod (pFirst, &pLast);
                if (*pLast != '\0')
                {
                    logMsg("drvOmsVme:drvOmsVmeInit: Cannot determine board type for OMS Card #:%d\n", lastCard,0,0,0,0,0);
                    SET_ERR_MSG ("Cannot determine OMS board type");
                    pRegisters->control = 0;
                    return (DRV_OMS_VME_S_PARSE_ERROR);
                } 
                break;  /* exit while loop */
            }
        }
            
        if (status < 0)
        {
            logMsg("drvOmsVme:drvOmsVmeInit: Cannot read initialization response for OMS Card #:%d\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("Cannot read OMS initialization response");
            pRegisters->control = 0;
            return (DRV_OMS_VME_S_SYS_ERROR);
        }


        /*
         * Do any card type specific setup here
         */
                              
        switch (pCard->type)
        {
        case 8:
            pCard->axes = DRV_OMS_VME_8_MAX_AXES;
            break;

        case 44:
            pCard->axes = DRV_OMS_VME_44_MAX_AXES;
            break;

        default:
            logMsg("drvOmsVme:drvOmsVmeInit: Unrecognized board type for OMS Card #:%d\n", 
                    lastCard,0,0,0,0,0);
            SET_ERR_MSG ("Unrecognized OMS board type");
            pRegisters->control = 0;
            return (DRV_OMS_VME_S_PARSE_ERROR);
            break;
        }
        

        /*
         *  If debugging is enabled then log the configuraton parameters for
         *  this card.
         */

        if (drvOmsVmeDebug) 
        {
            logMsg("drvOmsVme: %x v=%d s=%x t=%d\n", 
                   (int) localAddress, 
                   vector, 
                   (int) pCard, 
                   pCard->type, 0, 0);
        }


        /*
         *  Support for this card is complete.   Point to the next card, 
         *  next interrupt vector and next base address then go back and 
         *  see if this card exists.
         */

        lastCard++;
        vector = vector++;
        base = (void *) ((char *) base + DRV_OMS_VME_MEM_SPACE);
    }


    if (pCards[0] == NULL)
    {
        logMsg("drvOmsVme:No OMS interface cards found!!!\n",0,0,0,0,0,0);
    }

    return OK;
}




/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * drvOmsVmeIsr
 *
 * INVOCATION:
 * drvOmsVmeIsr (pCard)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pCard (void *) Pointer to card control structure
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Interrupt handler for OMS VME motion control cards
 *
 * DESCRIPTION:
 * Service all of the possible interrupt generating sources
 * using the following algorithm:
 *
 *
 *      Read the contents of the hardware communication registers
 *        (except for the done register) 
 *
 *      If status register done bit is set indicates motion done or error
 *      {
 *          If status register overtravel (soft limit bit) is set
 *          {
 *              Send a debug message if we are debugging.
 *          }
 *          If status register command error bit is set
 *          {
 *              Shouldn't happen so send a debug message.
 *          }            
 *          If status register initializing or encoder error bits are set.
 *          {
 *              Shouldn't happen so send a debug message.
 *          }
 *          If none of the above,  
 *          {
 *              No errors so must mean motion is done.
 *          }
 *          Else
 *          {
 *              Issue a CTL-X to reset the status register error bits
 *                leaving the done register as it was.
 *          }
 *      }
 *
 *      If input buffer full bit was set in the ISR.
 *      {
 *          If character in data register is non-whitespace.
 *          {
 *              if input buffer is not too large
 *              {
 *                  Increment input buffer and add character.
 *              }
 *              else input buffer is too large
 *              {
 *                  Error - input buffer overflow
 *              }
 *          }
 *          else if whitespace and input buffer is not empty.
 *          {
 *              Message complete, send it to the message queue.
 *              Reset input buffer.
 *          }
 *          else whitespace and input buffer still empty 
 *          {
 *              Reset input buffer.
 *          }
 *      }
 *
 *      If transmit buffer empty bit was set in the ISR.
 *      {
 *          If there is a character available on the ring buffer
 *          {
 *              Write character to card
 *          }
 *          else
 *          {
 *              Disable interrupt.
 *          }
 *      }
 *
 * Note that the transmit buffer full interrupt is disabled until a message
 * is loaded into the ring buffer.  At this time the interrupt is enabled 
 * causing the card to generate an interrupt immediately which invokes the isr
 * to transfer the characters to the card.  As soon as the ring buffer empties
 * the interrupt is disabled again until a new message is ready.
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
 * Errors are detected but not handled in any way.
 *-
 ************************************************************************
 */


static void drvOmsVmeIsr 
(
    void *pIsr              /* pointer passed by the system for this isr   */
)
{   
    uint8_t data;           /* contents of data register                    */
/*  uint8_t done;              contents of done/error register              */
    uint8_t intStatus;      /* contents of interrupt status register        */
    uint8_t control;        /* contents of control register                 */
    char writeChar;         /* character to write to card                   */
    register int fromP;     /* ring buffer location return pointer          */
    DRV_OMS_VME_CARD *pCard = (DRV_OMS_VME_CARD *) pIsr; /* card control 
                                                                  structure */
    DRV_OMS_VME_REGISTERS *pRegisters = pCard->pRegisters; /* register 
                                                                  structure */
    long status = 0;        /* function return status                       */

     
    /*
     * Read the contents of the OMS status, data & control registers.
     * Instead of reading the done register here and resetting the individual 
     * axis done flags, reset them individually with the IC command in 
     * drvOmsVmeMotorState().
     */

    intStatus = pRegisters->status;
    data = pRegisters->data; 
    control = pRegisters->control;


    /*
     *  Done (or error) handler.  The DON_S bit can mean either an error
     *  has caused the interrupt or that a motion has completed.
     */
         
    if (intStatus & DRV_OMS_VME_STAT_DONE)
    {

        /*
         *  Check for errors first...
         *
         *  Has a soft limit switch has been hit?
         */

        if (intStatus & DRV_OMS_VME_STAT_OVERTRAVEL)
        {
            
            /*
             *  Send a debug message if we are debugging otherwise ignore
             *  the interrupt.  A limit may or may not actually be an error.
             */

            if (drvOmsVmeDebug)
            {
                logMsg("OMS%d card: Limit detected, control=%d, status=%d\n", 
                        pCard->type, control, intStatus, 0, 0, 0);
            }
        }


        /*
         *  Did the card reject the command?  This could be the result of 
         *  hitting a limit switch which immediately empties the input buffer 
         *  for that axis.  This may erase part of a command string if the 
         *  system was writing to the card at that time.
         */

        if (intStatus & DRV_OMS_VME_STAT_ERROR)
        {
            /*
             *  If so, send a debug message.
             */

            logMsg("OMS%d card: Command rejected, control=%d, status=%d\n", 
                  pCard->type, control, intStatus, 0, 0, 0);
            dumpDebugBuffer (pCard);
        }


        /*
         *  Is the card OMS initializing (not likely), or feels it has an
         *  encoder problem (also not likely)?   
         */

        if ( (intStatus & DRV_OMS_VME_STAT_INIT) || 
             (intStatus & DRV_OMS_VME_STAT_ENCODER) )
        {
            /*
             *  If so, send a debug message.
             */
 
            logMsg("OMS%d card: Init or encoder error\n", 
                pCard->type, 0, 0, 0, 0, 0);
        }


        /*
         *  If none of the above then done bit means motion has
         *  completed on at least one axis - otherwise reset the   
         *  status register error bits by issuing a CTL-X.
         */

        if ( !(intStatus & DRV_OMS_VME_STAT_OVERTRAVEL) &&
             !(intStatus & DRV_OMS_VME_STAT_ERROR) &&
             !(intStatus & DRV_OMS_VME_STAT_INIT) &&
             !(intStatus & DRV_OMS_VME_STAT_ENCODER) )
        {
            if (drvOmsVmeDebug)
            {
                logMsg("OMS%d card: Axis motion done.\n", 
                        pCard->type, 0, 0, 0, 0, 0);
            }
        }
        else
        {
            if (drvOmsVmeDebug)
            {
                logMsg("OMS%d card: Resetting status register error bits:%d\n", 
                        pCard->type, intStatus, 0, 0, 0, 0);
            }
            pRegisters->data = DRV_OMS_VME_CTL_X;

        }

    }


    /*
     *  Input buffer full (next char available for reading) handler
     */
     
    if (intStatus & DRV_OMS_VME_STAT_INPUT_BUF_FULL)
    {
        /*
         *  Add non-whitespace characters to the receive buffer.  If debugging
         *  is enabled then save the input in the debug buffer as well.
         */

        if (!isspace(data))
        {
            if (pCard->charactersRead < DRV_OMS_VME_MAX_MSG_LEN) 
            {   
                pCard->readBuf[pCard->charactersRead++] = data;     
                if (drvOmsVmeDebug)
                {
                    pCard->readDebug[pCard->pReadDebug & 0x3f] = data;
                    pCard->pReadDebug++;
                }
            }
            else
            {
                SET_ERR_MSG ("OMS read buffer overflow");
                logMsg("OMS%d card .. OMS read buffer overflow\n", 
                     pCard->type, 0, 0, 0, 0, 0);
                pCard->status = DRV_OMS_VME_S_BUFFER_FULL;
            }
        }


        /*
         *  The first white space following one or more non-whitespaces 
         *  signals the end of a response string.
         */

        else if (pCard->charactersRead)
        {
            /*
             *  Message complete, add a null termination and put it on
             *  the message queue.
             */

            pCard->readBuf[pCard->charactersRead++] = '\0';
            status = msgQSend (pCard->readQId, 
                               pCard->readBuf, 
                               pCard->charactersRead,
                               NO_WAIT,
                               MSG_PRI_NORMAL);
            if (status)
            {
                SET_ERR_MSG ("Cannot send to OMS message queue");
                logMsg("OMS%d card .. OMS message queue send failure\n", 
                     pCard->type, 0, 0, 0, 0, 0);
                pCard->status = DRV_OMS_VME_S_SYS_ERROR;
            }
            

            /*
             *  Flush the input buffer and start again
             */

            pCard->charactersRead = 0;
        }


        /*
         *  All other whitespaces are ignored
         */

        else
        {
            pCard->charactersRead = 0;
        }
    } 
     
                
    /*
     *  Transmit buffer empty (ready for next input character) handler
     */
     
    if (intStatus & DRV_OMS_VME_STAT_TRANS_BUF_EMPTY)
    {
        /*
         *  If there is a character available on the ring buffer send
         *  it to the card by writing to the data register.  If debugging
         *  is enabled then save the character to the write debug buffer
         *  as a record that the character was sent.
         */
 
        if (RNG_ELEM_GET(pCard->writeBuf, &writeChar, fromP))
        {             
            pRegisters->data = writeChar;       
  
            if (drvOmsVmeDebug)
            {
                pCard->writeDebug[pCard->pWriteDebug & 0x3f] = writeChar;
                pCard->pWriteDebug++;
            }
        }

        /*  
         *  If the ring buffer is empty the command has been sent so
         *  disable the transmit buffer empty interrupt.  It will be 
         *  re-enabled when a new message is ready to be sent.
         */

        else
        {
            pRegisters->control &= ~DRV_OMS_VME_IRQ_TRANS_BUF;
        }
    }
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * drvOmsVmeMotorPosition
 *
 * INVOCATION:
 * status = drvOmsVmeMotorPosition (card, axis, &position, &encoder)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) card (int) card number to be queried
 * (>) axis (int) axis number to be queried
 * (<) position (long *) current step position counter value
 * (<) position (long *) current encoder value
 * 
 * FUNCTION VALUE:
 * (long) Function status return.
 *
 * PURPOSE:
 * Request current encoder and position from a given axes
 *
 * DESCRIPTION:
 * 
 *      Ensure request is valid.
 *      Flush the message queue.
 *      If this is an OMS 44 card
 *      {
 *          Try up to a defined maximum number of times
 *          {
 *              Send the "Report Encoder" request and exit if error.
 *              Read card response.
 *              If response can be converted to long integer
 *              {
 *                  Break out of read encoder loop
 *              }
 *              Print debug information
 *          }    
 *          If tried the maximum number without success
 *          {
 *              Exit function with an error
 *          }
 *      }
 *      Try up to a defined maximum number of times
 *      {
 *          Send the "Report Position" request and exit if error.
 *          Read card response.
 *          If response can be converted to long integer
 *          {
 *              Break out of read position loop
 *          }
 *          Print debug information
 *      }    
 *      If tried the maximum number without success
 *      {
 *          Exit function with an error
 *      }
 * 
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


long drvOmsVmeMotorPosition 
(
    int card,           /* card number to query                         */ 
    int axis,           /* axis number to query                         */
    long *position,     /* current position counter value               */
    long *encoder       /* current encoder value                        */
)
{
    char buffer[DRV_OMS_VME_MAX_MSG_LEN];   /* scratchpad buffer        */
    char *pEnd;                 /* end pointer for strtol               */
    DRV_OMS_VME_CARD *pCard;    /* card control structure               */ 
    int lostReply;              /* number of discarded replies          */
    int requests;               /* number of times query was repeated   */
    long status;                /* function return status               */ 
        
                 
    /*
     * Ensure that the request is valid
     */
     
    if ( (card < 0) || 
         (card >= DRV_OMS_VME_MAX_CARDS) || 
         ((pCard = pCards[card]) == NULL))
    {
        SET_ERR_MSG ("OMS position requested from invalid card");
        return DRV_OMS_VME_S_CFG_ERROR;
    }
          
    /*
     * Clear out any pending messages
     */                 

    for (lostReply = 0; 
         (drvOmsVmeReadCard (card, buffer, NO_WAIT)) > 0; 
         lostReply++)
    {
        ;
    }
        
    if (lostReply)
    {
        logMsg("OMS card:%d  Flushed %d messages from input queue before RE/RP.\n", 
             card, lostReply, 0, 0, 0, 0);
    }


    /*
     * OMS 44 cards also have an encoder .. read this as well
     * Note that the OMS Interface cards have this very nasty habit of flushing 
     * the input buffer for any axis that hits a soft limit!   This means that 
     * an encoder request can get lost or garbled if the switch is hit 
     * immediately after request string has been written but before it has 
     * recognized the command.   It may take a few attempts to get the 
     * input queue for that axis sorted out again.
     */

    if (pCard->type == 44)
    {
        for (requests = 1; requests < DRV_OMS_VME_MAX_REQUESTS; requests++)
        {
            /*
             *  Clear any existing errors encountered in ISR for input buffer   
             */

            pCard->status = 0;

            /*
             *  Write the "report encoder" command to the axis
             */

            status = drvOmsVmeWriteMotor (card, axis, "RE");
            if (status != 0)
            {

                logMsg("OMS card:%d RE cmd writeMotor failed %d:%d\n", 
                     card, status, pCard->status, 0, 0, 0);
                return status;
            }

            /*
             *  Read the card's response to the request and try to convert
             *  it to a long integer.
             */

            status = drvOmsVmeReadCard (card, 
                                        buffer, 
                                        DRV_OMS_VME_MAX_REPLY_TIME);

            *encoder = strtol (buffer, &pEnd, 10);


            /*
             *  If conversion was successful break out of RE query loop here
             */

            if (status >= 0 && pCard->status == 0 && *pEnd == '\0')
            {
                break;
            }

            /*
             *  Otherwise an error occurred.   Write debugging information
             *  and then go back and try again.
             */

            if ( *pEnd != '\0' )
            {
                logMsg("OMS card:%d RE bad string termination:%c, %d/%d\n", 
                    card, *pEnd, requests, 
                    DRV_OMS_VME_MAX_REQUESTS, 0, 0);
            }
            if ( pCard->status != 0 )
            {
                logMsg("OMS card:%d RE status register error:%d, %d/%d\n", 
                    card, pCard->status, requests, 
                    DRV_OMS_VME_MAX_REQUESTS, 0, 0);
            }
            if ( status < 0 )
            {
                logMsg("OMS card:%d RE readCard error:%d, %d/%d\n", 
                    card, status, requests,
                    DRV_OMS_VME_MAX_REQUESTS, 0, 0);
            }
            dumpDebugBuffer (pCard);

        }     /* end of RE query loop */


        /*
         * Either successful RE query or too many attempts. 
         * Check to see how many tries it took.
         */

        if ( requests > 1 )
        {
            /* 
             * This was not the first attempt.
             */
            if (requests >= DRV_OMS_VME_MAX_REQUESTS)
            {
                /*
                 * Too many attempts to read the encoder have failed, 
                 * so bail out with an error.
                 */
                SET_ERR_MSG ("OMS44 Encoder request read failure");
                logMsg("OMS card:%d RE cmd failed after %d attempts, %d:%d\n", 
                     card, requests, status, pCard->status, 0, 0);
                return DRV_OMS_VME_S_SYS_ERROR;
            } 
            /*
             * Otherwise this is the first successful read after n
             * failures, so if debugging is set just dump the read 
             * and write buffers and carry on.
             */
            else if (drvOmsVmeDebug)
            {
                logMsg("OMS card:%d RE command accepted on attempt:%d of %d\n", 
                        card, requests, DRV_OMS_VME_MAX_REQUESTS, 0, 0, 0);
                dumpDebugBuffer (pCard);
            }
        }
    }


    /*
     * Ask the OMS VME card for the current position.
     * Note that the OMS Interface cards have this very nasty habit of flushing 
     * the input buffer for any axis that hits a soft limit!   This means that 
     * a position request can get lost or garbled if the switch is hit 
     * immediately after request string has been written but before it has 
     * recognized the command.   It may take up to three attempts to get the 
     * input queue for that axis sorted out again.
     */

    for (requests = 1; requests < DRV_OMS_VME_MAX_REQUESTS; requests++)
    {
        /*
         *  Clear any existing errors encountered in ISR for input buffer   
         */

        pCard->status = 0;

        /*
         *  Write the "report position" command to the axis
         */

        status = drvOmsVmeWriteMotor (card, axis, "RP");
        if (status != 0)
        {
            logMsg("OMS card:%d RP cmd writeMotor failed %d:%d\n", 
                 card, status, pCard->status, 0, 0, 0);
            return status;
        }


        /*
         *  Read the card's response to the request and try to convert
         *  it to a long integer.
         */

        status = drvOmsVmeReadCard (card, 
                                    buffer, 
                                    DRV_OMS_VME_MAX_REPLY_TIME);

        *position = strtol (buffer, &pEnd, 10);


        /*
         *  If conversion was successful break out here
         */

        if (status >= 0 && pCard->status == 0 && *pEnd == '\0')
        {
            break;
        }
          
        /*
         *  Otherwise an error occurred.   Write debugging information
         *  and then go back and try again.
         */

        if ( *pEnd != '\0' )
        {
            logMsg("OMS card:%d RP bad string termination:%c,  %d/%d\n", 
                 card, *pEnd, requests, 
                 DRV_OMS_VME_MAX_REQUESTS, 0, 0);
        }
        if ( pCard->status != 0 )
        {
            logMsg("OMS card:%d RP status register error:%d, %d/%d\n", 
                 card, pCard->status, requests, 
                 DRV_OMS_VME_MAX_REQUESTS, 0, 0);
        }
        if ( status < 0 )
        {
            logMsg("OMS card:%d RP readCard error:%d, %d/%d\n", 
                 card, status, requests,
                 DRV_OMS_VME_MAX_REQUESTS, 0, 0);
        }
        dumpDebugBuffer (pCard);

    }     /* end of RP query loop */

    /*
     * Either successful RP query or too many attempts. 
     * Check to see how many tries it took.
     */

    if ( requests > 1 )
    {
        /* 
         * This was not the first attempt.
         */
        if (requests >= DRV_OMS_VME_MAX_REQUESTS)
        {
            /* 
             * Too many attempts to read the position have failed, 
             * so bail out with an error.
             */
            SET_ERR_MSG ("OMS Position read failure");
            logMsg("OMS card:%d RP cmd failed after %d attempts, %d:%d\n", 
                 card, requests, status, pCard->status, 0, 0);
            return DRV_OMS_VME_S_SYS_ERROR;
        } 
        /*
         * Otherwise this is the first successful read after n failures, 
         * so if debugging set just dump the read and write buffers
         * and carry on.
         */
        else if (drvOmsVmeDebug)
        {
            logMsg("OMS card:%d RP command accepted on attempt:%d of %d\n", 
                        card, requests, DRV_OMS_VME_MAX_REQUESTS, 0, 0, 0);
            dumpDebugBuffer (pCard);
        }
    }

    return OK;
}
       

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * drvOmsVmeMotorState
 *
 * INVOCATION:
 * status = drvOmsVmeMotorState (card, axis, &lowLimit, &highLimit, &homeSwitch)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) card       (int)   card to query
 * (>) axis       (int)   axis to query
 * (<) lowLimit   (int *) State of low limit switch
 * (<) highLimit  (int *) State of high limit switch
 * (<) homeSwitch (int *) State of home switch
 * (<) axisDone   (int *) State of axis done bit
 *
 * FUNCTION VALUE:
 * (long) function status return.
 *
 * PURPOSE:
 * Request current switch status for a given axes
 *
 * DESCRIPTION:
 * Query the given card to determine whether motion is done and the state of 
 * the home and limit switches associated with the given axes using the
 * following algorithm:
 *
 *      Ensure request is valid.
 *      Flush the message queue.
 *      Try up to a defined maximum number of times
 *      {
 *          Send the "Query Axis" request and exit if error.
 *          Read card response.
 *          If direction character of response is valid
 *          {
 *              If done
 *              {
 *                  Set local axisDone flag
 *                  Send CA string to clear done bit for this axis only
 *              }
 *              Else clear axisDone flag
 *
 *              Convert rest of response to switch values
 *              Break out of QA query loop
 *          }
 *          Print debug information
 *          Exit with error
 *          
 *      }    
 *      If tried the maximum number without success
 *      {
 *          Exit function with an error
 *      }
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


long drvOmsVmeMotorState 
(
int card,                       /*  card to query                       */ 
int axis,                       /*  axis to query                       */
int *lowLimit,                  /*  state of lower limit switch         */
int *highLimit,                 /*  state of upper limit switch         */
int *homeSwitch,                /*  state of home switch                */
int *axisDone                   /*  state of axis done flag             */
)
{
    char buffer[DRV_OMS_VME_MAX_MSG_LEN];   /* scratchpad buffer        */
    DRV_OMS_VME_CARD *pCard;    /* card control structure               */ 
    int lostReply;              /* number of discarded replies          */
    int requests;               /* number of times query was repeated   */
    long status;                /* function return status               */ 
         
                 
    /*
     * Ensure that the request is valid
     */
     
    if ( (card < 0) || 
         (card >= DRV_OMS_VME_MAX_CARDS) || 
         ((pCard = pCards[card]) == NULL))
    {
        SET_ERR_MSG ("OMS motor status requested from invalid card");
        return DRV_OMS_VME_S_CFG_ERROR;
    }

    pCard = pCards[card];  
          

    /*
     *  Flush the input message queue
     */                 

    for (lostReply = 0;
         (drvOmsVmeReadCard (card, buffer, NO_WAIT)) > 0;
         lostReply++)
    {
        ;
    }
        
    if (lostReply)
    {
        logMsg("OMS card:%d  Flushed %d messages from input queue before QA.\n",
             card , lostReply, 0, 0, 0, 0);
    }


    /*
     * Ask the OMS VME card for the current state
     * Note that the OMS Interface cards have this very nasty habit of flushing 
     * the input buffer for any axis that hits a soft limit!   This means that 
     * a state request can get lost or garbled if the switch is hit immediately 
     * after request string has been written but before it has recognized the 
     * command.   It may take up to three attempts to get the input queue for 
     * that axis sorted out again.
     */
    
    for (requests = 1; requests < DRV_OMS_VME_MAX_REQUESTS; requests++)
    {
        /*
         *  Clear any existing errors encountered in ISR for input buffer   
         */

        pCard->status = 0;

        /*
         *  Send the Query Axis command to the desired card and axis
         */

        status = drvOmsVmeWriteMotor (card, axis, "QA");
        if (status != 0)
        {
            logMsg("OMS card:%d QA cmd writeMotor failed %d:%d\n", 
                 card, status, pCard->status, 0, 0, 0);
            return status;
        }


        /*
         *  Read the response from the card
         */

        status= drvOmsVmeReadCard (card, 
                                   buffer, 
                                   DRV_OMS_VME_MAX_REPLY_TIME);

        if (status >= 0 && pCard->status == 0 )
        {
            /*
             *  Check the buffer to see if it is a valid return
             *
             *     [0] == M  axis moving in a negative direction
             *     [0] == P  axis moving in a positive direction
             *     [1] == D  axis motion is done
             *     [1] == N  axis motion either not done or not setup for it
             *     [2] == L  axis in limit for direction [0]
             *     [2] == N  axis not in limit for direction [0]
             *     [3] == H  home switch active 
             *     [3] == N  home switch not active
             */

            if ( (buffer[0] == 'M' || buffer[0] == 'P') &&
                 (buffer[1] == 'D' || buffer[1] == 'N') &&
                 (buffer[2] == 'L' || buffer[2] == 'N') &&
                 (buffer[3] == 'H' || buffer[3] == 'N') &&
                  buffer[4] == '\0'                        )
            {
                /*
                 *  Response is valid, extract done flag and switch 
                 *  info then exit read loop.
                 */

                if (buffer[1] == 'D')
                {
                    /* axis motion has completed */
                    *axisDone = 1;

                    /* clear done flag for this axis only */
                    status = drvOmsVmeWriteMotor (card, axis, "CA");
                    if (status != 0)
                    {
                        logMsg("OMS card:%d CA cmd writeMotor failed %d:%d\n", 
                             card, status, pCard->status, 0, 0, 0);
                        return status;
                    }
                }
                else if (buffer[1] == 'N')
                {
                    /* axis motion not complete or not enabled with ID */
                    *axisDone = 0;
                }

                *lowLimit =   (int) (buffer[2] == 'L' && buffer[0] == 'M');
                *highLimit =  (int) (buffer[2] == 'L' && buffer[0] == 'P'); 
                *homeSwitch = (int) (buffer[3] == 'H');

                break;            /* get out of query axis loop */
            }

            /*
             *  Invalid character returned by QA
             */

            else
            {
                SET_ERR_MSG ("OMS State request error");
                if (drvOmsVmeDebug)
                {
                    logMsg("OMS card:%d QA cmd retured bad character:%c\n", 
                            card, buffer[0], 0, 0, 0, 0);
                }
                return DRV_OMS_VME_S_SYS_ERROR;
            } 

        }    /* end if status...  */

        /*
         * Else dump the read and write buffers whenever the read request fails.
         */

        if ( buffer[4] != '\0' )
        {
            logMsg("OMS card:%d QA bad string termination:%c,  %d/%d\n", 
                 card, buffer[4], requests, 
                 DRV_OMS_VME_MAX_REQUESTS, 0, 0);
        }
        if ( pCard->status != 0 )
        {
            logMsg("OMS card:%d QA status register error:%d,  %d/%d\n", 
                 card, pCard->status, requests, 
                 DRV_OMS_VME_MAX_REQUESTS, 0, 0);
        }
        if ( status < 0 )
        {
            logMsg("OMS card:%d QA readCard error:%d, %d/%d\n", 
                 card, status, requests,
                 DRV_OMS_VME_MAX_REQUESTS, 0, 0);
        }
        dumpDebugBuffer (pCard);

    }    /*   end of query axis loop  */ 

    /*
     * Either successful QA query or too many attempts. 
     * Check to see how many tries it took.
     */

    if ( requests > 1 )
    {
        /* 
         * This was not the first attempt. 
         */

        if (requests >= DRV_OMS_VME_MAX_REQUESTS)
        {
            /* 
             * Too many attempts to read the status have failed, 
             * so bail out with an error.
             */

            SET_ERR_MSG ("OMS State request read failure");
            logMsg("OMS card:%d QA cmd failed after %d attempts, %d:%d\n", 
                 card, requests, status, pCard->status, 0, 0);
            return DRV_OMS_VME_S_SYS_ERROR;
        } 
        else if (drvOmsVmeDebug)
        {
            /*
             * Otherwise this is the first successful read after n failures, 
             * so if debugging set, just dump the read and write buffers
             * and carry on.
             */

            logMsg("OMS card:%d QA command accepted on attempt:%d of %d\n", 
                  card, requests, DRV_OMS_VME_MAX_REQUESTS, 0, 0, 0);
            dumpDebugBuffer (pCard);
        }
    }
                           
    return OK;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * drvOmsVmeReadCard
 *
 * INVOCATION:
 * status - drvOmsVmeReadCard (card, buffer, timeout);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) card     (int)    Card to read from
 * (<) buffer   (char *) String returned by card
 * (>) timeout  (int)    Maximum time to wait for response (sys ticks)
 *
 * FUNCTION VALUE:
 * (long) read function success.
 *
 * PURPOSE:
 * Recover data read from an OMS VME card
 *
 * DESCRIPTION:
 * Insure that a valid card number has been supplied.
 * If the timeout is zero check to see if there are messages ready
 * and return 0 immmediately if the read queue is empty.
 * Wait at the message queue until a complete string has been read
 * from the card.
 * If the card takes too long to respond then return an error.
 * Otherwise returns number of bytes copied to buffer
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * buffer should be large enough to accept DRV_OMS_VME_MAX_MSG_LEN
 * characters.
 *
 * SEE ALSO:
 * - other function name.
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */


long drvOmsVmeReadCard 
(
    int card,               /* Card number to read                          */ 
    char *buffer,           /* String returned from card                    */
    int timeout             /* Maximum time to wait for response            */
)
{
    DRV_OMS_VME_CARD *pCard; 
    long status = 0;

    /*
     * Ensure that the request is valid
     */
     
    if ((pCard = pCards[card]) == NULL)
    {
        SET_ERR_MSG ("Read from invalid OMS card");
        return DRV_OMS_VME_S_CFG_ERROR;
    }

    pCard = pCards[card];            
    
            
    /*
     * No timeout means simply check for messages. This is 
     * used when the object is to clear the message queue.
     */
     
    if (!timeout && !msgQNumMsgs (pCard->readQId))
    {
        *buffer = '\0';
        return 0;
    }
    
    /*
     * Wait for receipt of a message
     */
  
    status = msgQReceive (pCard->readQId,
                          buffer,
                          DRV_OMS_VME_MAX_MSG_LEN,
                          timeout);

    /*
     * Check to see if msgQReceive failed and why.
     * Could be ID error, deleted, unavailable,
     * timeout, invalid length.
     */

    if (status < 0)
    {
        logMsg("OMS card:%d readCard failed, %d:%d\n", 
             card, status, pCard->status, 0, 0, 0);
        return DRV_OMS_VME_S_NO_REPLY; 
    }
      
    return status;
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * drvOmsWriteCard
 *
 * INVOCATION:
 * status = drvOmsVmeWriteCard (card, buffer);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) card   (int)     Card number to write to
 * (>) buffer (char *)  String to be written 
 *
 * FUNCTION VALUE:
 * (long) write function success.
 *
 * PURPOSE:
 * Write a message to an OMS VME card
 *
 * DESCRIPTION:
 * Writes the given message to the given card using the following algorithm:
 *
 *      Copy the output string to a temp buffer.
 *      Confirm that a valid card number has been given. 
 *      If there is not enough space in the ring buffer return an error.
 *      Copy message into ring buffer. 
 *      If message was changed during write return an error.
 *      Enable the transmit buffer empty interrupt.
 *
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
 * If buffer was changed the corrupted message is left in the ring buffer.
 *-
 ************************************************************************
 */


long drvOmsVmeWriteCard 
(
    int card,               /* Card number to write to                      */ 
    char *buffer            /* String to be written to card                 */
)
{
    DRV_OMS_VME_CARD *pCard;                /* Card control structure       */
    DRV_OMS_VME_REGISTERS *pRegisters;      /* Register structure           */ 
    int msgSize;                            /* Length of message to write   */
    long status = 0;                        /* Function return status       */
    char bufCopy[DRV_OMS_VME_MAX_MSG_LEN+1];  /* Scratchpad buffer            */

    /*
     * Make a copy of the buffer to check later
     */

    strncpy (bufCopy, buffer, DRV_OMS_VME_MAX_MSG_LEN);

     
    /*
     * Ensure that the request is for a valid card
     */
     
    if ((pCard = pCards[card]) == NULL)
    {
        SET_ERR_MSG ("Write to invalid OMS card");
        logMsg("writeCard: OMS card:%d invalid\n", card, 0, 0, 0, 0, 0);
        return DRV_OMS_VME_S_CFG_ERROR;
    }

    pCard = pCards[card];            
    pRegisters = pCard->pRegisters;
             

    /*
     * Check to ensure there is enough space in the ring buffer
     */
        
    msgSize = strlen (buffer);

    semTake (pCard->writeMutex, WAIT_FOREVER);
    
    if (rngFreeBytes(pCard->writeBuf) < msgSize)
    {
        semGive (pCard->writeMutex);
        SET_ERR_MSG ("OMS write buffer overflow");
        logMsg("writeCard: OMS card:%d ring buffer overflow\n", 
             card, 0, 0, 0, 0, 0);
        return DRV_OMS_VME_S_BUFFER_FULL;
    }

 
    /*
     * Transfer the Message into the ring buffer
     */
                       
    if ( (rngBufPut (pCard->writeBuf, buffer, msgSize)) != msgSize )
    {
        semGive (pCard->writeMutex);
        SET_ERR_MSG ("OMS write buffer failure");
        logMsg("writeCard: OMS card:%d ring buffer failure, rngBufPut\n", 
             card, 0, 0, 0, 0, 0);
        return DRV_OMS_VME_S_CFG_ERROR;
    }

    semGive (pCard->writeMutex);


    /*
     * Sanity check to see if someone overwrote the buffer while we were
     * copying it into the ring buffer.
     */

    if (strcmp (bufCopy, buffer))
    {
        SET_ERR_MSG ("OMS write buffer failure");
        logMsg("writeCard: OMS card:%d buffer changed, was:%s, now:%s\n", 
             card, (int)bufCopy, (int)buffer, 0, 0, 0);
        return DRV_OMS_VME_S_CFG_ERROR;
    }

    
    /*
     *  Buffer write okay, so ensure that the transmit interrupt is enabled.
     *  Enabling the transmit buffer empty interrupt will cause the card to
     *  generate an interrupt immediately allowing the interrupt service routine
     *  to send the message to the card one character at a time.
     */
     
    pRegisters->control |= DRV_OMS_VME_IRQ_TRANS_BUF;
    
    return status;                                      
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * drvOmsWriteMotor
 *
 * INVOCATION:
 * status = drvOmsVmeWriteMotor (card, axis, buffer);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *
 * (>) card   (int)    card number to write to
 * (>) axis   (int)    axis on the card to get the message
 * (>) buffer (char *) message to be sent to axis 
 *
 * FUNCTION VALUE:
 * (long) write function success.
 *
 * PURPOSE:
 * Write a message to a specific OMS VME axis
 *
 * DESCRIPTION:
 * Writes the given message to the given axis on the given card using the
 * following algorithm:
 *
 *      Copy the output string to a temp buffer.
 *      Confirm that a valid card number and axis has been given. 
 *      If there is not enough space in the ring buffer return an error.
 *      Copy the axis selection string into the ring buffer.
 *      Copy message into ring buffer. 
 *      If message was changed during write return an error. 
 *      Enable the transmit buffer empty interrupt.
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
 * If buffer was changed the corrupted message is left in the ring buffer.
 *-
 ************************************************************************
 */

long drvOmsVmeWriteMotor (int card, int axis, char *buffer)
{
    DRV_OMS_VME_CARD *pCard;
    DRV_OMS_VME_REGISTERS *pRegisters; 
    register int toP;
    int msgSize;
    long status = 0;
    char bufCopy[DRV_OMS_VME_MAX_MSG_LEN+1];

    /*
     * Make a copy of the buffer to check later
     */

    strncpy (bufCopy, buffer, DRV_OMS_VME_MAX_MSG_LEN);
     

    /*
     * Ensure that the request is for a valid card
     */
     
    if ((pCard = pCards[card]) == NULL)
    {
        SET_ERR_MSG ("Write to invalid OMS card");
        logMsg("writeMotor: OMS card:%d invalid\n", card, 0, 0, 0, 0, 0);
        return DRV_OMS_VME_S_CFG_ERROR;
    }


    /*
     * Ensure that the request is for a valid axis
     */
     
    pCard = pCards[card];         
    if (axis >= pCard->axes)
    {
        SET_ERR_MSG ("Write to invalid OMS axis");
        logMsg("writeMotor: OMS card:%d invalid axis:%d\n", 
             card, axis, 0, 0, 0, 0);
        return DRV_OMS_VME_S_CFG_ERROR;
    }


    pRegisters = pCard->pRegisters;
    
    semTake (pCard->writeMutex, WAIT_FOREVER);


    /*
     * Check to ensure there is enough space in the ring buffer
     */
        
    msgSize = strlen (buffer);
    if (msgSize == 0)
    {
        logMsg("writeMotor: OMS card:%d, zero message length\n", 
                card, 0, 0, 0, 0, 0);
    }

    if ( (rngFreeBytes(pCard->writeBuf)) < (msgSize + 2) )
    {
        semGive (pCard->writeMutex);
        SET_ERR_MSG ("OMS write buffer overflow");
        logMsg("writeMotor: OMS card:%d buffer overflow, msgSize:%d\n", 
             card, msgSize, 0, 0, 0, 0);
        return DRV_OMS_VME_S_BUFFER_FULL;
    }

 
    /*
     *  Write the axis code for the selected axis to the ring buffer.
     */
        
    if ( !RNG_ELEM_PUT (pCard->writeBuf, 'A', toP) ||
         !RNG_ELEM_PUT (pCard->writeBuf, axisName[axis], toP)   )
    {
        semGive (pCard->writeMutex);
        SET_ERR_MSG ("OMS write buffer failure");
        logMsg("writeMotor: OMS card:%d ring buffer failed,RNG_ELEM_PUT\n", 
             card, 0, 0, 0, 0, 0);
        return DRV_OMS_VME_S_CFG_ERROR;
    }


    /*
     * ...then put the message itself in the ring buffer
     */
        
    if ( (rngBufPut (pCard->writeBuf, buffer, msgSize)) != msgSize  )
    {
        semGive (pCard->writeMutex);
        SET_ERR_MSG ("OMS write buffer failure");
        logMsg("writeMotor: OMS card%d ring buffer failed,rngBufPut\n", 
             card, 0, 0, 0, 0, 0);
        return DRV_OMS_VME_S_CFG_ERROR;
    }

    semGive (pCard->writeMutex);


    /*
     *  Sanity check to see if buffer was changed while transferring to
     *  ring buffer.
     */

    if (strcmp (bufCopy, buffer))
    {
        SET_ERR_MSG ("OMS write buffer failure");
        logMsg("writeMotor: OMS card:%d buffer changed, was:%s, now:%s\n", 
             card, (int)bufCopy, (int)buffer, 0, 0, 0);
        return DRV_OMS_VME_S_CFG_ERROR;
    }


    /*
     * Buffer write okay, so ensure that the transmit interrupt is enabled
     */

    pRegisters->control |= DRV_OMS_VME_IRQ_TRANS_BUF;

    return status;                                      
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * dumpDebugBuffer
 *
 * INVOCATION:
 * dumpDebugBuffer (pCard);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pCard (DRV_OMS_VME_CARD *) Card control structure
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Dump the read and write buffers to the screen using logMsg
 *
 * DESCRIPTION:
 * Write the read and write debugging buffers to the logMessage output
 * after converting all characters to 6 bit ascii values.
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

static void dumpDebugBuffer
(
    DRV_OMS_VME_CARD *pCard             /* Card control structure       */
)
{
    int i;                              /* generic index thing          */ 

    /*
     *  Insure that a valid card control structure has been supplied
     */

    if (pCard == NULL)
    {
        return;
    }


    /*
     *  Add null terminations to read and write buffers
     */

    readDebugBuffer[DRV_OMS_VME_MAX_MSG_LEN] = '\0';
    writeDebugBuffer[DRV_OMS_VME_MAX_MSG_LEN] = '\0';


    /*
     *  Constrain read debug buffer to 6 bit ascii and then log the message
     */

    for (i = 0; i < DRV_OMS_VME_MAX_MSG_LEN; i++)
    {
        readDebugBuffer[i] = pCard->readDebug[(pCard->pReadDebug + i) & 0x3f];
    }

    logMsg("drvOmsVme:Read Debug Buffer:  %s\n", 
           (int)readDebugBuffer,0,0,0,0,0);


    /*
     *  Constrain write debug buffer to 6 bit ascii and then log the message
     */

    for (i = 0; i < DRV_OMS_VME_MAX_MSG_LEN; i++)
    {
        writeDebugBuffer[i] = pCard->writeDebug[(pCard->pWriteDebug + i)&0x3f];
    }

    logMsg("drvOmsVme:Write Debug Buffer:  %s\n", 
            (int)writeDebugBuffer,0,0,0,0,0);

    return;
}

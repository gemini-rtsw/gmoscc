static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id$"};

/* *INDENT-OFF* */
/*
 *   FILENAME
 *   gmSeqLib.c
 *
 *   PURPOSE:
 *   Routines used by GMOS Sequencer: includes utility and status
 *   combination routines, but excluding those used by CAD records.
 *
 *   FUNCTION NAME(S):
 *    gmSeqNotEmptyString  - Check if a string has any alphanumeric characters
 *    gmSeqUc -              Convert a string to uppercase
 *    gmSeqDcString -        Decodes a string by matching an array element
 *    gmSeqSubsysCar  -      Combine CAR status and messages into a single activeC value
 *    gmSeqStateInit -       Initialise the state to BOOTING as a string
 *    gmSeqStateCombine -    Combine together multiple integer subsystem states
 *    gmSeqSetCADTest -      Set the relevant bits in the CAD test mask
 *    gmSeqClearCADTest -    Clear the CAD test mask
 *    gmSeqGetCADTest -      Get the CAD test mask
 *    gmSeqDisplayCADTest -  Display the CAD test mask
 *
 *
 *   Copyright Observatory Sciences Ltd. 1999-2000. All rights reserved.
 *   Under contract to the UK Astronomy Technology Centre, who modified the code
 *   in 2001.
 * 
 */
/*
 * $Log$
 * Revision 1.2  2001/11/28 19:35:26  mbec
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.16  2001/02/28 12:47:22  gmos
 * gmSeqDisplayCADTest function and additional debugging code added.
 *
 * Revision 1.15  2001/02/21 13:34:50  gmos
 * Copyright statement modified.
 *
 * Revision 1.14  2001/02/05 13:07:41  gmos
 * Code prologue tidied up.
 *
 * Revision 1.13  2001/01/30 11:50:40  gmos
 * Replaced dangerous strcpy statement with strncpy.
 *
 * Revision 1.12  2000/10/04 17:02:50  pbt
 * Modified gmSeqStateCombine: output blank connection state for the first 10 seconds after initialisation
 *
 * Revision 1.11  2000/09/20 17:06:10  gmos
 * Changed the priority order for states to RUNNING, CONFIGURING, INITIALIZING, DISCONNECTED, BOOTING, ERROR.
 *
 */
/* *INDENT-ON* */

#include <vxWorks.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <lstLib.h>

#include <dbDefs.h>
#include <tsDefs.h>
#include <alarm.h>
#include <cadRecord.h>
#include <cad.h>
#include <carRecord.h>
#include <genSubRecord.h>


#include "gmSeq.h"


/* External global variable containing the GMOS sequencer debug level */
extern int gmSeqDbgLevel;

/* Global bit mask representing CAD pre-conditions to be tested for */
static int testCondition;   


/*+
 *   Function name:
 *   gmSeqNotEmptyString
 *
 *   Purpose:
 *   Check if a string has any alphanumeric characters
 *
 *   Description:
 *   Check each character in a null terminated string to see if it is
 *   alphanumeric. If there are any alphanumeric characters then return
 *   true otherwise return false.
 *
 *   Invocation:
 *   status = gmSeqNotEmptyString(s)
 *
 *   Input parameter: 
 *         s    =>    (char *)   Null terminated string
 *
 *   Function value:
 *         (int)  Return status, 0 = FALSE otherwise TRUE 
 * 
 *   Deficiencies:
 *   What happens if the string isn't terminated with a NULL ?
 *
 *-
 */

int gmSeqNotEmptyString (char *s) 

{
  int status = FALSE ;

  while (*s != '\0') {
   if (isalnum(*s)){
    status = TRUE ;
    break ;
   }
   s++ ;
  }

  return status ;
}


/*+
 *   Function name:
 *   gmSeqUc
 *
 *   Purpose:
 *
 *  Convert a string to uppercase.
 *
 *   Invocation:
 *    gmSeqUc(in, lout, out)
 *
 *   Input parameters:
 * 
 *      <name> =>  <C type>   <parameter description>
 *
 *      in  =>  (char *)      String to be converted
 *      lout =>  (char *)     length of output string (including terminator)
 *
 *  Output parameters:
 *
 *      out => (char *)       String after conversion to upper-case
 *
 *   Function value: 
 *
 *   (char *) String after conversion to upper-case
 *
 *   Notes:
 *
 *      The two arguments can refer to the same string.
 *-
 */
char * gmSeqUc(char *in, int lout, char *out)
{
    int i, c;

    out[lout - 1] = (char) '\0';
    for (i = 0; i < lout - 1; i++) {
        c = (int) in[i];
        out[i] = (char) toupper(c);
        if (c == '\0')
            break;
    }
    return out;
}


/*+
 *   Function name:
 *   gmSeqDcString
 *
 *   Purpose:
 *   Decode a string 
 *
 *   Description:
 *   This routine decodes a string by matching it to one of the elements in
 *   a supplied array of strings and returns the index of the matching string.
 *
 *   Invocation:
 *   index = gmSeqDcString (match, string)
 *
 *   Input parameters:
 * 
 *      <name> =>  <C type>   <parameter description>
 *
 *      match  =>  (char **)     Pointer to array of string to match
 *      string =>  (char *)      String to decode
 *
 *   Function value: 
 *
 *   int  => Return status = index,  -1 Invalid string
 *-
 */

int gmSeqDcString(char **match, char *string) 

{
  int index = 0;

/* Reject NULL string */
  if (string[0] == '\0') return -1;
  
  for ( index = 0; match[index]; index++ ) {
    if (!strcmp (string, match[index])) return index ;
  } 
  return -1 ;

}



/*+
 *   Function name:
 *   gmSeqSubsysCar
 *
 *   Purpose:
 *   Combine CAR status and messages into a single activeC value
 *
 *   Description:
 *   This routine combines the activeC and commSentC CAR values of a
 *   single GMOS subsystem into a single CAR value. By default, the
 *   routine outputs an IDLE status together with a null message. 
 *   If the subsystem is present and the subsystem has an ERROR status set then
 *   a check is made to ensure that an error message has been provided. If 
 *   not then a default message is output.
 *   If the subsystem has no errors but is BUSY or PAUSED then output this state.
 *   If the subsystem has disconnected then set state ERROR with message.
 *
 *   Invocation:
 *   gmSeqSubsysCar (pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *
 *   Function value:
 *   (<)  status  (long)  Return status, 0 = OK
 * 
 *
 *   Epics inputs:
 *
 *   a => Flag to indicate if subsystem is disconnected (TRUE = Disconnected)
 *   b => Name of subsystem 
 *   c => subsystem command sent CAR value 
 *   d => subsystem activeC CAR value
 *   e => subsystem command sent CAR message 
 *   f => subsystem activeC CAR message 
 *
 *   Epics outputs:
 *
 *   vala => Overall subsystem activeC message
 *   valb => Overall subsystem activeC value
 * 
 *-
 *   Copyright OSL 1999. All rights reserved.
 */


long gmSeqSubsysCar (struct genSubRecord *pgsub) 

{

 long subsysAbsent ;        /* Subsystem disconnected? */
 long actCVal ;             /* Overall activeC value */
 char actCMess[MAX_STRING_SIZE] ; /* Overall activeC message */
 long subsysActCVal ;       /* ActiveC values for subsystem */
 long subsysCommSentVal;    /* Subsys command sent CAR value */
 char subsysMess[MAX_STRING_SIZE];    /* CC message */
 char subsysCsMess[MAX_STRING_SIZE];  /* CC message from "command sent" CAR */
 char subsysName[MAX_STRING_SIZE];     /* Name of subsystem */

/* Set default output */

 actCVal = menuCarstatesIDLE ;     /* IDLE by default when no other states set */
 strncpy (actCMess, " ", MAX_STRING_SIZE-1) ;

/* Get whether the subsystem is disconnected, and its name
*/

 subsysAbsent = *(long *)pgsub->a ;
 strncpy(subsysName, (char *)pgsub->b, MAX_STRING_SIZE-1) ; 

 if (!subsysAbsent) {

   subsysCommSentVal = *(long *)pgsub->c ;
   subsysActCVal = *(long *)pgsub->d ;
   strncpy(subsysCsMess, (char *)pgsub->e, MAX_STRING_SIZE-1);
   strncpy(subsysMess, (char *)pgsub->f, MAX_STRING_SIZE-1) ;
   
/* Now combine the CAR values to make a single combined output value. If
*  any of the CAR records are BUSY then the overall status is BUSY. If none
*  are BUSY but at least one is in ERR then output ERR otherwise output 
*  IDLE.
*/

   if (subsysActCVal == menuCarstatesERROR || subsysCommSentVal == menuCarstatesERROR ) {
     actCVal = menuCarstatesERROR ;
     if (subsysCommSentVal == menuCarstatesERROR) {
       if (gmSeqNotEmptyString(subsysCsMess)) {
         strncpy (actCMess, subsysCsMess, MAX_STRING_SIZE-1) ;
       } else  {
         sprintf (actCMess, "Error sending command to %.12s", subsysName) ;
       }
     } else if (subsysActCVal == menuCarstatesERROR) {
       if (gmSeqNotEmptyString(subsysMess)) {
         strncpy (actCMess, subsysMess, MAX_STRING_SIZE-1) ;
       } else  {
         sprintf (actCMess, "Unspecified error from %.12s", subsysName) ;
       }
     }
   }
   
/* No errors: if either BUSY, then output BUSY */
   else if (subsysActCVal == menuCarstatesBUSY || subsysCommSentVal == menuCarstatesBUSY ) {
     actCVal = menuCarstatesBUSY ;
     strcpy(actCMess, " ") ;
   }
   
/* No errors and not BUSY: if subsystem PAUSED, output PAUSED */
   else if (subsysActCVal == menuCarstatesPAUSED && subsysCommSentVal == menuCarstatesIDLE ) {
     actCVal = menuCarstatesPAUSED ;
     strcpy(actCMess, " ") ;
   }
   
 }
 
 else   /* Subsystem is disconnected - set ERROR */
 {
     actCVal = menuCarstatesERROR;
     sprintf(actCMess, "GMOS %.12s is disconnected", subsysName);
 }

 strncpy((char *)pgsub->vala, actCMess, MAX_STRING_SIZE-1) ;
 *(long *)pgsub->valb = actCVal ;

 return 0 ;

}

/* ===================================================================== */

/*+
 *   Function name:
 *   gmSeqStateInit
 *
 *   Purpose:
 *   Initialise the state to BOOTING where state values are stored as strings.
 *
 *   Description:
 *   This function generates a state value which corresponds to BOOTING. 
 *
 *   Invocation:
 *   status = gmSeqStateCombine( struct genSubRecord *pgensub );
 *
 *   EPICS inputs:
 *
 *   NONE
 *
 *   EPICS outputs:
 *
 *      vala => string   Booting state
 *      valb => long     State as integer (index) (zero)
 *
 *   Return value:
 *
 *      status      long        Status value
 *
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Limitations:
 *
 *   Original Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *
 *   o 04-Aug-1999: Original GMOS version.                         (smb)
 *   o 14-Oct-1999: Version for GMOS Sequencer.                    (pbt)
 *   
 *-
 */

long gmSeqStateInit( struct genSubRecord *pgensub ) 
{
    long status;                       /* return status */

/* Initialise the status */

    status = 0;

/* Output the booting state string and zero value. */

    strcpy( (char *)pgensub->vala, "BOOTING");
    *(long *)pgensub->valb = 0;

    return status;
}



/* ===================================================================== */

/*+
 *   Function name:
 *   gmSeqStateCombine
 *
 *   Purpose:
 *   Combine together multiple integer subsystem states
 *
 *   Description:
 *   Combine together multiple subsystem states where state values are 
 *   stored as integers.
 *   This function reads the states of the two subsystems as strings
 *   and generates a combined state. The string are converted to an index
 *   It is assumed that the state strings are ordered such that the 
 *   lower state takes precedence.
 *
 *   Additionally, values are read in signalling whether the subsystem
 *   is connected. The state is set to DISCONNECTED if either subsystem
 *   is disconnected. Messages are output if a connection state has changed 
 *   since the last time the routine was executed.
 *
 *   Invocation:
 *   status = gmSeqStateCombine( struct genSubRecord *pgensub );
 *
 *   EPICS inputs:
 *
 *      a =>   long    TRUE if components controller is disconnected
 *      b =>   long    TRUE if detector controller is disconnected
 *      c =>   string  State of components controller as a string
 *      d =>   string  State of detector controller as a string
 *
 *   EPICS outputs:
 *
 *      vala => string  Resulting state value string
 *      valb => long    Resulting state as an integer (index) value
 *      valc => string  Connection state change message string
 *
 *   Return value:
 *
 *      status      long        Status value
 *
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   o 04-Aug-1999: Original GMOS version.                         (smb)
 *   o 14-Oct-1999: Version for GMOS Sequencer.                    (pbt)
 *   
 *-
 */

long gmSeqStateCombine( struct genSubRecord *pgensub ) 
{
    static char *IOCstates[] = {"RUNNING","CONFIGURING","INITIALIZING",
                                "DISCONNECTED","BOOTING","ERROR"};
    
    static long firstTime = TRUE;
    long status;                       /* return status */
    long CCdisconnected;               /* components controller disconnected? */
    long DCdisconnected;               /* detector controller disconnected? */
    static long CCwasDisconnected = -1; /* components controller was disconnected? */
    static long DCwasDisconnected = -1; /* detector controller wasdisconnected? */
    char stateCC[MAX_STRING_SIZE] = ""; /* Input CC state */
    char stateDC[MAX_STRING_SIZE] = ""; /* Input DC state */
    long indexStateCC;                 /* Integer index of CC state */
    long indexStateDC;                 /* Integer index of DC state */
    long outIndex;                     /* State output integer value */
    TS_STAMP timestamp;                /* Current timestamp value */
    static long timeInit;              /* Time in secs. at initialisation */
    long timeNow;                      /* Current time in secs. */

/* Initialise the status */

    status = 0;

/* Get current timestamp, remember value on first entry with valid time */        
    timestamp = pgensub->time;
    timeNow = timestamp.secPastEpoch;
    if (firstTime && timeNow != 0)
    {
	firstTime = FALSE;
        timeInit = timeNow;
    }
    
/* Blank connection string if less than 10 secs. since startup */        
    if (firstTime || ((timeNow - timeInit) < 10))
	 strncpy(pgensub->valc, "", MAX_STRING_SIZE-1);
    else	
    {
/* Read in the CC disconnected status value (Input A). TRUE (1) = disconnected */
      CCdisconnected = *(long *)pgensub->a;

/* Output message on VALC if state has changed and remember current state */
      if (CCdisconnected != CCwasDisconnected)
      {
	  if (CCdisconnected)
	      strncpy(pgensub->valc, "Components Controller has disconnected", MAX_STRING_SIZE-1);
	  if (!CCdisconnected)
	      strncpy(pgensub->valc, "Components Controller has connected", MAX_STRING_SIZE-1);
	  CCwasDisconnected = CCdisconnected;
      }
    
/* Read in the DC disconnected status value (Input B). TRUE (1) = disconnected */
      DCdisconnected = *(long *)pgensub->b;

/* Output message also on VALC if state has changed and remember current state */
      if (DCdisconnected != DCwasDisconnected)
      {
	  if (DCdisconnected)
	      strncpy(pgensub->valc, "Detector Controller has disconnected", MAX_STRING_SIZE-1);
	  if (!DCdisconnected)
	      strncpy(pgensub->valc, "Detector Controller has connected", MAX_STRING_SIZE-1);
	  DCwasDisconnected = DCdisconnected;
      }

/* If either subsystem disconnected, the output system state is DISCONNECTED 
   and an alarm is raided */
      if (CCdisconnected || DCdisconnected )
         {
         strcpy( (char *)pgensub->vala, "DISCONNECTED");
         *(long  *)pgensub->valb = 4;  /* Value 4 = DISCONNECTED */
         pgensub->brsv = MAJOR_ALARM;
         return -1;
         }
     }  
/* Read in the states of both subsystems. Default to BOOTING if 
   subsystem state is unrecognised  */

    strncpy(stateCC, (char *)pgensub->c,  MAX_STRING_SIZE-1) ;
    stateCC[MAX_STRING_SIZE-1] = '\0';
    if ( (indexStateCC = gmSeqDcString(IOCstates, stateCC)) < 0)
      indexStateCC = gmSeqDcString(IOCstates, "BOOTING");

    strncpy(stateDC, (char *)pgensub->d,  MAX_STRING_SIZE-1) ;
    stateDC[MAX_STRING_SIZE-1] = '\0';
    if ( (indexStateDC = gmSeqDcString(IOCstates, stateDC)) < 0)
      indexStateDC = gmSeqDcString(IOCstates, "BOOTING");

/* The output state is the higher index value of the 2 strings */
    outIndex = (indexStateCC >= indexStateDC) ? indexStateCC : indexStateDC; 
   
/* Output the resulting state string and index */
    if (outIndex >= 0)
      strncpy((char *)pgensub->vala, IOCstates[outIndex],MAX_STRING_SIZE-1);
    *(long *)pgensub->valb = outIndex;

    return 0;
}

/*+
 *   Function name:
 *   gmSeqSetCADTest
 *
 *   Purpose:
 *   Set the relevant bits in the CAD test mask 
 *
 *   Description:
 *   An input mask is bitwise OR'ed with the current value of
 *   the test mask to signify any new test conditions.
 *
 *   Invocation:
 *   gmSeqSetCADTest(testMask)
 *
 *   Parameters:
 *  
 *      <name>     =>  <C type>   <parameter description>
 *      testMask   =>  (int)      Mask with relevant test bits set
 *
 *   Function value: none
 *
 *  Globals:
 *  
 *     testCondition  (int)  Bit mask representing conditions to be tested
 * 
 *-
 */

void gmSeqSetCADTest(const int testMask)
{

    if ( gmSeqDbgLevel & DBG_FULL )
    {
       printf ("gmSeqSetCADTest: Setting %x (", testMask);
       if ( testMask & CONFIGURING ) printf ("CONFIGURING ");
       if ( testMask & READING_OUT ) printf ("READING_OUT ");
       if ( testMask & OBSERVING )   printf ("OBSERVING ");
       if ( testMask & MASTER_ENABLE ) printf ("MASTER_ENABLE ");
       printf (")\n");
    }

    testCondition = testCondition | testMask;
}

/*+
 *   Function name:
 *   gmSeqClearCADTest
 *
 *   Purpose:
 *   Clear the CAD test mask 
 *
 *   Description:
 *   The global CAD test mask is set to zero, ready to
 *   have bits set by subsequent CAD subroutine code
 *
 *   Invocation:
 *   gmSeqClearCADTest(testMask)
 *
 *   Parameters: (">" input, "!" modified, "<" output):
 *
 *                none
 *
 *   Function value: none
 *
 *  Globals:
 *  
 *     testCondition  (int)  Bit mask representing conditions to be tested
 * 
 *-
 */

void gmSeqClearCADTest()
{

    if ( gmSeqDbgLevel & DBG_FULL )
    {
       printf ("gmSeqSetCADTest: Clearing test mask\n");
    }

    testCondition = 0;
}

/*+
 *   Function name:
 *   gmSeqGetCADTest
 *
 *   Purpose:
 *   Get the CAD test mask 
 *
 *   Description:
 *   Get the value of the global CAD test mask
 *
 *   Invocation:
 *   index = gmSeqGetCADTest()
 *
 *   Parameters: (">" input, "!" modified, "<" output):
 * 
 *                none
 *
 *   Function value: 
 *
 *    (int) The value of the test mask
 *
 *  Globals:
 *  
 *     testCondition  (int)  Bit mask representing conditions to be tested
 *  
 *-
 */

int gmSeqGetCADTest()
{
     if ( gmSeqDbgLevel & DBG_MIN ) gmSeqDisplayCADTest();

     return testCondition;
}

/*+
 *   Function name:
 *   gmSeqDisplayCADTest
 *
 *   Purpose:
 *   Display the CAD test mask 
 *
 *   Description:
 *   Debugging function to display the value of the global CAD test mask
 *
 *   Invocation:
 *   index = gmSeqDisplayCADTest()
 *
 *   Parameters: (">" input, "!" modified, "<" output):
 * 
 *                none
 *
 *   Function value: 
 *
 *    (int) The value of the test mask
 *
 *  Globals:
 *  
 *     testCondition  (int)  Bit mask representing conditions to be tested
 *  
 *-
 */

void gmSeqDisplayCADTest()
{

    printf ("gmSeqDisplayCADTest: Command test mask = %x (", testCondition);

    if ( testCondition & CONFIGURING ) printf ("CONFIGURING ");
    if ( testCondition & READING_OUT ) printf ("READING_OUT ");
    if ( testCondition & OBSERVING )   printf ("OBSERVING ");
    if ( testCondition & MASTER_ENABLE ) printf ("MASTER_ENABLE ");

    printf (")\n");

}

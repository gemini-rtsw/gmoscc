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
 * <gmOiwfsCadSupport.c>
 *
 * PURPOSE:
 * Provide oiwfs Command Action Directive (CAD) Record support
 *
 * FUNCTION NAME(S)
 * checkBuffer          - common internal for evaluating inputs
 * oiwfsdatumCad        - Datum command cad support 
 * oiwfsDebugCad        - Debug command cad support
 * oiwfsFollowCad       - Follow command cad support       
 * oiwfsinitCad         - Init command cad support
 * oiwfsMoveCad         - Move command cad support
 * oiwfsparkCad         - Park command cad support
 * oiwfsStopCad         - Stop command cad support
 * oiwfstestCad         - Test command cad support
 * oiwfsToleranceCad    - Tolerance command cad support 
 *
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.12  2000/12/19 13:30:23  gmos
 * Allow the DEBUG command to accept the QUIET string.
 *
 * Revision 1.11  2000/12/18 12:20:52  gmos
 * Used DAR_DBUG constants for setting debugging mode rather than hard-wired integers.
 *
 * Revision 1.10  2000/11/08 14:35:48  gmos
 * Reject DATUM, MOVE, FOLLOW and PARK commands when master enable is OFF.
 *
 * Revision 1.9  2000/11/03 01:05:00  gmos
 * Added VALC output to datum, init & test cads.  VALC either
 * enables (datum) or disables (init or test) a longout record
 * which sets followState to 0.
 *
 * Revision 1.8  2000/11/02 13:45:37  gmos
 * Modified oiwfsMoveCad to check the INIT, INDX, ILCK and ASTA fields of 
 * the probeAssembly record before proceeding.
 *
 * Revision 1.7  2000/11/01 16:29:53  gmos
 * Added extra command rejection checks depending on values of probe assembly 
 * INIT, INDX, ILCK and ASTA fields. More checks are still needed.
 *
 * Revision 1.6  2000/10/30 18:01:49  gmos
 * Added extra checks so high level commands are rejected if the probe is 
 * not initialised or not indexed.
 *
 * Revision 1.5  2000/10/16 23:48:00  gmos
 * Added checkBuffer() to help parsing arguments.
 *
 * Revision 1.3  2000/05/30 16:29:09  gmos
 * New version commented by William Rambold. Master enable bug fixed.
 *
 * Revision 1.2  2000/04/15 11:38:55  gmos
 * Angelics updated OIWFS software
 *
 * Revision 1.3  2000/04/13 08:25:50  gmos
 * Corrections released by Angelic on 13 April 2000
 *
 * Revision 1.5  2000/04/12 00:59:34  angelic
 * changed Park Command to always execute and to change
 * follow mode to 0
 *
 * Revision 1.4  2000/04/07 16:46:58  angelic
 * added support for a debug CAD to allow the TCS to set global
 * debug level.  Supports NONE,MIN,FULL,MAX though TCS will not
 * be using (know about) MAX.
 *
 * Revision 1.3  2000/02/16 19:15:21  dunn
 * Changed names of the snam for init, datum, park, test
 * to match the db.
 *
 * Revision 1.2  2000/02/01 07:00:49  daoinsw
 * Removed reference to devOiwfsControl.h
 *
 * Revision 1.1  2000/01/28 23:25:53  dunn
 * Initial revision
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h> 
#include <limits.h>

#include <dbEvent.h>
#include <cadRecord.h>
#include <cad.h>
#include <carRecord.h>

#include <assemblyControlRecord.h>
#include <recAssControl.h>


/*
 *  Definitions to provide access to the CAD record input fields
 */

#define STRING_A    (char *) pcr->a        /* CAD A field string value      */
#define STRING_B    (char *) pcr->b        /* CAD B field string value      */
#define STRING_C    (char *) pcr->c        /* CAD C field string value      */
#define STRING_D    (char *) pcr->d        /* CAD D field string value      */
#define STRING_E    (char *) pcr->e        /* CAD E field string value      */
#define STRING_F    (char *) pcr->f        /* CAD F field string value      */
#define STRING_G    (char *) pcr->g        /* CAD G field string value      */
#define STRING_H    (char *) pcr->h        /* CAD H field string value      */
#define STRING_I    (char *) pcr->i        /* CAD I field string value      */
#define STRING_J    (char *) pcr->j        /* CAD J field string value      */
#define STRING_K    (char *) pcr->k        /* CAD K field string value      */


/*
 *  Definitions to provide access to the CAD record output fields
 */

#define STRING_VALA   (char *) pcr->vala   /* CAD VALA field string value   */ 
#define LONG_VALA    *(long *) pcr->vala   /* CAD VALA field long value     */
#define DOUBLE_VALA  *(double *) pcr->vala /* CAD VALA field double value   */

#define STRING_VALB   (char *) pcr->valb   /* CAD VALB field string value   */
#define LONG_VALB    *(long *) pcr->valb   /* CAD VALB field long value     */
#define DOUBLE_VALB  *(double *) pcr->valb /* CAD VALB field double value   */

#define STRING_VALC   (char *) pcr->valc   /* CAD VALC field string value   */
#define LONG_VALC    *(long *) pcr->valc   /* CAD VALC field long value     */
#define DOUBLE_VALC  *(double *) pcr->valc /* CAD VALC field double value   */

#define STRING_VALD   (char *) pcr->vald   /* CAD VALD field string value   */
#define LONG_VALD    *(long *) pcr->vald   /* CAD VALD field long value     */
#define DOUBLE_VALD  *(double *) pcr->vald /* CAD VALD field double value   */

#define LONG_VALE    *(long *) pcr->vale   /* CAD VALE field long value     */
#define DOUBLE_VALE  *(double *) pcr->vale /* CAD VALE field double value   */

#define LONG_VALF    *(long *) pcr->valf   /* CAD VALF field long value     */
#define DOUBLE_VALF  *(double *) pcr->valf /* CAD VALF field double value   */


/*
 *  Local definitions
 */

#define STRTOL_BASE         10      /* conversion base for strtol function  */
#define SCRATCH_BUF_SIZE    64      /* error message formatting buffer size */

#define CMD_IDLE            0       /* System is idle                       */
#define CMD_BUSY            1       /* System is busy executing a command   */
#define CMD_ERROR           2       /* Last command executed failed         */

#define FOLLOW_MODE         2       /* Follow TCS target positions          */
#define MOVE_MODE           1       /* Stop following and move to a position*/
#define STOP_MODE           0       /* Stop following without moving probe  */

#define STOP_ENABLE         0       /* Allow CAD to Stop following          */
#define STOP_DISABLE       -1       /* Don't allow CAD to Stop following    */


/* For CAD input checking */

#define ALL_BLANKS     1       /* all white space                           */
#define SHORT_INTEGER  2       /* short integer                             */
#define LONG_INTEGER   3       /* long integer                              */
#define FLOAT_VALUE    4       /* floating number                           */
#define DOUBLE_VALUE   5       /* double precision number                   */
#define BAD_VALUE      6       /* not fit any of previous 5 classifications */

/*
 *  Function prototypes
 */

long oiwfsdatumCad (struct cadRecord *);
long oiwfsDebugCad (struct cadRecord *);
long oiwfsFollowCad (struct cadRecord *);
long oiwfsinitCad (struct cadRecord *);
long oiwfsMoveCad (struct cadRecord *);
long oiwfsparkCad (struct cadRecord *);
long oiwfsStopCad (struct cadRecord *);
long oiwfstestCad (struct cadRecord *);
long oiwfsToleranceCad (struct cadRecord *);

/*
 * Internal function prototypes
 */

static long checkBuffer (char *);

/*
 *  Macros
 *
 *  ERROR_MESSAGE transfers the given string, up to the maximum allowable
 *  number of characters, to the CAD record error message (MESS) field. 
 */

#define ERROR_MESSAGE(f)   strncpy(pcr->mess, f, MAX_STRING_SIZE-1)

/*
 *  Public debug control word
 */

int oiCadDebug = 0;             /* enable debug messages (T/F)          */



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * checkBuffer
 *
 * INVOCATION:
 *      status = checkBuffer ();
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)  
 * (>) null terminated string to check
 *      
 * FUNCTION VALUE:
 * status  (long) 
 *                      ALL_BLANKS, if all white space
 *                      SHORT_INTEGER
 *                      LONG_INTEGER
 *                      FLOAT_VALUE
 *                      DOUBLE_VALUE
 *                      BAD_VALUE, if cannot be converted
 * 
 * PURPOSE:
 * Checks the contents of the character buffer passed to see if it
 * holds an integer, double, real, string or is blank.
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * limits.h, float.h for limit checking and ctype.h for isspace
 *
 * SEE ALSO:
 * None 
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

long checkBuffer( char *buf )
{
    char   *startP;
    char   *endP;
    long   lval;
    double dval;

    startP = buf;

    while (isspace(*startP) )
        startP++;         /* startP refers to first non-whitespace character */
    if (*startP == '\0')
    {
        if (oiCadDebug)
            printf("checkBuffer: CAD input has all blanks\n");
        return ALL_BLANKS;          /* Nothing in the buffer but whitespace  */
    }
    else
    {
        lval = strtol(startP, &endP, STRTOL_BASE);

        if (lval > -LONG_MAX && lval < LONG_MAX && *endP == '\0')
        {
            if (lval >= -INT_MAX  && lval <= INT_MAX)
                return SHORT_INTEGER;          /* buffer holds short integer */
            else
                return LONG_INTEGER;            /* buffer holds long integer */
        }
        else
        {
            dval = strtod (startP, &endP);
            if (dval >= -DBL_MAX && dval <= DBL_MAX && *endP == '\0')
            {
                if (dval >= -FLT_MAX && dval <= FLT_MAX)
                    return FLOAT_VALUE;  /* buffer holds floating-point real */
                else
                    return DOUBLE_VALUE;              /* buffer holds double */
            }
            else
            {
                if (oiCadDebug)
                    printf("checkBuffer: CAD input does not hold integer or real\n");
                return BAD_VALUE;    /* buffer does not hold integer or real */
            }
        }
    }
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiwfsdatumCad
 *
 * INVOCATION:
 * status = oiwfsdatumCad (pcr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pcr  (struct cadRecord *) Pointer to calling CAD record.
 *
 * FUNCTION VALUE:
 * (long) command accept/reject flag.
 *
 * PURPOSE:
 * Re-index the OIWFS stages
 *
 * DESCRIPTION:
 * Reject a datum command if the input attributes can not be 
 * converted or the system is busy.
 *
 * Start a datum command by issuing an INDEX command to the 
 * oiwfs assembly Control record.
 * 
 * CAD input field assignments....
 *      A -> activeC.VAL
 *      B -> following_mode
 *      C -> OIWFS probe assembly initialized flag (0/1)
 *      D -> OIWFS probe assembly indexed flag (0/1)
 *      E -> OIWFS probe assembly interlocked flag (0/1)
 *      F -> OIWFS probe assembly state (IDLE, INITIALIZING, MOVING, etc.)
 *      G -> Master enable flag (ON/OFF)
 *
 * CAD output field assignments
 *      VALA -> oiwfs assemblyControl record mode field
 *      VALB -> oiwfs assemblyControl record directive field
 *      VALC -> enable STOP_MODE 
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

long oiwfsdatumCad 
(
    struct cadRecord *pcr           /* CAD record structure */
)
{
    char *pEnd;                     /* First non-translatable character     */
    long  actionState;              /* Current system action state          */
    long  followState;              /* Current system following state       */
    long  probeInit;                /* Has probe been initialized?          */
    long  probeIlock;               /* Has probe been interlocked?          */
    char *probeState;               /* Probe assembly state.                */
    char *masterEnable;             /* Master enable state (ON/OFF)         */
    char  errorMessage[SCRATCH_BUF_SIZE];  /* Error message                 */


    /*
     *  Processing depends on the state of the directive field as follows:
     */

    switch (pcr->dir)
    {
       /*
        *  Accept MARK, CLEAR & STOP directives, but do nothing
        */

        case menuDirectiveMARK:
        case menuDirectiveCLEAR:
        case menuDirectiveSTOP:
            break;

        /* 
         *  Preset and start reject the command if it is not safe to execute 
         *  a datum action at this time.
         */
 
        case menuDirectivePRESET:
        case menuDirectiveSTART:

            /*
             * Reject if the probe is interlocked.
             */

            probeIlock = strtol (STRING_E, &pEnd, STRTOL_BASE);
            if ( probeIlock != 0 )
            {
                if (oiCadDebug)
                    printf( "oiwfsdatumCad: %s, probe assembly interlocked.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe interlocked");
                return CAD_REJECT;
            }

            /*
             * Reject if the master enable is not enabled (which typically happens while
             * the software is still booting).
             */

            masterEnable = STRING_G;
            if ( strcmp(masterEnable, "ON") )
            {
                if (oiCadDebug)
                    printf( "oiwfsdatumCad: %s, master enable not enabled - still booting?\n",
                            pcr->name);
                ERROR_MESSAGE ("No master enable - Still booting?");
                return CAD_REJECT;
            }

            /*
             * Reject if the probe has not been initialised.
             */

            probeInit = strtol (STRING_C, &pEnd, STRTOL_BASE);
            if ( probeInit != 1 )
            {
                if (oiCadDebug)
                    printf( "oiwfsdatumCad: %s, probe assembly not initialized.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe not initialized");
                return CAD_REJECT;
            }

            /*
             *  Convert system action and following states from string
             *  to long. 
             */

            if (strcmp(STRING_A,"IDLE") == 0)
            {
                actionState = 0;
            }

            else if (strcmp(STRING_A,"BUSY") == 0) 
            {
                actionState = 1;
            }

            else if (strcmp(STRING_A,"ERR") == 0) 
            {
                actionState = 2;
            }

            else 
            {
                if (oiCadDebug)
                    printf("oiwfsdatumCad: %s, CAD action state input is invalid\n", pcr->name);
                ERROR_MESSAGE ("Invalid system action State");
                return CAD_REJECT;
            }

            followState = strtol (STRING_B, &pEnd, STRTOL_BASE);
            if (*pEnd != '\0')
            {
                if (oiCadDebug)
                    printf("oiwfsdatumCad: %s, CAD follow state input is invalid\n", pcr->name);
                ERROR_MESSAGE ("Invalid system follow State");
                return CAD_REJECT;
            }


            /*
             * Reject the command if any other command is being 
             * executed or following mode is active.
             */

            if (actionState == CMD_BUSY)
            {
                if (oiCadDebug)
                    printf("oiwfsdatumCad: %s, Cannot datum while action state BUSY\n", pcr->name);
                ERROR_MESSAGE ("Cannot datum while busy");
                return CAD_REJECT;
            }

            if (followState == 2)
            {
                if (oiCadDebug)
                    printf("oiwfsdatumCad: %s, Cannot datum while follow state FOLLOWING\n", pcr->name);
                ERROR_MESSAGE ("Cannot datum while following");
                return CAD_REJECT;
            }

            /*
             * Reject if the above checks have been passed but the probe is still BUSY.
             */

            probeState = STRING_F;
            if ( strcmp( probeState, "IDLE")    &&
                 strcmp( probeState, "ERROR")
               )
            {
                if (oiCadDebug)
                    printf( "oiwfsdatumCad: %s, probe assembly is %s.\n", pcr->name, probeState);
                sprintf( errorMessage, "OIWFS probe is %s", probeState);
                ERROR_MESSAGE (errorMessage);
                return CAD_REJECT;
            }

            /*
             * Preset bails out here before indexing anything.
             */

            if (pcr->dir == menuDirectivePRESET)
            {
                return CAD_ACCEPT;
            }


            /*
             *  Start will re-index the system by sending an INDEX command 
             *  to the oiwfs assemblyControl record.  It will also set
             *  the following state to be STOP_MODE.
             */
 
            LONG_VALA = DAR_MODE_INDEX;
            LONG_VALB = DAR_DIR_START;
            LONG_VALC = STOP_ENABLE;

            break;

        /*
         *  Invalid directives are rejected.
         */

        default:

            if (oiCadDebug)
                printf("oiwfsdatumCad: %s, invalid directive: %d\n", pcr->name, pcr->dir);
            ERROR_MESSAGE ("Invalid directive");
            return CAD_REJECT;
    }
    
    return CAD_ACCEPT;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiwfsDebugCad
 *
 * INVOCATION:
 * status = oiwfsDebugCad (pcr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pcr  (struct cadRecord *) Pointer to calling CAD record.
 *
 * FUNCTION VALUE:
 * (long) command accept/reject flag.
 *
 * PURPOSE:
 * Set the global debug level.
 *
 * DESCRIPTION:
 * Reject the command if it is not a valid debugging level.
 * Debug mode changes can happen any time.
 * 
 * CAD input field assignments....
 *      A -> requested debug level
 *
 * CAD output field assignments
 *      OUTA -> output DBUG level
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

long oiwfsDebugCad 
(
    struct cadRecord *pcr           /* CAD record structure */
)
{
    long debugLevel=DAR_DBUG_NONE;  /* Requested debugging level        */


    /*
     *  Processing depends on the state of the directive field as follows:
     */

    switch (pcr->dir)
    {
       /*
        *  Accept MARK & STOP directives, but do nothing
        */

        case menuDirectiveMARK:
        case menuDirectiveSTOP:

            return CAD_ACCEPT;


       /*
        *  CLEAR directive should clear target input
        */

        case menuDirectiveCLEAR:

            strcpy(pcr->a, "");
            db_post_events(pcr, &pcr->a, 1);
            return CAD_ACCEPT;

        /* 
         *  Preset and start reject the command if the requested debugging 
         *  level is not valid. Will accept numbered levels or named in caps.
         */
 
        case menuDirectivePRESET:
        case menuDirectiveSTART:
            
            if ( checkBuffer(pcr->a) == SHORT_INTEGER )
            {
                debugLevel = atoi(pcr->a);
                if (debugLevel < DAR_DBUG_QUIET || debugLevel > DAR_DBUG_MAX)
                {
                    if (oiCadDebug)
                        printf( "oiwfsDebugCad: %s, requested debug level out of range:%ld\n",
                                pcr->name, debugLevel );
                    ERROR_MESSAGE ("Invalid Debug Level - out of range");
                    return CAD_REJECT;
                }
            }
            else if ( strcmp(STRING_A,"QUIET") == 0) 
            {
                debugLevel = DAR_DBUG_QUIET;
            }

            else if ( strcmp(STRING_A,"NONE") == 0) 
            {
                debugLevel = DAR_DBUG_NONE;
            }

            else if (strcmp(STRING_A,"MIN") == 0) 
            {
                debugLevel = DAR_DBUG_MIN;
            }

            else if (strcmp(STRING_A,"FULL") == 0) 
            {
                debugLevel = DAR_DBUG_FULL;
            }

            else if (strcmp(STRING_A,"MAX") ==0 ) 
            {
                debugLevel = DAR_DBUG_MAX;
            }

            else 
            {
                if (oiCadDebug)
                    printf( "oiwfsDebugCad: %s, requested debug level contains bad string:\"%s\"\n", pcr->name, STRING_A);
                ERROR_MESSAGE ("Invalid Debug Level - bad string");
                return CAD_REJECT;
            }
 
            /*
             * Preset bails out here before setting debug level.
             */

            if (pcr->dir == menuDirectivePRESET)
            {
                return CAD_ACCEPT;
            }


            /* 
             *  Start writes the debugging level decoded above to the output 
             *  field VALA.
             */
 
            LONG_VALA = debugLevel;
            return CAD_ACCEPT;


        /*
         *  Invalid directives are rejected.
         */

        default:

            if (oiCadDebug)
                printf( "oiwfsDebugCad: %s, invalid directive: %d\n", pcr->name, pcr->dir);
            ERROR_MESSAGE ("Invalid directive");
            return CAD_REJECT;
    }
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiwfsFollowCad
 *
 * INVOCATION:
 * status = oiwfsFollowCad (pcr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pcr  (struct cadRecord *) Pointer to calling CAD record.
 *
 * FUNCTION VALUE:
 * (long) command accept/reject flag.
 *
 * PURPOSE:
 * Enable automatic position tracking
 *
 * DESCRIPTION:
 * Start TCS target position tracking by sending the FOLLOW command to the
 * following system.
 * 
 * CAD input field assignments....
 *      A -> OIWFS probe assembly initialized flag (0/1)
 *      B -> OIWFS probe assembly indexed flag (0/1)
 *      C -> OIWFS probe assembly interlocked flag (0/1)
 *      D -> OIWFS probe assembly state (IDLE, INITIALIZING, MOVING, etc.)
 *      E -> Master enable flag (ON/OFF)
 *
 * CAD output field assignments
 *      OUTA -> output following mode
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
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

long oiwfsFollowCad 
(
    struct cadRecord *pcr           /* CAD record structure                 */
)
{
    char *pEnd;                     /* First non-translatable character     */
    long  probeInit;                /* Has probe been initialized?          */
    long  probeIndexed;             /* Has probe been indexed?              */
    long  probeIlock;               /* Has probe been interlocked?          */
    char *probeState;               /* Probe assembly state.                */
    char *masterEnable;             /* Master enable state (ON/OFF)         */
    char  errorMessage[SCRATCH_BUF_SIZE];  /* Error message                 */

    /*
     *  Processing depends on the state of the directive field as follows:
     */

    switch (pcr->dir)
    {
       /*
        *  Accept MARK, CLEAR & STOP directives, but do nothing
        */

        case menuDirectiveMARK:
        case menuDirectiveCLEAR:
        case menuDirectiveSTOP:
            break;

        /*
         *  Preset rejects if the probe has not been initialised or indexed, or if
         *  the probe has been interlocked.
         */

        case menuDirectivePRESET:

            /*
             * Reject if the probe has been interlocked.
             */

            probeIlock = strtol (STRING_C, &pEnd, STRTOL_BASE);
            if ( probeIlock != 0 )
            {
                if (oiCadDebug)
                    printf( "oiwfsFollowCad: %s, probe assembly interlocked.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe interlocked");
                return CAD_REJECT;
            }

            /*
             * Reject if the master enable is not enabled (which typically happens while
             * the software is still booting).
             */

            masterEnable = STRING_E;
            if ( strcmp(masterEnable, "ON") )
            {
                if (oiCadDebug)
                    printf( "oiwfsFollowCad: %s, master enable not enabled - still booting?\n",
                            pcr->name);
                ERROR_MESSAGE ("No master enable - Still booting?");
                return CAD_REJECT;
            }

            /*
             * Reject if the probe has not been initialized.
             */

            probeInit = strtol (STRING_A, &pEnd, STRTOL_BASE);
            if ( probeInit != 1 )
            {
                if (oiCadDebug)
                    printf( "oiwfsFollowCad: %s, probe assembly not initialized.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe not initialized");
                return CAD_REJECT;
            }

            /*
             * Reject if the probe has not been indexed.
             */

            probeIndexed = strtol (STRING_B, &pEnd, STRTOL_BASE);
            if ( probeIndexed != 1 )
            {
                if (oiCadDebug)
                    printf( "oiwfsFollowCad: %s, probe assembly not indexed.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe not indexed");
                return CAD_REJECT;
            }

            /*
             * Reject if the probe is not in state that allows following to start
             * (e.g. following cannot be started while the probe is indexing).
             */

            probeState = STRING_D;
            if ( strcmp( probeState, "IDLE")     &&
                 strcmp( probeState, "MOVING")   &&
                 strcmp( probeState, "TRACKING") &&
                 strcmp( probeState, "STARTING") &&
                 strcmp( probeState, "ERROR")
               )
            {
                if (oiCadDebug)
                    printf( "oiwfsFollowCad: %s, probe assembly is %s.\n", pcr->name, probeState);
                sprintf( errorMessage, "OIWFS probe is %s", probeState);
                ERROR_MESSAGE (errorMessage);
                return CAD_REJECT;
            }

            break;

        /*
         *  Start sets the VALA field to the tracking enable code.
         */ 

        case menuDirectiveSTART:

            LONG_VALA = FOLLOW_MODE;
            break;

        /*
         *  Invalid directives are rejected.
         */

        default:

            if (oiCadDebug)
                printf("oiwfsFollowCad: %s, invalid directive: %d\n", pcr->name, pcr->dir);
            ERROR_MESSAGE ("Invalid directive");
            return CAD_REJECT;
    }
    
    return CAD_ACCEPT;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiwfsinitCad
 *
 * INVOCATION:
 * status = oiwfsinitCad (pcr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pcr  (struct cadRecord *) Pointer to calling CAD record.
 *
 * FUNCTION VALUE:
 * (long) command accept/reject flag.
 *
 * PURPOSE:
 * Initialize the OIWFS components
 *
 * DESCRIPTION:
 * Reject an init command if the system is busy or in following mode.
 * 
 * CAD input field assignments....
 *      A -> activeC.VAL
 *      B -> following_mode
 *      C -> OIWFS probe assembly initialized flag (0/1)
 *      D -> OIWFS probe assembly indexed flag (0/1)
 *      E -> OIWFS probe assembly interlocked flag (0/1)
 *      F -> OIWFS probe assembly state (IDLE, INITIALIZING, MOVING, etc.)
 *      G -> Master enable flag (ON/OFF)
 *
 * CAD output field assignments
 *      VALA -> oiwfs assemblyControl record mode field
 *      VALB -> oiwfs assemblyControl record directive field
 *      VALC -> disable STOP_MODE
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

long oiwfsinitCad 
(
    struct cadRecord *pcr           /* CAD record structure */
)
{
    char *pEnd;                     /* First non-translatable character     */
    long  actionState;              /* Current system action state          */
    long  followState;              /* Current system following state       */
    long  probeIlock;               /* Has probe been interlocked?          */
    char *probeState;               /* Probe assembly state.                */
    char  errorMessage[SCRATCH_BUF_SIZE];  /* Error message                 */


    /*
     *  Processing depends on the state of the directive field as follows:
     */

    switch (pcr->dir)
    {
 
       /*
        *  Accept MARK, CLEAR & STOP directives, but do nothing
        */

        case menuDirectiveMARK:
        case menuDirectiveCLEAR:
        case menuDirectiveSTOP:
            break;

        /*
         *  Preset and start will reject the command if the system is busy
         */

        case menuDirectivePRESET:
        case menuDirectiveSTART:

            /*
             * Reject if the probe has been interlocked.
             */

            probeIlock = strtol (STRING_E, &pEnd, STRTOL_BASE);
            if ( probeIlock != 0 )
            {
                if (oiCadDebug)
                    printf( "oiwfsinitCad: %s, probe assembly interlocked.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe interlocked");
                return CAD_REJECT;
            }

            /*
             *  Translate action and following states
             */

            if (strcmp(STRING_A,"IDLE") == 0)
            {
                actionState = 0;
            }

            else if (strcmp(STRING_A,"BUSY") == 0) 
            {
                actionState = 1;
            }

            else if (strcmp(STRING_A,"ERR") == 0) 
            {
                actionState = 2;
            }
            else 
            {
                if (oiCadDebug)
                    printf("oiwfsinitCad: %s, CAD action state input is invalid\n", pcr->name);
                ERROR_MESSAGE ("Invalid system action State");
                return CAD_REJECT;
            }

            followState = strtol (STRING_B, &pEnd, STRTOL_BASE);
            if (*pEnd != '\0')
            {
                if (oiCadDebug)
                    printf("oiwfsinitCad: %s, CAD follow state input is invalid\n", pcr->name);
                ERROR_MESSAGE ("Invalid system follow State");
                return CAD_REJECT;
            }


            /*
             * Reject the command if the system is busy or is in following 
             * mode
             */

            if (actionState == CMD_BUSY)
            {
                if (oiCadDebug)
                    printf("oiwfsinitCad: %s, Cannot init while action state BUSY\n", pcr->name);
                ERROR_MESSAGE ("Cannot init while busy");
                return CAD_REJECT;
            }

            else if (followState == 2)
            {
                if (oiCadDebug)
                    printf("oiwfsinitCad: %s, Cannot init while follow state FOLLOWING\n", pcr->name);
                ERROR_MESSAGE ("Cannot init while following");
                return CAD_REJECT;
            }

            /*
             * Reject if the above checks have been passed but the probe is still BUSY.
             */

            probeState = STRING_F;
            if ( strcmp( probeState, "IDLE")     &&
                 strcmp( probeState, "ERROR")
               )
            {
                if (oiCadDebug)
                    printf( "oiwfsinitCad: %s, probe assembly is %s.\n", pcr->name, probeState);
                sprintf( errorMessage, "OIWFS probe is %s", probeState);
                ERROR_MESSAGE (errorMessage);
                return CAD_REJECT;
            }


            /*
             * Preset bails out here before initializing anything.
             */

            if (pcr->dir == menuDirectivePRESET)
            {
                return CAD_ACCEPT;
            }


            /*
             *  Start will initialize the system by sending an INIT command 
             *  to the oiwfs assemblyControl record.
             */

            LONG_VALA = DAR_MODE_INIT;
            LONG_VALB = DAR_DIR_START;
            LONG_VALC = STOP_DISABLE;

            break;

        /*
         *  Invalid directives are rejected.
         */

        default:

            if (oiCadDebug)
                printf("oiwfsinitCad: %s, invalid directive: %d\n", pcr->name, pcr->dir);
            ERROR_MESSAGE ("Invalid directive");
            return CAD_REJECT;
    }

    return CAD_ACCEPT;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiwfsMoveCad
 *
 * INVOCATION:
 * status = oiwfsMoveCad (pcr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pcr  (struct cadRecord *) Pointer to calling CAD record.
 *
 * FUNCTION VALUE:
 * (long) command accept/reject flag.
 *
 * PURPOSE:
 * Position the OIWFS probe to the given coordinates
 *
 * DESCRIPTION:
 * Reject the command if the requested position is outside motion limits. 
 * Re-position the probe by setting the target position and issuing a 
 *  MOVE command to the following system.
 * 
 * CAD input field assignments....
 *      A -> requested X position
 *      B -> requested Y position
 *      C -> minimum allowable X position
 *      D -> maximum allowable X position
 *      E -> minimum allowable Y position
 *      F -> maximum allowable Y position
 *      G -> OIWFS probe assembly initialized flag (0/1)
 *      H -> OIWFS probe assembly indexed flag (0/1)
 *      I -> OIWFS probe assembly interlocked flag (0/1)
 *      J -> OIWFS probe assembly state (IDLE, INITIALIZING, MOVING, etc.)
 *      K -> Master enable flag (ON/OFF)
 *
 * CAD output field assignments
 *      VALA -> target object X position
 *      VALB -> target object Y position
 *      VALC -> following mode
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

long oiwfsMoveCad 
(
    struct cadRecord *pcr           /* CAD record structure */
)
{
    char  *pEnd;                    /* First non-translatable character     */
    double x_target;                /* Translated X target position         */
    double y_target;                /* Translated Y target position         */
    double minX;                    /* Translated X minimum position        */
    double maxX;                    /* Translated X maximum position        */
    double minY;                    /* Translated Y minimum position        */
    double maxY;                    /* Translated Y maximum position        */
    long   probeInit;               /* Has probe been initialized?          */
    long   probeIndexed;            /* Has probe been indexed?              */
    long   probeIlock;              /* Has probe been interlocked?          */
    char  *probeState;              /* Probe assembly state.                */
    char  *masterEnable;            /* Master enable state (ON/OFF)         */
    char   errorMessage[SCRATCH_BUF_SIZE];  /* Error message                */


    /*
     *  Processing depends on the state of the directive field as follows:
     */

    switch (pcr->dir)
    {
       /*
        *  Accept MARK & STOP directives, but do nothing
        */

        case menuDirectiveMARK:
        case menuDirectiveSTOP:
            return CAD_ACCEPT;

       /*
        *  CLEAR directive should clear target inputs
        */

        case menuDirectiveCLEAR:

            strcpy(pcr->a, "");
            db_post_events(pcr, &pcr->a, 1);
            strcpy(pcr->b, "");
            db_post_events(pcr, &pcr->b, 1);
            return CAD_ACCEPT;

        /*
         *  Preset or start will reject the command if the input arguments 
         *  can not be parsed into double values or if the requested probe 
         *  position is out of range.
         */

        case menuDirectivePRESET:
        case menuDirectiveSTART:

            /*
             * Reject if the probe has been interlocked.
             */

            probeIlock = strtol (STRING_I, &pEnd, STRTOL_BASE);
            if ( probeIlock != 0 )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, probe assembly interlocked.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe interlocked");
                return CAD_REJECT;
            }

            /*
             * Reject if the master enable is not enabled (which typically happens while
             * the software is still booting).
             */

            masterEnable = STRING_K;
            if ( strcmp(masterEnable, "ON") )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, master enable not enabled - still booting?\n",
                            pcr->name);
                ERROR_MESSAGE ("No master enable - Still booting?");
                return CAD_REJECT;
            }

            /*
             * Reject if the probe has not been initialized.
             */

            probeInit = strtol (STRING_G, &pEnd, STRTOL_BASE);
            if ( probeInit != 1 )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, probe assembly not initialized.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe not initialized");
                return CAD_REJECT;
            }

            /*
             * Reject if the probe has not been indexed.
             */

            probeIndexed = strtol (STRING_H, &pEnd, STRTOL_BASE);
            if ( probeIndexed != 1 )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, probe assembly not indexed.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe not indexed");
                return CAD_REJECT;
            }

            /*
             * Reject if the probe is not in state that allows it to be moved.
             */

            probeState = STRING_J;
            if ( strcmp( probeState, "IDLE")     &&
                 strcmp( probeState, "MOVING")   &&
                 strcmp( probeState, "TRACKING") &&
                 strcmp( probeState, "STARTING") &&
                 strcmp( probeState, "ERROR")
               )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, probe assembly is %s.\n", pcr->name, probeState);
                sprintf( errorMessage, "OIWFS probe is %s", probeState);
                ERROR_MESSAGE (errorMessage);
                return CAD_REJECT;
            }

            /*
             * Convert all input attributes to double values.
             */

            /* Check X target attribute */ 
            if ( checkBuffer(pcr->a)== BAD_VALUE || 
                 checkBuffer(pcr->a) == ALL_BLANKS )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, requested X target contains bad string:\"%s\"\n", pcr->name, STRING_A );
                ERROR_MESSAGE ("Invalid X target - bad string");
                return CAD_REJECT;
            }
            else
	    {
                x_target = strtod (STRING_A, &pEnd);
            }

            /* Check Y target attribute */ 
            if ( checkBuffer(pcr->b)== BAD_VALUE || 
                 checkBuffer(pcr->b) == ALL_BLANKS )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, requested Y target contains bad string:\"%s\"\n", pcr->name, STRING_B );
                ERROR_MESSAGE ("Invalid Y target - bad string");
                return CAD_REJECT;
            }
            else
	    {
                y_target = strtod (STRING_B, &pEnd);
            }

            /* Check X minimum range attribute */ 
            if ( checkBuffer(pcr->c)== BAD_VALUE || 
                 checkBuffer(pcr->c) == ALL_BLANKS )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, requested X minimum range contains bad string:\"%s\"\n", pcr->name, STRING_C );
                ERROR_MESSAGE ("Invalid X Minimum - bad string");
                return CAD_REJECT;
            }
            else
	    {
                minX = strtod (STRING_C, &pEnd);
            }

            /* Check X maximum range attribute */ 
            if ( checkBuffer(pcr->d)== BAD_VALUE || 
                 checkBuffer(pcr->d) == ALL_BLANKS )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, requested X maximum range contains bad string:\"%s\"\n", pcr->name, STRING_D );
                ERROR_MESSAGE ("Invalid X Maximum - bad string");
                return CAD_REJECT;
            }
            else
	    {
                maxX = strtod (STRING_D, &pEnd);
            }

            /* Check Y minimum range attribute */ 
            if ( checkBuffer(pcr->e)== BAD_VALUE || 
                 checkBuffer(pcr->e) == ALL_BLANKS )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, requested Y minimum range contains bad string:\"%s\"\n", pcr->name, STRING_E );
                ERROR_MESSAGE ("Invalid Y Minimum - bad string");
                return CAD_REJECT;
            }
            else
	    {
                minY = strtod (STRING_E, &pEnd);
            }

            /* Check Y maximum range attribute */ 
            if ( checkBuffer(pcr->f)== BAD_VALUE || 
                 checkBuffer(pcr->f) == ALL_BLANKS )
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, requested X maximum range contains bad string:\"%s\"\n", pcr->name, STRING_F );
                ERROR_MESSAGE ("Invalid Y Maximum - bad string");
                return CAD_REJECT;
            }
            else
	    {
                maxY = strtod (STRING_F, &pEnd);
            }


            /*
             * Make sure the targets are within the limits
             */

            if ((x_target < minX) || (x_target > maxX))
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, requested X Target out of range:%f\n", pcr->name, x_target );
                ERROR_MESSAGE ("X target out of range");
                return CAD_REJECT;
            }

            if ((y_target < minY) || (y_target > maxY))
            {
                if (oiCadDebug)
                    printf( "oiwfsMoveCad: %s, requested Y Target out of range:%f\n", pcr->name, y_target );
                ERROR_MESSAGE ("Y target out of range");
                return CAD_REJECT;
            }


            /*
             * Preset bails out here before moving anything.
             */

            if (pcr->dir == menuDirectivePRESET)
            {
                return CAD_ACCEPT;
            }


            /*
             * Start outputs the target position and sets the following mode
             * to the move code.
             */

            DOUBLE_VALA = x_target;
            DOUBLE_VALB = y_target;
            LONG_VALC = MOVE_MODE;

            return CAD_ACCEPT;


        /*
         *  Invalid directives are rejected.
         */

        default:

            if (oiCadDebug)
                printf( "oiwfsMoveCad: %s, invalid directive: %d\n", pcr->name, pcr->dir);
            ERROR_MESSAGE ("Invalid directive");
            return CAD_REJECT;
    }
    
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiwfsParkCad
 *
 * INVOCATION:
 * status = oiwfsParkCad (pcr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pcr  (struct cadRecord *) Pointer to calling CAD record.
 *
 * FUNCTION VALUE:
 * (long) command accept/reject flag.
 *
 * PURPOSE:
 * Place the probe in a safe position for shutdown
 *
 * DESCRIPTION:
 * Reject the command if the system is executing another command or 
 *    is in following mode.
 * Park the probe by sending the PARK command to the oiwfs assemblyControl
 *   record and sending the STOP command to the following system.
 * 
 * CAD input field assignments....
 *      A -> OIWFS probe assembly initialized flag (0/1)
 *      B -> OIWFS probe assembly indexed flag (0/1)
 *      C -> OIWFS probe assembly interlocked flag (0/1)
 *      D -> OIWFS probe assembly state (IDLE, INITIALIZING, MOVING, etc.)
 *      E -> Master enable flag (ON/OFF)
 *
 * CAD output field assignments
 *      VALA -> oiwfs assemblyControl record mode field
 *      VALB -> oiwfs assemblyControl record directive field
 *      VALC -> following mode
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
 * None known.
 *
 *-
 ************************************************************************
 */

long oiwfsParkCad 
(
    struct cadRecord *pcr           /* CAD record structure */
)
{
    char *pEnd;                     /* First non-translatable character     */
    long  probeInit;                /* Has probe been initialized?          */
    long  probeIndexed;             /* Has probe been indexed?              */
    long  probeIlock;               /* Has probe been interlocked?          */
    char *probeState;               /* Probe assembly state.                */
    char *masterEnable;             /* Master enable state (ON/OFF)         */
    char  errorMessage[SCRATCH_BUF_SIZE];  /* Error message                 */

    /*
     *  Processing depends on the state of the directive field as follows:
     */

    switch (pcr->dir)
    {
       /*
        *  Accept MARK, CLEAR & STOP directives, but do nothing
        */

        case menuDirectiveMARK:
        case menuDirectiveCLEAR:
        case menuDirectiveSTOP:
            break;

        /*
         *  Preset rejects if the probe has not been initialised or indexed, or if
         *  the probe has been interlocked.
         */

        case menuDirectivePRESET:

            /*
             * Reject if the probe has been interlocked.
             */

            probeIlock = strtol (STRING_C, &pEnd, STRTOL_BASE);
            if ( probeIlock != 0 )
            {
                if (oiCadDebug)
                    printf( "oiwfsParkCad: %s, probe assembly interlocked.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe interlocked");
                return CAD_REJECT;
            }

            /*
             * Reject if the master enable is not enabled (which typically happens while
             * the software is still booting).
             */

            masterEnable = STRING_E;
            if ( strcmp(masterEnable, "ON") )
            {
                if (oiCadDebug)
                    printf( "oiwfsParkCad: %s, master enable not enabled - still booting?\n",
                            pcr->name);
                ERROR_MESSAGE ("No master enable - Still booting?");
                return CAD_REJECT;
            }

            /*
             * Reject if the probe has not been initialized.
             */

            probeInit = strtol (STRING_A, &pEnd, STRTOL_BASE);
            if ( probeInit != 1 )
            {
                if (oiCadDebug)
                    printf( "oiwfsParkCad: %s, probe assembly not initialized.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe not initialized");
                return CAD_REJECT;
            }

            /*
             * Reject if the probe has not been indexed.
             */

            probeIndexed = strtol (STRING_B, &pEnd, STRTOL_BASE);
            if ( probeIndexed != 1 )
            {
                if (oiCadDebug)
                    printf( "oiwfsParkCad: %s, probe assembly not indexed.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe not indexed");
                return CAD_REJECT;
            }

            /*
             * Reject if the probe is not in state that allows it to be parked.
             */

            probeState = STRING_D;
            if ( strcmp( probeState, "IDLE")     &&
                 strcmp( probeState, "MOVING")   &&
                 strcmp( probeState, "TRACKING") &&
                 strcmp( probeState, "STARTING") &&
                 strcmp( probeState, "ERROR")
               )
            {
                if (oiCadDebug)
                    printf( "oiwfsParkCad: %s, probe assembly is %s.\n", pcr->name, probeState);
                sprintf( errorMessage, "OIWFS probe is %s", probeState);
                ERROR_MESSAGE (errorMessage);
                return CAD_REJECT;
            }

            break;


        /*
         *  Start parks the stage by issuing the PARK command to the
         *  oiwfs assemblyControl record.  The stop following command
         *  is issued as well to insure that following mode is cancelled.
         */

        case menuDirectiveSTART:

            LONG_VALA = DAR_MODE_PARK;
            LONG_VALB = DAR_DIR_START;
            LONG_VALC = STOP_MODE;

            break;


        /*
         *  Invalid directives are rejected.
         */

        default:

            if (oiCadDebug)
                printf( "oiwfsParkCad: %s, invalid directive: %d\n", pcr->name, pcr->dir);
            ERROR_MESSAGE ("Invalid directive");
            return CAD_REJECT;
    }

    return CAD_ACCEPT;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiwfsStopCad
 *
 * INVOCATION:
 * status = oiwfsStopCad (pcr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pcr  (struct cadRecord *) Pointer to calling CAD record.
 *
 * FUNCTION VALUE:
 * (long)  command accept/reject flag.
 *
 * PURPOSE:
 * Disable automatic position tracking
 *
 * DESCRIPTION:
 * Disable automatic position tracking and leave the probe where it is by
 * sending a STOP command to the following system.
 * 
 * CAD input field assignments
 *      None
 *
 * CAD output field assignments
 *      VALA -> following mode
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

long oiwfsStopCad
(
    struct cadRecord *pcr           /* CAD record structure */
)
{

    /*
     *  Processing depends on the state of the directive field as follows:
     */

    switch (pcr->dir)
    {
       /*
        *  Accept MARK, CLEAR & STOP directives, but do nothing
        */

        case menuDirectiveMARK:
        case menuDirectiveCLEAR:
        case menuDirectiveSTOP:
            break;

        /*
         *  Preset always accepts a park request!!!
         */

        case menuDirectivePRESET:

            break;


        /*
         *  Start cancels the following action by sending the STOP command
         *  to the following system.
         */

        case menuDirectiveSTART:

            LONG_VALA = STOP_MODE;

            break;


        /*
         *  Invalid directives are rejected.
         */

        default:

            if (oiCadDebug)
                printf( "oiwfsStopCad: %s, invalid directive: %d\n", pcr->name, pcr->dir);
            ERROR_MESSAGE ("Invalid directive");
            return CAD_REJECT;
    }
    
    return CAD_ACCEPT;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiwfstestCad
 *
 * INVOCATION:
 * status = oiwfstestCad (pcr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pcr  (struct cadRecord *) Pointer to calling CAD record.
 *
 * FUNCTION VALUE:
 * (long)  command accept/reject flag.
 *
 * PURPOSE:
 * Check to insure that all sub-systems are functioning normally without
 * moving anything.
 *
 * DESCRIPTION:
 * Reject a test command if the system is busy.
 * 
 * CAD input field assignments....
 *      A -> activeC.VAL
 *      B -> following_mode ?
 *      C -> OIWFS probe assembly initialized flag (0/1)
 *      D -> OIWFS probe assembly indexed flag (0/1)
 *      E -> OIWFS probe assembly interlocked flag (0/1)
 *      F -> OIWFS probe assembly state (IDLE, INITIALIZING, MOVING, etc.)
 *      G -> Master enable flag (ON/OFF)
 *
 * CAD output field assignments
 *      VALA -> oiwfs assemblyControl record mode field
 *      VALB -> oiwfs assemblyControl record directive field
 *      VALC -> following_mode passed through from input
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

long oiwfstestCad
(
    struct cadRecord *pcr           /* CAD record structure */
)
{
    char *pEnd;                     /* First non-translatable character     */
    long  actionState;              /* Current system action state          */
    long  followState;              /* Current system following state       */
    long  probeIlock;               /* Has probe been interlocked?          */
    char *probeState;               /* Probe assembly state.                */
    char  errorMessage[SCRATCH_BUF_SIZE];  /* Error message                 */


    /*
     *  Processing depends on the state of the directive field as follows:
     */

    switch (pcr->dir)
    {
       /*
        *  Accept MARK, CLEAR & STOP directives, but do nothing
        */

        case menuDirectiveMARK:
        case menuDirectiveCLEAR:
        case menuDirectiveSTOP:
            break;

        /* 
         *  Preset and start reject the command if it is not safe to execute 
         *  a test action at this time.
         */
 
        case menuDirectivePRESET:
        case menuDirectiveSTART:

            /*
             * Reject if the probe has been interlocked.
             */

            probeIlock = strtol (STRING_E, &pEnd, STRTOL_BASE);
            if ( probeIlock != 0 )
            {
                if (oiCadDebug)
                    printf( "oiwfstestCad: %s, probe assembly interlocked.\n", pcr->name);
                ERROR_MESSAGE ("OIWFS probe interlocked");
                return CAD_REJECT;
            }

            /*
             *  Convert system action and following states from 
             *  string to long. 
             */

            if (strcmp(STRING_A,"IDLE") == 0)
            {
                actionState = 0;
            }

            else if (strcmp(STRING_A,"BUSY") == 0) 
            {
                actionState = 1;
            }

            else if (strcmp(STRING_A,"ERR") == 0) 
            {
                actionState = 2;
            }

            else 
            {
                if (oiCadDebug)
                    printf("oiwfstestCad: %s, CAD action state input is invalid\n", pcr->name);
                ERROR_MESSAGE ("Invalid system action State");
                return CAD_REJECT;
            }

            followState = strtol (STRING_B, &pEnd, STRTOL_BASE);
            if (*pEnd != '\0')
            {
                if (oiCadDebug)
                    printf("oiwfstestCad: %s, CAD follow state input is invalid\n", pcr->name);
                ERROR_MESSAGE ("Invalid system follow State");
                return CAD_REJECT;
            }


            /*
             *  Reject the command if any other command is being 
             *  executed or following mode is active.
             */

            if (actionState == CMD_BUSY)
            {
                if (oiCadDebug)
                    printf("oiwfstestCad: %s, Cannot test while action state BUSY\n", pcr->name);
                ERROR_MESSAGE ("Cannot test while busy");
                return CAD_REJECT;
            }

            if (followState == 2)
            {
                if (oiCadDebug)
                    printf("oiwfstestCad: %s, Cannot test while follow state FOLLOWING\n", pcr->name);
                ERROR_MESSAGE ("Cannot test while following");
                return CAD_REJECT;
            }

            /*
             * Reject if the above checks have been passed but the probe is still BUSY.
             */

            probeState = STRING_F;
            if ( strcmp( probeState, "IDLE")     &&
                 strcmp( probeState, "ERROR")
               )
            {
                if (oiCadDebug)
                    printf( "oiwfstestCad: %s, probe assembly is %s.\n", pcr->name, probeState);
                sprintf( errorMessage, "OIWFS probe is %s", probeState);
                ERROR_MESSAGE (errorMessage);
                return CAD_REJECT;
            }

            /*
             * Preset bails out here before testing anything.
             */

            if (pcr->dir == menuDirectivePRESET)
            {
                return CAD_ACCEPT;
            }


            /* 
             *  Start will test the system by sending a TEST command to 
             *  the oiwfs assemblyControl record.  
             */
 
            LONG_VALA = DAR_MODE_TEST;
            LONG_VALB = DAR_DIR_START;
            LONG_VALC = STOP_DISABLE;

            break;

        /*
         *  Invalid directives are rejected.
         */

        default:

            if (oiCadDebug)
                printf("oiwfstestCad: %s, invalid directive: %d\n", pcr->name, pcr->dir);
            ERROR_MESSAGE ("Invalid directive");
            return CAD_REJECT;
      }

    return CAD_ACCEPT;
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiwfsToleranceCad
 *
 * INVOCATION:
 * status = oiwfsToleranceCad (pcr); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pcr  (struct cadRecord *) Pointer to calling CAD record.
 *
 * FUNCTION VALUE:
 * (long)  command accept/reject flag.
 *
 * PURPOSE:
 * Set the probe motion deadband.
 *
 * DESCRIPTION:
 * Check to make sure that the tolerance is within acceptable limits.
 * 
 * CAD input field assignments....
 *      A -> target X & Y tolerance
 *      B -> target Z tolerance
 *      C -> minimum allowable tolerance
 *      D -> maximum allowable tolerance
 *
 * CAD output field assignments
 *      VALA -> output tolerance
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
 * Attribute B (target Z tolerance) is always assumed to be 0
 * and is not checked in any way.
 *-
 ************************************************************************
 */

long oiwfsToleranceCad
(
    struct cadRecord *pcr           /* CAD record structure */
)
{
    char *pEnd;                     /* First non-translatable character */
    double deadband;                /* oiwfs X/Y tolerance              */
    double minD;                    /* minimum allowable                */
    double maxD;                    /* maximum allowable                */
    double deadbandZ;               /* oiwfs Z tolerance                */


    /*
     *  Processing depends on the state of the directive field as follows:
     */

    switch (pcr->dir)
    {
       /*
        *  Accept MARK, START & STOP directives, but do nothing
        */

        case menuDirectiveMARK:
        case menuDirectiveSTART:
        case menuDirectiveSTOP:
            break;

       /*
        *  CLEAR directive should clear target inputs
        */

        case menuDirectiveCLEAR:

            strcpy(pcr->a, "");
            db_post_events(pcr, &pcr->a, 1);
            strcpy(pcr->b, "");
            db_post_events(pcr, &pcr->b, 1);
            return CAD_ACCEPT;

        /* 
         *  Preset rejects the command if the inputs are not valid.
         */
 
        case menuDirectivePRESET:

            /*
             *  Reject the command if any of the inputs can not be converted
             *  from sting to double values.
             */

            /* Check X/Y Tolerance Target  attribute */ 
            if ( checkBuffer(pcr->a)== BAD_VALUE || 
                 checkBuffer(pcr->a) == ALL_BLANKS )
            {
                if (oiCadDebug)
                    printf( "oiwfsToleranceCad: %s, requested X/Y tolerance contains bad string:\"%s\"\n", pcr->name, STRING_A );
                ERROR_MESSAGE ("Invalid X/Y tolerance - bad string");
                return CAD_REJECT;
            }
            else
	    {
                deadband = strtod (STRING_A, &pEnd);
            }

            /* Check Z Tolerance attribute */ 
            if ( checkBuffer(pcr->b)== BAD_VALUE || 
                 checkBuffer(pcr->b) == ALL_BLANKS )
            {
                if (oiCadDebug)
                    printf( "oiwfsToleranceCad: %s, requested Z target contains bad string:\"%s\"\n", pcr->name, STRING_B );
                ERROR_MESSAGE ("Invalid Z tolerance - bad string");
                return CAD_REJECT;
            }
            else
	    {
                deadbandZ = strtod (STRING_B, &pEnd);
            }


            /* Check X/Y minimum tolerance range attribute */ 
            if ( checkBuffer(pcr->c)== BAD_VALUE || 
                 checkBuffer(pcr->c) == ALL_BLANKS )
            {
                if (oiCadDebug)
                    printf( "oiwfsToleranceCad: %s, requested X/Y minimum range contains bad string:\"%s\"\n", pcr->name, STRING_C );
                ERROR_MESSAGE ("Invalid X/Y Tolerance Min - bad string");
                return CAD_REJECT;
            }
            else
	    {
                minD = strtod (STRING_C, &pEnd);
            }

            /* Check X/Y maximum tolerance range attribute */ 
            if ( checkBuffer(pcr->d)== BAD_VALUE || 
                 checkBuffer(pcr->d) == ALL_BLANKS )
            {
                if (oiCadDebug)
                    printf( "oiwfsToleranceCad: %s, requested X/Y maximum range contains bad string:\"%s\"\n", pcr->name, STRING_D );
                ERROR_MESSAGE ("Invalid X/Y Tolerance Max - bad string");
                return CAD_REJECT;
            }
            else
	    {
                maxD = strtod (STRING_D, &pEnd);
            }


            /*
             * Reject the command if the given deadband is outside limits
             */

            if ((deadband < minD) || (deadband > maxD))
            {
                if (oiCadDebug)
                    printf( "oiwfsToleranceCad: %s, requested Tolerance out of range:%f\n", pcr->name, deadband );
                ERROR_MESSAGE ("X/Y Tolerance out of range");
                return CAD_REJECT;
            }

            /*
             * Give warning if the given Z deadband is not == 0
             */

            if (deadbandZ != 0)
            {
                if (oiCadDebug)
                    printf( "oiwfsToleranceCad: %s, Warning - Z Tolerance assumed to be 0:%f\n", pcr->name, deadbandZ );
            }

            /*
             *  All is okay so update the following system deadband value.
             *  Keep in mind that the routing of this output should be
             *  controlled by the STLK link.
             */

            DOUBLE_VALA = deadband;

            break;

        /*
         *  Invalid directives are rejected.
         */

        default:

            if (oiCadDebug)
                printf( "oiwfsToleranceCad: %s, invalid directive: %d\n", pcr->name, pcr->dir);
            ERROR_MESSAGE ("Invalid directive");
            return CAD_REJECT;
    }
    
    return CAD_ACCEPT;
}

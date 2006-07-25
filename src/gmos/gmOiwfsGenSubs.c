/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) <year>                           (c) <year>
 * National Research Council            Conseil national de recherches
 * Ottawa, Canada, K1A 0R6              Ottawa, Canada, K1A 0R6
 * All rights reserved                  Tous droits reserves
 *                                      
 * NRC disclaims any warranties,        Le CNRC denie toute garantie
 * expressed, implied, or statu-        enoncee, implicite ou legale,
 * tory, of any kind with respect       de quelque nature que se soit,
 * to the software, including           concernant le logiciel, y com-
 * without limitation any war-          pris sans restriction toute
 * ranty of merchantability or          garantie de valeur marchande
 * fitness for a particular pur-        ou de pertinence pour un usage
 * pose.  NRC shall not be liable       particulier.  Le CNRC ne
 * in any event for any damages,        pourra en aucun cas etre tenu
 * whether direct or indirect,          responsable de tout dommage,
 * special or general, consequen-       direct ou indirect, particul-
 * tial or incidental, arising          ier ou general, accessoire ou
 * from the use of the software.        fortuit, resultant de l'utili-
 *                                      sation du logiciel.
 *
 ************************************************************************
 *
 * FILENAME
 * oiwfsGenSubs.c
 *
 * PURPOSE:
 * Gensub record support for GMOS OIWFS system.
 *
 * FUNCTION NAME(S)
 * oiAngles2Position - Convert current base and pickoff angles to position
 * oiCalcAbsAngles   - Calculate current angles from absolute encoders
 * oiFollowA         - Convert target position to Mask frame of reference
 * oiProbeOffset     - Make Offset info available for engineering
 * oiProbeMap        - Build an array of coefficients for probe map corrections
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.5  2006/04/12 02:57:53  gemvx
 * implemented OIWFS probe mapping
 *
 * Revision 1.4  2005/12/28 00:32:47  gemvx
 * added Probe Map Correction
 *
 * Revision 1.3  2004/03/08 21:03:52  gemvx
 * *** empty log message ***
 *
 * Revision 1.2  2003/01/31 14:12:46  gemvx
 * Merged gmos-south
 *
 * Revision 1.1  2001/11/28 20:08:49  mbec
 * *** empty log message ***
 *
 * Revision 1.3  2001/05/09 19:20:39  smb
 * New focus settings: Changed to pass the Z offset, which is now a 75.0mm nominal) in with the rest of the offsets using gmOiwfsSetOffsets. Removed z-offset subtraction since it now happens in gmOiwfsCalc.c.
 *
 * Revision 1.10  2001/03/20 13:40:30  gmos
 * Modified DEBUG macro. All files now use printf() rather than logMsg(). All also print the output from taskName(0).
 *
 * Revision 1.9  2000/11/03 13:28:28  gmos
 * Change arrayS to LATE when the target stream is ok but the probe is not in position.
 *
 * Revision 1.8  2000/11/02 19:56:17  gmos
 * Add another follow mode in oiFollowA() to split the move command into
 * one part that sends the target and another that checks "in position".
 * This will allow the assembly to respond to all move commands.
 *
 * Revision 1.7  2000/11/02 00:22:30  gmos
 * Total rework of oiFollowA().  Remove oiFollowToMeasuring()
 * because this function is now done in followA genSub.
 *
 * Revision 1.6  2000/10/27 16:22:18  gmos
 * Reset the stream count in oiFollow when new trackId detected.
 *
 * Revision 1.5  2000/10/16 23:24:50  gmos
 * Changes to definitions for AbsAngles.  Changed equations for absolute X/Y
 * in oiCalcAbsAngles().  Removed everything related to oiCalcIndexOffset().
 * Lots of additions and corrections to comments.  Changes to oiFollowA()
 * including addition of OUT_NEW_TRACK.  This requires v1.4 or higher of
 * gmOiwfsCalc.h
 *
 * Revision 1.4  2000/05/30 16:29:10  gmos
 * New version commented by William Rambold. Master enable bug fixed.
 *
 * Revision 1.3  2000/04/15 11:38:56  gmos
 * Angelics updated OIWFS software
 *
 * Revision 1.15  2000/04/13 17:46:17  angelic
 * IN_POSITION is unset when array_S goes to 1
 *
 * Revision 1.14  2000/04/12 23:45:07  angelic
 * added DEBUG macro to use in place of printf
 * prints messages when gmOiwfsFollowDebug is true
 *
 * Revision 1.13  2000/04/12 01:00:24  angelic
 * added ideal focal distance offset subtraction to angles2position
 * handle angle offset returned by calculateProbePosition
 *
 * Revision 1.12  2000/04/07 16:41:29  angelic
 * Added support for probeOffset genSub
 * Added support for measure SIR record (may need a sir c file?)
 * generate array_s, in_position flags in followA for sir records
 * create and pass on offset structure for the TCS
 *
 * Revision 1.11  2000/02/15 23:34:24  angelic
 * removed record support for transformTarget ... now using followA
 *
 * Revision 1.10  2000/02/01 22:13:33  angelic
 * replaced record support for transformTarget gensub as followA is not
 * yet in use.
 *
 * Revision 1.9  1999/12/16 20:27:08  angelic
 * implemented follow mode
 *
 * Revision 1.8  1999/10/08 19:02:03  wooff
 * Patrol limits are now CalcAngles genSub inputs.
 *
 * Revision 1.7  1999/10/04 23:51:50  wooff
 * Now ensures X/Y is within patrol area in oiPosition2Angles.
 *
 * Revision 1.6  1999/09/22 23:26:43  wooff
 * Removed offset calls from oiPosition2Angles 
 * (via gmOiwfsCalculateProbePosition) and oiAngles2Position 
 * (via gmOiwfsCalculateProbeAngles) and made them only
 * from oiPosition2Angles (via gmOiwfsSetOffsets).
 *
 * Revision 1.5  1999/09/17 23:41:53  wooff
 * Fixed typo.
 *
 * Revision 1.4  1999/09/17 17:35:27  wooff
 * Added output focus (zPos) to oiAngles2Position and shifted current rotation
 * down.
 *
 * Revision 1.3  1999/09/15 23:19:19  wooff
 * Added IN_Y_ABS_OFFSET and IN_X_ABS_OFFSET to oiCalcAbsAngles and
 * included them in calculations for outputting X and Y based on
 * the sin/cos pots.
 *
 * Revision 1.2  1999/09/10 16:51:37  wooff
 * Change definitions for offset inputs in CalcAngles and
 * CalcPosition.  Also a major cleanup of names in general.
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

#include <stdioLib.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vxWorks.h>

#include <dbDefs.h>
#include <recSup.h>
#include <logLib.h>
#include <tickLib.h>
#include <taskLib.h>

#include <genSubRecord.h>

#include <gmOiwfsCalc.h>


/****************************************************************************
 *  --- FollowA gensub record --- 
 *  input field access mnemonics 
 */

#define IN_X_TARGET  *(double *)pgs->a    /* X Position Target              */
#define IN_Y_TARGET  *(double *)pgs->b    /* Y Position Target              */
#define IN_CURPOS    ((double *)pgs->c)   /*(X,Y,Z,R) pos from CalcPosition */
#define IN_X_HI_LIM  *(double *)pgs->d    /* Probe travel X upper limit     */
#define IN_X_LO_LIM  *(double *)pgs->e    /* Probe travel X lower limit     */
#define IN_Y_HI_LIM  *(double *)pgs->f    /* Probe travel Y upper limit     */
#define IN_Y_LO_LIM  *(double *)pgs->g    /* Probe travel Y lower limit     */
#define IN_FOLLOW      *(long *)pgs->h    /* Current follow Mode            */
#define IN_TOLERANCE *(double *)pgs->i    /* Tolerance (tracking deadband)  */
#define IN_STREAM    ((double *)pgs->j)   /* Input target stream to follow  */
#define IN_EO          *(long *)pgs->k    /* Electronic offsets flag        */
#define IN_COEFF    ((double *)pgs->u)    /* Probe map coefficients in */

/*
 *  --- FollowA gensub record ---
 *  output field access mnemonics
 */

#define OUT_REJECT      *(long *)pgs->vala  /* Inhibit assembly directive   */
#define OUT_FOLLOW_S    *(long *)pgs->valb  /* Flag to drive followS in SAD */
#define OUT_NEW_TRACK   *(long *)pgs->valc  /* Flag for new trackID         */
#define OUT_REJ_OFFSET  *(long *)pgs->vald  /* Inhibit probe offsets        */
#define OUT_X_MASK    *(double *)pgs->vale  /* X target in Mask frame       */
#define OUT_Y_MASK    *(double *)pgs->valf  /* Y target in Mask frame       */
#define OUT_IN_POSITION *(long *)pgs->valg  /* In Position flag for the SAD */
#define OUT_ARRAY_S     *(long *)pgs->valh  /* Array valid flag for the SAD */
#define OUT_STREAM    ((double *)pgs->valj) /* Probe offset stream out      */


/****************************************************************************
 *  --- ProbeMap gensub record --- 
 *  input field access mnemonics 
 */

#define IN_A0  *(double *)pgs->a    /* transformation term              */
#define IN_A1  *(double *)pgs->b    /* transformation term              */
#define IN_A2  *(double *)pgs->c    /* transformation term              */
#define IN_A3  *(double *)pgs->d    /* transformation term              */
#define IN_A4  *(double *)pgs->e    /* transformation term              */
#define IN_A5  *(double *)pgs->f    /* transformation term              */
#define IN_B0  *(double *)pgs->g    /* transformation term              */
#define IN_B1  *(double *)pgs->h    /* transformation term              */
#define IN_B2  *(double *)pgs->i    /* transformation term              */
#define IN_B3  *(double *)pgs->j    /* transformation term              */
#define IN_B4  *(double *)pgs->k    /* transformation term              */
#define IN_B5  *(double *)pgs->l    /* transformation term              */

/*
 *  --- ProbeMap gensub record ---
 *  output field access mnemonics
 */

#define OUT_COEFF    ((double *)pgs->vala) /* Probe map coefficients out */


/****************************************************************************
 *  --- probeOffset gensub record --- 
 *  input field access mnemonics
 */

#define IN_OFFSET    ((double *)pgs->a)     /* Probe offset structure in   */

/*
 *  --- probeOffset gensub record --- 
 *  output field access mnemonics
 */

#define OUT_X_OFF    *(double *)pgs->vala   /* X offset in mm               */
#define OUT_Y_OFF    *(double *)pgs->valb   /* Y offset in mm               */
#define OUT_Z_OFF    *(double *)pgs->valc   /* Z offset in mm               */
#define OUT_R_OFF    *(double *)pgs->vald   /* angle offset in mm           */
#define OUT_TRACK_ID *(double *)pgs->vale   /* track ID                     */
#define OUT_OFFSET   ((double *)pgs->valj)  /* Probe offset stream out      */


/****************************************************************************
 *  --- AbsAngles genSub ---
 *  input field access mnemonics
 */

/* Offsets added absolute angles to make them zero when stages are indexed  */ 
#define IN_BAS_ABS_OFFSET  *(double *)pgs->a  /* Base offset angle          */
#define IN_PKO_ABS_OFFSET  *(double *)pgs->b  /* Pickoff offset angle       */
                         /* c-f defined in CalcPosition */
/* Raw Sin/Cos values from the absolute encoders                            */
#define IN_BAS_SINE    *(double *)pgs->g    /* Current base sine value      */
#define IN_BAS_COSINE  *(double *)pgs->h    /* Current base cosine value    */
#define IN_PKO_SINE    *(double *)pgs->i    /* Current pickoff sine value   */
#define IN_PKO_COSINE  *(double *)pgs->j    /* Current pickoff cosine value */

/*
 *  --- AbsAngles genSub ---
 *  output field access mnemonics
 */

/* Angles calculated from sin/cos pots and including index zeroing offsets  */
#define OUT_BAS_ANGLE  *(double *)pgs->vala /* approx. absolute base angle  */
#define OUT_PKO_ANGLE  *(double *)pgs->valb /* approx. absolute pickoff     */
/* Approximate X/Y probe position calculated from angles above              */
#define OUT_X_POSITION *(double *)pgs->valc /* calculated X probe position  */
#define OUT_Y_POSITION *(double *)pgs->vald /* calculated Y probe position  */

/****************************************************************************
 *  --- LimitCheck genSub ---
 *  input and output field access mnemonics
 */

#define IN_ABS_X  *(double *)pgs->b  /* Absolute X Position */
#define IN_ABS_Y  *(double *)pgs->d  /* Absolute Y Position */
#define IN_X_NL   *(double *)pgs->f  /* X Negative Limit */
#define IN_X_PL   *(double *)pgs->h  /* X Positive Limit */
#define IN_Y_NL   *(double *)pgs->j  /* Y Negative Limit */
#define IN_Y_PL   *(double *)pgs->l  /* Y Positive Limit */
#define IN_BUFFER *(double *)pgs->o  /* Y Buffer btw limit and patrol area */

#define OUT_MESS     (char *)pgs->vala /* Limit Status */
#define OUT_Y_LL  *(double *)pgs->valc /* Effective Y Lo Limit */
#define OUT_Y_HL  *(double *)pgs->vale /* Effective Y Hi Limit */
#define OUT_ILOCK   *(long *)pgs->vals /* Interlock Status */
#define OUT_SDIS    *(long *)pgs->valu /* Interlock Out SDIS */


/****************************************************************************
 *  --- CalcPosition genSub record ---
 *  input field access mnemonics
 */ 

 /* Current angles from deviceControl records                               */
#define IN_B_ENCODER  *(double *)pgs->a    /* Base Angle                    */
#define IN_P_ENCODER  *(double *)pgs->b    /* Pickoff Angle                 */
 /* Large offsets to convert angles from encoder to mask frame of reference */
#define IN_B_OFFSET   *(double *)pgs->c    /* Base Angle Offset             */
#define IN_P_OFFSET   *(double *)pgs->d    /* Pickoff Angle Offset          */
 /* Correction offsets to convert positions from encoder to mask FOR        */
#define IN_X_OFFSET   *(double *)pgs->e    /* X Position Offset             */
#define IN_Y_OFFSET   *(double *)pgs->f    /* Y Position Offset             */
#define IN_Z_OFFSET   *(double *)pgs->g    /* Focal plane focus offset      */

/*
 *  --- CalcPosition genSub record ---
 *  output field access mnemonics
 */

#define OUT_X_CURRENT *(double *)pgs->vala  /* Current X Position           */
#define OUT_Y_CURRENT *(double *)pgs->valb  /* Current Y Position           */
#define OUT_Z_CURRENT *(double *)pgs->valc  /* Current Calculated Focus     */
#define OUT_R_CURRENT *(double *)pgs->vald  /* Current Rotation             */
#define OUT_CURPOS    ((double *)pgs->vale) /* (X,Y,Z,R) in mask coordinates*/



/*
 *  Local Defines
 */

#define OI_PRESET_ANGLE   -2.0              /* target angle for final index */ 
#define CALC_SUCCESS      0                 /* calculation results valid    */
#define CALC_FAILURE      -1                /* calculation results invalid  */   
#define D2R              (M_PI/180.0)       /* Convert degrees to radians   */
#define R2D              (1.0/D2R)          /* Convert radians to degrees   */
#define ARRAY_GOOD        0                 /* Demand stream achievable     */ 
#define ARRAY_INVALID     1                 /* Demand stream not achievable */
#define ARRAY_LATE        2                 /* Demand stream is late        */
#define FOLLOW_STOP       0                 /* Stop following/Moving        */
#define FOLLOW_MOVE       1                 /* Move mode - send target      */
#define FOLLOW_FOLLOW     2                 /* Follow mode                  */
#define FOLLOW_MOVE_R     3                 /* Move mode - read position    */
#define SCAN_DISABLE     -1                 /* Disable record scanning      */
#define SCAN_ENABLE       0                 /* Enable record scanning       */
    /*  
     * The demand stream from the TCS will come into input J of the
     * followA genSub and is expected to be at a rate of about 20Hz.
     * The next two definitions are magic numbers that are assigned
     * to stream_count.  FOLLOW_RECENTER is the number used to
     * reduce number of updates while following and FOLLOW_CENTER is
     * used to show that the previous stream was not used.
     */

#define FOLLOW_RECENTER  20        /* When out of tolerance, recenter once 
                                        every FOLLOW_CENTER stream updates */
#define FOLLOW_RESUME    -1        /* Resume following target from
                                        previous trackID                   */
#define num_elements     20      /* number of values to keep in the potentiometer array */

/*
 *  Gensub record support function prototypes
 */

long oiAngles2Position (genSubRecord *);
long oiInitCalcAbsAngles (genSubRecord *);
long oiCalcAbsAngles (genSubRecord *);
long oiLimitCheck (genSubRecord *);
long oiInitLimitCheck (genSubRecord *);
long oiFollowA (genSubRecord *);
long oiProbeOffset(genSubRecord *);
long oiProbeMap(genSubRecord *);

/*
 * Local function prototypes 
 */

long insert_pots (double, double, double, double);
long get_filtered_pots (double *, double *, double *, double *);
static void next_ptr (int *);
static void prev_ptr (int *);

/*
 *  Global Variables
 */

typedef struct {
  double    sinBas;
  double    cosBas; 
  double    sinPko;
  double    cosPko;
}OI_POT_ARRAY;

static  OI_POT_ARRAY pots[num_elements];
static int start_ptr;           /* first sequential element of the pot array */
static int end_ptr;             /* last sequential element of the pot array */
static int limitState;          /* current limit state */
long    prev_follow;            /* previous follow mode                     */
double  prev_x;                 /* previous x target                        */
double  prev_y;                 /* previous y target                        */
long    stream_count;           /* number of targets since last start cmd   */
int     gmOiwfsFollowDebug = 0; /* debug output level flag                  */
long    newTrack = TRUE;        /* indicates 1st pass with new trackID      */
int     gmOiwfsFilter = 1;      /* turn software filtering on/off           */
int     oiDebugLevel=0;
int     oiPutDebugLevel;

/*
 *  Debug macro to control debugging information output
 */

#define DEBUG(FMT,V)                                                        \
{                                                                           \
        if (gmOiwfsFollowDebug)                                             \
             printf ("%s: "FMT, taskName(0), tickGet(), pgs->name, V);                         \
}

/*
 ************************************************************************
 *+
 *  FUNCTION: next_ptr
 *
 * RETURNS: void
 *
 * DESCRIPTION: Increment an array pointer sequentially.
 * Wraps around to beginning if the end of the array is reached.
 *
 * [NOTES:]:
 *-
 ************************************************************************
 */

static void next_ptr
(
    int *ptr
)
{
  (*ptr)++;

  if (*ptr >= num_elements) *ptr = 0;

  return;
}

/*
 ************************************************************************
 *+
 *  FUNCTION: prev_ptr
 *
 * RETURNS: void
 *
 * DESCRIPTION: Decrement an array pointer sequentially.
 * Wraps around to end if the beginning of the array is reached.
 *
 * [NOTES:]:
 *-
 ************************************************************************
 */

static void prev_ptr
(
    int *ptr
)
{
  (*ptr)--;

  if (*ptr < 0) *ptr = num_elements-1;

  return;
}


/*
 ************************************************************************
 *+
 *  FUNCTION: insert_pots
 *
 * RETURNS: long, processing status
 *
 * DESCRIPTION: Insert a new pots into the pots_array structure
 * New point is added at position pots_ptr.  pots_ptr increments
 * and wraps around to zero as a ring buffer.
 *
 * [NOTES:]:
 *-
 ************************************************************************
 */

long insert_pots
(
    double sinBas,
    double cosBas,
    double sinPko,
    double cosPko
)
{

  next_ptr(&end_ptr);

  pots[end_ptr].sinBas = sinBas;
  pots[end_ptr].cosBas = cosBas;
  pots[end_ptr].sinPko = sinPko;
  pots[end_ptr].cosPko = cosPko;

  if (oiPutDebugLevel) 
    {
      printf("\nInserted pots at %d.  sinBas: %f, cosBas: %f, sinPko: %f, cosPko: %f\n", end_ptr,
             sinBas, cosBas, sinPko, cosPko);
    }

  if (start_ptr == end_ptr)
    {
      next_ptr(&start_ptr);
    }

  return CALC_SUCCESS;
}


/*
 ************************************************************************
 *+
 *  FUNCTION: get_filtered_pots
 *
 * RETURNS: long, processing status
 *
 * DESCRIPTION: get a set of pot values from the queue, after filtering
 *
 * [NOTES:]:
 *-
 ************************************************************************
 */

long get_filtered_pots
(
    double *sinBas,
    double *cosBas,
    double *sinPko,
    double *cosPko
)
{
  int ptr;         /* array pointer */
  int cnt;       /* in case the array isn't full */
  double sbSum, cbSum, spSum, cpSum;
  sbSum = 0;
  cbSum = 0;
  spSum = 0;
  cpSum = 0;
  cnt = 0;

  if ( oiDebugLevel >=2 )
     printf("\nget_filtered_pots:  end_ptr=%d, start_ptr=%d \n", end_ptr, start_ptr);

  ptr = start_ptr;  /* start of structure */

  /* 
   * Simplest of filters ... average
   */

  while (ptr != end_ptr)
    {
      next_ptr(&ptr);
      sbSum = sbSum + pots[ptr].sinBas;
      cbSum = cbSum + pots[ptr].cosBas;
      spSum = spSum + pots[ptr].sinPko;
      cpSum = cpSum + pots[ptr].cosPko;
      cnt ++;
    }

  *sinBas = sbSum/cnt;
  *cosBas = cbSum/cnt;
  *sinPko = spSum/cnt;
  *cosPko = cpSum/cnt;

  return CALC_SUCCESS;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiCalcProbeX
 *
 * INVOCATION:
 * status = oiCalcProbeX(xmask,ymask);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>)xmask  Mask X coordinate
 * (>)ymask  Mask Y coordinate
 *
 * FUNCTION VALUE:
 * (double) xnew  Transformed X coordinate
 *
 * PURPOSE:
 * Apply Probe Map Correction to the mask X coordinate
 *
 * DESCRIPTION:
 * Calculate the new mask X coordinate using the Probe Map coefficients
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

double oiCalcProbeX
(
    struct genSubRecord *pgs,        /* calling record structure             */
    double xmask,       /* mask X coord */
    double ymask        /* mask Y coord */
)
{
  double xnew;
  DEBUG("<%ld> %s: oiCalcProbeX xmask: %f\n",xmask);
  xnew = IN_COEFF[0] + IN_COEFF[1]*xmask + IN_COEFF[2]*xmask*xmask + IN_COEFF[3]*ymask
    + IN_COEFF[4]*ymask*ymask + IN_COEFF[5]*xmask*ymask;
  DEBUG("<%ld> %s: oiCalcProbeX xnew: %f\n",xnew);  
  return xnew;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiCalcProbeY
 *
 * INVOCATION:
 * status = oiCalcProbeY(xmask,ymask);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>)xmask  Mask X coordinate
 * (>)ymask  Mask Y coordinate
 *
 * FUNCTION VALUE:
 * (double) ynew  Transformed X coordinate
 *
 * PURPOSE:
 * Apply Probe Map Correction to the mask Y coordinate
 *
 * DESCRIPTION:
 * Calculate the new mask Y coordinate using the Probe Map coefficients
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

double oiCalcProbeY
(
    struct genSubRecord *pgs,        /* calling record structure             */
    double xmask,       /* mask X coord */
    double ymask        /* mask Y coord */
)
{
  double ynew;

  DEBUG("<%ld> %s: oiCalcProbeY ymask: %f\n",ymask);
  ynew = IN_COEFF[6] + IN_COEFF[7]*xmask + IN_COEFF[8]*xmask*xmask + IN_COEFF[9]*ymask
    + IN_COEFF[10]*ymask*ymask + IN_COEFF[11]*xmask*ymask;
  DEBUG("<%ld> %s: oiCalcProbeY ynew: %f\n",ynew);
  return ynew;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiAngles2Position
 *
 * INVOCATION:
 * status = oiAngles2Position (pgs);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pgs (struct genSubRecord *)  Pointer to calling genSub record structure
 *
 * FUNCTION VALUE:
 * (long) processing status.
 *
 * PURPOSE:
 * Convert base and pickoff angles to x/y position 
 *
 * DESCRIPTION:
 * Calculate the actual probe position in the mask frame of reference using
 * the current base and pickoff stage angles reported by the incremental 
 * encoders. 
 * All calculation offset parameters are updated first in case they have been
 * adjusted since the last calculation.
 * 
 * Gensub Inputs:
 *    A -> Current Base Angle from deviceControl record
 *    B -> Current Pickoff Angle from deviceControl record
 *    C -> Base Angle Offset from index point to probe coordinate origin 
 *    D -> Pickoff Angle Offset from index point to probe coordinate origin
 *    E -> X Position Offset from index point to probe coordinate origin
 *    F -> Y Position Offset from index point to probe coordinate origin
 *    G -> Z (virtual focus) Offset from index point to probe coordinate origin
 *
 * Gensub outputs:
 *    VALA ->  Current probe X Position in mask frame of reference 
 *    VALB ->  Current probe Y Position in mask frame of reference
 *    VALC ->  Current (virtual) focus offset
 *    VALD ->  Current probe frame of reference rotation
 *    VALE ->  Probe position structure (X,Y,Z,R) from the above
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

long oiAngles2Position 
(
    struct genSubRecord *pgs        /* calling record structure             */
)
{
    long status;                    /* function return status               */
    double xPos;                    /* calculated probe X position          */
    double yPos;                    /* calculated probe Y position          */
    double zPos;                    /* calculated virtual Z position        */
    double angle;                   /* calculated probe axis rotation angle */


    /*
     *  Update offsets for future oiwfs calculations
     */

    gmOiwfsSetOffsets ( IN_B_OFFSET * D2R, 
                        IN_P_OFFSET * D2R, 
                        IN_X_OFFSET, 
                        IN_Y_OFFSET,
                        IN_Z_OFFSET );

    /*
     *  Reconstruct the current probe position from the base and pickoff
     *  angles reported by the incremental shaft encoders.
     */

    status = gmOiwfsCalculateProbePosition (IN_B_ENCODER * D2R,
                 IN_P_ENCODER * D2R, &xPos, &yPos, &zPos, &angle);


    /*
     *  If the calculation was successful then update the position 
     *  output fields.
     */

    if (status == CALC_SUCCESS);
    {
        OUT_X_CURRENT = xPos;
        OUT_Y_CURRENT = yPos;
        OUT_Z_CURRENT = zPos;
        OUT_R_CURRENT = angle;

        /*
         * Update the probe position structure.... still in mask frame of
         * reference.
         */

        OUT_CURPOS[0] = xPos;
        OUT_CURPOS[1] = yPos;
        OUT_CURPOS[2] = zPos;
        OUT_CURPOS[3] = angle;
    }

    return (status); 
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiLimitCheck
 *
 * INVOCATION:
 * status = oiLimitCheck (pgs); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pgs  (genSubRecord *) Pointer to gensub record structure.
 *
 *
 * FUNCTION VALUE:
 * (long) function return status.
 *
 * PURPOSE:
 * Check Absolute position against limits.
 *
 * DESCRIPTION:
 * Check absolute positions calculated from the sin and cos pots against
 * field of travel limits set in the MOVE Gensub.
 *
 * Gensub input fields:
 *    B -> Absolute X Position
 *    D -> Absolute Y Position
 *    F -> X Negative Limit
 *    H -> X Positive Limit
 *    J -> Y Negative Limit
 *    L -> Y Positive Limit
 *
 * Gensub output fields:
 *    VALA -> calculated base stage angle from potentiometers
 *    VALB -> calculated pickoff stage angle from potentiometers
 *    VALC -> calculated X probe position from potentiometers
 *    VALD -> calculated Y probe position from potentiometers
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

long oiInitLimitCheck
(
    genSubRecord *pgs               /* calling record structure             */
)
{
  IN_BUFFER = 3.0; /* some buffer btw patrol area and limit ... 3mm is a guess */
  limitState = 0;
  OUT_SDIS = 1; /* disable ILOCK Out */
  OUT_ILOCK = 0; /* No Interlock */
  strcpy(OUT_MESS, "No Limit Detected");
  return 0;
}

long oiLimitCheck
(
    genSubRecord *pgs               /* calling record structure             */
)
{
  /* 
   * The main collision is in the Y axis, so only check the y axis here.
   * The Hard stop nedds to be a bit beyond the move limits to allow for 
   * inaccuracy in the absolute encoders.  This space was also designed into
   * the system.  This is temporarily replacing the hard limits.
   * Also need to tweak the move limits to allow for indexing and parking 
   */

  /* 
   * NOTE: This sets an interlock to stop the motor.  The regular
   * interlock system will override this interlock automatically after a
   * brief amount of time.  Since index is lost, the assembly will have to 
   * be indexed before continuing ... which will move the OIWFS away from the
   * Limit.
   */

  /* set the effective limits */
  OUT_Y_LL = IN_Y_NL - IN_BUFFER;
  OUT_Y_HL = IN_Y_PL + IN_BUFFER;

  if ( IN_ABS_Y < OUT_Y_LL) {
    if (limitState == 0) {
      limitState = 1;
      OUT_ILOCK = 1;
      OUT_SDIS = 0;
      strcpy(OUT_MESS, "Y Negative Limit Detected");
      /*printf ("\noiLimitCheck: Y Negative Limit Detected\n");*/
    } else {
      OUT_SDIS = 1; /* disable ILOCK Out */
    }
  } else if ( IN_ABS_Y > OUT_Y_HL) {
    if (limitState == 0) {
      limitState = 1;
      OUT_ILOCK = 1;
      OUT_SDIS = 0;
      strcpy(OUT_MESS, "Y Positive Limit Detected");
      /*printf ("\noiLimitCheck: Y Positive Limit Detected\n");*/
    } else {
      OUT_SDIS = 1; /* disable ILOCK Out */
    }
  } else {
    limitState = 0;
    OUT_SDIS = 1; /* disable ILOCK Out */
    OUT_ILOCK = 0; /* No Interlock */
    strcpy(OUT_MESS, "No Limit Detected");
  } 
  return 0;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiCalcAbsAngles
 *
 * INVOCATION:
 * status = oiCalcAbsAngles (pgs); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pgs  (genSubRecord *) Pointer to gensub record structure.
 *
 *
 * FUNCTION VALUE:
 * (long) function return status.
 *
 * PURPOSE:
 * Calculate current oiwfs stage positions using absolute encoders.
 *
 * DESCRIPTION:
 * Derives bas angle, pko angle and probe (rotated) X-Y position
 * from sine/cosine encoder data. The two *ABS_OFFSET values are used
 * to make the zero point from the absolute encoders equal to the zero 
 * points from the motors (which use the incremental encoders).
 *
 * Gensub input fields:
 *    A -> Base offset angle to zero and agree with incremental encoder
 *    B -> Pickoff offset angle to zero and agree with incremental encoder
 *    C -> Base Angle Offset from index point to probe coordinate origin 
 *    D -> Pickoff Angle Offset from index point to probe coordinate origin
 *    E -> X Position Offset from index point to probe coordinate origin
 *    F -> Y Position Offset from index point to probe coordinate origin
 *    G -> Base stage sine potentiometer value
 *    H -> Base stage cosine potentiometer value
 *    I -> Pickoff stage sine potentiometer value
 *    J -> Pickoff stage cosine potentiometer value
 *
 * Gensub output fields:
 *    VALA -> calculated base stage angle from potentiometers
 *    VALB -> calculated pickoff stage angle from potentiometers
 *    VALC -> calculated X probe position from potentiometers
 *    VALD -> calculated Y probe position from potentiometers
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
 * Calculation for OUT_BAS_ANGLE could potentially divide by zero
 * if either basCosine or pkoCosine is zero.
 *-
 ************************************************************************
 */

/*
 * Init Function for CalcAbsAngles
 */
long oiInitCalcAbsAngles 
(
    genSubRecord *pgs               /* calling record structure             */
)
{
  int i;  /* loop counter */
  
  /* initialize the target array */
  for(i=0;i<num_elements;i++)
    {
      pots[i].sinBas = 0;
      pots[i].cosBas = 0;
      pots[i].sinPko = 0;
      pots[i].cosPko = 0;
    }
  
  start_ptr = 0;
  end_ptr = -1;
  
  return CALC_SUCCESS;

}

long oiCalcAbsAngles 
(
    genSubRecord *pgs               /* calling record structure             */
)
{
    double basSine;                 /* sine of base stage angle             */
    double basCosine;               /* cosine of base stage angle           */
    double pkoSine;                 /* sine of pickoff stage angle          */
    double pkoCosine;               /* cosine of pickoff stage angle        */    
    double basSineSign;             /* sign of base sine angle              */
    double basCosineSign;           /* sign of base cosine angle            */
    double pkoSineSign;             /* sign of pickoff sine angle           */
    double pkoCosineSign;           /* sign of pickoff cosine angle         */
    double filtBasSin;
    double filtBasCos;
    double filtPkoSin;
    double filtPkoCos;

    /*
     * First store incoming values in a queue
     */

    insert_pots(IN_BAS_SINE, IN_BAS_COSINE, IN_PKO_SINE, IN_PKO_COSINE);

    if (gmOiwfsFilter) {

      /*
       * Get filtered values
       */
      
      get_filtered_pots(&filtBasSin, &filtBasCos, &filtPkoSin, &filtPkoCos);
      
      /*
       *  Determine the sign of each of the angular components
       */
      
      basSineSign =   (filtBasSin > 0.0)   ? 1.0 : -1.0;
      basCosineSign = (filtBasCos > 0.0) ? 1.0 : -1.0;
      pkoSineSign =   (filtPkoSin > 0.0)   ? 1.0 : -1.0;
      pkoCosineSign = (filtPkoCos > 0.0) ? 1.0 : -1.0;
      
      
      /*
       *  Then determine the angular position components
       */
      
      basSine = filtBasSin * M_PI / 2.0;
      basCosine = filtBasCos * M_PI / 2.0;
      pkoSine = filtPkoSin * M_PI / 2.0;
      pkoCosine = filtPkoCos * M_PI / 2.0;
      
    } else {
      /*
       *  Determine the sign of each of the angular components
       */
      
      basSineSign =   (IN_BAS_SINE > 0.0)   ? 1.0 : -1.0;
      basCosineSign = (IN_BAS_COSINE > 0.0) ? 1.0 : -1.0;
      pkoSineSign =   (IN_PKO_SINE > 0.0)   ? 1.0 : -1.0;
      pkoCosineSign = (IN_PKO_COSINE > 0.0) ? 1.0 : -1.0;
      
      
      /*
       *  Then determine the angular position components
       */
      
      basSine = IN_BAS_SINE * M_PI / 2.0;
      basCosine = IN_BAS_COSINE * M_PI / 2.0;
      pkoSine = IN_PKO_SINE * M_PI / 2.0;
      pkoCosine = IN_PKO_COSINE * M_PI / 2.0;
    }

    /*
     *  Calculate the approximate stage angles adding in the zero offset.
     */

    if ( basSine == 0.0 && basCosine == 0.0 &&
         pkoSine == 0.0 && pkoCosine == 0.0 )
    {
        OUT_BAS_ANGLE = 0.0;
        OUT_PKO_ANGLE = 0.0;
    }
    else
    {
      OUT_BAS_ANGLE = IN_BAS_ABS_OFFSET +
	(180.0 / M_PI) * (atan (basSine / basCosine)) + 
	90.0 * (1.0 - basCosineSign) * basSineSign; 
      OUT_PKO_ANGLE = IN_PKO_ABS_OFFSET +
	(180.0 / M_PI) * (atan (pkoSine / pkoCosine)) + 
	90.0 * (1.0 - pkoCosineSign) * pkoSineSign; 
    }

    /*
     *  Calculate new angular position components based on
     *  approximate probe angles calculated above and factoring in 
     *  offsets to convert from encoder frame of reference to
     *  mask frame of reference (also used in oiAngles2Position).
     */

    basSine = sin( (OUT_BAS_ANGLE - IN_B_OFFSET) * M_PI / 180.0 );
    basCosine = cos( (OUT_BAS_ANGLE - IN_B_OFFSET) * M_PI / 180.0 );
    pkoSine = sin( (OUT_PKO_ANGLE - IN_P_OFFSET) * M_PI / 180.0 );
    pkoCosine = cos( (OUT_PKO_ANGLE - IN_P_OFFSET) * M_PI / 180.0 );


    /*
     *  Calculate approximate X/Y positions in the mask frame of
     *  reference using the new angular components based on the
     *  absolute encoder pots and adding the X/Y origin offsets 
     *  defined in gmOiwfsCalc.h (which also defines arm lengths).
     */

    OUT_X_POSITION = BASE_ORIGIN_X + 
            (BASE_ARM_LENGTH * basCosine) + ( PICKOFF_ARM_LENGTH * 
              ( (basCosine * pkoCosine) - (basSine * pkoSine) ) );

    OUT_Y_POSITION = BASE_ORIGIN_Y + 
            (BASE_ARM_LENGTH * basSine) + ( PICKOFF_ARM_LENGTH *
              ( (basSine * pkoCosine) + (basCosine * pkoSine) ) );

    return 0;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiFollowA
 *
 * INVOCATION:
 * status = oiFollowA (pgs);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pgs (struct genSubRecord *)  Pointer to calling genSub record structure
 *
 * FUNCTION VALUE:
 * (long) processing status.
 *
 * PURPOSE:
 * Convert x/y target position in telescope frame to x/y target position in
 * mask frame of reference
 *
 * DESCRIPTION: 
 * This gensub function is the main driving engine of the oiwfs system.   It 
 * is processed at a 20 Hz interval by the incoming TCS target stream.   The
 * record is processed with every new TCS target regardless of the following 
 * state.
 * 
 * The X-Y target from either the TCS target stream or the explicit move
 * command is transformed from the telescope frame of reference to the mask 
 * frame x/y and these positions are written to VALE and VALF respectively.
 *  
 * If the conversion was successful, CALC_SUCCESS is written to VALA, the
 * MODE value of MOVE is written to VALB and the DIR value of START is 
 * written to VALI to cause the owifs assemblyControl record to process.  
 * If the conversion was unsuccessful, CALC_FAILURE is written to VALA, 
 * which is used downstream to disable the assemblyControl record thereby
 * preventing motion to invalid positions.
 *
 * Since this record is processed at periodic intervals rather than when the
 * inputs have changed so it is necessary to compare the state of the
 * inputs against the state at last processing to detect when they have
 * been updated.
 * 
 * Gensub input fields:
 *    A -> X Position Target in the telescope frame of reference
 *    B -> Y Position Target in the telescope frame of reference
 *    C -> Current probe position structure (X,Y,Z,R) from CalcPosition gensub
 *    D -> Probe travel X upper limit in telescope frame of reference
 *    E -> Probe travel X lower limit in telescope frame of reference
 *    F -> Probe travel Y upper limit in telescope frame of reference
 *    G -> Probe travel Y lower limit in telescope frame of reference
 *    H -> Current following mode
 *    I -> Current tolerance (tracking deadband)
 *    J -> Stream of probe target structures generated by the TCS system
 *    K -> Electronic offsets flag
 *    U -> Probe Map Correction Coefficients
 *
 * Gensub output fields:
 *    VALA -> Probe assemblyControl record inhibit flag
 *    VALB -> followS flag (0 == not following; 1 == following)
 *    VALC -> New trackId detected flag (1 = new trackId; 0 = same trackId)
 *    VALD -> Probe offset update inhibit flag
 *    VALE -> X target in Mask frame of reference
 *    VALF -> Y target in Mask frame of reference
 *    VALG -> Boolean "in-position" flag for the SAD
 *    VALH -> Boolean "target stream valid" flag for the SAD
 *    VALJ -> Probe offset structure out to A&G system
 *
 * EXTERNAL VARIABLES:
 *    prev_follow  - previous follow mode
 *    prev_x       - previous x target
 *    prev_y       - previous y target
 *    stream_count - number of targets since last start cmd
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * - other function name.
 *
 * DEFICIENCIES:
 * By not reading the input stream once and making a local copy,
 * code cannot guarantee that the structure members belong to the same 
 * array element.
 * Does not convert telescope to mask frame of reference!!!
 * Subsequent moves to the same position are not acted upon unless
 * the follow mode has changed (activeC will not go BUSY/IDLE).
 *-
 ************************************************************************
 */


long oiFollowA
(
    struct genSubRecord *pgs              /* calling record structure       */
)
{
    long status       = CALC_SUCCESS;     /* function return status         */
    long offsetCtrl   = SCAN_DISABLE;     /* disable position updates       */
    long assemblyCtrl = SCAN_DISABLE;     /* disable assemblyControl link   */
    long inPosition   = FALSE;            /* in position flag               */
    long tcsArray     = ARRAY_INVALID;    /* array valid flag               */
    long followMode;                      /* follow mode                    */
    long eoFlag;                          /* electronic offsets flag        */
    double  xTarget;                      /* target X                       */ 
    double  yTarget;                      /* target Y                       */
    double  zTarget;                      /* target Z                       */

    double tcsDemands[6];                 /* demand stream from TCS         */
    double tAppl;                         /* time demand applies            */
    double trackId;                       /* trackID of demand              */

    double probePosition[4];              /* current probe positions        */
    double xCurrent;                      /* current x position             */
    double yCurrent;                      /* current Y position             */
    double zCurrent;                      /* current Z position             */
    double rCurrent;                      /* current R position             */

    /*
     *  Make a local copy of the current position structure
     */

    memcpy(probePosition, IN_CURPOS, pgs->noc * sizeof(double));

    xCurrent = probePosition[0];
    yCurrent = probePosition[1];
    zCurrent = probePosition[2];
    rCurrent = probePosition[3];
        
    /*
     *  Further processing depends on the current following mode,
     *  which can be:
     *               0  FOLLOW_STOP    stop following or moving
     *               1  FOLLOW_MOVE    move to given probe target
     *               2  FOLLOW_FOLLOW  follow TCS stream of targets
     *               3  FOLLOW_MOVE_R  check to see if move is in position
     */
    eoFlag = IN_EO; 
    followMode = IN_FOLLOW;

    switch (followMode)
    {

        /*
         *  In MOVE mode we are being asked to re-position the probe to
         *  a given X-Y target in the telescope frame of reference.   This
         *  target is given directly - the TCS target stream is ignored.  
         */

        case FOLLOW_MOVE:

	    /*
	     * Apply OIWFS Probe Mapping transformation
	     */
	    xTarget = oiCalcProbeX(pgs, IN_X_TARGET, IN_Y_TARGET);
	    yTarget = oiCalcProbeY(pgs, IN_X_TARGET, IN_Y_TARGET);
	  
            if (prev_follow != FOLLOW_MOVE)
	    {
                DEBUG("<%ld> %s: oiFollowA: Move mode%c\n",' ');
            }

            /*
             *  Change to MOVE_R follow mode for subsequent passes
             */

            followMode = FOLLOW_MOVE_R;


           /* 
            *  Set stream count to "resume" on next follow.
            */

            stream_count = FOLLOW_RESUME;


            /*
             *  Check the target position against the motion limits.  If
             *  the request is outside the safe operating region then
             *  the assemblyControl link does not get enabled.
             */

            if (xTarget > IN_X_HI_LIM || 
                xTarget < IN_X_LO_LIM ||
                yTarget > IN_Y_HI_LIM || 
                yTarget < IN_Y_LO_LIM)
            {
                status = CALC_FAILURE;
            }

            /*
             *  Motion is withing limits ... convert from telescope to
             *  mask frame of reference since that is the physical space 
             *  that the probe knows about ... (someday) NOT DONE!!!!!
             */

            else
            {	      
                status = CALC_SUCCESS;
            }

            /*
             *  If the transformation was successful (probe can physically
             *  reach the target position) send the transformed target
             *  position to the oiwfs assemblyControl record as its target
             *  position.   Enable the assemblyControl record output link.
             */
                   
            if (status == CALC_SUCCESS)
            {
                OUT_X_MASK = xTarget;
                OUT_Y_MASK = yTarget;
                assemblyCtrl = SCAN_ENABLE;
            }

            break;


        /*
         *  In MOVE_R mode the probe position is read to determine if it is
         *  in position for a MOVE command.
         */

        case FOLLOW_MOVE_R:

	    /*
	     * Apply OIWFS Probe Mapping transformation
	     */
	    xTarget = oiCalcProbeX(pgs, IN_X_TARGET, IN_Y_TARGET);
	    yTarget = oiCalcProbeY(pgs, IN_X_TARGET, IN_Y_TARGET);

            /*
             *  If the motion vector is less than the tolerance value then
             *  set the in-position flag.
             */

            if (sqrt((xTarget - xCurrent) * (xTarget - xCurrent) + 
                          (yTarget - yCurrent) * (yTarget - yCurrent)) < 
                     IN_TOLERANCE                         )
            {
                inPosition = TRUE;
                if (OUT_IN_POSITION == FALSE)  
                    DEBUG("<%ld> %s: oiFollowA: Moved to within tolerance of target%c\n",' ');
            }

            break;

        /*
         *  In FOLLOW mode the probe position is constantly updated to
         *  follow the target stream generated by the TCS system.  Note that
         *  the probe is only physically moved if the position offset is
         *  outside of the current deadband.
         */

        case FOLLOW_FOLLOW:

            if (prev_follow != FOLLOW_FOLLOW)
            {
                DEBUG("<%ld> %s: oiFollowA: Follow mode%c\n",' ');
            }

            /*
             *  Make a local copy of the demand stream now so that it is only 
             *  read once and all members belong to the same array.
             */

            memcpy(tcsDemands, IN_STREAM, pgs->noj * sizeof(double));

            tAppl   = tcsDemands[0];
            trackId = tcsDemands[2];
            zTarget = tcsDemands[5];

	    /*
	     * Apply OIWFS Probe Mapping transformation
	     */
	    xTarget = oiCalcProbeX(pgs, tcsDemands[3], tcsDemands[4]);
	    yTarget = oiCalcProbeY(pgs, tcsDemands[3], tcsDemands[4]);

              /*
              *  Copy the follow targets to the move targets inputs to
        .     *  show updates on the engineering screens.
              */
 
             IN_X_TARGET = xTarget;
             IN_Y_TARGET = yTarget;
             db_post_events(pgs, &IN_X_TARGET, DBE_VALUE|DBE_LOG);
             db_post_events(pgs, &IN_Y_TARGET, DBE_VALUE|DBE_LOG);


            /*
             *  If electronic offsets are disabled,
             *  set new track ID flag if new ID differs from old ID 
             *  electronic offsets are enabled when eoFlag = 1, disabled when eoFlag = 0
             */

            if ( eoFlag != 1 )
            { 
                /*printf("oiFollowA : EO DEBUG eoFlag = %ld \n",eoFlag);*/
                if ( OUT_STREAM[1] != trackId )  
            	{
                	newTrack = TRUE;
            	}
            }

            /* 
             *  Check the newTrack flag here and set output if 
             *  not already done.
             */ 

            if (newTrack == TRUE && OUT_NEW_TRACK == FALSE)
            {
                if (gmOiwfsFollowDebug)
                    printf("oiFollowA: New TrackID:%f\n", trackId);
                OUT_NEW_TRACK = TRUE;
            }

            /*
             *  Calculate the probe offsets by subtracting the current probe
             *  position calculated from the incremental encoders from the 
             *  desired probe position requested by the TCS.  The offsets are 
             *  then sent to the A&G system as a single structure.
             */

            OUT_STREAM[0] = tAppl;
            OUT_STREAM[1] = trackId;
            OUT_STREAM[2] = rCurrent;
            OUT_STREAM[3] = xCurrent - xTarget;
            OUT_STREAM[4] = 0;
            OUT_STREAM[5] = yCurrent - yTarget;
            OUT_STREAM[6] = 0;
            OUT_STREAM[7] = zCurrent;
            OUT_STREAM[8] = 0;

            /* 
             *  Probe offset information is always updated in follow
             *  mode.
             */ 

            offsetCtrl = SCAN_ENABLE;


            /*
             *  Test the target position against the motion limits.  If
             *  requested position is outside of the safe operating area 
             *  then set status to FAIL & stream count to resume on next follow
             */

            if (xTarget > IN_X_HI_LIM ||  xTarget < IN_X_LO_LIM ||
                yTarget > IN_Y_HI_LIM ||  yTarget < IN_Y_LO_LIM ||
                zTarget != 0 )
            {
                status = CALC_FAILURE;
		stream_count = FOLLOW_RESUME;
            }

            /*
             *  Target is within limits, so check to see if current position
             *  is within tolerance value.   If it is outside the deadband 
             *  or if the trackID has changed, then move the probe to the 
             *  current target position.
             */

            else if ( ( sqrt((xTarget - xCurrent) * (xTarget - xCurrent) +
                             (yTarget - yCurrent) * (yTarget - yCurrent)  ) > 
                        IN_TOLERANCE) ||
                       newTrack == TRUE                                        )
            {

                /*
                 * Indicate the target stream is valid but, since the probe needs to
                 * be slewed to the target it is likely the probe will be late.
                 */

                tcsArray = ARRAY_LATE;

                /*
                 *  In order to reduce the number of probe motions the target
                 *  is only updated under these conditions:
                 *       o  once every (MAGIC NUMBER) of target stream updates
                 *       o  on first motion request after follow mode has started
                 *       o  when trackID has changed
                 */

                if ( stream_count >= FOLLOW_RECENTER || 
                     stream_count == FOLLOW_RESUME   ||
                     newTrack     == TRUE               )
                {
                    if (newTrack == TRUE)
                    {
                        newTrack = FALSE;
                        DEBUG("<%ld> %s: oiFollowA: Slewing to new trackID target%c\n", ' ');
                    }

                    else if (stream_count == FOLLOW_RESUME)
                    {
                        DEBUG("<%ld> %s: oiFollowA: Slewing to previous trackID target%c\n", ' ');
                    }                    

                    else
                    {
                        DEBUG("<%ld> %s: oiFollowA: Recentering on new target%c\n", ' ');
                    }

                    
                    /*
                     *  If the transformation was successful (probe can physically
                     *  reach the target position), send the transformed target
                     *  position to the oiwfs assemblyControl record as its target
                     *  position.  Enable the assemblyControl record output link
                     *  and reset stream count.
                     */
		    
                    OUT_X_MASK = xTarget;
                    OUT_Y_MASK = yTarget;

                    assemblyCtrl = SCAN_ENABLE;

                    stream_count = 0;
                }

                else 
                {
                    /*
                     *  Target is valid and still out of position but
                     *  no need to drive the assemblyControl record yet, 
                     *  so keep it disabled and increment stream count.
                     */

                    stream_count ++; 
                }
            }


            /*
             *  Otherwise we are in the tolerance region.  Disable the
             *  assemblyControl link. Indicate that we are in position, 
             *  that the target stream is valid and clear the new track output.
             */

            else 
            {
                inPosition = TRUE;
                tcsArray = ARRAY_GOOD;
                if (OUT_NEW_TRACK == TRUE)
                {
                    DEBUG("<%ld> %s: oiFollowA: Following within tolerance%c\n",' ');
                    OUT_NEW_TRACK = FALSE;
                }
            }

            break;

        /*
         *  In STOP mode we do not follow the TCS target stream or update the 
         *  probe offset information.   This is an idle state that waits for
         *  a request to move or to resume TCS position tracking.  Treat any
         *  invalid follow modes as a STOP.
         */

        case FOLLOW_STOP:

            if (prev_follow != FOLLOW_STOP)
            {
                DEBUG("<%ld> %s: oiFollowA: Stop following/moving%c\n",' ');
            }

        default:

            if (followMode != FOLLOW_STOP && followMode != prev_follow)
            {
                DEBUG("<%ld> %s: oiFollowA: ERROR ... invalid follow mode, stop following/moving%c\n",' ');
            }

           /* 
            *  Set stream count to "resume" on next follow.
            */

            stream_count = FOLLOW_RESUME;

            break;
    }       


    /*
     *  Update output flags if they've changed
     */

    if (OUT_IN_POSITION != inPosition)
        OUT_IN_POSITION = inPosition;
    if (OUT_ARRAY_S != tcsArray)
        OUT_ARRAY_S = tcsArray;
    if (OUT_FOLLOW_S != (followMode == 2))
        OUT_FOLLOW_S = (followMode == 2);

    /*
     *  Update scanning control outputs if they've changed
     */

    if (OUT_REJ_OFFSET != offsetCtrl) 
        OUT_REJ_OFFSET = offsetCtrl;
    if (OUT_REJECT != assemblyCtrl)  
        OUT_REJECT = assemblyCtrl;

    /*
     *  Update the input comparison fields so that we know if they have
     *  changed when the next periodic processing happens.
     */

    prev_follow = followMode;
    prev_x = IN_X_TARGET;
    prev_y = IN_Y_TARGET;

    /*
     *  Change the input follow mode if followMode was changed
     *  after a move command.
     */

    if (IN_FOLLOW != followMode)
        IN_FOLLOW  = followMode;

    return (status); 
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiProbeMap
 *
 * INVOCATION:
 * status = oiProbeMap (pgs); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pgs  (genSub *) Pointer to genSub record structure.
 *
 *
 * FUNCTION VALUE:
 * (long) function return status.
 *
 * PURPOSE:
 * Combine 12 transformation coefficients into an array for input into followA
 *
 * DESCRIPTION:
 * Simply build the array from the inputs.  This is triggered by a PP INP link.
 *
 * Gensub input fields:
 *    A -> A0 coefficient
 *    B -> A1 coefficient
 *    C -> A2 coefficient
 *    D -> A3 coefficient
 *    E -> A4 coefficient
 *    F -> A5 coefficient
 *    G -> B0 coefficient
 *    H -> B1 coefficient
 *    I -> B2 coefficient
 *    J -> B3 coefficient
 *    K -> B4 coefficient
 *    L -> B5 coefficient
 *
 * Gensub output fields:
 *    VALA -> array of 12 coefficients
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

long oiProbeMap
(
    genSubRecord *pgs               /* gensub record structure              */
)
{
  OUT_COEFF[0] = IN_A0;
  OUT_COEFF[1] = IN_A1;
  OUT_COEFF[2] = IN_A2;
  OUT_COEFF[3] = IN_A3;
  OUT_COEFF[4] = IN_A4;
  OUT_COEFF[5] = IN_A5;
  OUT_COEFF[6] = IN_B0;
  OUT_COEFF[7] = IN_B1;
  OUT_COEFF[8] = IN_B2;
  OUT_COEFF[9] = IN_B3;
  OUT_COEFF[10] = IN_B4;
  OUT_COEFF[11] = IN_B5;

  return 0;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiProbeOffset
 *
 * INVOCATION:
 * status = oiProbeOffset (pgs); 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pgs  (genSub *) Pointer to genSub record structure.
 *
 *
 * FUNCTION VALUE:
 * (long) function return status.
 *
 * PURPOSE:
 * Make Offset info available for engineering
 *
 * DESCRIPTION:
 * Control writing of the probe offset information into the 
 * A&G offset record.   If this record is enabled then the
 * offset structure will be written to the A&G every time the
 * followA record is processed.   Disabling this record via the
 * scan disable field will prevent the offsets from being processed
 * and written to the A&G.
 * Decompose the probe offset structure into its component parts
 * so that they can be displayed on engineering screens.
 *
 * Gensub input fields:
 *    A -> Probe offset structure in from followA gensub.
 *
 * Gensub output fields:
 *    VALA -> X offset in mm in telescope frame of reference
 *    VALB -> Y offset in mm in telescope frame of reference
 *    VALC -> Z offset in mm in telescope frame of reference
 *    VALD -> probe angle offset in mm
 *    VALE -> current tracking stream ID
 *    VALJ -> Probe offset structure out
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

long oiProbeOffset
(
    genSubRecord *pgs               /* gensub record structure              */
)
{

    /*
     *  Update the output field values with the current values from
     *  the offset structure.
     */

    OUT_X_OFF = IN_OFFSET[3];
    OUT_Y_OFF = IN_OFFSET[5];
    OUT_Z_OFF = IN_OFFSET[7];
    OUT_R_OFF = IN_OFFSET[2];
    OUT_TRACK_ID = IN_OFFSET[1];

    /*
     *  And copy the whole input structure to the output structure to be
     *  sent to the A&G system.
     */

    OUT_OFFSET[0] = IN_OFFSET[0];
    OUT_OFFSET[1] = IN_OFFSET[1];
    OUT_OFFSET[2] = IN_OFFSET[2];
    OUT_OFFSET[3] = IN_OFFSET[3];
    OUT_OFFSET[4] = IN_OFFSET[4];
    OUT_OFFSET[5] = IN_OFFSET[5];
    OUT_OFFSET[6] = IN_OFFSET[6];
    OUT_OFFSET[7] = IN_OFFSET[7];
    OUT_OFFSET[8] = IN_OFFSET[8];

    return 0;
}


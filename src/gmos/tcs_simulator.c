/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) 1999                         (c) 1999
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
 * tcs_simulator.c
 *
 * PURPOSE:
 * Generates a simulated TCS target stream for the oiwfs probe to follow.
 * The probe targets descirbe a circle whose origin, radius and rate of
 * change are defined by genSub record input fields.
 *
 * 
 * FUNCTION NAME(S)  [ Global scope ]
 * oiTargetSimulator -  Gensub record support function to generate stream
 *
 *
 * FUNCTION NAME(S)  [ Local scope ]
 * getNextTarget - Return the next point on the circle
 * setTarget     - Set the circle parameters
 * 
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.3  2000/10/16 23:30:32  gmos
 * Add trackID in/out.
 *
 * Revision 1.2  2000/05/30 16:29:12  gmos
 * New version commented by William Rambold. Master enable bug fixed.
 *
 * Revision 1.2  2000/04/15 11:38:55  gmos
 * Angelics updated OIWFS software
 *
 * Revision 1.4  2000/04/13 08:25:50  gmos
 * Corrections released by Angelic on 13 April 2000
 *
 * Revision 1.3  2000/04/11 22:34:57  msmith
 * Added probeAngle parameter to  gmOiwfsCalculateProbePosition.
 *
 * Revision 1.2  2000/03/21 23:07:22  msmith
 * Modified gmOiwfsCalculateVerticalRange so that it returns an error
 * code if the computed midpoint is invalid.  When this occurs, the
 * midpoint is set to a "safe" value (EMERGENCY_X, EMERGENCY_Y).
 *
 * Added code to protect divides from possible divide by zero.
 *
 * Revision 1.1  2000/01/28 23:31:04  dunn
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

#include <stdioLib.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vxWorks.h>

#include <dbDefs.h>
#include <recSup.h>
#include <genSubRecord.h>

#ifndef M_PI
#define M_PI  3.1415926535897932384626
#endif


/*
 *  --- TCSsimulator genSub record ---
 *  Input field access mnemonics
 */

#define IN_X_ORIGIN  *(double *)pgs->a    /* enter X Origin                 */
#define IN_Y_ORIGIN  *(double *)pgs->b    /* enter Y Origin                 */
#define IN_PERIOD    *(double *)pgs->c    /* time to complete one revolution*/
#define IN_RADIUS    *(double *)pgs->d    /* radius of circle               */
#define IN_TRACK_ID  *(double *)pgs->e    /* enter trackID                  */


/*
 *  --- TCSsimulator genSub record ---
 *  Output field access mnemonics
 */

#define OUT_X_TARGET  *(double *)pgs->vala    /* X Target                   */
#define OUT_Y_TARGET  *(double *)pgs->valb    /* Y Target                   */
#define OUT_TRACK_ID  *(double *)pgs->valc    /* trackID                    */
#define OUT_STREAM    ((double *)pgs->valj)   /* Target in a TCS struct     */


/*
 *  Other definitions
 */
 
#define CALC_SUCCESS        0           /* Calculation was successful       */
#define CALC_FAILURE        -1          /* Calculation generated an error   */

#define  UPDATE_INTERVAL    0.05        /* 20 Hz scan period in seconds     */


/*
 *  Public function prototypes
 */

long oiTargetSimulator (struct genSubRecord *);


/*
 *  Local function prototypes
 */

static int getNextTarget (double *, double *);
static int setTarget (double, double, double, double);


/*
 *  Data structures
 */

static double x_orig;               /* X origin of the circle           */ 
static double y_orig;               /* Y origin of the circle           */ 
static double radius = -1.0;        /* Radius of the circle             */ 
static double period = -1.0;        /* Time to do one revolution        */
static double x_current;            /* Current X coordinate             */
static double y_current;            /* Current Y coordinate             */



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * setTarget
 *
 * INVOCATION:
 * status = oiBusyStateChange (x_position, y_position);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (<) x_position  (double *)     new x coordinate for the oiwfs probe.
 * (<) y_position  (double *)     new y coordinate for the oiwfs probe.
 *
 *
 * FUNCTION VALUE:
 * (int) function return status.
 *
 * PURPOSE:
 * Calculate the next virtual probe positon
 *
 * DESCRIPTION:
 * This function advances the virtual probe one step along the
 * perimiter of the given circle and returns the X and Y positions
 * of the probe.
 *
 * EXTERNAL VARIABLES:
 * x_org        - local X origin storage
 * y_org        - local Y origin storage
 * radius       - local radius storage
 * period       - local period storage
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

static int getNextTarget
(
    double * xPos,                  /* new probe target x position          */
    double * yPos                   /* new probe target y position          */
)
{
    double angle;                   /* angle working space                  */


    /*
     *  Reject invalid circle parameters
     */

    if ((radius < 0) || (period == 0))
    {
        return 0;
    }



    /*
     *  Generate a new probe position along the circle
     */

    else
    {
        /*
         *  Compute the angle of the previous (X,Y) position.
         */

        angle = atan2(y_current - y_orig, x_current - x_orig);


        /*
         *  Update the angle to the next angular position, then ensure
         *  that it is within the range -PI...PI.
         */

        angle = angle + 2.0 * M_PI * UPDATE_INTERVAL / period;
        if (angle > M_PI)
        {
            angle = angle - 2 * M_PI;
        }
        

        /*
         *  Compute a new position based on the new angle, then return it
         *  to the caller via *xPos and *yPos.
         */

        x_current = x_orig + radius * cos(angle);
        y_current = y_orig + radius * sin(angle);

        *xPos = x_current;
        *yPos = y_current;

        return 1;
    }
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * setTarget
 *
 * INVOCATION:
 * status = oiBusyStateChange (x_orig, y_orig, radius, period);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) x_orig  (double)     new x coordinate of the center of the circle.
 * (>) y_orig  (double)     new y coordinate of the center of the circle.
 * (>) radius  (double)     new radius of the circle.
 * (>) peroid  (double)     new number of seconds taken to trace the circle.
 *
 *
 * FUNCTION VALUE:
 * (int) function return status.
 *
 * PURPOSE:
 * Set the target generation algorithm parameters
 *
 * DESCRIPTION:
 * This function sets the ineternal variables used by the target
 * generation algorithm that simulates a TCS target stream.  Input
 * values are in milimeters in the telescope frame of reference.
 *
 * In order to insure that the first vector starts at an angle of
 * zero this function performs a single backwards step so that the
 * next output (after a forward step is done) will be returned to
 * zero
 *
 * EXTERNAL VARIABLES:
 * x_org        - local X origin storage
 * y_org        - local Y origin storage
 * radius       - local radius storage
 * period       - local period storage
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

static int setTarget
(
    double x_orig_in_mm,        /* desired x origin for center of circle    */ 
    double y_orig_in_mm,        /* desired y origin for center of circle    */ 
    double radius_in_mm,        /* desired radius of circle                 */ 
    double period_in_sec        /* desired length of time to trace circle   */
)
{

    /*
     *  Update current origin and radius values with the given ones 
     */

    x_orig = x_orig_in_mm;    
    y_orig = y_orig_in_mm;   
    radius = radius_in_mm;


    /*
     *  Set the current position to the zero angle position, then back up
     *  one simulation step so that the first call to getNextTarget returns
     *  the zero angle position.  The backing up is done by changing the 
     *  direction (sign) of the period and taking one simulation step
     *  backwards.
     */

    x_current = x_orig + radius;
    y_current = y_orig;

    period = -period_in_sec;

    getNextTarget(&x_current, &y_current);


    /*
     *  Restore the period to the correct direction (sign) and we are done.
     */

    period = period_in_sec;

    return 1;
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * oiTargetSimulator
 *
 * INVOCATION:
 * status = oiBusyStateChange (x_orig, y_orig, radius, period);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) x_orig  (double)     new x coordinate of the center of the circle.
 * (>) y_orig  (double)     new y coordinate of the center of the circle.
 * (>) radius  (double)     new radius of the circle.
 * (>) peroid  (double)     new number of seconds taken to trace the circle.
 *
 *
 * FUNCTION VALUE:
 * (long) function return status.
 *
 * PURPOSE:
 * Simulate the TCS generated following stream
 *
 * DESCRIPTION:
 * This function is called every time the TCSsimulator gensub
 * record processes.  This is normally set to a 20Hz rate to simulate
 * the action of the Gemini TCS system.
 *
 * If any of the input arguments have changed then the internal 
 * position generation algorithm parameters are updated to the
 * new values.
 *
 * The oiwfs probe is advanced along the perimiter of an imaginary
 * circle every time the function is called.   The X and Y target 
 * positions are formatted into a TCS style following structure to
 * be written to the oiwfs system.
 *
 * Gensub input fields:
 *      A -> IN_X_ORIGIN        - X Origin
 *      B -> IN_Y_ORIGIN        - Y Origin
 *      C -> IN_PERIOD          - time to complete one revolution
 *      D -> IN_RADIUS          - radius of circle
 *
 * Gensub output fields:
 *      VALA -> OUT_X_TARGET    - X Target position
 *      VALB -> OUT_Y_TARGET    - Y Target position
 *      VALJ -> OUT_STREAM      - Target as an oiwfs TCS target struct
 *
 * EXTERNAL VARIABLES:
 * x_org        - local X origin storage
 * y_org        - local Y origin storage
 * radius       - local radius storage
 * period       - local period storage
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

long oiTargetSimulator
(
    struct genSubRecord *pgs   /* (in) pointer to calling record structure */
)
{
    double x_target;                    /* new x coordinate for the probe   */
    double y_target;                    /* new y coordinate for the probe   */
    long status = CALC_SUCCESS;         /* calculation success code         */


    /* 
     *  Update the local parameters if any of the input parameters have
     *  changed....
     */

    if ((x_orig != IN_X_ORIGIN) ||
        (y_orig != IN_Y_ORIGIN) ||
        (radius != IN_RADIUS) || 
        (period != IN_PERIOD))
    {
        setTarget(IN_X_ORIGIN, IN_Y_ORIGIN, IN_RADIUS, IN_PERIOD);
    }


    /*
     *  Then calculate the next target position and update the output
     *  fields and the target structure.
     */

    getNextTarget (&x_target, &y_target);

    OUT_X_TARGET = x_target;
    OUT_Y_TARGET = y_target;
    OUT_TRACK_ID = IN_TRACK_ID;

    OUT_STREAM[2] = IN_TRACK_ID;
    OUT_STREAM[3] = x_target;
    OUT_STREAM[4] = y_target;

    return (status);
}

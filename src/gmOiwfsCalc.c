static char rcsid[] = "$Id$";
/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) 1999  				        (c) 1999
 * National Research Council		Conseil national de recherches
 * Ottawa, Canada, K1A 0R6 		    Ottawa, Canada, K1A 0R6
 * All rights reserved			    Tous droits reserves
 * 					
 * NRC disclaims any warranties,	Le CNRC denie toute garantie
 * expressed, implied, or statu-	enoncee, implicite ou legale,
 * tory, of any kind with respect	de quelque nature que se soit,
 * to the software, including		concernant le logiciel, y com-
 * without limitation any war-		pris sans restriction toute
 * ranty of merchantability or		garantie de valeur marchande
 * fitness for a particular pur-	ou de pertinence pour un usage
 * pose.  NRC shall not be liable	particulier.  Le CNRC ne
 * in any event for any damages,	pourra en aucun cas etre tenu
 * whether direct or indirect,		responsable de tout dommage,
 * special or general, consequen-	direct ou indirect, particul-
 * tial or incidental, arising		ier ou general, accessoire ou
 * from the use of the software.	fortuit, resultant de l'utili-
 * 					                sation du logiciel.
 *
 ************************************************************************
 *
 * FILENAME
 * gmOiwfsCalc.c
 *
 * PURPOSE:
 * Performs transformations between the X,Y coordinate frame of the
 * GMOS mask and the stage angles of the OIWFS probe.
 *
 * FUNCTION NAME(S)  [ Global scope ]
 *
 * gmOiwfsSetOffsets             - Initializes the calibration coefficients.
 *
 * gmOiwfsGetOffsets             - Get copies of the calibration coefficients.
 *
 * gmOiwfsCalculateProbeAngles   - Calculates the stage angles required to
 *                                 position the probe at the desired X,Y
 *                                 location in the mask coordinate frame.
 *
 * gmOiwfsCalculateProbePosition - Calculates the X,Y position of the probe
 *                                 in the mask coordinate frame based on
 *                                 the stage angles.
 *
 * gmOiwfsCalculateVerticalRange - Calculates the minimum and maximum Y values
 *                                 which the probe will have during a move from
 *                                 one location to another.  If the move is
 *                                 predicted to encounter a limit, then an
 *                                 intermediate location is calculated and a
 *                                 value of 1 is returned.  If the move is
 *                                 predicted to not encounter a limit, a value
 *                                 of 0 is returned.
 *
 * FUNCTION NAME(S)  [ Local scope ]
 *
 * CalcApproxProbeAngles         - Calculates the approximate stage angles
 *                                 given the probe's desired X,Y position 
 *                                 on the mask. The approximation ignores
 *                                 the effects of stage tilt.
 *
 * RefineProbeAngles             - Attempts to improve the accuracy of the
 *                                 stage angles required to position the
 *                                 probe at a specified X,Y position.
 *
 * CalcApproxProbePosition       - Calculates the approximate position
 *                                 of the probe from the stage angles.
 *                                 This approximation ignores the effects
 *                                 of stage tilt.
 *
 * CalcExactProbePosition        - Calculates the position from the stage
 *                                 angles (taking into account the stage
 *                                 tilt).
 *
 * MatVecMult4                   - Multiplies vector V by Matrix M and
 *                                 overwrites the original vector V with
 *                                 the result, M * V.
 *
 * InitPickoffToProbeTrans       - Constructs the transformation matrix
 *                                 which transforms coordinates in the
 *                                 pickoff arm frame of reference into 
 *                                 coordinates in the probe reference frame.
 *
 * InitBaseToPickoffTrans        - Constructs the transformation matrix
 *                                 which transforms coordinates in the
 *                                 base arm frame of reference into coordinates
 *                                 in the pickoff arm reference frame.
 *
 * InitTelescopeToBaseTrans      - Constructs the transformation matrix
 *                                 which transforms coordinates in the
 *                                 telescope frame of reference into 
 *                                 coordinates in the base arm reference frame.
 *
 * PickBestOrientation           - Returns the index of which solution
 *                                 results in the most horizontal probe 
 *                                 orientation.
 *
 * sign                          - Returns the sign of a number (0 if the
 *                                 number is 0.0)
 *
 * PrintMatrix                   - Prints a 4x4 matrix of doubles.  Used
 *                                 for debugging purposes only.
 *
 *
 *  Three coordinate systems are used in the calculations....
 *
 *  1)  The base coordinate system.  This system has its origin at the 
 *      center of the base stage rotation axis and is used when manipulating
 *      the base and pickoff stages directly.
 * 
 *  2)  The probe coordinate system. This system describes the probe tip
 *      physical position in relation to the mask surface.   It has its 
 *      origin at the optical axis in front of the mask.
 *
 *  3)  The telescope coordinate system.   This system describes the probe
 *      tip position in relation to the ISS interface port and is the frame
 *      of reference used by the Telescope Control and the Acquisiton
 *      and Guiding systems.
 *
 *  At the time of writing this code the transformation between the 
 *  telescope cordinate system and the mask coordinate system has not been
 *  defined.   A simple axis transformation is used to align the two
 *  coordinate systems.
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.5  2000/10/16 23:59:15  gmos
 * Moved some definitions into gmOiwfsCalc.h because gmOiwfsGenSubs.c
 * also uses them.
 *
 * Revision 1.4  2000/07/03 10:50:58  gmos
 * William removed chooseMidAngles function
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

#include <stdio.h>
#include <math.h>
#include <logLib.h>
#include "gmOiwfsCalc.h"




#define  SQR(X)  ((X) * (X))            /* Hard definiton for square funct  */

#define  EMERGENCY_X  60.0              /* Fixed midpoint X coordinate      */
#define  EMERGENCY_Y  60.0              /* Filed midpoint Y coordinate      */

#define  T_XZ  (-sqrt(T_X * T_X + T_Z * T_Z))  /* vector length X->Z        */


/*
 *  Coordinate system transformations
 *  TB -> telescope to base
 *  BP -> base to pickoff
 *  PM -> pickoff to mask (probe)
 */

#define  D_P       16400.0
#define  GAMMA_TB  atan(T_XZ / D_P)
#define  GAMMA_BP  (atan((T_XZ + BASE_ARM_LENGTH) / D_P) - GAMMA_TB)
#define  GAMMA_PM  (atan((T_XZ + BASE_ARM_LENGTH + PICKOFF_ARM_LENGTH) / D_P) \
                     - GAMMA_TB - GAMMA_BP)
#define  BETA_TB    (-atan(T_Z / T_X))


/*
 *  Distance to move the virtual probe between each iteration of the 
 *  vertical range calculation algorithm.
 */

#define  CALC_VERT_RANGE_STEP_SIZE   (0.5 * M_PI / 180.0)


/*
 *  These public global variables affect the caluclations used to determine
 *  probe target angles.   
 *      - gmOiwfsRefineSteps specifies how many iterations of the 
 *        refinement algorithm are used to clean up the rough position
 *        generated by the inverse kinematics calculation.
 *      - gmOiwfsStageTiltCorrection enables or disables the inclusion 
 *        of stage tilt in the calculations.
 *      - gmOiwfsSolutionId specifies which of the two possible solutions
 *        generated by the position to angle algorithm to use.
 */

int gmOiwfsRefineSteps = 2;           /* Set to 2, but 1 is okay too   */
int gmOiwfsStageTiltCorrection = 1;   /* 0 to ignore stage tilt        */
int gmOiwfsSolutionId = 3;            /* default to "best orientation" */


/*
 *  Local storage for the calibration offsets
 */

static GM_OIWFS_RAD baseOffset;
static GM_OIWFS_RAD pickoffOffset;
static GM_OIWFS_MM  xOffset;
static GM_OIWFS_MM  yOffset;


/*
 *  Transformation algorithm array definitions
 */

typedef double VECTOR[4];       /* generic four element vector array        */
typedef double MATRIX[4][4];    /* generic four by four calculation matrix  */


/*
 *  Function Prototypes (local).  Global prototypes are in gmOiwfsCalc.h.
 */


static int CalcApproxProbeAngles(GM_OIWFS_MM xTarget, GM_OIWFS_MM yTarget, 
                                 int solution,
                                 GM_OIWFS_RAD * baseAngle, 
                                 GM_OIWFS_RAD * pickoffAngle,
                                 GM_OIWFS_RAD * probeAngle);
  
static int RefineProbeAngles(GM_OIWFS_MM xTarget, GM_OIWFS_MM yTarget,
                             GM_OIWFS_RAD * baseAngle, 
                             GM_OIWFS_RAD * pickoffAngle,
                             GM_OIWFS_RAD * probeAngle);

static int CalcApproxProbePosition(GM_OIWFS_RAD baseAngle, 
                                   GM_OIWFS_RAD pickoffAngle,
                                   GM_OIWFS_MM * xPos,  GM_OIWFS_MM * yPos,
                                   GM_OIWFS_MM * focus);

static int CalcExactProbePosition(GM_OIWFS_RAD baseAngle, 
                                  GM_OIWFS_RAD pickoffAngle,
                                  GM_OIWFS_MM * xPos,  GM_OIWFS_MM * yPos,  
                                  GM_OIWFS_MM * focus);

static void MatVecMult4(MATRIX T, VECTOR X);

static void InitPickoffToProbeTrans(GM_OIWFS_RAD pickoffAngle, MATRIX T);

static void InitBaseToPickoffTrans(GM_OIWFS_RAD baseAngle, MATRIX T);

static void InitTelescopeToBaseTrans(MATRIX T);

static int PickBestOrientation(double orient1, double orient2);

static double sign(double x);


/*
 * Print matrix debugging info if matrix printing is enabled
 */

#ifdef DEBUG_MATRIX
#define PRINT_MATRIX(S,M)  PrintMatrix(S,M)
static void PrintMatrix(char * s, MATRIX A);
#else
#define PRINT_MATRIX(S,M)  /* define PRINT_MATRIX as a NOP */
#endif


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * gmOiwfsSetOffsets
 *
 * INVOCATION:
 * gmOiwfsSetOffsets(base_offset, pickoff_offset, x_offset, y_offset);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  (>) base (double)    Offset required to convert base angle from
 *                       encoder value to mask reference frame (rad).
 *  (>) pickoff (double) Offset required to convert pickoff angle from
 *                       encoder value to base reference frame (rad).
 *  (>) x (double)       Offset required to convert X coordinate to
 *                       mask reference frame (mm).
 *  (>) y (double)       Offset required to convert Y coordinate to
 *                       mask reference frame (mm).
 *
 * FUNCTION VALUE:
 * (void)   Not applicable.
 *
 * PURPOSE:
 * Initialize the calibration coefficients for the probe positioning
 *
 * DESCRIPTION:
 * The calibration coefficients are simply copied from the parameters
 * into their appropriate variable.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * The calibration coefficients must have been previously calculated.
 *
 * SEE ALSO:
 *  - gmOiwfsGetOffsets
 *  - gmOiwfsCalcProbePosition
 *  - gmOiwfsCalcProbeAngles
 *  - gmOiwfsCalcVerticalRange
 *  - gmOiwfsCalcVerticalRange
 *  - CalcApproxProbeAngles
 *  - CalcApproxProbePosition
 *  - CalcExactProbePosition
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

void gmOiwfsSetOffsets
(
    GM_OIWFS_RAD  base,      /* (in) baseOffset coefficient     */
    GM_OIWFS_RAD  pickoff,   /* (in) pickoffOffset coefficient  */
    GM_OIWFS_MM   x,         /* (in) xOffset coefficient        */
    GM_OIWFS_MM   y          /* (in) yOffset coefficient        */
)
{
    /*
     * Update the local offset information
     */

    baseOffset = base;
    pickoffOffset = pickoff;
    xOffset = x;
    yOffset = y;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * gmOiwfsGetOffsets
 *
 * INVOCATION:
 * gmOiwfsGetOffsets(&base_offset, &pickoff_offset, &x_offset, &y_offset);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  (>) base (double *)    Offset required to convert base angle from
 *                         encoder value to mask reference frame (rad).
 *  (>) pickoff (double *) Offset required to convert pickoff angle from
 *                         encoder value to base reference frame (rad).
 *  (>) x (double *)       Offset required to convert X coordinate to
 *                         mask reference frame (mm).
 *  (>) y (double *)       Offset required to convert Y coordinate to
 *                         mask reference frame (mm).
 *
 * FUNCTION VALUE:
 * (int)   Returns 0 if calibration offsets are properly copied into the
 *         locations specified by the parameters.  Non-zero return values
 *         indicate one or more NULL pointers as parameters.
 *
 *         1  -  NULL base    pointer.
 *         2  -  NULL pickoff pointer.
 *         4  -  NULL x       pointer.
 *         8  -  NULL y       pointer.
 *
 *         All of the four NULL pointer tests are performed, and the resulting
 *         error bits are added together, so each value 1..15 corresponds to a
 *         different set of one or more NULL pointers.
 *
 * PURPOSE:
 * Return the current calibration coefficients
 *
 * DESCRIPTION:
 * The calibration coefficients are simply copied to the appropriate parameters.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * For this to return meaningful values, the calibration coefficients must have
 * been set previously.
 *
 * SEE ALSO:
 *  - gmOiwfsSetOffsets
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */


int gmOiwfsGetOffsets
(
    GM_OIWFS_RAD * base,      /* (out) Pointer to baseOffset coefficient     */
    GM_OIWFS_RAD * pickoff,   /* (out) Pointer to pickoffOffset coefficient  */
    GM_OIWFS_MM  * x,         /* (out) Pointer to xOffset coefficient        */
    GM_OIWFS_MM  * y          /* (out) Pointer to yOffset coefficient        */
)
{
    int error_bits = 0;

    /*
     *  If base offset return pointer is valid then return the current
     *  base offset value.
     */

    if (base == NULL)
    {
        error_bits = 1;
    }
    else
    {
        *base = baseOffset;
    }


    /*
     *  If pickoff offset return pointer is valid then return the current
     *  base offset value.
     */

    if (pickoff == NULL)
    {
        error_bits = error_bits + 2;
    }
    else
    {
        *pickoff = pickoffOffset;
    }


    /*
     *  If X offset return pointer is valid then return the current
     *  base offset value.
     */

    if (x == NULL)
    {
        error_bits = error_bits + 4;
    }
    else
    {
        *x = xOffset;
    }

    /*
     *  If Y offset return pointer is valid then return the current
     *  base offset value.
     */

    if (y == NULL)
    {
        error_bits = error_bits + 8;
    }
    else
    {
        *y = yOffset;
    }


    return error_bits;
}



/*
 ***********************************************************************    
 *
 *+
 * FUNCTION NAME:
 * gmOiwfsCalculateProbeAngles
 *
 * INVOCATION:
 *  status = gmOiwfsCalculateProbeAngles(xTarget, yTarget, solution,
 *                                       &baseAngle &pickoffAngle, &probeAngle);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  (>) xTarget (double) Desired probe X target in physical space (mm)
 *  (>) yTarget (double) Desired probe Y target in physical space (mm)
 *  (>) solution (int)   Desired solution
 *                       0 - Use previous solution (#1 or #2)
 *                       1 - Use solution #1 to calculate angles
 *                       2 - Use solution #2 to calculate angles
 *                       3 - Use probe angle to determine solution
 *  (<) baseAngle (double) Calculated base stage angle (rad)
 *  (<) pickoffAngle (double) Calculated pickoff stage angle (rad)
 *  (<) probeAngle (double) Calculated probe tip angle (rad)
 *
 * FUNCTION VALUE:
 *  (int) success code:
 *	 0 - Angles calculated successfully
 *	-1 - Given target outside probe patrol area.
 *
 * PURPOSE:
 *  Convert X-Y stage targets to probe mechanism angles
 *
 * DESCRIPTION:
 *  The X,Y coordinate is first shifted by xOffset, yOffset.  Approximate
 *  base and pickoff angles are then calculated using inverse kinematics.
 *  The angles are then "improved" by performing gmOiwfsRefineSteps
 *  applications of RefineProbeAngles.  The final step ensures that the
 *  calculated angles are within the range -180...180 degrees (-PI .. PI rad).
 *
 * EXTERNAL VARIABLES:
 *  gmOiwfsRefineSteps
 *
 * PRIOR REQUIREMENTS:
 *  None.
 *
 * SEE ALSO:
 *  - CalcApproxProbeAngles
 *  - RefineProbeAngles
 *
 * DEFICIENCIES:
 *  None.
 *
 ***********************************************************************
 *
 */

int gmOiwfsCalculateProbeAngles (GM_OIWFS_MM xTarget, 
                                 GM_OIWFS_MM yTarget, 
                                 int solution,
                                 GM_OIWFS_RAD * baseAngle,
                                 GM_OIWFS_RAD * pickoffAngle,
                                 GM_OIWFS_RAD * probeAngle)
{
    int  status;              /* function status return */

  
    /* 
     *  Translate mask coordinates to physical coordinates
     */

    xTarget = xTarget + xOffset;
    yTarget = yTarget + yOffset;


    /* 
     *  Do the inverse kinematics thing to get an approximate value for the
     *  required base and pickoff stage angles
     */

    status = CalcApproxProbeAngles(xTarget, yTarget, solution, 
                                   baseAngle, pickoffAngle, probeAngle);

    /*
     *  If a more precise position is required then refine the base and
     *  pickoff stage angles.
     */
  
    if ((gmOiwfsRefineSteps > 0) && (status == 0))
    {
        status = RefineProbeAngles(xTarget, yTarget, 
                                   baseAngle, pickoffAngle, probeAngle);
    }


    /*
     * Force both of the angles to be within the range -180 ... 180 degrees.
     * The angle is first shifted by 180 degree to make the range 0 ... 360,
     * then by an extra 360 degrees to ensure that there are no negative
     * angles due to rounding errors.
     */

    *baseAngle = fmod(3.0 * M_PI + *baseAngle, 2.0 * M_PI) - M_PI;
    *pickoffAngle = fmod(3.0 * M_PI + *pickoffAngle, 2.0 * M_PI) - M_PI;

    return status;
}


/*
 ***********************************************************************    
 *
 *+
 * FUNCTION NAME:
 * gmOiwfsCalculateProbePosition
 *
 * INVOCATION:
 * status = gmOiwfsCalculateProbePosition (baseAngle, 
 *                                         pickoffAngle, &x, &y, &z, &angle);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  (>) baseAngle (double)      Current base stage angle (rad)
 *  (>) pickoffAngle (double)   Current pickoff stage angle (rad)
 *  (<) xPosition (double)      Calculated probe X position (mm)
 *  (<) yPosition (double)      Calculated probe Y position (mm)
 *  (<) zPosition (double)      Calculated probe Z position (mm)
 *  (<) probeAngle (double)     Calculated probe angle (rad)
 *
 * FUNCTION VALUE:
 *  (int) success code:
 *     0 - Angles calculated successfully
 *    -1 - Given angles can not be converted.
 *
 * PURPOSE:
 *  Convert current base and pickoff stage angles to probe positon.
 *
 * DESCRIPTION:
 *  This is a wrapper function which calls either CalcApproxProbePosition
 *  (if no correction for stage tilt is to be done) or the more accurate
 *  CalcExactProbePosition (if gmOiwfsStageTiltCorrection != 0).
 *  
 *  After computing the X and Y position of the probe, and the focus
 *  term, the X and Y positions are corrected using the X and Y offset values.
 *
 * EXTERNAL VARIABLES:
 *  gmOiwfsStageTiltCorrection
 *
 * PRIOR REQUIREMENTS:
 *  None.
 *
 * SEE ALSO:
 *  - CalcApproxProbePosition
 *  - CalcExactProbePosition
 *
 * DEFICIENCIES:
 *  None.
 *
 ***********************************************************************
 *
 */


int gmOiwfsCalculateProbePosition
(
    GM_OIWFS_RAD    baseAngle,        /* current base stage angle         */ 
    GM_OIWFS_RAD    pickoffAngle,     /* current pickoff stage angle      */
    GM_OIWFS_MM   * xPos,             /* calculated probe x coordinate    */ 
    GM_OIWFS_MM   * yPos,             /* calculated probe y coordinate    */
    GM_OIWFS_MM   * focus,            /* calculated probe z coordinate    */
    GM_OIWFS_RAD  * probeAngle        /* calculated probe angle           */
)
{
    int     status;                   /* function status return           */

    /*
     *  Compute *xPos, *yPos and *focus using either the approximate
     *  (but faster) method or the more exact (but slower) method.
     */

    if (gmOiwfsStageTiltCorrection == 0)
    {
        status =  CalcApproxProbePosition(baseAngle, 
                                          pickoffAngle, 
                                          xPos, 
                                          yPos, 
                                          focus);
    }
    else
    {
        status =  CalcExactProbePosition(baseAngle, 
                                         pickoffAngle, 
                                         xPos, 
                                         yPos, 
                                         focus);
    }

    /*
     *  If the calculation of xPos and yPos was successful, then
     *  convert the physical coordinates back to mask coordinates.
     */

    if (status == 0)
    {
        *xPos = *xPos - xOffset;
        *yPos = *yPos - yOffset;


        /*
         *  Compute probe angle in mask reference frame
         */

         *probeAngle = (baseAngle-baseOffset) + (pickoffAngle-pickoffOffset);

        if (*probeAngle < 0.0)
        {
            *probeAngle = *probeAngle + M_PI;
        }
        else
        {
            *probeAngle = *probeAngle - M_PI;
        }
    }

    return status;
}

/*
 ***********************************************************************    
 *
 *+
 * FUNCTION NAME:
 * gmOiwfsCalculateVerticalRange
 *
 * INVOCATION:
 * status = gmOiwfsCalculateVerticalRange(base1, pickoff1, base2, pickoff2,
 *                                        lowerLimit, upperLimit, &yMin, &yMax,
 *                                        &mid_base, &mid_pickoff);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  (>) base1       (GM_OIWFS_RAD)    Base angle of start of move.
 *  (>) pickoff1    (GM_OIWFS_RAD)    Pickoff angle of start of move.
 *  (>) base12      (GM_OIWFS_RAD)    Base angle of end of move.
 *  (>) pickoff2    (GM_OIWFS_RAD)    Pickoff angle of end of move.
 *  (>) lowerLimit  (GM_OIWFS_MM)     Lower limit of vertical travel.
 *  (>) upperLimit  (GM_OIWFS_MM)     Upper limit of vertical travel.
 *  (<) yMin        (* GM_OIWFS_MM)   Miny value encountered during move.
 *  (<) yMax        (* GM_OIWFS_MM)   Maxy value encountered during move.
 *  (<) mid_base    (* GM_OIWFS_RAD)  Base angle of calculated mid-point.
 *  (<) mid_pickoff (* GM_OIWFS_RAD)  Pickoff angle of calculated mid-point.
 *
 * FUNCTION VALUE:
 *  (long) success code:
 *     0 - Move is predicted to not hit limits.  Mid-point is not calculated.
 *    -1 - Move is predicted to hit limits.  Mid-point is calculated.
 *
 * PURPOSE:
 *  Determine range of vertical motion encountered during a probe move
 *
 * DESCRIPTION:
 *  The probe move is simulated by advancing the base and pickoff angles
 *  in small steps.
 *
 * EXTERNAL VARIABLES:
 *  gmOiwfsStageTiltCorrection  (local to file)
 *  gmOiwfsRefineSteps          (local to file)
 *
 * PRIOR REQUIREMENTS:
 *  Calibration coefficients must be set.
 *
 * SEE ALSO:
 *  - gmOiwfsCalculateProbeAngles
 *  - gmOiwfsCalculateProbePosition
 *
 * DEFICIENCIES:
 *  Not very efficient!
 *
 ***********************************************************************
 *
 */


int gmOiwfsCalculateVerticalRange
(
    GM_OIWFS_RAD base1,         /* Base angle of start of move              */
    GM_OIWFS_RAD pickoff1,      /* Pickoff angle of start of move           */
    GM_OIWFS_RAD base2,         /* Base angle of end of move                */
    GM_OIWFS_RAD pickoff2,      /* Pickoff angle of end of move             */
    GM_OIWFS_MM lowerLimit,     /* Lower limit of vertical travel           */
    GM_OIWFS_MM upperLimit,     /* Upper limit of vertical travel           */
    GM_OIWFS_MM * yMin,         /* Miny value encountered during move       */
    GM_OIWFS_MM * yMax,         /* Maxy value encountered during move       */
    GM_OIWFS_RAD * mid_base,    /* Base angle of calculated mid-point       */
    GM_OIWFS_RAD * mid_pickoff) /* Pickoff angle of calculated mid-point    */
{
    GM_OIWFS_RAD orient;        /* Temporary probe angle storage            */
    GM_OIWFS_RAD base;          /* base position storage variable           */
    GM_OIWFS_RAD pickoff;       /* pickoff position storage variable        */
    GM_OIWFS_RAD base_delta;    /* size of virtual probe base offset step   */
    GM_OIWFS_RAD pickoff_delta; /* size of virtual probe pickoff offset step*/
    GM_OIWFS_RAD max_delta;     /* larger of the two above..                */
    GM_OIWFS_MM x1;             /* Probe X coordinate at start of move      */
    GM_OIWFS_MM y1;             /* Probe Y coordinate at start of move      */
    GM_OIWFS_MM x2;             /* Probe X coordinate at end of move        */
    GM_OIWFS_MM y2;             /* Probe Y coordinate at end of move        */
    GM_OIWFS_MM xt;             /* Temporary X coordinate                   */
    GM_OIWFS_MM yt;             /* Temporary Y coordinate                   */
    GM_OIWFS_MM zt;             /* Temporary Z coordinate                   */
    GM_OIWFS_MM x_mid;          /* X coordinate of midpoint position        */
    GM_OIWFS_MM y_mid;          /* Y coordinate of midpoint position        */ 
    int    i;                   /* generic index thing                      */
    int    steps;               /* number of virtual probe steps to do      */
    int    status;              /* function return status                   */
    int    save_tilt;           /* local copy of tilt correction flag       */
    int    save_refine;         /* local copy of refinement level flag      */
    int    midStatus;           


    /*
     *  Save the current setting of gmOiwfsStageTiltCorrection.
     */

    save_tilt = gmOiwfsStageTiltCorrection;
    save_refine = gmOiwfsRefineSteps;


    /*
     *  We need to perform the corrections for the stage tilt or the
     *  calculation of the probe position will be inaccurate. 
     *  Also need to save the current solution index so that it can be
     *  restored (since this does not actually do the move).
     */

    gmOiwfsStageTiltCorrection = 1;
    gmOiwfsRefineSteps = 1;


    /*
     *  Determine the base and pickoff angles for (x2, y2).
     */

    /*
     * gmOiwfsCalculateProbeAngles(x2, y2, sol2, &base2, &pickoff2, &orient);
     */

    (void) gmOiwfsCalculateProbePosition(base2, pickoff2, 
                                         &x2, &y2, &zt, &orient);

    base_delta = base2 - base1;
    pickoff_delta = pickoff2 - pickoff1;

    if (fabs(base_delta) > fabs(pickoff_delta))
    {
        /*
         * Large step = CALC_VERT_RANGE_STEP_SIZE (in the correct direction)
         */

        max_delta = fabs(base_delta);
        if (max_delta > CALC_VERT_RANGE_STEP_SIZE)
        {
            base_delta = sign(base_delta) * CALC_VERT_RANGE_STEP_SIZE;
            pickoff_delta = fabs(base_delta) * pickoff_delta / max_delta;
            steps = (int) fabs(max_delta / base_delta);
        }
        else
        {
            steps = 0;
        }

    }
    else
    {
        /*
         * Large step = CALC_VERT_RANGE_STEP_SIZE (in the correct direction)
        */

        max_delta = fabs(pickoff_delta);
        if (max_delta > CALC_VERT_RANGE_STEP_SIZE)
        {
            pickoff_delta = sign(pickoff_delta) * CALC_VERT_RANGE_STEP_SIZE;
            base_delta = fabs(pickoff_delta) * base_delta / max_delta;
            steps = (int) fabs(max_delta / pickoff_delta);
        }
        else
        {
            steps = 0;
        }
    }


    /*
     *  Determine the (x,y) starting location.
     */

    (void) gmOiwfsCalculateProbePosition(base1, pickoff1, 
                                       &x1, &y1, &zt, &orient);

    /*
     *  Set yMin and yMax to the minimum and maximum y values of the
     *  source and destination of the move.
     */

    *yMin = y1;
    *yMax = y1;

    if (y2 < y1)
    {
        *yMin = y2;
    }
    else
    {
        *yMax = y2;
    }


    /*
     *  Check the position of the probe every CALC_VERT_RANGE_STEP_SIZE
     *  degrees.
     */

    base = base1;
    pickoff = pickoff1;

    for (i=0; i < steps; ++i)
    {
        base = base + base_delta;
        pickoff = pickoff + pickoff_delta;

        (void) gmOiwfsCalculateProbePosition(base, 
                                             pickoff, &xt, &yt, &zt, &orient);


        /*
         *  Update the minimum or maximum y value if required.
         */

        if (yt < *yMin)
        {
            *yMin = yt;
        }
        else if (yt > *yMax)
        {
            *yMax = yt;
        }
    }

    /*
     *  Now check to see if the calculated route would cause the probe
     *  to travel outside limits...
     */

    if ((*yMin < lowerLimit) || (*yMax > upperLimit))
    {
        status = GM_OIWFS_MIDPOINT_OK;
        x_mid = 0.5 * (x1 + x2);

        if (*yMin < lowerLimit)
        {
            y_mid = 0.5 * (y1 + y2) + (lowerLimit - *yMin);
        }
        else
        {
            y_mid = 0.5 * (y1 + y2) + (upperLimit - *yMax);
        }


        /*
         *  Now need to convert (x_mid, y_mid) into the appropriate angles.
         */

        gmOiwfsRefineSteps = 2;

        midStatus = gmOiwfsCalculateProbeAngles(x_mid, y_mid, 1, 
                                                mid_base, mid_pickoff, &orient);
        if (midStatus  == -1)
        {
            status = GM_OIWFS_MIDPOINT_EMERGENCY;
        
            logMsg ("Calculated mid-point out of range, using emergency values X=%d Y=%d\n",
                    (int) EMERGENCY_X, (int) EMERGENCY_Y, 0, 0, 0, 0);

            (void) gmOiwfsCalculateProbeAngles(EMERGENCY_X, EMERGENCY_Y, 1, 
                                               mid_base, mid_pickoff, &orient);
        }

    }
    else
    {
        status = GM_OIWFS_MIDPOINT_NONE;
        *mid_base = 0.0;
        *mid_pickoff = 0.0;
    }


    /*
     *  Restore the previous values of gmOiwfsStageTiltCorrection and
     *  gmOiwfsRefineSteps before returning status.
     */

    gmOiwfsStageTiltCorrection = save_tilt;
    gmOiwfsRefineSteps = save_refine;

    return status;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * CalcApproxProbeAngles
 *
 * INVOCATION:
 * status = CalcApproxProbeAngles (x, y, solution, &base, &pickoff, &probeA);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  (>) xTarget (double) Desired probe X target in physical space (mm)
 *  (>) yTarget (double) Desired probe Y target in physical space (mm)
 *  (>) solution (int)   Desired solution
 *                       0 - Use previous solution (#1 or #2)
 *                       1 - Use solution #1 to calculate angles
 *                       2 - Use solution #2 to calculate angles
 *                       3 - Use probe angle to determine solution
 *  (<) baseAngle (double) Calculated base stage angle (rad)
 *  (<) pickoffAngle (double) Calculated pickoff stage angle (rad)
 *  (<) probeAngle (double) Calculated probe tip angle (rad)
 *
 * FUNCTION VALUE:
 * (int) success code
 *     0 - Angles calculated successfully
 *    -1 - Given target outside probe patrol area.
 *
 * PURPOSE:
 * Calculate base and pickoff stage angles required to place the probe
 * in a given X-Y position
 *
 * DESCRIPTION:
 * Use inverse kinematics to determine the two possible solutions to the
 * problem of where to move the base and pickoff stages to place the probe
 * in a given X-Y position.
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

static int CalcApproxProbeAngles 
(
    GM_OIWFS_MM    xTarget,        /* Desired probe X target                */
    GM_OIWFS_MM    yTarget,        /* Desired probe Y target                */
    int            solution,       /* Solution to use                       */
    GM_OIWFS_RAD * baseAngle,      /* Calculated base stage angle           */
    GM_OIWFS_RAD * pickoffAngle,   /* Calculated pickoff stage angle        */
    GM_OIWFS_RAD * probeAngle)     /* Calculated probe tip angle            */
{
    double   x_prime;     /* X position of target relative to base          */
    double   y_prime;     /* Y position of target relative to base          */
    double   r;           /* Vector from target to base rotation axis       */
    double   alpha;       /* Angle of vector "r" above                      */
    double   phi;         /* angle opposite the side "r" above              */
    double   theta;       /* angle opposite the pickoff arm                 */
    double   cval;        /* cosine of angle opposite vector "r"            */
    double   sval;        /* sine of angle opposite the pickoff arm         */
    double   base1;       /* Base angle from solution #1                    */
    double   pickoff1;    /* Pickoff angle from solution #1                 */
    double   orient1;     /* Probe angle from solution #1                   */
    double   base2;       /* Base angle from solution #2                    */
    double   pickoff2;    /* Pickoff angle from solution #2                 */
    double   orient2;     /* Probe angle from solution #2                   */


    /*
     * Convert from mask coordinates to base coordinates
     * which use the axis of rotation of the base arm as the origin.
     */

    x_prime = xTarget - BASE_ORIGIN_X;
    y_prime = yTarget - BASE_ORIGIN_Y;


    /*
     *  Compute distance from the target to the axis of base arm.
     */

    r = sqrt(SQR(x_prime) + SQR(y_prime));


    /*  If the target is too far from (or too close to) the axis of
     *  the base arm, then the target cannot be reached by the
     *  probe.
     */

    if (r > BASE_ARM_LENGTH + PICKOFF_ARM_LENGTH)
    {
        return -1;
    }
    else if (r < PICKOFF_ARM_LENGTH - BASE_ARM_LENGTH)
    {
        return -1;
    }


    /*
     *  Compute the angle from the base origin to the target
     *  position.
     */

    alpha = atan2(y_prime,x_prime);


    /*
     *  For a triangle with sides A, B, and C, and opposing angles
     *  a, b and c, we have the following relationship:
     *
     *  cos a = (C*C + B*B - A*A) / (2 * B * C)
     *
     *  Using BASE_ARM_LENGTH as side B and PICKOFF_ARM_LENGTH as side C,
     *  we have r as side A and so we can compute the angle phi as angle a.
     */

    cval = (r*r - SQR(PICKOFF_ARM_LENGTH) - SQR(BASE_ARM_LENGTH))
           / (2.0 * PICKOFF_ARM_LENGTH * BASE_ARM_LENGTH);

    phi = acos(cval);


    /*
     *  For any triangle, the we have the equality
     *
     *  sin(a) / A  =  sin(b) / B  =  sin(c) / C
     *
     *  so if phi is the angle opposite the side of length r,
     *  and theta is the angle opposite the pickoff arm,
     *  we have,
     *  
     *  sin(theta) / PICKOFF_ARM_LENGTH = sin(phi) / r.
     */

    sval = PICKOFF_ARM_LENGTH * sin(phi) / r;


    /* 
     *  Now that we have sin(theta), we can compute theta.
     */

    if (SQR(PICKOFF_ARM_LENGTH) > SQR(r) + SQR(BASE_ARM_LENGTH))
    {
        theta = M_PI - asin(sval);
    }
    else
    {
        theta = asin(sval);
    }


    /*
     *  Compute both sets of solutions for the stage angles and 
     *  probe orientation angle based on the angle (the orientation 
     *  of the "triangle") and the angles within the triangle.
     */

    base1 = alpha - theta;
    pickoff1 = phi;
    orient1 = base1 + pickoff1;

    base2 = alpha + theta;
    pickoff2 = -phi;
    orient2 = base2 + pickoff2;


    /*
     *  Determine which solution to use.
     */

    if (solution > 0)
    {
        gmOiwfsSolutionId = solution;
    }


    /*
     *  Make sure that the previous solution is set to which
     *  of the two solutions will actually be used.
     */

    if (gmOiwfsSolutionId == 3)
    {
        gmOiwfsSolutionId = PickBestOrientation(orient1, orient2);
    }

  
    /*
     *  Now return the appropriate angles.
     */

    if (gmOiwfsSolutionId == 1)
    {
        *baseAngle = base1 + baseOffset;
        *pickoffAngle = pickoff1 + pickoffOffset;
        *probeAngle = orient1;
    }
    else
    {
        *baseAngle = base2 + baseOffset;
        *pickoffAngle = pickoff2 + pickoffOffset;
        *probeAngle = orient2;
    }

    return 0;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * RefineProbeAngles
 *
 * INVOCATION:
 * status = RefineProbeAngles (xTarget,
                               yTarget, baseAngle, pickoffAngle, probeAngle);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  (>) xTarget (double) Desired probe X target in physical space (mm)
 *  (>) yTarget (double) Desired probe Y target in physical space (mm)
 *  (<) baseAngle (double) Calculated base stage angle (rad)
 *  (<) pickoffAngle (double) Calculated pickoff stage angle (rad)
 *  (<) probeAngle (double) Calculated probe tip angle (rad)
 *
 * FUNCTION VALUE:
 * (int)  0 if successful.  At moment, always returns 0.
 *
 * PURPOSE:
 * Calculate a more precise (but takes longer) pair of base and pickoff
 * angles required to place the probe in a given X-Y position
 *
 * DESCRIPTION:
 * Iterate a fixed number of times to reduce the error between the
 * calculated probe position based on the base and pickoff stage
 * angles and the actual probe position desired.   The algorithm that
 * does the probe angle to position transformation is more precise than
 * the position to angle algorithm.....
 *
 * EXTERNAL VARIABLES:
 * gmOiwfsRefineSteps
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * - other function name.
 *
 * DEFICIENCIES:
 * Should do some checking to make sure refinement succeeded.
 *-
 ************************************************************************
 */


static int RefineProbeAngles
(
  GM_OIWFS_MM    xTarget,      /* X coordinate of target pos.               */
  GM_OIWFS_MM    yTarget,      /* Y coordinate of target pos.               */
  GM_OIWFS_RAD * baseAngle,    /* Original (in) improved (out) base angle   */
  GM_OIWFS_RAD * pickoffAngle, /* Original (in) improved (out) pickoff angle*/
  GM_OIWFS_RAD * probeAngle    /* Original (in) improved (out) probe angle  */
)
{
    GM_OIWFS_MM      xProbe;    /* Calculated probe X position              */
    GM_OIWFS_MM      yProbe;    /* Calculated probe Y position              */
    GM_OIWFS_MM      focus;     /* Calculated probe Z position              */
    double      xNew;           /* Refined X target position                */
    double      yNew;           /* Refined Y target position                */
    double      xError;         /* X error between calc and desired position*/
    double      yError;         /* Y error between calc and desired position*/
    int         i;              /* iteration counter                        */


    /*
     *  Make a copy of the target position.  This will change as the
     *  refinements are made, but the original xTarget and yTarget must
     *  be preserved so that it can be used to measure the error (difference
     *  between calculated probe position and target position).
     */

    xNew = xTarget;
    yNew = yTarget;
  

    /*
     *  Perform the required number of refinement steps.
     */

    for (i=0; i < gmOiwfsRefineSteps; ++i)
    {

        /*
         *  Compute the exact location of the probe if it was moved to
         *  the angles specified by *baseAngle and *pickoffAngle.
         *  This location is stored in (xProbe, yProbe).
         */

        CalcExactProbePosition(*baseAngle, 
                               *pickoffAngle, 
                               &xProbe, 
                               &yProbe,
                               &focus);
        
        /*
         *  Compute the positioning error (diff between the target position 
         *  and the calculated probe position.
         */

        xError = xProbe - xTarget;
        yError = yProbe - yTarget;


        /*
         *  Adjust the target position so that the probe positioning error
         *  is approximately cancelled (by this change to the target position).
         */

        xNew = xNew - xError;
        yNew = yNew - yError;


        /*
         *  Get new probe angles for the adjusted target position.  These angles
         *  should correspond to an improved probe position.
         */

        CalcApproxProbeAngles(xNew, 
                              yNew, 
                              gmOiwfsSolutionId, 
                              baseAngle, 
                              pickoffAngle, 
                              probeAngle);
    }

    return 0;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * CalcApproxProbePosition
 *
 * INVOCATION:
 * status = CalcApproxProbePosition (base, pickoff, xPos, yPos, focus);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) baseAngle    (GM_OIWFS_RAD) Current base stage angle (rad)
 * (>) pickoffAngle (GM_OIWFS_RAD) Current pickoff stage angle (rad)
 * (<) xPos         (GM_OIWFS_MM)  Calculated probe X position (mm)
 * (<) yPos         (GM_OIWFS_MM)  Calculated probe Y position (mm)
 * (<) focus        (GM_OIWFS_MM)  Calculated probe Z position (mm)
 *
 * FUNCTION VALUE:
 * (int) 0 if successful (always successful at moment).
 *
 * PURPOSE:
 * Calculate the approximate (fast) probe position based on the 
 * base and pickoff stage angles
 *
 * DESCRIPTION:
 * Perform a simple trig calculation to see where the probe should
 * be in the base frame of reference given the current base and
 * pickoff stage angles.
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

static int CalcApproxProbePosition
(
    GM_OIWFS_RAD     baseAngle,      /* Current base stage angle            */
    GM_OIWFS_RAD     pickoffAngle,   /* Current pickoff stage angle         */
    GM_OIWFS_MM    * xPos,           /* Calculated probe X position         */
    GM_OIWFS_MM    * yPos,           /* Calculated probe Y position         */
    GM_OIWFS_MM    * focus           /* Calculated probe Z position         */
)
{

    /*
     * Remove encoder offsets
     */
 
    baseAngle = baseAngle - baseOffset;
    pickoffAngle = pickoffAngle - pickoffOffset;


    /*
     * Then calculate the equivalent probe X-Y position
     */

    *xPos = BASE_ORIGIN_X + BASE_ARM_LENGTH * cos(baseAngle) + 
            PICKOFF_ARM_LENGTH * cos(baseAngle+pickoffAngle);

    *yPos = BASE_ORIGIN_Y + BASE_ARM_LENGTH * sin(baseAngle) +
            PICKOFF_ARM_LENGTH * sin(baseAngle+pickoffAngle);

    *focus = 0.0;

    return 0;  /* always a success */
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * CalcExactProbePosition
 *
 * INVOCATION:
 * status = CalcExactProbePosition (baseAngle, pickoffAngle, xPos, yPos);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) baseAngle (double) Current base stage angle (rad)
 * (>) pickoffAngle (double) Current pickoff stage angle (rad)
 * (<) xPosition (double) Calculated probe position (mm)
 * (<) yPosition (double) Calculated probe position (mm)
 *
 * FUNCTION VALUE:
 * (int) calculation success code.
 *
 * PURPOSE:
 * Convert base and pickoff stage angles to probe position in the
 * probe frame of reference
 *
 * DESCRIPTION:
 * Calculate the exact probe positon by converting the current base and
 * pickoff stage angles to a probe position in the telescope fame of
 * reference then transforming that back to the probe frame.
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

static int CalcExactProbePosition
(
    GM_OIWFS_RAD    baseAngle,      /* Current base stage angle             */
    GM_OIWFS_RAD    pickoffAngle,   /* Current pickoff stage angle          */
    GM_OIWFS_MM   * xPos,           /* Calculated probe X position          */
    GM_OIWFS_MM   * yPos,           /* Calculated probe Y position          */
    GM_OIWFS_MM   * focus           /* Calculated probe Z position          */
)
{
    MATRIX  A;                      /* working matrix array                 */
    VECTOR  X;                      /* working vector array                 */


    /*
     * Remove encoder offsets
     */
                                                                            
    baseAngle = baseAngle - baseOffset - BETA_TB;
    pickoffAngle = pickoffAngle - pickoffOffset;


    /* Initialize the vector B to the position of the probe in its own 
     * reference frame
     */

    X[0] = 0.0;
    X[1] = 0.0;
    X[2] = 0.0;
    X[3] = 1.0;


    /* 
     *  Update the vector using the pickoff stage to probe transformation.
     */

    InitPickoffToProbeTrans(pickoffAngle, A);

    PRINT_MATRIX("Pickoff -> Probe", A);

    MatVecMult4(A, X);


    /*
     *  And again using the base to pickoff stage transformation.
     */

    if (baseAngle < 0.0)
    {
        baseAngle = baseAngle + M_PI;
    } 
    else
    {
        baseAngle = baseAngle - M_PI;
    }

    InitBaseToPickoffTrans(baseAngle, A);

    PRINT_MATRIX("Base -> Pickoff", A);

    MatVecMult4(A, X); 


    /*
     *  This leaves the vector representation of the actual probe position
     *  in the probe frame of reference.  Since the telescope frame is
     *  simply a rotated probe frame use the telescope to base transformation
     *  to represent the probe position in the base fame of reference.
     */

    InitTelescopeToBaseTrans(A);

    PRINT_MATRIX("Telescope -> Base", A);

    MatVecMult4(A, X);

 
    /* 
     * Finally, de-rotate the base coordinate system to compensate for
     * using the telescope instead of probe transformation.
     */

    *xPos = -X[0];
    *yPos = X[2];

    *focus = X[1];

    return 0;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * InitPickoffToProbeTrans
 *
 * INVOCATION:
 * InitPickoffToProbeTrans (pickoff, matrix);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pickoff (GM_OIWFS_RAD)   current pickoff stage angle
 * (<) matrix (MATRIX)          resulting transformation matrix
 *
 * FUNCTION VALUE:
 * None.
 *
 * PURPOSE:
 * Constructs the transformation matrix which transforms coordinates in the
 * pickoff arm frame of reference into coordinates in probe reference frame.
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

static void InitPickoffToProbeTrans
(
    GM_OIWFS_RAD   pickoffAngle,   /* Current pickoff stage angle          */
    MATRIX         T               /* Resulting transformation matrix      */
)
{
    double    Cp;      /* Cosine of pickoff angle                         */
    double    Sp;      /* Sine of pickoff angle                           */
    double    C4;      /* Cosine of pickoff to probe vector               */
    double    S4;      /* Sine of pickoff to probe vector                 */

    /*
     *  Initialize the trig values.
     */

    C4 = cos(GAMMA_PM);
    S4 = sin(GAMMA_PM);
    Cp = cos(pickoffAngle);
    Sp = sin(pickoffAngle);


    /*
     *  Now initialize the matrix.
     */

    T[0][0] =  C4 * Cp;
    T[0][1] = -S4 * Cp;
    T[0][2] =  Sp;
    T[0][3] =  C4 * Cp * PICKOFF_ARM_LENGTH - S4 * Cp * PICKOFF_ARM_HEIGHT;

    T[1][0] =  S4;
    T[1][1] =  C4;
    T[1][2] =   0;
    T[1][3] =  S4 * PICKOFF_ARM_LENGTH + C4 * PICKOFF_ARM_HEIGHT;

    T[2][0] =  -C4 * Sp;
    T[2][1] =   S4 * Sp;
    T[2][2] =   Cp;
    T[2][3] =  -C4 * Sp * PICKOFF_ARM_LENGTH + S4 * Sp * PICKOFF_ARM_HEIGHT;

    T[3][0] =  0;
    T[3][1] =  0;
    T[3][2] =  0;
    T[3][3] =  1;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * InitBaseToPickoffTrans
 *
 * INVOCATION:
 * InitBaseToPickoffTrans (base, matrix);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) base (GM_OIWFS_RAD)  current base stage angle
 * (!) matrix (MATRIX)      resulting transform
 *
 * FUNCTION VALUE:
 * None
 *
 * PURPOSE:
 * Constructs the transformation matrix which transforms coordinates in the
 * base arm frame of reference into coordinates in pickoff arm reference frame.
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

static void InitBaseToPickoffTrans
(
    GM_OIWFS_RAD  baseAngle,      /* current base stage angle             */
    MATRIX        T               /* resulting transformation matrix      */
)
{
    double    Cb;      /* Cosine of base angle                            */
    double    Sb;      /* Sine of base angle                              */
    double    C3;      /* Cosine of base to pickoff vector                */
    double    S3;      /* Sine of base to pickoff vector                  */

    /*
     *  Initialize the trig values.
     */

    C3 = cos(GAMMA_BP);
    S3 = sin(GAMMA_BP);
    Cb = cos(baseAngle);
    Sb = sin(baseAngle);


    /*
     *  Now initialize the matrix.
     */

    T[0][0] =  C3 * Cb;
    T[0][1] = -S3 * Cb;
    T[0][2] =  Sb;
    T[0][3] =  Cb * BASE_ARM_LENGTH;

    T[1][0] =  S3;
    T[1][1] =  C3;
    T[1][2] =   0;
    T[1][3] =  BASE_ARM_HEIGHT;

    T[2][0] =  -C3 * Sb;
    T[2][1] =   S3 * Sb;
    T[2][2] =   Cb;
    T[2][3] =  -Sb * BASE_ARM_LENGTH;

    T[3][0] =  0;
    T[3][1] =  0;
    T[3][2] =  0;
    T[3][3] =  1;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * InitTelescopeToBaseTrans
 *
 * INVOCATION:
 * InitTelescopeToBaseTrans (matrix)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (<) matrix (MATRIX)    telescope to base transformation matrix
 *
 * FUNCTION VALUE:
 * None
 *
 * PURPOSE:
 * Constructs the transformation matrix which transforms coordinates in the
 * telescope frame of reference into coordinates in base arm reference frame.
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

static void InitTelescopeToBaseTrans
(
    MATRIX      T               /* resulting matrix                     */
)
{
    double    C1;      /* Cosine of BETA_TB                               */
    double    S1;      /* Sine of BETA_TB                                 */
    double    C2;      /* Cosine of GAMMA_TB                              */
    double    S2;      /* Sine of GAMMA_TB                                */

    /*
     *  Initialize the trig values.
     */

    C1 = cos(BETA_TB);
    S1 = sin(BETA_TB);
    C2 = cos(GAMMA_TB);
    S2 = sin(GAMMA_TB);


    /*
     *  Now initialize the matrix.
     */

    T[0][0] =  C1 * C2;
    T[0][1] = -C1 * S2;
    T[0][2] =  S1;
    T[0][3] =  T_X;

    T[1][0] =  S2;
    T[1][1] =  C2;
    T[1][2] =   0;
    T[1][3] =  T_Y;

    T[2][0] =  -S1 * C2;
    T[2][1] =   S1 * S2;
    T[2][2] =   C1;
    T[2][3] =   T_Z;

    T[3][0] =  0;
    T[3][1] =  0;
    T[3][2] =  0;
    T[3][3] =  1;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * PickBestOrientation
 *
 * INVOCATION:
 * PickBestOrientation (orientation1, orientation2);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) orientation1 (GM_OIWFS_RAD)  Probe angle for solution 1.
 * (>) orientation2 (GM_OIWFS_RAD)  Probe angle for solution 2.
 *
 * FUNCTION VALUE:
 * (int) Returns 1 if solution 1 provides the "best" orientation, otherwise
 *       returns 2.
 *
 * PURPOSE:
 * Determines which of the solutions obstructs the least mask area
 *
 * DESCRIPTION:
 * Computes the distance of each angle from 180 degrees.  The solution which
 * has an probe orientation closest to 180 degrees will obstruct the least
 * amount of mask area and is considered the "best" solution.
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

static int PickBestOrientation
(
  double    orient1,   /* (in)  Orientation of probe for solution 1.  */
  double    orient2    /* (in)  Orientation of probe for solution 2.  */
)
{
  /*
   *  For each of the angles, compute the distance from the horizontal.
   */

  orient1 = fabs(M_PI - fabs(orient1));
  orient2 = fabs(M_PI - fabs(orient2));


  /*
   *  Return 1 if the orient1 angle is closest to the horizontal,
   *  otherwise return 2.
   */

  if (orient1 < orient2)
  {
    return 1;
  }
  else
  {
    return 2;
  }
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * sign
 *
 * INVOCATION:
 * sgn = sign(x)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) x (int) Value of which sign is to be determined.
 *
 * FUNCTION VALUE:
 * (double) Returns the sign of x.
 *
 * PURPOSE:
 * Determine the sign of the given integer
 *
 * DESCRIPTION:
 * Return the sign of the given integer.
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

static double sign
(
    double x
)
{

    /*
     *  Determine the sign of the given integer
     */

    if (x < 0.0)
    {
        return -1.0;
    }
    else if (x > 0.0)
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * solutionIndex
 *
 * INVOCATION:
 * ndx = solutionIndex(pickoffAngle)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pickoff (GM_OIWFS_RAD) Pickoff angle in radians.
 *
 * FUNCTION VALUE:
 * (int)  1 (or 2) if pickoff corresponds to solution 1 (or 2).
 *
 * PURPOSE:
 * Return index of the solution corresponding to given pickoff angle
 *
 * DESCRIPTION:
 * Compares angle to pickoffOffset to determine if the pickoff arm
 * is in solution 1 or solution 2.  Returns 0 if angle corresponds
 * to a solution where the pickoff and base are fully extended.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * - N/A
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

int solutionIndex
(
    GM_OIWFS_RAD   pickoff     /* (in)  comment  */
)
{
    if (pickoff > pickoffOffset)
    {
        return 1;
    } else if (pickoff < pickoffOffset)
    {
        return 2;
    }
    else 
    {
        return 0;
    }
}

/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * PrintMatrix
 *
 * INVOCATION:
 * PRINT_MATRIX("Pickoff -> Probe", A);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) s (char *)  String to be printed along with matrix.
 * (>) A (MATRIX)  Matrix to be printed.
 *
 * FUNCTION VALUE:
 * (void) No return value.
 *
 * PURPOSE:
 * Prints the name and values of a 4x4 matrix
 *
 * DESCRIPTION:
 * Prints the matrix name (or some other programmer specified string)
 * and the values in the matrix.  This function is only compiled if the
 * DEBUG-MATRIX keyword is defined and should only be invoked
 * using the PRINT_MATRIX macro.
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


#ifdef DEBUG_MATRIX
static void PrintMatrix
(
    char  * s,              /* String to be printed along with matrix       */
    MATRIX A                /* Matrix to be printed                         */
)
{
    int r;                  /* row counter                                  */
    int c;                  /* column counter                               */

    /*
     * Print the prefix string
     */

    printf("\n\n%s\n", s);


    /*
     * Followed by the contents of the matrix
     */

    for (r=0; r < 4; ++r)
    {
        for (c=0; c < 4; ++c)
        {
            printf("%12.6f", A[r][c]);
        }
        printf("\n");
    }
}
#endif


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * MatVecMult4
 *
 * INVOCATION:
 * MatVectMult4 (matrix, vector)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) matrix (MATRIX)      Multiplication matrix
 * (!) vector (VECTOR)      Input/output vector
 *
 * FUNCTION VALUE:
 * none
 *
 * PURPOSE:
 * Vector matrix multiplication function
 *
 * DESCRIPTION:
 * Multiply the given vector by the given matrix.  Vector is overwritten
 * by the result.
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

static void MatVecMult4
(
    MATRIX  T,              /* Multiplication matrix                    */ 
    VECTOR  X               /* Input/output vector                      */
)
{
    double  sum;            /* summing location                         */
    VECTOR  b;              /* resulting vector                         */
    int     r;              /* Row counter                              */
    int     c;              /* Column counter                           */


    /*
     * Perform the vector multiplication
     */

    for (r=0; r < 4; ++r)
    {
        sum = 0.0;

        for (c=0; c < 4; ++c)
        {
            sum = sum + T[r][c] * X[c];
        }

        b[r] = sum;
    }


    /*
     *  Now copy result vector back into input vector.
     */

    for (r=0; r < 4; ++r)
    {
        X[r] = b[r];
    }
}

/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) 1998.                        (c) 1998
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
 * gmOiwfsCalc.h
 *
 * PURPOSE:
 *
 * Provides function prototypes for gmOiwfsCalculateProbeAngles,
 * gmOiwfsCalculateProbePosition and gmOiwfsCalculateVerticalRange.
 *
 * Provides access to global which control the behaviour of the
 * gmOiwfsCalculate* functions.
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.2  2001/05/09 19:17:02  smb
 * New focus settings: Malcolm Smith added new data structures required for ideal focal surface and AtmDC compensation.
 *
 * Revision 1.4  2000/10/16 23:19:35  gmos
 * Added some definitions that were previously in gmOiwfsCalc.c
 * gmOiwfsGenSubs.c also uses this header file now.
 *
 * Revision 1.3  2000/05/30 16:29:10  gmos
 * New version commented by William Rambold. Master enable bug fixed.
 *
 * Revision 1.2  2000/04/15 11:38:55  gmos
 * Angelics updated OIWFS software
 *
 * Revision 1.3  2000/04/13 08:25:50  gmos
 * Corrections released by Angelic on 13 April 2000
 *
 * Revision 1.3  2000/04/11 22:34:37  msmith
 * Added probeAngle parameter to  gmOiwfsCalculateProbePosition.
 *
 * Revision 1.2  2000/03/21 23:24:36  angelic
 * added GM_OIWFS_MIDPOINT status defines
 *
 * Revision 1.1  2000/01/28 23:31:17  dunn
 * Initial revision
 *
 *INDENT-ON*
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/

#ifndef  GM_OIWFS_CALC
#define  GM_OIWFS_CALC


/*
 * The following definition of PI is required since some <math.h> header
 * files do not appear to define it.  This definition MUST APPEAR AFTER
 * the inclusion of <math.h>.
 */

#ifndef  M_PI
#define  M_PI  3.1415926535897932384626         /* Definition of PI         */
#endif

#define  GM_OIWFS_MIDPOINT_NONE        0        /* Midpoint calc hit limits */
#define  GM_OIWFS_MIDPOINT_OK          1        /* Midpoint calc successful */
#define  GM_OIWFS_MIDPOINT_EMERGENCY  -1        /* Midpoint set to midrange */


 /*
  *  Offsets to base stage origin {B} from telescope origin {T} (in mm).
  */
#define  T_X                -265.30     /* Away from cassette in X          */
#define  T_Y                 -17.58     /* Back in Y behind slit mask       */
#define  T_Z                  63.20     /* Up in Z                          */

 /*
  *  X & Y offsets to pickoff stage origin {P} from base stage 
  *  origin {B} (in mm).  Z offset is zero.
  */
#define  BASE_ARM_LENGTH      77.50     /* Bx = Length of base stage arm    */
#define  BASE_ARM_HEIGHT      29.62     /* By = Height of base stage arm    */

 /*
  *  X & Y offsets to probe mirror origin {M} from pickoff stage 
  *  origin {P} (in mm).  Z offset is zero.
  */
#define  PICKOFF_ARM_LENGTH  213.03     /* Px = Length of pickoff stage arm */
#define  PICKOFF_ARM_HEIGHT   64.03     /* Py = Height of pickoff stage arm */


 /*
  *  Offsets to base stage origin {B} from slit mask coordinate system
  *  origin {MC}.  The origin of {MC} is at the same point as the origin
  *  of {T} but rotated 180 degrees about Y and 180 degrees about X. 
  */
#define  BASE_ORIGIN_X       (-T_X)  /* {MC} X goes in opposite direction   */
#define  BASE_ORIGIN_Y         T_Z   /* {MC} Y is actually {T} Z            */
#define  BASE_ORIGIN_Z         T_Y   /* {MC} Z is focus which is {T} Y      */



typedef  double  GM_OIWFS_RAD;                  /* Radians are double words */             
typedef  double  GM_OIWFS_MM;                   /* Millimeters are doubles  */

typedef  struct
{
    double offset;
    double xSlope;
    double ySlope;
    double tanZen2;
    double xyQuad;
    double xxQuad;
    double yyQuad;
    double xCos2zen;
} GM_OIWFS_REG_COEF;

extern int gmOiwfsSolutionId;                   /* select probe orientation */
extern int gmOiwfsStageTiltCorrection;          /* Include tilt in calcs    */
extern int gmOiwfsRefineSteps;                  /* Angle calc iterations    */

extern int gmAtmdcInstalled;                    /* Is an ADC installed?     */

void gmOiwfsSetOffsets(GM_OIWFS_RAD base, GM_OIWFS_RAD pickoff, 
                       GM_OIWFS_MM x, GM_OIWFS_MM y, GM_OIWFS_MM z);

int  gmOiwfsGetOffsets(GM_OIWFS_RAD * base, GM_OIWFS_RAD * pickoff, 
                       GM_OIWFS_MM * x, GM_OIWFS_MM * y, GM_OIWFS_MM * z);

int gmOiwfsCalculateProbePosition (GM_OIWFS_RAD baseAngle, 
                                   GM_OIWFS_RAD pickoffAngle,
                                   GM_OIWFS_MM  * xPosition,
                                   GM_OIWFS_MM  * yPosition,
                                   GM_OIWFS_MM  * zPosition,
                                   GM_OIWFS_RAD * probeAngle);

int gmOiwfsCalculateProbeAngles (GM_OIWFS_MM xTarget, 
                                 GM_OIWFS_MM yTarget, 
                                 int solution,
                                 GM_OIWFS_RAD * baseAngle, 
                                 GM_OIWFS_RAD * pickoffAngle,
                                 GM_OIWFS_RAD * probeAngle);

int gmOiwfsCalculateVerticalRange(GM_OIWFS_RAD base1, 
                                  GM_OIWFS_RAD pickoff1, 
                                  GM_OIWFS_RAD base2, 
                                  GM_OIWFS_RAD pickoff2,
                                  GM_OIWFS_MM lowerLimit, 
                                  GM_OIWFS_MM upperLimit,
                                  GM_OIWFS_MM * ymin, 
                                  GM_OIWFS_MM * ymax,
                                  GM_OIWFS_RAD * mid_base, 
                                  GM_OIWFS_RAD * mid_pickoff);

int solutionIndex(GM_OIWFS_RAD pickoff);

#endif /*  GM_OIWFS_CALC  */

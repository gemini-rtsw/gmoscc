static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id$"};

/* *INDENT-OFF* */
/*
 *   FILENAME
 *   gmSeqTracking
 *
 *   FUNCTION NAME(S)
 *   gmSeqAtmDemands    - fetch TCS context and calculate angles of prisms for atmDc
 *   gmSeqCadAtmTrack   - implement the atmTrack command
 *   gmSeqCadDtaTrack   - implement the dtaTrack command
 *   gmSeqDtaDemands    - compute detector translation demands from flexure and thermal models
 *   gmSeqDtaFlexCoeffs - fetch coefficients of the detector translation flexure model
 *   gmSeqDtaTempCoeffs - fetch coefficients of the detector translation thermal model
 *   gmSeqGetRmaZ       - fetch rotator mechanical angle and zenith distance from TCS
 *
 *   Copyright Observatory Sciences Ltd. 1999. All rights reserved
 *   Under contract to the UK Astronomy Technology Centre, who modified the code
 *   in 2001.
 *
 */
/*
 * $Log$
 * Revision 1.3  2001/11/28 19:35:26  mbec
 * *** empty log message ***
 *
 * Revision 1.2  2001/08/08 01:06:12  gemvx
 * Version after commissioning. See RELEASE.NOTES
 *
 * Revision 1.4  2001/07/10 13:45:22  gmos
 * V2-6 imported from Gemini.
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.9  2001/03/01 14:10:13  gmos
 * Use (char *) explicitly for CAD string fields.
 *
 * Revision 1.8  2001/02/28 12:46:20  gmos
 * Allow AtmDC and DTA movement while detector is reading out. Extra trap for master enable added.
 *
 * Revision 1.7  2001/02/28 10:37:50  gmos
 * A couple of comments changed. Default wavelength range for AtmDC changed from 400-800nm to 400-900nm, since upper limit of wavelength has been increased.
 *
 * Revision 1.6  2001/02/22 16:11:06  gmos
 * Reworked grating anamorphic factor correction again. This time Hy and Hz are dilution factors for the flexure correction aplied to Y as a result of thetaY and thetaZ. There is no focus correction.
 *
 * Revision 1.5  2001/02/22 14:18:28  gmos
 * Corrected degrees to radians conversion bug in anamorphic correction.
 *
 * Revision 1.4  2001/02/22 10:51:56  gmos
 * Corrected equation for alpha in grating anamorphism correction.
 *
 * Revision 1.3  2001/02/21 13:55:19  gmos
 * Switched flexure equation for instrument X from a sum of two sine waves to the product of two sine waves.
 *
 * Revision 1.2  2001/02/20 18:02:43  gmos
 * Reversed sign of flexure coefficients. Corrected mistake in anamorphic factor equation.
 *
 * Revision 1.1  2001/02/20 11:52:40  gmos
 * Renamed from gmSeqAtm.c to gmSeqTracking.c. Major changes to use GMOS coordinates as intermediate. Flexure model changed to the one that best fits the actual GMOS flexure measurements.
 *
 * Revision 1.19  2001/02/09 17:12:22  gmos
 * Added dummy gmSeqAtmInit function.
 *
 * Revision 1.18  2001/02/05 13:07:40  gmos
 * Code prologue tidied up.
 *
 * Revision 1.17  2001/02/01 11:03:42  gmos
 * Major change to the way that filter and grating offsets are handled. Filters now have surface power and effective wavelength offsets. Gratings now have a surface power offset in addition to wavelength offset.
 *
 * Revision 1.16  2001/01/30 11:50:18  gmos
 * Replaced dangerous strcpy statement with strncpy. strncpy sizes changed from MAX_STRING_SIZE to MAX_STRING_SIZE-1.
 *
 * Revision 1.15  2001/01/23 11:57:53  gmos
 * Attributes of dtaTrack CAD are now offsets from the centre. EFG attributes now contain the centre position.
 *
 * Revision 1.14  2001/01/23 09:41:18  gmos
 * dtaTrack CAD will reject when assembly not indexed. gmSeqDtaParams genSub will report error when demands go out of range.
 *
 * Revision 1.13  2000/09/22 08:21:48  gmos
 * Parameters used to define a universal wavelength range of 300 to 1000 nanometres.
 *
 * Revision 1.12  2000/09/15 08:48:29  gmos
 * Multiplied constants in gmSeqAtmDemands by 1000 to convert them from microns to nanometres. Chris Mayer needs to check this.
 *
 * Revision 1.11  2000/09/13 11:12:47  pbt
 * Units are now nanometres for wavelengths and microns for translator position
 *
 * Revision 1.10  2000/09/12 16:20:48  pbt
 * Modified gmSeqDtaCoeffs for input of wavelength dependent focus offset
 *
 * Revision 1.9  2000/02/17 15:48:26  pbt
 * Add READNG_OUT interlocks
 *
 * Revision 1.8  2000/02/17 14:55:27  pbt
 * Enhanced/changed interlock checking
 *
 * Revision 1.7  1999/12/22 15:10:48  pbt
 * Use DTA and atmDC limit symbols throughout
 *
 * Revision 1.6  1999/12/17 16:59:28  cjm
 * Add routine to access translation model coefficients
 *
 * Revision 1.5  1999/12/15 12:48:15  cjm
 * Add routine DtaDemands to compute flexure and temperature model
 *
 * Revision 1.4  1999/12/13 16:21:19  cjm
 * Add CadDtaTrack subroutine
 *
 * Revision 1.3  1999/12/10 12:36:21  cjm
 * Change gmSeqCADatmTrack to gmSeqCadAtmTrack to fit naming scheme
 *
 * Revision 1.2  1999/12/07 17:18:49  cjm
 * Fix refraction formula and guard against potentila divide by zero
 *
 * Revision 1.1  1999/11/24 15:24:39  pbt
 * Atmospheric Dispersion Corrector C routines, modified from original by CJM
 *
 */
/* *INDENT-ON* */

#include <vxWorks.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <lstLib.h>
#include <semLib.h>
#include <logLib.h>

#include <dbDefs.h>
#include <alarm.h>
#include <cadRecord.h>
#include <cad.h>
#include <carRecord.h>
#include <genSubRecord.h>

#include <slamac.h>    /* Contains DD2R and DR2D */
#include <slalib.h>
#include <astLib.h>

#include "gmSeq.h"

/* Local global variables containing coefficients of the detector translation model. */

static double gmSeqDtaX0 ;    /* X zero point of models (microns)                    */
static double gmSeqDtaY0 ;    /* Y zero point of models (microns)                    */
static double gmSeqDtaZ0 ;    /* Z (focus) zero point of models (microns)            */
static double gmSeqDtaAx ;    /* First X flexure coefficient (microns)               */
static double gmSeqDtaAy ;    /* First Y flexure coefficient (microns)               */
static double gmSeqDtaAz ;    /* First Z (focus) flexure coefficient (microns)       */
static double gmSeqDtaBx ;    /* Second X flexure coefficient (microns)              */
static double gmSeqDtaBy ;    /* Second Y flexure coefficient (microns)              */
static double gmSeqDtaBz ;    /* Second Z (focus) flexure coefficient (microns)      */
static double gmSeqGrHthetay ; /* thetaY dilution factor for anamorphic correction   */
static double gmSeqGrHthetaz ; /* thetaZ dilution factor for  anamorphic correction  */
static double gmSeqGrRefAng ; /* Grating reference angle, psi (radians).             */
static double gmSeqDtaPhiYX ; /* X flexure phase offset for thetaY angle (radians)   */
static double gmSeqDtaPhiZX ; /* X flexure phase offset for thetaZ angle (radians)   */
static double gmSeqDtaPhiYY ; /* Y flexure phase offset for thetaY angle (radians)   */
static double gmSeqDtaPhiZY ; /* Y flexure phase offset for thetaZ angle (radians)   */
static double gmSeqDtaPhiYZ ; /* Z flexure phase offset for thetaY angle (radians)   */
static double gmSeqDtaPhiZZ ; /* Z flexure phase offset for thetaZ angle (radians)   */

static double gmSeqDtaTx ;    /* Linear X temperature coefficient (microns/degC)     */
static double gmSeqDtaTy ;    /* Linear Y temperature coefficient (microns/degC)     */
static double gmSeqDtaTz ;    /* Linear Z temperature coefficient (microns/degC)     */
static double gmSeqDtaT2x ;   /* Quadratic X temperature coeff. (microns/degC/degC)  */
static double gmSeqDtaT2y ;   /* Quadratic Y temperature coeff. (microns/degC/degC)  */
static double gmSeqDtaT2z ;   /* Quadratic Z temperature coeff. (microns/degC/degC)  */

/* Local global variables containing the TCS telescope context.  */

static double gmSeqZenDist ;     /* Zenith distance (radians)                  */
static double gmSeqRotMechAng ;  /* Rotator mechanical angle (radians)         */
static int    gmSeqTcsStatus ;   /* Flag to show TCS status (1=present,        */
                                 /* 0=absent, -1=simulated, -2=flexure tests)  */
static SEM_ID gmSeqTcsCtxSem = NULL;    /* Semaphore protecting TCS context.   */

/* Local global variables containing GMOS information. */

static long gmSeqIssPort = 0 ;           /* ISS port on which GMOS is installed.       */
static double gmSeqThetaY, gmSeqThetaZ ; /* Orientation in GMOS coordinates (radians). */
static SEM_ID gmSeqGmosSem = NULL;       /* Semaphore protecting GMOS coordinates.     */

/* Forward function declarations */

void gmSeqGetRmaZ (struct genSubRecord *pgsub, double *rma, double *zd) ;
void gmSeqGetRefParams (struct genSubRecord *pgsub, double *tempK,
                        double *press, double *relH, double *refWl,
                        double *refa, double *refb) ;


/*+
 *   Function name:
 *   gmSeqCadAtmTrack 
 *
 *   Purpose:
 *   Implements the atmTrack command in the GMOS sequencer
 *
 *   Description:
 *   The routine first reads in the mode that the user wants the atmDC to 
 *   operate in. If the mode is MOVE then inputs B and C are read and 
 *   validated. If the mode is FOLLOW then inputs D and E are used. If the
 *   mode is STOP then all other inputs are ignored. Once the inputs have
 *   been validated, they are copied to the outputs for use by down stream
 *   processes.
 *   It is assumed that any interlocks are handled at a higher level
 *
 *   Invocation:
 *   gmSeqCadAtmTrack (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to cad record structure
 *
 *   EPICS input parameters:
 *      a => Tracking mode, MOVE, FOLLOW or STOP
 *      b => Entrance prism angle (degrees)
 *      c => Exit prism angle (degrees)
 *      d => Minimum wavelength (nanometres)
 *      e => Maximum wavelength (nanometres)
 *      f => Indexed flag (0 or 1)
 *
 *   EPICS output parameters:
 *      vala => Demanded tracking mode
 *      valb => Demanded entrance prism angle (degrees)
 *      valc => Demanded exit prism angle (degrees)
 *      vald => Demanded minimum wavelength (nanometres)
 *      vale => Demanded maximum wavelength (nanometres)
 *      valf => Index for track mode 
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   Deficiencies:
 *   None known
 *
 *-
 */

long gmSeqCadAtmTrack (struct cadRecord *pcad)

{

  long status ;           /* Return status */
  char *trackModes[] = {"MOVE", "STOP", "FOLLOW", NULL} ;
  char trackModeIn[MAX_STRING_SIZE];    /* Input tracking mode, as a string */
  char enpAngleString[MAX_STRING_SIZE]; /* Entrance angle as a string */
  char expAngleString[MAX_STRING_SIZE]; /* Exit angle as a string */
  char minWaveString[MAX_STRING_SIZE];  /* Min. wavelength as a string */
  char maxWaveString[MAX_STRING_SIZE];  /* Max. wavelength as a string */
  static int mode ;                     /* Index to demanded track mode */
  static double theta1 ;                /* requested angle for prism 1 */
  static double theta2 ;                /* Requested angle for prism 2 */
  static double lambdaMin ;             /* Minimum wavelength for track mode */
  static double lambdaMax ;             /* Maximum wavelength for track mode */
  static int indexed ;                  /* Has the assembly been indexed? (0 or 1) */  

  status = CAD_ACCEPT ;

  switch (pcad->dir) 
  {

   case menuDirectivePRESET :

    status = CAD_REJECT ;
    
/* Read in tracking mode, upper-case and verify */
    strncpy(trackModeIn, (char *)pcad->a, MAX_STRING_SIZE-1);
    gmSeqUc(trackModeIn, strlen(trackModeIn) + 1, trackModeIn);
    if ((mode = gmSeqDcString(trackModes, trackModeIn)) < 0 ) 
    {
      strncpy(pcad->mess, "atmTrack: mode not recognised", MAX_STRING_SIZE-1);
      break ;
    }
    mode++ ;        /* Increment: index starts from zero, mode from 1 */

/* The assembly must be indexed for any mode except STOP */

    if ( mode != 2 ) {
     if (pcad->f[0] ) {
      if (sscanf((char *)pcad->f, "%d", &indexed) != 1) {
       strncpy(pcad->mess, "atmTrack: Illegal INDEXED flag",
               MAX_STRING_SIZE-1);
       break; 
      }
      if (indexed == 0) {
        strncpy (pcad->mess, "atmTrack: Assembly not indexed", 
                 MAX_STRING_SIZE-1) ;
        break ;
      }
     }
    }

/* No tracking possible without master enable */
    gmSeqSetCADTest(MASTER_ENABLE);

/* Handle move mode first */
    if (mode == 1) { 

/* MOVE mode not allowed during observing */
    gmSeqSetCADTest(OBSERVING);

/* Is there an angle specified and if so is it within range */
     strncpy(enpAngleString, (char *)pcad->b, MAX_STRING_SIZE-1);
     if (sscanf(enpAngleString, "%lf", &theta1) != 1)
     {
       strncpy(pcad->mess, "atmTrack: Illegal entrance prism angle (B)",
               MAX_STRING_SIZE-1);
       break; 
     }

     if (theta1 < ATMENPMIN || theta1 > ATMENPMAX) 
     {
       strncpy(pcad->mess, "atmTrack: Entr angle > 360 or < -360", MAX_STRING_SIZE-1) ;
       break ;
     }

/* Now repeat for the exit prism angle */

     strncpy(expAngleString, (char *)pcad->c, MAX_STRING_SIZE-1);
     if (sscanf(expAngleString, "%lf", &theta2) != 1)
     {
       strncpy(pcad->mess, "atmTrack: Illegal exit prism angle (C)",
               MAX_STRING_SIZE-1);
       break; 
     }

     if (theta2 < ATMEXPMIN || theta2 > ATMEXPMAX) 
     {
       strncpy(pcad->mess, "atmTrack: Exit angle > 360 or < -360", MAX_STRING_SIZE-1) ;
       break ;
     }
    }

/* Now deal with FOLLOW mode */
    if (mode == 3) {
/* Is there a wavelength specified and if so is it within range */

     strncpy(minWaveString, (char *)pcad->d, MAX_STRING_SIZE-1);
     if (sscanf(minWaveString, "%lf", &lambdaMin) != 1)
     {
       strncpy(pcad->mess, "atmTrack: Illegal min. wavelength (D)",
               MAX_STRING_SIZE-1);
       break; 
     }

     if (lambdaMin < MIN_WAVELENGTH  || lambdaMin > MAX_WAVELENGTH) 
     {
       strncpy(pcad->mess, "atmTrack: Min. wavelength out-of-range", MAX_STRING_SIZE-1) ;
       break ;
     }

/* Now repeat for maximum wavelength */

     strncpy(maxWaveString, (char *)pcad->e, MAX_STRING_SIZE-1);
     if (sscanf(maxWaveString, "%lf", &lambdaMax) != 1)
     {
       strncpy(pcad->mess, "atmTrack: Illegal max. wavelength (E)",
               MAX_STRING_SIZE-1);
       break; 
     }

     if (lambdaMax < MIN_WAVELENGTH  || lambdaMax > MAX_WAVELENGTH) 
     {
       strncpy(pcad->mess, "atmTrack: Max. wavelength out-of-range", MAX_STRING_SIZE-1) ;
       break ;
     }


/* Check that minimum is less than maximum */
     if (lambdaMin > lambdaMax) {
       strncpy(pcad->mess, "atmTrack: Min > Max wavelength", MAX_STRING_SIZE-1) ;
       break ;
     }

    }

    status = CAD_ACCEPT ;
   break ;

   case menuDirectiveSTART :

/* Place data on output links */
    strncpy ((char *)pcad->vala, (char *)pcad->a, MAX_STRING_SIZE-1) ;
    *(double *)pcad->valb = theta1 ;
    *(double *)pcad->valc = theta2 ;
    *(double *)pcad->vald = lambdaMin ;
    *(double *)pcad->vale = lambdaMax ;
    *(long *)pcad->valf = mode ;
   break ;

   case menuDirectiveMARK :
   break ;

   case menuDirectiveSTOP :
   break ;

   case menuDirectiveCLEAR :
   break ;

  }

  return status ;
}


/*+
 *   Function name:
 *   gmSeqCadDtaTrack 
 *
 *   Purpose:
 *   Implements the dtaTrack command in the GMOS sequencer
 *
 *   Description:
 *   The routine first reads the mode that the user wants the detector
 *   tracking assembly to operate in. Possible modes are MOVE, FOLLOW,
 *   FOLLOW-XY or STOP. If the mode is not STOP then the input parameters
 *   Dx, Dy and Dz are read. If no parameter is given then a value of
 *   0.0 is assumed. The offset parameters are checked for validity and
 *   if everything is Ok are written along with the mode to the output links.
 *   It is assumed that any interlocks i.e. any disabling of the command,
 *   are handled at a higher level
 *
 *   Invocation:
 *   gmSeqCadDtaTrack (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to cad record structure
 *
 *   EPICS input parameters:
 *      a => Tracking mode, MOVE, FOLLOW, FOLLOW-XY or STOP
 *      b => X translation offset (microns) 
 *      c => Y translation offset (microns) 
 *      d => Z translation offset (microns) 
 *      e => X translation centre (microns) 
 *      f => Y translation centre (microns) 
 *      g => Z translation nominal best focus (microns) 
 *      h => Indexed flag (0 or 1)
 *
 *   EPICS output parameters:
 *      vala => Demanded tracking mode
 *      valb => Demanded X position (microns)
 *      valc => Demanded Y position (microns)
 *      vald => Demanded Z position (microns)
 *      vale => Index for track mode 
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   Deficiencies:
 *   None known
 *
 *-
 */

long gmSeqCadDtaTrack (struct cadRecord *pcad)

{

  long status ;           /* Return status */
  char *trackModes[] = {"STOP", "MOVE", "FOLLOW", "FOLLOW-XY", NULL} ;
  char trackModeIn[MAX_STRING_SIZE]; /* Input tracking mode, as a string */
  double dx ;                        /* X offset (microns) */
  double dy ;                        /* Y offset (microns) */
  double dz ;                        /* Z offset (microns) */
  double xcen ;                      /* X centre (microns) */
  double ycen ;                      /* Y centre (microns) */
  double zcen ;                      /* Z centre (microns) */
  int indexed ;                      /* Has the assembly been indexed? (0 or 1) */

  static double xpos;                /* X position (microns) */
  static double ypos;                /* Y position (microns) */
  static double zpos;                /* Z position (microns) */
  static int mode ;                  /* Index to demanded track mode */

  status = CAD_ACCEPT ;

  switch (pcad->dir) 
  {
     case menuDirectivePRESET :

        status = CAD_REJECT ;

/* Read in tracking mode, upper-case and verify */

        strncpy(trackModeIn, (char *)pcad->a, MAX_STRING_SIZE-1);
        gmSeqUc(trackModeIn, strlen(trackModeIn) + 1, trackModeIn);

        if ((mode = gmSeqDcString(trackModes, trackModeIn)) < 0 ) 
        {
           strncpy(pcad->mess, "dtaTrack: mode not recognised", MAX_STRING_SIZE-1);
           break ;
        }

/* No tracking possible without master enable */
        gmSeqSetCADTest(MASTER_ENABLE);

/* MOVE mode not allowed during observing */

        if (mode == TRACKMOVE) gmSeqSetCADTest(OBSERVING);

/* If mode is not equal to STOP then start checking the offsets */

        if (mode != TRACKSTOP)
        { 

/* The assembly must be indexed */

           if (pcad->h[0] )
           {

              if (sscanf((char *)pcad->h, "%d", &indexed) != 1)
              {
                 strncpy(pcad->mess, "dtaTrack: Illegal INDEXED flag",
                         MAX_STRING_SIZE-1);
                 break; 
              }
              if (indexed == 0)
              {
                 strncpy (pcad->mess, "dtaTrack: Assembly not indexed", 
                          MAX_STRING_SIZE-1) ;
                 break ;
              }
           }

/* Is there an X offset specified and if so is it within range */

           if ( pcad->b[0] )
           {
              if (sscanf((char *)pcad->b, "%lf", &dx) != 1)
              {
                 strncpy(pcad->mess, "dtaTrack: Illegal X offset",
                         MAX_STRING_SIZE-1);
                 break; 
              }
           }
           else
           {
              dx = 0.0;
           }

           if ( pcad->e[0] )
           {
              if (sscanf((char *)pcad->e, "%lf", &xcen) != 1)
              {
                 strncpy(pcad->mess, "dtaTrack: Illegal X centre",
                         MAX_STRING_SIZE-1);
                 break; 
              }
           }
           else
           {
              xcen = 0.0;
           }

           xpos = xcen + dx;

           if (xpos < DTAXMIN || xpos > DTAXMAX)
           {
              strncpy (pcad->mess, "dtaTrack: X offset out of range", 
                       MAX_STRING_SIZE-1) ;
              break ;
           }

/* Is there an Y offset specified and if so is it within range */

           if ( pcad->c[0] )
           {
              if (sscanf((char *)pcad->c, "%lf", &dy) != 1)
              {
                 strncpy(pcad->mess, "dtaTrack: Illegal Y offset",
                         MAX_STRING_SIZE-1);
                 break; 
              }
           }
           else
           {
              dy = 0.0;
           }

           if ( pcad->f[0] )
           {
              if (sscanf((char *)pcad->f, "%lf", &ycen) != 1)
              {
                 strncpy(pcad->mess, "dtaTrack: Illegal Y centre",
                         MAX_STRING_SIZE-1);
                 break; 
              }
           }
           else
           {
              ycen = 0.0;
           }

           ypos = ycen + dy;

           if (ypos < DTAYMIN || ypos > DTAYMAX)
           {
              strncpy (pcad->mess, "dtaTrack: Y offset out of range", 
                       MAX_STRING_SIZE-1) ;
              break ;
           }

/* Is there an Z offset specified and if so is it within range */

           if ( pcad->d[0] )
           {
              if (sscanf((char *)pcad->d, "%lf", &dz) != 1)
              {
                 strncpy(pcad->mess, "dtaTrack: Illegal Z offset",
                         MAX_STRING_SIZE-1);
                 break; 
              }
           }
           else
           {
              dz = 0.0;
           }

           if ( pcad->g[0] )
           {
              if (sscanf((char *)pcad->g, "%lf", &zcen) != 1)
              {
                 strncpy(pcad->mess, "dtaTrack: Illegal Z centre",
                         MAX_STRING_SIZE-1);
                 break; 
              }
           }
           else
           {
              zcen = 0.0;
           }

           zpos = zcen + dz;

           if (zpos < DTAZMIN || zpos > DTAZMAX)
           {
              strncpy (pcad->mess, "dtaTrack: Z offset out of range", 
                       MAX_STRING_SIZE-1) ;
              break ;
           }
        }

        status = CAD_ACCEPT ;
        break ;

     case menuDirectiveSTART :

/* Increment the mode index before passing it to EPICS */

        mode++ ;

/* Place data on output links */

       strncpy ((char *)pcad->vala, (char *)pcad->a, MAX_STRING_SIZE-1) ;
       *(double *)pcad->valb = xpos ;
       *(double *)pcad->valc = ypos ;
       *(double *)pcad->vald = zpos ;
       *(long *)pcad->vale   = mode ;
       break ;

    case menuDirectiveMARK :
       break ;

    case menuDirectiveSTOP :

/* On receipt of a STOP directive, set the mode to STOP */

       strcpy ((char *)pcad->vala, "STOP") ;
       mode = 1;
       *(long *)pcad->vale   = mode ;

       break ;

    case menuDirectiveCLEAR :
       break ;

  }

  return status ;
}


/*+
 *   Function name:
 *   gmSeqDtaFlexCoeffs
 *
 *   Purpose:
 *   Pull in flexure coefficients of detector translation model
 *
 *   Description:
 *   This routine simply pulls in the coefficients specified by the user
 *   for how to adjust the detector translation stage as a function of
 *   temperature, orientation etc. It exists as a separate routine from
 *   gmSeqDtaDemands simply because there are insufficient inputs on a
 *   gensub record to handle all the coefficients. Further gensubs may
 *   be added to the chain if even more coefficients are needed.
 *
 *   Note:
 *   Angles are exchanged with the outside world in degrees but are stored
 *   internally in radians.
 *
 *   Invocation:
 *   gmSeqDtaFlexCoeffs(pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)    pgsub    (genSubRecord *) Pointer to gensub structure
 *
 *   Function value:
 *   (<)  status  (long)  Return status, 0 = OK
 * 
 *
 *   Epics inputs:
 *   
 *   a => X axis zero point, X0 (microns)
 *   b => Y axis zero point, Y0 (microns)
 *   c => Z axis zero point, Z0 (microns)
 *   d => First X flexure coefficient, Ax (microns)
 *   e => First Y flexure coefficient, Ay (microns)
 *   f => First Z flexure coefficient, Az (microns)
 *   g => Second X flexure coefficient, Bx (microns)
 *   h => Second Y axis flexure coefficient, By (microns)
 *   i => Second Z axis flexure coefficient, Bz (microns)
 *   j => Grating amamorphic factor for Y, Hy (no units)
 *   k => Grating amamorphic factor for Z, Hz (no units)
 *   l => Grating reference angle, psi (degrees)
 *   m => X flexure phase offset for thetaY angle (degrees)
 *   n => X flexure phase offset for thetaZ angle (degrees)
 *   o => Y flexure phase offset for thetaY angle (degrees)
 *   p => Y flexure phase offset for thetaZ angle (degrees)
 *   q => Z flexure phase offset for thetaY angle (degrees)
 *   r => Z flexure phase offset for thetaZ angle (degrees)
 *
 *   Epics outputs:
 *   None
 *
 *   Global variables:
 *   The coefficients are stored in global variables.
 *
 *   Assumptions:
 *   The ranges of the values read in are assumed to be checked elsewhere
 *
 *   Deficiencies:
 *   At present theere is no attempt to synchronise the data before it is 
 *   stored for future use. This means that it is possible for the translation
 *   tracking system to use a partially updated model for a short time.
 *
 *-
 */

long gmSeqDtaFlexCoeffs (struct genSubRecord *pgsub)
{

/*
 * Read the flexure coefficients into global variables.
 */
  
   gmSeqDtaX0 = *(double *)pgsub->a ;
   gmSeqDtaY0 = *(double *)pgsub->b ;
   gmSeqDtaZ0 = *(double *)pgsub->c ;

   gmSeqDtaAx = *(double *)pgsub->d ;
   gmSeqDtaAy = *(double *)pgsub->e ;
   gmSeqDtaAz = *(double *)pgsub->f ;

   gmSeqDtaBx = *(double *)pgsub->g ;
   gmSeqDtaBy = *(double *)pgsub->h ;
   gmSeqDtaBz = *(double *)pgsub->i ;

/*
 * Read in the grating anamorphic factors and grating reference angle,
 * converting the latter into radians.
 */

   gmSeqGrHthetay = *(double *)pgsub->j ;
   gmSeqGrHthetaz = *(double *)pgsub->k ;
   gmSeqGrRefAng  = *(double *)pgsub->l * DD2R ;

/*
 * Read the phase offsets in degrees and convert them to radians.
 */

   gmSeqDtaPhiYX = *(double *)pgsub->m * DD2R ;
   gmSeqDtaPhiZX = *(double *)pgsub->n * DD2R ;
   gmSeqDtaPhiYY = *(double *)pgsub->o * DD2R ;
   gmSeqDtaPhiZY = *(double *)pgsub->p * DD2R ;
   gmSeqDtaPhiYZ = *(double *)pgsub->q * DD2R ;
   gmSeqDtaPhiZZ = *(double *)pgsub->r * DD2R ;

   return OK ;
}


/*+
 *   Function name:
 *   gmSeqDtaTempCoeffs
 *
 *   Purpose:
 *   Pull in temperature coefficients of detector translation model
 *
 *   Description:
 *   This routine simply pulls in the coefficients specified by the user
 *   for how to adjust the detector translation stage as a function of
 *   temperature, orientation etc. It exists as a separate routine from
 *   gmSeqDtaDemands simply because there are insufficient inputs on a
 *   gensub record to handle all the coefficients. Further gensubs may
 *   be added to the chain if even more coefficients are needed.
 *
 *   Invocation:
 *   gmSeqDtaTempCoeffs(pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)    pgsub    (genSubRecord *) Pointer to gensub structure
 *
 *   Function value:
 *   (<)  status  (long)  Return status, 0 = OK
 * 
 *
 *   Epics inputs:
 *   
 *   a => X axis linear temperature coefficient (microns/deg C)
 *   b => Y axis linear temperature coefficient (microns/deg C)
 *   c => Z axis linear temperature coefficient (microns/deg C)
 *   d => X axis quadratic temperature coefficient (microns/deg C/deg C)
 *   e => Y axis quadratic temperature coefficient (microns/deg C/deg C)
 *   f => Z axis quadratic temperature coefficient (microns/deg C/deg C)
 *
 *   Epics outputs:
 *   None
 *
 *   Global variables:
 *   The coefficients are stored in global variables.
 *
 *   Assumptions:
 *   The ranges of the values read in are assumed to be checked elsewhere
 *
 *   Deficiencies:
 *   At present theere is no attempt to synchronise the data before it is 
 *   stored for future use. This means that it is possible for the translation
 *   tracking system to use a partially updated model for a short time.
 *
 *-
 */

long gmSeqDtaTempCoeffs (struct genSubRecord *pgsub)
{

/*
 * Read the temperature coefficients into global variables.
 */

   gmSeqDtaTx = *(double *)pgsub->a ;
   gmSeqDtaTy = *(double *)pgsub->b ;
   gmSeqDtaTz = *(double *)pgsub->c ;

   gmSeqDtaT2x = *(double *)pgsub->d ;
   gmSeqDtaT2y = *(double *)pgsub->e ;
   gmSeqDtaT2z = *(double *)pgsub->f ;

   return OK ;
}


/*+
 *   Function name:
 *   gmSeqAtmInit
 *
 *   Purpose:
 *   Initialisation function for gmSeqAtmDemands
 *
 *   Description:
 *
 *   Invocation:
 *   gmSeqAtmInit(pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)    pgsub    (genSubRecord *) Pointer to gensub structure
 *
 *   Function value:
 *   (<)  status  (long)  Return status, 0 = OK; -1 = failure
 * 
 *
 *   Epics inputs:
 *
 *   Epics outputs:
 *
 *   External functions:
 *   Any external functions used in this function or procedure e.g
 *   function1      (library name)     What function does
 *
 *   Deficiencies:
 *   Any known problems with the function
 *
 *-
 */

long gmSeqAtmInit (struct genSubRecord *pgsub)
{

/*
 * Create the semaphore used to prevent the TCS context from being used
 * at the same time it is being updated.
 */

   gmSeqTcsCtxSem = semMCreate ( SEM_Q_FIFO | SEM_DELETE_SAFE );

/*
 * Create the semaphore used to prevent the GMOS coordinates from being used
 * at the same time they are being updated.
 */

   gmSeqGmosSem   = semMCreate ( SEM_Q_FIFO | SEM_DELETE_SAFE );

   return OK;
}


/*+
 *   Function name:
 *   gmSeqAtmDemands
 *
 *   Purpose:
 *   Calculates the angles of the atmDC prisms to compensate for 
 *   atmospheric dispersion
 *
 *   Description:
 *   The routine first pulls in its input data and performs sanity checks
 *   on the minimum and maximum wavelengths specified. Provided a
 *   connection is available to the TCS then the angles of the prisms
 *   needed to compensate for the atmospheric dispersion at this elevation
 *   are calculated.
 *
 *   Note:
 *   Angles are exchanged with the outside world in degrees but are stored
 *   internally in radians.
 *
 *   Invocation:
 *   gmSeqAtmDemands(pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)    pgsub    (genSubRecord *) Pointer to gensub structure
 *
 *   Function value:
 *   (<)  status  (long)  Return status, 0 = OK; -1 = out of range
 * 
 *
 *   Epics inputs:
 *   
 *   a => Instrument alignment angle (degrees)
 *   b => Offset of prisms from Instrument Principal Direction (degrees)
 *   c => Minimum wavelength (nanometres)
 *   d => Maximum wavelength (nanometres)
 *   e => What to do if TCS can't connect (0=Give up, 1=Simulate TCS coords,
 *                                         2=Simulate GMOS coords for Flexure tests).
 *   f => Simulated zenith distance or GMOS thetaY (degrees)
 *   g => Simulated rotator mechanical angle or GMOS thetaZ (degrees)
 *   h => ISS port number
 *   i => Unused
 *   j => Telescope context array
 *
 *   Epics outputs:
 *   vala => TCS Status (1=present, 0=absent, -1=simulated, -2=flexure tests)
 *   valb => GMOS thetaY coordinate (degrees)
 *   valc => GMOS thetaZ coordinate (degrees)
 *   vald => Entrance prism angle (degrees)
 *   vale => Exit prism angle (degrees)
 *   valf => Refraction (ZD shift) at Lmin (arcsec)
 *   valg => Refraction (ZD shift) at Lmax (arcsec)
 *   valh => Dispersion between Lmin and Lmax (arcsec)
 *
 *   External functions:
 *   Any external functions used in this function or procedure e.g
 *   function1      (library name)     What function does
 *
 *   Deficiencies:
 *   Any known problems with the function
 *
 *-
 */

long gmSeqAtmDemands (struct genSubRecord *pgsub)
{
   static double lmin = 400.0;       /* Minimum wavelength (nanometres)           */
   static double lmax  = 900.0;      /* Maximum wavelength (nanometres)           */
   long   tcsSim ;                   /* What to do if TCS can't connect (0/1/2)   */
   double iaa ;                      /* Inst. Alignment Angle (degrees)           */
   double ipdOffset ;                /* offset of prisms from IPD (degrees)       */
   double zdSim, rmaSim;             /* Simulated ZD and rotator mech angle       */
   double wlmin ;                    /* Minimum wavelength read from CAD          */
   double wlmax ;                    /* Maximum wavelength read from CAD          */
   double tempK ;                    /* Ambient temperature (deg K)               */
   double relH ;                     /* Relative humidity                         */
   double pressMb ;                  /* Pressure (millibars)                      */
   double refWl ;                    /* Reference wavelength (nanometres)         */
   double refa ;                     /* Refraction coefficient A                  */
   double refb ;                     /* refraction coefficient B                  */
   double refaLmin, refbLmin ;       /* Refraction coeffs at min wavelength, lmin */
   double refaLmax, refbLmax ;       /* Refraction coeffs at max wavelength, lmax */
   double theta ;                    /* Prism angle (degrees)                     */
   double phi ;                      /* Overall orientation of prisms (degrees)   */
   double enPrism, exPrism;          /* Entrance and exit prism angles (degrees)  */
   static double prevenPrism = 0.0;  /* Previous entrance prism angle.            */
   static double prevexPrism = 0.0;  /* Previous exit prism angle.                */
   double tanzd ;                    /* Tan(zenith angle)                         */
   double sinzd ;                    /* Sin(zenith angle)                         */
   double tan2zd ;                   /* Tan(zd)**2                                */
   double dzLmin = 0.0 ;             /* Zenith distance shift at lmin (arcsec)    */
   double dzLmax = 0.0 ;             /* Zenith distance shift at lmax (arcsec)    */
   double disp = 0.0 ;               /* Dispersion (arcsec)                       */
   double conv ;                     /* Conversion factor                         */
   double sinthetaY, sinthetaZ;      /* Sine of the GMOS coordinates.             */
   double rotGmosAngle;              /* Rotator angle seen by GMOS (radians)      */

   /* Pull in the input data */

   iaa       = *(double *)pgsub->a ;
   ipdOffset = *(double *)pgsub->b ;

   wlmin  = *(double *)pgsub->c ;
   wlmax  = *(double *)pgsub->d ;

   tcsSim = *(long *)pgsub->e ;
   zdSim  = *(double *)pgsub->f ;
   rmaSim = *(double *)pgsub->g ;

   gmSeqIssPort = *(long *)pgsub->h ;


   /*
    * Perform a sanity check on the wavelength data. It is possible that the
    * user has either never specified a wavelength or was in the middle of
    * updating them when this routine was called. If the new values look odd
    * the previous values are used. If this is the first pass the defaults
    * defined in the variable declarations are used.
    */

   if (wlmax >= wlmin)
   {

       /*
        * The data passed to this routine should already be checked for range but
        * check again just in case 
        */

       if (wlmin > MIN_WAVELENGTH && wlmin < MAX_WAVELENGTH && wlmax > MIN_WAVELENGTH &&
           wlmax < MAX_WAVELENGTH )
       {
          lmin = wlmin ;
          lmax = wlmax ;
       }
    }

    /*
     * Determine whether the TCS is present or absent. If absent, use the tcsSim flag
     * to decide what to do (0= give up, 1=simulate TCS, 2=simulate GMOS coordinates).
     */

    if (pgsub->sevr != INVALID_ALARM && pgsub->noj == AST_CTXA_SIZE)
    {

        /*
         * The TCS is present and connected. NOTE: If the TCS is present it connects
         * regardless of the simulation setting.
         */

        if ( gmSeqTcsStatus != 1 )
        {
           logMsg ("*** TCS connected ***\n", 0,0,0,0,0,0);
           gmSeqTcsStatus = 1;
        }
     }
     else if ( tcsSim == 1 )
     {

        /* The TCS is being simulated */

        if ( gmSeqTcsStatus != -1 )
        {
           logMsg ("*** TCS simulated ***\n", 0,0,0,0,0,0);
           gmSeqTcsStatus = -1;
        }
     }
     else if ( tcsSim == 2 )
     {

        /* Flexure tests are being carried out - use GMOS coordinates directly */

        if ( gmSeqTcsStatus != -2 )
        {
           logMsg ("*** Flexure tests ***\n", 0,0,0,0,0,0);
           gmSeqTcsStatus = -2;
        }
     }
     else
     {
        /* The TCS is not being simulated and has disconnected. */

        if ( gmSeqTcsStatus != 0 )
        {
           logMsg ("*** TCS disconnected ***\n", 0,0,0,0,0,0);
           gmSeqTcsStatus = 0;
        }
     }


     /* Check whether real or simulated TCS context information is available. */

     if ( gmSeqTcsStatus != 0 )
     {

        /* If the TCS has connected read its context, otherwise use simulated parameters */

        if ( gmSeqTcsStatus == 1 )
        { 

           /* Protect the TCS context global variables with a semaphore */

           if ( semTake( gmSeqTcsCtxSem, WAIT_FOREVER ) == OK )
           {
              gmSeqGetRmaZ (pgsub, &gmSeqRotMechAng, &gmSeqZenDist) ;
              gmSeqGetRefParams (pgsub, &tempK, &pressMb, &relH, &refWl, &refa, &refb) ;
              semGive( gmSeqTcsCtxSem );
           }
        }
        else
        {

           /* Use simulated TCS information */

           if ( semTake( gmSeqTcsCtxSem, WAIT_FOREVER ) == OK )
           {
              gmSeqZenDist    = zdSim  * DD2R;
              gmSeqRotMechAng = rmaSim * DD2R;
              tempK   = 273.0;
              pressMb = 650.0;
              relH    = 0.25;
              refWl   = 0.4;
              refa    = 0.10;
              refb    = 0.11;
              semGive( gmSeqTcsCtxSem );
           }
        }

        /*
         * Compute the orientation in GMOS coordinates, protecting
         * the coordinates global variables with a semaphore.
         */

        if ( semTake( gmSeqGmosSem, WAIT_FOREVER ) == OK )
        {

           if ( gmSeqTcsStatus != -2 )
           {

              /* Convert TCS coordinates to GMOS coordinates */

              if ( gmSeqIssPort > 1 )
              {

                 rotGmosAngle = (DPIBY2 * (double) (gmSeqIssPort-1)) + gmSeqRotMechAng;

                 /* Side looking port - 2,3,4,5 (wraps for larger port numbers) */

                 sinzd = sin( gmSeqZenDist );
                 sinthetaY = sinzd * cos( rotGmosAngle );
                 sinthetaZ = sinzd * sin( rotGmosAngle );

                 gmSeqThetaY = asin( sinthetaY );
                 gmSeqThetaZ = (iaa * DD2R) + asin( sinthetaZ );
              }
              else
              {

                 /* Assume an upward looking port for port 1 (or smaller) */

                 gmSeqThetaY = gmSeqZenDist - DPIBY2;
                 gmSeqThetaZ = (iaa * DD2R) + gmSeqRotMechAng;
              }
           }
           else
           {

              /* Flexure tests - GMOS coordinates provided directly */

              gmSeqThetaY = gmSeqZenDist;
              gmSeqThetaZ = gmSeqRotMechAng;
           }

           semGive( gmSeqGmosSem );
        }

        /* Generate the refraction coefficients for the min & max wavelengths */

        slaAtmdsp (tempK, pressMb, relH, refWl, refa, refb, lmin, &refaLmin,
                   &refbLmin) ;
        slaAtmdsp (tempK, pressMb, relH, refWl, refa, refb, lmax, &refaLmax,
                   &refbLmax) ;

        /* Compute the dispersion */

        tanzd  = tan(gmSeqZenDist) ;
        tan2zd = tanzd * tanzd ;
        dzLmin = (refaLmin + refbLmin * tan2zd) * tanzd * DR2AS ;
        dzLmax = (refaLmax + refbLmax * tan2zd) * tanzd * DR2AS ;

        /*
         * NOTE: wlmin should be chosen so that lmin and lmax can never be
         * equal to 230.0
         */

        disp = dzLmin - dzLmax ; 
        conv = 572.0 * ( 1.0 /(lmin - 230.0) - 1.0 /(lmax - 230.0)) ;

        if (conv < 1.e-6)
        {
           theta = 90.0 ;
        }
        else if (disp/conv <= 1.0)
        {
           theta = acos (disp/conv) * DR2D ;
        }
        else
        {
           theta = 0.0 ;
        }

        phi   = - (iaa + gmSeqRotMechAng*DR2D + ipdOffset) ;

        /*
         * Determine the entrance and exit prism angles, ensuring they remain
         * within the 0-360 degree range.
         */

        enPrism = phi - theta ;
        if ( enPrism > 360.0 ) enPrism -= 360.0;
        if ( enPrism < 0.0 )   enPrism += 360.0;
        prevenPrism = enPrism ;

        exPrism = phi + theta ;
        if ( exPrism > 360.0 ) exPrism -= 360.0;
        if ( exPrism < 0.0 )   exPrism += 360.0;
        prevexPrism = exPrism ;

        /* Output the data for the downstream processing */

        *(double *)pgsub->valb = gmSeqThetaY * DR2D ;
        *(double *)pgsub->valc = gmSeqThetaZ * DR2D ;
        *(double *)pgsub->vald = enPrism ;
        *(double *)pgsub->vale = exPrism ;
        *(double *)pgsub->valf = dzLmin ;
        *(double *)pgsub->valg = dzLmax ;
        *(double *)pgsub->valh = disp ;
     }
     else
     {

        /*
         * There is no TCS information. Use the previous prism angle targets
         * until the TCS connects again. Set other outputs to dummy values.
         */

        *(double *)pgsub->valb = 0.0 ;
        *(double *)pgsub->valc = 0.0 ;
        *(double *)pgsub->vald = prevenPrism ;
        *(double *)pgsub->vale = prevexPrism ;
        *(double *)pgsub->valf = -1.0 ;
        *(double *)pgsub->valg = -1.0 ;
        *(double *)pgsub->valh = -1.0 ;
     }

     /*
      * Write the TCS status.
      */

     *(long *)pgsub->vala = gmSeqTcsStatus ;
  
    return OK ;
}

/*+
 *   Function name:
 *   gmSeqDtaDemands
 *
 *   Purpose:
 *   Calculates the detector translations as a function of orientation
 *   and temperature
 *
 *   Description:
 *   This routine implements the open loop model used to correct the 
 *   position of the GMOS detector for flexure and temperature. The routine
 *   first pulls in all the input data from its links and then, on the 
 *   basis of the tracking mode computes demands for the X, Y and Z axes of
 *   the detector translation assembly.
 *
 *   Invocation:
 *   gmSeqDtaDemands(pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)    pgsub    (genSubRecord *) Pointer to gensub structure
 *
 *   Function value:
 *   (<)  status  (long)  Return status, 0 = OK; -1 = out of range
 * 
 *
 *   Epics inputs:
 *   
 *   a => Current temperature (deg C) 
 *   b => Temperature reference point (deg C)
 *   c => Unused
 *   d => Mode flag (long)
 *   e => Unused 
 *   f => Unused 
 *   g => Focus offset for current grating (microns), filter 1, filter 2 and IFU
 *   h => Focus offset for filter 1 effective wavelength (microns)
 *   i => Focus offset for filter 1 effective wavelength (microns)
 *   j => Focus offset for grating effective wavelength (microns)
 *   k => Grating order (0 for imaging) (long)
 *   l => Grating tilt angle (degrees)
 *   m => X offset specified by user (microns)
 *   n => Y offset specified by user (microns)
 *   o => Z offset specified by user (microns)
 *   p => Minimum X acceptable to translation assembly (microns)
 *   q => Maximum X acceptable to translation assembly (microns)
 *   r => Minimum Y acceptable to translation assembly (microns)
 *   s => Maximum Y acceptable to translation assembly (microns)
 *   t => Minimum Z acceptable to translation assembly (microns)
 *   u => Minimum Z acceptable to translation assembly (microns)
 *
 *   Epics outputs:
 *   vala => X demand (microns) - truncated to be within range
 *   valb => Y demand (microns) - truncated to be within range
 *   valc => Z demand (microns) - truncated to be within range
 *   vald => X flexure displacement (microns)
 *   vale => Y flexure displacement (microns)
 *   valf => Z flexure displacement (microns)
 *   valg => X Temperature displacement (microns)
 *   valh => Y Temperature displacement (microns)
 *   vali => Z Temperature displacement (microns)
 *   valj => Total focus offset applied (microns)
 *
 *   External functions:
 *   Any external functions used in this function or procedure e.g
 *   function1      (library name)     What function does
 *
 *   Deficiencies:
 *   Any known problems with the function
 *
 *   History:
 *    12-Sep-2000:  pbt Added wavelength related focus offset on input j,
 *                      consequently changed input fields j/k/l => k/l/m.
 *    22-Jan-2001:  smb Check demands are within acceptable range.
 *    31-Jan-2001:  smb Scientific specification has changed. Filter
 *                      effective wavelengths are used to determine
 *                      focus offsets when in imaging mode. Gratings
 *                      can have focus offsets from their surface
 *                      curvature.
 *-
 */

long gmSeqDtaDemands (struct genSubRecord *pgsub)
{
   double currTemp ;        /* Temperature of camera barrel (deg C)             */
   double refTemp ;         /* Reference temperature for expansion (deg C)      */
   long   trackMode ;       /* Current track mode                               */
   static long oldTrackMode = TRACKFOLLOW ;  /* Old track mode                  */
   double filt1Offset ;     /* Focus offset for filter 1 (microns)              */
   double filt2Offset ;     /* Focus offset for filter 2 (microns)              */
   double grOffset ;        /* Focus offset for grating (microns)               */
   long   grOrder ;         /* Grating order (0=imaging)                        */
   double grTilt ;          /* Grating tilt angle (degrees)                     */
   double lambda1Offset ;   /* Focus offset for filter 1 wavelength (microns)   */
   double lambda2Offset ;   /* Focus offset for filter 2 wavelength (microns)   */
   double lambdaGrOffset;   /* Wavelength related focus offset (microns)        */
   double userXoff ;        /* X offset specified by user (microns)             */
   double userYoff ;        /* Y offset specified by user (microns)             */
   double userZoff ;        /* Z offset specified by user (microns)             */
   double flexXoff ;        /* Flexure offset in X (microns)                    */
   double flexYoff ;        /* Flexure offset in Y (microns)                    */
   double flexZoff ;        /* Flexure offset in Z (microns)                    */
   static double prevFlexXoff = 0.0 ; /* Previous Flexure offset in X (microns) */
   static double prevFlexYoff = 0.0 ; /* Previous Flexure offset in Y (microns) */
   static double prevFlexZoff = 0.0 ; /* Previous Flexure offset in Z (microns) */
   double tempXoff ;        /* Temperature X offset (microns)                   */
   double tempYoff ;        /* Temperature Y offset (microns)                   */
   double tempZoff ;        /* Temperature Z offset (microns)                   */
   double deltaT ;          /* Temperature difference from reference (deg C)    */
   static double moveXref = 0.0 ; /* Reference position for move mode           */
   static double moveYref = 0.0 ; /* Reference position for move mode           */
   static double moveZref = 0.0 ; /* Reference position for move mode           */
   double xdem, ydem, zdem ;      /* Demands to translation assembly (microns)  */
   double xmin, xmax;             /* Allowed X range (microns)                  */
   double ymin, ymax;             /* Allowed Y range (microns)                  */
   double zmin, zmax;             /* Allowed Z range (microns)                  */

   double gY, gZ;            /* Anamorphic flexure correction factors to apply  */
                             /* to Y flexure resulting from thetaY and thetaZ.  */
   double alpha, beta;       /* Angles determined from grating tilt (radians).  */
   double oneOverA;          /* Reciprocal of the anamorphic factor, 1/A.       */
   double oneMinusOneOverA;  /* 1 - 1/A                                         */

   double powerFocusOffset;  /* Total focus offset due to surface curvature     */
   double lambdaFocusOffset; /* Total wavelength dependent focus offset         */
   double totalFocusOffset;  /* Total focus offset applied                      */

   long status;            /* Return status */

/* Initialise the return status */

   status = OK;

/* Fetch the data from the input links */

   currTemp  = *(double *)pgsub->a ;
   refTemp   = *(double *)pgsub->b ;
   trackMode = *(long *)pgsub->d ;

/* filt1Offset = *(double *)pgsub->e ;
   filt2Offset = *(double *)pgsub->f ;
   grOffset    = *(double *)pgsub->g ;
*/
   powerFocusOffset = *(double *)pgsub->g;
   lambda1Offset   = *(double *)pgsub->h;
   lambda2Offset   = *(double *)pgsub->i;
   lambdaGrOffset  = *(double *)pgsub->j;

   grOrder     = *(long *)pgsub->k;
   grTilt      = *(double *)pgsub->l;

   userXoff    = *(double *)pgsub->m;
   userYoff    = *(double *)pgsub->n;
   userZoff    = *(double *)pgsub->o;

   xmin        = *(double *)pgsub->p;
   xmax        = *(double *)pgsub->q;
   ymin        = *(double *)pgsub->r;
   ymax        = *(double *)pgsub->s;
   zmin        = *(double *)pgsub->t;
   zmax        = *(double *)pgsub->u;

/* The track mode has been incremented for use by EPICS so decrement it */

   if (trackMode > 0) trackMode-- ;
 
/*
 *  Check if TCS is either connected or being simulated. If neither is the case
 *  then there is nothing we can do about the flexure corrections so we just set
 *  them to the previous values. If this is the first time through the routine
 *  then they'll be set to 0.0
 */

   if (gmSeqTcsStatus == 0)
   {

      flexXoff = prevFlexXoff ;
      flexYoff = prevFlexYoff ;
      flexZoff = prevFlexZoff ;

   }
   else
   {

      /*
       * Calculate the grating anamorphic correction factors if not observing at
       * zero order and the grating tilt is within a sensible range. The correction
       * factor is set to unity if the coefficients are negative or any calculation
       * is invalid.
       */

      if ( (grOrder != 0) && (grTilt < 89.0) && (grTilt > 1.0) )
      {

         alpha = (90.0 - grTilt) * DD2R;   /* radians */
         beta  = gmSeqGrRefAng - alpha;    /* radians */

         /* Avoid regions where oneOverA blows up. */

         if ( beta > -DPIBY2 && beta < DPIBY2 )
         {
            oneOverA = cos ( alpha ) / cos ( beta );
            oneMinusOneOverA = (1.0 - oneOverA);

            if ( gmSeqGrHthetay > 0.0 )
               gY = 1.0 - (oneMinusOneOverA * gmSeqGrHthetay);
            else
               gY = 1.0;

            if ( gmSeqGrHthetaz > 0.0 )
               gZ = 1.0 - (oneMinusOneOverA * gmSeqGrHthetaz);
            else
               gZ = 1.0;
         }
         else
         {

            /* Bad grating reference angle  - set correction factors to unity. */

            gY = gZ = 1.0;
         }
      }
      else
      {

         /* Zero order or invalid tilt angle  - set correction factors to unity. */

         gY = gZ = 1.0;
      }

      /*
       * Obtain the present GMOS orientation, using a semaphore to ensure the
       * values are not obtained while they are being changed by gmSeqAtmDemands.
       * Then use the flexure model to determine the flexure offsets at the present
       * GMOS orientation.
       */

      if ( semTake( gmSeqGmosSem, WAIT_FOREVER ) == OK )
      {

         /* Instrument X = CCD Y (rows) */

/* OLD EQUATION
         flexXoff = - (gmSeqDtaAx * sin ( gmSeqThetaY + gmSeqDtaPhiYX ) +
                       gmSeqDtaBx * sin ( gmSeqThetaZ + gmSeqDtaPhiZX ));
*/
         flexXoff = - (gmSeqDtaAx * sin ( gmSeqThetaY + gmSeqDtaPhiYX ) *
                                    sin ( gmSeqThetaZ + gmSeqDtaPhiZX ));

         /* Instrument Y = CCD X (columns) = dispersion direction */
         /* This direction is affected by the grating anamorphic factor */

         flexYoff = - (gY * gmSeqDtaAy * sin ( gmSeqThetaY + gmSeqDtaPhiYY ) +
                       gZ * gmSeqDtaBy * sin ( gmSeqThetaZ + gmSeqDtaPhiZY ));

         /* Instrument Z = CCD focus */

         flexZoff = - (gmSeqDtaAz * sin ( gmSeqThetaY + gmSeqDtaPhiYZ ) +
                       gmSeqDtaBz * sin ( gmSeqThetaZ + gmSeqDtaPhiZZ ));

         semGive( gmSeqGmosSem );
      }
   }

/* Save flexure offsets for next call to this routine */

   prevFlexXoff = flexXoff ;
   prevFlexYoff = flexYoff ;
   prevFlexZoff = flexZoff ;

/*
 *  Now set up the temperature corrections. These are calculated whether
 *  TCS is present or not. The current model assumes a quadratic dependence
 *  of displacement on temperature.
 *
 *  NOTE: The translation assembly itself has the ability to compensate for
 *  non-analytical thermal effects by means of a lookup table. See devTranslationAss.c.
 */

   deltaT = currTemp - refTemp ;
   tempXoff = (gmSeqDtaTx  + gmSeqDtaT2z * deltaT) * deltaT ;
   tempYoff = (gmSeqDtaTy  + gmSeqDtaT2y * deltaT) * deltaT ;
   tempZoff = (gmSeqDtaTz  + gmSeqDtaT2z * deltaT) * deltaT ;

/*
 * The total focus offset due to surface power is the sum of the contributions
 * from the filters and grating.
 */

/*   This addition is done in the sumOffset record which also adds the ifu offset */

/*   powerFocusOffset = filt1Offset + filt2Offset + grOffset;*/

/*
 * In imaging mode the wavelength dependent focus offset is derived from
 * the sum of the focus offsets from the filter effective wavelengths.
 * In spectroscopy mode the wavelength dependent offset for the grating
 * is used instead.
 */

   if ( grOrder == 0 )
   {

      /* Imaging mode */
      lambdaFocusOffset = lambda1Offset + lambda2Offset;
   }
   else
   {

      /* Spectroscopy mode */
      lambdaFocusOffset = lambdaGrOffset;

   }

   totalFocusOffset = powerFocusOffset + lambdaFocusOffset;


/* Now calculate the output demands on the basis of what the current mode is */
   if (trackMode == TRACKMOVE)
   {
      /* These three lines were missing in the previous version of the software.       */
      /* In MOVE mode apply the current flexure offsets rather than the previous ones. */
      moveXref = flexXoff + tempXoff ;
      moveYref = flexYoff + tempYoff ;
      moveZref = flexZoff + tempZoff ;

      xdem = moveXref + userXoff + gmSeqDtaX0 ;
      ydem = moveYref + userYoff + gmSeqDtaY0 ;
      zdem = moveZref + userZoff + gmSeqDtaZ0 + totalFocusOffset ;
      oldTrackMode = trackMode ;
   }
   else if (trackMode == TRACKFOLLOW)
   {

      moveXref = flexXoff + tempXoff ;
      moveYref = flexYoff + tempYoff ;
      moveZref = flexZoff + tempZoff ;
      xdem = moveXref + userXoff + gmSeqDtaX0 ;
      ydem = moveYref + userYoff + gmSeqDtaY0 ;
      zdem = moveZref + userZoff + gmSeqDtaZ0 + totalFocusOffset ; 
      oldTrackMode = trackMode ;

   }
   else if (trackMode == TRACKFOLLOWXY)
   {

      moveXref = flexXoff + tempXoff ;
      moveYref = flexYoff + tempYoff ;
      xdem = moveXref + userXoff + gmSeqDtaX0 ;
      ydem = moveYref + userYoff + gmSeqDtaY0 ;
      zdem = moveZref + userZoff + gmSeqDtaZ0 + totalFocusOffset ; 
      oldTrackMode = trackMode ;

    }
    else
    {

/* If any other mode has managed to get through then treat it as STOP. */

       if (oldTrackMode == TRACKMOVE) {

          /* These three lines were missing in the previous version of the software.      */
          /* In MOVE mode apply the current flexure offsets rather than the previous ones. */
          moveXref = flexXoff + tempXoff ;
          moveYref = flexYoff + tempYoff ;
          moveZref = flexZoff + tempZoff ;

          xdem = moveXref + userXoff + gmSeqDtaX0 ;
          ydem = moveYref + userYoff + gmSeqDtaY0 ;
          zdem = moveZref + userZoff + gmSeqDtaZ0 + totalFocusOffset ;

       }
       else if (oldTrackMode == TRACKFOLLOW)
       {

          moveXref = flexXoff + tempXoff ;
          moveYref = flexYoff + tempYoff ;
          moveZref = flexZoff + tempZoff ;
          xdem = moveXref + userXoff + gmSeqDtaX0 ;
          ydem = moveYref + userYoff + gmSeqDtaY0 ;
          zdem = moveZref + userZoff + gmSeqDtaZ0 + totalFocusOffset ; 

       }
       else if (oldTrackMode == TRACKFOLLOWXY)
       {

          moveXref = flexXoff + tempXoff ;
          moveYref = flexYoff + tempYoff ;
          xdem = moveXref + userXoff + gmSeqDtaX0 ;
          ydem = moveYref + userYoff + gmSeqDtaY0 ;
          zdem = moveZref + userZoff + gmSeqDtaZ0 + totalFocusOffset ; 
       }
    }

/*
 * If the output demands are not within the acceptable range then truncate
 * then and return an ERROR status.
 */

   if ( xdem < xmin )
   {
      xdem = xmin;
      status = ERROR;
   }
   else if ( xdem > xmax )
   {
      xdem = xmax;
      status = ERROR;
   }
   else if ( ydem < ymin )
   {
      ydem = ymin;
      status = ERROR;
   }
   else if ( ydem > ymax )
   {
      ydem = ymax;
      status = ERROR;
   }
   else if ( zdem < zmin )
   {
      zdem = zmin;
      status = ERROR;
   }
   else if ( zdem > zmax )
   {
      zdem = zmax;
      status = ERROR;
   }


/* Output the data */

   *(double *)pgsub->vala = xdem ;
   *(double *)pgsub->valb = ydem ;
   *(double *)pgsub->valc = zdem ;

   *(double *)pgsub->vald = flexXoff ;
   *(double *)pgsub->vale = flexYoff ;
   *(double *)pgsub->valf = flexZoff ;

   *(double *)pgsub->valg = tempXoff ;
   *(double *)pgsub->valh = tempYoff ;
   *(double *)pgsub->vali = tempZoff ;

   *(double *)pgsub->valj = totalFocusOffset ;

   return status ;
}


/*+
 *   Function name:
 *   gmSeqGetRmaZ
 *
 *   Purpose:
 *   Returns rotator mechanical angle and zenith distance 
 *
 *   Description:
 *   Just copy the relevant variables from the array in pgsub
 *
 *   Invocation:
 *   gmSeqGetRmaZd (pgsub, &rma, &zd)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)    pgsub    (genSubRecord *)  Pointer to gensub structure
 *      (<)    rma      (double *)        Rotator mechanical angle
 *      (<)    zd       (double *)        Zenith distance
 *
 *   Function value:
 *   Return value from function e.g.
 *   (<)  status  (int)  Return status, 0 = OK
 * 
 *   External functions:
 *   Any external functions used in this function or procedure e.g
 *   function1      (library name)     What function does
 *
 *   External variables:
 *   Any external variables used in this function or procedure e.g.
 *   (>)   <name>    <C type>    <variable description>
 *
 *   Prior requirements:
 *   It is assumed that the gensub record using this function has
 *   its INPJ field connected to the TCS context source (tcs:ak:astCtx.VALA)
 *
 *   Deficiencies:
 *   Relies information being in particular locations within the astlib array
 *   and will fail if the astlib array is ever changed.
 *
 *-
 */

void gmSeqGetRmaZ (struct genSubRecord *pgsub, double *rma, double *zd)
{

  double *valjp = pgsub->j ;        /* Pointer j array               */
  double v[3] ;                     /* Components of position vector */
  double az ;                       /* Telescope azimuth (radians)   */
  double el ;                       /* Telescope elevation (radians) */
  double tick ;                     /* Time tick                     */
  double fl ;                       /* Telescope focal length        */

  tick = *valjp++ ;
  v[0] = *valjp++;
  v[1] = *valjp++;
  v[2] = *valjp++;
  fl   = *valjp++;
  *rma = *valjp++;

  slaDcc2s (v, &az, &el) ;
  *zd = DPIBY2 - el ;

}


/*+
 *   Function name:
 *   gmSeqGetRefParams
 *
 *   Purpose:
 *   Returns parameters describing the atmospheric dispersion
 *
 *   Description:
 *   Just copy the relevant variables from the array in pgsub
 *
 *   Invocation:
 *   gmSeqGetRefParams (pgsub, &tempK, &pressMb, &relH, &refWl, &refa, &refb)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)    pgsub    (genSubRecord *)  Pointer to gensub structure
 *      (<)    tempK    (double *)        Ambient temperature in K
 *      (<)    pressMb  (double *)        Ambient pressure in millibars
 *      (<)    relH     (double *)        Relative humidity
 *      (<)    refWl    (double *)        Reference wavelength
 *      (<)    refa     (double *)        ?
 *      (<)    refa     (double *)        ?
 *
 *   Function value:
 *   Return value from function e.g.
 *   (<)  status  (int)  Return status, 0 = OK
 * 
 *   External functions:
 *   Any external functions used in this function or procedure e.g
 *   function1      (library name)     What function does
 *
 *   External variables:
 *   Any external variables used in this function or procedure e.g.
 *   (>)   <name>    <C type>    <variable description>
 *
 *   Prior requirements:
 *   It is assumed that the gensub record using this function has
 *   its INPJ field connected to the TCS context source (tcs:ak:astCtx.VALA)
 *
 *   Deficiencies:
 *   Relies information being in particular locations within the astlib array
 *   and will fail if the astlib array is ever changed.
 *
 *-
 */
 
void gmSeqGetRefParams (struct genSubRecord *pgsub, double *tempK,
                        double *pressMb, double *relH, double *refWl,
                        double *refa, double *refb )
{
   double *valjp = pgsub->j ;
   int i ;
   double lapse ;                 /* Tropospheric lapse rate */
   double dummy ;                 /* Dummy variable */

/* Skip the first 16 entries */

   for (i = 0; i < 16 ; i++ ) {
      dummy = *valjp++ ;
   }

   *tempK   = *valjp++ ;
   *pressMb = *valjp++ ;
   *relH    = *valjp++ ;
   *refWl   = *valjp++ ;
   lapse    = *valjp++ ;
   *refa    = *valjp++ ;
   *refb    = *valjp++ ;

}

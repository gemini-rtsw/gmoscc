/* ===================================================================== */
/* INDENT OFF */
/*+
 *
 * FILENAME
 * -------- 
 * gmosLutLib.h
 * 
 * PURPOSE
 * -------
 * Header file for gmosLutLib library
 * 
 * NOTE
 * ----
 * The LUT arrays must be unit offset. I.E. They start at index 1 NOT 0
 *
 * AUTHORS
 * -------
 * Nigel Dipper, Dept. of Physics, University of Durham  (N.A.Dipper@durham.ac.uk)
 * Steven Beard, UK Astronomy Technology Centre          (smb@roe.ac.uk)
 * 
 * HISTORY
 * -------
 * $Log$
 * Revision 1.1  2002/04/24 05:25:35  ajf
 * Added for port to epics3.13.4GEM8.4.
 *
 * Revision 1.1  2001/11/28 20:08:50  mbec
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.3  2000/06/02 11:17:10  gmos
 * Cyclic lookup table handling added
 *
 * Revision 1.2  2000/05/15 16:49:05  gmos
 * Lookup table code tidied up and now assumes arrays start from index
 *
 * Revision 1.1  2000/05/11 12:55:56  gmos
 * gratingLib renamed gmosLutLib
 *
 * Revision 1.2  2000/05/10 08:15:50  gmos
 * Code padded out and made more readable. Lookup table checking function and error message function added.
 *
 * Revision 1.1  1999/12/10 14:33:46  gmos
 * Contents of assembly modules merged into gmoscc
 *
 * Revision 1.2  1999/11/02 15:22:06  pbt
 * Added routine gratingSci2Tilt
 *
 * Revision 1.1  1999/09/22 14:56:27  gmos
 * New grating tilt function received from Nigel Dipper and converted to Gemini/VxWorks library
 *
 *-
 */
/* INDENT ON */
/* ===================================================================== */

#ifndef GMOSLUTLIB_INC
#define GMOSLUTLIB_INC

/* Public constants */

#define MAX_GMOS_LUT_ENTRIES  1500             /* Max number of entries in lookup table */

/* Error status codes */

#define GMOSLUT_S_OK               0         /* Status return: OK                   */
#define GMOSLUT_E_TOOSMALL        -1000      /* Insufficient LUT entries            */
#define GMOSLUT_E_OUTRANGE        -1001      /* Value outside range of lookup table */
#define GMOSLUT_E_INVLUT          -1002      /* Invalid lookup table entry          */
#define GMOSLUT_E_LUTEQUAL        -1003      /* Two or more LUT entries equal       */
#define GMOSLUT_E_LUTNOTMONO      -1004      /* Lookup table entries not monotonic  */
#define GMOSLUT_E_NOSOLUTION      -1005      /* No solution found                   */
#define GMOSLUT_E_INTERPOLATION   -1006      /* Error during interpolation          */

/* Lookup table structure definition */

typedef struct
{
  int numberOfEntries;                       /* Number of entries in the table */
  double input[MAX_GMOS_LUT_ENTRIES];        /* Input parameter value          */
  double measured[MAX_GMOS_LUT_ENTRIES];     /* Measured response              */
} gmosLookupTable;

/* Public function prototypes */

int  gmosLutCheck( gmosLookupTable *lut );
int  gmosLutApply( const double given, gmosLookupTable *lut, double *measured );
int  gmosLutCycleApply( const double given, gmosLookupTable *lut, double *measured );
int  gratingSci2Tilt( const double wavelength, const int order,
                      const double r, double *tiltAngle );
void gmosLutMessage( const int code, const int size, char *message );

#endif

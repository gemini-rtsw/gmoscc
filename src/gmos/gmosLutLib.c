static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id$"};
/* ===================================================================== */
/* INDENT OFF */
/*+
 *
 ************************************************************************
 *  U N I V E R S I T Y    O F    D U R H A M                           *
 *                                                                      *
 *  Astronomical Instrumentation Group                                  *
 *                                                                      *
 *  Science Laboratories, South Road, Durham, DH1 3LE, UK.              *
 *                                                                      *
 *  Funded by the Particle Physics and Astronomy Research Council.      *
 *                                                                      *
 ************************************************************************
 *  U K     A S T R O N O M Y    T E C H N O L O G Y    C E N T R E     *
 *                                                                      *
 *  Royal Observatory, Blackford Hill, Edinburgh, EH9 3HJ, UK.          *
 *                                                                      *
 *  Funded by the Particle Physics and Astronomy Research Council.      *
 *                                                                      *
 ************************************************************************
 *
 * FILENAME
 * -------- 
 * gmosLutLib.c
 * 
 * PURPOSE
 * -------
 * Library of GMOS lookup table and calibration functions
 * 
 * FUNCTION NAME(S)
 * ----------------
 * gmosLutCheck         - check lookup table to ensure adherence to requirements
 * gmosLutApply         - convert given value into measured value by interpolation
 * gmosLutCycleApply    - convert given value into measured value by interpolation using cyclic lookup table
 * gratingSci2Tilt      - convert grating wavelength, order and ruling density into tilt
 * gmosLutMessage       - translate error code into message
 *
 * PRIVATE FUNCTIONS
 * -----------------
 * gmosLutPolint        - polynomial interpolation routine 
 * gmosLutLocate        - finds first index in array below X value required
 *
 * DEPENDENCIES
 * ------------
 * The routines do not rely on any external libraries that are not ANCI C
 * standard.
 *
 * LIMITATIONS
 * -----------
 * These functions only work with MONOTONIC lookup tables with at least 4 points.
 * Extrapolations outside the lookup table range are not supported.
 *
 * NOTE
 * ----
 * The gratingSci2Tilt function is now looking out of place.
 * The functions could do with some wflman style function prologues.
 *
 * AUTHORS
 * -------
 * Nigel Dipper, Dept. of Physics, University of Durham  (N.A.Dipper@durham.ac.uk)
 * Steven Beard, UK Astronomy Technology Centre          (smb@roe.ac.uk)
 * 
 * HISTORY
 * -------
 * $Log$
 * Revision 1.2  2003/05/27 21:16:51  gemvx
 * Version 4-1
 *
 * Revision 1.1  2001/11/28 20:08:50  mbec
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.4  2000/06/02 11:18:32  gmos
 * Whoops. I forgot to disable VERBOSE
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
 * Revision 1.1  1999/12/10 14:33:44  gmos
 * Contents of assembly modules merged into gmoscc
 *
 * Revision 1.5  1999/11/30 13:46:54  gmos
 * Added UKATC and University of Durham banner
 *
 * Revision 1.4  1999/11/02 15:15:37  pbt
 * Added routine gratingSci2Tilt
 *
 * Revision 1.3  1999/10/05 14:33:13  dlt
 * Use correct vxWorks preprocessor symbol
 *
 * Revision 1.2  1999/09/23 13:05:24  gmos
 * Nigel Dippers own comments extracted from email and added
 *
 * Revision 1.1  1999/09/22 14:56:27  gmos
 * New grating tilt function received from Nigel Dipper and converted to Gemini/VxWorks library
 *
 * 21-Sep-1999: Original version (1.1) received from Durham (ndipper)
 *
 *-
 */
/* INDENT ON */
/* ===================================================================== */

#ifdef vxWorks
#include <vxWorks.h>
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "gmosLutLib.h"

/* Private constants and macros  (public ones are defined in gmosLutLib.h) */

/*#define VERBOSE */               /* Define this constant for verbose messages */

#define TRUE 1                      /* Logical constant. */

/* Lookup table interpolation calibration constants */
#define NUM_POINTS 4                /* Number of points between which to interpolate */

/* Grating science to tilt conversion constants */
#define COL_TO_SPEC_ANGLE 50.0      /* Angle between collim. and spect. axes in degs*/  
#define STEP_SIZE 0.1               /* Step size for tilt angle in degs*/  

/* Macros */
#define MIN(a, b)       ((a) < (b) ? (a) : (b)) /* Find minimum of two integers */ 
#define MAX(a, b)       ((a) < (b) ? (b) : (a)) /* Find maximum of two integers */

/* Private function prototypes (public functions are defined in gmosLutLib.h) */

static int gmosLutPolint(double xa[], double ya[], double x, double *y, double *dy);
static int gmosLutLocate(double xx[], int n, double x, int *j);
static int gmosLutCycleLocate(double xx[], int n, double x, double *newx, int *j);


/* --------------------------------------------------------------------- */

#ifndef vxWorks        /* Only compile the next section for UNIX test version of code */

/* Dummy main program for testing purposes only */
int main()
{
  int i;
  float x;
  double tiltAngle, fsteps, wavelength;
  int status, order, steps, rulingDensity;
  gmosLookupTable lut;

  char message[80];

  /* Determine which test is needed */
  printf ("Test gmosLutApply (1), gmosLutCycleApply (2) or gratingSci2Tilt: (3): ");
  while ( (scanf ("%d", &i) == 1) && (i != 1) && (i != 2) && (i != 3) )
    printf ("1, 2 or 3: ");

  if ( i == 1 )
  {

    /* Make a dummy LUT structure */
    lut.numberOfEntries = 91;

    for (i=0; i<lut.numberOfEntries; i++)
    {
      lut.input[i]    = (double) i;
      lut.measured[i] = (double) 10*i*i;
    }

    printf ("Testing gmosLutCheck\n");
    status = gmosLutCheck( &lut );
    gmosLutMessage( status, 80, message );
    printf("status=%d (%s)\n", status, message);

    printf ("Testing gmosLutApply\n");
    while (TRUE)
   {
      printf("Enter tilt angle: ");
      if ( scanf("%f", &x) == 1 )
      {
        tiltAngle = (double) x;
        status = gmosLutApply(tiltAngle, &lut, &fsteps);
        steps = (int) (fsteps+0.5);
        gmosLutMessage( status, 80, message );
        printf("Steps value is: %d, fsteps=%f, status=%d (%s)\n",
               steps, fsteps, status, message);
      }
      else
      {
        /* Exit from the loop if there is an end of file or bad conversion. */
        break;
      }
    }
  }
  else if ( i == 2 )
  {

    /* Make a dummy LUT structure */
    lut.numberOfEntries = 91;

    for (i=0; i<lut.numberOfEntries; i++)
    {
      lut.input[i]    = (double) i;
      lut.measured[i] = (double) 10*i*i;
    }

    printf ("Testing gmosLutCheck\n");
    status = gmosLutCheck( &lut );
    gmosLutMessage( status, 80, message );
    printf("status=%d (%s)\n", status, message);

    printf ("Testing gmosLutCycleApply\n");
    while (TRUE)
   {
      printf("Enter tilt angle: ");
      if ( scanf("%f", &x) == 1 )
      {
        tiltAngle = (double) x;
        status = gmosLutCycleApply(tiltAngle, &lut, &fsteps);
        steps = (int) (fsteps+0.5);
        gmosLutMessage( status, 80, message );
        printf("Steps value is: %d, fsteps=%f, status=%d (%s)\n",
               steps, fsteps, status, message);
      }
      else
      {
        /* Exit from the loop if there is an end of file or bad conversion. */
        break;
      }
    }
  }
  else
  {
    while (TRUE)
    {
      printf("Enter central wavelength (nm): ");
      scanf("%f", &x);
      wavelength = (double) x;

      printf("Enter grating ruling density (/mm): ");
      scanf("%f", &x);
      rulingDensity = (double) x;

      printf("Enter (integer) order: ");
      if ( scanf("%d", &order) == 1 )
      {
        status = gratingSci2Tilt(wavelength, order, rulingDensity, &tiltAngle);

        if (status == GMOSLUT_S_OK)
        {
          printf("Grating tilt angle is: %f degrees\n", tiltAngle);
        }
        else
        {
          gmosLutMessage( status, 80, message );
          printf("Error in gratingSci2Tilt (%s)\n", message);
        }
      }
      else
      {
        /* Exit from the loop if there is an end of file or bad conversion. */
        break;
      }
    }
  }
  return 0;
}
#endif   /* NOT VXWORKS */


/* --------------------------------------------------------------------- */

/*+
 * Return an interpolated measured value for a given value
 *
 * The given value versus measured value data is assumed to be perfect. This should be a
 * reasonable assumption. The calibration data are expected to accurately
 * represent the (non-linear) functional form of the drive system. We are
 * NOT interested in defining the function (or its coefficients). We are ONLY
 * interested in a good interpolation to a given value given the calibration
 * data.
 *
 * The routine fits an m-1 order polynomial to the m points centred as far as
 * possible on the given data point. The number of points is a parameter:
 * NUM_POINTS which I have set to 4. It should be fine left at 4.
 *
 * The LUT structure can be any set of MONATONIC points and can run over any range
 * (e.g. -5 to 95 degrees).
 *-
 */

int gmosLutApply ( 
    const double givenValue,  /* Given value, such as tilt angle */
    gmosLookupTable *lut,     /* Pointer to lookup table */
    double *measuredValue     /* Pointer to measured value, such as number of steps */
)
{
    int    status;            /* Status return */
    int    indexBelow;        /* Index of the first entry in the table BELOW givenValue */
    int    firstIndex;        /* Index of first entry in lookup table to use for interpolation */ 
    double fmeas;             /* Interpolated measured value */
    double dfmeas;            /* Error in interpolated measured value */

#ifdef VERBOSE
    printf ("gmosLutApply: givenValue=%f\n", givenValue);
#endif

    /* Find first entry in the table BELOW givenValue */

    status = gmosLutLocate (lut->input, lut->numberOfEntries, givenValue, &indexBelow);


    if (status == GMOSLUT_S_OK)
    {
        /* Find first index to use, allowing for end effects */

        firstIndex = MIN ( MAX(indexBelow-(NUM_POINTS-1)/2,0), (lut->numberOfEntries)-NUM_POINTS);

        /* Do the interpolation */

        status = gmosLutPolint ((lut->input)+firstIndex, (lut->measured)+firstIndex, givenValue,
                                &fmeas, &dfmeas);

#ifdef VERBOSE
        printf ("gmosLutApply: indexBelow=%d, firstIndex=%d, fmeas=%f, dfmeas=%f\n",
            indexBelow, firstIndex, fmeas, dfmeas);
#endif

        *measuredValue = fmeas;
    }
    return status;
}



/* --------------------------------------------------------------------- */

/*+
 * Return an interpolated measured value for a given value assuming a cyclic lookup table
 *
 * This function is returns an interpolated value from the lookup table, just like
 * gmosLutApply, except the lookup table is assumed to be cyclic. This means the
 * lookup table repeats itself over and over again for given values which are
 * outside the normal range of the lookup table.
 *
 * The same assumptions apply to the lookup table. It must be MONOTONIC.
 *-
 */

int gmosLutCycleApply ( 
    const double givenValue,  /* Given value, such as tilt angle */
    gmosLookupTable *lut,     /* Pointer to cyclic lookup table */
    double *measuredValue     /* Pointer to measured value, such as number of steps */
)
{
    int    status;            /* Status return */
    int    indexBelow;        /* Index of the first entry in the table BELOW givenValue */
    int    firstIndex;        /* Index of first entry in lookup table to use for interpolation */
    double insideValue;       /* Given value as processed to appear inside the lookup table */
    double fmeas;             /* Interpolated measured value */
    double dfmeas;            /* Error in interpolated measured value */

#ifdef VERBOSE
    printf ("gmosLutCycleApply: givenValue=%f\n", givenValue);
#endif

    status = GMOSLUT_S_OK;

    /* Find first entry in the table BELOW givenValue */

    status = gmosLutCycleLocate (lut->input, lut->numberOfEntries, givenValue,
                                 &insideValue, &indexBelow);

    if ( status == GMOSLUT_S_OK )
    {

        /* Find first index to use, allowing for end effects */

        firstIndex = MIN ( MAX(indexBelow-(NUM_POINTS-1)/2,0), (lut->numberOfEntries)-NUM_POINTS);

        /* Do the interpolation */

        status = gmosLutPolint ((lut->input)+firstIndex, (lut->measured)+firstIndex, insideValue,
                                &fmeas, &dfmeas);

#ifdef VERBOSE
        printf ("gmosLutCycleApply: insideValue=%f, indexBelow=%d, firstIndex=%d, fmeas=%f, dfmeas=%f\n",
            insideValue, indexBelow, firstIndex, fmeas, dfmeas);
#endif

        *measuredValue = fmeas;
    }
    return status;
}


/* --------------------------------------------------------------------- */

/*+
 * Polynomial interpolation routine.
 *
 * The routine makes use of Neville's algorithm.
 * It builds a tableau of Parent and Daughter corrections that make
 * the interpolation one order higher.
 *-
 */

static int gmosLutPolint (
    double xa[],  /* Pointer to the first x (given)  value to be used   */
    double ya[],  /* Pointer to the first y (measured) value to be used */
    double x,     /* The requested value of x (given)                   */
    double *y,    /* Pointer to interpolated y (measured) value         */ 
    double *dy)   /* Pointer to the error in y (measured) value         */
{
    const int n = NUM_POINTS;                /* A short name for the number of points. */

    int i, m, ns;
    double den, mult, dif, dift, ho, hp, w;
    double c[NUM_POINTS], d[NUM_POINTS];       /* The arrays of correction terms */
    int status;                                /* Returned status */

    status = GMOSLUT_S_OK;

   /*
    * Find the index, ns, of the nearest lookup table entry to the given x.
    * At the same time initialise the correction terms, c and d.
    */

    ns  = 0;
    dif = fabs(x-xa[0]);

    for (i=0; i<n; i++)
    {
         dift = fabs( x - xa[i] );

         if ( dift < dif )
         {
             ns = i;
             dif = dift;
         }

         /* Initialise the correction terms */
         c[i] = ya[i];
         d[i] = ya[i];
    }

    /* Save an initial approximation to y, and then decrement ns */

    *y = ya[ns];
    ns--;

   /*
    * For each column of the tableau, loop over the current
    * correction terms (c and d) and update them.
    */

    for (m=1; m<n; m++)
    {       
        for (i=0; i<(n-m); i++)
        {
            ho = xa[i] - x;
            hp = xa[i+m] - x;
            w  = c[i+1] - d[i];
            den = ho - hp;

            if (fabs(den) < 1.0e-10)
            {
                /*
                 * Division by zero during polynomial interpolation,
                 * most likely caused by bad lookup table entries.
                 */

                 status = GMOSLUT_E_INVLUT;
                 return status;
            }

            /* Update c's and d's */

            mult = w/den;
            d[i] = hp*mult;
            c[i] = ho*mult;
        }

       /*
        * Column of tableau complete. Decide whether to add c or d to accumulating
        * value of y. Update ns accordingly
        */

        if ( 2*(ns+1) < (n-m) )
        {
            *dy = c[ns+1];
        }
        else
        {
            *dy = d[ns];
            ns--;
        }

        *y += *dy;
    }

    return status;
}


/* --------------------------------------------------------------------- */

/*+
 * Checks the lookup table to ensure it is monotonic.
 *-
 */

int gmosLutCheck (
    gmosLookupTable *lut  /* Pointer to lookup table */
    )
{
    int status;       /* Status return */

    int i;            /* Index */
    int n;            /* Number of entries */
    int ascending;    /* Given table ascending or descending */

    double *xx;       /* Pointer to given value */
    double *yy;       /* Pointer to measured value */

    status = GMOSLUT_S_OK;

   /* Initialise the number of points and pointers to the start of the table */

    n  = lut->numberOfEntries;
    xx = lut->input;
    yy = lut->measured;

   /* Ensure the lookup table has at least NUM_POINTS entries. */

    if ( n < NUM_POINTS )
    {
        status = GMOSLUT_E_TOOSMALL;
        return (status);
    }

   /* Determine whether the table entries are ascending or descending */

    ascending = (xx[n-1] >= xx[0]);

   /*
    * Check the given table to ensure the table changes monotonically and no two
    * entries are the same.
    */

    if ( ascending )
    {
#ifdef VERBOSE
        printf ("gmosLutCheck: A i=0 xx=%f yy=%f\n", xx[0], yy[0]);
#endif

        for ( i=1; i<n; i++ )
        {
#ifdef VERBOSE
            printf ("gmosLutCheck: A i=%d xx=%f yy=%f\n", i, xx[i], yy[i]);
#endif
            if ( xx[i-1] > xx[i] )
            {
                /* Given table is not monotonic */

                status = GMOSLUT_E_LUTNOTMONO;
                return (status);
            }
            else if ( xx[i-1] == xx[i] )
            {

                /* Equal lookup table entries */

                status = GMOSLUT_E_LUTEQUAL;
                return (status);
            }
        } 
    }
    else
    {
#ifdef VERBOSE
        printf ("gmosLutCheck: D i=0 xx=%f yy=%f\n", xx[0], yy[0]);
#endif

        for ( i=1; i<n; i++ )
        {
#ifdef VERBOSE
            printf ("gmosLutCheck: D i=%d xx=%f yy=%f\n", i, xx[i], yy[i]);
#endif
            if ( xx[i-1] < xx[i] )
            {

                /* Given table is not monotonic */

                status = GMOSLUT_E_LUTNOTMONO;
                return (status);
            }
            else if ( xx[i-1] == xx[i] )
            {

                /* Equal lookup table entries */

                status = GMOSLUT_E_LUTEQUAL;
                return (status);
            }
        } 
    }

    return (status);
}


/* --------------------------------------------------------------------- */

/*+
 * Return first index in array BELOW or equal to the x value required.
 * Returns bad status if x is out of range of lookup table.
 * Assumes the lookup table is monotonic and has at least two entries
 *-
 */

static int gmosLutLocate (
    double xx[],   /* Pointer to first entry in lookup table */
    int    n,      /* Number of entries in the lookup table */
    double x,      /* Value of x (given value) to locate */
    int    *j      /* Pointer to returned index */
    )
{
    int status;    /* Return status */
    int ju;        /* Upper index */
    int jm;        /* Middle index */
    int jl;        /* Lower index */
    int ascending; /* Table ascending or descending */

    status = GMOSLUT_S_OK;

    jl = -1;       /* Initialise lower index */
    ju = n;        /* Initialise upper index */

   /*
    * Find if there are ascending or descending values in table,
    * assuming the table is monotonic.
    */

    ascending = (xx[n-1] >= xx[0]);

   /*
    * Check for an out of range value.
    */

    if ( n < 2 )
    {
        /* Lookup table is too small */

        status = GMOSLUT_E_TOOSMALL;
        return (status);
    }

    if ( ascending )
    {
        if (x < xx[0])
        {
            /* Value is outside the lower index of the lookup table */

            status = GMOSLUT_E_OUTRANGE;
            return (status);
        }
        else if ( x > xx[n-1])
        {
            /* Value is outside the upper index of the lookup table */

            status = GMOSLUT_E_OUTRANGE;
            return (status);
        }
    }
    else
    {
        if (x > xx[0])
        {
            /* Value is outside the lower index of the lookup table */

            status = GMOSLUT_E_OUTRANGE;
            return (status);
        }
        else if ( x < xx[n-1])
        {
            /* Value is outside the upper index of the lookup table */

            status = GMOSLUT_E_OUTRANGE;
            return (status);
        }
    }

   /* Perform a binary search */

    while ((ju - jl) > 1)
    {

        /* Calculate the index value in the middle */

        jm = (ju + jl) / 2;

       /*
        * The next piece of code replaces the lower of middle index, according to whether
        * X is greater than the middle and the lookup table values are ascending or descending.
        */

        if ( ascending )
        {
            if (x >= xx[jm])
            {
                jl = jm;
            }
            else
            {
                ju = jm;
            }
        }
        else
        {
            if (x >= xx[jm])
            {
                ju = jm;
            }
            else
            {
                jl = jm;
            }
        }
    }

    if (x == xx[0])
    {
        /* Return first index of table */

        *j = 0;
    }
    else if (x == xx[n-1])
    {
        /* Return second to last index in table */

        *j = n-2;
    }
    else
    {
        /* Return the lower index */

        *j = jl;
    }

    return (status);
}



/* --------------------------------------------------------------------- */

/*+
 * Return first index in array BELOW or equal to the x value required.
 * This function also adjusts the given value until it is within the range
 * of the lookup table, assuming the lookup table is cyclic.
 * Assumes the lookup table is monotonic and has at least two entries
 *-
 */

static int gmosLutCycleLocate (
    double xx[],   /* Pointer to first entry in lookup table */
    int    n,      /* Number of entries in the lookup table */
    double x,      /* Value of x (given value) to locate */
    double *newx,  /* New value of X adjusted to be within range of lookup table */
    int    *j      /* Pointer to returned index */
    )
{
    int status;    /* Return status */
    int ju;        /* Upper index */
    int jm;        /* Middle index */
    int jl;        /* Lower index */
    int ascending; /* Table ascending or descending */

    double xrange; /* X range of lookup table */

    status = GMOSLUT_S_OK;

    jl = -1;       /* Initialise lower index */
    ju = n;        /* Initialise upper index */

   /*
    * Find if there are ascending or descending values in table,
    * assuming the table is monotonic.
    */

    ascending = (xx[n-1] >= xx[0]);
    xrange = fabs( xx[n-1] - xx[0] );

   /*
    * Check the lookup table has sufficient entries and the X range is valid.
    * Return a bad status if the lookup table is unacceptable.
    */

    if ( n < 2 )
    {
        /* Lookup table is too small */

        status = GMOSLUT_E_TOOSMALL;
        return (status);
    }

    if ( xrange < 1.0e-10 )
    {
        /* The lookup table has an almost zero range */
        status = GMOSLUT_E_LUTEQUAL;
        return (status);
    }

    /*
     * Adjust the X value to bring it with the range of the lookup table, assuming
     * the lookup table cycles endlessly for values outside its range.
     */

    if ( ascending )
    {
        if (x < xx[0])
        {
            /* Value is outside the lower index of the lookup table */

            *newx = xx[n-1] - fmod((xx[n-1] - x), xrange);
        }
        else if ( x > xx[n-1])
        {
            /* Value is outside the upper index of the lookup table */

            *newx = xx[0] + fmod((x - xx[0]), xrange);
        }
        else
        {
            /* Value is already within the range of the lookup table. */

            *newx = x;
        }
    }
    else
    {
        if (x > xx[0])
        {
             /* Value is outside the lower index of the lookup table */

             *newx = xx[n-1] + fmod((x - xx[n-1]), xrange);
        }
        else if ( x < xx[n-1])
        {
             /* Value is outside the upper index of the lookup table */

             *newx = xx[0] - fmod((xx[0] - x), xrange);
        }
        else
        {
             /* Value is already within the range of the lookup table. */

             *newx = x;
        }
    }

   /* Perform a binary search */

    while ((ju - jl) > 1)
    {

        /* Calculate the index value in the middle */

        jm = (ju + jl) / 2;

       /*
        * The next piece of code replaces the lower of middle index, according to whether the
        * new X is greater than the middle and the lookup table values are ascending or descending.
        */

        if ( ascending )
        {
            if (*newx >= xx[jm])
            {
                jl = jm;
            }
            else
            {
                ju = jm;
            }
        }
        else
        {
            if (*newx >= xx[jm])
            {
                ju = jm;
            }
            else
            {
                jl = jm;
            }
        }
    }

    if (*newx == xx[0])
    {
        /* Return first index of table */

        *j = 0;
    }
    else if (*newx == xx[n-1])
    {
        /* Return second to last index in table */

        *j = n-2;
    }
    else
    {
        /* Return the lower index */

        *j = jl;
    }

    return (status);
}


/* --------------------------------------------------------------------- */

/* Return a grating tilt angle for a given wavelength, order and ruling density */

int gratingSci2Tilt (
    const double wavelength,         /* Central wavelength in nm */
    const int    order,              /* Grating order */
    const double rulingDensity,      /* Grating ruling density /mm */
    double       *tiltAngle          /* Pointer grating tilt angle in degs */
    )
{
    int status;                      /* Status return */
    double psi;                      /* Angle between collimator and spectrometer */
    double psi0, psi1, psi2;         /* Running value of psi for linear interpolation */
    double wl;                       /* Wavelength in mm */

    int j;
    const double radDeg = 0.017453292; /* Conversion from degrees to radians */
    double degs90;                     /* 90 degrees in radians */

    double alpha, alpha1, alpha2;      /* Incidence angle (From grating normal)  */
                                       /* for linear interpolation               */
    double beta        ;               /* Diffraction angle (From grating normal) */
    double dangle;                     /* Step size for alpha */
    double temp;

    status = GMOSLUT_S_OK;

    /* Initialize constants */

    degs90 = 90.0 * radDeg;
    wl     = wavelength * 1.0e-6;         /* Convert from nm to mm */
    psi    = COL_TO_SPEC_ANGLE * radDeg;  /* Convert to radians */
    dangle = STEP_SIZE * radDeg;          /* Increment to use in radians */

    /* Initialize angles */

    psi0 = psi1 = psi2 = 0.0;
    alpha = alpha1 = alpha2 = 0.0;
    beta = 0.0;

    /* Increment alpha until solution found */

    j = 0;
    while ((psi0 < psi) && (alpha < degs90))
    {
        j++;
        alpha += dangle;          /* Increment alpha */

        temp = ((double) order * wl * rulingDensity) - sin(alpha);

        if (abs(temp) < 1.0)
        { 
            beta = asin(temp);
        }
        else
        {
            beta = 0.0;
        }

        psi0 = alpha - beta;

        if (psi0 > psi)
        {
            /* Value just above what we want */

            alpha2 = alpha;
            psi2 = psi0;
        }
        else
        {
            /* Value below what we want */

            alpha1 = alpha;
            psi1 = psi0;
        }
    }

    if ((alpha >= 90.0) || (fabs(beta) < 1.0e-6))
    {
        /* No solution found */

        status = GMOSLUT_E_NOSOLUTION;
    }

    /* Do linear interpolation */

    if (status == GMOSLUT_S_OK)
    {
        if (fabs(psi2-psi1) < 1.0e-6)
        {
            /* Potential divide by zero in linear interpolation */

            status = GMOSLUT_E_INTERPOLATION;
        }
        else
        {
            alpha = alpha1 + (alpha2-alpha1) * (psi-psi1)/(psi2-psi1);

            /* GMOS tilt angle is 90-alpha */

            *tiltAngle = 90.0 - alpha/radDeg;
        }
    }

    return status;
}

/* --------------------------------------------------------------------- */

/* Convert grating status code into null terminated error message string */
/* Message must be smaller than or equal to maxlength in size */

void gmosLutMessage (
    const int code,          /* Error code to be translated */
    const int maxlength,     /* Length of string buffer provided */
    char *message            /* Pointer to string buffer provided */
    )
{
   switch ( code )
   {
       case (GMOSLUT_S_OK):
           strncpy (message, "Success", maxlength-1);
           break;

       case GMOSLUT_E_TOOSMALL:
           strncpy (message, "Insufficient LUT entries", maxlength-1);
           break;

       case GMOSLUT_E_OUTRANGE:
           strncpy (message, "Value outside range of lookup table", maxlength-1);
           break;

       case GMOSLUT_E_INVLUT:
           strncpy (message, "Invalid lookup table entry", maxlength-1);
           break;

       case GMOSLUT_E_LUTEQUAL:
           strncpy (message, "Two or more LUT entries equal", maxlength-1);
           break;

       case GMOSLUT_E_LUTNOTMONO:
           strncpy (message, "Lookup table entries not monotonic", maxlength-1);
           break;

       case GMOSLUT_E_NOSOLUTION:
           strncpy (message, "No solution found", maxlength-1);
           break;

       case GMOSLUT_E_INTERPOLATION:
           strncpy (message, "Error during interpolation", maxlength-1);
           break;

       default:
           strncpy (message, "Unknown LUT status", maxlength-1);
           break;
    }

    return;
}

/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) 1999  				(c) 1999
 * National Research Council		Conseil national de recherches
 * Ottawa, Canada, K1A 0R6 		Ottawa, Canada, K1A 0R6
 * All rights reserved			Tous droits reserves
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
 * 					sation du logiciel.
 *
 ************************************************************************
 *
 * FILENAME
 * gmOiwfsCalibrate.c
 *
 * PURPOSE:
 * Computes calibration offsets from a set of calibration data points.
 *
 * FUNCTION NAME(S)  [ Global scope ]
 *
 *   None.
 *
 * FUNCTION NAME(S)  [ Local scope ]
 *
 * aveError         - Computes the average X and Y errors for the calibration
 *                    data points.
 *
 * evaluate         - Computes the sum of the squared errors for the
 *                    calibration data points.
 *
 * explore          - Takes steps, one dimension at a time, to try to find
 *                    a new minima.             
 *
 * minSearch        - Attempt to find the location of a function minimum.
 *                    In this case, it attempts to find the set of offsets
 *                    which provide the smallest sum of squares error.
 *
 * printProgress    - Prints error value and parameter values.
 *
 * readFile         - Reads a file of calibration data points.  The file
 *                    format allows comments lines (line must START with
 *                    a # character), and consists of one line per
 *                    calibration point with the values being in the following
 *                    order:
 *                            X      Y      Base Angle      Pickoff Angle
 *
 * readFilename     - Reads a filename from the user.
 *
 * skipLine         - Used to skip comment lines in the data file.
 *
 * ssq              - Function called by the minSearch function.  This
 *                    is simply a wrapper for evaluate.  The parameters
 *                    of ssq consist of the search space parameters
 *                    (the offsets), but the evaluation function calls
 *                    gmOiwfsSetOffsets() to set the parameters, then
 *                    calls evaluate with parameters indicating the number
 *                    of calibration points and whether to print the results.
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/

#ifndef vxWorks

/*
 *  Includes
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "gmOiwfsCalc.h"

#define  MAX_DIMENSIONS  4     /* Number of minimization parameters  */
#define  MAX_NUM  200          /* Max # of calibration points        */

#define  MIN_CAL_POINTS  5

#if (MIN_CAL_POINTS <= 4)
#error  MIN_CAL_POINTS must be at least 5.
#endif

#define  ERR_INVALID_CAL_FILE   -1
#define  ERR_HELP_REQUESTED     -2
#define  ERR_UNKNOWN_PARAM      -3
#define  ERR_INSUFFICIENT_DATA  -4
#define  ERR_INVALID_ANGLE      -5
#define  ERR_NOT_CONVERGED      -6

#define  MIN_ANGLE_OFFSET   -360.0
#define  MAX_ANGLE_OFFSET    360.0

#define  MIN_XY_OFFSET   -20.0
#define  MAX_XY_OFFSET    20.0

static double xPos[MAX_NUM];
static double yPos[MAX_NUM];
static double base[MAX_NUM];
static double pickoff[MAX_NUM];

static int    numCalPts;
static int    traceFlag;
 
/*
 *  Function Prototypes (local).
 */

static void aveError(double * xErr, double * yErr);

static double evaluate(int n, int print);

static double explore(int nDim, double (*f)(int, double []),
                     double x[], double dx[], double oldMin, int * nEvals);

static int minSearch(int nDim, double (*f)(int, double []),
                     double x[], double dx[], double epsilon[],
                     double rho, int maxEval);

static void printProgress(double errSSq, double params[], int label, int mod);

static int readFile(const char * filename);

static void readFilename(char * fname, int bufferSize);

static void skipLine(FILE * fp);

static double ssq(int n, double X[]);


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * aveError
 *
 * INVOCATION:
 * aveError(&xError, &yError);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  (<) xErr (double)  Average error in X dimension.
 *  (<) yErr (double)  Average error in Y dimension.
 *
 * FUNCTION VALUE:
 * (void)   Not applicable.
 *
 * PURPOSE:
 *   Provide a check of the X and Y calibration offsets (the average
 *   error should be small if the calibration offsets after minimization).
 *
 * DESCRIPTION:
 *   For each calibration point, the known X and Y values are compared to
 *   their estimated values from the measured angles.   The differences
 *   between the known X (and Y) values and the estimated X (and Y) values
 *   is averaged.
 *
 * EXTERNAL VARIABLES:
 *   xPos, yPos, base, pickoff, numCalPts
 *
 * PRIOR REQUIREMENTS:
 *   Calibration data must be valid (previously read from file).  Only
 *   makes sense to call after the minimization has determined a set of
 *   good offsets.
 * 
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */


static void aveError
(
    double * xErr,   /* (out)  Average error in the X direction */
    double * yErr    /* (out)  Average error in the Y direction */
)
{
    double xt;
    double yt;
    double zt;
    double probeAngle;
    int i;


    /*
     *  Initialize the total errors to zero then loop through all
     *  of the calibration data points.
     */

    *xErr = 0;
    *yErr = 0;

    for (i=0; i < numCalPts; ++i) {


        /*
         *  Calculate the X, Y and Z probe position corresponding
         *  to the measured base and angle for calibration point i.
         */

        (void) gmOiwfsCalculateProbePosition(base[i], pickoff[i], 
					     &xt, &yt, &zt, &probeAngle);


        /*
         *  Update the total X and Y error.
         */

        *xErr += xt - xPos[i];
        *yErr += yt - yPos[i];
    }


    /*
     *  Scale the errors to get averages.
     */

    *xErr = *xErr / numCalPts;
    *yErr = *yErr / numCalPts;
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * evaluate
 *
 * INVOCATION:
 * evaluate(numCalPts, 0);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *   (>) n     (int)  Number of data points in calibration set.
 *   (>) print (int)  Print details (0 = no, nonzero = yes)
 *
 * FUNCTION VALUE:
 *   (double)   Sum of squares of "positioning" errors.
 *
 * PURPOSE:
 *   Evaluates the total (squared) error of the calibration data set
 *   using the current calibration offsets.
 *
 * DESCRIPTION:
 *   For each of the calibration points, the measured base and pickoff
 *   angles are used to compute the estimated X,Y positions.  These
 *   estimated positions are compared with the known X,Y positions.
 *   The total of the squared distances between estimated and known
 *   X,Y positions is returned.
 *
 * EXTERNAL VARIABLES:
 *   xPos, yPos, base, pickoff
 *
 * PRIOR REQUIREMENTS:
 *   Calibration data must be valid (previously read from file).
 * 
 * DEFICIENCIES:
 *   None.
 *-
 ************************************************************************
 */


static double evaluate(int n, int print)
{
    double err;           /* Total squared error.                   */
    double dx;            /* Error of data point i in X direction.  */
    double dy;            /* Error of data point i in Y direction.  */
    double d2;            /* Squared error of data point i.         */
    double xt, yt, zt;    /* X,Y,Z position estimated from angles.  */
    double probeAngle;
    int i;


    if (print != 0)
    {
        printf("\n%10s  %10s    %10s  %10s  %10s\n",
                "Base", "Pickoff", "Target", "Calculated", "Error");
    }

    /*
     * For each of the calibration points, determine the X,Y position
     * corresponding to the measured base and pickoff angles.  Add the
     * squared distance between this X,Y location and the known X,Y
     * location to the sum of squares error.
     */

    err = 0;
    for (i=0; i < n; ++i) {

        /*
         * Using the current set of offset parameters, determine the 
         * X,Y position (xt,yt) corresponding to the measured
         * base and pickoff angles.
         */

        (void) gmOiwfsCalculateProbePosition(base[i], pickoff[i], 
                                             &xt, &yt, &zt, &probeAngle);

        /*
         * Add the error from this point to the total error.
         */

        dx = xt - xPos[i];
        dy = yt - yPos[i];
        d2 = dx * dx + dy * dy;
        err = err + d2;


        /*
         * If required, print the details of the calibration point.
         */

        if (print != 0) {
            printf("%10.3f  %10.3f", 
               base[i] * 180.0 / M_PI, pickoff[i] * 180.0 / M_PI);
            printf("    X: %7.3f --> %7.3f  %10.6f\n",
                xPos[i], xt, sqrt(d2));
            printf("%28s %7.3f --> %7.3f\n", "Y:", yPos[i], yt);
        }
    }

    return err;
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * explore
 *
 * INVOCATION:
 * minValue = explore(4, ssq, offsets, delta, minDelta, &nFuncs);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *   (>) n     (int)  Number of data points in calibration set.
 *   (>) print (int)  Print details (0 = no, nonzero = yes)
 *
 * FUNCTION VALUE:
 *   (double)   Minimum "nearby" sum of 
 *
 * PURPOSE:
 *   Evaluates the total (squared) error of the calibration data set
 *   using the current calibration offsets.
 *
 * DESCRIPTION:
 *   For each of the calibration points, the measured base and pickoff
 *   angles are used to compute the estimated X,Y positions.  These
 *   estimated positions are compared with the known X,Y positions.
 *   The total of the squared distances between estimated and known
 *   X,Y positions is returned.
 *
 * EXTERNAL VARIABLES:
 *   xPos, yPos, base, pickoff
 *
 * PRIOR REQUIREMENTS:
 *   Calibration data must be valid (previously read from file).
 * 
 * SEE ALSO:
 *   minSearch, ssq
 *
 * DEFICIENCIES:
 *   None.
 *-
 ************************************************************************
 */

static double explore
(
    int nDim, 
    double (*f)(int, double []),     /* Pointer to function being solved. */
    double x[], 
    double dx[],
    double oldMin,
    int    * nEvals
)
{
    double newMin;
    double fx;
    double original;
    int i;

    /*
     *  Minimum function value found so far.
     */

    newMin = oldMin;

    /*
     *  For each dimension, try to make a step that reduces the value
     *  of the function.
     */

    for (i=0; i < nDim; ++i)
    {
        /*
         *  Save current value of parameter i.
         */

        original = x[i];

        /*
         *  Try a step of size dx[i] in the i dimension.
         */

        x[i] = x[i] + dx[i];

        fx = (*f)(nDim, x);

        ++(*nEvals);

        /*
         *  If the function got smaller after the dx[i] step,
         *  then update we have a new minimum.
         */

        if (fx < newMin)
        {
            newMin = fx;
        }
        else
        {
            /*
             *  Function did not decrease after the the dx[i] step,
             *  so try a step in the opposite direction.
             */

            x[i] = original - dx[i];

            fx = (*f)(nDim, x);
            ++(*nEvals);


            /*
             *  If the function decreased during the -dx[i] step,
             *  then record the new minimum, otherwise restore x[i]
             *  back to its original value.
             */

            if (fx < newMin)
            {
                dx[i] = -dx[i];
                newMin = fx;
            }
            else
            {
                x[i] = original;
            }
        }

    }

    return newMin;
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * minSearch
 *
 * INVOCATION:
 * nEvals = minSearch(4, ssq, offsets, steps, minSteps, 0.5, 10000);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  (>) nDim    (int)          Number of dimensions in search space.
 *  (>) f       (ptr to func)  Pointer to function to be minimized.
 *  (!> x       (double [])    On entry, location of search starting point.
 *                             On exit, location of function minima.
 *  (!) dx      (double [])    On entry, initial step sizes.
 *                             On exit, final step sizes used.
 *  (>) epsilon (double [])    Smallest allowable step sizes.
 *  (>) rho     (double)       Step size shrink factor.
 *  (>) maxEval (int)          Maximum allowable number of function evals.
 *
 * FUNCTION VALUE:
 * (int)   Number of function evaluations required.
 *
 * PURPOSE:
 *   Attempt to minimize the objective function.  In this case, the
 *   objective function is the sum of squares error of the calibration
 *   data points.
 *
 * DESCRIPTION:
 *   See function body and references for details.  The code is based
 *   on the Algol code given in the paper by Bell and Pike (see refs),
 *   which is based on work by Kaupe (1963) and Hooke and Jeeves (1961).
 *
 *   The code from Bell and Pike has been translated into structured C,
 *   and some minor improvements have been made.
 *
 * EXTERNAL VARIABLES:
 *   (see evaluate)
 *
 * PRIOR REQUIREMENTS:
 *   To minimize the errors in the calibration points, the calibration
 *   data must be valid (previously loaded from file).
 * 
 * SEE ALSO:
 *   ssq, evaluate, explore
 *
 * REFERENCES:
 *
 *   Hooke, Robert and Jeeves, T.A.
 *   "Direct Search" Solution of Numerical and Statistical Problems,
 *   Journal of the ACM, Vol. 8, No. 2, 1961.   pp. 212-229
 *         (this paper is quite long winded)
 *
 *   Kaupe, Arthur F., Jr.
 *   Algorithm 178 : Direct Search
 *   Communications of the ACM, Vol. 6, No. 6, 1963.  pp. 313-314
 *
 *   Bell, M. and Pike, M.C.
 *   Remark on Algorithm 178 : Direct Search
 *   Communications of the ACM, Vol 9, No. 9, 1966.  pp. 684-685
 *
 * DEFICIENCIES:
 *   WARNING : This function, like most minimization software, is 
 *             NOT GUARANTEED to find a minimum of the objective function.
 *             It incorrectly converge on a local minima.  Running the
 *             program from different starting values is a useful way
 *             to test the robustness of the "minimum".
 *-
 ************************************************************************
 */


static int minSearch
(
    int nDim,
    double (*f)(int, double []),     /* Pointer to function being solved. */
    double x[],
    double dx[],
    double epsilon[],
    double rho,
    int    maxEval
)
{
    double old[MAX_DIMENSIONS];
    double oMin;
    double exploreMin;
    double temp;
    int    nEvals;
    int    i;
    int    unconverged;
    int    traceLines;

    /*
     *  Make sure that the search space is not too large.
     */

    if (nDim > MAX_DIMENSIONS)
    {
        printf("\nError : ndim = %d > %d = MAX_DIMENSIONS.\n", 
               nDim, MAX_DIMENSIONS);
        return 0;
    }

    /*
     *  Copy the starting search position into the old array.
     */

    for (i=0; i < nDim; ++i)
    {
        old[i] = x[i];
    }

    oMin = (*f)(nDim, x);

    nEvals = 1;
    traceLines = 0;

    do
    {

        for (i=0; i < nDim; ++i)
        {
            x[i] = old[i];
        }

        exploreMin = explore(nDim, f, x, dx, oMin, &nEvals);

        /*
         *  If explore found a new minima, then try another step
         *  in the same direction.
         */

	    while (exploreMin < oMin)
	    {
            if (traceFlag != 0)
            {
                if ((traceLines % 50) == 0)
                {
                    printProgress(exploreMin, x, 1, 0);
                }
                else
                {
                    printProgress(exploreMin, x, 0, 0);
                }
                ++traceLines;
            }

            /*
             *  For each of the dimensions, point the step size in
             *  the direction of the last move, and ...
             */

            for (i=0; i < nDim; ++i)
            {
                if (x[i] < old[i])
                {
                    dx[i] = -fabs(dx[i]);
                }
                else
                {
                    dx[i] = fabs(dx[i]);
                }

                temp = old[i];
                old[i] = x[i];
                x[i] = x[i] + (x[i] - temp);
            }


            /*
             *  Go exploring for a new minima after taking a tentative
             *  step in the same direction as the last one.
             */

            oMin = exploreMin;
            exploreMin = explore(nDim, f, x, dx, oMin, &nEvals);
        }


        /*
         *  Scale the step sizes by a factor of rho, unless the steps size
         *  is already at below its minimum.
         */

        unconverged = 0;

        for (i=0; i < nDim; ++i)
        {
            if (fabs(dx[i]) >= epsilon[i])
            {
                ++unconverged;
                dx[i] = rho * dx[i];
            }
        }

    } while ((nEvals < maxEval) && (unconverged > 0));


    /*
     *  The most recent "best" location is stored in "old", so copy it
     *  into the output solution array.
     */

    for (i=0; i < nDim; ++i)
    {
        x[i] = old[i];
    }

    if (unconverged > 0)
    {
        nEvals = -nEvals;
    }

    return nEvals;
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * readFile
 *
 * INVOCATION:
 *   readFile();
 *
 * PARAMETERS: 
 *  (>) filename  (char *)    Name of datafile, or NULL if filename is
 *                            to be read by the user.
 *
 * FUNCTION VALUE:
 *   (int)  Number of data points read, or 0 if unsuccessful.
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *   numCalPts
 *
 * PRIOR REQUIREMENTS:
 *   Calibration data must be valid (previously read from file).
 * 
 * SEE ALSO:
 *   minSearch, ssq
 *
 * DEFICIENCIES:
 *   None.
 *-
 ************************************************************************
 */


static int readFile
(
    const char * filename
)
{
    FILE * fp;
    int    n;
    int    c;
    int    line;
    int    success;
    char   fname[FILENAME_MAX];


    /*
     *  Read the filename from the user and open the file.
     */

    if (filename != (char *) NULL)
    {
        fp = fopen(filename, "r");

        if (fp == (FILE *) NULL)
        {
            printf("\nCould not read file \"%s\".\n", filename);
            return 0;
        }
        else
        {
            strncpy(fname, filename, FILENAME_MAX);
            fname[FILENAME_MAX-1] = (char) 0;
        }
    }
    else
    {
        printf("\nWhat file contains the calibration measurements? ");
        readFilename(fname, FILENAME_MAX);

        fp = fopen(fname, "r");
    }


    /*
     *  If the file could not be opened, then get a new filename
     *  and try again.  Keep trying until we can open the file.
     */

    while (fp == (FILE *) NULL)
    {
        printf("\nCould not open file \"%s\".\n", fname);
        printf("\nPlease enter the calibration measurements filename: ");
        readFilename(fname, FILENAME_MAX);
        fp = fopen(fname, "r");
    }


    /*
     *  No data points read yet, but we are about to read line #1 of file.
     */

    n = numCalPts;
    line = 1;


    /*
     *  Print header for data file.
     */

    printf("\n\nReading data file: %s\n", fname);

    printf("\n%10s  %10s  %10s  %10s\n", 
           " X Pos.", "Y Pos.", "Base", "Pickoff");

    /*
     *  Keep reading data until the end of file is found. 
     */

    while (! feof(fp))
    {

        /*
         *  Get a character and check for a comment character.
         */

        c = fgetc(fp);

        if (c == '#')
        {

            /*
             *  Comment character was found, so skip rest of the line.
             */

            skipLine(fp);
            ++line;
        }
        else if (c == EOF)
        {

            /*
             *  If we just hit the end of file then leave loop.
             */

            break;
        }
        else
        {

            /*
             *  Character read was not comment character so put it
             *  back then read the four data values.
             */

            ungetc(c, fp);
            success = fscanf(fp, "%lf %lf %lf %lf", &xPos[n], &yPos[n], 
                                                    &base[n], &pickoff[n]);


            /*
             *  Check if all data was read was successfully.
             */

            if (success != 4)
            {

                /*
                 *  We could not read all four data values.
                 *  Print error message.
                 */

                printf("\nError: Unable to read line %d.\n", line);
                printf("\nSkipping line and attempting to continue...\n");
            }
            else
            {

                /*
                 *  Read was successful, so print the data on the
                 *  screen, convert angles from degrees into radians,
                 *  and increment the number of data points read.
                 */

                printf("%10.4f  %10.4f  %10.4f  %10.4f\n", 
                          xPos[n], yPos[n], base[n], pickoff[n]);

                base[n] = base[n] * M_PI / 180.0;
                pickoff[n] = pickoff[n] * M_PI / 180.0;

                ++n;
            }


            /*
             *  Regardless of whether read was successful,
             *  skip rest of input line and increment the line counter.
             */

            skipLine(fp);
            ++line;
        }
    }

    fclose(fp);

    return (n - numCalPts);
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * readFilename
 *
 * INVOCATION:
 *   readFilename(filename, FILENAME_MAX);
 *
 * PARAMETERS: 
 *  (<) fname      (char *)   Buffer to hold filename read from user.
 *  (>) bufferSize (int)      Size of fname buffer.
 *   
 *
 * FUNCTION VALUE:
 *   (int)  Number of data points read.
 *
 * PURPOSE:
 *   Safely read a filename from the user.
 *
 * DESCRIPTION:
 *   Reads a filename from the user using fgets (to avoid buffer overflow),
 *   then remove the trailing newline character which fgets leaves.
 *
 * EXTERNAL VARIABLES:
 *   None.
 *
 * PRIOR REQUIREMENTS:
 *   None.
 * 
 * SEE ALSO:
 *   None.
 *
 * DEFICIENCIES:
 *   None.
 *-
 ************************************************************************
 */



static void readFilename
(
    char * fname, 
    int bufferSize
)
{
    int  len;


    /*
     *  Read the filename from the user (use fgets since gets is not
     *  safe from buffer overruns (this is such a common source of
     *  problems (especially root exploits) that FreeBSD even warns when
     *  a program uses gets!)).
     */

    fgets(fname, bufferSize, stdin);


    /*
     *  Remove the trailing newline character (if any).
     */

    len = strlen(fname);

    if (fname[len-1] == '\n')
    {
        fname[len-1] = (char) 0;
    }
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * skipLine
 *
 * INVOCATION:
 *   skipLine(fp);
 *
 * PARAMETERS: 
 *  (>) fp      (FILE *)     File handle to input file.
 *   
 * FUNCTION VALUE:
 *   None
 *
 * PURPOSE:
 *   Skips over the current line of input.
 *
 * DESCRIPTION:
 *   Reads characters from file fp until a newline character is found.
 *
 * EXTERNAL VARIABLES:
 *   None.
 *
 * PRIOR REQUIREMENTS:
 *   None.
 * 
 * SEE ALSO:
 *   None.
 *
 * DEFICIENCIES:
 *   None.
 *-
 ************************************************************************
 */

static void skipLine
(
    FILE * fp
)
{
    int c;


    /*
     *  Make sure that the program is well behaved if end-of-file is found.
     */

    while (! feof(fp))
    {

        /*
         *  Read a character and leave if it is the newline character.
         */

        c = getc(fp);
        if (c == '\n') return;
    }
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * ssq
 *
 * INVOCATION:
 *   minima = ssq(4, offsets);
 *
 * PARAMETERS: 
 *  (>) n    (int)        Dimension of search space.
 *  (>) X    (double *)   n-dimensional location within search space.
 *   
 * FUNCTION VALUE:
 *   (double)  Value of objective function at location X.
 *
 * PURPOSE:
 *   Evaluates the objective function at the current location within
 *   the search space.
 *
 * DESCRIPTION:
 *   Transfers the location in search space (X) into the gmOiwfsCalc
 *   calibration offsets, then calculates the sum of the squared deviations
 *   between the known and the computed probe positions.
 *
 * EXTERNAL VARIABLES:
 *   numCalPts, xPos, yPos, base, pickoff
 *
 * PRIOR REQUIREMENTS:
 *   Calibration data must be valid (previously read from file).
 * 
 * SEE ALSO:
 *   None.
 *
 * DEFICIENCIES:
 *   None.
 *-
 ************************************************************************
 */


static double ssq
(
    /*@ unused @*/ int n, 
    double X[]
)
{
    gmOiwfsSetOffsets(X[0], X[1], X[2], X[3], 0.0);
    return evaluate(numCalPts, 0);
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * printHelp
 *
 * INVOCATION:
 *   printHelp();
 *
 * PARAMETERS: 
 *   None.
 *   
 * FUNCTION VALUE:
 *   None
 *
 * PURPOSE:
 *   Prints a list of allowable command line options.
 *
 * DESCRIPTION:
 *   Prints a list of allowable command line options.
 *
 * EXTERNAL VARIABLES:
 *   None.
 *
 * PRIOR REQUIREMENTS:
 *   None.
 * 
 * SEE ALSO:
 *   None.
 *
 * DEFICIENCIES:
 *   None.
 *-
 ************************************************************************
 */


static void printHelp
(
    void
)
{
    printf("\ngmOiwfsCalibrate command line options:\n");
    printf("\n-b #          Set starting base offset to #");
    printf("\n-f fname      Use calibration file fname");
    printf("\n-h            Print this help information");
    printf("\n-p #          Set starting pickoff offset to #");
    printf("\n-t            Turn on optimization tracing");
    printf("\n-x #          Set starting X offset to #");
    printf("\n-y #          Set starting Y offset to #");
    printf("\n-?            Print this help information");
    printf("\n\n");
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * getInitialValueArg
 *
 * INVOCATION:
 *   ok = getInitialValueArg(argv[i], offsets[0]);
 *
 * PARAMETERS: 
 *  (>) arg    (char *)      Pointer to command line argument
 *  (<) param  (double *)    Pointer to corresponding parameter
 *   
 *   
 * FUNCTION VALUE:
 *   (int)  Returns 1 if argument could be converted to a double, else 0.
 *
 * PURPOSE:
 *   Reads parameter value from command line.
 *
 * DESCRIPTION:
 *   Attempts to convert command line argument arg into a double,
 *   and store it in *param.
 *
 * EXTERNAL VARIABLES:
 *   None.
 *
 * PRIOR REQUIREMENTS:
 *   None.
 * 
 * SEE ALSO:
 *   None.
 *
 * DEFICIENCIES:
 *   None.
 *-
 ************************************************************************
 */

static int getInitialValueArg
(
    const char * arg, 
    double * param
)
{
   int ok;
   ok = sscanf(arg, "%lf", param);
   if (ok == 0)
   {
       printf("\nInvalid parameter value: %s\n", arg);
   }
   return ok;
}



/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * readInitialValue
 *
 * INVOCATION:
 *   getInitialValue("Please enter starting base angle", offsets[0],
                      180.0, MIN_ANGLE_OFFSET, MAX_ANGLE_OFFSET);
 *
 * PARAMETERS: 
 *  (>) prompt  (char *)     Prompt string
 *  (<) param  (double *)    Pointer to parameter to be read
 *  (>) defValue  (double)   Default parameter value
 *  (>) minValue  (double)   Minimum parameter value
 *  (>) maxValue  (double)   Maximum parameter value
 *   
 * FUNCTION VALUE:
 *   None.
 *
 * PURPOSE:
 *   Reads a parameter value from the user.
 *
 * DESCRIPTION:
 *   Reads a parameter value from the user.  Performs range checking
 *   on the parameters to ensure the values are reasonable.
 *
 * EXTERNAL VARIABLES:
 *   None.
 *
 * PRIOR REQUIREMENTS:
 *   None.
 * 
 * SEE ALSO:
 *   None.
 *
 * DEFICIENCIES:
 *   None.
 *-
 ************************************************************************
 */

static void readInitialValue
(
    const char * prompt, 
    double * param, 
    double defValue,
    double minValue,
    double maxValue
)
{
    int ok;
    char buff[40];
    char * s;

    do {
        printf("\n%s [ %5.3f ]: ", prompt, defValue);
        fgets(buff, 40, stdin);
        ok = sscanf(buff, "%lf", param);
        if (ok == 1)
        {
            if (*param < minValue)
            {
                printf("\nValue too small.  Minimum value = %9.3f\n", minValue);
                ok = 0;
            }
            else if (*param > maxValue)
            {
                printf("\nValue too large.  Maximum value = %9.3f\n", maxValue);
                ok = 0;
            }
        }
        else
        {
            s = buff;
            while (isspace(*s))
            {
                ++s;
            }
            if (*s == (char) 0)
            {
                *param = defValue;
                ok = 1;
            }
        }
    } while (ok == 0);
}



static void printProgress
(
    double errSSq,
    double params[],
    int    label,
    int    mod
)
{
    double sdev;
    double baseAngle;
    double pickoffAngle;

    if (label != 0)
    {
        printf("\n%15s  %12s  %12s  %12s  %12s\n",
                 "St. Dev", "Base", "Pickoff", "X", "Y");
    }


    /*
     *  Divide the sum of the squared errors by the degrees of freedom.
     */

    sdev = sqrt(errSSq / (numCalPts - 4));


    /*
     *  Convert base and pickoff angles from radians into degrees.
     *  Map into -180..180 if mod flag is set.
     */

    if (mod == 0)
    {
        baseAngle = params[0] * 180.0 / M_PI;
    }
    else
    {
        baseAngle = fmod(3.0 * M_PI + params[0], 2.0 * M_PI) - M_PI;
        baseAngle = baseAngle * 180.0 / M_PI;
    }


    if (mod == 0)
    {
        pickoffAngle = params[1] * 180.0 / M_PI;
    }
    else
    {
        pickoffAngle = fmod(3.0 * M_PI + params[1], 2.0 * M_PI) - M_PI;
        pickoffAngle = pickoffAngle * 180.0 / M_PI;
    }


    /*
     *  Print the standard deviation and the calibration offsets.
     */

    printf("%15.3f  %12.6f  %12.6f  %12.6f  %12.6f\n",
           sdev,  baseAngle, pickoffAngle, params[2], params[3]);
}


static double convertAngle
(
    double degrees
)
{

    /*
     *  Ensure that angle is within -180..180 range.
     */

    degrees = fmod(degrees + 540.0, 360) - 180.0;


    /*
     *  Return angle in radians.
     */

    return (degrees * M_PI / 180.0);
}


int main
(
    int argc, 
    char * argv[]
)
{
    double offsets[4];
    double stepSize[4];
    double minStep[4];
    double xErr, yErr;
    double err;
    int    fileArg, baseArg, pickoffArg, xArg, yArg;
    int    i;
    int    npts;
    int    nEvals;
  


    /*
     *  Initialize (clear) the flags.
     */

    numCalPts = 0;
    traceFlag = 0;
    fileArg = 0;
    baseArg = 0;
    pickoffArg = 0;
    xArg = 0;
    yArg = 0;


    /*
     *  Process each of the command line parameters.
     */

    for (i=1; i < argc; ++i)
    {

        /*
         *  Check for data file specified on command line.
         */

        if (strcmp(argv[i], "-f") == 0)
        {

            /*
             *  Advance parameter index and read the specified file.
             */

            ++i;
            npts = readFile(argv[i]);


            /*
             *  If no new data points were read, then print an
             *  error message, otherwise update the number of
             *  data points in the calibration data set and set
             *  the fileArg flag.
             */

            if (npts == 0)
            {
                printf("\nNo valid data read from file \"%s\".\n", argv[i]);
                return ERR_INVALID_CAL_FILE;
            }
            else
            {
                fileArg = 1;
                numCalPts = numCalPts + npts;
            }
        }


        /*
         *  Print a simple help screen if requested.
         */

        else if ( (strcmp(argv[i], "-help") == 0) 
                || (strcmp(argv[i], "-h") == 0)
                || (strcmp(argv[i], "-?") == 0))
        {
            printHelp();
            return ERR_HELP_REQUESTED;
        }


        /*
         *  Enable trace mode.
         */

        else if (strcmp(argv[i], "-t") == 0) 
        {
            traceFlag = 1;
        }



        /*
         *  Read initial base angle offset from command line?
         */

        else if (strcmp(argv[i], "-b") == 0)
        {
            ++i;
            baseArg = getInitialValueArg(argv[i], &offsets[0]);

            if ((baseArg == 1) && ((offsets[0] < MIN_ANGLE_OFFSET) 
                || (offsets[0] > MAX_ANGLE_OFFSET)))
            {
                printf("\nInvalid base angle.");
                printf("\nAngles must be in range %9.3f ... %9.3f\n",
                       MIN_ANGLE_OFFSET, MAX_ANGLE_OFFSET);
                return ERR_INVALID_ANGLE;
            }
        }


        /*
         *  Read initial pickoff angle offset from command line?
         */

        else if (strcmp(argv[i], "-p") == 0)
        {
            ++i;
            pickoffArg = getInitialValueArg(argv[i], &offsets[1]);

            if ((pickoffArg == 1) && ((offsets[1] < MIN_ANGLE_OFFSET) 
                 || (offsets[1] > MAX_ANGLE_OFFSET)))
            {
                printf("\nInvalid pickoff angle.");
                printf("\nAngles must be in range %9.3f ... %9.3f\n",
                       MIN_ANGLE_OFFSET, MAX_ANGLE_OFFSET);
                return ERR_INVALID_ANGLE;
            }
        }


        /*
         *  Read initial X offset from command line?
         */

        else if (strcmp(argv[i], "-x") == 0)
        {
            ++i;
            xArg = getInitialValueArg(argv[i], &offsets[2]);

            if ((offsets[2] < MIN_XY_OFFSET) || (offsets[2] > MAX_XY_OFFSET))
            {
                printf("\nInvalid X offset.");
                printf("\nX offset must be in range %9.3f ... %9.3f\n",
                       MIN_XY_OFFSET, MAX_XY_OFFSET);
                return ERR_INVALID_ANGLE;
            }
        }


        /*
         *  Read initial Y offset from command line?
         */

        else if (strcmp(argv[i], "-y") == 0)
        {
            ++i;
            yArg = getInitialValueArg(argv[i], &offsets[3]);

            if ((offsets[3] < MIN_XY_OFFSET) || (offsets[3] > MAX_XY_OFFSET))
            {
                printf("\nInvalid Y offset.  \n");
                printf("\nY offset must be in range %9.3f ... %9.3f\n",
                       MIN_XY_OFFSET, MAX_XY_OFFSET);
                return ERR_INVALID_ANGLE;
            }
        }


        /*
         *  User specified an invalid command line option.
         */

        else
        {
            printf("\nUnknown parameter: \"%s\"\n", argv[i]);
            printHelp();

            return ERR_UNKNOWN_PARAM;
        }
    }


    /*
     * Read the name of the data file from the user (if the filename
     * was not specified on the command line), then read the calibration 
     * measurements from the data file.
     */

    if (fileArg == 0)
    {
        printf("\nIn order to determine the calibration coefficients,");
        printf("\nthis program requires a file containing the calibration");
        printf("\ndata, with each line of the file corresponding to one");
        printf("\ncalibration point with values listed in the order");
        printf("\n\nX position, Y position, Base angle and Pickoff angle.");
        printf("\n\nA \"#\" character at the start of a line can be used");
        printf("\nto make that line a comment.  A sample data file is:\n");
        printf("\n\n# X      Y     Base        Pickoff");
        printf("\n  0.0   0.0   -25.7414     34.3248");
        printf("\n 25.4   0.0   -45.0376     60.5096");
        printf("\n 50.8   0.0   -60.7874     80.8316");
        printf("\n 76.2   0.0   103.716    -123.252");
        printf("\n 25.4   0.0    65.2530    -84.5524");
        printf("\n  0.0   0.0    43.1956    -58.3656");
        printf("\n-12.7   0.0    27.1722    -37.8460");
        printf("\n  0.0   0.0   -25.7404     34.3240");
        printf("\n 50.8   0.0    84.3028   -104.898");
        printf("\n 76.2   0.0   -76.0178     99.1484");
        printf("\n-12.7   0.0   -10.8972     13.8408");
        printf("\n  0.0   0.0    43.1956    -58.3656");
        printf("\n");

        numCalPts = readFile((const char *) NULL);
    }

    printf("\n");



    /*
     *  Check that there are enought calibration points to make an
     *  optimization meaningful.
     */

    if (numCalPts < MIN_CAL_POINTS)
    {
        printf("\nInsufficient data points.\n");
        printf("Must have at least %d data points.\n", MIN_CAL_POINTS);
        return ERR_INSUFFICIENT_DATA;
    }



    /*
     *  Determine the user specified starting base offset.
     */

    if (baseArg == 0)
    {
        readInitialValue("Enter initial base offset angle in degrees",
                         &offsets[0], -180.0, 
                         MIN_ANGLE_OFFSET, MAX_ANGLE_OFFSET);
    }


    /*
     *  Convert base starting angle from degrees to radians.
     *  Set starting base step size and minimum step size.
     */

    offsets[0] = convertAngle(offsets[0]);
    stepSize[0] = M_PI / 180.0;                /* 1 degree */
    minStep[0] = 1.0e-9 * stepSize[0];


    /*
     *  Determine the user specified starting pickoff offset.
     */

    if (pickoffArg == 0)
    {
        readInitialValue("Enter initial pickoff offset angle in degrees",
                         &offsets[1], 0.0, 
                         MIN_ANGLE_OFFSET, MAX_ANGLE_OFFSET);
    }


    /*
     *  Convert pickoff starting angle from degrees to radians.
     *  Set starting pickoff step size and minimum step size.
     */

    offsets[1] = convertAngle(offsets[1]);

    stepSize[1] = M_PI / 180.0;                /* 1 degree */
    minStep[1] = 1.0e-9 * stepSize[1];

    offsets[1] = fmod(3.0 * M_PI + offsets[1], 2.0 * M_PI) - M_PI;

    /*
     *  Determine the user specified starting X offset.
     *  Set starting X step size and minimum step size.
     */

    if (xArg == 0)
    {
        readInitialValue("Enter initial X offset in mm",
                         &offsets[2], 0.0, MIN_XY_OFFSET, MAX_XY_OFFSET);
    }

    stepSize[2] = 1.0;
    minStep[2] = 1.0e-9;


    /*
     *  Determine the user specified starting Y offset.
     *  Set starting Y step size and minimum step size.
     */

    if (yArg == 0)
    {
        readInitialValue("Enter initial Y offset in mm",
                         &offsets[3], 0.0, MIN_XY_OFFSET, MAX_XY_OFFSET);
    }

    stepSize[3] = 1.0;
    minStep[3] = 1.0e-9;


    /*
     * Ensure that the accurate position calculation is used.
     */

    gmOiwfsStageTiltCorrection = 1;


    /*
     * As a reference point, print the sum of the squared residuals
     * using the initial offset values.
     */

    gmOiwfsSetOffsets(offsets[0], offsets[1], offsets[2], offsets[3], 0.0);

    printf("\nStarting offsets:\n");

    err = evaluate(numCalPts, 0);

    printProgress(err, offsets, 1, 0);

    /*
     * Search for a minimum using the algorithm of Hooke and Jeeves.
     */

    printf("\n\nOptimizing...\n");

    nEvals = minSearch(4, ssq, offsets, stepSize, minStep, 0.9, 10000);

    /*
     * Print the results of the minimization.
     */

    printf("\n\nResults of minimization search (calibration data set):\n");

    err = evaluate(numCalPts, 1);


    printf("\n\nOptimized offsets:\n");
    printProgress(err, offsets, 1, 1);


    /*
     * Print the average X and Y error.
     */

    aveError(&xErr, &yErr);

    printf("\nAverage errors: %10.3f  %10.3f\n", xErr, yErr);


    /*
     *  Return 0 if successful, ERR_NOT_CONVERGED if minimization
     *  terminated without achieving convergence.
     */

    if (nEvals > 0)
    {
        return 0;
    }
    {
        return ERR_NOT_CONVERGED;
    }
}
#endif

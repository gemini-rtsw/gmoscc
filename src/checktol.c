static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id$"};

/*
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
 * checktol.c
 *
 * PURPOSE:
 * Report if two numbers are within a tolerance of each other.
 *
 * INVOCATION:
 * checktol val1 val2 toler
 *
 * EXAMPLES:
 * set xpos = `checktol $xpos $xtolerance`
 *
 * DESCRIPTION:
 * This Unix program is designed to be called from a C shell script
 * when there is a need to check whether two floating point variable are
 * the same to within a defined tolerance. The first two arguments supplied to
 * the program on the command line are the two values, and the third argument
 * is the tolerance. If the numbers are with tolerance a 0 is written to
 * standard output, otherwise a 1 is written. A -1 is written to standard
 * output if there is a problem.
 *
 *INDENT-OFF*
 * $Log$
 *
 *INDENT-ON*
 *
*/

/*
 *  Includes
 */

#include <stdio.h>
#include <string.h>

#ifndef vxWorks        /* Only compile this section for UNIX */

int main(
   int argc,		/* Command argument counter          */
   char **argv		/* Pointer to first command argument */
   )
{
   double val1;		/* Value extracted from command argument 1*/
   double val2;		/* Value extracted from command argument 2*/
   double adiff;	/* Absolute difference                   */
   double toler;        /* Tolerance extracted from argument 3   */

   adiff = 0.0;
   toler = 0.0;

   /*
    * Decrement the argument counter and increment the argument
    * pointer to skip over the first argument, which contains the
    * name of the command used to invoke the program itself.
    */

   argc--;
   argv++;

   /*
    * Obtain the first two command arguments and calculate their
    * absolute difference.
    */

   if ( (argc--) && (sscanf ( *argv++, "%lf", &val1) == 1) )
   {
      if ( (argc--) && (sscanf ( *argv++, "%lf", &val2) == 1) )
      {
         adiff = val1 - val2;
         if ( adiff < 0.0 ) adiff = -adiff;
      }
      else
      {
         printf ("-1\n");
         return (-1);
      }
   }
   else
   {
      printf ("-1\n");
      return (-1);
   }

   /*
    * Obtain the tolerance from the third argument, compare the
    * values and write the result to standard output.
    */

   if ( (argc--) && (sscanf ( *argv++, "%lf", &toler) == 1) )
   {
      if ( adiff <= toler )
         printf ("0\n");
      else
         printf ("1\n");
   }
   else
   {
      printf ("-1\n");
      return (-1);
   }

   return (0);

}
#endif   /* NOT VXWORKS */

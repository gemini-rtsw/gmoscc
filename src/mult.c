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
 * mult.c
 *
 * PURPOSE:
 * Multiply together all the values on the command line.
 *
 * INVOCATION:
 * mult val1 val2 val3 ...
 *
 * EXAMPLES:
 * set xpos = `mult $xpos $xmultiple`
 * set total = `mult $one $two $three $four`
 *
 * DESCRIPTION:
 * This Unix program is designed to be called from a C shell script
 * when there is a need to multiply together two or more floating point
 * variables. All the arguments supplied to the program on the command
 * line are multiplied together and the result written to the standard output.
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.1  2000/08/14 08:33:26  gmos
 * Initial version
 *
 *
 *INDENT-ON*
 *
*/

/*
 *  Includes
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef vxWorks        /* Only compile this section for UNIX */

int main(
   int argc,		/* Command argument counter          */
   char **argv		/* Pointer to first command argument */
   )
{
   float val;		/* Value extracted from command argument */
   float product;	/* Running product                         */

   /* Initialise the product */
   product = 1.0;

   /*
    * Decrement the argument counter and increment the argument
    * pointer to skip over the first argument, which contains the
    * name of the command used to invoke the program itself.
    */

   argc--;
   argv++;

   /*
    * Loop through the command arguments, decrementing the argument
    * counter each time until it reaches zero.
    */

   while ( argc-- )
   {

      /*
       * Attempt to extract a floating point value from the current
       * command argument. If the conversion works add the value to
       * the current sum. Ignore arguments that cannot be converted.
       */

      if ( sscanf ( *argv++, "%f", &val) == 1 )
      {
         product *= val;
      }
   }

   /* Finally, write the product of the arguments to the standard output,
    * rounded to the highest integer.
    */

   printf ("%d\n", (int) ceil(product));
   return (0);

}
#endif   /* NOT VXWORKS */


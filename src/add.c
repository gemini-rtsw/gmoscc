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
 * add.c
 *
 * PURPOSE:
 * Add together all the values on the command line.
 *
 * INVOCATION:
 * add val1 val2 val3 ...
 *
 * EXAMPLES:
 * set xpos = `add $xpos $xincrement`
 * set total = `add $one $two $three $four`
 *
 * DESCRIPTION:
 * This Unix program is designed to be called from a C shell script
 * when there is a need to add together two or more floating point
 * variables. All the arguments supplied to the program on the command
 * line are added together and the result written to the standard output.
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.1  2000/08/09 13:44:14  gmos
 * Added add.c
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

#ifndef vxWorks        /* Only compile this section for UNIX */

int main(
   int argc,		/* Command argument counter          */
   char **argv		/* Pointer to first command argument */
   )
{
   float val;		/* Value extracted from command argument */
   float sum;		/* Running total                         */

   /* Initialise the sum */
   sum = 0.0;

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
         sum += val;
      }
   }

   /* Finally, write the sum of the arguments to the standard output */

   printf ("%f\n", sum);
   return (0);

}
#endif   /* NOT VXWORKS */

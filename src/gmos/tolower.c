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
 * tolower.c
 *
 * PURPOSE:
 * Accept string(s) - convert all characters to lower case using tolower()
 *
 * INVOCATION:
 * mult str1 str2...
 *
 * EXAMPLES:
 *
 * DESCRIPTION:
 * This Unix program is designed to be called from a C shell script
 *
 *INDENT-OFF*
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
#include <ctype.h>

#ifndef vxWorks        /* Only compile this section for UNIX */

int main(
   int argc,		/* Command argument counter          */
   char **argv		/* Pointer to first command argument */
   )
{
     char String[256];		/* String to convert */
     int i;                       /* A counter         */

     if (argc == 1)
     {
	  if (scanf ("%s", String) == 1 )
	  {
	       for (i = 0; String[i] != EOF; String[i++] = tolower(String[i]));
	       printf ("%s ", String);
	  }

     }
     else
     {

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

	       if ( sscanf ( *argv++, "%s", String) == 1 )
	       {
		    for (i = 0; String[i] != EOF; String[i++] = tolower(String[i]));
		    printf ("%s ", String);
	       }
	  }
     }
     /* Finally, write the product of the arguments to the standard output,
      * rounded to the highest integer.
      */

     printf ("\n");
     return (0);

}
#endif   /* NOT VXWORKS */



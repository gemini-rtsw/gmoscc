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
 * rand.c
 *
 * PURPOSE:
 * Generate a seeded random integer between 0 and N-1.
 *
 * INVOCATION:
 * rand seed N
 *
 * EXAMPLES:
 * set var = `rand 3 11`
 * set var = `rand $var 11`
 *
 * DESCRIPTION:
 * This Unix program is designed to be called from a C shell script.
 *
 *INDENT-OFF*
 *
 * $Log$
 * Revision 1.2  2001/02/16 11:34:35  gmos
 * randMax tested to avoid floating exception. Error messages added but commented out for now in case they affect the flexlog script.
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
#include <stdlib.h>

#ifndef vxWorks        /* Only compile this section for UNIX */

int main(
   int argc,		/* Command argument counter          */
   char **argv		/* Pointer to first command argument */
   )
{
     int randMax;                       /* Maximum number requested */
     int seed;

     if (argc == 1)
     {
	  if (scanf ("%d %d", &seed, &randMax) != 2 )
	  {
               /* printf ("seed and N needed.\n"); */
	       return (-1);
	  }

     }
     else if (argc == 3)
     {

	  argv++;

	  if ( sscanf ( *argv++, "%d", &seed) != 1 )
	  {
               /* printf ("Invalid seed.\n"); */
	       return (-1);
	  }
	  else if ( sscanf ( *argv++, "%d", &randMax) != 1 )
	  {
               /* printf ("Invalid N.\n"); */
	       return (-1);
	  }
     }

     if ( randMax > 0 )
     {

        srand (seed);
        printf ("%d\n", ((int) rand()) % randMax);
     }
     else
     {
        /* printf ("N must be > 0.\n"); */
        return (-1);
     }

     return (0);

}
#endif   /* NOT VXWORKS */



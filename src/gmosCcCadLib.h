/*
*   FILENAME
*   -------- 
*   gmosCcCadLib.h
*
*   DESCRIPTION
*   -----------
*   Header file for gmosCcCadLib library.
*
*   AUTHOR
*   ------
*   Steven Beard  (smb@roe.ac.uk)
*
*   HISTORY
*   -------
*   09-Jul-1999: Original version.                             (smb)
*/
/* *INDENT-OFF* */
/*
 * $Log$
 * Revision 1.1  1999/08/02 14:22:54  gmos
 * First version
 *
 */
/* *INDENT-ON* */

#ifndef GMOS_CCCADLIB_INC
#define GMOS_CCCADLIB_INC

#include  <cadRecord.h>
#include  <cad.h>

/* Function definitions */

long gmosCadModeInit( struct cadRecord *pcad );
long gmosCadInit( struct cadRecord *pcad );
long gmosCadDatum( struct cadRecord *pcad );
long gmosCadTest( struct cadRecord *pcad );
long gmosCadUpdate( struct cadRecord *pcad );
long gmosCadPark( struct cadRecord *pcad );
long gmosCadDebug( struct cadRecord *pcad );
long gmosCadSimulate( struct cadRecord *pcad );
long gmosCadAttributeFanout( struct cadRecord *pcad );
long gmosCadDirFanout( const char *dir, struct cadRecord *pcad );
long gmosCadModeFanout( const long mode, struct cadRecord *pcad );

#endif

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
 *   FILENAME
 *   -------- 
 *   ddrMessageLevels.h
 *
 *   PURPOSE
 *   -----------
 *   Define the mapping of device message levels onto debugging levels.
 *
 *   DESCRIPTION
 *   ------------
 *   This file defines a set of DDR_MSG_xxx constants which represent
 *   the kinds of message that GMOS device control software can generate.
 *   It maps those messages onto particular device control record debugging
 *   modes, identified in DDR_DBUG_xxx constants. For example, the
 *   following definition
 *
 *       #define DDR_MSG_LOG  DDR_DBUG_MIN
 *
 *   maps the "LOG" type message onto the "MIN" debugging
 *   mode, which means that "LOG" type messages will appear
 *   at debugging modes of MIN or higher.
 *
 *   The messages that appear at various debugging levels can
 *   be changed by editing this file rather than by changing
 *   the source code, as long as the source code includes this
 *   file after "recDeviceControl.h".
 *
 *   AUTHOR
 *   ------
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *
 *INDENT-OFF*
 *
 * $Log$
 * Revision 1.1  2001/11/28 20:15:06  mbec
 * *** empty log message ***
 *
 * Revision 1.1  2001/08/10 14:01:48  ptaylor
 * Restructured src directory with 4 sub-directories, including pv and lut which were previously in pv as well as deviceControl and gcal previously in src
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.1  2001/03/01 14:13:36  gmos
 * Added new device message levels file based on darMessageLevels.
 *
 *
 *
 *INDENT-ON*
 */

#ifndef    DDR_MESSLEV_INC
#define    DDR_MESSLEV_INC

/*
 * This file uses constants defined in recDeviceControl.h
 */

#include <deviceControl.h>

/*
 * Definitions for device control message types, and their
 * mapping onto the debugging modes. The messages will appear
 * provided the current debugging mode is higher than or equal
 * to than the mode associated here with each kind of message.
 *
 * These definitions can be modified to tailor which messages
 * appear at which debugging levels.
 */

#define DDR_MSG_FATAL           DDR_DBUG_QUIET   /* Fatal error messages       */
#define DDR_MSG_ERROR           DDR_DBUG_NONE    /* Serious error messages     */
#define DDR_MSG_WARNING         DDR_DBUG_NONE    /* Warning messages           */
#define DDR_MSG_LOG             DDR_DBUG_MIN     /* Log messages               */
#define DDR_MSG_MIN             DDR_DBUG_MIN     /* Minimal debugging messages */
#define DDR_MSG_FULL            DDR_DBUG_FULL    /* Full debugging messages    */
#define DDR_MSG_MAX		DDR_DBUG_MAX     /* Insane debugging messages  */

#endif /* DDR_MESSLEV_INC */

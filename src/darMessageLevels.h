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
 *   darMessageLevels.h
 *
 *   PURPOSE
 *   -----------
 *   Define the mapping of assembly message levels onto debugging levels.
 *
 *   DESCRIPTION
 *   ------------
 *   This file defines a set of DAR_MSG_xxx constants which represent
 *   the kinds of message that GMOS assembly software can generate.
 *   It maps those messages onto particular assembly record debugging
 *   modes, identified in DAR_DBUG_xxx constants. For example, the
 *   following definition
 *
 *       #define DAR_MSG_LOG  DAR_DBUG_MIN
 *
 *   maps the "LOG" type message onto the "MIN" debugging
 *   mode, which means that "LOG" type messages will appear
 *   at debugging modes of MIN or higher.
 *
 *   The messages that appear at various debugging levels can
 *   be changed by editing this file rather than by changing
 *   the source code, as long as the source code includes this
 *   file after "recAssControl.h".
 *
 *   AUTHOR
 *   ------
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *
 *INDENT-OFF*
 *
 * $Log$
 * Revision 1.1  2001/02/28 17:24:09  gmos
 * Renamed gmMessageLevels.h to darMessageLevels.h.
 *
 * Revision 1.2  2000/12/19 13:29:51  gmos
 * Mapped fatal initialisation errors onto DAR_DBUG_QUIET, so these cannot be turned off.
 *
 * Revision 1.1  2000/12/08 09:22:25  gmos
 * New set of constants mapping message types onto debugging levels
 *
 *
 *INDENT-ON*
 */

#ifndef    DAR_MESSLEV_INC
#define    DAR_MESSLEV_INC

/*
 * This file uses constants defined in recAssControl.h
 */

#include        <recAssControl.h>

/*
 * Definitions for assembly control message types, and their
 * mapping onto the debugging modes. The messages will appear
 * provided the current debugging mode is higher than or equal
 * to than the mode associated here with each kind of message.
 *
 * These definitions can be modified to tailor which messages
 * appear at which debugging levels.
 */

#define DAR_MSG_FATAL           DAR_DBUG_QUIET   /* Fatal error messages       */
#define DAR_MSG_ERROR           DAR_DBUG_NONE    /* Serious error messages     */
#define DAR_MSG_WARNING         DAR_DBUG_NONE    /* Warning messages           */
#define DAR_MSG_LOG             DAR_DBUG_NONE    /* Log messages               */
#define DAR_MSG_MIN             DAR_DBUG_MIN     /* Minimal debugging messages */
#define DAR_MSG_FULL            DAR_DBUG_FULL    /* Full debugging messages    */
#define DAR_MSG_MAX		DAR_DBUG_MAX     /* Insane debugging messages  */

#endif /* DAR_MESSLEV_INC */

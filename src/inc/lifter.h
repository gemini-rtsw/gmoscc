
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
 * lifter.h
 *
 * PURPOSE:
 * Symbolic constants for the GMOS grating lifter
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.4  2003/05/27 21:16:51  gemvx
 * Version 4-1
 *
 * Revision 1.3  2003/01/31 14:12:46  gemvx
 * Merged gmos-south
 *
 * Revision 1.3  2002/07/23 14:44:36  gmos
 * Before Cold tests and addition of SMB's ADC software mods
 *
 * Revision 1.4  2001/07/10 13:45:20  gmos
 * V2-6 imported from Gemini.
 *
 * Revision 1.3  2001/04/20 02:12:19  smb
 * Lifter cycle time changed again from 10 to 8 seconds.
 *
 * Revision 1.2  2001/04/20 01:02:50  smb
 * Increased lifter wait time from 5 to 10 seconds.
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.5  2000/09/01 09:22:30  gmos
 * Random tabs removed from source code.
 *
 * Revision 1.4  2000/07/10 09:51:13  cjt
 * Changed GR_TURRET_POWER_ON/OFF to GR_TURRET_POWER_STAT_ON/OFF to avoid clash
 * with devGratingAss.c.
 *
 * Revision 1.3  2000/07/10 08:21:24  gmos
 * Lifter disabled while turret not powered.
 *
 * Revision 1.2  2000/07/06 17:24:09  gmos
 * Lifter cycle time increased. Automatic lift after failure to lower added.
 *
 * Revision 1.1  1999/12/10 14:33:46  gmos
 * Contents of assembly modules merged into gmoscc
 *
 * Revision 1.3  1999/12/03 18:29:57  gmos
 * Adjusted watchdog time and fixed some problems with the lifter code. Diagnostic message added.
 *
 * Revision 1.2  1999/11/30 13:46:31  gmos
 * Added UKATC banner
 *
 * Revision 1.1  1999/11/23 16:16:21  gmos
 * More corrections to lifter. Make default demand OFF rather than LOWER
 *
 *
 *INDENT-ON*
 *
*/

#ifndef GR_LIFTER_INC
#define GR_LIFTER_INC

#define GR_SWITCH_INACTIVE 1
#define GR_SWITCH_ACTIVE   0
#define GR_VALVE_INACTIVE  1
#define GR_VALVE_ACTIVE    0
#define GR_PRESSURE_OK     0

#define GR_LIFTER_CYCLE_TIME  10	/* Lifter cycle time in seconds			*/
#define GR_LIFTER_SHORT_TIME  2		/* Lifter short reprocessing time in seconds	*/
#define GR_LIFTER_MAX_CYCLES  1		/* Maximum number of lifter cycles		*/

#define LIFTER_DBUG_NONE   0
#define LIFTER_DBUG_MIN    1
#define LIFTER_DBUG_FULL   2
#define LIFTER_DBUG_MAX    3

typedef enum
{
    GR_LIFTER_OFF = 0,
    GR_LIFTER_DOWN,
    GR_LIFTER_UP,
    GR_LIFTER_STOP,
    GR_LIFTER_EUP
} gr_demands;

typedef enum
{
    GR_LIFTER_ENGAGED = 0,
    GR_LIFTER_RAISED,
    GR_LIFTER_MOVING,
    GR_LIFTER_ERROR,
    GR_LIFTER_INIT
} gr_states;

typedef enum
{
    GR_ACTION_RAISING = 0,
    GR_ACTION_LOWERING,
    GR_ACTION_IDLE,
    GR_ACTION_EMERGENCY_RAISE
} gr_actions;

typedef enum
{
    GR_TURRET_POWER_STAT_OFF = 0,
    GR_TURRET_POWER_STAT_ON
} gr_turret_power_status;

#endif

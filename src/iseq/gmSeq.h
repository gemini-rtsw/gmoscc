/* *INDENT-OFF* */
/*
*   FILENAME
*   gmSeq.h
* 
*   PURPOSE:
*   Symbolic constants, macro and structure definitions for the GMOS Sequencer
*
*   Copyright Observatory Sciences Ltd. 1999-2000. All rights reserved.
*   Under contract to the UK Astronomy Technology Centre, who modified the code
*   in 2001.
*
*/
/*
 * $Log$
 * Revision 1.2  2003/01/31 14:12:46  gemvx
 * Merged gmos-south
 *
 * Revision 1.1  2002/04/24 05:26:18  ajf
 * Added for epics3.13.4GEM8.4.
 *
 * Revision 1.3  2002/01/15 21:06:07  mbec
 * *** empty log message ***
 *
 * Revision 1.2  2001/11/28 19:35:26  mbec
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.28  2001/02/28 12:46:39  gmos
 * gmSeqDisplayCADTest function added.
 *
 * Revision 1.27  2001/02/28 10:34:47  gmos
 * Maximum wavelength changed from 1000nm to 1100nm
 *
 * Revision 1.26  2001/02/23 13:12:50  gmos
 * Renamed global variables so they begin gmSeq. Added more comments.
 *
 * Revision 1.25  2001/02/21 18:14:16  gmos
 * New code to separate mask data into three separate menus - one for each cassette.
 *
 * Revision 1.24  2001/02/01 11:02:52  gmos
 * Min. and Max wavelengths should be floating point constants.
 *
 * Revision 1.23  2001/01/29 18:00:59  gmos
 * Added filter effective wavelength and grating focus offset to database. Define string buffer size here.
 *
 * Revision 1.22  2001/01/29 15:02:19  gmos
 * Reading lookup tables could result in buffer overflow. Fixed.
 *
 * Revision 1.21  2001/01/23 11:58:34  gmos
 * Translation stage limits now include a border around the edge.
 *
 * Revision 1.20  2000/12/19 13:32:16  gmos
 * Define DBG_QUIET.
 *
 * Revision 1.19  2000/12/15 11:35:25  gmos
 * MASTER_ENABLE test condition added. Test for MASTER_ENABLE in all commands except INIT and TEST.
 *
 * Revision 1.18  2000/09/22 08:21:48  gmos
 * Parameters used to define a universal wavelength range of 300 to 1000 nanometres.
 *
 * Revision 1.17  2000/09/20 14:05:45  gmos
 * Fixed mistake in CVS variables Log and Id
 *
 */
/* *INDENT-ON* */

#ifndef INCgmseqh
#define INCgmseqh

#include <lstLib.h>	/* Contains definition of NODE */
#include <dbDefs.h>     /* Contains definition of MAX_STRING_SIZE */

/* Bit masks for CAD command test conditions */
#define CONFIGURING    0x0001     /* If set, reject when instrument configuring.    */
#define READING_OUT    0x0002     /* If set, reject when detector reading out.      */
#define OBSERVING      0x0004     /* If set, reject when detector acquiring data.   */
#define MASTER_ENABLE  0x0008     /* If set, reject when master enable not enabled. */

/*
 * Limits (microns) for the demand positions of the detector translation stage,
 * before flexure offsets are applied. These limits are set inside the actual
 * range of the translation assembly by the defined X, Y and Z borders to allow
 * for flexure offsets.
 */

#define DTAXBORDER    30.0  /* Size of X border (microns) */
#define DTAYBORDER    30.0  /* Size of Y border (microns) */
#define DTAZBORDER    30.0  /* Size of Z border (microns) */

#define DTAABSXMAX   243.0  /* Maximum X of detector translation stage (microns) */
#define DTAABSXMIN   -17.0  /* Minimum X of detector translation stage (microns) */
#define DTAABSYMAX   258.0  /* Maximum Y of detector translation stage (microns) */
#define DTAABSYMIN   -14.0  /* Minimum Y of detector translation stage (microns) */
#define DTAABSZMAX  4950.0  /* Maximum Z of detector translation stage (microns) */
#define DTAABSZMIN    30.0  /* Minimum Z of detector translation stage (microns) */

#define DTAXMAX   (DTAABSXMAX - DTAXBORDER)
#define DTAXMIN   (DTAABSXMIN + DTAXBORDER)
#define DTAYMAX   (DTAABSYMAX - DTAYBORDER)
#define DTAYMIN   (DTAABSYMIN + DTAYBORDER)
#define DTAZMAX   (DTAABSZMAX - DTAZBORDER)
#define DTAZMIN   (DTAABSZMIN + DTAZBORDER)

/* Limits (degrees) for movement of atmDC entrance and exit prism angles */

#define ATMENPMIN -360.0  /* atmDC entrance prism minimum angle (degrees) */
#define ATMENPMAX  360.0  /* atmDC entrance prism maximum angle (degrees) */
#define ATMEXPMIN -360.0  /* atmDC exit prism minimum angle (degrees)     */
#define ATMEXPMAX  360.0  /* atmDC exit prism maximum angle (degrees)     */

/* Min. and Max acceptable wavelengths (nanometres) */
#define MIN_WAVELENGTH  300.0
#define MAX_WAVELENGTH 1300.0

/*
 * Tracking modes for atmospheric dispersion compensator and detector
 * translation.
 */
#define TRACKSTOP     0
#define TRACKMOVE     1
#define TRACKFOLLOW   2
#define TRACKFOLLOWXY 3

/*
 * Define the debug levels recognised by the GMOS instrument sequencer.
 */

#define DBG_QUIET 0x0000
#define DBG_NONE  0x0001
#define DBG_MIN   0x0002
#define DBG_FULL  0x0004
#define DBG_MAX   0x0008

/*
 * Define the debugging macros used by the intrument sequencer.
 * Each macro will display the message associated with it if the
 * bit corresponding to the debugging level is defined in the dbglevel
 * bit mask.
 */

/*extern int epicsPrintf(char *, ...);*/   /* printf function provided by EPICS logging */

#define DBGMSG( _dbgLevel, _dbgMsg) \
  do { \
     if (gmSeqDbgLevel & (_dbgLevel) ) epicsPrintf ("gmSeq: %s\n", _dbgMsg) ; \
  } while(0)

#define DBGMSGSTRING( _dbgLevel, _dbgMsg, _dbgString) \
  do { \
     if (gmSeqDbgLevel & (_dbgLevel) ) epicsPrintf ("gmseq: %s %s\n", _dbgMsg, _dbgString) ; \
  } while(0)

#define DBGMSGREAL( _dbgLevel, _dbgMsg, _dbgFloat) \
  do { \
     if (gmSeqDbgLevel & (_dbgLevel) ) epicsPrintf ("gmseq: %s %f\n", _dbgMsg, _dbgFloat) ; \
  } while(0)


#define DBGMSGINT( _dbgLevel, _dbgMsg, _dbgInt) \
  do { \
     if (gmSeqDbgLevel & (_dbgLevel) ) epicsPrintf ("gmSeq: %s %d\n", _dbgMsg, _dbgInt) ; \
  } while(0)

/*
 * GMOS limits, array and buffer size definitions
 */

#define FILTPERWHEEL    12      /* Max filters per wheel                                */
#define MASKPERCASS      9      /* Max masks per cassette                               */
#define MAXGRATINGS      4      /* Max loaded gratings                                  */

#define MAXMENU         16      /* Max number of strings allowed in mbbi menu           */
#define LUT_TAG_SZ	MAX_STRING_SIZE
                                /* Make LUT name size same as maximum EPICS string size */
#define STRING_BUF_SZ   256     /* Size of large string buffers.                        */
#define HALF_BUF_SZ     128     /* Half size of large string buffers.                   */


/*
 * Structure for data from filters.lut data file describing all the filters
 * known to the software.
 */ 

typedef struct FILTLUT
{
    NODE node;                     /* Next node in linked list.                  */
    char tag[LUT_TAG_SZ];          /* Filter name.                               */
    long barcodeId;                /* Barcode ID of filter.                      */
    double focusOffset;            /* Focus offset of filter (microns)           */
    double effWavelength;          /* Effective wavelength of filter (nm)        */
} FILTLUT;

/*
 * Structure for data from flt.lut data file describing the filters
 * installed on a particular filter wheel.
 */

typedef struct WHEELFILTLUT
{
    NODE node;                     /* Next node in linked list.                  */
    long barcode;                  /* Barcode ID of filter.                      */
    long wheelnum;                 /* Wheel number on which filter installed.    */
    char position[LUT_TAG_SZ];     /* Position name on wheel.                    */
} WHEELFILTLUT;

/*
 * Structure for data from gratings.lut data file describing all the gratings
 * known to the software. There are two entries for each grating representing
 * the two possible orientations it may be installed in.
 */ 

typedef struct GRATLUT
{
    NODE node;                     /* Next node in linked list.                  */
    char tag[LUT_TAG_SZ];          /* Grating/orientation combination name.      */
    long barcodeId;                /* Barcode ID of grating at this orientation. */
    long linesPerMm;               /* Ruling density (lines per mm).             */
    long blazeDir;                 /* Blaze direction at this orientation.       */
    double focusOffset;            /* Focus offset of grating (microns).         */
} GRATLUT;

/*
 * Structure for data from gr.lut data file describing the gratings
 * installed on the turret, and recording their tilt angles.
 */

typedef struct LOADEDGRATLUT
{
    NODE node;                     /* Next node in linked list.                  */
    long barcode;                  /* Barcode ID of grating.                     */
    long turretPos;                /* Turret position at which grating installed.*/
    double tilt;                   /* Current grating tilt angle (degrees).      */
} LOADEDGRATLUT;

/*
 * Structure for data from masks.lut data file describing all the special named
 * masks known to the software. NOTE: Only the special masks have names. Most of
 * the science masks are referred to only by their barcodes.
 */ 

typedef struct MASKLUT
{
    NODE node;                     /* Next node in linked list.                  */
    char tag[LUT_TAG_SZ];          /* Mask name.                                 */
    long barcodeId;                /* Barcode ID of mask.                        */
    double focusOffset;            /* Focus offset for ifu                       */
} MASKLUT;

/*
 * Structure for data from msk.lut data file describing the masks
 * installed in the loaded cassettes.
 */

typedef struct LOADEDMASKLUT
{
    NODE node;                     /* Next node in linked list.                  */
    long barcode;                  /* Barcode ID of mask.                        */
    long cassette;                 /* Cassette in which mask is installed.       */
    long slot;                     /* Slot number of mask within cassette.       */
} LOADEDMASKLUT;


/* Global and external functions */

/* 
 * Convert a string to upper case.
 */
char * gmSeqUc(char *in, int lout, char *out);

/*
 * Return a grating tilt angle for a given wavelength, order and ruling density 
 */
int gratingSci2Tilt(
            const double wavelength,    /* Central wavelength in nm              */
            const int order,            /* Grating order                         */
            const double rulingDensity, /* Grating ruling density per mm         */
            double *tiltAngle);         /* Pointer grating tilt angle in degrees */

/*
 * Functions for managing the bit mask of CAD command test conditions.
 */
void gmSeqSetCADTest(const int testMask);
void gmSeqClearCADTest();
int  gmSeqGetCADTest();
void gmSeqDisplayCADTest();

/*
 * Decode a string by matching an array element.
 */
int  gmSeqDcString(char **match, char *string);

#endif	/* !INCgmseqh */

static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id$"};

/* *INDENT-OFF* */
/*
 *   FILENAME
 *   gmSeqCadLib.c
 *
 *   PURPOSE:
 *   Routines used by GMOS Sequencer CAD records
 *
 *   FUNCTION NAME(S):
 *    gmSeqFilterLUTread -       Initialise the filter CAD database
 *    gmSeqCadInitFilter -       Initialise the filter CAD
 *    gmSeqCadFilter -           Implements the fltPos CAD command
 *    gmSeqLambdaFocusLUTread -  Read the wavelength against focus lookup table file
 *    gmSeqGratingLUTread -      Initialise the grating database
 *    gmSeqCadInitGrating -      Initialise the grating CAD
 *    gmSeqCadGrating -          Implements the grSelect CAD command
 *    gmSeqCadReboot  -          Null routine called by reboot CAD record
 *    gmSeqReboot  -             Reboot the GMOS Instrument Sequencer
 *    gmSeqCadInit  -            Implements the Sequencer init command
 *    gmSeqCadSetup  -           For various CAD routines involved in setting up GMOS
 *    gmSeqCadDebug -            Implements the Instrument Sequencer debug command
 *    gmSeqNullInit  -           Null CAD initialisation
 *    gmSeqConfigBegin  -        Implements the start of configuration apply handling.
 *    gmSeqConfigEnd  -          Implements the end of configuration apply handling.
 *    gmSeqCadTrivial   -        Routine for trivial GMOS sequence CAD record
 *    gmSeqCadObserve -          Implements the CAD for the 'observe' sequence command
 *    gmSeqCadStopObserve  -     For sequence CADs: pause/stop/abort an observation
 *    gmSeqCadEndObserve  -      For GMOS sequence CAD endObserve
 *    gmSeqCadContinue  -        For GMOS sequence CAD continue
 *    gmSeqCadMask -             Implements the mskPos CAD command 
 *    gmSeqCadAtmPos -           Implements the atmPos CAD command
 *    gmSeqCadDtaPos -           Implements the dtaPos CAD command
 *
 *   Copyright Observatory Sciences Ltd. 1999-2000. All rights reserved.
 *   Under contract to the UK Astronomy Technology Centre, who modified the code
 *   in 2001.
 *
 */
/*
 * $Log$
 * Revision 1.2  2001/04/25 19:51:09  smb
 * Fixed bug in which PAUSE/CONTINUE/STOP/ABORT commands could return a garbage status.
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.50  2001/03/01 14:09:19  gmos
 * Fixed mistake in gmSeqCadDebug where pcad->b was being treated as a long rather than string. Use (char *) explicitly for CAD string fields.
 *
 * Revision 1.49  2001/02/28 10:36:39  gmos
 * Some printfs reworded. Additional checking of lambdaFocus lookup table.
 *
 * Revision 1.48  2001/02/23 13:14:08  gmos
 * Added functions to translate mask names into barcode IDs, based on names read from masks.lut. Renamed global variables so they begin gmSeq. Made the filter and mask LUT reading functions more robust.
 *
 * Revision 1.47  2001/02/21 13:34:49  gmos
 * Copyright statement modified.
 *
 * Revision 1.46  2001/02/12 17:59:23  gmos
 * Use effective wavelength specified in grating SEL mode to predict focus offset.
 *
 * Revision 1.45  2001/02/12 16:56:47  gmos
 * Zero wavelength focus offset when mirror selected. Treat a grating used at zero order like a mirror.
 *
 * Revision 1.44  2001/02/05 13:07:29  gmos
 * Grating effective wavelength added to SAD. Code prologue tidied up.
 *
 * Revision 1.43  2001/02/01 11:04:16  gmos
 * Major change to the way that filter and grating offsets are handled. Filters now have surface power and effective wavelength offsets. Gratings now have a surface power offset in addition to wavelength offset. Debug command now has option to set only Instrument Sequencer mode.
 *
 * Revision 1.42  2001/01/30 12:00:50  gmos
 * Substantial amount of tidying up. Subtract 1 from strncpy lengths and format lengths to allow for trailing NULL. Illegal STOP directive now generates a message.
 *
 * Revision 1.41  2001/01/29 18:10:06  gmos
 * Format size deficiency commented.
 *
 * Revision 1.40  2001/01/29 18:02:40  gmos
 * LUT_TAG_SZ is 40 characters, not 20 - adjusted formats accordingly. Use symbolic constants for LUT_TAG_SZ and STRING_BUF_SZ wherever possible.
 *
 * Revision 1.39  2001/01/29 15:02:19  gmos
 * Reading lookup tables could result in buffer overflow. Fixed.
 *
 * Revision 1.38  2001/01/26 10:25:11  gmos
 * Added effective wavelength parameter to grSelect CAD. Tidyied up code layout in gmSeqGrating function considerably.
 *
 * Revision 1.37  2001/01/24 15:10:42  gmos
 * Added reading of lambdaFocus.lut, after IS lookup table changes.
 *
 * Revision 1.36  2001/01/24 10:41:29  gmos
 * Rework of IS lookup table reading (in work)
 *
 * Revision 1.35  2000/12/19 13:32:44  gmos
 * Allow the DEBUG command to accept the QUIET string. Also do not send MAX to detector controller.
 *
 * Revision 1.34  2000/12/15 11:35:25  gmos
 * MASTER_ENABLE test condition added. Test for MASTER_ENABLE in all commands except INIT and TEST.
 *
 * Revision 1.33  2000/10/25 16:18:01  pbt
 * Initial filter/grating names now "unknown"
 *
 * Revision 1.32  2000/10/25 09:24:10  gmos
 * Load the lookup table file names from gensub database fields rather than hard-wired names. Cad records decide the names from the database prefix.
 *
 * Revision 1.31  2000/10/24 11:34:43  gmos
 * Modified to reject OBSERVE, PAUSE, CONTINUE, STOP and ABORT commands when detector controller disconnected.
 *
 * Revision 1.30  2000/10/17 13:43:40  gmos
 * Lookup tables moved from data to /gemini/gmos directory.
 *
 * Revision 1.29  2000/09/26 13:55:14  gmos
 * Supressed superflous printf statements.
 *
 * Revision 1.28  2000/09/22 08:21:48  gmos
 * Parameters used to define a universal wavelength range of 300 to 1000 nanometres.
 *
 * Revision 1.27  2000/09/20 14:05:45  gmos
 * Fixed mistake in CVS variables Log and Id
 *
 */
/* *INDENT-ON* */

#include  <vxWorks.h>
#include  <stdlib.h>
#include  <types.h>
#include  <stdioLib.h>
#include  <lstLib.h>
#include  <strLib.h>
#include  <taskLib.h>
#include  <rebootLib.h>
#include  <sysLib.h>

#include  <stdio.h>
#include  <string.h>
#include  <math.h>

#include  <dbDefs.h>
#include  <dbEvent.h>
#include  <genSubRecord.h>
#include  <cadRecord.h>
#include  <subRecord.h>
#include  <dbCommon.h>
#include  <recSup.h>
#include  <cad.h>
#include  <carRecord.h>
/*#include  <subCadRecord.h>*/
#include <cad.h>

#include "gmSeq.h"
#include "gmosLutLib.h"

/* #define VERBOSE */    /* Define to enable extra printf statements */

/*
 * NOTE: The global pointers declared below are shared with gmSeqDisplayMenus.c
 */

void *gmSeqGratLutPtr = NULL;                /* Global grating lookup table pointer */
static int gratingInitOK;                    /* If grating LUT table read OK */

void *gmSeqFiltLutPtr = NULL;                /* Global filter lookup table pointer  */
static int filterInitOK;                     /* If filter LUT table read OK */

void *gmSeqMaskLutPtr = NULL;                /* Global mask lookup table pointer  */
static int maskInitOK;                       /* If mask LUT table read OK */

static gmosLookupTable lambdaFocusLut;       /* Wavelength vs. focus offset LUT data      */

int gmSeqDbgLevel = DBG_NONE;                /* Global debug level flag, initialised to DBG_NONE */

/* Function definitions */

long gmSeqMaskLUTread( char * lutfilename );
long gmSeqFilterLUTread( char * lutfilename );
long gmSeqGratingLUTread( char * lutfilename );

/******************************************************************************/


/*+
 *   Function name:
 *   gmSeqFilterLUTread
 *
 *   Purpose:
 *   Initialise the filter CAD database
 *
 *   Description:
 *   Read the filter LUT data file 
 *   and set up an internal list of filter names with their 
 *   attributes (barcode and focus offset)
 *
 *   Invocation:
 *   gmSeqFilterLUTread( char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)  lutfilename (string) name of filters lut file
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqFilterLUTread( char * lutfilename )
{
    FILE *fp;                /* LUT data file */
    char buf[STRING_BUF_SZ]; /* Input buffer */
    char tag[LUT_TAG_SZ];    /* Tag (= filter) name */
    FILTLUT *p;              /* Look-up table data structure */
    long n;                  /* Status return for fscanf */
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    filterInitOK = FALSE;

/* Free up old list if this is not the first entry */

    if (gmSeqFiltLutPtr != NULL) 
    {
       lstFree((LIST *) gmSeqFiltLutPtr);
       free(gmSeqFiltLutPtr);
       gmSeqFiltLutPtr = NULL;
    }    
 
/* read data file from remote disk */

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
       printf("gmSeqFilterLUTread: Failed to open file %s\n", lutfilename);
       return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"gmSeqFilterLUTread: opened filter lut file: ", lutfilename);
    
    gmSeqFiltLutPtr = (void *) malloc (sizeof (LIST));
    if (gmSeqFiltLutPtr == NULL)    /* malloc failed */
    {
        printf("gmSeqFilterLUTread: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }

    lstInit ((LIST *) gmSeqFiltLutPtr);
    
    lcount = 0;
    while (status == CAD_ACCEPT)
    {

       /* skip blank lines and comments */

       n = fscanf (fp, "%39s", tag);     /* Format size should be LUT_TAG_SZ-1 */
       lcount++;
       if (n == 0 || tag[0] == '#')
       {
          /* Throw away the rest of a comment line */
          (void) fgets (buf, sizeof (buf)-1, fp);
       }
       else if (n == EOF)
       {
          /* End of File. Break out of loop. */
          break;
       }
       else
       {

          /* create new node, read tag and number of values */

          p = (FILTLUT *) malloc (sizeof (FILTLUT));

          if (p == NULL)    /* malloc failed */
          {
             printf("gmSeqFilterLUTread: LUT node malloc failed\n");
             fclose(fp);
             return CAD_REJECT;
          }

          lstAdd ((LIST *) gmSeqFiltLutPtr, (NODE *) p);
          strncpy (p->tag, tag, LUT_TAG_SZ-1);
          if (fscanf(fp,"%ld%lf%lf", &(p->barcodeId), &(p->focusOffset), &(p->effWavelength)) != 3 )
          {
             printf("gmSeqFilterLUTread: Missing value(s) in %s line %ld\n",
                    lutfilename, lcount);
             status = CAD_REJECT;
          }
          else
          {
             DBGMSGINT(DBG_FULL,"gmSeqFilterLUTread: data line read OK: ",lcount); 
          }
       } 
    }
    fclose (fp);
    if (status == CAD_ACCEPT) filterInitOK = TRUE;
    
    return status;
}

/*+
 *   Function name:
 *   gmSeqCadInitFilter 
 *
 *   Purpose:
 *   Initialise the filter CAD 
 *
 *   Description:
 *   Called at EPICS iocInit. Calls gmSeqFilterLUTread
 *
 *   Invocation:
 *   gmSeqCadInitFilter(pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqCadInitFilter(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadFilter 
 *
 *   Purpose:
 *   Implements the fltPos command
 *
 *   Description:
 *   This routine is the process subroutine for the fltPos CAD.
 *   It translates the input filter name by looking up the name in
 *   the filter look-up table, The resulting barcode ID is written to the
 *   GMOS Components Controller fltAssembly record.
 *
 *   Invocation:
 *   gmSeqCadFilter (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Filter name 1 as a text string
 *      b => Filter name 2 as a text string
 *
 *   EPICS output parameters:
 *      vala => translated Barcode ID for filter 1 (string)
 *      valb => translated Barcode ID for filter 2 (string)
 *      valc => Focus offset due to filter 1 surface curvature (double)
 *      vald => Focus offset due to filter 2 surface curvature (double)
 *      vale => Focus offset due to filter 1 effective wavelength (double)
 *      valf => Focus offset due to filter 2 effective wavelength (double)
 *      valg => Filter name 1 (string)
 *      valh => Filter name 2 (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */


long gmSeqCadFilter(struct cadRecord *pcad)
{ 
 
    long status;           /* Return status */
    FILTLUT *p;
    char buff[STRING_BUF_SZ];
    static char fname1[MAX_STRING_SIZE] = "unknown";
    static char fname2[MAX_STRING_SIZE] = "unknown";
    static char barcodeId1[MAX_STRING_SIZE];
    static char barcodeId2[MAX_STRING_SIZE];
    static double focusOffset1, focusOffset2;
    static double lambdaOffset1, lambdaOffset2;
    double lambda1, lambda2;
    long ret;

    status = CAD_REJECT ;
     
    DBGMSGINT(DBG_MIN,"CAD fltPos. Directive = ", pcad->dir);
    
    switch (pcad->dir)
    {

       case menuDirectivePRESET:

          if (!filterInitOK) 
          {
             strncpy(pcad->mess, "Filter name translation uninitialised",
                     MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }

/* Set the appropriate global test conditions for this CAD */

          gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);
    
          strncpy(fname1, (char *)pcad->a, MAX_STRING_SIZE-1);
          if (fname1[0])
          {

/*
 * Try to translate the 1st input filter name. Read through all filter lookup
 * table entries, searching for specified name. The search will stop immediately
 * if gmSeqFiltLutPtr is NULL.
 */

             for (p = (FILTLUT *) lstFirst ((LIST *)gmSeqFiltLutPtr); p != NULL;
                  p = (FILTLUT *) lstNext ((NODE *) p))
             {
                if (strncmp (p->tag, fname1, LUT_TAG_SZ) == 0)
                {
                   sprintf(barcodeId1, "%10ld%1c", p->barcodeId,'\0');
                   focusOffset1 = p->focusOffset;
                   lambda1 = p->effWavelength;
                   status = CAD_ACCEPT;
                   break;     /* Breaks out of "for" statement, not "case". */
                }
             }
          }

          if (status == CAD_REJECT)
          {
             strncpy(buff, "Unknown filter name (A): ", MAX_STRING_SIZE-1);
             strncat(buff, fname1, MAX_STRING_SIZE - strlen(buff));
             strncpy(pcad->mess, buff, MAX_STRING_SIZE-1);
             return(status);
          }

          status = CAD_REJECT;
          strncpy(fname2, (char *)pcad->b, MAX_STRING_SIZE-1);
          if (fname2[0])
          {

/*
 * Try to translate the 2nd input filter name. Read through all filter lookup
 * table entries, searching for specified name. The search will stop immediately
 * if gmSeqFiltLutPtr is NULL.
 */

             for (p = (FILTLUT *) lstFirst ((LIST *)gmSeqFiltLutPtr); p != NULL;
                  p = (FILTLUT *) lstNext ((NODE *) p))
             {
                if (strncmp (p->tag, fname2, LUT_TAG_SZ) == 0)
                {
                   sprintf(barcodeId2, "%10ld%1c", p->barcodeId,'\0');
                   focusOffset2 = p->focusOffset;
                   lambda2 = p->effWavelength;
                   status = CAD_ACCEPT;
                   break;     /* Breaks out of "for" statement, not "case". */
                }
             }
          }
          else
          {
             status = CAD_ACCEPT;
          }

          if (status == CAD_REJECT)
          {
             strncpy(buff, "Unknown filter name (B): ", MAX_STRING_SIZE-1);
             strncat(buff, fname2, MAX_STRING_SIZE - strlen(buff));
             strncpy(pcad->mess, buff, MAX_STRING_SIZE-1);
             return(status);
          }

/*
 * If the filters have a non-zero effective wavelength determine the focus offset
 * corresponding to that wavelength. This offset will be used when GMOS is
 * used for imaging. (If the call to gmosLutApply fails, don't exit, just
 * set offset focus value to zero.)
 */

          if ( lambda1 > 0.0 )
          {
             ret = gmosLutApply(lambda1, &lambdaFocusLut, &lambdaOffset1);
             if (ret != GMOSLUT_S_OK)
             {
                gmosLutMessage(ret, STRING_BUF_SZ, buff);
                lambdaOffset1 = 0.0;
             }
          }
          else
          {
             lambdaOffset1 = 0.0;
          }

          if ( lambda2 > 0.0 )
          {
             ret = gmosLutApply(lambda2, &lambdaFocusLut, &lambdaOffset2);
             if (ret != GMOSLUT_S_OK)
             {
                gmosLutMessage(ret, STRING_BUF_SZ, buff);
                lambdaOffset2 = 0.0;
             }
          }
          else
          {
             lambdaOffset2 = 0.0;
          }

/* Output the barcode values for further checking by CC */

          strncpy((char *)pcad->vala, barcodeId1, MAX_STRING_SIZE-1);
          strncpy((char *)pcad->valb, barcodeId2, MAX_STRING_SIZE-1);
          break ;

       case menuDirectiveSTART:

/* Filters are OK, so output focus offsets and original filter names */

          *(double *) pcad->valc = focusOffset1;
          *(double *) pcad->vald = focusOffset2;
          *(double *) pcad->vale = lambdaOffset1;
          *(double *) pcad->valf = lambdaOffset2;
          strncpy(pcad->valg, fname1, MAX_STRING_SIZE-1);
          strncpy(pcad->valh, fname2, MAX_STRING_SIZE-1);
          status = CAD_ACCEPT;
   
          DBGMSG(DBG_FULL,"CAD fltPos. Output values A..H:");
          DBGMSGSTRING(DBG_FULL,"A: ", (char *)pcad->vala);
          DBGMSGSTRING(DBG_FULL,"B: ",(char *)pcad->valb);
          DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
          DBGMSGREAL(DBG_FULL,"D: ",*(double *)pcad->vald); 
          DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
          DBGMSGREAL(DBG_FULL,"F: ",*(double *)pcad->valf); 
          DBGMSGSTRING(DBG_FULL,"G: ",pcad->valg);
          DBGMSGSTRING(DBG_FULL,"H: ",pcad->valh);
   
          break;

       case menuDirectiveMARK :

          status = CAD_ACCEPT;
          break ;

       case menuDirectiveSTOP :

          status = CAD_REJECT;
          strncpy(pcad->mess, "STOP directive not implemented", MAX_STRING_SIZE-1);
          break ;

       case menuDirectiveCLEAR :

          status = CAD_ACCEPT;
          break ;
   }

   return(status);
}


/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqLambdaFocusLUTread 
 *
 *   Purpose:
 *   Read the wavelength against focus lookup table file
 *
 *   Description:
 *   Open and read each line of the wavelength against focus lookup 
 *   table file. Initialise a suitable LUT structure for later use with the
 *   gmosLutxxx routines (e.g. gmosLutApply)
 *
 *    The wavelength against focus lookup file has the following format:
 *
 *     First line: "lambdaFocus"
 *     Comment lines starting with '#'
 *     Data lines of 2 real numbers: wavelength in nanometres
 *     followed by focus offset in microns.
 *
 *
 *   Invocation:
 *   gmSeqLambdaFocusLUTread( char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output) 
 *   (>) lutfilename (string) Name of lut file 
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing/illegal data values
 * 
 *   External functions:
 *   gmosLutCheck
 *
 *-
 */

long gmSeqLambdaFocusLUTread( char * lutfilename )
{
   double wavelength, offset;
   int ret;
   int nline = 0;
   FILE *lfFile;
   char line[STRING_BUF_SZ];
   char mess[STRING_BUF_SZ];
 
/* Open the file and check that the first line is "lambdaFocus" */

   lfFile = fopen(lutfilename,"r");
   if (lfFile == NULL)
   {
       printf("gmSeqLambdaFocusLUTread: Error opening file %s\n", lutfilename);
       return CAD_REJECT;
   }
  
   fgets(line, STRING_BUF_SZ, lfFile);  
   if (strncmp(line, "lambdaFocus", 11) != 0)
   {
      printf("gmSeqLambdaFocusLUTread: Error in header line (%s) for file %s\n", line, lutfilename);
      return CAD_REJECT;
   }
  
/* Read each line, skipping comment and blank lines */

   while ((fgets(line, STRING_BUF_SZ, lfFile)) != NULL)
   {
      if ((strlen(line) > 1) && (line[0] != '#'))
      {
         sscanf(line,"%lf %lf", &wavelength, &offset);

/*
 * Some basic sanity checking for the values read. The lookup table can have entries
 * just outside the normal GMOS wavelength range. Focus offsets in excess of 5000 microns
 * are suspect.
 */

         if ( ((wavelength < (MIN_WAVELENGTH-200.0)) || (wavelength > (MAX_WAVELENGTH+200.0)) ||
               (fabs(offset) > 5000.0)) )
         {
            printf("gmSeqLambdaFocusLUTread: Invalid data in %s: %s\n", lutfilename, line);
         }
         else
         {

/* Pairs of values now stored in the gmos LUT structure */

            lambdaFocusLut.input[nline]   = wavelength;
            lambdaFocusLut.measured[nline] = offset;
            nline++;
            if (nline >= MAX_GMOS_LUT_ENTRIES)
            {
               printf("gmSeqLambdaFocusLUTread: Too many LUT entries in file %s\n", lutfilename);
               break;
            }
         }
      }
   }
   lambdaFocusLut.numberOfEntries = nline;   /* Total number of entries */

#ifdef VERBOSE
   printf("gmSeqLambdaFocusLUTread: read %d entries OK\n", nline);
#endif

/* gmosLutCheck checks for monotonic X values in LUT data */

   ret = gmosLutCheck(&lambdaFocusLut);
   gmosLutMessage(ret, STRING_BUF_SZ, mess);
  
   fclose(lfFile);   /* Close the LUT data file */
  
   if (ret != GMOSLUT_S_OK)
   {
       printf("gmSeqLambdaFocusLUTread: gmosLutCheck: %s\n", mess);
       return CAD_REJECT;
   }
   else
   {
       return CAD_ACCEPT;
   }
}


/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqGratingLUTread 
 *
 *   Purpose:
 *   Initialise the grating database
 *
 *   Description:
 *   Read the grating LUT data 
 *   file and set up an internal list of grating names with their 
 *   attributes (barcode, ruling density and blaze orientation)
 *
 *   Invocation:
 *   gmSeqGratingLUTread(  char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output) 
 *   (>) lutfilename (string) Name of lut file 
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqGratingLUTread( char * lutfilename )
{
    FILE *fp;                /* LUT data file */
    char buf[STRING_BUF_SZ]; /* Input buffer */
    char tag[LUT_TAG_SZ];    /* Tag (= grating) name */
    GRATLUT *p;              /* Look-up table data structure */
    long n;
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    gratingInitOK = FALSE;

/* Free up old list if this is not the first entry */

    if (gmSeqGratLutPtr != NULL) 
    {
       lstFree((LIST *) gmSeqGratLutPtr);
       free(gmSeqGratLutPtr);
       gmSeqGratLutPtr = NULL;
    }
    
/* Read data file from remote disk */

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
        printf("gmSeqGratingLUTread: Failed to open file %s\n", lutfilename);
        return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"gmSeqGratingLUTread: Opened grating data file: ", lutfilename);
    
    gmSeqGratLutPtr = (void *) malloc (sizeof (LIST));
    if (gmSeqGratLutPtr == NULL)    /* malloc failed */
    {
        printf("gmSeqGratingLUTread: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }

    lstInit ((LIST *) gmSeqGratLutPtr);
    
    lcount = 0;
    while (status == 0)
    {

       /* skip blank lines and comments */

       n = fscanf (fp, "%39s", tag);     /* Format size should be LUT_TAG_SZ-1 */
       lcount++;
       if (n == 0 || tag[0] == '#')
       {
          (void) fgets (buf, sizeof (buf)-1, fp);
       }
       else if (n == EOF)
       {
          /* End of File. Break out of loop. */
          break;
       }
       else
       {
          /* create new node, read tag and number of values */

          p = (GRATLUT *) malloc (sizeof (GRATLUT));
          if (p == NULL)    /* malloc failed */
          {
             printf("gmSeqGratingLUTread: LUT node malloc failed\n");
             fclose(fp);
             return CAD_REJECT;
          }

          lstAdd ((LIST *) gmSeqGratLutPtr, (NODE *) p);
          strncpy (p->tag, tag, LUT_TAG_SZ-1);
          if (fscanf(fp,"%ld%ld%ld%lf", &(p->barcodeId), 
                     &(p->linesPerMm), &(p->blazeDir), &(p->focusOffset)) != 4 )
          {
             printf("gmSeqGratingLUTread: Missing value(s) in %s line %ld\n",
                         lutfilename, lcount);
             status = CAD_REJECT;
          }
          else
          {
             DBGMSGINT(DBG_FULL,"gmSeqGratingLUTread: Data line read OK: ",lcount);
          }
        
       }
    }

    fclose (fp);
    if (status == CAD_ACCEPT) gratingInitOK = TRUE;

    return status;
}


/*+
 *   Function name:
 *   gmSeqCadInitGrating 
 *
 *   Purpose:
 *   Initialise the grating CAD 
 *
 *   Description:
 *   Called at EPICS iocInit. Calls gmSeqGratingLUTread
 *
 *   Invocation:
 *   gmSeqCadInitGrating(pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 *
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqCadInitGrating(struct cadRecord *pcad)
{ 
    long status = CAD_ACCEPT;

#ifdef VERBOSE    
    printf("Called gmSeqCadInitGrating. DIR = %d\n", pcad->dir);
#endif

    return status;
}

/*+
 *   Function name:
 *   gmSeqCadGrating 
 *
 *   Purpose:
 *   Implements the grSelect command
 *
 *   Description:
 *   This routine is the process subroutine for the grSelect CAD.
 *   It translates the input grating name by looking up the name in
 *   the grating look-up table. The resuting barcode ID is written to the
 *   GMOS Components Controller grAssembly record. The specified 
 *   central wavelength and order is converted into a grating tilt angle.
 *
 *   Invocation:
 *   gmSeqCadGrating (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Grating positioning mode as a string :
 *             SEL  = Select named grating only
 *             WLEN = Tilt named grating to specified position
 *      b => Grating name as a string
 *      c => Central wavelength (nanometres) (double)
 *      d => Grating order (integer)
 *      e => Effective wavelength for focus offset (nanometres) (double)
 *
 *   EPICS output parameters:
 *      vala => Selection mode (long) :
 *               0 = Select grating and set angle
 *               1 = Select grating only
 *      valb => translated Barcode ID for grating (string)
 *      valc => Grating tilt angle in rads.  (double)
 *      vald => Name of grating (string)
 *      vale => Desired central wavelength in nanometres (double)
 *      valf => Requested grating order (long)
 *      valg => Grating ruling density (long)
 *      valh => Grating blaze orientation (long)
 *      vali => Focus offset corresponding to wavelength in microns (double)
 *      valj => Focus offset due to grating surface curvature in microns (double)
 *      valk => Desired effective wavelength in nanometres (double)
 *
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *   gratingSci2Tilt() Convert central wavelength and grating order 
 *                     into a grating tilt angle (written 
 *                     by Nigel Dipper, Durham University)
 *
 *   Deficiencies:
 *   The "select only" mode will not necessarily get the grating focus offset right,
 *   since the wavelength that the grating was originally selected with is not known
 *   to the instrument sequencer. Instead the user can supply an effective wavelength.
 *-
 */


long gmSeqCadGrating(struct cadRecord *pcad)
{ 
 
    long status, ret;                   /* Return status */
    GRATLUT *p;
    char buff[STRING_BUF_SZ];
    static char selectModeIn[MAX_STRING_SIZE];
    static char gratingName[MAX_STRING_SIZE] = "unknown";
    static double cenWavelength;        /* Central wavelength for grating tilt   */
    static double effWavelength;        /* Effective wavelength for focus offset */
    static double lambdaOffset;         /* Focus offset interpolated from LUT    */
    static double focusOffset;          /* Focus offset for this grating         */
    static long gratingOrder;           /* Grating order required                */
    char wavelengthString[MAX_STRING_SIZE];
    char gratingOrderString[MAX_STRING_SIZE];    
    static long selectModeOut;
    static char barcodeId[ MAX_STRING_SIZE];
    static double tiltAngle;
    static long linesPerMm;
    static long blazeDir;
    static long isMirror = FALSE;

    status = CAD_REJECT ;
        
    DBGMSGINT(DBG_MIN,"CAD grSelect. Directive = ", pcad->dir);

    switch (pcad->dir) 
    {

       case menuDirectivePRESET:

          if (!gratingInitOK) 
          {
             strncpy(pcad->mess, "Grating name translation not initialised",
                     MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }

/* Set the appropriate global test conditions for this CAD */

          gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);

/* Read in the grating selection mode, upper case then convert to integer */

          strncpy(selectModeIn, (char *)pcad->a, MAX_STRING_SIZE-1);
          gmSeqUc(selectModeIn, strlen(selectModeIn) + 1, selectModeIn);
         
          if (strncmp(selectModeIn, "SEL", 3) == 0)
          {
             selectModeOut = 1;
          }
          else if (strncmp(selectModeIn, "WLEN", 4) == 0)
          {
             selectModeOut = 0;
          }
          else
          {
             strncpy(pcad->mess,"grSelect mode not SEL or WLEN", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          } 
    
          strncpy(gratingName, (char *)pcad->b, MAX_STRING_SIZE-1);

          if (gratingName[0])
          {
/*
 * Try to translate the 1st input grating name. Read through all grating lookup
 * table entries, searching for specified name. The search will stop immediately
 * if gmSeqGratLutPtr is NULL.
 */

             for (p = (GRATLUT *) lstFirst ((LIST *)gmSeqGratLutPtr); p != NULL;
                  p = (GRATLUT *) lstNext ((NODE *) p))
             {

                if (strncmp (p->tag, gratingName, LUT_TAG_SZ) == 0)
                {
                   sprintf(barcodeId, "%10ld%1c", p->barcodeId,'\0');
                   linesPerMm = p->linesPerMm;
                   blazeDir = p->blazeDir;
                   focusOffset = p->focusOffset;
                   status = CAD_ACCEPT;
                   break;     /* Breaks out of "for" statement, not "case". */
                }
             }
          }
          else
          {
            strncpy(pcad->mess,"Grating name not specified", MAX_STRING_SIZE-1);
            return CAD_REJECT;
          }
    
          if (status == CAD_REJECT)
          {
             strncpy(buff, "Unknown grating name: ", MAX_STRING_SIZE-1);
             strncat(buff, gratingName, MAX_STRING_SIZE - strlen(buff));
             strncpy(pcad->mess, buff, MAX_STRING_SIZE-1);
             return(status);
          }

/*
 * The named grating is assumed to be a mirror if it is named "MIRROR",
 * "mirror" or its ruling density is zero.
 */

          gmSeqUc(gratingName, strlen(gratingName) + 1, gratingName);
          isMirror = ((strncmp(gratingName,"MIRROR",6) == 0) || (linesPerMm == 0));

          if (isMirror)
          {
             status = CAD_ACCEPT;

/* Set the wavelength related grating data to zero for mirror and specify order zero */

             gratingOrder  = 0;
             cenWavelength = 0.0;
             effWavelength = 0.0;
             lambdaOffset  = 0.0;
             linesPerMm    = 0;
             blazeDir      = 0;

/* Calculate the grating angle required for zero order reflection. */

             if ((gratingSci2Tilt(cenWavelength, gratingOrder, (double)linesPerMm, 
                  &tiltAngle)) != 0)
             {
                strncpy(pcad->mess, "Failed to calculate grating angle",
                        MAX_STRING_SIZE-1);
                return CAD_REJECT; 
             }
          }
          else
          {
             status = CAD_ACCEPT;

/* Check for WLEN (select grating, wavelength and order) or SEL (select grating only) mode */

             if (selectModeOut == 0)
             {

/* WLEN mode - get central wavelength and order and calculate angle */

/* Check that the grating order is a valid integer, and positive */

                strncpy(gratingOrderString, (char *)pcad->d, MAX_STRING_SIZE-1);
                if (sscanf(gratingOrderString, "%ld", &gratingOrder) != 1)
                {
                   strncpy(pcad->mess, "Illegal value for grating order ",
                           MAX_STRING_SIZE-1);
                   return CAD_REJECT; 
                }
                else if (gratingOrder < 0 || gratingOrder > 10)
                {
                   strncpy(pcad->mess, "Illegal value for grating order ",
                           MAX_STRING_SIZE-1);
                   return CAD_REJECT; 
                }

/*
 * If the grating order is greater than zero, check that specified central wavelength
 * is a valid number and in-range. If the grating order is zero the central wavelength
 * is ignored.
 */

                if ( gratingOrder > 0 )
                {
                   strncpy(wavelengthString, (char *)pcad->c, MAX_STRING_SIZE-1);

                   if (sscanf(wavelengthString, "%lf", &cenWavelength) != 1)
                   {
                      strncpy(pcad->mess, "Illegal value for wavelength ",
                              MAX_STRING_SIZE-1);
                      return CAD_REJECT; 
                   }
                   else if (cenWavelength < MIN_WAVELENGTH ||
                            cenWavelength > MAX_WAVELENGTH)
                   {
                      strncpy(pcad->mess, "Wavelength value out-of-range",
                              MAX_STRING_SIZE-1);
                      return CAD_REJECT; 
                   }
/*
 * Get the effective wavelength for the focus offset. If not specified,
 * this is assumed to be the same as the central wavelength.
 */       

                   strncpy(wavelengthString, (char *)pcad->e, MAX_STRING_SIZE-1);
                   if (sscanf(wavelengthString, "%lf", &effWavelength) != 1)
                   {
                      effWavelength = cenWavelength;
                   }
                   else if (effWavelength < MIN_WAVELENGTH ||
                            effWavelength > MAX_WAVELENGTH)
                   {
                      effWavelength = cenWavelength;
                   }
                }
                else
                {
 /*
  * Zero order - central and effective wavelengths have no meaning.
  */

                   cenWavelength = 0.0;
                   effWavelength = 0.0;
                }

#ifdef VERBOSE
                printf ("Central and effective wavelengths: %f %f\n", cenWavelength, effWavelength);
#endif

/*
 * Get the interpolated value for the focus offset at this effective
 * wavelength. (If the call to gmosLutApply fails, don't exit, just
 * set offset focus value to zero.)
 */

                if ( effWavelength >= MIN_WAVELENGTH )
                {
                   ret = gmosLutApply(effWavelength, &lambdaFocusLut, &lambdaOffset);
                   if (ret != GMOSLUT_S_OK)
                   {
                      gmosLutMessage(ret, STRING_BUF_SZ, buff);
                      lambdaOffset = 0.0;
                   }
                }
                else
                {
                   lambdaOffset = 0.0;
                }

/*
 * Calculate grating tilt angle required to achieve the specified central
 * wavelength and order. Flip the order if the blaze direction is negative.
 */

                if ( blazeDir >= 0 )
                {
                   if ((gratingSci2Tilt(cenWavelength, gratingOrder, (double)linesPerMm, 
                        &tiltAngle)) != 0)
                   {
                      strncpy(pcad->mess, "Failed to calculate grating angle",
                              MAX_STRING_SIZE-1);
                      return CAD_REJECT; 
                   }
                }
                else
                {
                   if ((gratingSci2Tilt(cenWavelength, -gratingOrder, (double)linesPerMm, 
                        &tiltAngle)) != 0)
                   {
                       strncpy(pcad->mess, "Failed to calculate grating angle",
                               MAX_STRING_SIZE-1);
                       return CAD_REJECT; 
                   }
                }
             }
             else
             {

/*
 * SEL mode - get effective wavelength only and set angle to zero.
 */

                tiltAngle = 0.0;

/*
 * Get the effective wavelength for the focus offset from fields E or C.
 * Set order to 1 (spectroscopy mode) if valid wavelength specified or 0 if not specified.
 */       

                strncpy(wavelengthString, (char *)pcad->e, MAX_STRING_SIZE-1);
                if (sscanf(wavelengthString, "%lf", &effWavelength) != 1)
                {
                   strncpy(wavelengthString, (char *)pcad->c, MAX_STRING_SIZE-1);
                   if (sscanf(wavelengthString, "%lf", &effWavelength) != 1)
                   {
                      gratingOrder = 0;
                      effWavelength = 0.0;
                   }
                }

                if (effWavelength >= MIN_WAVELENGTH &&
                    effWavelength <= MAX_WAVELENGTH)
                {
                   gratingOrder = 1;

/*
 * Get the interpolated value for the focus offset at this effective
 * wavelength. (If the call to gmosLutApply fails, don't exit, just
 * set offset focus value to zero.)
 */

                   if ( effWavelength >= MIN_WAVELENGTH )
                   {
                      ret = gmosLutApply(effWavelength, &lambdaFocusLut, &lambdaOffset);
                      if (ret != GMOSLUT_S_OK)
                      {
                         gmosLutMessage(ret, STRING_BUF_SZ, buff);
                         lambdaOffset = 0.0;
                      }
                   }
                   else
                   {
                      lambdaOffset = 0.0;
                   }

                }
                else
                {
                   gratingOrder = 0;
                   effWavelength = 0.0;
                   lambdaOffset = 0.0;
                }
             }
          }

          if (status == CAD_ACCEPT)
          {

/*   Output the mode, barcode & tilt angle for checking by CC */

             *(long *)pcad->vala = selectModeOut;
             strncpy((char *)pcad->valb, barcodeId, MAX_STRING_SIZE-1);
             if (selectModeOut == 0) *(double *) pcad->valc = tiltAngle;
          }
          break ;

       case menuDirectiveSTART:

/* Copy out the input parameters for storing in the SAD etc.*/

          strncpy((char *)pcad->vald, gratingName, MAX_STRING_SIZE-1);
          *(double *) pcad->vale = cenWavelength;
          *(long *)pcad->valf = gratingOrder;
          *(long *)pcad->valg = linesPerMm;
          *(long *)pcad->valh = blazeDir;
          *(double *)pcad->vali = lambdaOffset;
          *(double *)pcad->valj = focusOffset;
          *(double *) pcad->valk = effWavelength;

          DBGMSG(DBG_FULL,"CAD grSelect. Output values A..K: ");
          DBGMSGINT(DBG_FULL,"A: ",*(long *)pcad->vala);
          DBGMSGSTRING(DBG_FULL,"B: ",(char *)pcad->valb);
          DBGMSGREAL(DBG_FULL,"C: ",*(double *)pcad->valc);
          DBGMSGSTRING(DBG_FULL,"D: ", (char *)pcad->vald); 
          DBGMSGREAL(DBG_FULL,"E: ",*(double *)pcad->vale);
          DBGMSGINT(DBG_FULL,"F: ",*(long *)pcad->valf);
          DBGMSGINT(DBG_FULL,"G: ",*(long *)pcad->valg);
          DBGMSGINT(DBG_FULL,"H: ",*(long *)pcad->valh);
          DBGMSGREAL(DBG_FULL,"I: ",*(double *)pcad->vali);
          DBGMSGREAL(DBG_FULL,"J: ",*(double *)pcad->valj);
          DBGMSGREAL(DBG_FULL,"K: ",*(double *)pcad->valk);

          status = CAD_ACCEPT;
          break;

       case menuDirectiveMARK :
          status = CAD_ACCEPT;
          break ;

       case menuDirectiveSTOP :
          status = CAD_REJECT;
          strncpy(pcad->mess, "STOP directive not implemented", MAX_STRING_SIZE-1);
          break ;

       case menuDirectiveCLEAR :
          status = CAD_ACCEPT;
          break ;
    }
    return(status);
}

long gmSeqCadReboot (struct cadRecord *pcad)
/*
 * Null routine called by reboot CAD record -
 * actual reboot is done by a subroutine record 
 * calling gmSeqReboot
 */
{

   return CAD_ACCEPT;
}

/*+
 *   Function name:
 *   gmSeqReboot
 *
 *   Purpose:
 *   Reboot the GMOS Instrument Sequencer
 *
 *   Description:
 *   Called by subroutine record to reboot Instrument Sequencer IOC
 *
 *   Invocation:
 *   gmSeqReboot(psub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   psub   (struct subRecord*)  Pointer to subroutine record structure
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *   (VxWorks rebootLib routine) reboot
 *
 *-
 */
long gmSeqReboot (struct subRecord *psub)
{ 
   printf ("*** REBOOTING ***\n");
   taskDelay(60);

   reboot(BOOT_QUICK_AUTOBOOT);
   return CAD_ACCEPT;
}


/*+
 *   Function name:
 *   gmSeqCadInit
 *
 *   Purpose:
 *   Implements the Sequencer init command
 *
 *   Description:
 *   This routine is the process subroutine for the init sequence
 *   command  CAD. It calls the initialisation routines for both the
 *   filter and grating CADs.
 *
 *   Invocation:
 *   gmSeqCadinit (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *   a => LUT directory
 *   b => filters lut file name
 *   c => gratings lut file name
 *   d => lamdafocus lut file name
 *   e => masks lut file name
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                 non-zero = either or both initialisation 
 *                 calls returned an error status
 *
 *   External functions:
 *
 *-
 */
long gmSeqCadInit (struct cadRecord *pcad)
{
     long fstatus, gstatus, mstatus;
     long status = CAD_ACCEPT;
     char lutName[STRING_BUF_SZ];
      
     switch (pcad->dir) 
     {

     case menuDirectivePRESET:   

          /* Set the appropriate global test conditions for this CAD */

          gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING);
          break;
    
     case menuDirectiveSTART:

          /* Initialise from the Mask, Filter and Grating databases */

          /*
           * Fish out the lut names from inputs a, b, c, d, e
           */

          strncpy (lutName, (char *)pcad->a, HALF_BUF_SZ);
          strcat  (lutName, "/");
          strncat (lutName, (char *)pcad->e, HALF_BUF_SZ-2);

          mstatus = gmSeqMaskLUTread(lutName);
          if (mstatus != CAD_ACCEPT)
          {
               strncpy(pcad->mess, "Error initializing Mask database",
                       MAX_STRING_SIZE-1);
               DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Mask database");
          }
          else
          {
               DBGMSG(DBG_MIN,"Initialized mask database OK");
          }

          strncpy (lutName, (char *)pcad->a, HALF_BUF_SZ);
          strcat  (lutName, "/");
          strncat (lutName, (char *)pcad->b, HALF_BUF_SZ-2);

          fstatus = gmSeqFilterLUTread(lutName);
          if (fstatus != CAD_ACCEPT)
          {
               strncpy(pcad->mess, "Error initializing Filter database",
                       MAX_STRING_SIZE-1);
               DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Filter database");
          }
          else
          {
               DBGMSG(DBG_MIN,"Initialized filter database OK");
          }

          strncpy (lutName, (char *)pcad->a, HALF_BUF_SZ);
          strcat  (lutName, "/");
          strncat (lutName, (char *)pcad->c, HALF_BUF_SZ-2);

          gstatus = gmSeqGratingLUTread(lutName);
          if (gstatus != CAD_ACCEPT)
          {
               strncpy(pcad->mess, "Error initializing Grating database",
                       MAX_STRING_SIZE-1);
               DBGMSG(DBG_MIN,"gmSeqCadInit: Error initialising Grating database");
          }
          else
          {
               DBGMSG(DBG_MIN,"Initialized grating database OK");
          }

          if ((mstatus != CAD_ACCEPT) || (fstatus != CAD_ACCEPT) || (gstatus != CAD_ACCEPT)) 
               status = CAD_REJECT;
          else
          {
               DBGMSG(DBG_MIN,"Initialized all three databases OK");
          }

          /* Ignore any errors reading lambdaFocus.lut */

          strncpy (lutName, (char *)pcad->a, HALF_BUF_SZ);
          strcat  (lutName, "/");
          strncat (lutName, (char *)pcad->d, HALF_BUF_SZ-2);

          (void) gmSeqLambdaFocusLUTread(lutName);

          break;
    
     case menuDirectiveMARK :
          status = CAD_ACCEPT;
          break ;

     case menuDirectiveSTOP :
          status = CAD_REJECT;
          strncpy(pcad->mess, "STOP directive not implemented", MAX_STRING_SIZE-1);
          break ;

     case menuDirectiveCLEAR :
          status = CAD_ACCEPT;
          break ;
     }
  
     return status;
}


long gmSeqCadSetup (struct cadRecord *pcad)
{
/*
 * For any CAD routines involved in setting up the instrument
 * not already covered by other functions.
 * Currently does nothing. Just set the appropriate 
 * global test conditions for this CAD
 */

    gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING);
    return CAD_ACCEPT;
}

long gmSeqCadTest (struct cadRecord *pcad)
{
/* Set global test conditions for TEST */
    gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING);
    return CAD_ACCEPT;
}

long gmSeqCadDatum (struct cadRecord *pcad)
{
/* Set global test conditions for DATUM */
    gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING+MASTER_ENABLE);
    return CAD_ACCEPT;
}

long gmSeqCadPark (struct cadRecord *pcad)
{
/* Set global test conditions for PARK */
    gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING+MASTER_ENABLE);
    return CAD_ACCEPT;
}


/*+
 *   Function name:
 *   gmSeqCadDebug
 *
 *   Purpose:
 *   Implements the Instrument Sequencer debug command
 *
 *   Description:
 *   This routine is the process subroutine for the debug
 *   command CAD. It sets the internal gmSeqDbgLevel flag to 
 *   the specified debug level and forwards the debug level
 *   to the components controller and detector controller.
 *   A flag may be used to prevent the debug level being forwarded,
 *   in order to debug the Instrument Sequencer alone.
 *
 *   Invocation:
 *   gmSeqCadDebug (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to CAD record structure
 *
 *   EPICS input parameters:
 *      a => Requested debug level : NONE, MIN or FULL (string)
 *      b => Instrument sequencer only flag            (long)
 *
 *   EPICS output parameters:
 *      vala => Componnets controller copy of the input debug level (string)
 *      valb => Detector controller copy of the input debug level   (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *
 *   External functions:
 *
 *-
 */
long gmSeqCadDebug (struct cadRecord *pcad)
{
    static char debugModeIn[MAX_STRING_SIZE];
    static long debugMode = DBG_NONE;
    static long isOnlyFlag = 0;
    long status;

    status = CAD_ACCEPT;

    switch (pcad->dir) 
    {

          case menuDirectivePRESET:

/* Read in the debug mode, upper case, then check validity */

          strncpy(debugModeIn, (char *) pcad->a, MAX_STRING_SIZE-1);
          gmSeqUc(debugModeIn, strlen(debugModeIn) + 1, debugModeIn);
         
          if (strncmp(debugModeIn, "QUIET", 5) == 0)
          {
             debugMode = DBG_QUIET;
          }
          else if (strncmp(debugModeIn, "NONE", 4) == 0)
          {
             debugMode = DBG_QUIET | DBG_NONE;
          }
          else if (strncmp(debugModeIn, "MIN", 3) == 0)
          {
             debugMode = DBG_QUIET | DBG_NONE | DBG_MIN;
          }
          else if (strncmp(debugModeIn, "FULL", 4) == 0)
          {
             debugMode = DBG_QUIET | DBG_NONE | DBG_MIN | DBG_FULL;
          }
          else if (strncmp(debugModeIn, "MAX", 3) == 0)
          {
             debugMode = DBG_QUIET | DBG_NONE | DBG_MIN | DBG_FULL | DBG_MAX;
          }
          else
          {
             strncpy(pcad->mess,"Unrecognised debug mode", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          } 

/*
 * Read in the "Instrument Sequencer only" flag, if specified.
 */

          if ( pcad->b != NULL )
          {
             if ( sscanf ((char *)pcad->b, "%ld", &isOnlyFlag ) != 1)
             {
                isOnlyFlag = 0;
             }
          }
          else
          {
             isOnlyFlag = 0;
          }
/*
 * Copy the debugging mode to the components controller.
 */

          if ( !isOnlyFlag )
          {
             strncpy((char *)pcad->vala, debugModeIn, MAX_STRING_SIZE-1);
          }

/*
 * The detector controller does not recognise the same debugging levels
 * as GMOS. Convert QUIET mode into NOLOG mode and MAX mode into FULL.
 */
          if ( !isOnlyFlag )
          {
             if (strncmp(debugModeIn, "QUIET", 5) == 0)
             {
                strcpy((char *)pcad->valb, "NOLOG");
             }
             else if (strncmp(debugModeIn, "MAX", 3) == 0)
             {
                strcpy((char *)pcad->valb, "FULL");
             }
             else
             {
                strncpy((char *)pcad->valb, debugModeIn, MAX_STRING_SIZE-1);
             }
          }
          break;
    
       case menuDirectiveSTART:

/* Set the debug level global variable */

          gmSeqDbgLevel = debugMode;
          DBGMSGINT(DBG_MIN,"Instrument Sequencer DEBUG level set to : ", gmSeqDbgLevel);

/* Reset the "Instrument Sequencer only" flag. */

          isOnlyFlag = 0;
          break;

       case menuDirectiveMARK :
          status = CAD_ACCEPT;
          break ;

       case menuDirectiveSTOP :
          status = CAD_REJECT;
          strncpy(pcad->mess,"STOP directive not implemented", MAX_STRING_SIZE-1);
          break ;

       case menuDirectiveCLEAR :
          status = CAD_ACCEPT;
          break ;
   }
    
   return status;
}


long gmSeqNullInit (struct subRecord *psub)
/*
 * Null CAD initialisation, required by subroutine records.
 */

{
   return 0;
}

/*+
 *   Function name:
 *   gmSeqConfigBegin
 *
 *   Purpose:
 *   Implements the start of configuration apply handling.
 *
 *   Description:
 *   This routine is the subroutine invoked by the SubCAD
 *   record triggered as the first "CAD" from the top-level 
 *   apply record. It decides whether the apply command triggering
 *   can progress by checking the interlock status. It also clears
 *   the test condition mask, which will be set by subsequent CADs 
 *   triggered by the apply record.
 *
 *   Invocation:
 *   gmSeqConfigBegin (subcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   subcad   (struct subCadRecord*)  Pointer to subCad record structure
 *
 *   EPICS input parameters:
 *      a => Gemini system interlock status (1 = TRUE)
 *
 *   EPICS output parameters:
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                            -1 = Interlock set
 * 
 *   External functions:
 *
 *-
 */
long gmSeqConfigBegin (struct cadRecord *cad)
{
    char interlock[MAX_STRING_SIZE];
    
    switch (cad->dir) 
    {

       case menuDirectiveCLEAR:
       case menuDirectivePRESET:

/* Clear the CAD condition test mask for PRESET and CLEAR */

          gmSeqClearCADTest();
    
/* Directive START : check for interlock */
       case menuDirectiveSTART:

          strncpy(interlock, cad->a, MAX_STRING_SIZE-1);
    
          DBGMSGSTRING(DBG_FULL,"ConfigBegin: interlock status = ", interlock);
    
          if (strncmp(interlock, "OK", 2))
          {
             strncpy(cad->mess, "GMOS is interlocked", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }

    }
    return CAD_ACCEPT;
}


/*+
 *   Function name:
 *   gmSeqConfigEnd
 *
 *   Purpose:
 *   Implements the end of configuration apply handling.
 *
 *   Description:
 *   This routine is the subroutine invoked by the SubCAD
 *   record triggered as the last "CAD" from the top-level 
 *   apply record. It decides whether the apply command triggering
 *   can progress by checking the specified test conditions as indicated
 *   by the CADtest mask value. These may include configuration, detector
 *   readout and observing in progress status checks. If any of these 
 *   are preventing the apply being processed then an error status 
 *   is returned. The input CAD directive is output, to be forwarded 
 *   to the subsystem apply record.
 *
 *   Invocation:
 *   gmSeqConfigEnd (subcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   subcad   (struct subCadRecord*)  Pointer to subCAD record structure
 *
 *   EPICS input parameters:
 *      dir => Directive to be forwarded to the subsystem apply record
 *      a => GMOS applyC CAR status value.
 *      b => Whether observing is in progress (observeC CAR value)
 *      c => Whether Detector controller is disconnected
 *      d => Whether GMOS detector controller readout 
 *              is currently in progress (gm:dc:rdout 1=TRUE)
 *      e => Whether GMOS "master enable" has been switched on
 *      
 *
 *   EPICS output parameters:
 *      vala => A copy of the input directive field (long)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqConfigEnd (struct cadRecord *cad)
{
    long dir;
    char applyC_str[MAX_STRING_SIZE];
    char observeC_str[MAX_STRING_SIZE];
    char readout_str[MAX_STRING_SIZE];
    char menable_str[MAX_STRING_SIZE];
    long readout;
    int testMask, n;
    int DCdisconnected = FALSE;  /* Is detector controller disconnected? */
    char DCdisconnected_str[MAX_STRING_SIZE];
    
/* Condition flags */
    int reading_out   = FALSE;
    int configuring   = FALSE;
    int observing     = FALSE;
    int master_enable = FALSE;

/* Always get the current CAD test condition mask */
    testMask = gmSeqGetCADTest();
    DBGMSGINT(DBG_FULL, "configEnd: testMask = ", testMask);

/* Read in the applyC values from the subsystems .
   Items at A and B are strings representing CAR state  */
    strncpy(applyC_str, cad->a, MAX_STRING_SIZE-1);
    DBGMSGSTRING(DBG_FULL, "A = ", applyC_str);
    
/* Read the OBSERVING condition value */    
    strncpy(observeC_str, cad->b, MAX_STRING_SIZE-1);
    DBGMSGSTRING(DBG_FULL, "B = ", observeC_str);
     
/* Read the DC DISCONNECTED condition value : an integer */    
    strncpy(DCdisconnected_str, cad->c, MAX_STRING_SIZE-1);
    DBGMSGSTRING(DBG_FULL, "C = ", DCdisconnected_str);
    n = sscanf(DCdisconnected_str, "%d", &DCdisconnected);
    if (n != 1)  DCdisconnected = 0;    /* No valid integer found? */           
    DBGMSGINT(DBG_FULL," DCdisconnected = ", DCdisconnected);   
        
/* Read in the READING OUT condition value; an integer */
    strncpy(readout_str, cad->d, MAX_STRING_SIZE-1);
    DBGMSGSTRING(DBG_FULL, "D = ", readout_str);
    n = sscanf(readout_str, "%ld", &readout);
    if (n != 1) readout = 0;    /* No valid integer found? */    
    DBGMSGINT(DBG_FULL,"readout = ", readout);

/* Read in the MASTER ENABLE condition value; an integer */
    strncpy(menable_str, cad->e, MAX_STRING_SIZE-1);
    DBGMSGSTRING(DBG_FULL, "E = ", menable_str);
    n = sscanf(menable_str, "%d", &master_enable);
    if (n != 1) master_enable = 0;    /* No valid integer found? */    
    DBGMSGINT(DBG_FULL,"master enable = ", master_enable);


/* Determine the current condition of the system. */
    if (readout != 0 && !DCdisconnected)  /* Ignore reading out status if disconnected */
    {
       reading_out= TRUE;
       DBGMSG(DBG_FULL,"GMOS is READING OUT");
    }
      
    else if (strncmp(observeC_str,"BUSY",4) == 0)
    {
       observing = TRUE;
       DBGMSG(DBG_FULL,"GMOS is OBSERVING");
    }

    else if (strncmp(applyC_str,"BUSY",4) == 0)
    {
       configuring = TRUE;
       DBGMSG(DBG_FULL,"GMOS is CONFIGURING");
    }
             
    switch (cad->dir) 
    {

/* PRESET only: Check whether configuring and  whether master enabled, if specified
   by CONFIGURING or MASTER_ENABLE in the test mask */

       case menuDirectivePRESET:

          if ( ((testMask & CONFIGURING) != 0) && (configuring == TRUE) ) 
          {
             DBGMSG(DBG_FULL,"GMOS PRESET fail: configuring interlock");
             strncpy(cad->mess, "GMOS currently reconfiguring", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }

          if ( ((testMask & MASTER_ENABLE) != 0) && (master_enable == FALSE) ) 
          {
             DBGMSG(DBG_FULL,"GMOS PRESET fail: no master enable interlock");
             strncpy(cad->mess, "No GMOS master enable - still booting?", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }


/* START: Perform any other checks (reading out, observing)
   as specified in the test mask (Values READING_OUT and OBSERVING) */

       case menuDirectiveSTART: 
    
          if ( ((testMask & READING_OUT) != 0) && (reading_out == TRUE) )
          {
             DBGMSG(DBG_FULL,"GMOS START fail: readout interlock");
             strncpy(cad->mess, "GMOS detector currently reading out", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }
      
          if ( ((testMask & OBSERVING) != 0) && (observing == TRUE) )
          {
             DBGMSG(DBG_FULL,"GMOS START fail: observing interlock");
             strncpy(cad->mess, "GMOS currently observing", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }    
     
       default:

/* Always copy out the directive to VALA */

          dir = cad->dir;
          *(long *) cad->vala = dir;
    }
   
    return CAD_ACCEPT;
}


long gmSeqCadTrivial (struct cadRecord *pcad)

/*
 * Simple routine for GMOS sequence CAD record 'type A',
 * a trivial command which is not forwarded to CC or DC and 
 * there are no specific checks for accept/reject except that
 * the STOP directive is unimplemented.
 */
   
{   
    DBGMSGSTRING(DBG_MIN,"CAD ", pcad->name);
    DBGMSGINT(DBG_MIN,"Directive = ", pcad->dir);

    switch (pcad->dir) 
    {

       case menuDirectiveSTOP:
          strncpy(pcad->mess,"STOP directive not implemented", MAX_STRING_SIZE-1);
          return CAD_REJECT;
    
       default:
          return CAD_ACCEPT;
    }
}

/*+
 *   Function name:
 *   gmSeqCadObserve
 *
 *   Purpose:
 *   Implements the CAD for the 'observe' sequence command
 *
 *   Description:
 *   This routine is the process subroutine for the observe CAD.
 *   Simply passes on the dataset ID string for the detector controller
 *
 *   Invocation:
 *   gmSeqCadObserve (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to cad record structure
 *
 *   EPICS input parameters:
 *      a => dataset ID (string)
 *      d => whether detector controller enabled (long)
 *
 *   EPICS output parameters:
 *      vala => dataset ID to DC (string)
 * 
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadObserve(struct cadRecord *pcad)
{ 
 
    long status;           /* Return status */
    char datasetID[MAX_STRING_SIZE];
    long dcDisabled;

    DBGMSGINT(DBG_MIN,"CAD observe. Directive = ", pcad->dir);

    switch (pcad->dir) 
    {

       case menuDirectivePRESET:

/* Reject the command if the detector controller is disabled */

          sscanf( (char *)pcad->d, "%ld", &dcDisabled);
          DBGMSGINT(DBG_FULL,"CAD observe. DC disabled = ", dcDisabled);
          if ( dcDisabled )
          {
             strncpy(pcad->mess, "Detector controller not available", MAX_STRING_SIZE-1);
             status = CAD_REJECT;
          }
          else
          {

             /* Set the appropriate global test conditions for this CAD */
             gmSeqSetCADTest(CONFIGURING+READING_OUT+OBSERVING);

             /* Read in the dataset ID */
             strncpy(datasetID, (char *)pcad->a, MAX_STRING_SIZE-1);

             /* Output the dataset ID string */
             strncpy((char *)pcad->vala, datasetID, MAX_STRING_SIZE-1);
   
             status = CAD_ACCEPT;
          }

          break ;

       case menuDirectiveSTART:

          DBGMSGSTRING(DBG_FULL,"A: ",(char *)pcad->vala);

          status = CAD_ACCEPT;
          break;

       case menuDirectiveMARK :
          status = CAD_ACCEPT;
          break ;

       case menuDirectiveSTOP :
          status = CAD_REJECT;
          strncpy(pcad->mess,"STOP directive not implemented", MAX_STRING_SIZE-1);
          break ;

       case menuDirectiveCLEAR :
          status = CAD_ACCEPT;
          break ;
   }
   
   return(status);
}


long gmSeqCadStopObserve (struct cadRecord *pcad)

/*
 * Simple routine for GMOS sequence CAD records which pauses or
 * stops an observation.
 */
   
{
    long status;           /* Return status */
    long dcDisabled;

    status = CAD_ACCEPT;

    switch (pcad->dir)
    {

       case menuDirectivePRESET:

/*      Reject the command if the detector controller is disabled */

          sscanf( (char *)pcad->d, "%ld", &dcDisabled);
          DBGMSGINT(DBG_FULL,"CAD stop observe. DC disabled = ", dcDisabled);
          if ( dcDisabled )
          {
             strncpy(pcad->mess, "Detector controller not available", MAX_STRING_SIZE-1);
             status = CAD_REJECT;
          }
          else
          {

/*         Set the appropriate global test condition for this CAD */

             gmSeqSetCADTest( 0 );
             status = CAD_ACCEPT;
          }

       default:
          /* Accept all other directives */
          status = CAD_ACCEPT;
    }
    return (status);
}

long gmSeqCadEndObserve (struct cadRecord *pcad)
/*
 * Simple routine for GMOS sequence CAD endObserve
 */
   
{
    switch (pcad->dir)
    {
       case menuDirectivePRESET:

/*       Set the appropriate global test condition for this CAD */ 
          gmSeqSetCADTest(READING_OUT+OBSERVING);
    }
    
    return CAD_ACCEPT;
}

long gmSeqCadContinue (struct cadRecord *pcad)

/*
 * Simple routine for GMOS continue sequence CAD record to
 * continue an observation. The command should be rejected if 
 * a reconfiguration is in progress or the detector controller
 * is not available.
 */
   
{
    long status;           /* Return status */
    long dcDisabled;

    status = CAD_ACCEPT;

    switch (pcad->dir)
    {
       case menuDirectivePRESET:

/*       Reject the command if the detector controller is disabled */

          sscanf( (char *)pcad->d, "%ld", &dcDisabled);
          DBGMSGINT(DBG_FULL,"CAD continue. DC disabled = ", dcDisabled);
          if ( dcDisabled )
          {
             strncpy(pcad->mess, "Detector controller not available", MAX_STRING_SIZE-1);
             status = CAD_REJECT;
          }
          else
          {

/*         Set the appropriate global test condition for this CAD */

             gmSeqSetCADTest(CONFIGURING);
             status = CAD_ACCEPT;
          }

       default:
          /* Accept all other directives */
          status = CAD_ACCEPT;
    }
    
    return (status);
}


/*+
 *   Function name:
 *   gmSeqMaskLUTread
 *
 *   Purpose:
 *   Initialise the mask CAD database
 *
 *   Description:
 *   Read the mask LUT data file 
 *   and set up an internal list of mask names with their 
 *   attributes (barcode and focus offset)
 *
 *   Invocation:
 *   gmSeqMaskLUTread( char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)  lutfilename (string) name of masks lut file
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *-
 */

long gmSeqMaskLUTread( char * lutfilename )
{
    FILE *fp;                /* LUT data file */
    char buf[STRING_BUF_SZ]; /* Input buffer */
    char tag[LUT_TAG_SZ];    /* Tag (= mask) name */
    MASKLUT *p;              /* Look-up table data structure */
    long n;                  /* Status return for fscanf */
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    maskInitOK = FALSE;

/* Free up old list if this is not the first entry */

    if (gmSeqMaskLutPtr != NULL) 
    {
       lstFree((LIST *) gmSeqMaskLutPtr);
       free(gmSeqMaskLutPtr);
       gmSeqMaskLutPtr = NULL;
    }    
 
/* read data file from remote disk */

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
       printf("gmSeqMaskLUTread: Failed to open file %s\n", lutfilename);
       return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"gmSeqMaskLUTread: Opened mask lut file: ", lutfilename);
    
    gmSeqMaskLutPtr = (void *) malloc (sizeof (LIST));
    if (gmSeqMaskLutPtr == NULL)    /* malloc failed */
    {
        printf("gmSeqMaskLUTread: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }

    lstInit ((LIST *) gmSeqMaskLutPtr);
    
    lcount = 0;
    while (status == CAD_ACCEPT)
    {

       /* skip blank lines and comments */

       n = fscanf (fp, "%39s", tag);     /* Format size should be LUT_TAG_SZ-1 */
       lcount++;
       if (n == 0 || tag[0] == '#')
       {
          /* Throw away the rest of a comment line */
          (void) fgets (buf, sizeof (buf)-1, fp);
       }
       else if (n == EOF)
       {
          /* End of File. Break out of loop. */
          break;
       }
       else
       {

          /* create new node, read tag and number of values */

          p = (MASKLUT *) malloc (sizeof (MASKLUT));

          if (p == NULL)    /* malloc failed */
          {
             printf("gmSeqMaskLUTread: LUT node malloc failed\n");
             fclose(fp);
             return CAD_REJECT;
          }

          lstAdd ((LIST *) gmSeqMaskLutPtr, (NODE *) p);
          strncpy (p->tag, tag, LUT_TAG_SZ-1);
          if (fscanf(fp,"%ld", &(p->barcodeId)) != 1 )
          {
             printf("gmSeqMaskLUTread: Missing value(s) in %s line %ld\n",
                    lutfilename, lcount);
             status = CAD_REJECT;
          }
          else
          {
             DBGMSGINT(DBG_FULL,"gmSeqMaskLUTread: Data line read OK: ",lcount); 
          }
       } 
    }
    fclose (fp);
    if (status == CAD_ACCEPT) maskInitOK = TRUE;
    
    return status;
}


/*+
 *   Function name:
 *   gmSeqCadMask
 *
 *   Purpose:
 *   Implements the mskPos command
 *
 *   Description:
 *   This routine is the process subroutine for the mskPos CAD.
 *   It attempts to match the input string with a list of known
 *   mask names. If the string can be matched it is translated
 *   into a barcodeID. If the string cannot be matched it is assumed to
 *   be a barcode ID. The resulting barcode ID is written to the
 *   GMOS Components Controller mskAssembly record.
 *
 *   Invocation:
 *   gmSeqCadMask (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to cad record structure
 *
 *   EPICS input parameters:
 *      a => Mask or IFU name or ID (provided by OCS) (string)
 *      b => Mask/IFU location: In-BEAM or OUT-OF-BEAM (string)
 *           (Mask ID not used if OUT-OF-BEAM)
 *
 *   EPICS output parameters:
 *      vala => Target mask/IFU code
 *      valb => Target mask/IFU location as an integer:
 *               0 = in beam
 *               1 = out of beam
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */
long gmSeqCadMask(struct cadRecord *pcad)
{ 
 
    long status;           /* Return status */
    MASKLUT *p;
    static char maskname[MAX_STRING_SIZE] = "unknown";
    static char barcodeIDIn[MAX_STRING_SIZE];
    static char maskLocationIn[MAX_STRING_SIZE];
    static long barcodeID;
    static long maskLocation;
    int found;

    DBGMSGINT(DBG_MIN,"CAD mskPos. Directive = ", pcad->dir);

    switch (pcad->dir) 
    {

       case menuDirectivePRESET:

          if (!maskInitOK) 
          {
             strncpy(pcad->mess, "Mask name translation uninitialised",
                     MAX_STRING_SIZE-1);
             return CAD_REJECT;     /* Is this really a problem ? */
          }

/*       Set the appropriate global test conditions for this CAD */

          gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);

/*       Read in the mask location, upper case then convert to integer */

          strncpy(maskLocationIn, (char *)pcad->b, MAX_STRING_SIZE-1);
          gmSeqUc(maskLocationIn, strlen(maskLocationIn) + 1, maskLocationIn);
         
          if (strncmp(maskLocationIn, "IN-BEAM", 7) == 0)
          {
             maskLocation = 0;
          }
          else if (strncmp(maskLocationIn, "OUT-OF-BEAM", 11) == 0)
          {
             maskLocation = 1;
          }
          else
          {
             strncpy(pcad->mess,"Location not IN-BEAM/OUT-OF-BEAM", 
                     MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }
      
          status = CAD_ACCEPT;

/*       Read in either a mask name or barcode ID from input A if mask location is IN-BEAM */

          if (maskLocation == 0)
          {

             found = FALSE;

             strncpy(maskname, (char *)pcad->a, MAX_STRING_SIZE-1);
             if (maskname[0])
             {

/*
 * Try to translate mask name. Read through all mask lookup table entries,
 * searching for specified name. The search will stop immediately if
 * gmSeqMaskLutPtr is NULL.
 */

                for (p = (MASKLUT *) lstFirst ((LIST *)gmSeqMaskLutPtr); p != NULL;
                     p = (MASKLUT *) lstNext ((NODE *) p))
                {
                   if (strncmp (p->tag, maskname, LUT_TAG_SZ) == 0)
                   {
                      sprintf(barcodeIDIn, "%10ld%1c", p->barcodeId,'\0');
                      found = TRUE;
                      break;     /* Breaks out of "for" statement, not "case". */
                   }
                }
             }

/*
 * If a match has been found use the barcode translation, otherwise assume the
 * original string contained the barcode.
 */

             if ( !found )
             {
                strncpy(barcodeIDIn, (char *)pcad->a, MAX_STRING_SIZE-1);
             }
             if (sscanf(barcodeIDIn, "%ld", &barcodeID) != 1)
             {
                strncpy(pcad->mess,"Not a mask name or integer BarcodeID", MAX_STRING_SIZE-1);
                return CAD_REJECT;   
             }
             else
             {
                status = CAD_ACCEPT;
             }
          }

          if (status == CAD_ACCEPT)
          {

/*          Output the location and barcode ID  */
             if (maskLocation == 0) *(long *) pcad->vala = barcodeID;
             *(long *)pcad->valb = maskLocation;
          }
          break ;

      case menuDirectiveSTART:
   
         DBGMSG(DBG_FULL,"CAD mskPos. Output values A..B: ");
         DBGMSGINT(DBG_FULL,"A: ", *(long *)pcad->vala);
         DBGMSGINT(DBG_FULL,"B: ", *(long *)pcad->valb);

         status = CAD_ACCEPT;
         break;

      case menuDirectiveMARK :
         status = CAD_ACCEPT;
         break ;

      case menuDirectiveSTOP :
         status = CAD_REJECT;
         strncpy(pcad->mess,"STOP directive not implemented", MAX_STRING_SIZE-1);
         break ;

      case menuDirectiveCLEAR :
         status = CAD_ACCEPT;
         break ;
   }

   return(status);
}


/*+
 *   Function name:
 *   gmSeqCadAtmPos
 *
 *   Purpose:
 *   Implements the atmPos command
 *
 *   Description:
 *   This routine is the process subroutine for the atmPos CAD.
 *   Simply passes on the input prism angles unchanged to the 
 *   components controller atmAssembly record
 *
 *   Invocation:
 *   gmSeqCadAtmPos (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to cad record structure
 *
 *   EPICS input parameters:
 *      a => Entrance prism angle (double)
 *      b => Exit prism angle (double)
 *
 *   EPICS output parameters:
 *      vala => Entrance prism angle (double)
 *      valb => Exit prism angle (double)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */


long gmSeqCadAtmPos(struct cadRecord *pcad)
{ 
 
    long status;           /* Return status */
    char entranceAngleIn[MAX_STRING_SIZE];
    char exitAngleIn[MAX_STRING_SIZE];
    static double entranceAngle;
    static double exitAngle;

    DBGMSGINT(DBG_MIN,"CAD atmPos. Directive = ", pcad->dir);

    switch (pcad->dir) 
    {

       case menuDirectivePRESET:

/*       Set the appropriate global test conditions for this CAD */
          gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);

/*       Read in the entrance prism angle, convert to double */
          strncpy(entranceAngleIn, (char *)pcad->a, MAX_STRING_SIZE-1);
          if (!entranceAngleIn[0])
          {
              strncpy(pcad->mess,"Entrance prism angle missing", MAX_STRING_SIZE-1);
              return CAD_REJECT;
          }
 
          if (sscanf(entranceAngleIn, "%lf", &entranceAngle) != 1)
          {
              strncpy(pcad->mess,"Invalid entrance prism angle", MAX_STRING_SIZE-1);
              return CAD_REJECT;   
          }


          if (entranceAngle < ATMENPMIN || entranceAngle > ATMENPMAX )
          {
             strncpy(pcad->mess,"Entrance prism angle out-of-range", MAX_STRING_SIZE-1);
             return CAD_REJECT;   
          }

/*       Read in the exit prism angle, convert to double */
          strncpy(exitAngleIn, (char *)pcad->b, MAX_STRING_SIZE-1);
          if (!exitAngleIn[0])
          {
             strncpy(pcad->mess,"Exit prism angle missing", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }

          if (sscanf(exitAngleIn, "%lf", &exitAngle) != 1)
          {
             strncpy(pcad->mess,"Invalid exit prism angle", MAX_STRING_SIZE-1);
             return CAD_REJECT;   
          }

          if (exitAngle < ATMEXPMIN || exitAngle > ATMEXPMAX)
          {
             strncpy(pcad->mess,"Exit prism angle out-of-range", MAX_STRING_SIZE-1);
             return CAD_REJECT;   
          }

          status = CAD_ACCEPT;

/*      Output the prism angles */
         *(double *) pcad->vala = entranceAngle;
         *(double *) pcad->valb = exitAngle;
   
         break ;

      case menuDirectiveSTART:

         DBGMSG(DBG_FULL,"CAD atmPos. Output values A..B: ");
         DBGMSGREAL(DBG_FULL,"A: ", *(double *) pcad->vala);
         DBGMSGREAL(DBG_FULL,"B: ", *(double *) pcad->valb);

         status = CAD_ACCEPT;
         break;

      case menuDirectiveMARK :
         status = CAD_ACCEPT;
         break ;

      case menuDirectiveSTOP :
         status = CAD_REJECT;
         strncpy(pcad->mess,"STOP directive not implemented", MAX_STRING_SIZE-1);
         break ;

      case menuDirectiveCLEAR :
         status = CAD_ACCEPT;
         break ;
   }

   return(status);
}

/*+
 *   Function name:
 *   gmSeqCadDtaPos
 *
 *   Purpose:
 *   Implements the dtaPos command
 *
 *   Description:
 *   This routine is the process subroutine for the dtaPos CAD.
 *   Simply passes on the input detector translation assembly positions 
 *   to the components controller dtaAssembly record
 *
 *   Invocation:
 *   gmSeqCadDtaPos (pcad)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pcad   (struct cadRecord*)  Pointer to cad record structure
 *
 *   EPICS input parameters:
 *      a => X-axis position (double)
 *      b => Y-axis position (double)
 *      c => Z-axis position (double)
 *
 *   EPICS output parameters:
 *      vala => X-axis position (double)
 *      valb => Y-axis position (double)
 *      valc => Z-axis position (double) 
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 * 
 *   External functions:
 *
 *-
 */


long gmSeqCadDtaPos(struct cadRecord *pcad)
{ 
 
    long status;           /* Return status */
    char xPosIn[MAX_STRING_SIZE];
    char yPosIn[MAX_STRING_SIZE];
    char zPosIn[MAX_STRING_SIZE];
    static double xPos, yPos, zPos;

    DBGMSGINT(DBG_MIN,"CAD dtaPos. Directive = ", pcad->dir);

    switch (pcad->dir) 
    {

       case menuDirectivePRESET:

/*       Set the appropriate global test conditions for this CAD */
          gmSeqSetCADTest(OBSERVING+MASTER_ENABLE);

/*       Read in the X position, convert to double */
          strncpy(xPosIn, (char *)pcad->a, MAX_STRING_SIZE-1);
          if (!xPosIn[0])
          {
             strncpy(pcad->mess,"X position missing", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }
 
          if (sscanf(xPosIn, "%lf", &xPos) != 1)
          {
             strncpy(pcad->mess,"Invalid X position", MAX_STRING_SIZE-1);
             return CAD_REJECT;   
          }

          if (xPos < DTAXMIN || xPos > DTAXMAX)
          {
             strncpy(pcad->mess,"X position out-of-range", MAX_STRING_SIZE-1);
             return CAD_REJECT;   
          }

/*       Read in the Y position, convert to double */
          strncpy(yPosIn, (char *)pcad->b, MAX_STRING_SIZE-1);
          if (!yPosIn[0])
          {
             strncpy(pcad->mess,"Y position missing", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }
 
          if (sscanf(yPosIn, "%lf", &yPos) != 1)
          {
             strncpy(pcad->mess,"Invalid Y position", MAX_STRING_SIZE-1);
             return CAD_REJECT;   
          }

          if (yPos < DTAYMIN || yPos > DTAYMAX)
          {
             strncpy(pcad->mess,"Y position out-of-range", MAX_STRING_SIZE-1);
             return CAD_REJECT;   
          }

/*       Read in the Z position, convert to double */
          strncpy(zPosIn, (char *)pcad->c, MAX_STRING_SIZE-1);
          if (!zPosIn[0])
          {
             strncpy(pcad->mess,"Z position missing", MAX_STRING_SIZE-1);
             return CAD_REJECT;
          }
 
          if (sscanf(zPosIn, "%lf", &zPos) != 1)
          {
             strncpy(pcad->mess,"Invalid Z position", MAX_STRING_SIZE-1);
             return CAD_REJECT;   
          }

          if (zPos < DTAZMIN || zPos > DTAZMAX)
          {
             strncpy(pcad->mess,"Z position out-of-range", MAX_STRING_SIZE-1);
             return CAD_REJECT;   
          }

          status = CAD_ACCEPT;

/*       Output the translation assembly positions */
          *(double *) pcad->vala = xPos;
          *(double *) pcad->valb = yPos;
          *(double *) pcad->valc = zPos;

          break ;

       case menuDirectiveSTART:
 
          DBGMSG(DBG_FULL,"CAD dtaPos. Output values A..C: ");
          DBGMSGREAL(DBG_FULL,"A: ", *(double *) pcad->vala);
          DBGMSGREAL(DBG_FULL,"B: ", *(double *) pcad->valb);
          DBGMSGREAL(DBG_FULL,"C: ", *(double *) pcad->valc);

          status = CAD_ACCEPT;
          break;

       case menuDirectiveMARK :
          status = CAD_ACCEPT;
          break ;

       case menuDirectiveSTOP :
          status = CAD_REJECT;
          strncpy(pcad->mess,"STOP directive not implemented", MAX_STRING_SIZE-1);
          break ;

       case menuDirectiveCLEAR :
          status = CAD_ACCEPT;
          break ;
   }

   return(status);
}


static struct {void *v; char *c;} rcsid = {&rcsid,
	"$Id$"};

/* *INDENT-OFF* */
/*
 *   FILENAME
 *   -------- 
 *   gmSeqDisplayMenus.c
 *
 *   DESCRIPTION
 *   -----------
 *   This file contains the source for functions used by the GMOS
 *   Sequencer to get the names of the filters mounted in the two 
 *   filter wheels, the grating names and the mask names. All to be used
 *   to provide selection menu records of these items for display use. 
 *
 *   FUNCTION NAME(S)
 *   ----------------
 *
 *   gmSeqAllFilterLUTread    - Read database file of all filters + barcodes.
 *   gmSeqWheelFilterLUTread  - Read database file of all barcodes with wheel
 *                              number.
 *   gmSeqFilterNameMatch     - Search through the two filter lookup tables to
 *                              obtain the names of filters loaded on a
 *                              particular wheel.
 *   gmSeqFilterWheelNames    - gensub subroutine to get filter names and
 *                              write the names as record outputs.
 *   gmSeqMaskNames           - Read database file of all mask barcodes (old function)
 *   gmSeqNamedMaskLUTread    - Read database file of all named masks + barcodes
 *   gmSeqLoadedMaskLUTread   - Read database file of loaded masks with cassette
 *                              number.
 *   gmSeqMaskNameMatch       - Search through the two mask lookup tables to
 *                              obtain the names of masks loaded in a particular
 *                              cassette.
 *   gmSeqMaskCassetteNames   - Search through two lookup tables to match
 *                              barcodes with mask names for a cassette.
 *   gmSeqLoadedGratingRead   - Read database file of loaded gratings.
 *   gmSeqGratingNameMatch    - Search through the two grating lookup
 *                              tables to obtain the names of all the loaded
 *                              gratings.
 *   gmSeqGratingNames        - Read database file of grating details.
 *
 *   AUTHOR
 *   ------
 *   Philip Taylor  (pbt@observatorysciences.co.uk)
 *
 *   Copyright Observatory Sciences Ltd. 2000. All rights reserved.
 *   Under contract to the UK Astronomy Technology Centre, who modified the code
 *   in 2000/2001.
 *
 *   HISTORY
 *   -------
 *   10-May-2000: Original version.                       (pbt)
 *   24-Oct-2000: Removed hard-wired lookup table names.  (smb)
 *   05-Feb-2001: Added CVS log below.                    (smb)
 */
/*
 * $Log$
 * Revision 1.2  2001/11/28 19:35:26  mbec
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.22  2001/02/23 13:14:08  gmos
 * Added functions to translate mask names into barcode IDs, based on names read from masks.lut. Renamed global variables so they begin gmSeq. Made the filter and mask LUT reading functions more robust.
 *
 * Revision 1.21  2001/02/22 17:16:09  gmos
 * Comments updated
 *
 * Revision 1.20  2001/02/21 18:14:16  gmos
 * New code to separate mask data into three separate menus - one for each cassette.
 *
 * Revision 1.19  2001/02/21 14:41:29  gmos
 * in 2001 --> in 2000/2001
 *
 * Revision 1.18  2001/02/21 13:34:49  gmos
 * Copyright statement modified.
 *
 * Revision 1.17  2001/02/05 13:07:41  gmos
 * Code prologue tidied up.
 *
 *
 */
/* *INDENT-ON* */

#include  <vxWorks.h>
#include  <stdlib.h>
#include  <types.h>
#include  <stdioLib.h>
#include  <lstLib.h>
#include  <strLib.h>
#include  <rebootLib.h>
#include  <sysLib.h>

#include  <stdio.h>
#include  <string.h>
#include  <math.h>

#include  <genSubRecord.h>
#include  <cad.h>

#include "gmSeq.h"

/* #define VERBOSE */    /* Define to enable extra printf statements */

/*
 * Define pointers to local and external data structures :-
 */

/*
 * Pointers to the local and global lookup table containing the list of all known
 * filters, together with their barcodes and focus offsets. The local pointer is
 * redundant, being available to test this module stand-alone. The global pointer
 * is initialized by function gmSeqFilterLUTRead() in gmSeqCadLib.c.
 */

static void *localFiltLutPtr = NULL;      /* Local pointer (redundant) */
extern void *gmSeqFiltLutPtr;             /* Global pointer            */
    
/*
 * Pointer to the local lookup table of loaded filter barcodes, wheel number
 * and position.
 */

static void *wheelfiltltbl = NULL;
    
/*
 * Pointers to the local and global lookup table containing the list of all known
 * masks, together with their barcodes. The local pointer is
 * redundant, being available to test this module stand-alone. The global pointer
 * is initialized by function gmSeqMaskLUTRead() in gmSeqCadLib.c.
 */

static void *localMaskLutPtr = NULL;      /* Local pointer (redundant) */
extern void *gmSeqMaskLutPtr;             /* Global pointer            */
 
/*
 * Pointer to the local lookup table of loaded mask barcodes and cassette numbers.
 */

static void *loadedgmSeqMaskLutPtr = NULL;

/*
 * Pointer to the global lookup table containing the list of all known gratings,
 * together with their ruling density, blaze direction and focus offset. The
 * global pointer is initialized by function gmSeqGratingLUTRead in gmSeqCadLib.c
 * For some reason there isn't a redundant local pointer.
 */

extern void *gmSeqGratLutPtr;             /* Global pointer            */

/*
 * Pointer to the local lookup table with list of loaded grating barcodes
 * and positions
 */

static void *gratloadtbl = NULL;

/*
 * The following local flags record whether the various lookup table files
 * have been read successfully.
 */

static int wheelFileReadOK    = FALSE;
static int filtFileReadOK     = FALSE;
static int gratFileReadOK     = FALSE;
static int maskFileReadOK     = FALSE;
static int maskCassFileReadOK = FALSE;

/*
 * Global variable containing the debugging level flag declared in gmSeqCadLib.
 */

extern int gmSeqDbgLevel;

/*
 * Local function definitions.
 */

long gmSeqFilterNameMatch(long wheelnum, char filtnames[][LUT_TAG_SZ], int *nfound);
long gmSeqMaskNameMatch(long cassettenum, char masknames[][LUT_TAG_SZ], int *nfound);

/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqAllFilterLUTread
 *
 *   Purpose:
 *   Initialise the all filter names database
 *
 *   Description:
 *   Read the filter data file containing a list of all filters
 *   along with the corresponding barcode. 
 *   and set up an internal list of filter names with their 
 *   attributes (barcode and focus offset)
 *
 *   Invocation:
 *   gmSeqAllFilterLUTread( char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)  lutfilename  (string)   Name of all filters LUT file (filters.lut)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *   Almost identical to the routine gmSeqFilterLUTread in gmSeqCadLib.c but
 *   to be used exclusively with setting up the filter menus 
 * 
 *-
 */

long gmSeqAllFilterLUTread( char * lutfilename )
{
    FILE *fp;                /* LUT data file */
    char buf[STRING_BUF_SZ]; /* Input buffer */
    char tag[LUT_TAG_SZ];    /* Tag (= filter) name */
    FILTLUT *p;              /* Look-up table data structure */
    long n;
    long nfilt = 0;
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    filtFileReadOK = FALSE;

/* Free up old list if this is not the first entry */
    if (localFiltLutPtr != NULL) 
    {
      lstFree((LIST *) localFiltLutPtr);
      free(localFiltLutPtr);
      localFiltLutPtr = NULL;
    }    
 
/* read defined filters data file from remote disk */

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
        printf("gmSeqAllFilterLUTread error: failed to open file %s\n", lutfilename);
        return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"gmSeqAllFilterLUTread: opened filter lut file:", lutfilename);
    
    localFiltLutPtr = (void *) malloc (sizeof (LIST));
    if (localFiltLutPtr == NULL)    /* malloc failed */
    {
        printf("gmSeqAllFilterLUTread: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }
    lstInit ((LIST *) localFiltLutPtr);
    
    lcount = 0;
    while (status == 0)
      {
/*      skip blank lines and comments */

            n = fscanf (fp, "%39s", tag);     /* Format size should be LUT_TAG_SZ */
            lcount++;
            if (n == 0 || tag[0] == '#')
            {
                (void) fgets (buf, sizeof (buf)-1, fp);
            }
            /* EOF */
            else if (n == -1)
                break;
            /* create new node, read tag and number of values */
            else
            {
                p = (FILTLUT *) malloc (sizeof (FILTLUT));
                if (p == NULL)    /* malloc failed */
                {
                  printf("gmSeqAllFilterLUTread: LUT node malloc failed\n");
                  fclose(fp);
                  return CAD_REJECT;
                }
                lstAdd ((LIST *) localFiltLutPtr, (NODE *) p);
                strncpy (p->tag, tag, LUT_TAG_SZ-1);
                if (fscanf(fp,"%ld%lf%lf", &(p->barcodeId), &(p->focusOffset), &(p->effWavelength)) != 3 )
                  {
                  printf("gmSeqAllFilterLUTread error: missing value(s) in %s line %ld\n",
                         lutfilename, lcount);
                  status = CAD_REJECT;
                  }
                else
                  {
                   DBGMSGINT(DBG_FULL,"gmSeqAllFilterLUTread data line read OK:",lcount);
                   nfilt++;
                  }
            } 
      }
    fclose (fp);

/* If no valid filter lines found, then error */
      if (nfilt == 0)
      {
          printf("ERROR: no valid filter definitions found in file %s\n", lutfilename);
          status = CAD_REJECT;
      }    
    
      if (status == CAD_ACCEPT) filtFileReadOK = TRUE;
    
    return status;
}


/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqWheelFilterLUTread
 *
 *   Purpose:
 *   Initialise the filter wheel database
 *
 *   Description:
 *   Read the filter data file containing a list of all loaded filter
 *   barcodes along with the corresponding wheel number and position name. 
 *   and set up an internal list of barcodes with wheel number.
 *
 *   Invocation:
 *   gmSeqWheelFilterLUTread( char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)  lutfilename  (string)  Name of loaded filters lut file (flt.lut)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *   Derived from the routine gmSeqFilterLUTread but
 *   to be used to read the barcode list file for
 *   setting up the filter menus 
 * 
 *-
 */


long gmSeqWheelFilterLUTread( char * lutfilename )
{
    FILE *fp;                /* LUT data file */
    char buf[STRING_BUF_SZ]; /* Input buffer */
    char tag[LUT_TAG_SZ];    /* Tag (= barcode) as a string */
    long barcode;            /* barcodeID value */
    WHEELFILTLUT *p;         /* Look-up table data structure */
    long n;
    long nfilt = 0;
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    wheelFileReadOK = FALSE;

/* Free up old list if this is not the first entry */
    if (wheelfiltltbl != NULL) 
    {
      lstFree((LIST *) wheelfiltltbl);
      free(wheelfiltltbl);
      wheelfiltltbl = NULL;
    }    
 
/* read wheel contents data file from remote disk */

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
        printf("gmSeqWheelFilterLUTread error: failed to open file %s\n", lutfilename);
        return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"gmSeqWheelFilterLUTread: opened filter lut file:", lutfilename);
    
    wheelfiltltbl = (void *) malloc (sizeof (LIST));
    if (wheelfiltltbl == NULL)    /* malloc failed */
    {
        printf("gmSeqWheelFilterLUTread: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }
    lstInit ((LIST *) wheelfiltltbl);
    
    lcount = 0;
    while (status == 0)
      {
/*      skip blank lines and comments */

            n = fscanf (fp, "%39s", tag);     /* Format size should be LUT_TAG_SZ */
            lcount++;
            if (n == 0 || tag[0] == '#')
            {
                (void) fgets (buf, sizeof (buf)-1, fp);
            }
            /* EOF */
            else if (n == -1)
                break;
            /* create new node, read barcode and other values */
            else
            {
                n = sscanf(tag, "%ld", &barcode);
                if (n != 0)
                {
                  p = (WHEELFILTLUT *) malloc (sizeof (WHEELFILTLUT));
                  if (p == NULL)    /* malloc failed */
                  {
                    printf("gmSeqWheelFilterLUTread: LUT node malloc failed\n");
                    fclose(fp);
                    return CAD_REJECT;
                  }
                  lstAdd ((LIST *) wheelfiltltbl, (NODE *) p);
                  p->barcode = barcode;
                  if (fscanf(fp,"%ld%39s", &(p->wheelnum), p->position) != 2 )  /* Format size should be LUT_TAG_SZ */
                    {
                    printf("gmSeqWheelFilterLUTread error: missing value(s) in %s line %ld\n",
                           lutfilename, lcount);
                    status = CAD_REJECT;
                    }
                  else
                    {
                     DBGMSGINT(DBG_FULL,"gmSeqWheelFilterLUTread data line read OK:",lcount);
                     nfilt++;
                    }
                }
           }
      }
    fclose (fp);

/* If no valid filter lines found, then error */
    if (nfilt == 0)
    {  
        printf("ERROR: no valid filter definitions found in file %s\n", lutfilename);
        status = CAD_REJECT;
    }    
    
    if (status == CAD_ACCEPT) wheelFileReadOK = TRUE;
    
    return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqFilterNameMatch
 *
 *   Purpose:
 *   Find filter names for a given filter wheel : return the names
 *   as an array of strings
 *
 *   Description:
 *   Search through the barcode/wheel database list for each barcode
 *   for that wheel. Find the name of the filter corresponding to that
 *   barcode by searching through the filter name/barcode list. Only
 *   MAXMENU filter names per wheel are sought: any found beyond this are 
 *   ignored  
 *
 *   Invocation:
 *   gmSeqFilterNameMatch(wheelnum, filtnames, &nfound)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)   wheelnum    (long)       Search for the filters on this wheel
 *   (<)   filtnames   (char [][])  Array of names of filters on that wheel 
 *   (<)   nfound      (int)        Number of filters found on wheel
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if no filter names located for this wheel
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstFirst, lstNext
 * 
 *-
 */

long gmSeqFilterNameMatch(long wheelnum, char filtnames[][LUT_TAG_SZ], int *nfound)
{
    WHEELFILTLUT *pw;  /* Pointer to barcode/wheel lookup table item */
    FILTLUT   *pa;     /* Pointer to filter name/barcode lookup table item */
    long barcode;
    long status;
    long nWheelFilt = 0; /* Number of filters found on this wheel */
    long nfilt = 0;      /* Number of filter names found */
    long nfailed = 0;    /* Number of filter names not found */

    status = CAD_ACCEPT;
    
      /*
       * Search through the list of barcodes for each barcode corresponding
       * to the specified wheel number. The search will stop immediately if
       * wheelfiltltbl is NULL.
       */

      for (pw = (WHEELFILTLUT *) lstFirst ((LIST *)wheelfiltltbl); pw != NULL;
           pw = (WHEELFILTLUT *) lstNext ((NODE *) pw))
      {
         if (pw->wheelnum == wheelnum) /* We are interested in this wheel */
         {
            /* Break out if there are already too many filters found on this wheel */
            nWheelFilt++;
            if ( nWheelFilt > FILTPERWHEEL )
            {
               printf("WARNING: More than %d filters defined for wheel %ld\n",
                       FILTPERWHEEL, wheelnum);
               break;
            }

            barcode = pw->barcode;  /* The barcode to be searched for */
            
            /*
             * Now search through the list of all filters for the name of the filter 
             * with this barcode. The search will stop immediately if gmSeqFiltLutPtr
             * is NULL.
             */

            for (pa = (FILTLUT *) lstFirst ((LIST *)gmSeqFiltLutPtr); pa != NULL;
                 pa = (FILTLUT *) lstNext ((NODE *) pa))
            {
              if (barcode == pa->barcodeId)   /* Found the barcode we wanted */
              {
                if (nfilt >= FILTPERWHEEL)
                {
                  printf("WARNING: More than %d filter names found for wheel %ld\n",
                         FILTPERWHEEL, wheelnum);
                  break;
                }
                else
                {

                  /* Copy the filter name for this barcode into the returned name array */

                  strncpy(filtnames[nfilt], pa->tag, LUT_TAG_SZ-1);
#ifdef VERBOSE
                  printf("%d. Barcode %d, filter name %s\n", nfilt, barcode,
                         filtnames[nfilt]);
#endif
                  nfilt++;
                  break;
                }
              }
            }
            
            if (pa == NULL)
            {

              /* No match found for barcode: use the barcode string instead of the name */

              DBGMSGINT(DBG_MIN, "Check filters.lut. No name found for filter with barcode:",
                 barcode);
              sprintf(filtnames[nfilt], "%ld%c", barcode, '\0');
              nfilt++;
              nfailed++;
            }
          }
        }

        if (nWheelFilt == 0)
        {
           DBGMSGINT(DBG_NONE,"WARNING: No filters listed for wheel:", wheelnum);
           status = CAD_REJECT;   /* No filter barcodes found for this wheel */
        }
                 
        if (nfilt == 0)
        {
           DBGMSGINT(DBG_NONE,"WARNING: No filter barcodes could be translated into names on wheel:",
                  wheelnum);
           return CAD_REJECT;   /* No filter names found at all */
        }

        if ( nfailed > 0 )
        {
           printf("WARNING: %ld filter barcodes on wheel %ld could not be translated into names\n",
                  nfailed, wheelnum);
        }

        *nfound = nfilt;
        return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqFilterWheelNames
 *
 *   Purpose:
 *   Read the filter definition files and output the filter names
 *   for a specified wheel on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 12 strings on gensub output links A..L. These are intended
 *   to be used to set 12 MBBI record state strings (ZRST,..,ELST)
 *
 *   Invocation:
 *   gmSeqFilterWheelNames(wheelnum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Wheel number (long)
 *      c => all filters lut file name (string)
 *      e => filter wheel lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vall => filter names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no filter names found.
 * 
 *-
 */

long gmSeqFilterWheelNames(struct genSubRecord *pgsub)
{

     char filtnames[MAXMENU][LUT_TAG_SZ] = {""}; /* Filter name list */
     long stat = CAD_ACCEPT;
     long wheelNumber;
     char filterLutName[STRING_BUF_SZ];
     int  nfound = 0;

/* Read in wheel number at field B ; check validity (integer, 1 or 2 ) */
     wheelNumber =  *(long *)pgsub->b;

#ifdef VERBOSE
     printf("Wheel number = %ld\n", wheelNumber);
#endif

     if (wheelNumber < 1 || wheelNumber > 2)
     {
          strncpy(pgsub->valm,"Wheel number out of range (1 or 2)", MAX_STRING_SIZE-1);
          return CAD_REJECT;   
     }

     /* Read the file of filter names with barcodes */

     strncpy (filterLutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (filterLutName, "/");
     strncat (filterLutName, (char *)pgsub->l, HALF_BUF_SZ-2);

     stat = gmSeqWheelFilterLUTread(filterLutName);
     if (stat == CAD_ACCEPT)
     {

#ifdef VERBOSE
          printf("Filter Wheel %d :\n", wheelNumber);
#endif

          stat = gmSeqFilterNameMatch(wheelNumber, filtnames, &nfound);
     }

     /* Copy out the array of names to the output fields VALA..VALL */

     if ( nfound > 0 )
     {
        strncpy((char *)pgsub->vala, filtnames[0], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valb, filtnames[1], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valc, filtnames[2], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vald, filtnames[3], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vale, filtnames[4], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valf, filtnames[5], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valg, filtnames[6], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valh, filtnames[7], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vali, filtnames[8], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valj, filtnames[9], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valk, filtnames[10], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vall, filtnames[11], LUT_TAG_SZ-1) ;
     }
     else
     {
        strncpy((char *)pgsub->vala, "(none)", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valb, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valc, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vald, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vale, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valf, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valg, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valh, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vali, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valj, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valk, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vall, "", LUT_TAG_SZ-1) ;
     }

     return stat;
}

/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqMaskNames
 *
 *   Purpose:
 *   Read the mask database and output names
 *  
 *   Description:
 *   Read the mask data file containing a list of all masks
 *        Format of the file is: 
 *                 cassette#  slot#  barcodeid
 *
 *   Invocation:
 *   gmSeqMaskNames(pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *   EPICS input parameters:
 *      c => lut file name
 *
 *   EPICS output parameters:
 *      vala..valp => mask names (string). Some may be blank.
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if no Mask names found. * 
 *
 *   Derived from the routine gmSeqFilterLUTread but
 *   here used to read the mask list file
 *
 *   Deficiences:
 *   Only MAXMENU names are read from file even though there may be
 *   many more masks
 * 
 *-
 */


long gmSeqMaskNames(struct genSubRecord *pgsub)
{
    FILE *fp;                /* LUT data file */
    char line[STRING_BUF_SZ];/* Input buffer */

/* List of mask barcodes as strings */
    char maskBarcode[MAXMENU][MAX_STRING_SIZE] = {""};
    char cassette[MAX_STRING_SIZE], slot[MAX_STRING_SIZE]; /* Other data items in file */
    long n, nmask;
    char *p;
    long lcount;             /* Input line count */
    long status;             /* Return status */
    char maskLutName[STRING_BUF_SZ];

    status = CAD_ACCEPT;
    
/* read mask contents data file from remote disk */

    strncpy (maskLutName, (char *)pgsub->j, HALF_BUF_SZ);
    strcat  (maskLutName, "/");
    strncat (maskLutName, (char *)pgsub->l, HALF_BUF_SZ-2);

    if ((fp = fopen (maskLutName, "r")) == NULL)
    {
        printf("gmSeqMaskNames error: failed to open file %s\n", (char *) pgsub->c);
        return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"gmSeqMaskNames: opened mask lut file:", (char *) pgsub->c);
    
    lcount = 0;
    nmask = 0;
    while (status == 0)
      {
/*      skip blank lines and comments */

            p = fgets (line, sizeof(line), fp);
            /* EOF  or read enough mask data */
            if (p == NULL || nmask == MAXMENU)
                break;
            /* Read barcode and other values */
            else
            {
                if ((strlen(line) != 0) && (line[0] != '#'))
                {
                  if ( (n = sscanf(line,"%s%s%s", cassette, slot, maskBarcode[nmask]) != 3 ))
                  {
                    printf("gmSeqMaskNames error: missing value(s) in %s line %ld\n",
                           (char *) pgsub->c, lcount);
                    status = CAD_REJECT;
                  }
                  else
                  {
#ifdef VERBOSE
                     printf("%d. Mask barcode = %s\n", nmask, maskBarcode[nmask]);
#endif
                     nmask++;
                     DBGMSGINT(DBG_FULL,"gmSeqMaskNames data line read OK:",lcount);
                  }
              }
          }
      }
    fclose (fp);
    
/* If no valid mask lines found, then error */
      if (nmask == 0)
      {
          printf("ERROR: no valid mask definitions found in file %s\n", (char *) pgsub->c);
          status = CAD_REJECT;
      }
    
/* Copy out the array of 16 mask barcodes to the output fields VALA..VALP */
        strncpy((char *)pgsub->vala, maskBarcode[0], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valb, maskBarcode[1], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valc, maskBarcode[2], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->vald, maskBarcode[3], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->vale, maskBarcode[4], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valf, maskBarcode[5], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valg, maskBarcode[6], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valh, maskBarcode[7], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->vali, maskBarcode[8], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valj, maskBarcode[9], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valk, maskBarcode[10], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->vall, maskBarcode[11], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valm, maskBarcode[12], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valn, maskBarcode[13], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valo, maskBarcode[14], MAX_STRING_SIZE-1) ;
        strncpy((char *)pgsub->valp, maskBarcode[15], MAX_STRING_SIZE-1) ;

        return status;
}



/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqNamedMaskLUTread
 *
 *   Purpose:
 *   Initialise the named mask names database
 *
 *   Description:
 *   Read the mask data file containing a list of all named masks
 *   along with the corresponding barcode. 
 *   and set up an internal list of mask names
 *
 *   Invocation:
 *   gmSeqNamedMaskLUTread( char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)  lutfilename  (string)   Name of named masks LUT file (masks.lut)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *   Almost identical to the routine gmSeqMaskLUTread in gmSeqCadLib.c but
 *   to be used exclusively with setting up the mask menus 
 * 
 *-
 */

long gmSeqNamedMaskLUTread( char * lutfilename )
{
    FILE *fp;                /* LUT data file */
    char buf[STRING_BUF_SZ]; /* Input buffer */
    char tag[LUT_TAG_SZ];    /* Tag (= mask) name */
    MASKLUT *p;              /* Look-up table data structure */
    long n;
    long nmask = 0;
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    maskFileReadOK = FALSE;

/* Free up old list if this is not the first entry */
    if (localMaskLutPtr != NULL) 
    {
      lstFree((LIST *) localMaskLutPtr);
      free(localMaskLutPtr);
      localMaskLutPtr = NULL;
    }    
 
/* read defined masks data file from remote disk */

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
        printf("gmSeqNamedMaskLUTread error: failed to open file %s\n", lutfilename);
        return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"gmSeqNamedMaskLUTread: opened mask lut file:", lutfilename);
    
    localMaskLutPtr = (void *) malloc (sizeof (LIST));
    if (localMaskLutPtr == NULL)    /* malloc failed */
    {
        printf("gmSeqNamedMaskLUTread: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }
    lstInit ((LIST *) localMaskLutPtr);
    
    lcount = 0;
    while (status == 0)
      {
/*      skip blank lines and comments */

            n = fscanf (fp, "%39s", tag);     /* Format size should be LUT_TAG_SZ */
            lcount++;
            if (n == 0 || tag[0] == '#')
            {
                (void) fgets (buf, sizeof (buf)-1, fp);
            }
            /* EOF */
            else if (n == -1)
                break;
            /* create new node, read tag and number of values */
            else
            {
                p = (MASKLUT *) malloc (sizeof (MASKLUT));
                if (p == NULL)    /* malloc failed */
                {
                  printf("gmSeqNamedMaskLUTread: LUT node malloc failed\n");
                  fclose(fp);
                  return CAD_REJECT;
                }
                lstAdd ((LIST *) localMaskLutPtr, (NODE *) p);
                strncpy (p->tag, tag, LUT_TAG_SZ-1);
                if (fscanf(fp,"%ld", &(p->barcodeId)) != 1 )
                  {
                  printf("gmSeqNamedMaskLUTread error: missing value(s) in %s line %ld\n",
                         lutfilename, lcount);
                  status = CAD_REJECT;
                  }
                else
                  {
                   DBGMSGINT(DBG_FULL,"gmSeqNamedMaskLUTread data line read OK:",lcount);
                   nmask++;
                  }
            } 
      }
    fclose (fp);

/* If no valid mask lines found, then error */
      if (nmask == 0)
      {
          printf("ERROR: no valid mask definitions found in file %s\n", lutfilename);
          status = CAD_REJECT;
      }    
    
      if (status == CAD_ACCEPT) maskFileReadOK = TRUE;
    
    return status;
}



/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqLoadedMaskLUTread
 *
 *   Purpose:
 *   Initialise the loaded mask database
 *
 *   Description:
 *   Read the mask data file containing a list of all loaded mask
 *   barcodes along with the corresponding cassette and slot number
 *   and set up an internal list of masks with cassette number.
 *
 *   Invocation:
 *   gmSeqLoadedMaskLUTread( char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)  lutfilename  (string)  Name of loaded masks lut file (msk.lut)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *   Derived from the routine gmSeqFilterLUTread but
 *   to be used to read the barcode list file for
 *   setting up the mask menus 
 * 
 *-
 */


long gmSeqLoadedMaskLUTread( char * lutfilename )
{
    FILE *fp;                /* LUT data file */
    char buf[STRING_BUF_SZ]; /* Input buffer */
    char tag[LUT_TAG_SZ];    /* Tag (= barcode) as a string */
    long cassette;           /* cassette number value */
    LOADEDMASKLUT *p;        /* Look-up table data structure */
    long n;
    long nmasks = 0;
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    maskCassFileReadOK = FALSE;

/* Free up old list if this is not the first entry */
    if (loadedgmSeqMaskLutPtr != NULL) 
    {
      lstFree((LIST *) loadedgmSeqMaskLutPtr);
      free(loadedgmSeqMaskLutPtr);
      loadedgmSeqMaskLutPtr = NULL;
    }    
 
/* read cassette contents data file from remote disk */

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
        printf("gmSeqLoadedMaskLUTread error: failed to open file %s\n", lutfilename);
        return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"gmSeqLoadedMaskLUTread: opened mask lut file:", lutfilename);
    
    loadedgmSeqMaskLutPtr = (void *) malloc (sizeof (LIST));
    if (loadedgmSeqMaskLutPtr == NULL)    /* malloc failed */
    {
        printf("gmSeqLoadedMaskLUTread: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }
    lstInit ((LIST *) loadedgmSeqMaskLutPtr);
    
    lcount = 0;
    while (status == 0)
      {
/*      skip blank lines and comments */

            n = fscanf (fp, "%39s", tag);     /* Format size should be LUT_TAG_SZ */
            lcount++;
            if (n == 0 || tag[0] == '#')
            {
                (void) fgets (buf, sizeof (buf)-1, fp);
            }
            /* EOF */
            else if (n == -1)
                break;
            /* create new node, read cassette number and other values */
            else
            {
                n = sscanf(tag, "%ld", &cassette);
                if (n != 0)
                {
                  p = (LOADEDMASKLUT *) malloc (sizeof (LOADEDMASKLUT));
                  if (p == NULL)    /* malloc failed */
                  {
                    printf("gmSeqLoadedMaskLUTread: LUT node malloc failed\n");
                    fclose(fp);
                    return CAD_REJECT;
                  }
                  lstAdd ((LIST *) loadedgmSeqMaskLutPtr, (NODE *) p);
                  p->cassette = cassette;
                  if (fscanf(fp,"%ld%ld", &(p->slot), &(p->barcode)) != 2 )
                    {
                    printf("gmSeqLoadedMaskLUTread error: missing value(s) in %s line %ld\n",
                           lutfilename, lcount);
                    status = CAD_REJECT;
                    }
                  else
                    {
                     DBGMSGINT(DBG_FULL,"gmSeqLoadedMaskLUTread data line read OK:",lcount);
                     nmasks++;
                    }
                }
           }
      }
    fclose (fp);

/* If no valid mask lines found, then error */
    if (nmasks == 0)
    {  
        printf("ERROR: no valid mask definitions found in file %s\n", lutfilename);
        status = CAD_REJECT;
    }    
    
    if (status == CAD_ACCEPT) maskCassFileReadOK = TRUE;
    
    return status;
}


/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqMaskNameMatch
 *
 *   Purpose:
 *   Find mask names for a given mask cassette : return the names
 *   as an array of strings
 *
 *   Description:
 *   Search through the barcode/cassette database list for each barcode
 *   for that cassette. Find the name of the mask corresponding to that
 *   barcode by searching through the mask name/barcode list. Only
 *   MAXMENU mask names per cassette are sought: any found beyond this are 
 *   ignored  
 *
 *   Invocation:
 *   gmSeqMaskNameMatch(cassettenum, masknames, &nfound)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)   cassettenum    (long)       Search for the masks on this cassette
 *   (<)   masknames      (char [][])  Array of names of masks on that cassette 
 *   (<)   nfound         (int)        Number of masks found on cassette
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if no mask names located for this cassette
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstFirst, lstNext
 * 
 *-
 */

long gmSeqMaskNameMatch(long cassettenum, char masknames[][LUT_TAG_SZ], int *nfound)
{
    LOADEDMASKLUT *pw;  /* Pointer to barcode/cassette lookup table item */
    MASKLUT       *pa;  /* Pointer to mask name/barcode lookup table item */
    long barcode;
    long status;
    long masksInCassette = 0; /* Number of masks found on this cassette */
    long nmasks = 0;          /* Number of mask names found */

    status = CAD_ACCEPT;
    
      /*
       * Search through the list of barcodes for each barcode corresponding
       * to the specified cassette number. The search will stop immediately
       * if loadedgmSeqMaskLutPtr is NULL.
       */

      for (pw = (LOADEDMASKLUT *) lstFirst ((LIST *)loadedgmSeqMaskLutPtr); pw != NULL;
           pw = (LOADEDMASKLUT *) lstNext ((NODE *) pw))
      {
         if (pw->cassette == cassettenum) /* We are interested in this cassette */
         {

            /* Break out if there are already too many masks found in this cassette */
            masksInCassette++;
            if ( masksInCassette > MASKPERCASS )
            {
               printf("WARNING: More than %d masks defined for cassette %ld\n",
                       MASKPERCASS, cassettenum);
               break;
            }

            barcode = pw->barcode;  /* The barcode to be searched for */
            
            /*
             * Now search through the list of all named masks for the name of the mask 
             * with this barcode. The search will stop immediately if gmSeqMaskLutPtr
             * is NULL.
             */

            for (pa = (MASKLUT *) lstFirst ((LIST *)gmSeqMaskLutPtr); pa != NULL;
                 pa = (MASKLUT *) lstNext ((NODE *) pa))
            {
              if (barcode == pa->barcodeId)   /* Found the barcode we wanted */
              {
                if (nmasks >= MASKPERCASS)
                {
                  printf("WARNING: More than %d mask names found for cassette %ld\n",
                          MASKPERCASS, cassettenum);
                  break;
                }
                else
                {
                  /* Copy the mask name for this barcode into the returned name array */

                  strncpy(masknames[nmasks], pa->tag, LUT_TAG_SZ-1);
#ifdef VERBOSE
                  printf("%d. Barcode %d, mask name %s\n", nmasks, barcode,
                         masknames[nmasks]);
#endif
                  nmasks++;
                  break;
                }
              }
            }
            
            if (pa == NULL)
            {

              /*
               * No match found for barcode: use the barcode string instead of the name.
               * This is not an error, since most masks will not have names.
               */

              DBGMSGINT(DBG_FULL, "NOTE: No name found for mask with barcode:", barcode);
              sprintf(masknames[nmasks], "%ld%c", barcode, '\0');
              nmasks++;
            }
          }
        }

        if (masksInCassette == 0)
         {
           /* No mask barcodes found for this cassette. Missing cassettes are allowed. */
           printf("NOTE: No masks listed for cassette %ld\n", cassettenum);
           status = CAD_REJECT;
         }
                 
        if (nmasks == 0)
         {
           /* No mask names found at all. This is ok. */
           DBGMSGINT(DBG_MIN,
               "NOTE: None of the mask barcodes could be translated into names on cassette:",
               cassettenum);
           status = CAD_REJECT;
         }

        *nfound = nmasks;
        return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqMaskCassetteNames
 *
 *   Purpose:
 *   Read the mask definition files and output the mask names
 *   for a specified cassette on the gensub record output links
 *
 *   Description:
 *   Call the routines to read the files and define the two lookup tables.
 *   Output 9 strings on gensub output links A..I. These are intended
 *   to be used to set 9 MBBI record state strings (ZRST,..,NIST)
 *
 *   Invocation:
 *   gmSeqMaskCassetteNames(cassettenum, pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *
 *   EPICS input parameters:
 *      b => Cassette number (long)
 *      c => all masks lut file name (string)
 *      e => mask cassette lut file name (string)
 *
 *   EPICS output parameters:
 *      vala..vali => mask names (string). Some may be blank.
 *      valm       => error message (string)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file reads fail or no mask names found.
 * 
 *-
 */

long gmSeqMaskCassetteNames(struct genSubRecord *pgsub)
{

     char masknames[MAXMENU][LUT_TAG_SZ] = {""}; /* Mask name list */
     long stat = CAD_ACCEPT;
     long cassetteNumber;
     char maskLutName[STRING_BUF_SZ];
     int nfound = 0;

/* Read in cassette number at field B ; check validity (integer, 1-3 ) */
     cassetteNumber =  *(long *)pgsub->b;

#ifdef VERBOSE
     printf("Cassette number = %ld\n", cassetteNumber);
#endif

     if (cassetteNumber < 1 || cassetteNumber > 3)
     {
          strncpy(pgsub->valm,"Cassette number out of range (1-3)", MAX_STRING_SIZE-1);
          return CAD_REJECT;   
     }

     /* Read the file of mask names with barcodes */

     strncpy (maskLutName, (char *)pgsub->j, HALF_BUF_SZ);
     strcat  (maskLutName, "/");
     strncat (maskLutName, (char *)pgsub->l, HALF_BUF_SZ-2);

     stat = gmSeqLoadedMaskLUTread(maskLutName);
     if (stat == CAD_ACCEPT)
     {

#ifdef VERBOSE
          printf("Mask Cassette %d :\n", cassetteNumber);
#endif

          stat = gmSeqMaskNameMatch(cassetteNumber, masknames, &nfound);
     }

     /* Copy out the array of names to the output fields VALA..VALI */

     if ( nfound > 0 )
     {
        strncpy((char *)pgsub->vala, masknames[0], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valb, masknames[1], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valc, masknames[2], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vald, masknames[3], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vale, masknames[4], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valf, masknames[5], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valg, masknames[6], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valh, masknames[7], LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vali, masknames[8], LUT_TAG_SZ-1) ;
     }
     else
     {
        strncpy((char *)pgsub->vala, "(none)", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valb, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valc, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vald, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vale, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valf, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valg, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->valh, "", LUT_TAG_SZ-1) ;
        strncpy((char *)pgsub->vali, "", LUT_TAG_SZ-1) ;
     }

     return stat;
}


/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqLoadedGratingRead
 *
 *   Purpose:
 *   Initialise the loaded grating database
 *
 *   Description:
 *   Read the grating data file containing a list of all loaded grating
 *   barcodes along with the corresponding turret position and tilt angle. 
 *
 *   Invocation:
 *   gmSeqLoadedGratingRead( char * lutfilename )
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>) lutfilename  (string)  Name of loaded grating lut file (gr.lut)
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if file not opened or missing data values
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstInit, lstAdd
 *
 *   Derived from the routine gmSeqFilterLUTread but
 *   to be used to read the grating loaded data file for
 *   setting up the grating menus 
 * 
 *-
 */


long gmSeqLoadedGratingRead( char * lutfilename )
{
    FILE *fp;                /* LUT data file */
    char buf[STRING_BUF_SZ]; /* Input buffer */
    char tag[LUT_TAG_SZ];    /* Tag (= barcode) as a string */
    long barcode;            /* barcodeID value */
    LOADEDGRATLUT *p;        /* Look-up table data structure */
    long n;
    long lcount;             /* Input line count */
    long status;             /* Return status */

    status = CAD_ACCEPT;
    gratFileReadOK = FALSE;

/* Free up old list if this is not the first entry */
    if (gratloadtbl != NULL) 
    {
      lstFree((LIST *) gratloadtbl);
      free(gratloadtbl);
      gratloadtbl = NULL;
    }    

/* check for a sensible LUT name */

    if (lutfilename == NULL)
    {
        printf("gmSeqLoadedGratingRead error: invalid file name pointer\n");
        return CAD_REJECT;
    }

/* read loaded gratings contents data file from remote disk */

    if ((fp = fopen (lutfilename, "r")) == NULL)
    {
        printf("gmSeqLoadedGratingRead error: failed to open file %s\n", lutfilename);
        return CAD_REJECT;
    }

    DBGMSGSTRING(DBG_MIN,"gmSeqLoadedGratingRead: opened grating lut file:", lutfilename);
    
    gratloadtbl = (void *) malloc (sizeof (LIST));
    if (gratloadtbl == NULL)    /* malloc failed */
    {
        printf("gmSeqLoadedGratingRead: LUT malloc failed\n");
        fclose(fp);
        return CAD_REJECT;
    }
    lstInit ((LIST *) gratloadtbl);
    
    lcount = 0;
    while (status == 0)
      {
/*      skip blank lines and comments */

            n = fscanf (fp, "%39s", tag);     /* Format size should be LUT_TAG_SZ */
            lcount++;
            if (n == 0 || tag[0] == '#')
            {
                (void) fgets (buf, sizeof (buf)-1, fp);
            }
            /* EOF */
            else if (n == -1)
                break;
            /* create new node, read barcode and other values */
            else
            {
                n = sscanf(tag, "%ld", &barcode);
                if (n != 0)
                {
                  p = (LOADEDGRATLUT *) malloc (sizeof (LOADEDGRATLUT));
                  if (p == NULL)    /* malloc failed */
                  {
                    printf("gmSeqLoadedGratingRead: LUT node malloc failed\n");
                    fclose(fp);
                    return CAD_REJECT;
                  }
                  lstAdd ((LIST *) gratloadtbl, (NODE *) p);
                  p->barcode = barcode;
                  if (fscanf(fp,"%ld%lf", &(p->turretPos), &(p->tilt)) != 2 )
                    {
                    printf("gmSeqLoadedGratingRead error: missing value(s) in %s line %ld\n",
                           lutfilename, lcount);
                    status = CAD_REJECT;
                    }
                  else
                     {
                     DBGMSGINT(DBG_FULL,"gmSeqLoadedGratingRead data line read OK:",lcount);
                     }
                }
           }
      }
    fclose (fp);
    if (status == CAD_ACCEPT) gratFileReadOK = TRUE;
    
    return status;
}

/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqGratingNameMatch
 *
 *   Purpose:
 *   Find grating name for a given grating barcode 
 *
 *   Description:
 *   Return the name of the grating corresponding to a specified
 *   barcode by searching through the grating name/barcode list.
 *
 *   Invocation:
 *   gmSeqGratingNameMatch(barcode, name)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *   (>)   barcode    (long)    Get name of grating with this barcode
 *   (>)   name       (char *)  Name of grating
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if no grating name located for this barcode
 * 
 *   External functions:
 *   (VxWorks lstLib routines) lstFirst, lstNext
 * 
 *-
 */

long gmSeqGratingNameMatch(long barcode, char *name)
{
    GRATLUT   *pa;    /* Pointer to all gratings name/barcode lookup table item */
            
    /*
     * Search through the list of all gratings for the name of the grating 
     * with this barcode. The search will stop immediately if gmSeqGratLutPtr
     * is NULL.
     */

    for (pa = (GRATLUT *) lstFirst ((LIST *)gmSeqGratLutPtr); pa != NULL;
         pa = (GRATLUT *) lstNext ((NODE *) pa))
    {
       if (barcode == pa->barcodeId)
       {
           /* Found the barcode we wanted */
           strncpy(name, pa->tag, LUT_TAG_SZ-1);     /* Tag is the grating name string */
           break; /* Break out of for loop */
       }
    }
            
    if (pa == NULL)
    {
       DBGMSGINT(DBG_MIN, "WARNING: No grating name found with barcode:", barcode);
       return CAD_REJECT;   /* No grating name found */
    }
    else
    {
       return CAD_ACCEPT;
    }
}



/******************************************************************************/

/*+
 *   Function name:
 *   gmSeqGratingNames
 *
 *   Purpose:
 *   Output the grating names on the gensub record output links
 *
 *   Description:
 *   Get the grating names from the grating lookup table.
 *   Output 16 strings on gensub output links A..P. These are intended
 *   to be used to set 16 MBBI record state strings (ZRST,..,FFST)
 *
 *   Invocation:
 *   gmSeqGratingNames(pgsub)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)   pgsub  (struct genSubRecord *pgsub)  Pointer to gensub structure
 *      
 *   EPICS input parameters:
 *      c          <= name of lookup table file
 *
 *   EPICS output parameters:
 *      vala..valp => mask names (string). Some may be blank.
 *
 *   Function value:
 *   (<)  status  (long) Return status, 0 = OK
 *                       Error if no grating names found.
 * 
 *-
 */

long gmSeqGratingNames(struct genSubRecord *pgsub)
{

        char gratnames[MAXMENU][LUT_TAG_SZ] = {""}; /* Grating name list */
        long stat = CAD_ACCEPT;
        long lkstat;
        LOADEDGRATLUT *pg;
        long ngrat = 0;
        long nameFound = 0;
        long nfailed = 0;
        long barcode;
        char gratingLutName[STRING_BUF_SZ];

/* Read all the gratings barcodes and attributes */

        strncpy (gratingLutName, (char *)pgsub->j, HALF_BUF_SZ);
        strcat  (gratingLutName, "/");
        strncat (gratingLutName, (char *)pgsub->l, HALF_BUF_SZ-2);

        gmSeqLoadedGratingRead(gratingLutName);
        
/* Now search through the list of all loaded gratings and try to get the names */
            for (pg = (LOADEDGRATLUT *) lstFirst ((LIST *)gratloadtbl); pg != NULL;
            pg = (LOADEDGRATLUT *) lstNext ((NODE *) pg))
            {
                barcode = pg->barcode;
                lkstat = gmSeqGratingNameMatch(barcode, gratnames[ngrat]);
                if (lkstat == CAD_ACCEPT)
                {
#ifdef VERBOSE
                  printf("%d. Grating barcode %d: name = %s\n", 
                          ngrat, barcode, gratnames[ngrat]);
#endif
                  nameFound++;
                }
                else
/* Lookup failed : use barcode instead of name as ID */
                {
                  sprintf(gratnames[ngrat], "%ld%c", barcode, '\0');
                  DBGMSGINT(DBG_MIN, "Check gratings.lut. No name found for grating with barcode:", 
                          barcode);
                }
                ngrat++;
                if (ngrat > MAXMENU -1) break;
            }

/* Check whether any gratings found at all; if none, then failed */
            if (ngrat == 0)
            {
                printf("ERROR: No valid list of loaded gratings\n");
                stat = CAD_REJECT;
            }

/* But if found gratings, but no names found, then this is just a warning */
            else if (nameFound == 0)
            {
                printf("WARNING: None of the grating barcodes could be translated into names\n");
                stat = CAD_ACCEPT;
            }
            else if (nfailed > 0)
            {
                printf("WARNING: %ld of the grating barcodes could not be translated into names\n",
                       nfailed);
                stat = CAD_ACCEPT;
            }

/* Copy out the array of names to the output fields VALA..VALP */
             strncpy((char *)pgsub->vala, gratnames[0], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valb, gratnames[1], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valc, gratnames[2], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->vald, gratnames[3], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->vale, gratnames[4], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valf, gratnames[5], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valg, gratnames[6], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valh, gratnames[7], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->vali, gratnames[8], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valj, gratnames[9], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valk, gratnames[10], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->vall, gratnames[11], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valm, gratnames[12], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valn, gratnames[13], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valo, gratnames[14], LUT_TAG_SZ-1) ;
             strncpy((char *)pgsub->valp, gratnames[15], LUT_TAG_SZ-1) ;

             return stat;
}

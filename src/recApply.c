/* $Id$ */

/* recApply.c - Record Support Routines for Apply */
/*
 *      Author: Bret Goodrich
 *      Date:   14 February 1995
 *              21apr96,bdg changed client ID to integer.
 *              10jul96,bdg added fwd link.  reorganized process().
 *              13jun96,bdg process now ignores MARK directive
 *              04nov96,bdg save old directive so PRESET not done twice
 *              08nov96,bdg VAL monitor posted last.
 *              26dec96,bdg changed last field of recGblInitFastOutLink() call
 *              21mar97,bdg check links exist before changing NPP to PP.
 *                          process didn't handle callbacks right.
 *              30apr97,bdg another callback fix in process.
 *              02may97,bdg client ID returned as VAL.
 *              04jun97,bdg return only one monitor on VAL
 *		18jul97,bdg CLID not incremented on subr error
 *		15sep97,bdg fixed PRESET error handling
 *		16sep97,bdg fixed PRESET monitor
 *		03nov97,ajf Increment CLID before processing output links
 *              11sep00,cjm Rewrite to work between IOCs
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      Copyright 2000, A.U.R.A 
 *
 */


#include	<vxWorks.h>
#include	<types.h>
#include	<stdioLib.h>
#include	<lstLib.h>
#include	<string.h>
#include	<symLib.h>
#include        <sysSymTbl.h>   /* for sysSymTbl*/
#include        <a_out.h>       /* for N_TEXT */

#include	<alarm.h>
#include	<dbDefs.h>
#include	<dbAccess.h>
#include	<dbEvent.h>
#include	<dbFldTypes.h>
#include	<errMdef.h>
#include	<recSup.h>
#include	<devSup.h>
#include 	<special.h>
#include	<applyRecord.h>
#include        <cad.h>

/* Create RSET - Record Support Entry Table*/

static long init_record();
static long process();
static long get_value();
static long get_enum_str();
static long get_enum_strs();
static long put_enum_str();
#define report             NULL
#define initialize         NULL
#define special            NULL
#define cvt_dbaddr         NULL
#define get_units          NULL
#define get_precision      NULL
#define get_array_info     NULL
#define put_array_info     NULL
#define get_graphic_double NULL
#define get_control_double NULL
#define get_alarm_double   NULL

struct rset applyRSET={
	RSETNUMBER,
	report,
	initialize,
	init_record,
	process,
	special,
	get_value,
	cvt_dbaddr,
	get_array_info,
	put_array_info,
	get_units,
	get_precision,
	get_enum_str,
	get_enum_strs,
	put_enum_str,
	get_graphic_double,
	get_control_double,
	get_alarm_double };

struct applydset
{
    long    number ;
    DEVSUPFUN dev_report ;
    DEVSUPFUN init ;
    DEVSUPFUN init_record ;
    DEVSUPFUN get_ioint_info ;
    DEVSUPFUN do_apply ;
} ;

#define DEBUG 0

static void monitor (struct applyRecord *);

#define ARG_MAX 8


/*******************************************************************************
* init_record
*/

static long init_record (
    void  *precord,
    int pass)
{
    struct applyRecord *papply = (struct applyRecord *)precord ;
    struct applydset *pdset ;
    long status = OK;
    int i;

    /*
     * Pass 0: nothing to do
     */
    if (pass == 0)
    {
	return 0;
    }

    /*
     * Pass 1: make fast links to OUTx OCLx INPx INMx.  Force OUTx to
     * be process passive, force others to be NPP. Check for existence
     * of device support.
     */
    else if (pass == 1)
    {
        if ((pdset = (struct applydset *)(papply->dset)) == NULL) { 
          recGblRecordError (S_dev_noDSET, papply, "apply: init_record") ;
          return (S_dev_noDSET);
        }

        if ( (pdset->number < 5) || (pdset->do_apply == NULL) ) {
          recGblRecordError (S_dev_missingSup, papply, "apply: init_record") ;
          return (S_dev_missingSup) ;
        }
 
/* For the time being leave this initialisation here as it is device
*  independent and do the device specific part afterwards.
*/
	for (i = 0; i < ARG_MAX; i++)
	{
	    if ((&papply->outa + i)->type == DB_LINK)
		(&papply->outa+i)->value.db_link.process_passive = TRUE;
	    status = recGblInitFastOutLink (&papply->outa + i,
		(void *) papply, DBR_SHORT, "DIR");
	    if (status)
		return status;

	    if ((&papply->ocla + i)->type == DB_LINK)
		(&papply->ocla+i)->value.db_link.process_passive = FALSE;
	    status = recGblInitFastOutLink (&papply->ocla + i,
		(void *) papply, DBR_LONG, "CLID");
	    if (status)
		return status;

	    if ((&papply->inpa + i)->type == DB_LINK)
		(&papply->inpa+i)->value.db_link.process_passive = FALSE;
	    status = recGblInitFastInLink (&papply->inpa + i,
		(void *) papply, DBR_LONG, "VAL");
	    if (status)
		return status;

	    if ((&papply->inma + i)->type == DB_LINK)
		(&papply->inma+i)->value.db_link.process_passive = FALSE;
	    status = recGblInitFastInLink (&papply->inma + i,
		(void *) papply, DBR_STRING, "MESS");
	    if (status)
		return status;
	}
/* Device specific initialisation */
        if (pdset->init_record) {
          if ( (status = (*pdset->init_record)(papply))) return (status) ;
        }
    }

    return status;
}


/*******************************************************************************
* process
*/

static long process (void *precord)
{
    struct applyRecord *papply = (struct applyRecord *)precord ;
    struct applydset *pdset   = (struct applydset *)papply->dset ;
    long status = 0;

    /* check for valid device routines */
    if ((pdset == NULL) || (pdset->do_apply == NULL) ) {
      papply->pact = TRUE ;
      recGblRecordError(S_dev_missingSup, papply, "do_apply") ;
      return (S_dev_missingSup) ;
    }

    /* ignore all MARK directives */
    if (papply->dir == CAD_MARK) 
	goto done;

    status = (*pdset->do_apply)(papply) ;
    if (papply->pact) return 0 ;

done:
    papply->pact = TRUE ;
    if (status == 0)
    {
	recGblGetTimeStamp (papply);
	monitor (papply);
    }
    recGblFwdLink (papply);
    papply->pact = FALSE;
    return status;
}

/*******************************************************************************
* get_value
*/

static long get_value (
    struct applyRecord *papply,
    struct valueDes *pvdes)
{
    pvdes->field_type = DBF_LONG;
    pvdes->no_elements = 1;
    pvdes->pvalue = (void *)&papply->val;
    return 0;
}


/*******************************************************************************
* get_enum_str
*/

static long get_enum_str (
    struct dbAddr *paddr,
    char *pstring)
{
    struct applyRecord *papply = (struct applyRecord *) paddr->precord;

    switch (papply->dir)
    {
	case CAD_MARK:
	    strcpy (pstring, "MARK");
	    break;
	case CAD_CLEAR:
	    strcpy (pstring, "CLEAR");
	    break;
	case CAD_PRESET:
	    strcpy (pstring, "PRESET");
	    break;
	case CAD_START:
	    strcpy (pstring, "START");
	    break;
	case CAD_STOP:
	    strcpy (pstring, "STOP");
	    break;
	default:
	    strcpy (pstring, "Illegal_Value");
	    return -1;
	    break;
    }

    return 0;
}


/*******************************************************************************
* get_enum_strs
*/

static long get_enum_strs (
    struct dbAddr *paddr,
    struct dbr_enumStrs *pes)
{
    pes->no_str = 5;
    memset (pes->strs, '\0', sizeof (pes->strs));
    strcpy (pes->strs[0], "MARK");
    strcpy (pes->strs[1], "CLEAR");
    strcpy (pes->strs[2], "PRESET");
    strcpy (pes->strs[3], "START");
    strcpy (pes->strs[4], "STOP");

    return 0;
}


/*******************************************************************************
* put_enum_str
*/

static long put_enum_str (
    struct dbAddr *paddr,
    char *pstring)
{
    struct applyRecord *papply = (struct applyRecord *) paddr->precord;

    if (!strcmp (pstring, "MARK"))
	papply->dir = CAD_MARK;
    else if (!strcmp (pstring, "CLEAR"))
	papply->dir = CAD_CLEAR;
    else if (!strcmp (pstring, "PRESET"))
	papply->dir = CAD_PRESET;
    else if (!strcmp (pstring, "START"))
	papply->dir = CAD_START;
    else if (!strcmp (pstring, "STOP"))
	papply->dir = CAD_STOP;
    else
	return (S_db_badChoice);

    return 0;
}


/*******************************************************************************
* monitor
*/

static void monitor (
    struct applyRecord *papply)
{
    unsigned short monitor_mask;

    monitor_mask = recGblResetAlarms (papply);
    monitor_mask |= DBE_VALUE | DBE_LOG;

    /*
     * Raise monitors on VAL and MESS.  The latter only fires if
     * there is an error in VAL or the MESS changes.
     */
    if (monitor_mask)
    {
	db_post_events (papply, &papply->clid, monitor_mask);
	if (papply->val < 0 || strncmp (papply->omss, papply->mess, 40) != 0)
	{
	    db_post_events (papply, &papply->mess, monitor_mask);
	    strncpy (papply->omss, papply->mess, 40);
	}
	db_post_events (papply, &papply->val, monitor_mask);
    }

    return;
}

/* devApplyLocal.c */
/* base/src/dev $Id$ */

/* devApplyLocal.c - Device Support Routines for apply record */
/*
 *      Original Author: Chris Mayer 
 *      Date:           12 Sept. 2000 
 *
 *      Experimental Physics and Industrial Control System (EPICS)
 *
 *      Copyright 2000, A.U.R.A 
 *
 */


#include	<vxWorks.h>
#include	<types.h>
#include	<stdioLib.h>
#include	<wdLib.h>
#include	<memLib.h>
#include	<string.h>

#include	<alarm.h>
#include	<callback.h>
#include	<dbDefs.h>
#include	<dbAccess.h>
#include	<recSup.h>
#include	<devSup.h>
#include	<link.h>
#include	<dbCommon.h>
#include	<applyRecord.h>
#include        <cad.h>

/* Create the dset for devApplyLocal */
static long init_record();
static long do_apply();
static long process_sub();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	do_apply;
}devApplyLocal={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	do_apply };

#define ARG_MAX 8

static long init_record(struct applyRecord *papply)
{
    /* printf ("Local apply init_record\n"); */
    return(0);
}

static long do_apply(struct applyRecord *papply)
{
  long status ;

   papply->pact = TRUE ;

/* Clear the return MESS field */
   *papply->mess = NULL ;

/* If directive is START do PRESET first */
   if (papply->dir == CAD_START) {

/* Don't PRESET if last directive was PRESET. This prevents multiple
*  presets when apply records are cascaded 
*/
     if (papply->mark != CAD_PRESET) {
       papply->dir = CAD_PRESET ;
       status = process_sub (papply) ;
       papply->dir = CAD_START ;
       if (status != 0 || papply->val < 0 ) {
         papply->pact = FALSE ;
         return status ;
       }
     }
   }

   papply->mark = papply->dir ;       /* save old state */
   if (papply->dir == CAD_START && papply->val >= 0 )
     papply->clid++ ;
   status = process_sub (papply) ;

   if (status == 0) {
     if (papply->dir == CAD_START && papply->val >= 0)
       papply->val = papply->clid ;
   }

   papply->pact = FALSE;
   return(status);
}

static long process_sub (struct applyRecord *papply) 
{

  long status ;
  int i ;

/* Send clid & dir to all out links, get all input links */
  for (i = 0 ; i < ARG_MAX; i++) {

    status = recGblPutFastLink (&papply->ocla + i, (void *)papply, 
        &papply->clid) ;
    if (status != 0)
        break ;

    status = recGblPutFastLink (&papply->outa + i, (void *) papply,
        &papply->dir) ;
    if (status !=0)
        break ;

    status = recGblGetFastLink (&papply->inpa + i, (void *) papply,
       &papply->val) ;
    if (status != 0)
       break ;

    if (papply->val < 0 ) {
      status = recGblGetFastLink (&papply->inma + i, (void *) papply,
         papply->mess) ;
      break ;
    }

   
  }

  return (status) ;

}

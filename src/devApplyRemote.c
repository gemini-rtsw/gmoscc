/* devApplyRemote.c */
/* base/src/dev $Id$ */

/* devApplyRemote.c - Device Support Routines for apply record */
/*
 *      Original Author: Chris Mayer 
 *      Date:            14 Sept. 2000 
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
#include        <stdlib.h>
#include        <sysLib.h>

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

static long put_links() ;
static long get_links() ;

/* Create the dset for devApplyRemote */
static long init_record();
static long do_apply();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	do_apply;
}devApplyRemote={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	do_apply };

/* Control block for the callback */
struct applyControl {
        CALLBACK callback ;
        WDOG_ID wd_id ;
        int preset ;
};

#define ARG_MAX 8
#define CALLBACK_DELAY 0.5

static void applyCallback (CALLBACK *pcallback)
{
  struct dbCommon  *precord ;
  struct rset  *prset ;

  callbackGetUser(precord, pcallback) ;
  prset = (struct rset *)precord->rset ;
  dbScanLock(precord) ;
  (*prset->process)(precord) ;
  dbScanUnlock(precord) ;
}

static long init_record(struct applyRecord *papply)
{
   struct applyControl *pappcontrol ;

   /* printf ("Remote apply init_record\n"); */

   pappcontrol=(struct applyControl *)(calloc(1, sizeof(struct applyControl)));
   papply->dpvt = (void *)pappcontrol ;
   callbackSetCallback (applyCallback, &pappcontrol->callback) ;
   callbackSetUser((void *)papply, &pappcontrol->callback) ;
   pappcontrol->wd_id = wdCreate() ;
   pappcontrol->preset = FALSE ;

   return(0);
}

static long do_apply(struct applyRecord *papply)
{
  long status ;
  int wait_time ;
  struct applyControl *pappcontrol = (struct applyControl *)(papply->dpvt) ;

  wait_time = CALLBACK_DELAY * vxTicksPerSecond ;

/* This routine can be called as a result of a callback or via dbProcess
*  when a record is activated for the first time. The flag pact tells
*  us which is which.
*/

  if (papply->pact) {

/* First check if this callback is from a preset during a start.
*  If it is then we have to check that it completed OK and if so trigger
*  the output links again with the start directive
*/
    if (pappcontrol->preset) {
      status = get_links (papply) ;
      if (status != 0 || papply->val < 0) {
        papply->pact = FALSE ;
        return status ;
      } else {
        papply->dir = CAD_START ;
        papply->mark = papply->dir ;
        papply->clid++ ;
        status = put_links(papply) ;
        if (!status) {
          pappcontrol->preset = FALSE ;
          wdStart (pappcontrol->wd_id, wait_time, (FUNCPTR)callbackRequest,
                   (int)&pappcontrol->callback) ;
        }
      }

    } else {

      status = get_links(papply) ;
      if (status == 0) {
        if (papply->dir == CAD_START && papply->val >= 0)
          papply->val = papply->clid ;
      }
      papply->pact = FALSE ;

    }


  } else {

/* This is the section of code that is executed when the record is 
*  called by dbProcess */

/* Clear the return MESS field */
    *papply->mess = NULL ;

/* If directive is START do PRESET first */
    if (papply->dir == CAD_START) {

/* Don't PRESET if last directive was PRESET. This prevents multiple
*  presets when apply records are cascaded 
*/
      if (papply->mark != CAD_PRESET) {
         papply->dir = CAD_PRESET ;
         status = put_links(papply) ;
         if (!status) {

/*  Arrange the call back and set flag to show this is a preset during a
*   start 
*/
           callbackSetPriority(papply->prio, &pappcontrol->callback) ;
           pappcontrol->preset = TRUE ;
           wdStart(pappcontrol->wd_id, wait_time, 
                   (FUNCPTR)callbackRequest,
                   (int)&pappcontrol->callback) ;
           papply->pact = TRUE ;
         }
         return status ;
       }
    }

    papply->mark = papply->dir ;       /* save old state */
    if (papply->dir == CAD_START && papply->val >= 0 )
      papply->clid++ ;

/*  Trigger output links and if necessary arrange a call back */
    status = put_links(papply) ;
    if (!status) {
      callbackSetPriority(papply->prio, &pappcontrol->callback) ;
      pappcontrol->preset = FALSE ;
      wdStart(pappcontrol->wd_id, wait_time, (FUNCPTR)callbackRequest,
              (int)&pappcontrol->callback) ;
      papply->pact = TRUE ;
    }

  }

  return(status);

}

static long put_links (struct applyRecord *papply) 
{
  long status ;
  int i ;

/* Send clid & dir to all out links */
  for (i = 0 ; i < ARG_MAX; i++) {

    status = recGblPutFastLink (&papply->ocla + i, (void *)papply, 
        &papply->clid) ;
    if (status != 0)
        break ;

    status = recGblPutFastLink (&papply->outa + i, (void *) papply,
        &papply->dir) ;
    if (status !=0)
        break ;

  }
  return (status) ;

}

static long get_links (struct applyRecord *papply)
{
  long status ;
  int i ;

  for (i = 0; i < ARG_MAX; i++) {

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

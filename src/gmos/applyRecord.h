
/* applyRecord.h */
#ifndef INCapplyRecordh
#define INCapplyRecordh
#include <ellLib.h>	/* list structure definition for the monitor list */
#ifndef	INCLsemLibh
#include <semLib.h>	/* semaphore structure definition for the monitor lock*/
#endif
#ifndef INCLfast_lockh
#include <fast_lock.h>	/* fast lock data structures and macros */
#endif
#ifndef INCLlinkh
#include <link.h>	/* fast lock data structures and macros */
#endif
#ifndef INC_tsDefs_h
#include <tsDefs.h>	/* time-stamp related definitions */
#endif
struct applyRecord	{
	char		name[29];		/* Record Name */
	char		desc[29];		/* Descriptor */
	char		asg[29];		/* Access Security Group */
	char		p1  [1];		/* Created Pad  */
	unsigned short	scan;		/* Scan Mechanism */
	unsigned short	pini;		/* Process at iocInit */
	short		phas;		/* Scan Phase */
	short		evnt;		/* Event Number */
	short		tse;		/* Time Stamp Event */
	char		p2  [6];		/* Created Pad  */
	struct link	tsel;		/* Time Stamp Link */
	unsigned short	dtyp;		/* Device Type */
	short		disv;		/* Disable Value */
	short		disa;		/* Disable */
	char		p3  [2];		/* Created Pad  */
	struct link	sdis;		/* Scanning Disable */
	FAST_LOCK	mlok;		/* Monitor fastlock */
	ELLLIST		mlis;		/* Monitor List */
	unsigned char	disp;		/* Disable putField */
	unsigned char	proc;		/* Force Processing */
	unsigned short	stat;		/* Alarm Status */
	unsigned short	sevr;		/* Alarm Severity */
	unsigned short	nsta;		/* New Alarm Status */
	unsigned short	nsev;		/* New Alarm Severity */
	unsigned short	acks;		/* Alarm Ack Severity */
	unsigned short	ackt;		/* Alarm Ack Transient */
	unsigned short	diss;		/* Disable Alarm Sevrty */
	short		lset;		/* Lock Set */
	unsigned char	lcnt;		/* Lock Count */
	unsigned char	pact;		/* Record active */
	unsigned char	putf;		/* dbPutField process */
	unsigned char	rpro;		/* Reprocess  */
	char		p4  [2];		/* Created Pad  */
	void		*asp;		/* Access Security Pvt */
	void		*ppn;		/* addr of PUTNOTIFY */
	void		*ppnn;		/* next record PUTNOTIFY */
	void		*spvt;		/* Scan Private */
	void		*rset;		/* Address of RSET */
	struct dset	*dset;		/* DSET address */
	void		*dpvt;		/* Device Private */
	unsigned short	prio;		/* Scheduling Priority */
	unsigned char	tpro;		/* Trace Processing */
	char bkpt;		/* Break Point */
	unsigned char	udf;		/* Undefined */
	char		p5  [3];		/* Created Pad  */
	TS_STAMP	time;		/* Time */
	struct link	flnk;		/* Forward Process Link */
	/* start of apply specific fields */ 
	long		val;		/* Return Error Code */
	unsigned short	dir;		/* CAD Directive */
	char		p6  [2];		/* Created Pad  */
	long		clid;		/* Client ID */
	char		mess[40];		/* Message */
	char		omss[40];		/* Old Message */
	short		mark;		/* Mark */
	char		p7  [2];		/* Created Pad  */
	struct link	outa;		/* Output Link A */
	struct link	outb;		/* Output Link B */
	struct link	outc;		/* Output Link C */
	struct link	outd;		/* Output Link D */
	struct link	oute;		/* Output Link E */
	struct link	outf;		/* Output Link F */
	struct link	outg;		/* Output Link G */
	struct link	outh;		/* Output Link H */
	struct link	ocla;		/* Output CLID Link A */
	struct link	oclb;		/* Output CLID Link B */
	struct link	oclc;		/* Output CLID Link C */
	struct link	ocld;		/* Output CLID Link D */
	struct link	ocle;		/* Output CLID Link E */
	struct link	oclf;		/* Output CLID Link F */
	struct link	oclg;		/* Output CLID Link G */
	struct link	oclh;		/* Output CLID Link H */
	struct link	inpa;		/* Input Link A */
	struct link	inpb;		/* Input Link B */
	struct link	inpc;		/* Input Link C */
	struct link	inpd;		/* Input Link D */
	struct link	inpe;		/* Input Link E */
	struct link	inpf;		/* Input Link F */
	struct link	inpg;		/* Input Link G */
	struct link	inph;		/* Input Link H */
	struct link	inma;		/* Input Msg Link A */
	struct link	inmb;		/* Input Msg Link B */
	struct link	inmc;		/* Input Msg Link C */
	struct link	inmd;		/* Input Msg Link D */
	struct link	inme;		/* Input Msg Link E */
	struct link	inmf;		/* Input Msg Link F */
	struct link	inmg;		/* Input Msg Link G */
	struct link	inmh;		/* Input Msg Link H */
};
typedef struct applyRecord applyRecord;
#endif


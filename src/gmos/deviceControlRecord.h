
/* deviceControlRecord.h */
#ifndef INCdeviceControlRecordh
#define INCdeviceControlRecordh
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
struct deviceControlRecord	{
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
	/* start of deviceControl specific fields */ 
	unsigned short	dir;		/* Directive */
	unsigned short	mode;		/* Operating Mode */
	char		p146[4];		/* Created Pad  */
	double		val;		/* User Position Value */
	char		vals[40];		/* User Position String */
	float		velo;		/* Velocity (EGU/s) */
	float		vbas;		/* Base Velocity (EGU/s) */
	float		fivl;		/* Final Index Vel (EGU/s) */
	float		accl;		/* Acceleration (EGU/Sec) */
	float		mres;		/* Motor Resolution */
	float		eres;		/* Encoder Resolution */
	long		ialg;		/* Index Algorithm */
	float		blco;		/* Backlash Correct (EGU) */
	long		pwr;		/* Power Control */
	long		brk;		/* Brake Control */
	long		psta;		/* Power Feedback State */
	long		bsta;		/* Brake Feedback State */
	long		flt;		/* Fault */
	unsigned short	uapb;		/* Use Auxiliary Power Bit */
	unsigned short	ueip;		/* Use Encoder if Present */
	unsigned short	upsb;		/* Use Power Status Bit */
	unsigned short	ubsb;		/* Use Brake Status Bit */
	float		mdbd;		/* Motion Deadband (EGU) */
	long		edbd;		/* Encoder Deadband */
	long		mrnd;		/* Position rounding val */
	unsigned short	omsl;		/* Output Mode Select */
	unsigned short	simm;		/* Simulation Mode */
	unsigned short	dbug;		/* Debug Mode */
	char		tdir[40];		/* Translation Directory */
	char		tfil[40];		/* Translation File */
	char		p147[2];		/* Created Pad  */
	struct link	out;		/* Output Specification */
	struct link	dol;		/* Desired Output Loc */
	struct link	bsyl;		/* Busy Link */
	struct link	msgl;		/* Error Message Link */
	struct link	pwrl;		/* Power Control Link */
	struct link	brkl;		/* Brake Control Link */
	struct link	siml;		/* Simulation Mode Link */
	struct link	dbgl;		/* Debug Mode Link */
	long		ack;		/* Command Status */
	unsigned short	busy;		/* Device Action State */
	char		mess[40];		/* Error Message */
	char		p148[2];		/* Created Pad  */
	double		mpos;		/* Motor Position */
	unsigned short	mip;		/* Motion In Progress */
	char		p149[2];		/* Created Pad  */
	long		msta;		/* Motor Status */
	short		hpvl;		/* Home Position Valid */
	short		hswa;		/* Home Switch Active */
	short		lswa;		/* Limit Switch Active */
	char		p150[2];		/* Created Pad  */
	long		rpos;		/* Raw motor position */
	long		rvel;		/* Raw motor velocity */
	long		rrbv;		/* Raw position readback */
	long		renc;		/* Raw encoder readback */
	float		phlm;		/* Position High Limit */
	float		pllm;		/* Position Low Limit */
	float		vhlm;		/* Velocity High Limit */
	float		vllm;		/* Velocity Low Limit */
	short		lvio;		/* Limit Violation */
	short		btmo;		/* Brake Timeout */
	short		ptmo;		/* Power Timeout */
	short		prec;		/* Display Precision */
	char		egu[16];		/* Engineering Units */
	float		vers;		/* Code Version */
	char		p151[4];		/* Created Pad  */
	double		alst;		/* Last Value Archived */
	double		mlst;		/* Last Value Monitored */
	short		pp;		/* Post process command */
	short		osta;		/* Operating State */
	void *lthp;		/* Lookup Table Head Ptr */
	unsigned long	mmap;		/* Monitor Mask */
	unsigned long	lmap;		/* Link Trigger Mask */
	float		mdel;		/* Monitor Deadband */
	float		adel;		/* Archive Deadband */
	float		hihi;		/* Hihi Alarm Limit */
	float		lolo;		/* Lolo Alarm Limit */
	float		high;		/* High Alarm Limit */
	float		low;		/* Low Alarm Limit */
	unsigned short	hhsv;		/* Hihi Severity */
	unsigned short	llsv;		/* Lolo Severity */
	unsigned short	hsv;		/* High Severity */
	unsigned short	lsv;		/* Low Severity */
	unsigned short	hlsv;		/* HW Limit Violation Svr */
};
typedef struct deviceControlRecord deviceControlRecord;
#endif


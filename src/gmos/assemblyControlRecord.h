#include <vxWorks.h>
#include <semLib.h>
#include "ellLib.h"
#include "fast_lock.h"
#include "link.h"
#include "tsDefs.h"

#ifndef INCassemblyControlSIMMH
#define INCassemblyControlSIMMH
typedef enum {
	assemblyControlSIMM_NONE,
	assemblyControlSIMM_VSM,
	assemblyControlSIMM_FAST,
	assemblyControlSIMM_FULL
}assemblyControlSIMM;
#endif /*INCassemblyControlSIMMH*/

#ifndef INCassemblyControlMODEH
#define INCassemblyControlMODEH
typedef enum {
	assemblyControlMODE_INIT,
	assemblyControlMODE_MOVE,
	assemblyControlMODE_TRACK,
	assemblyControlMODE_INDEX,
	assemblyControlMODE_PARK,
	assemblyControlMODE_TEST,
	assemblyControlMODE_UPDATE
}assemblyControlMODE;
#endif /*INCassemblyControlMODEH*/

#ifndef INCassemblyControlHLTHH
#define INCassemblyControlHLTHH
typedef enum {
	assemblyControlHLTH_GOOD,
	assemblyControlHLTH_WARNING,
	assemblyControlHLTH_BAD
}assemblyControlHLTH;
#endif /*INCassemblyControlHLTHH*/

#ifndef INCassemblyControlDIRH
#define INCassemblyControlDIRH
typedef enum {
	assemblyControlDIR_MARK,
	assemblyControlDIR_CLEAR,
	assemblyControlDIR_PRESET,
	assemblyControlDIR_START,
	assemblyControlDIR_STOP
}assemblyControlDIR;
#endif /*INCassemblyControlDIRH*/

#ifndef INCassemblyControlDBUGH
#define INCassemblyControlDBUGH
typedef enum {
	assemblyControlDBUG_QUIET,
	assemblyControlDBUG_NONE,
	assemblyControlDBUG_MIN,
	assemblyControlDBUG_FULL,
	assemblyControlDBUG_MAX
}assemblyControlDBUG;
#endif /*INCassemblyControlDBUGH*/

#ifndef INCassemblyControlBUSYH
#define INCassemblyControlBUSYH
typedef enum {
	assemblyControlBUSY_IDLE,
	assemblyControlBUSY_PAUSED,
	assemblyControlBUSY_BUSY,
	assemblyControlBUSY_ERR
}assemblyControlBUSY;
#endif /*INCassemblyControlBUSYH*/

#ifndef INCassemblyControlASTAH
#define INCassemblyControlASTAH
typedef enum {
	assemblyControlASTA_IDLE,
	assemblyControlASTA_INITIALIZING,
	assemblyControlASTA_MOVING,
	assemblyControlASTA_TRACKING,
	assemblyControlASTA_TESTING,
	assemblyControlASTA_INDEXING,
	assemblyControlASTA_UPDATING,
	assemblyControlASTA_STARTING
}assemblyControlASTA;
#endif /*INCassemblyControlASTAH*/
#ifndef INCassemblyControlH
#define INCassemblyControlH
typedef struct assemblyControlRecord {
	char		name[29]; /*Record Name*/
	char		desc[29]; /*Descriptor*/
	char		asg[29]; /*Access Security Group*/
	unsigned short	scan;	/*Scan Mechanism*/
	unsigned short	pini;	/*Process at iocInit*/
	short		phas;	/*Scan Phase*/
	short		evnt;	/*Event Number*/
	short		tse;	/*Time Stamp Event*/
	DBLINK		tsel;	/*Time Stamp Link*/
	unsigned short	dtyp;	/*Device Type*/
	short		disv;	/*Disable Value*/
	short		disa;	/*Disable*/
	DBLINK		sdis;	/*Scanning Disable*/
	FAST_LOCK	mlok;	/*Monitor fastlock*/
	ELLLIST		mlis;	/*Monitor List*/
	unsigned char	disp;	/*Disable putField*/
	unsigned char	proc;	/*Force Processing*/
	unsigned short	stat;	/*Alarm Status*/
	unsigned short	sevr;	/*Alarm Severity*/
	unsigned short	nsta;	/*New Alarm Status*/
	unsigned short	nsev;	/*New Alarm Severity*/
	unsigned short	acks;	/*Alarm Ack Severity*/
	unsigned short	ackt;	/*Alarm Ack Transient*/
	unsigned short	diss;	/*Disable Alarm Sevrty*/
	unsigned char	lcnt;	/*Lock Count*/
	unsigned char	pact;	/*Record active*/
	unsigned char	putf;	/*dbPutField process*/
	unsigned char	rpro;	/*Reprocess */
	void		*asp;	/*Access Security Pvt*/
	struct putNotify *ppn;	/*addr of PUTNOTIFY*/
	struct pnWaitNode *ppnn;	/*next record PUTNOTIFY*/
	struct scan_element *spvt;	/*Scan Private*/
	struct rset	*rset;	/*Address of RSET*/
	struct dset	*dset;	/*DSET address*/
	void		*dpvt;	/*Device Private*/
	struct dbRecordType *rdes;	/*Address of dbRecordType*/
	struct lockRecord *lset;	/*Lock Set*/
	unsigned short	prio;	/*Scheduling Priority*/
	unsigned char	tpro;	/*Trace Processing*/
	char bkpt;	/*Break Point*/
	unsigned char	udf;	/*Undefined*/
	TS_STAMP	time;	/*Time*/
	DBLINK		flnk;	/*Forward Process Link*/
	unsigned short	dir;	/*Directive*/
	unsigned short	mode;	/*Operating Mode*/
	long		val;	/*Directive Response*/
	short		ilck;	/*Interlock*/
	unsigned short	busy;	/*Device Action State*/
	unsigned short	asta;	/*Assembly Rec State*/
	char		mess[40]; /*Error Message*/
	unsigned short	hlth;	/*Health*/
	short		indx;	/*Is it Indexed*/
	short		init;	/*Is it Initialized*/
	short		park;	/*Is it Parked*/
	short		mark;	/*Is Record Preset?*/
	unsigned short	simm;	/*Simulation Mode*/
	unsigned short	dbug;	/*Debug Mode*/
	char		tdir[40]; /*Translation Directory*/
	char		tfil[40]; /*Translation File*/
	DBLINK		msgl;	/*Error Message Link*/
	DBLINK		siml;	/*Simulation Mode Link*/
	DBLINK		dbgl;	/*Debug Mode Link*/
	DBLINK		bsyl;	/*Busy Link*/
	short		prec;	/*Display Precision*/
	char		egu[16]; /*Engineering Units*/
	char		vers[16]; /*Code Version*/
	short		pp;	/*Post process command*/
	void *lthp;	/*Lookup Table Head Ptr*/
	short		nmdv;	/*Total devices attached*/
	float		mdel;	/*Monitor Deadband*/
	float		adel;	/*Archive Deadband*/
	unsigned long	mmap;	/*Monitor Mask*/
	unsigned long	lmap;	/*Link Trigger Mask*/
	float		hihi;	/*Hihi Alarm Limit*/
	float		lolo;	/*Lolo Alarm Limit*/
	float		high;	/*High Alarm Limit*/
	float		low;	/*Low Alarm Limit*/
	unsigned short	hhsv;	/*Hihi Severity*/
	unsigned short	llsv;	/*Lolo Severity*/
	unsigned short	hsv;	/*High Severity*/
	unsigned short	lsv;	/*Low Severity*/
	unsigned short	hlsv;	/*HW Limit Violation Svr*/
	void *a;	/*Attribute A*/
	void *b;	/*Attribute B*/
	void *c;	/*Attribute C*/
	void *d;	/*Attribute D*/
	void *e;	/*Attribute E*/
	void *vala;	/*Out Attr A*/
	void *valb;	/*Out Attr B*/
	void *valc;	/*Out Attr C*/
	void *vald;	/*Out Attr D*/
	void *vale;	/*Out Attr E*/
	unsigned short	fta;	/*Attr A Data Type*/
	unsigned short	ftb;	/*Attr B Data Type*/
	unsigned short	ftc;	/*Attr C Data Type*/
	unsigned short	ftd;	/*Attr D Data Type*/
	unsigned short	fte;	/*Attr E Data Type*/
	double		aahl;	/*Attr A High Limit*/
	double		aall;	/*Attr A Low Limit*/
	double		abhl;	/*Attr B High Limit*/
	double		abll;	/*Attr B Low Limit*/
	double		achl;	/*Attr C High Limit*/
	double		acll;	/*Attr C Low Limit*/
	double		adhl;	/*Attr D High Limit*/
	double		adll;	/*Attr D Low Limit*/
	double		aehl;	/*Attr E High Limit*/
	double		aell;	/*Attr E Low Limit*/
	DBLINK		ack1;	/*Command Stat from 1*/
	DBLINK		ack2;	/*Command Stat from 2*/
	DBLINK		ack3;	/*Command Stat from 3*/
	DBLINK		ack4;	/*Command Stat from 4*/
	DBLINK		ack5;	/*Command Stat from 5*/
	short		bus1;	/*Device Action from 1*/
	short		bus2;	/*Device Action from 2*/
	short		bus3;	/*Device Action from 3*/
	short		bus4;	/*Device Action from 4*/
	short		bus5;	/*Device Action from 5*/
	DBLINK		odr1;	/*Directive to 1*/
	DBLINK		odr2;	/*Directive to 2*/
	DBLINK		odr3;	/*Directive to 3*/
	DBLINK		odr4;	/*Directive to 4*/
	DBLINK		odr5;	/*Directive to 5*/
	DBLINK		mod1;	/*Operating Mode to 1*/
	DBLINK		mod2;	/*Operating Mode to 2*/
	DBLINK		mod3;	/*Operating Mode to 3*/
	DBLINK		mod4;	/*Operating Mode to 4*/
	DBLINK		mod5;	/*Operating Mode to 5*/
	DBLINK		pos1;	/*Position to 1*/
	DBLINK		pos2;	/*Position to 2*/
	DBLINK		pos3;	/*Position to 3*/
	DBLINK		pos4;	/*Position to 4*/
	DBLINK		pos5;	/*Position to 5*/
	DBLINK		vel1;	/*Velocity to 1*/
	DBLINK		vel2;	/*Velocity to 2*/
	DBLINK		vel3;	/*Velocity to 3*/
	DBLINK		vel4;	/*Velocity to 4*/
	DBLINK		vel5;	/*Velocity to 5*/
	void *sij;	/*Sensor In J*/
	void *sik;	/*Sensor In K*/
	void *sil;	/*Sensor In L*/
	void *sim;	/*Sensor In M*/
	void *sin;	/*Sensor In N*/
	unsigned short	ftsj;	/*Sensor In Data Type J*/
	unsigned short	ftsk;	/*Sensor In Data Type K*/
	unsigned short	ftsl;	/*Sensor In Data Type L*/
	unsigned short	ftsm;	/*Sensor In Data Type M*/
	unsigned short	ftsn;	/*Sensor In Data Type N*/
	DBLINK		sor;	/*Sensor Out R*/
	DBLINK		sos;	/*Sensor Out S*/
	DBLINK		sot;	/*Sensor Out T*/
	DBLINK		sou;	/*Sensor Out U*/
	DBLINK		sov;	/*Sensor Out V*/
} assemblyControlRecord;
#define assemblyControlRecordNAME	0
#define assemblyControlRecordDESC	1
#define assemblyControlRecordASG	2
#define assemblyControlRecordSCAN	3
#define assemblyControlRecordPINI	4
#define assemblyControlRecordPHAS	5
#define assemblyControlRecordEVNT	6
#define assemblyControlRecordTSE	7
#define assemblyControlRecordTSEL	8
#define assemblyControlRecordDTYP	9
#define assemblyControlRecordDISV	10
#define assemblyControlRecordDISA	11
#define assemblyControlRecordSDIS	12
#define assemblyControlRecordMLOK	13
#define assemblyControlRecordMLIS	14
#define assemblyControlRecordDISP	15
#define assemblyControlRecordPROC	16
#define assemblyControlRecordSTAT	17
#define assemblyControlRecordSEVR	18
#define assemblyControlRecordNSTA	19
#define assemblyControlRecordNSEV	20
#define assemblyControlRecordACKS	21
#define assemblyControlRecordACKT	22
#define assemblyControlRecordDISS	23
#define assemblyControlRecordLCNT	24
#define assemblyControlRecordPACT	25
#define assemblyControlRecordPUTF	26
#define assemblyControlRecordRPRO	27
#define assemblyControlRecordASP	28
#define assemblyControlRecordPPN	29
#define assemblyControlRecordPPNN	30
#define assemblyControlRecordSPVT	31
#define assemblyControlRecordRSET	32
#define assemblyControlRecordDSET	33
#define assemblyControlRecordDPVT	34
#define assemblyControlRecordRDES	35
#define assemblyControlRecordLSET	36
#define assemblyControlRecordPRIO	37
#define assemblyControlRecordTPRO	38
#define assemblyControlRecordBKPT	39
#define assemblyControlRecordUDF	40
#define assemblyControlRecordTIME	41
#define assemblyControlRecordFLNK	42
#define assemblyControlRecordDIR	43
#define assemblyControlRecordMODE	44
#define assemblyControlRecordVAL	45
#define assemblyControlRecordILCK	46
#define assemblyControlRecordBUSY	47
#define assemblyControlRecordASTA	48
#define assemblyControlRecordMESS	49
#define assemblyControlRecordHLTH	50
#define assemblyControlRecordINDX	51
#define assemblyControlRecordINIT	52
#define assemblyControlRecordPARK	53
#define assemblyControlRecordMARK	54
#define assemblyControlRecordSIMM	55
#define assemblyControlRecordDBUG	56
#define assemblyControlRecordTDIR	57
#define assemblyControlRecordTFIL	58
#define assemblyControlRecordMSGL	59
#define assemblyControlRecordSIML	60
#define assemblyControlRecordDBGL	61
#define assemblyControlRecordBSYL	62
#define assemblyControlRecordPREC	63
#define assemblyControlRecordEGU	64
#define assemblyControlRecordVERS	65
#define assemblyControlRecordPP	66
#define assemblyControlRecordLTHP	67
#define assemblyControlRecordNMDV	68
#define assemblyControlRecordMDEL	69
#define assemblyControlRecordADEL	70
#define assemblyControlRecordMMAP	71
#define assemblyControlRecordLMAP	72
#define assemblyControlRecordHIHI	73
#define assemblyControlRecordLOLO	74
#define assemblyControlRecordHIGH	75
#define assemblyControlRecordLOW	76
#define assemblyControlRecordHHSV	77
#define assemblyControlRecordLLSV	78
#define assemblyControlRecordHSV	79
#define assemblyControlRecordLSV	80
#define assemblyControlRecordHLSV	81
#define assemblyControlRecordA	82
#define assemblyControlRecordB	83
#define assemblyControlRecordC	84
#define assemblyControlRecordD	85
#define assemblyControlRecordE	86
#define assemblyControlRecordVALA	87
#define assemblyControlRecordVALB	88
#define assemblyControlRecordVALC	89
#define assemblyControlRecordVALD	90
#define assemblyControlRecordVALE	91
#define assemblyControlRecordFTA	92
#define assemblyControlRecordFTB	93
#define assemblyControlRecordFTC	94
#define assemblyControlRecordFTD	95
#define assemblyControlRecordFTE	96
#define assemblyControlRecordAAHL	97
#define assemblyControlRecordAALL	98
#define assemblyControlRecordABHL	99
#define assemblyControlRecordABLL	100
#define assemblyControlRecordACHL	101
#define assemblyControlRecordACLL	102
#define assemblyControlRecordADHL	103
#define assemblyControlRecordADLL	104
#define assemblyControlRecordAEHL	105
#define assemblyControlRecordAELL	106
#define assemblyControlRecordACK1	107
#define assemblyControlRecordACK2	108
#define assemblyControlRecordACK3	109
#define assemblyControlRecordACK4	110
#define assemblyControlRecordACK5	111
#define assemblyControlRecordBUS1	112
#define assemblyControlRecordBUS2	113
#define assemblyControlRecordBUS3	114
#define assemblyControlRecordBUS4	115
#define assemblyControlRecordBUS5	116
#define assemblyControlRecordODR1	117
#define assemblyControlRecordODR2	118
#define assemblyControlRecordODR3	119
#define assemblyControlRecordODR4	120
#define assemblyControlRecordODR5	121
#define assemblyControlRecordMOD1	122
#define assemblyControlRecordMOD2	123
#define assemblyControlRecordMOD3	124
#define assemblyControlRecordMOD4	125
#define assemblyControlRecordMOD5	126
#define assemblyControlRecordPOS1	127
#define assemblyControlRecordPOS2	128
#define assemblyControlRecordPOS3	129
#define assemblyControlRecordPOS4	130
#define assemblyControlRecordPOS5	131
#define assemblyControlRecordVEL1	132
#define assemblyControlRecordVEL2	133
#define assemblyControlRecordVEL3	134
#define assemblyControlRecordVEL4	135
#define assemblyControlRecordVEL5	136
#define assemblyControlRecordSIJ	137
#define assemblyControlRecordSIK	138
#define assemblyControlRecordSIL	139
#define assemblyControlRecordSIM	140
#define assemblyControlRecordSIN	141
#define assemblyControlRecordFTSJ	142
#define assemblyControlRecordFTSK	143
#define assemblyControlRecordFTSL	144
#define assemblyControlRecordFTSM	145
#define assemblyControlRecordFTSN	146
#define assemblyControlRecordSOR	147
#define assemblyControlRecordSOS	148
#define assemblyControlRecordSOT	149
#define assemblyControlRecordSOU	150
#define assemblyControlRecordSOV	151
#endif /*INCassemblyControlH*/
#ifdef GEN_SIZE_OFFSET
int assemblyControlRecordSizeOffset(dbRecordType *pdbRecordType)
{
    assemblyControlRecord *prec = 0;
  pdbRecordType->papFldDes[0]->size=sizeof(prec->name);
  pdbRecordType->papFldDes[0]->offset=(short)((char *)&prec->name - (char *)prec);
  pdbRecordType->papFldDes[1]->size=sizeof(prec->desc);
  pdbRecordType->papFldDes[1]->offset=(short)((char *)&prec->desc - (char *)prec);
  pdbRecordType->papFldDes[2]->size=sizeof(prec->asg);
  pdbRecordType->papFldDes[2]->offset=(short)((char *)&prec->asg - (char *)prec);
  pdbRecordType->papFldDes[3]->size=sizeof(prec->scan);
  pdbRecordType->papFldDes[3]->offset=(short)((char *)&prec->scan - (char *)prec);
  pdbRecordType->papFldDes[4]->size=sizeof(prec->pini);
  pdbRecordType->papFldDes[4]->offset=(short)((char *)&prec->pini - (char *)prec);
  pdbRecordType->papFldDes[5]->size=sizeof(prec->phas);
  pdbRecordType->papFldDes[5]->offset=(short)((char *)&prec->phas - (char *)prec);
  pdbRecordType->papFldDes[6]->size=sizeof(prec->evnt);
  pdbRecordType->papFldDes[6]->offset=(short)((char *)&prec->evnt - (char *)prec);
  pdbRecordType->papFldDes[7]->size=sizeof(prec->tse);
  pdbRecordType->papFldDes[7]->offset=(short)((char *)&prec->tse - (char *)prec);
  pdbRecordType->papFldDes[8]->size=sizeof(prec->tsel);
  pdbRecordType->papFldDes[8]->offset=(short)((char *)&prec->tsel - (char *)prec);
  pdbRecordType->papFldDes[9]->size=sizeof(prec->dtyp);
  pdbRecordType->papFldDes[9]->offset=(short)((char *)&prec->dtyp - (char *)prec);
  pdbRecordType->papFldDes[10]->size=sizeof(prec->disv);
  pdbRecordType->papFldDes[10]->offset=(short)((char *)&prec->disv - (char *)prec);
  pdbRecordType->papFldDes[11]->size=sizeof(prec->disa);
  pdbRecordType->papFldDes[11]->offset=(short)((char *)&prec->disa - (char *)prec);
  pdbRecordType->papFldDes[12]->size=sizeof(prec->sdis);
  pdbRecordType->papFldDes[12]->offset=(short)((char *)&prec->sdis - (char *)prec);
  pdbRecordType->papFldDes[13]->size=sizeof(prec->mlok);
  pdbRecordType->papFldDes[13]->offset=(short)((char *)&prec->mlok - (char *)prec);
  pdbRecordType->papFldDes[14]->size=sizeof(prec->mlis);
  pdbRecordType->papFldDes[14]->offset=(short)((char *)&prec->mlis - (char *)prec);
  pdbRecordType->papFldDes[15]->size=sizeof(prec->disp);
  pdbRecordType->papFldDes[15]->offset=(short)((char *)&prec->disp - (char *)prec);
  pdbRecordType->papFldDes[16]->size=sizeof(prec->proc);
  pdbRecordType->papFldDes[16]->offset=(short)((char *)&prec->proc - (char *)prec);
  pdbRecordType->papFldDes[17]->size=sizeof(prec->stat);
  pdbRecordType->papFldDes[17]->offset=(short)((char *)&prec->stat - (char *)prec);
  pdbRecordType->papFldDes[18]->size=sizeof(prec->sevr);
  pdbRecordType->papFldDes[18]->offset=(short)((char *)&prec->sevr - (char *)prec);
  pdbRecordType->papFldDes[19]->size=sizeof(prec->nsta);
  pdbRecordType->papFldDes[19]->offset=(short)((char *)&prec->nsta - (char *)prec);
  pdbRecordType->papFldDes[20]->size=sizeof(prec->nsev);
  pdbRecordType->papFldDes[20]->offset=(short)((char *)&prec->nsev - (char *)prec);
  pdbRecordType->papFldDes[21]->size=sizeof(prec->acks);
  pdbRecordType->papFldDes[21]->offset=(short)((char *)&prec->acks - (char *)prec);
  pdbRecordType->papFldDes[22]->size=sizeof(prec->ackt);
  pdbRecordType->papFldDes[22]->offset=(short)((char *)&prec->ackt - (char *)prec);
  pdbRecordType->papFldDes[23]->size=sizeof(prec->diss);
  pdbRecordType->papFldDes[23]->offset=(short)((char *)&prec->diss - (char *)prec);
  pdbRecordType->papFldDes[24]->size=sizeof(prec->lcnt);
  pdbRecordType->papFldDes[24]->offset=(short)((char *)&prec->lcnt - (char *)prec);
  pdbRecordType->papFldDes[25]->size=sizeof(prec->pact);
  pdbRecordType->papFldDes[25]->offset=(short)((char *)&prec->pact - (char *)prec);
  pdbRecordType->papFldDes[26]->size=sizeof(prec->putf);
  pdbRecordType->papFldDes[26]->offset=(short)((char *)&prec->putf - (char *)prec);
  pdbRecordType->papFldDes[27]->size=sizeof(prec->rpro);
  pdbRecordType->papFldDes[27]->offset=(short)((char *)&prec->rpro - (char *)prec);
  pdbRecordType->papFldDes[28]->size=sizeof(prec->asp);
  pdbRecordType->papFldDes[28]->offset=(short)((char *)&prec->asp - (char *)prec);
  pdbRecordType->papFldDes[29]->size=sizeof(prec->ppn);
  pdbRecordType->papFldDes[29]->offset=(short)((char *)&prec->ppn - (char *)prec);
  pdbRecordType->papFldDes[30]->size=sizeof(prec->ppnn);
  pdbRecordType->papFldDes[30]->offset=(short)((char *)&prec->ppnn - (char *)prec);
  pdbRecordType->papFldDes[31]->size=sizeof(prec->spvt);
  pdbRecordType->papFldDes[31]->offset=(short)((char *)&prec->spvt - (char *)prec);
  pdbRecordType->papFldDes[32]->size=sizeof(prec->rset);
  pdbRecordType->papFldDes[32]->offset=(short)((char *)&prec->rset - (char *)prec);
  pdbRecordType->papFldDes[33]->size=sizeof(prec->dset);
  pdbRecordType->papFldDes[33]->offset=(short)((char *)&prec->dset - (char *)prec);
  pdbRecordType->papFldDes[34]->size=sizeof(prec->dpvt);
  pdbRecordType->papFldDes[34]->offset=(short)((char *)&prec->dpvt - (char *)prec);
  pdbRecordType->papFldDes[35]->size=sizeof(prec->rdes);
  pdbRecordType->papFldDes[35]->offset=(short)((char *)&prec->rdes - (char *)prec);
  pdbRecordType->papFldDes[36]->size=sizeof(prec->lset);
  pdbRecordType->papFldDes[36]->offset=(short)((char *)&prec->lset - (char *)prec);
  pdbRecordType->papFldDes[37]->size=sizeof(prec->prio);
  pdbRecordType->papFldDes[37]->offset=(short)((char *)&prec->prio - (char *)prec);
  pdbRecordType->papFldDes[38]->size=sizeof(prec->tpro);
  pdbRecordType->papFldDes[38]->offset=(short)((char *)&prec->tpro - (char *)prec);
  pdbRecordType->papFldDes[39]->size=sizeof(prec->bkpt);
  pdbRecordType->papFldDes[39]->offset=(short)((char *)&prec->bkpt - (char *)prec);
  pdbRecordType->papFldDes[40]->size=sizeof(prec->udf);
  pdbRecordType->papFldDes[40]->offset=(short)((char *)&prec->udf - (char *)prec);
  pdbRecordType->papFldDes[41]->size=sizeof(prec->time);
  pdbRecordType->papFldDes[41]->offset=(short)((char *)&prec->time - (char *)prec);
  pdbRecordType->papFldDes[42]->size=sizeof(prec->flnk);
  pdbRecordType->papFldDes[42]->offset=(short)((char *)&prec->flnk - (char *)prec);
  pdbRecordType->papFldDes[43]->size=sizeof(prec->dir);
  pdbRecordType->papFldDes[43]->offset=(short)((char *)&prec->dir - (char *)prec);
  pdbRecordType->papFldDes[44]->size=sizeof(prec->mode);
  pdbRecordType->papFldDes[44]->offset=(short)((char *)&prec->mode - (char *)prec);
  pdbRecordType->papFldDes[45]->size=sizeof(prec->val);
  pdbRecordType->papFldDes[45]->offset=(short)((char *)&prec->val - (char *)prec);
  pdbRecordType->papFldDes[46]->size=sizeof(prec->ilck);
  pdbRecordType->papFldDes[46]->offset=(short)((char *)&prec->ilck - (char *)prec);
  pdbRecordType->papFldDes[47]->size=sizeof(prec->busy);
  pdbRecordType->papFldDes[47]->offset=(short)((char *)&prec->busy - (char *)prec);
  pdbRecordType->papFldDes[48]->size=sizeof(prec->asta);
  pdbRecordType->papFldDes[48]->offset=(short)((char *)&prec->asta - (char *)prec);
  pdbRecordType->papFldDes[49]->size=sizeof(prec->mess);
  pdbRecordType->papFldDes[49]->offset=(short)((char *)&prec->mess - (char *)prec);
  pdbRecordType->papFldDes[50]->size=sizeof(prec->hlth);
  pdbRecordType->papFldDes[50]->offset=(short)((char *)&prec->hlth - (char *)prec);
  pdbRecordType->papFldDes[51]->size=sizeof(prec->indx);
  pdbRecordType->papFldDes[51]->offset=(short)((char *)&prec->indx - (char *)prec);
  pdbRecordType->papFldDes[52]->size=sizeof(prec->init);
  pdbRecordType->papFldDes[52]->offset=(short)((char *)&prec->init - (char *)prec);
  pdbRecordType->papFldDes[53]->size=sizeof(prec->park);
  pdbRecordType->papFldDes[53]->offset=(short)((char *)&prec->park - (char *)prec);
  pdbRecordType->papFldDes[54]->size=sizeof(prec->mark);
  pdbRecordType->papFldDes[54]->offset=(short)((char *)&prec->mark - (char *)prec);
  pdbRecordType->papFldDes[55]->size=sizeof(prec->simm);
  pdbRecordType->papFldDes[55]->offset=(short)((char *)&prec->simm - (char *)prec);
  pdbRecordType->papFldDes[56]->size=sizeof(prec->dbug);
  pdbRecordType->papFldDes[56]->offset=(short)((char *)&prec->dbug - (char *)prec);
  pdbRecordType->papFldDes[57]->size=sizeof(prec->tdir);
  pdbRecordType->papFldDes[57]->offset=(short)((char *)&prec->tdir - (char *)prec);
  pdbRecordType->papFldDes[58]->size=sizeof(prec->tfil);
  pdbRecordType->papFldDes[58]->offset=(short)((char *)&prec->tfil - (char *)prec);
  pdbRecordType->papFldDes[59]->size=sizeof(prec->msgl);
  pdbRecordType->papFldDes[59]->offset=(short)((char *)&prec->msgl - (char *)prec);
  pdbRecordType->papFldDes[60]->size=sizeof(prec->siml);
  pdbRecordType->papFldDes[60]->offset=(short)((char *)&prec->siml - (char *)prec);
  pdbRecordType->papFldDes[61]->size=sizeof(prec->dbgl);
  pdbRecordType->papFldDes[61]->offset=(short)((char *)&prec->dbgl - (char *)prec);
  pdbRecordType->papFldDes[62]->size=sizeof(prec->bsyl);
  pdbRecordType->papFldDes[62]->offset=(short)((char *)&prec->bsyl - (char *)prec);
  pdbRecordType->papFldDes[63]->size=sizeof(prec->prec);
  pdbRecordType->papFldDes[63]->offset=(short)((char *)&prec->prec - (char *)prec);
  pdbRecordType->papFldDes[64]->size=sizeof(prec->egu);
  pdbRecordType->papFldDes[64]->offset=(short)((char *)&prec->egu - (char *)prec);
  pdbRecordType->papFldDes[65]->size=sizeof(prec->vers);
  pdbRecordType->papFldDes[65]->offset=(short)((char *)&prec->vers - (char *)prec);
  pdbRecordType->papFldDes[66]->size=sizeof(prec->pp);
  pdbRecordType->papFldDes[66]->offset=(short)((char *)&prec->pp - (char *)prec);
  pdbRecordType->papFldDes[67]->size=sizeof(prec->lthp);
  pdbRecordType->papFldDes[67]->offset=(short)((char *)&prec->lthp - (char *)prec);
  pdbRecordType->papFldDes[68]->size=sizeof(prec->nmdv);
  pdbRecordType->papFldDes[68]->offset=(short)((char *)&prec->nmdv - (char *)prec);
  pdbRecordType->papFldDes[69]->size=sizeof(prec->mdel);
  pdbRecordType->papFldDes[69]->offset=(short)((char *)&prec->mdel - (char *)prec);
  pdbRecordType->papFldDes[70]->size=sizeof(prec->adel);
  pdbRecordType->papFldDes[70]->offset=(short)((char *)&prec->adel - (char *)prec);
  pdbRecordType->papFldDes[71]->size=sizeof(prec->mmap);
  pdbRecordType->papFldDes[71]->offset=(short)((char *)&prec->mmap - (char *)prec);
  pdbRecordType->papFldDes[72]->size=sizeof(prec->lmap);
  pdbRecordType->papFldDes[72]->offset=(short)((char *)&prec->lmap - (char *)prec);
  pdbRecordType->papFldDes[73]->size=sizeof(prec->hihi);
  pdbRecordType->papFldDes[73]->offset=(short)((char *)&prec->hihi - (char *)prec);
  pdbRecordType->papFldDes[74]->size=sizeof(prec->lolo);
  pdbRecordType->papFldDes[74]->offset=(short)((char *)&prec->lolo - (char *)prec);
  pdbRecordType->papFldDes[75]->size=sizeof(prec->high);
  pdbRecordType->papFldDes[75]->offset=(short)((char *)&prec->high - (char *)prec);
  pdbRecordType->papFldDes[76]->size=sizeof(prec->low);
  pdbRecordType->papFldDes[76]->offset=(short)((char *)&prec->low - (char *)prec);
  pdbRecordType->papFldDes[77]->size=sizeof(prec->hhsv);
  pdbRecordType->papFldDes[77]->offset=(short)((char *)&prec->hhsv - (char *)prec);
  pdbRecordType->papFldDes[78]->size=sizeof(prec->llsv);
  pdbRecordType->papFldDes[78]->offset=(short)((char *)&prec->llsv - (char *)prec);
  pdbRecordType->papFldDes[79]->size=sizeof(prec->hsv);
  pdbRecordType->papFldDes[79]->offset=(short)((char *)&prec->hsv - (char *)prec);
  pdbRecordType->papFldDes[80]->size=sizeof(prec->lsv);
  pdbRecordType->papFldDes[80]->offset=(short)((char *)&prec->lsv - (char *)prec);
  pdbRecordType->papFldDes[81]->size=sizeof(prec->hlsv);
  pdbRecordType->papFldDes[81]->offset=(short)((char *)&prec->hlsv - (char *)prec);
  pdbRecordType->papFldDes[82]->size=sizeof(prec->a);
  pdbRecordType->papFldDes[82]->offset=(short)((char *)&prec->a - (char *)prec);
  pdbRecordType->papFldDes[83]->size=sizeof(prec->b);
  pdbRecordType->papFldDes[83]->offset=(short)((char *)&prec->b - (char *)prec);
  pdbRecordType->papFldDes[84]->size=sizeof(prec->c);
  pdbRecordType->papFldDes[84]->offset=(short)((char *)&prec->c - (char *)prec);
  pdbRecordType->papFldDes[85]->size=sizeof(prec->d);
  pdbRecordType->papFldDes[85]->offset=(short)((char *)&prec->d - (char *)prec);
  pdbRecordType->papFldDes[86]->size=sizeof(prec->e);
  pdbRecordType->papFldDes[86]->offset=(short)((char *)&prec->e - (char *)prec);
  pdbRecordType->papFldDes[87]->size=sizeof(prec->vala);
  pdbRecordType->papFldDes[87]->offset=(short)((char *)&prec->vala - (char *)prec);
  pdbRecordType->papFldDes[88]->size=sizeof(prec->valb);
  pdbRecordType->papFldDes[88]->offset=(short)((char *)&prec->valb - (char *)prec);
  pdbRecordType->papFldDes[89]->size=sizeof(prec->valc);
  pdbRecordType->papFldDes[89]->offset=(short)((char *)&prec->valc - (char *)prec);
  pdbRecordType->papFldDes[90]->size=sizeof(prec->vald);
  pdbRecordType->papFldDes[90]->offset=(short)((char *)&prec->vald - (char *)prec);
  pdbRecordType->papFldDes[91]->size=sizeof(prec->vale);
  pdbRecordType->papFldDes[91]->offset=(short)((char *)&prec->vale - (char *)prec);
  pdbRecordType->papFldDes[92]->size=sizeof(prec->fta);
  pdbRecordType->papFldDes[92]->offset=(short)((char *)&prec->fta - (char *)prec);
  pdbRecordType->papFldDes[93]->size=sizeof(prec->ftb);
  pdbRecordType->papFldDes[93]->offset=(short)((char *)&prec->ftb - (char *)prec);
  pdbRecordType->papFldDes[94]->size=sizeof(prec->ftc);
  pdbRecordType->papFldDes[94]->offset=(short)((char *)&prec->ftc - (char *)prec);
  pdbRecordType->papFldDes[95]->size=sizeof(prec->ftd);
  pdbRecordType->papFldDes[95]->offset=(short)((char *)&prec->ftd - (char *)prec);
  pdbRecordType->papFldDes[96]->size=sizeof(prec->fte);
  pdbRecordType->papFldDes[96]->offset=(short)((char *)&prec->fte - (char *)prec);
  pdbRecordType->papFldDes[97]->size=sizeof(prec->aahl);
  pdbRecordType->papFldDes[97]->offset=(short)((char *)&prec->aahl - (char *)prec);
  pdbRecordType->papFldDes[98]->size=sizeof(prec->aall);
  pdbRecordType->papFldDes[98]->offset=(short)((char *)&prec->aall - (char *)prec);
  pdbRecordType->papFldDes[99]->size=sizeof(prec->abhl);
  pdbRecordType->papFldDes[99]->offset=(short)((char *)&prec->abhl - (char *)prec);
  pdbRecordType->papFldDes[100]->size=sizeof(prec->abll);
  pdbRecordType->papFldDes[100]->offset=(short)((char *)&prec->abll - (char *)prec);
  pdbRecordType->papFldDes[101]->size=sizeof(prec->achl);
  pdbRecordType->papFldDes[101]->offset=(short)((char *)&prec->achl - (char *)prec);
  pdbRecordType->papFldDes[102]->size=sizeof(prec->acll);
  pdbRecordType->papFldDes[102]->offset=(short)((char *)&prec->acll - (char *)prec);
  pdbRecordType->papFldDes[103]->size=sizeof(prec->adhl);
  pdbRecordType->papFldDes[103]->offset=(short)((char *)&prec->adhl - (char *)prec);
  pdbRecordType->papFldDes[104]->size=sizeof(prec->adll);
  pdbRecordType->papFldDes[104]->offset=(short)((char *)&prec->adll - (char *)prec);
  pdbRecordType->papFldDes[105]->size=sizeof(prec->aehl);
  pdbRecordType->papFldDes[105]->offset=(short)((char *)&prec->aehl - (char *)prec);
  pdbRecordType->papFldDes[106]->size=sizeof(prec->aell);
  pdbRecordType->papFldDes[106]->offset=(short)((char *)&prec->aell - (char *)prec);
  pdbRecordType->papFldDes[107]->size=sizeof(prec->ack1);
  pdbRecordType->papFldDes[107]->offset=(short)((char *)&prec->ack1 - (char *)prec);
  pdbRecordType->papFldDes[108]->size=sizeof(prec->ack2);
  pdbRecordType->papFldDes[108]->offset=(short)((char *)&prec->ack2 - (char *)prec);
  pdbRecordType->papFldDes[109]->size=sizeof(prec->ack3);
  pdbRecordType->papFldDes[109]->offset=(short)((char *)&prec->ack3 - (char *)prec);
  pdbRecordType->papFldDes[110]->size=sizeof(prec->ack4);
  pdbRecordType->papFldDes[110]->offset=(short)((char *)&prec->ack4 - (char *)prec);
  pdbRecordType->papFldDes[111]->size=sizeof(prec->ack5);
  pdbRecordType->papFldDes[111]->offset=(short)((char *)&prec->ack5 - (char *)prec);
  pdbRecordType->papFldDes[112]->size=sizeof(prec->bus1);
  pdbRecordType->papFldDes[112]->offset=(short)((char *)&prec->bus1 - (char *)prec);
  pdbRecordType->papFldDes[113]->size=sizeof(prec->bus2);
  pdbRecordType->papFldDes[113]->offset=(short)((char *)&prec->bus2 - (char *)prec);
  pdbRecordType->papFldDes[114]->size=sizeof(prec->bus3);
  pdbRecordType->papFldDes[114]->offset=(short)((char *)&prec->bus3 - (char *)prec);
  pdbRecordType->papFldDes[115]->size=sizeof(prec->bus4);
  pdbRecordType->papFldDes[115]->offset=(short)((char *)&prec->bus4 - (char *)prec);
  pdbRecordType->papFldDes[116]->size=sizeof(prec->bus5);
  pdbRecordType->papFldDes[116]->offset=(short)((char *)&prec->bus5 - (char *)prec);
  pdbRecordType->papFldDes[117]->size=sizeof(prec->odr1);
  pdbRecordType->papFldDes[117]->offset=(short)((char *)&prec->odr1 - (char *)prec);
  pdbRecordType->papFldDes[118]->size=sizeof(prec->odr2);
  pdbRecordType->papFldDes[118]->offset=(short)((char *)&prec->odr2 - (char *)prec);
  pdbRecordType->papFldDes[119]->size=sizeof(prec->odr3);
  pdbRecordType->papFldDes[119]->offset=(short)((char *)&prec->odr3 - (char *)prec);
  pdbRecordType->papFldDes[120]->size=sizeof(prec->odr4);
  pdbRecordType->papFldDes[120]->offset=(short)((char *)&prec->odr4 - (char *)prec);
  pdbRecordType->papFldDes[121]->size=sizeof(prec->odr5);
  pdbRecordType->papFldDes[121]->offset=(short)((char *)&prec->odr5 - (char *)prec);
  pdbRecordType->papFldDes[122]->size=sizeof(prec->mod1);
  pdbRecordType->papFldDes[122]->offset=(short)((char *)&prec->mod1 - (char *)prec);
  pdbRecordType->papFldDes[123]->size=sizeof(prec->mod2);
  pdbRecordType->papFldDes[123]->offset=(short)((char *)&prec->mod2 - (char *)prec);
  pdbRecordType->papFldDes[124]->size=sizeof(prec->mod3);
  pdbRecordType->papFldDes[124]->offset=(short)((char *)&prec->mod3 - (char *)prec);
  pdbRecordType->papFldDes[125]->size=sizeof(prec->mod4);
  pdbRecordType->papFldDes[125]->offset=(short)((char *)&prec->mod4 - (char *)prec);
  pdbRecordType->papFldDes[126]->size=sizeof(prec->mod5);
  pdbRecordType->papFldDes[126]->offset=(short)((char *)&prec->mod5 - (char *)prec);
  pdbRecordType->papFldDes[127]->size=sizeof(prec->pos1);
  pdbRecordType->papFldDes[127]->offset=(short)((char *)&prec->pos1 - (char *)prec);
  pdbRecordType->papFldDes[128]->size=sizeof(prec->pos2);
  pdbRecordType->papFldDes[128]->offset=(short)((char *)&prec->pos2 - (char *)prec);
  pdbRecordType->papFldDes[129]->size=sizeof(prec->pos3);
  pdbRecordType->papFldDes[129]->offset=(short)((char *)&prec->pos3 - (char *)prec);
  pdbRecordType->papFldDes[130]->size=sizeof(prec->pos4);
  pdbRecordType->papFldDes[130]->offset=(short)((char *)&prec->pos4 - (char *)prec);
  pdbRecordType->papFldDes[131]->size=sizeof(prec->pos5);
  pdbRecordType->papFldDes[131]->offset=(short)((char *)&prec->pos5 - (char *)prec);
  pdbRecordType->papFldDes[132]->size=sizeof(prec->vel1);
  pdbRecordType->papFldDes[132]->offset=(short)((char *)&prec->vel1 - (char *)prec);
  pdbRecordType->papFldDes[133]->size=sizeof(prec->vel2);
  pdbRecordType->papFldDes[133]->offset=(short)((char *)&prec->vel2 - (char *)prec);
  pdbRecordType->papFldDes[134]->size=sizeof(prec->vel3);
  pdbRecordType->papFldDes[134]->offset=(short)((char *)&prec->vel3 - (char *)prec);
  pdbRecordType->papFldDes[135]->size=sizeof(prec->vel4);
  pdbRecordType->papFldDes[135]->offset=(short)((char *)&prec->vel4 - (char *)prec);
  pdbRecordType->papFldDes[136]->size=sizeof(prec->vel5);
  pdbRecordType->papFldDes[136]->offset=(short)((char *)&prec->vel5 - (char *)prec);
  pdbRecordType->papFldDes[137]->size=sizeof(prec->sij);
  pdbRecordType->papFldDes[137]->offset=(short)((char *)&prec->sij - (char *)prec);
  pdbRecordType->papFldDes[138]->size=sizeof(prec->sik);
  pdbRecordType->papFldDes[138]->offset=(short)((char *)&prec->sik - (char *)prec);
  pdbRecordType->papFldDes[139]->size=sizeof(prec->sil);
  pdbRecordType->papFldDes[139]->offset=(short)((char *)&prec->sil - (char *)prec);
  pdbRecordType->papFldDes[140]->size=sizeof(prec->sim);
  pdbRecordType->papFldDes[140]->offset=(short)((char *)&prec->sim - (char *)prec);
  pdbRecordType->papFldDes[141]->size=sizeof(prec->sin);
  pdbRecordType->papFldDes[141]->offset=(short)((char *)&prec->sin - (char *)prec);
  pdbRecordType->papFldDes[142]->size=sizeof(prec->ftsj);
  pdbRecordType->papFldDes[142]->offset=(short)((char *)&prec->ftsj - (char *)prec);
  pdbRecordType->papFldDes[143]->size=sizeof(prec->ftsk);
  pdbRecordType->papFldDes[143]->offset=(short)((char *)&prec->ftsk - (char *)prec);
  pdbRecordType->papFldDes[144]->size=sizeof(prec->ftsl);
  pdbRecordType->papFldDes[144]->offset=(short)((char *)&prec->ftsl - (char *)prec);
  pdbRecordType->papFldDes[145]->size=sizeof(prec->ftsm);
  pdbRecordType->papFldDes[145]->offset=(short)((char *)&prec->ftsm - (char *)prec);
  pdbRecordType->papFldDes[146]->size=sizeof(prec->ftsn);
  pdbRecordType->papFldDes[146]->offset=(short)((char *)&prec->ftsn - (char *)prec);
  pdbRecordType->papFldDes[147]->size=sizeof(prec->sor);
  pdbRecordType->papFldDes[147]->offset=(short)((char *)&prec->sor - (char *)prec);
  pdbRecordType->papFldDes[148]->size=sizeof(prec->sos);
  pdbRecordType->papFldDes[148]->offset=(short)((char *)&prec->sos - (char *)prec);
  pdbRecordType->papFldDes[149]->size=sizeof(prec->sot);
  pdbRecordType->papFldDes[149]->offset=(short)((char *)&prec->sot - (char *)prec);
  pdbRecordType->papFldDes[150]->size=sizeof(prec->sou);
  pdbRecordType->papFldDes[150]->offset=(short)((char *)&prec->sou - (char *)prec);
  pdbRecordType->papFldDes[151]->size=sizeof(prec->sov);
  pdbRecordType->papFldDes[151]->offset=(short)((char *)&prec->sov - (char *)prec);
    pdbRecordType->rec_size = sizeof(*prec);
    return(0);
}
#endif /*GEN_SIZE_OFFSET*/

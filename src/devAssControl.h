/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) 1998.				(c) 1998
 * National Research Council		Conseil national de recherches
 * Ottawa, Canada, K1A 0R6 		Ottawa, Canada, K1A 0R6
 * All rights reserved			Tous droits reserves
 * 					
 * NRC disclaims any warranties,	Le CNRC denie toute garantie
 * expressed, implied, or statu-	enoncee, implicite ou legale,
 * tory, of any kind with respect	de quelque nature que se soit,
 * to the software, including		concernant le logiciel, y com-
 * without limitation any war-		pris sans restriction toute
 * ranty of merchantability or		garantie de valeur marchande
 * fitness for a particular pur-	ou de pertinence pour un usage
 * pose.  NRC shall not be liable	particulier.  Le CNRC ne
 * in any event for any damages,	pourra en aucun cas etre tenu
 * whether direct or indirect,		responsable de tout dommage,
 * special or general, consequen-	direct ou indirect, particul-
 * tial or incidental, arising		ier ou general, accessoire ou
 * from the use of the software.	fortuit, resultant de l'utili-
 * 					sation du logiciel.
 *
 ************************************************************************
 *
 * FILENAME
 * devAssControl.h
 *
 * PURPOSE:
 * assemblyControl device support public information
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.4  2000/05/15 16:50:49  gmos
 * Revised history
 *
 * Revision UK1.3  2000/04/19 10:28:37  cjt
 * Restored devAssControl.h !
 *
 * Revision UK1.2  2000/04/10 09:19:58  gmos
 * gmtools and gmoscc merged
 *
 * Revision UK1.3?  2000/04/03 15:43:09  cjt
 * Version 5 merged into old version 4 files. *NEW.* files removed. Correction to half-step mode support in post focal plane device records.
 *
 * Revision UK1.1  2000/03/08 16:28:14  gmos
 * Missed from DAO release
 *
 * Revision UK1.1.1.1  2000/02/29 11:41:58  gmos
 * V1.00 release from HIA
 *
 * Revision 1.2  2000/02/10 21:35:58  angelic
 * added processFault to DSET
 *
 * Revision 1.1  1999/04/28 21:05:00  dunn
 * Initial revision
 *
 *INDENT-ON*
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/

#ifndef DEV_ASS_CONTROL_INC
#define	DEV_ASS_CONTROL_INC

typedef struct {
    long            number;
    DEVSUPFUN       devReport;
    DEVSUPFUN       devInit;
    DEVSUPFUN       initDeviceSupport;
    DEVSUPFUN       devGetIoIntInfo;
    DEVSUPFUN       checkAttributes;
    DEVSUPFUN       stopDirective;
    DEVSUPFUN       initMode;
    DEVSUPFUN       moveMode;
    DEVSUPFUN       trackMode;
    DEVSUPFUN       indexMode;
    DEVSUPFUN       testMode;
    DEVSUPFUN       ackReceived;
    DEVSUPFUN       updateMode;
    DEVSUPFUN       processFault;
} ASSEMBLY_CONTROL_DSET;

#endif


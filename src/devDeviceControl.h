
/*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) 1998.                        (c) 1998
 * National Research Council        Conseil national de recherches
 * Ottawa, Canada, K1A 0R6          Ottawa, Canada, K1A 0R6
 * All rights reserved              Tous droits reserves
 *                     
 * NRC disclaims any warranties,    Le CNRC denie toute garantie
 * expressed, implied, or statu-    enoncee, implicite ou legale,
 * tory, of any kind with respect   de quelque nature que se soit,
 * to the software, including       concernant le logiciel, y com-
 * without limitation any war-      pris sans restriction toute
 * ranty of merchantability or      garantie de valeur marchande
 * fitness for a particular pur-    ou de pertinence pour un usage
 * pose.  NRC shall not be liable   particulier.  Le CNRC ne
 * in any event for any damages,    pourra en aucun cas etre tenu
 * whether direct or indirect,      responsable de tout dommage,
 * special or general, consequen-   direct ou indirect, particul-
 * tial or incidental, arising      ier ou general, accessoire ou
 * from the use of the software.    fortuit, resultant de l'utili-
 *                                  sation du logiciel.
 *
 ************************************************************************
 *
 * FILENAME
 * devDeviceControl.h
 *
 * PURPOSE:
 * Publish deviceControl record OMS8/44 device support public information
 * for devDeviceControl module.
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.3  2000/07/06 17:25:59  gmos
 * Williams new code, tested and debugged
 *
 * Revision 1.2  2000/04/14 10:00:35  gmos
 * Merged pre and post FP software
 *
 * Revision 1.1.1.1  2000/02/29 11:42:26  gmos
 * V1.00 release from HIA
 *
 * Revision 1.4  1999/08/13 19:24:25  angelic
 * remove setLimts from DSET
 *
 * Revision 1.3  1999/04/28 20:27:58  dunn
 * Bug fixes.
 *
 * Revision 1.2  1999/02/26 20:44:08  rambold
 * beta version
 *
 * Revision 1.1  1998/11/20 03:19:01  rambold
 * Initial revision
 *
 *INDENT-ON*
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/

#ifndef DEV_DEVICE_CONTROL_INC
#define DEV_DEVICE_CONTROL_INC


/* 
 * Define the epics deviceControl record device support access structure.
 */

typedef struct {
    long            number;
    DEVSUPFUN       devReport;
    DEVSUPFUN       init;
    DEVSUPFUN       initDeviceSupport;
    DEVSUPFUN       getIointInfo;
    DEVSUPFUN       configureDrive;
    DEVSUPFUN       controlPower;
    DEVSUPFUN       controlMotion;
    DEVSUPFUN       setDelay;
    DEVSUPFUN       setPosition;
} DEVICE_CONTROL_DSET;


/* 
 * Define the epics binary output device support access structure.
 */

typedef struct {
    long            number;
    DEVSUPFUN       report;
    DEVSUPFUN       init;
    DEVSUPFUN       initRecord;
    DEVSUPFUN       getIointInfo;
    DEVSUPFUN       writeBo;
} DEVICE_BO_OMS_DAO_DSET;

#endif


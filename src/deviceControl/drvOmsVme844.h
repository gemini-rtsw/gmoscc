
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
 * drvOmsVme.h
 *
 * PURPOSE:
 * Publish deviceControl record OMS-VME8/44 device driver public information
 * for the devOmsVme module.
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.1  2001/11/28 20:15:07  mbec
 * *** empty log message ***
 *
 * Revision 1.1  2001/08/10 14:01:48  ptaylor
 * Restructured src directory with 4 sub-directories, including pv and lut which were previously in pv as well as deviceControl and gcal previously in src
 *
 * Revision 1.2  2001/07/12 19:11:00  gemvx
 * Re-enabled power check
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.5  2000/10/04 00:32:08  gmos
 * Added max axes definition (also used in devDeviceControl.c now).
 *
 * Revision 1.4  2000/07/12 16:57:45  gmos
 * Added DRV_OMS_VME_S_PARAM_ERR error code.
 *
 * Revision 1.3  2000/07/06 17:25:35  gmos
 * Williams new code, tested and debugged
 *
 * Revision 1.2  2000/04/14 10:00:38  gmos
 * Merged pre and post FP software
 *
 * Revision 1.1.1.1  2000/02/29 11:42:26  gmos
 * V1.00 release from HIA
 *
 * Revision 1.5  1999/10/30 00:17:20  rambold
 * added error define for card overtravel interrupt
 *
 * Revision 1.4  1999/08/13 19:22:32  angelic
 * added function prototype for drvOmsVmeGetCardType()
 *
 * Revision 1.3  1999/07/28 19:51:12  rambold
 * added drvOmsVmeGetErrorMessage prototype
 *
 * Revision 1.2  1999/04/28 20:27:58  dunn
 * Bug fixes.
 *
 * Revision 1.1  1999/02/26 20:44:08  rambold
 * Initial revision
 *
 *INDENT-ON*
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/

#ifndef DRV_OMS_VME_DRV_INC
#define DRV_OMS_VME_DRV_INC

#define DRV_OMS_VME_BASE            0xFC00      /* base address of the card */
#define DRV_OMS_VME_VECTOR          180         /* interrupt vector         */
#define DRV_OMS_VME_INTERRUPT       5           /* interrupt line           */
 
#define DRV_OMS_VME_MAX_MSG_LEN     64          /* maximum message length   */ 

#define DRV_OMS_VME_S_CMD_REJECT    -1          /* OMS card rejected cmd    */
#define DRV_OMS_VME_S_NO_REPLY      -2          /* OMS card did not reply   */
#define DRV_OMS_VME_S_SYS_ERROR     -3          /* VxWorks system error     */
#define DRV_OMS_VME_S_CFG_ERROR     -4          /* non-existant axis request*/
#define DRV_OMS_VME_S_BUFFER_FULL   -5          /* transmit ring buffer full*/
#define DRV_OMS_VME_S_PARSE_ERROR   -6          /* error parsing return str */
#define DRV_OMS_VME_S_OVERTRAVEL    -7          /* limit switch detected    */
#define DRV_OMS_VME_S_PARAM_ERR     -8          /* motor configuration error*/ 

#define DRV_OMS_VME_8_MAX_AXES       8          /* max axes on VME-8 card   */
#define DRV_OMS_VME_44_MAX_AXES      4          /* max axes on VME-44 card  */

/*
 * Public access functions
 */
 
long drvOmsVmeMotorPosition (int, int, long *, long *);
long drvOmsVmeMotorState (int, int, int *, int *, int *, int *);
int  drvOmsVmeGetCardType (int);
void drvOmsVmeGetErrorMessage (char *);
long drvOmsVmeInit();
long drvOmsVmeReadCard (int, char *, int);
long drvOmsVmeWriteCard (int, char *);  
long drvOmsVmeWriteMotor (int, int, char *);  
long locationProbe(epicsAddressType, char *);

   
#endif


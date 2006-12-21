 /*
 ************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) <1999>                       (c) <1999>
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
 * gmMskMisc.c
 *
 * PURPOSE:
 * EPICS Miscellaneous Mask Assembly Support code for the genSubRecord.
 *
 * FUNCTION NAME(S)
 * mkExtLvdtZones     Identify position of extractor by zone.
 * mkExtSwitchWord    Combine switch status into word.
 *
 *INDENT-OFF*
 * $Log$
 * Revision 1.3  2005/08/23 21:40:56  gemvx
 * *** empty log message ***
 *
 * Revision 1.2  2005/03/09 19:46:14  gemvx
 * *** empty log message ***
 *
 * Revision 1.1  2001/11/28 20:08:48  mbec
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/04/13 01:37:34  smb
 * Initial creation of the Gemini GMOS repository
 *
 * Revision 1.2  2000/05/30 16:29:09  gmos
 * New version commented by William Rambold. Master enable bug fixed.
 *
 * Revision 1.1  2000/04/10 09:20:01  gmos
 * gmtools and gmoscc merged
 *
 * Revision 1.1.1.1  2000/02/29 11:42:26  gmos
 * V1.00 release from HIA
 *
 * Revision 1.1  2000/01/28 20:33:53  dunn
 * Initial revision
 *
 *INDENT-ON*
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/

/*
 *  Includes
 */

#include <stdioLib.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vxWorks.h>
#include <logLib.h>
#include <tickLib.h>  
#include <sysLib.h>   
#include <wdLib.h>

#include <dbDefs.h>
#include <recSup.h>
#include <genSubRecord.h>


/*
 *  Local Defines
 */

/*
 * LVDT position decoding gensub record input field access defintions
 */

#define INPUT_LVDTLO  *(double *)pgs->a  /* Extractor at LVDT lo limit.     */
#define INPUT_GRPLO   *(double *)pgs->b  /* Extractor at grip lo limit.     */
#define INPUT_GRPHI   *(double *)pgs->c  /* Extractor at grip hi limit.     */
#define INPUT_RELLO   *(double *)pgs->d  /* Extractor at release lo limit.  */
#define INPUT_RELHI   *(double *)pgs->e  /* Extractor at release hi limit.  */
#define INPUT_LVDT    *(double *)pgs->f  /* Current value of LVDT sensor    */


/*
 * LVDT position decoding gensub record output field access defintions
 */

#define OUTPUT_ZONES  *(long *)pgs->vala /* Extractor position zone word    */
#define OUTPUT_ZONE_0 *(long *)pgs->valb /* Extractor position zone 0       */
#define OUTPUT_ZONE_1 *(long *)pgs->valc /* Extractor position zone 1       */
#define OUTPUT_ZONE_2 *(long *)pgs->vald /* Extractor position zone 2       */
#define OUTPUT_ZONE_3 *(long *)pgs->vale /* Extractor position zone 3       */
#define OUTPUT_ZONE_4 *(long *)pgs->valf /* Extractor position zone 4       */
#define OUTPUT_ZONE_5 *(long *)pgs->valg /* Extractor position zone 5       */


/*
 * LVDT position decoding gensub record internal definitions
 */

#define OUT_ZONE  0                      /* Outside configurable zone       */
#define IN_ZONE   1                      /* Within configurable zone        */


/*
 * Microswitch decoding gensub record input field access mnemonics
 */

#define IN_RELNOTLCK  *(long *)pgs->a    /* Release not in locked position. */
#define IN_RELNOTREL  *(long *)pgs->b    /* Release not in rel. position.   */
#define IN_GRPNOTREL  *(long *)pgs->c    /* Gripper not in rel. position.   */
#define IN_GRPNOTGRP  *(long *)pgs->d    /* Gripper not in grip position.   */
#define IN_NOTALIGNED *(long *)pgs->e    /* Slot not aligned with extractor */
#define IN_NOTOCCUPIED *(long *)pgs->f   /* Slot has no Mask/IFU            */
#define IN_NOTINSTALLED *(long *)pgs->g  /* Cassette not installed          */
#define IN_MASKNOTIN  *(long *)pgs->h    /* Mask not in focal plane         */
#define IN_IFUNOTIN   *(long *)pgs->i    /* IFU not in focal plane          */
#define IN_NOTINFP    *(long *)pgs->j    /* Mask or IFU not in focal plane  */



/*
 * Microswitch decoding gensub record output field access mnemonics
 */

#define OUT_SWITCHES  *(long *)pgs->vala /* Mask switch status word         */
#define REL_STATE     *(long *)pgs->valb /* released, locked or in between  */
#define GRP_STATE     *(long *)pgs->valc /* gripped, released or between    */
#define FP_STATE      *(long *)pgs->vald /* Mask, IFU or neither in FP      */



/*
 * Microswitch decoding gensub internal definitions
 */

#define REL_BETWEEN   0                  /* Release not locked or released  */
#define REL_LOCKED   -1                  /* Release is locked               */
#define REL_RELEASED  1                  /* Release is released             */
#define GRP_BETWEEN   0                  /* Gripper not gripped or released */
#define GRP_GRIPPED  -1                  /* Gripper is gripped              */
#define GRP_RELEASED  1                  /* Gripper is released             */
#define FP_NONE       0                  /* Neither mask nor IFU in F.P.    */
#define FP_MASK       1                  /* Mask in Focal Plane             */
#define FP_IFU       -1                  /* IFU in Focal Plane              */


/*
 *  Public function Prototypes
 */

long mkExtLvdtZones (struct genSubRecord *);
long mkSwitchWord (struct genSubRecord *);
long mkIfuSelection (struct genSubRecord *);


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkExtLvdtZones
 *
 * INVOCATION:
 * Called when LvdtZones genSub record processes
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pgs (struct genSubRecord *)  Pointer to calling gensub record structure
 *
 *
 * FUNCTION VALUE:
 * ([long] 0 ) Indicates completed successfully.
 *
 * PURPOSE:
 * Process GenSub record
 *
 * DESCRIPTION:
 * Evaluates current position of extractractor stage from absolute encoder
 * (LVDT into input F) based on configurable limits (inputs A thru E).
 * (>) A     Extractor at LVDT lo limit.
 * (>) B     Extractor at grip lo limit.
 * (>) C     Extractor at grip hi limit.
 * (>) D     Extractor at release lo limit.
 * (>) E     Extractor at release hi limit.
 * (>) F     Current value of LVDT sensor.
 * (<) VALA  Extractor position zone word
 * (<) VALB  Extractor position zone 0
 * (<) VALC  Extractor position zone 1
 * (<) VALD  Extractor position zone 2
 * (<) VALE  Extractor position zone 3
 * (<) VALF  Extractor position zone 4
 * (<) VALG  Extractor position zone 5
 * The current zone status for five defined extractor zones is written to 
 * outputs VALB thru VALG as a binary value (IN_ZONE or OUT_ZONE).  The
 * five values are also encoded (by left shifting each bit) into a zone 
 * word and written to VALA.
 * 
 *  (F>D&&F<E)+(F>B&&F<C)*2+(F>C&&F<D)*4+(F>A)*8+(F>D)*16+(F<B)*32
 *
 * Extractor Stage Zones by number:
 *
 * SoftLimit   Release  Grip   Lvdt   ID  Clear  Mask  Ifu  SoftLimit
 *     |         | |     | |     |     |    |      |    |       |
 *     |         |0|<-2->|1|<-5->|                               
 *     |<----4---->|             |                               
 *     |<-----------3----------->|                               
 *     |                         |                               
 *
 *
 * In the mask assembly record zones are also sometimes referred to by letter:
 *
 * SoftLimit   Release  Grip   Lvdt  ID  Clear  Mask  Ifu  SoftLimit
 *     |         | |     | |     |     |    |      |    |       |
 *     |         |         |                                    |
 *     |<---A--->|<---B--->|<----------------C----------------->|
 *     |                                                        |
 *
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * 
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */


long mkExtLvdtZones
(
    struct genSubRecord *pgs   /* (in) pointer to calling record structure */
)
{
    long status = 0;

    /*
     *  Evaluate Zone 0, Extractor in a position where it is safe
     *  to raise or lower the locking pin.
     */

    if ((INPUT_LVDT > INPUT_RELLO) && (INPUT_LVDT < INPUT_RELHI))
    {
        OUTPUT_ZONE_0 = IN_ZONE;
    }
    else
    {
        OUTPUT_ZONE_0 = OUT_ZONE;
    }


    /*
     *  Evaluate Zone 1, Extractor in a position where it is safe
     *  to rotate the gripper mechanism.
     */

    if ((INPUT_LVDT > INPUT_GRPLO) && (INPUT_LVDT < INPUT_GRPHI))
    {
        OUTPUT_ZONE_1 = IN_ZONE;
    }
    else
    {
        OUTPUT_ZONE_1 = OUT_ZONE;
    }


    /*
     *  Evaluate Zone 2, Extractor between the above release & grip ranges.
     */

    if ((INPUT_LVDT > INPUT_GRPHI) && (INPUT_LVDT < INPUT_RELLO))
    {
    OUTPUT_ZONE_2 = IN_ZONE;
    }
    else
    {
        OUTPUT_ZONE_2 = OUT_ZONE;
    }


    /*
     *  Evaluate Zone 3, Extractor within the sensing range of the LVDT.
     */

    if (INPUT_LVDT > INPUT_LVDTLO)
    {
        OUTPUT_ZONE_3 = IN_ZONE;
    }
    else
    {
        OUTPUT_ZONE_3 = OUT_ZONE;
    }


    /*
     *  Evaluate Zone 4, Extractor in a position where it is safe to 
     *  lock/unlock position or is closer to the cassette.
     */

    if (INPUT_LVDT > INPUT_RELLO)
    {
        OUTPUT_ZONE_4 = IN_ZONE;
    }
    else
    {
        OUTPUT_ZONE_4 = OUT_ZONE;
    }


    /*
     *  Evaluate Zone 5, Extractor within the sensing range of the
     *  LVDT but has not made it as far as the safe gripping area.
     */

    if (INPUT_LVDT < INPUT_GRPLO)
    {
        OUTPUT_ZONE_5 = IN_ZONE;
    }
    else
    {
        OUTPUT_ZONE_5 = OUT_ZONE;
    }


    /*
     *  Encode zones 0 thru 5 into zone word by left shifting each
     *  subsequent bit to create the following:
     *
     *  Bit 0 - Extractor in zone 0 (0=NO, 1=YES)
     *  Bit 1 - Extractor in zone 1 (0=NO, 1=YES) 
     *  Bit 2 - Extractor in zone 2 (0=NO, 1=YES) 
     *  Bit 3 - Extractor in zone 3 (0=NO, 1=YES) 
     *  Bit 4 - Extractor in zone 4 (0=NO, 1=YES) 
     *  Bit 4 - Extractor in zone 5 (0=NO, 1=YES) 
     */

    OUTPUT_ZONES = (OUTPUT_ZONE_0 | OUTPUT_ZONE_1<<1 | OUTPUT_ZONE_2<<2 | \
        OUTPUT_ZONE_3<<3 | OUTPUT_ZONE_4<<4 | OUTPUT_ZONE_5<<5);

    return( status );
}


/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkIfuSelection
 *
 * INVOCATION:
 * Called when ifuSelection genSub record processes
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pgs (struct genSubRecord *)  Pointer to calling genSub record structure
 *
 *
 * FUNCTION VALUE:
 * ([long] 0 ) Indicates completed successfully.
 *
 * PURPOSE:
 * Process GenSub record
 *
 * DESCRIPTION:
 * Replaces IFU barcode with a user selected barcode
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * - other function name.
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

long mkIfuSelection
(
    struct genSubRecord *pgs /* (in) pointer to calling recored structure */
)
{
   char ifuSelection[9] = "";
   char barcode[9] = "";

/* Read in user IFU selection */
   strncpy (ifuSelection, (char *)pgs->b, 8);
   ifuSelection[8] = '\0';
   printf("ifuSelection = %s\n",ifuSelection);
/* Read in barcode */
   strncpy (barcode, (char *)pgs->a, 8);
   barcode[8] = '\0';
   printf("barcode read = %s\n", (char *)barcode);
      
/* Check to see if IFU  barcode is read */
   /* if (strncmp (barcode,"10000002", 8) == 0) */
   if (strncmp (barcode, (char *) pgs->c, 8) == 0)
      /* ifu barcode read then output the user selected barcode */
      {
       strncpy((char *)pgs->vala,ifuSelection,8); 
       printf("barcode read is IFU, output is user selection\n");
      }
   else
      /* the barcode does not correspond to the ifu */
      /* copy the original barcode to the output */
      {
       strncpy((char *)pgs->vala,barcode,8);
       printf("barcode read is not IFU, output is original barcode\n");
      }
return(0);
}
/*
 ************************************************************************
 *+
 * FUNCTION NAME:
 * mkSwitchWord
 *
 * INVOCATION:
 * Called when SwitchWord genSub record processes
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) pgs (struct genSubRecord *)  Pointer to calling genSub record structure
 *
 *
 * FUNCTION VALUE:
 * ([long] 0 ) Indicates completed successfully.
 *
 * PURPOSE:
 * Process GenSub record
 *
 * DESCRIPTION:
 * Encodes 10 binary status items into a word by left shifting, then writes
 * the value to VALA.  Those 10 values are:
 * > A     Release not in locked position.
 * > B     Release not in released position.
 * > C     Gripper not in released position.
 * > D     Gripper not in gripped position.
 * > E     Mask slot not aligned with extractor stage.
 * > F     Aligned slot does not contain Mask/IFU.
 * > G     Cassette not installed.
 * > H     Mask not in focal plane.
 * > I     IFU not in focal plane.
 * > J     Mask or IFU not in focal plane.
 * < VALA  Combined mask switch word.
 * < VALB  Current location of Release mechanism.
 * < VALC  Current location of gripper mechanism.
 * < VALD  Current type in focal plane.
 *
 * EXTERNAL VARIABLES:
 * None.
 *
 * PRIOR REQUIREMENTS:
 * None.
 *
 * SEE ALSO:
 * - other function name.
 *
 * DEFICIENCIES:
 * None.
 *-
 ************************************************************************
 */

long mkSwitchWord
(
    struct genSubRecord *pgs   /* (in) pointer to calling record structure */
)
{
    long status = 0;

    /*
     *  Combine raw switch states into single negative-logic word as follows:
     *
     *      Bit 0 - Release contacting released switch (0=YES, 1=NO)
     *      Bit 1 - Release contacting locked switch (0=YES, 1=NO)
     *      Bit 2 - Grip contacting released switch (0=YES, 1=NO)
     *      Bit 3 - Grip contacting gripped switch (0=YES, 1=NO)
     *      Bit 4 - Slot aligned (0=YES, 1=NO)
     *      Bit 5 - Slot occupied (0=YES, 1=NO)
     *      Bit 6 - Cassette installed (0=YES, 1=NO)
     *      Bit 7 - Mask in focal plane (0=YES, 1=NO)
     *      Bit 8 - IFU in focal plane (0=YES, 1=NO)
     *      Bit 9 - Focal plane occupied (0=YES, 1=NO)
     */

    OUT_SWITCHES = (IN_RELNOTLCK | IN_RELNOTREL<<1 | IN_GRPNOTREL<<2 | \
            IN_GRPNOTGRP<<3 | IN_NOTALIGNED<<4 | IN_NOTOCCUPIED<<5 | \
        IN_NOTINSTALLED<<6 | IN_MASKNOTIN<<7 | IN_IFUNOTIN<<8 | \
                IN_NOTINFP<<9);

    /*
     *  Determine the current location of release mechanism
     */

    if (IN_RELNOTLCK && !IN_RELNOTREL)
    {
        REL_STATE = REL_RELEASED;
    }

    else if (!IN_RELNOTLCK && IN_RELNOTREL)
    {
        REL_STATE = REL_LOCKED;
    }

    else
    {
        REL_STATE = REL_BETWEEN;
    }


    /*
     *  Determine the current location of gripper mechanism
     */

    if (IN_GRPNOTGRP && !IN_GRPNOTREL)
    {
        GRP_STATE = GRP_RELEASED;
    }

    else if (!IN_GRPNOTGRP && IN_GRPNOTREL)
    {
        GRP_STATE = GRP_GRIPPED;
    }

    else
    {
        GRP_STATE = GRP_BETWEEN;
    }


    /*
     *  Determine the current contents of focal plane
     */

    if (!IN_NOTINFP && !IN_MASKNOTIN && IN_IFUNOTIN)
    {
        FP_STATE = FP_MASK;
    }

    else if (!IN_NOTINFP && IN_MASKNOTIN && !IN_IFUNOTIN)
    {
        FP_STATE = FP_IFU;
    }

    else
    {
        FP_STATE = FP_NONE;
    }

    return( status );
}





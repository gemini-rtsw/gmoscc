/************************************************************************
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 *
 * (c) 2000                         (c) 2000
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
 * FILENAME:     testOms844.c
 *
 * PURPOSE:      Use driver level functions of the deviceControl record 
 *               without using the record. Load object file after normal 
 *               bootup, then use functions.
 *
 *               For all functions:
 *                     card = 0 to ?
 *                     axis = 0 to ?
 *
 * FUNCTION NAME(S)
 * omsRead       Read the card response (if any).
 * omsGetStatus  Gets the response from a query axis "QA".
 * omsOff        Turn power to a single axis off.
 * omsOn         Turn power to a single axis on.
 * omsPos        Get current position and encoder value.
 * omsState      Query current state of a single axis.
 * omsWrite      Write buffer to a card.
 * omsAllOff     Turn power for all axes off.
 * omsAllOn      Turn power for all axes on.
 * omsAllOnSlow  Turn power for all axes on (delay between each one).
 *
 *INDENT-OFF*
 *
 *INDENT-ON*
 *
 ****      D A O   I N S T R U M E N T A T I O N   G R O U P        *****
 ************************************************************************
*/

/*
 *  Local Defines
 */

#include <stdio.h>
#include <stdlib.h>

#include <taskLib.h>

#include <devLib.h>

#include <drvOmsVme.h>


/*
 *  Local Types
 */
#define DRV_OMS_VME_MAX_CARDS           3       /* 3 cards in GMOS */



/*
 *  Function Prototypes *** ALL LOCAL FUNCTIONS TO BE PROTOTYPED ***
 */


/*
 *  Data Structures
 */


/*
 *  Macros
 */





/*
 ************************************************************************
 *+
 * FUNCTION: omsRead
 *
 * RETURNS: long [function status]
 *
 * DESCRIPTION: 
 * Read the card response (if any).
 *
 *-
 ************************************************************************
 */

long omsRead
(
    int card
)
{
    char buffer[256];
    long status;

    status = drvOmsVmeReadCard (card, buffer, NO_WAIT);
    printf ("omsRead: %s\n", buffer);
    return status;
}


/*
 ************************************************************************
 *+
 * FUNCTION: omsGetStatus
 *
 * RETURNS: long [function status]
 *
 * DESCRIPTION: 
 * Gets the response from a query axis "QA"
 *
 *-
 ************************************************************************
 */

long omsGetStatus 
(
    int card, 
    int axis
)
{
    long status;
    status = drvOmsVmeWriteMotor (card, axis, "QA ");
    if (!status)
    {
    	taskDelay(3);
        status = omsRead(card);
    }
    return status;
}


/*
 ************************************************************************
 *+
 * FUNCTION: omsOff
 *
 * RETURNS: long [function status]
 *
 * DESCRIPTION: 
 * Turn power to a single axis off.
 *
 *-
 ************************************************************************
 */

long omsOff 
(
    int card,
    int axis
)
{
    long status;

    status = drvOmsVmeWriteMotor (card, axis, "AF");
    return status;
}


/*
 ************************************************************************
 *+
 * FUNCTION: omsOn
 *
 * RETURNS: long [function status]
 *
 * DESCRIPTION: 
 * Turn power to a single axis on.
 *
 *-
 ************************************************************************
 */

long omsOn
(
    int card,
    int axis
)
{
    long status;

    status = drvOmsVmeWriteMotor(card, axis, "AN");
    return status;
}


/*
 ************************************************************************
 *+
 * FUNCTION: omsPos
 *
 * RETURNS: long [function status]
 *
 * DESCRIPTION: 
 * Get current position and encoder value.
 *
 *-
 ************************************************************************
 */

long omsPos 
(
    int card, 
    int axis
)
{
    long position, encoder;
    long status;

    status = drvOmsVmeMotorPosition (card, axis, &position, &encoder);
    if ( status != 0 )
    {
	printf ("omsPos: FAILED, status=%ld \n", status );
    }
    else
    {
	printf ("omsPos: pos = %ld, enc = %ld\n", position, encoder);
    }
    return status;
}


/*
 ************************************************************************
 *+
 * FUNCTION: omsState
 *
 * RETURNS: long [function status]
 *
 * DESCRIPTION: 
 * Query current state of a single axis.
 *
 *-
 ************************************************************************
 */

long omsState 
(
    int card, 
    int axis
)
{
    int hlim, llim, home;
    long status;

    status = drvOmsVmeMotorState (card, axis, &llim, &hlim, &home);
    printf ("omsState: hlim = %d, llim = %d, home = %d\n", hlim, llim, home);
    return status;
}


/*
 ************************************************************************
 *+
 * FUNCTION: omsWrite
 *
 * RETURNS: long [function status]
 *
 * DESCRIPTION: 
 * Write buffer to a card.
 *
 *-
 ************************************************************************
 */

long omsWrite 
(
    int card, 
    char *buffer
)
{
    long status;

    status = drvOmsVmeWriteCard (card, buffer);
    return status;
}


/*
 ************************************************************************
 *+
 * FUNCTION: omsAllOff
 *
 * RETURNS: long [function status]
 *
 * DESCRIPTION: 
 * Turn power for all axes off.
 *
 *-
 ************************************************************************
 */

long omsAllOff
(
    void
)
{   long status, type, card, axis;

    for (card = 0; card < DRV_OMS_VME_MAX_CARDS; card++)
    {
        type = drvOmsVmeGetCardType (card);
        if (type == 8)
        {
            for (axis = 0; axis < DRV_OMS_VME_8_MAX_AXES; axis++)
            {
                printf("omsAllOff: turning Card:%d Axis:%d OFF\n",card ,axis);
                status = omsOff(card, axis);
            }
        }
        if (type == 44)
        {
            for (axis = 0; axis < DRV_OMS_VME_44_MAX_AXES; axis++)
            {
                printf("omsAllOff: turning Card:%d Axis:%d OFF\n",card ,axis);
                status = omsOff(card, axis);
            }
        }
    }

    return status;
}


/*
 ************************************************************************
 *+
 * FUNCTION: omsAllOn
 *
 * RETURNS: long [function status]
 *
 * DESCRIPTION: 
 * Turn power for all axes on.
 *
 *-
 ************************************************************************
 */

long omsAllOn
(
    void
)
{   long status, type, card, axis;

    for (card = 0; card < DRV_OMS_VME_MAX_CARDS; card++)
    {
        type = drvOmsVmeGetCardType (card);
        if (type == 8)
        {
            for (axis = 0; axis < DRV_OMS_VME_8_MAX_AXES; axis++)
            {
                printf("omsAllOn: turning Card:%d Axis:%d ON\n",card ,axis);
                status = omsOn(card, axis);
            }
        }
        if (type == 44)
        {
            for (axis = 0; axis < DRV_OMS_VME_44_MAX_AXES; axis++)
            {
                printf("omsAllOn: turning Card:%d Axis:%d ON\n",card ,axis);
                status = omsOn(card, axis);
            }
        }
    }

    return status;
}

/*
 ************************************************************************
 *+
 * FUNCTION: omsAllOnSlow
 *
 * RETURNS: long [function status]
 *
 * DESCRIPTION: 
 * Same as omsAllOn except with one second delay between each one.
 *
 *-
 ************************************************************************
 */

long omsAllOnSlow
(
    void
)
{   long status, type, card, axis;

    for (card = 0; card < DRV_OMS_VME_MAX_CARDS; card++)
    {
        type = drvOmsVmeGetCardType (card);
        if (type == 8)
        {
            for (axis = 0; axis < DRV_OMS_VME_8_MAX_AXES; axis++)
            {
                printf("omsAllOnSlow: turning Card:%d Axis:%d ON\n",card ,axis);
                status = omsOn(card, axis);
                taskDelay(60);
            }
        }
        if (type == 44)
        {
            for (axis = 0; axis < DRV_OMS_VME_44_MAX_AXES; axis++)
            {
                printf("omsAllOnSlow: turning Card:%d Axis:%d ON\n",card ,axis);
                status = omsOn(card, axis);
                taskDelay(60);
            }
        }
    }

    return status;
}


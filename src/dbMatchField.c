static char rcsid[]="$Id$";

/* *INDENT-OFF* */
/*
 * $Log$
 * Revision 1.5  1999/11/16 14:28:52  pbt
 * Update on November 16th 1999
 *
 * Revision 1.1.1.1  1998/11/08 00:21:12  epics
 * Version 1-2 of the TCS as transferred from DRAL
 *
 * Revision 1.1  1997/12/11 14:35:03  pbt
 * Routines to search for record fields with specified values
 *
 */
/* *INDENT-ON* */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <epicsTypes.h>
#include <dbStaticLib.h>


extern struct dbBase *pdbBase;   /* Global pointer to base of current EPICS database */

/*+
 *   Function name:
 *   dbMatchField
 *
 *   Purpose:
 *   Print out any database records with a specified field
 *   having the specified string value
 *
 *   Description:
 *   The routine uses static db access routines to search for
 *   all instances of the field with the specified name, and prints
 *   out the record type and name if matching the specified string.
 *
 *   Invocation:
 *   dbMatchField(field, value)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)   field  char*   Field name (upper case, no leading .)
 *      (>)   value  char*   String to match in the specified field
 *
 *   Function value:
 *   long   Number of matching records
 * 
 *   External functions:
 *   (EPICS static database access routines. See Chapter 5 of EPICS
 *   "IOC Application Developer's Guide" for further details)
 *
 *   External variables:
 *     pdbBase              EPICS static database entry point (see dbStaticLib.h)
 *
 *-
 */

long dbMatchField(char *field, char *value)
{
  DBENTRY *pdbentry;               /* Database entry */
  long nrec = 0;                   /* Record count */
  int status;
  char recname[MAX_STRING_SIZE];   /* Record name */
  char valstring[MAX_STRING_SIZE]; /* (string) Value of field */
  char recdes[MAX_STRING_SIZE];    /*  Record description */

 
  dbInitEntry(pdbBase, pdbentry);         /* Allocate DBENTRY on the stack */
  status = dbFirstRecdes(pdbentry);       /* Get first record description */
  if (status) {printf("No record descriptions\n"); return -1;}
  while (!status)  
  {
    strcpy(recdes, dbGetRecdesName(pdbentry));   /* Get recprd description */
    status = dbFirstRecord(pdbentry);            /* First record of this description */
        while (!status)
         {
           strcpy(recname, dbGetRecordName(pdbentry)); /* Save record name */
           status = dbFindField(pdbentry, field);      /* Find record field (if defined) */
           if (!status)
             { 
              strcpy(valstring, dbGetString(pdbentry)); /* Get field value */
/* Test whether field value matches input string.
   If so, print out record description, name, field name and field value */
              if (!strncmp(valstring, value, strlen(value)))
                {
                nrec++;
                printf("%s %s.%s = %s\n", recdes, recname, field, valstring);
                }
             }
           status = dbNextRecord(pdbentry); /* Get successive records of this description */
         }
    status = dbNextRecdes(pdbentry);        /* Get next record description */
  }

  return nrec;

}


/*+
 *   Function name:
 *   dbNoMatchField
 *
 *   Purpose:
 *   Print out any database records with a specified field
 *   not matching the specified string value
 *
 *   Description:
 *   The routine uses static db access routines to search for
 *   all instances of the field with the specified name, and prints
 *   out the record type and name if not matching the specified string.
 *
 *   Invocation:
 *   dbNoMatchField(field, value)
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)   field  char*   Field name (upper case, no leading .)
 *      (>)   value  char*   String to NOT match in the specified field
 *
 *   Function value:
 *   long   Number of non-matching records
 * 
 *   External functions:
 *   (EPICS static database access routines. See Chapter 5 of EPICS
 *   "IOC Application Developer's Guide" for further details)
 *
 *   External variables:
 *     pdbBase              EPICS static database entry point (see dbStaticLib.h)
 *
 *-
 */

long dbNoMatchField(char *field, char *value)
{
  DBENTRY *pdbentry;               /* Database entry */
  long nrec = 0;                   /* Record count */
  int status;
  char recname[MAX_STRING_SIZE];   /* Record name */
  char valstring[MAX_STRING_SIZE]; /* (string) Value of field */
  char recdes[MAX_STRING_SIZE];    /*  Record description */

 
  dbInitEntry(pdbBase, pdbentry);         /* Allocate DBENTRY on the stack */
  status = dbFirstRecdes(pdbentry);       /* Get first record description */
  if (status) {printf("No record descriptions\n"); return -1;}
  while (!status)  
  {
    strcpy(recdes, dbGetRecdesName(pdbentry));   /* Get recprd description */
    status = dbFirstRecord(pdbentry);            /* First record of this description */
        while (!status)
         {
           strcpy(recname, dbGetRecordName(pdbentry)); /* Save record name */
           status = dbFindField(pdbentry, field);      /* Find record field (if defined) */
           if (!status)
             { 
              strcpy(valstring, dbGetString(pdbentry)); /* Get field value */
/* Test whether field value does not match input string.
   If so, print out record description, name, field name and field value */
              if (strncmp(valstring, value, strlen(value)))
                {
                nrec++;
                printf("%s %s.%s = %s\n", recdes, recname, field, valstring);
                }
             }
           status = dbNextRecord(pdbentry); /* Get successive records of this description */
         }
    status = dbNextRecdes(pdbentry);        /* Get next record description */
  }

  return nrec;

}

#include <stdio.h>
#include <rngLib.h>

STATUS rngShow (RING_ID ringId)
{
     int count;

     printf ("Ring buffer %p contains:\n", ringId);
     for (count = 0; count < ringId->bufSize; count++)
     {
          if (count == ringId->pFromBuf)
               printf ("<r>");
          if (count == ringId->pToBuf)
               printf ("<w>");
          printf ("%c", ringId->buf[count]);
     }
     printf ("\n(<r> and <w> are current read/write points)\n");

     return (OK);
}


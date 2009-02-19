/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
*/

#include <stdlib.h> 
#include "abcp.h"

int abc_tmpi;

abcFraction abc_getfraction(abcScanner *scn,int ndx)
{
   short num=1 , den=-1;
   int k;
   
   if (abcTokenLen(scn,ndx) > 0) num = atoi(abcTokenStart(scn,ndx));
   if (abcTokenLen(scn,ndx+2) > 0) den = atoi(abcTokenStart(scn,ndx+2));
   
   k = abcTokenLen(scn,ndx+1);
   if (k>0) {
     if (den <= 0) den = 2;
     den <<= (k-1);
   }
   else if (den <= 0)
     den = 1;
   
   return ((num & 0xFF)<< 8 | (den & 0xFF));
}


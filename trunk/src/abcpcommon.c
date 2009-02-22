/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
*/

#include <stdlib.h> 
#include "abcp_priv.h"

int abc_tmpi;

float abc_getfraction(abcScanner *scn,int ndx)
{

  int k, j = 0;
  float num = 1.0 ,den=-1.0;
  
  if (abcToken(scn) != T_NOTE) return 0;

  if (abcTokenLen(scn,ndx)>0) {num = atof(abcTokenStart(scn,ndx)); j++;}
  if (abcTokenLen(scn,ndx+2)>0) {den = atof(abcTokenStart(scn,ndx+2)); j++;}
  
  k = abcTokenLen(scn,ndx+1);
  if (k > 0) {
    if (den <= 0.0) den = 2.0;
    den = den * ((float)(1 << (k-1)));
    j++;
  }
  
  if (den <= 0.0)  den = 1.0;

  _dbgmsg("FR: %d %f/%f\n",j,num,den);

  return num/den;
}


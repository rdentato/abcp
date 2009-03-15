/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
*/

#include <stdlib.h> 
#include "abcp_priv.h"

int abc_tmpi;


/*
     (&F)(<*=/>)(&F)
     |   |      |
     a___b______c___
      al    b    cl
*/
float abc_getfraction(char *a, int al,
                     char *b, int bl,
                     char *c, int cl)
{
  int j = 0;
  float num = 1.0 ,den=-1.0;
  
  if (al>0) {num = atof(a); j++;}
  if (cl>0) {den = atof(c); j++;}

  if (bl > 0) {
    if (den <= 0.0) den = 2.0;
    den = den * ((float)(1 << (bl-1)));
    j++;
  }
  
  if (den <= 0.0)  den = 1.0;

  _dbgmsg("FR: %d %f/%f\n",j,num,den);

  return num/den;
} 



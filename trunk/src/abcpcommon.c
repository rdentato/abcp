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
float abc_getfracion(char *a, int al,
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


float abc_notebending(char *e, int el, char *a, int al,
                      char *b, int bl, char *c, int cl)
{
  float cents = 0.0;
  char *p;

  p = e+el;
  
  if (al > 0 || bl > 0 ) {
    /* Got microtones last accidental gives direction*/
    cents = abc_getfracion(a,al,b,bl,c,cl);
    if (bl > 0)
      cents = cents * 100.0;
    if (p > e && *--p == '_')
      cents = cents *-1.0;
  }
  else if (el == 1 && *e == '=')
    return abcNatural;

  while (p > e) {
    switch (*--p) {
      case '^' : cents += 100.0; break ;
      case '_' : cents -= 100.0; break ;
    }
  } 
  
  return cents ; 
}

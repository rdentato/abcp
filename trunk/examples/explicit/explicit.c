/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
**
** Based on "explicit.tcl" by Hudson Lacerda
**
*/

/* .% Overview
==============

  This is an example of use for ABCp. It adds explicit accidentals to
each note that doesn't have one. It will uses the K: field for the key and will
hold accidentals that happen in the same measure.

*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include "abcp.h"

float keysig[7];
float expsig[7];
float barsig[7];

void printtok(abcScanner *scn)
{
  printf("%.*s",abcTokenLen(scn,0),abcTokenStart(scn,0));
}

void printnotetok(abcScanner *scn, float bend)
{
  if (bend != 0.0) {
    barsig[abcNote2Num(*abcTokenStart(scn,5))] = bend; 
    printtok(scn);
    return;
  }

  bend = barsig[abcNote2Num(*abcTokenStart(scn,5))];
  if (bend != abcNatural) {
    while (bend <= -100.0) {
      printf("_");
      bend += 100.0;
    }
    if (bend < 0.0) {
      printf("_%.2f",bend);
    } 
    while (bend >= 100.0) {
      printf("^");
      bend -= 100.0;
    }
    if (bend > 0.0) {
      printf("^%.2f",bend);
    }
  } 
  printf("%.*s",abcTokenLen(scn,5),abcTokenStart(scn,5));
  printf("%.*s",abcTokenLen(scn,6),abcTokenStart(scn,6));
  printf("%.*s",abcTokenLen(scn,7),abcTokenStart(scn,7));
  printf("%.*s",abcTokenLen(scn,8),abcTokenStart(scn,8));
  printf("%.*s",abcTokenLen(scn,9),abcTokenStart(scn,9));
}

void zerosig()
{
  int k;
  for (k=0; k<7; k++) {
    keysig[k] = 0.0;
    expsig[k] = 0.0;
    barsig[k] = 0.0;
  }  
}

void copysig(float *dest, float *src)
{
  int k;
  for (k=0; k<7; k++) {
    dest[k] = src[k];
  }  
}

void resetbarsig()
{
  int k;
  for (k=0; k<7; k++) {
    if (expsig[k] != 0.0)
      barsig[k] = expsig[k];
    else
      barsig[k] = keysig[k];
  }  
}

int main(int argc, char *argv[])
{
  abcScanner *scn;
  abcToken tok;
  
  if (argc < 2) {
    fprintf(stderr,"Usage: explicit filename\n");
    exit (1);
  }

  scn = abcScannerNew(file,argv[1]);
  if (scn == NULL) {
    fprintf(stderr,"Unable to create an ABC scanner for file: '%s'\n",argv[1]);
    exit (1);    
  }
  
  zerosig();
  resetbarsig();
   
  while ((tok = abcNextToken(scn)) != T_EOF) {
    switch(tok) {
      case T_BAR:
        resetbarsig();
        printtok(scn);
        break;
        
      case T_FIELD : 
        switch (abcField(scn)) {
          case 'K' :
            copysig(keysig, abcKeySignature(scn));
            copysig(expsig, abcKeyExpSignature(scn));
            resetbarsig();
            printtok(scn);
            break;
            
          default: printtok(scn);
        }
        break;
        
      case T_NOTE :
        printnotetok(scn,abcNoteBending(scn));
        break;
      
      default: printtok(scn);
    }
  }

  abcScannerFree(scn);
  return (0);
}

/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
*/

/* .% Overview
==============

  This is an example of use for ABCp. It creates and index of the ABC file

*/


#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include "abcp.h"

#define MAXTITLELEN 512

char title[MAXTITLELEN];

int main(int argc, char *argv[])
{
  abcScanner *scn;
  abcToken tok;
  int k;
  int n=-1;
  int state = S_LIMBO;
  
  if (argc < 2) {
    fprintf(stderr,"Usage: abcndx filename\n");
    exit (1);
  }

  scn = abcScannerNew(file,argv[1]);
  if (scn == NULL) {
    fprintf(stderr,"Unable to create an ABC scanner for file: '%s'\n",argv[1]);
    exit (1);    
  }
  
  *title = 0;
  while ((tok = abcNextToken(scn)) != T_EOF) {
    switch(tok) {    
      case T_FIELD :
        switch (abcField(scn)) {
          case 'T' :
            k = abcFieldLen(scn);
            if (k == 0) strcpy(title,"Untitled");
            else {
              if (k>=MAXTITLELEN) k=MAXTITLELEN;
              strncpy(title,abcFieldStart(scn),k);
              title[k] = '\0';
            }
            break;
            
          case 'X' :
            n = atoi(abcFieldStart(scn));
            break;
                  
          case 'K' :
            if (*title == '\0' && n <0) break;
            if (*title == '\0') strcpy(title,"Untitled");
            if (n<0) n = 0;
            printf("%d\t%s\t%s\n",n,title,argv[1]);
            n=-1; *title = '\0';
            break;      
        }
        break;
    }
  }

  abcScannerFree(scn);

  return (0);
}

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
#include "abcp.h"

#define GLOBAL_HEADER 0
#define LOCAL_HEADER 1
#define USYM_MAXLEN 100
#define USYM_NUM 35

char usyms[2][USYM_NUM][USYM_MAXLEN];

abcFraction keyacc[7];
abcFraction measureacc[7];



void resetsyms()
{
  int k;
  for (k = 0; k<USYM_NUM; k++ ) {
    strcpy(usyms[LOCAL_HEADER][k],usyms[GLOBAL_HEADER][k]);
  }
}

void printtok(abcScanner *scn)
{
  printf("%.*s",abcTokenLen(scn,0),abcTokenStart(scn,0));
}

void initkey(abcScanner *scn)
{
  int k;

  
  
}

int main(int argc, char *argv[])
{
  abcScanner *scn;
  abcToken tok;
  int cur_header = GLOBAL_HEADER;
  char sym;
  int k;
  
  if (argc < 2) {
    fprintf(stderr,"Usage: explicit filename\n");
    exit (1);
  }

  scn = abcScannerNew(file,argv[1]);
  if (scn == NULL) {
    fprintf(stderr,"Unable to create an ABC scanner for file: '%s'\n",argv[1]);
    exit (1);    
  }
  
  initkey(scn);
  
  while ((tok = abcNextToken(scn)) != T_EOF) {
    switch(tok) {
      case T_EMPTYLINE : resetsyms();
                         cur_header = GLOBAL_HEADER;
                         printtok(scn);
                         break;
      
      case T_FIELD : 
        switch (abcField(scn)) {
          case 'U' :
            setsym(cur_header,scn);
            break;
            
          case 'X' :
            cur_header = LOCAL_HEADER;
            printtok(scn);
            break;
            
          default: printtok(scn);
        }
        break;
        
      case T_USERSYMBOL : 
        printf("%s", symval(cur_header,abcUserSymbol(scn)));
        break;
      
      default: printtok(scn);
    }
  }

  abcScannerFree(scn);
  return (0);
}

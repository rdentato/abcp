/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
*/

/* .% Overview
==============

  This is an example of use for ABCp. It extracts lyrics and guitar
chords and prints them aligned them.
*/


#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "abcp.h"
#include "libutl.h"

#define STATE_OUT 1
#define STATE_IN 2

int state = STATE_OUT;
int count_notes;

int main(int argc, char *argv[])
{
  abcScanner *scn;
  abcToken tok;
  int line_end = 1;
  
  if (argc < 2) {
    fprintf(stderr,"Usage: crdlyr filename\n");
    exit (1);
  }

  scn = abcScannerNew(file,argv[1]);
  if (scn == NULL) {
    fprintf(stderr,"Unable to create an ABC scanner for file: '%s'\n",argv[1]);
    exit (1);    
  }
  
  count_notes = 0;
  while ((tok = abcNextToken(scn)) != T_EOF) {
    switch(tok) {
    
      case T_FIELD : 
        switch (abcField(scn)) {
          case 'T' :
          case 'C' :
            printf("          %.*s\n",abcTokenLen(scn,3),abcTokenStart(scn,3));
            break;
            
        }
        break;
 
      case T_ENDLINE:
        count_notes = 0;
        break;

      case T_REST : 
      case T_NOTE : 
        count_notes++;
        break;
      
      case T_BAR :
        printf("%d|\n",count_notes);
        break;
       
      case T_GCHORD :
        printf("%d%.*s",count_notes,abcTokenLen(scn,0),abcTokenStart(scn,0));
        break;
       
    }
  }

  abcScannerFree(scn);

  return (0);
}

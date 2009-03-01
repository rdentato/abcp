/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
*/

/* .% Overview
==============

  This is an example of use for ABCp. It extracts guitar
chords and prints the corresponding MIDI notes.

*/


#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include "abcp.h"

char *notes[12]={"C", "^C", "D", "^D","E","F","^F","G","^G","A","^A","B"};
/*char *notes[12]={"Cb","C", "Db", "D", "Eb","E","F","Gb","G","Ab","A","Bb","B"}*/

int main(int argc, char *argv[])
{
  abcScanner *scn;
  abcToken tok;
  char *formula;
  int k;
  int n;
  
  if (argc < 2) {
    fprintf(stderr,"Usage: crdmidi filename\n");
    exit (1);
  }

  scn = abcScannerNew(file,argv[1]);
  if (scn == NULL) {
    fprintf(stderr,"Unable to create an ABC scanner for file: '%s'\n",argv[1]);
    exit (1);    
  }
  
  while ((tok = abcNextToken(scn)) != T_EOF) {
    switch(tok) {    
      case T_GCHORD :
        n = abcSemitones[abcNote2Num(*abcChordRootStart(scn))]-1;
        if (abcChordRootLen(scn)>1) {
          k = abcChordRootStart(scn)[1];
          if (k == '#') n++;
          if (k == 'b') n--;
        }
        n = (n + 12) % 12;
        formula = abcChordFormula(scn);
        
        printf("\"");
             printf("%.*s",abcChordRootLen(scn),abcChordRootStart(scn));
             printf("%.*s",abcChordTypeLen(scn),abcChordTypeStart(scn));
             printf("%.*s",abcChordBassLen(scn),abcChordBassStart(scn));
        printf("\"  ->  ");

        printf("[%s",notes[n]);
        for (k=0; formula[k] != 0; k++) {
          printf("%s",notes[(n+formula[k]) % 12]);
        }
        printf("]\n");
        break;
    }
  }

  abcScannerFree(scn);

  return (0);
}

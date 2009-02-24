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
int count_notes = 0;
int count_chords = 0;
int count_syl = 0;

char chords_buf[2048];

/* no more than 100 chords per line */
char *chord_name[100];  /* name of the ith chord */
short chord_pos[100];   /* notes before the ith chord */


int main(int argc, char *argv[])
{
  abcScanner *scn;
  abcToken tok;
  char *cur_chord_ptr;
  int chords_used;
    
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
  cur_chord_ptr = chords_buf;
  chords_used = 0;
  count_chords = 0;
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
        if (abcScannerCurState(scn) == S_LYRICS) {
          while (chords_used < count_chords) {
            printf("{%s}",chord_name[chords_used++]);
          }
          chords_used = 0;
          count_chords = 0;
          count_syl = 0;
          chord_pos[0] = 0x7F00;
          printf("\n");
        }
        count_notes = 0;
        break;

      case T_REST : 
      case T_NOTE :
        count_notes++;
        break;
      
      case T_BAR :
        break;
       
      case T_LYRBAR :
        break;
        
      case T_GCHORD :
        chord_pos[count_chords] = count_notes;
        chord_name[count_chords] = cur_chord_ptr;
        
        sprintf(cur_chord_ptr,"%.*s",abcTokenLen(scn,0)-2,abcTokenStart(scn,0)+1);
        cur_chord_ptr[abcTokenLen(scn,0)-2] = '\0';
        _dbgmsg("CHORD: %d @ %d [%s]\n",count_chords,chord_pos[count_chords] ,chord_name[count_chords]);
        cur_chord_ptr += (abcTokenLen(scn,0)-1);
        chord_pos[++count_chords] = 0x7F00;
        break;
        
      case T_SYLLABLE :
        while (count_syl >= chord_pos[chords_used]) {
          printf("{%s}",chord_name[chords_used++]);
        }
        printf("%.*s", abcTokenLen(scn,2),abcTokenStart(scn,2));
        count_syl += 1 + abcSyllableHold(scn);
        break;
       
      case T_WHITESPACE :
        if (abcScannerCurState(scn) == S_LYRICS) printf(" ");
        break;
    }
  }

  abcScannerFree(scn);

  return (0);
}

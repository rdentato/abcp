/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
*/

/* .% Overview
==============

  This is an example of use for ABCp. It extracts guitar
chords and prints them embedded into lyrics.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abcp.h"

int print_output();

int count_notes = 0;
int count_chords = 0;
int count_syl = 0;

char chords_buf[2048];

char aligned = 1; /* aligned format (boolean)  */

char out_buf[4096]; /* buffer for the output */
#define LEFT  '{' /* left delimiter of chord */
#define RIGHT '}' /* right delimiter of chord */
#define LSIZE 512 /* max line length */

/* no more than 100 chords per line */
char *chord_name[100];  /* name of the ith chord */
short chord_pos[100];   /* notes before the ith chord */


int main(int argc, char *argv[])
{
  abcScanner *scn;
  abcToken tok;
  char *cur_chord_ptr;
  int chords_used;

  char tmp_buf[LSIZE];
  out_buf[0]='\0';
    
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
            sprintf(tmp_buf,"          %.*s\n",abcTokenLen(scn,3),abcTokenStart(scn,3));
	    strncat(out_buf,tmp_buf,strlen(tmp_buf));
            break;
        }
        break;
 
      case T_ENDLINE:
        if (abcStateCurrent(scn) == S_LYRICS) {
          while (chords_used < count_chords) {
            sprintf(tmp_buf,"{%s}",chord_name[chords_used++]);
	    strncat(out_buf,tmp_buf,strlen(tmp_buf));
          }
          chords_used = 0;
          count_chords = 0;
          count_syl = 0;
          chord_pos[0] = 0x7F00;
	  strncat(out_buf,"\n",strlen("\n"));

	  print_output();

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
        cur_chord_ptr += (abcTokenLen(scn,0)-1);
        chord_pos[++count_chords] = 0x7F00;
        break;
        
      case T_SYLLABLE :
        while (count_syl >= chord_pos[chords_used]) {
          sprintf(tmp_buf,"{%s}",chord_name[chords_used++]);
	  strncat(out_buf,tmp_buf,strlen(tmp_buf));
        }
        sprintf(tmp_buf,"%.*s", abcTokenLen(scn,2),abcTokenStart(scn,2));
	strncat(out_buf,tmp_buf,strlen(tmp_buf));
        count_syl += 1 + abcSyllableHold(scn);
        break;
       
      case T_WHITESPACE :
        if (abcStateCurrent(scn) == S_LYRICS) {
	    strncat(out_buf," ",1);
	}
        break;
    }
  }

  abcScannerFree(scn);
  print_output();

  return (0);
}


int print_output()
{
    int c;			/* a character from the buffer */
    char words[LSIZE];		/* store words line */
    char chords[LSIZE];		/* store chords line */
    char * where = & words[0]; 	/* where to put next chars */
    char * p;			/* pointer for char in chords[]/words[] */
    char * bp; 			/* pointer for char in out_buf[] */
    int count = 0;		/* counter of chars in a line */
    int diff = 0;		/* difference of length (to align words/chords) */
    int found = 0;		/* whether found any chord in current line */
    int incr = 0;		/* increment for chord length */
    int len = 0;                /* chord length */

    /* If not aligned, print simple output, empty buffer and return */
    if (aligned==0) {
	printf("%s",out_buf);
	out_buf[0]='\0';
	return 0;
    }

    words[0]=chords[0]='\0';
    bp = out_buf;
    while ((c = *bp++)!='\0') {
	/* Whether in a chord or in words */
	if(c==LEFT) {
	    where = & chords[strlen(chords)];
	    found = 1;
	    incr = 1;
	    continue;
	}
	else if(c==RIGHT) {
	    *where = ' ';
	    *++where = '\0';
	    where = & words[strlen(words)];
	    len = 0;
	    incr = 0;
	    continue;
	}

	/* Align chords and words */
	if ((diff = strlen(chords)-strlen(words)-len) > 0) {
	    while (diff-- > 0) {
		p = &words[strlen(words)];
		*p = ' ';
		*++p='\0';
	    }
	}
	else {
	    while (diff++ <=0) {
		p = &chords[strlen(chords)];
		*p = ' ';
		*++p='\0';

	    }
	}

	/* Add the character to the proper buffer */
	*where = c;
	*(++where)='\0';
	len+=incr;

	/* Output a line and reset variables */
	if(c=='\n' || count++ >= (LSIZE-1)) {
	    if(found)
		printf("%s\n",chords);
	    printf("%s",words);
	    words[0]=chords[0]='\0';
	    count = 0;
	    where = &words[0];
	    found = 0;
	    len = 0;
	    incr = 0;
	}
    }
    /* Output last line */
    if(found)
	printf("%s\n",chords);
    printf("%s",words);

    /* empty buffer */
    out_buf[0]='\0';

    return 0;

}

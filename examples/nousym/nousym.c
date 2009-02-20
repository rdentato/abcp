/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
*/

/* .% Overview
==============

  This is an example of use for ABCp. It removes all the user symbols
(.~H-Wh-w) and replace them with their meaning (either the default one
or the one assigned with U:).

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

int usymnum(char sym) 
{
  if (sym == '.') return 32;
  if (sym == '~') return 33;
  if ('H' <= sym && sym <= 'W') return sym - 'H';
  if ('h' <= sym && sym <= 'w') return sym - 'h' + 16;
  return 34;
}

char *symval(int header, char sym)
{
  return usyms[header][usymnum(sym)];
}

char *setsym(int header,abcScanner *scn)
{
  int len;
  char sym;
  char *def;
  
  sym = abcUserSymbol(scn);
  def = abcUserSymbolDefineStart(scn);
  len = abcUserSymbolDefineLen(scn);
  
  if (len > USYM_MAXLEN) len = USYM_MAXLEN;
  
  strncpy(symval(header, sym), def, len);
  symval(header, sym)[len] = '\0';
  
  return NULL;
}

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

void initsyms()
{
  int k;
  
  for (k = 0; k<USYM_NUM; k++ ) 
    usyms[GLOBAL_HEADER][k][0] = '\0';
  
  strcpy(usyms[GLOBAL_HEADER][usymnum('.')], "+staccato+");
  strcpy(usyms[GLOBAL_HEADER][usymnum('~')], "+roll+");
  strcpy(usyms[GLOBAL_HEADER][usymnum('T')], "+trill+");
  strcpy(usyms[GLOBAL_HEADER][usymnum('H')], "+fermata+");
  strcpy(usyms[GLOBAL_HEADER][usymnum('L')], "+emphasis+");
  strcpy(usyms[GLOBAL_HEADER][usymnum('M')], "+lowermordent+");
  strcpy(usyms[GLOBAL_HEADER][usymnum('P')], "+uppermordent+");
  strcpy(usyms[GLOBAL_HEADER][usymnum('S')], "+segno+");
  strcpy(usyms[GLOBAL_HEADER][usymnum('O')], "+coda+");
  strcpy(usyms[GLOBAL_HEADER][usymnum('u')], "+upbow+");
  strcpy(usyms[GLOBAL_HEADER][usymnum('v')], "+downbow+");
  resetsyms();  
}

int main(int argc, char *argv[])
{
  abcScanner *scn;
  abcToken tok;
  int cur_header = GLOBAL_HEADER;
  
  if (argc < 2) {
    fprintf(stderr,"Usage: nousym filename\n");
    exit (1);
  }

  scn = abcScannerNew(file,argv[1]);
  if (scn == NULL) {
    fprintf(stderr,"Unable to create a scanner for file: '%s'\n",argv[1]);
    exit (1);    
  }
  
  initsyms();
  
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

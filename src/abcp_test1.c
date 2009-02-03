/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This sofwtare is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
*/

#define UTL_UNITTEST 
#include "libutl.h"

#include "abcp.h"
#include "abcp_priv.h"

int main(int argc, char *argv[])
{
  abcScanner *scn;
  abcToken tok;
  chs_t abctext = NULL;
  int k; 
  
  abctext = chsRead(abctext,stdin,'w');
  if (abctext && *abctext) {
    scn = abcNewScanner(str,abctext);
    printf("<<\n%s\n>>\n",scn->abc);
    while ((tok = abcNextToken(scn)) != T_EOF) {
      printf("%04d %04X %04X ",abcLineNumber(scn), abcCurState(scn), abcToken(scn));
      switch (tok) {
        case T_NOTE:
             printf("NOTE: ");
             printf("%c", abcNotePitch(scn));
             printf("%.*s", abcNoteSharps(scn), "####");
             printf("%.*s", abcNoteFlats(scn), "bbbb");
             k = abcNoteDuration(scn);
             printf(" %d/%d",abcNumerator(k),abcDenominator(k));
             printf(" oct: %d",abcNoteOctave(scn));
             printf(" MIDI: %d",abcNoteMidi(scn));
             printf(" cents: %d",abcNoteCents(scn));
             printf("\n");
             break;
             
        case T_INFIELD:
        case T_FIELD:
             switch (*abcTokenStart(scn,1)) {
               case 'K' : 
                 printf("KEY:");
                 printf(" %.*s", abcKeyTonicLen(scn), abcKeyTonicStart(scn));
                 printf(" %c", abcKeyMode(scn));
                 printf(" t=%d", abcKeyTranspose(scn));
                 printf("\n");
                 break;
                 
              default:
                 printf("%.*s", abcTokenLen(scn,0),abcTokenStart(scn,0));
                 if (tok == T_INFIELD) printf("\n");
             }
             break; 
        
        default:
             printf("%.*s\n", abcTokenLen(scn,0),abcTokenStart(scn,0));
      }
    }
    scn = abcFreeScanner(scn);
  }

  return(0);
}

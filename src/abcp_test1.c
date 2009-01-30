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
  
  abctext = chsRead(abctext,stdin,'w');
  if (abctext && *abctext) {
    scn = abcNewScanner(str,abctext);
    printf("<<\n%s\n>>\n",scn->abc);
    while ((tok = abcNextToken(scn)) != T_EOF) {
      printf("%04d %04X %04X %.*s\n",abcLineNumber(scn), abcCurState(scn), abcToken(scn),abcTokenLen(scn,0),abcTokenStart(scn,0));
    }
    scn = abcFreeScanner(scn);
  }

  return(0);
}

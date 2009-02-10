/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
**   http://opensource.org/licenses/bsd-license.php 
*/

#include <ctype.h>
#include "abcp_priv.h"

unsigned short abcNoteAccidentals(abcScanner *scn)
{
  unsigned short acc = 0;
  char *p;
  int k;
  
  if (abcToken(scn) == T_NOTE) {
    p = abcTokenStart(scn,1);
    for (k=0; k< abcTokenLen(scn,1) && k<10 ; k++,p++) {
      switch (*p) {
        case '_' : acc += 0x0001; break;
        case '^' : acc += 0x0010; break;
        case '=' : acc += 0x0100; break;
      }
    }
    if (*abcTokenStart(scn,0) == '(')
      acc |= 0x1000;
  }
  return acc; 
}

abcFraction abcNoteDuration(abcScanner *scn)
{
  if (abcToken(scn) != T_NOTE) return 1;
  return abc_getfraction(scn,7);
}

abcFraction abcNoteMicrotone(abcScanner *scn)
{
  if (abcToken(scn) != T_NOTE) return 1;
  return abc_getfraction(scn,2);
}

unsigned short abcNoteCents(abcScanner *scn)
{
  unsigned short k;
  if (abcToken(scn) != T_NOTE) return 0;
  k = abc_getfraction(scn,2);
  if (k == 257) /* 1/1 */
    k = 100;
  else if (abcDenominator(k) <= 1) 
    k >>= 8; 
  else
    k = (abcNumerator(k)*100)/abcDenominator(k); 
  return k;
}

unsigned short abcNoteOctave(abcScanner *scn)
{
  int oct = 4;
  int k;
  char *p;
    
  if (abcToken(scn) != T_NOTE) return 0;
  
  if (islower(*abcTokenStart(scn,5))) 
    oct++; 
    
  p = abcTokenStart(scn,6);
  for (k=0; k< abcTokenLen(scn,6); k++,p++) {
    switch (*p) {
      case '\'' : oct++; break;
      case ','  : oct--; break;
    }
  } 
  if (oct <  0) oct = 0;
  else if (oct > 10) oct = 10;
  return oct;
}

unsigned char abcNotePitch(abcScanner *scn)
{
  if (abcToken(scn) != T_NOTE) return 0;
  assert(abcTokenLen(scn,5) > 0);
  return toupper(*abcTokenStart(scn,5)); 
}

unsigned short abcNoteMidi(abcScanner *scn)
{
  static char semitones[8] = {9,11,0,2,4,5,7,0};
  int n;
  unsigned short acc;
  
  if (abcToken(scn) != T_NOTE) return 0;
  
  n = semitones[toupper(*abcTokenStart(scn,5))-'A'];
  
  acc = abcNoteAccidentals(scn);
  
  n += (acc & 0xF0) >> 4;
  n -= (acc & 0x0F);
   
  n += 12 * (abcNoteOctave(scn)+1);
  
  if (n<0) n = 0;
  return n & 0x7F;
}

unsigned short abcNotePitchBend(abcScanner *scn)
{
  return 0;
}

unsigned short abcRestInvisible(abcScanner *scn)
{
  if (abcToken(scn) != T_REST) return 1;
  return (*abcTokenStart(scn,1) == 'x'); 
}

unsigned short abcRestMultimeasure(abcScanner *scn)
{
  if (abcToken(scn) != T_REST) return 0;
  return (*abcTokenStart(scn,1) == 'Z'); 
}

abcFraction abcRestDuration(abcScanner *scn)
{
  if (abcToken(scn) != T_REST) return 1;
  return abc_getfraction(scn,2);
}

/*************/

char *abcBarStart(abcScanner *scn)
{
  if (abcToken(scn) != T_BAR) return utlEmptyString;
  if (abcTokenLen(scn,3) == 0) return utlEmptyString;
  return abcTokenStart(scn,3);
}

int abcBarLen(abcScanner *scn)
{
  if (abcToken(scn) != T_BAR) return 0;
  return abcTokenLen(scn,3);
}

int abcBarDotted(abcScanner *scn)
{ 
  if (abcToken(scn) != T_BAR) return 0;
  if (abcTokenLen(scn,2) > 0) return 1;
  if (abcTokenLen(scn,3) == 0 && abcTokenLen(scn,1) & 1) return 1;
  return 0;
}

int abcBarInvisible(abcScanner *scn)
{
  char *b;
  int l;
  
  if (abcToken(scn) != T_BAR) return 1;
  l = abcTokenLen(scn,3);
  b = abcTokenStart(scn,3);
  if (b[1] == '[') {
    if (l == 2 && b[1] == ']') return 1;
    if (l == 3 && b[1] == '|' && b[2] == ']') return 1;
  }
  else if (l == 1 && b[0] == ']') return 1;
  return 0;
}

int abcBarRepeatBefore(abcScanner *scn)
{
  if (abcToken(scn) != T_BAR) return 0;
  if (abcTokenLen(scn,2) > 0 || abcTokenLen(scn,3) > 0) 
    return abcTokenLen(scn,1);
  else 
    return abcTokenLen(scn,1) / 2;
}

int abcBarRepeatAfter(abcScanner *scn)
{
  if (abcToken(scn) != T_BAR) return 0;
  if (abcTokenLen(scn,2) > 0 || abcTokenLen(scn,3) > 0) 
    return abcTokenLen(scn,4);
  else 
    return abcTokenLen(scn,1) / 2;
}

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

float abcNoteDuration(abcScanner *scn)
{
  if (abcToken(scn) != T_NOTE) return 1.0;
  return abc_getfraction(scn,7);
}

float abcNoteCents(abcScanner *scn)
{
  float cents;
  if (abcToken(scn) != T_NOTE) return 0.0;
  if (abcTokenLen(scn,2) == 0) return 0.0;
   
  cents = abc_getfraction(scn,2);
  if (abcTokenLen(scn,3) > 0)
    cents = cents * 100.0; 
  return cents ; 
}

float abc_notebending(abcScanner *scn, unsigned short tok)
{
  float cents = 0.0;
  char *p;
  
  if (abcToken(scn) != tok) return 0.0;
  
  p = abcTokenEnd(scn,1);
  
  if (abcTokenLen(scn,2) > 0 || abcTokenLen(scn,3) > 0 ) {
    /* Got microtones last accidental gives direction*/
    cents = abc_getfraction(scn,2);
    if (abcTokenLen(scn,3) > 0)
      cents = cents * 100.0;
    if (p > abcTokenStart(scn,1)) {
      if (*--p == '_') cents = cents *-1.0;
    } 
  } 
  
  while (p > abcTokenStart(scn,1)) {
    switch (*--p) {
      case '^' : cents += 100.0; break ;
      case '_' : cents -= 100.0; break ;
    }
  } 
  printf("==%s==\n",abcTokenStart(scn,1));
   
  return cents ; 
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

int abcNoteNatural(abcScanner *scn)
{
  if (abcToken(scn) != T_NOTE) return 0;
  if (abcNoteBending(scn) > 0.0) return 0;
  if (abcTokenLen(scn,1) != 1) return 0;
  if (*abcTokenStart(scn,1) != '=') return 0;
  return 1;
}

int abcNoteCourtesyAccidentals(abcScanner *scn) 
{
  if (abcToken(scn) != T_NOTE) return 0;
  if (*abcTokenStart(scn,0) != '(') return 0;
  return 1;
}


unsigned char *abcNotePitch(abcScanner *scn)
{
  static char pitch[8];
  int k = 0;
  float bend;
  
  if (abcToken(scn) == T_NOTE) {  
    pitch[k++] = toupper(*abcTokenStart(scn,5));
    bend = abcNoteBending(scn);
    if (bend > 0.0) {
      while (bend >= 100.0) {
        pitch[k++] = '#';
        bend -= 100;
      }
    }  
    else if (bend < 0.0) {
      while (bend <= -100.0) {
        pitch[k++] = 'b';
        bend += 100;
      } 
    }
  }
  pitch[k] = '\0';
  return pitch; 
}

unsigned short abcNoteMidi(abcScanner *scn)
{
  int n; 
  if (abcToken(scn) != T_NOTE) return 0;
  
  n = abcSemitones[abcNote2Num(*abcTokenStart(scn,5))];
  n += (int)(abcNoteBending(scn)/100.0);   
  n += 12 * (abcNoteOctave(scn)+1);
  if (n<0) n = 0;
  return n & 0x7F;
}

unsigned short abcNoteMidiPitchBend(abcScanner *scn)
{
  float bend;
  if (abcToken(scn) != T_NOTE) return 0;
  
  bend =  abcNoteBending(scn)/100;
  return (8192 + (int)(8192.0 * (bend - trunc(bend))));
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

float abcRestDuration(abcScanner *scn)
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

char *abcBarEndingStart(abcScanner *scn)
{
  if (abcToken(scn) != T_ENDING) return utlEmptyString;
  return abcTokenStart(scn,2);
}

int abcBarEndingLen(abcScanner *scn)
{
  if (abcToken(scn) != T_ENDING) return 0;
  return abcTokenLen(scn,2);
}

/*** Broken rythm */

int abcBroken(abcScanner *scn)
{
  if (abcToken(scn) != T_BROKENLEFT && abcToken(scn) != T_BROKENRIGHT)
     return 0;
  return abcTokenLen(scn,0);
}

/*** Chord */

float abcChordDuration(abcScanner *scn)
{
  if (abcToken(scn) != T_CHORDEND) return 1;
  return abc_getfraction(scn,1);
}

/*** Overlay */

int abcOverlay(abcScanner *scn)
{
  if (abcToken(scn) != T_OVLRESET  && abcToken(scn) != T_OVLSTART   && abcToken(scn) != T_OVLEND)
     return 0;
  return abcTokenLen(scn,1);
}

/** slur and tie */

char abcSlurDirection(abcScanner *scn)
{
  if ((abcToken(scn) != T_SLURSTART && abcToken(scn) != T_SLUREND) ||
      abcTokenLen(scn,2) == 0) return '-';
  return *abcTokenStart(scn,2);
}

char abcTieDirection(abcScanner *scn)
{
  if (abcToken(scn) != T_TIE || abcTokenLen(scn,2) == 0) return '-';
  return *abcTokenStart(scn,2);
}

int abcSlurDotted(abcScanner *scn)
{
  if (abcToken(scn) != T_SLURSTART && abcToken(scn) != T_SLUREND) return 0;
  return (abcTokenLen(scn,1) > 0);
}

int abcTieDotted(abcScanner *scn)
{
  if (abcToken(scn) != T_TIE) return 0;
  return (abcTokenLen(scn,1) > 0);
}

/** TUPLET */

int abcTuplet(abcScanner *scn)
{
  if (abcToken(scn) != T_TUPLET) return 0;
  return atoi(abcTokenStart(scn,1));
}

int abcTupletTime(abcScanner *scn)
{
  if (abcToken(scn) != T_TUPLET || abcTokenLen(scn,2)==0) return 0;
  return atoi(abcTokenStart(scn,2));
}

int abcTupletNext(abcScanner *scn)
{
  if (abcToken(scn) != T_TUPLET || abcTokenLen(scn,3)==0) return 0;
  return atoi(abcTokenStart(scn,3));
}


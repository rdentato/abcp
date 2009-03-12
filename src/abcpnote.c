/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This software is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
**   http://opensource.org/licenses/bsd-license.php 
*/

#include <ctype.h>
#include "abcp_priv.h"



#define getfraction(s,n) abc_getfracion(abcTokenStart(s,n),   \
                                        abcTokenLen(s,n),     \
                                        abcTokenStart(s,n+1), \
                                        abcTokenLen(s,n+1),   \
                                        abcTokenStart(s,n+2), \
                                        abcTokenLen(s,n+2)) 

static char *sagittal[] = {
"!",
"!!!(",
"!!!)",
"!!!/",
"!!!/)",
"!!!//",
"!!!~",
"!!)",
"!!/",
"!!~",
"!(",
"!)",
"!/",
"!/)",
"!//",
"!~",
"(",
"(!",
"(!!",
"(!!!",
"(!!!(",
"(!!!)",
"(!!!/",
"(!!!~",
"(!!(",
"(!!~",
"(!(",
"(!)",
"(!/",
"(!~",
"(/|",
"(/|||",
"(X(",
"(X~",
"(Y",
"(Y(",
"(Y~",
"(\\!",
"(\\!!!",
"(|",
"(|(",
"(|)",
"(|\\",
"(||",
"(||(",
"(|||",
"(|||(",
"(|||)",
"(|||\\",
"(|||~",
"(||~",
"(|~",
")!",
")!!!",
")!!!(",
")!!!)",
")!!!//",
")!!!~",
")!!(",
")!!)",
")!!~",
")!(",
")!)",
")!//",
")!~",
")/",
")//",
")//|",
")//||",
")//|||",
")/|",
")/|\\",
")/||",
")/|||",
")/|||\\",
")X(",
")X)",
")X~",
")Y(",
")Y)",
")Y~",
")\\!",
")\\!!",
")\\!!!",
")\\!!!/",
")\\!/",
")\\Y",
")\\\\!",
")\\\\!!",
")\\\\!!!",
")\\\\Y",
")|",
")|(",
")|)",
")|\\\\",
")||(",
")||)",
")|||",
")|||(",
")|||)",
")|||\\\\",
")|||~",
")||~",
")|~",
")~",
")~!",
")~!!",
")~!!!",
")~Y",
")~|",
")~||",
")~|||",
"/",
"//",
"//|",
"//||",
"//|||",
"/X)",
"/X\\",
"/X~",
"/|",
"/|)",
"/|\\",
"/||",
"/||)",
"/||\\",
"/|||",
"/|||)",
"/|||\\",
"/|||~",
"/||~",
"/|~",
"X)",
"X\\",
"X~",
"Y)",
"Y/",
"Y~",
"\\!",
"\\!!",
"\\!!!",
"\\!!!)",
"\\!!!/",
"\\!!!~",
"\\!!)",
"\\!!/",
"\\!!~",
"\\!)",
"\\!/",
"\\!~",
"\\Y",
"\\Y)",
"\\Y/",
"\\Y~",
"\\\\!",
"\\\\!!",
"\\\\!!!",
"\\\\Y",
"^",
"^^",
"_",
"__",
"|",
"|(",
"|)",
"|//|",
"|\\",
"|\\)",
"|\\\\",
"||)",
"||\\",
"|||(",
"|||)",
"|||\\",
"|||\\)",
"|||\\\\",
"|||~",
"||~",
"|~",
"~!",
"~!!!",
"~!!!(",
"~!!!)",
"~!!!/",
"~!!(",
"~!!)",
"~!!/",
"~!(",
"~!)",
"~!/",
"~X(",
"~X)",
"~X\\",
"~Y(",
"~Y)",
"~|",
"~|(",
"~|)",
"~|/",
"~|\\",
"~||(",
"~||)",
"~||\\",
"~|||",
"~|||(",
"~|||)",
"~|||\\",
"~~",
"~~!",
"~~!!",
"~~!!!",
"~~Y",
"~~|",
"~~||",
"~~|||",
};

static float sagittalbend[] = {
  0.0         ,
  -119.4428   ,
  -140.9491   ,
  -145.4517   ,
  -172.8418   ,
  -177.4751   ,
  -130.2293   ,
  -86.4209    ,
  -92.1787    ,
  -75.6344    ,
  -5.7578     ,
  -27.2641    ,
  -31.7667    ,
  -59.1568    ,
  -63.7901    ,
  -16.5443    ,
  207.2880    ,
  -33.1480    ,
  -93.6030    ,
  -146.8330   ,
  -152.5908   ,
  -174.0971   ,
  -178.5996   ,
  -163.5799   ,
  -98.9546    ,
  -110.3070   ,
  -38.9058    ,
  -60.4121    ,
  -64.9146    ,
  -49.8949    ,
  54.5282     ,
  168.2132    ,
  212.6396    ,
  223.9920    ,
  -207.2880   ,
  -212.6396   ,
  -223.9920   ,
  -54.5282    ,
  -168.2132   ,
  33.1480     ,
  38.9058     ,
  60.4121     ,
  64.9146     ,
  93.6030     ,
  98.9546     ,
  146.8330    ,
  152.5908    ,
  174.0971    ,
  178.5996    ,
  163.5799    ,
  110.3070    ,
  49.8949     ,
  -3.3780     ,
  -117.0630   ,
  -123.3730   ,
  -144.3271   ,
  -180.9761   ,
  -133.7670   ,
  -70.6724    ,
  -88.8007    ,
  -80.5370    ,
  -9.6880     ,
  -30.6421    ,
  -67.2911    ,
  -20.0820    ,
  196.7279    ,
  218.6404    ,
  46.3939     ,
  104.9554    ,
  160.0790    ,
  24.8843     ,
  56.4819     ,
  83.0429     ,
  138.5693    ,
  170.1669    ,
  184.3574    ,
  202.4857    ,
  194.2220    ,
  -184.3574   ,
  -202.4857   ,
  -194.2220   ,
  -24.8843    ,
  -83.0429    ,
  -138.5693   ,
  -170.1669   ,
  -56.4819    ,
  -196.7279   ,
  -46.3939    ,
  -104.9554   ,
  -160.0790   ,
  -218.6404   ,
  3.3780      ,
  9.6880      ,
  30.6421     ,
  67.2911     ,
  70.6724     ,
  88.8007     ,
  117.0630    ,
  123.3730    ,
  144.3271    ,
  180.9761    ,
  133.7670    ,
  80.5370     ,
  20.0820     ,
  187.0399    ,
  -12.0644    ,
  -73.3549    ,
  -125.7494   ,
  -187.0399   ,
  12.0644     ,
  73.3549     ,
  125.7494    ,
  195.6034    ,
  217.6821    ,
  43.0126     ,
  103.9970    ,
  156.6976    ,
  221.6122    ,
  227.3700    ,
  210.8257    ,
  21.5063     ,
  48.7704     ,
  53.2729     ,
  81.9184     ,
  107.9272    ,
  113.6850    ,
  135.1913    ,
  162.4554    ,
  166.9579    ,
  151.7356    ,
  97.1407     ,
  38.0506     ,
  200.1059    ,
  205.8637    ,
  189.3194    ,
  -200.1059   ,
  -205.8637   ,
  -189.3194   ,
  -21.5063    ,
  -81.9184    ,
  -135.1913   ,
  -162.4554   ,
  -166.9579   ,
  -151.7356   ,
  -107.9272   ,
  -113.6850   ,
  -97.1407    ,
  -48.7704    ,
  -53.2729    ,
  -38.0506    ,
  -195.6034   ,
  -221.6122   ,
  -227.3700   ,
  -210.8257   ,
  -43.0126    ,
  -103.9970   ,
  -156.6976   ,
  -217.6821   ,
  113.6850    ,
  227.3700    ,
  -113.6850   ,
  -227.3700   ,
  0.0         ,
  5.7578      ,
  27.2641     ,
  0.0000      ,
  31.7667     ,
  59.1568     ,
  63.7901     ,
  86.4209     ,
  92.1787     ,
  119.4428    ,
  140.9491    ,
  145.4517    ,
  172.8418    ,
  177.4751    ,
  130.2293    ,
  75.6344     ,
  16.5443     ,
  -8.7296     ,
  -122.4146   ,
  -128.4154   ,
  -149.3818   ,
  -154.0151   ,
  -74.7792    ,
  -96.1089    ,
  -101.6206   ,
  -14.7304    ,
  -35.6968    ,
  -40.3301    ,
  188.4642    ,
  209.7939    ,
  215.3056    ,
  -188.4642   ,
  -209.7939   ,
  8.7296      ,
  14.7304     ,
  35.6968     ,
  -215.3056   ,
  40.3301     ,
  74.7792     ,
  96.1089     ,
  101.6206    ,
  122.4146    ,
  128.4154    ,
  149.3818    ,
  154.0151    ,
  191.6732    ,
  -17.5761    ,
  -77.9882    ,
  -131.2611   ,
  -191.6732   ,
  17.5761     ,
  77.9882     ,
  131.2611    ,
  };

unsigned char sagittalcode[] = {
 0x32,
 0x5A,
 0x50,
 0x4E,
 0x41,
 0x3F,
 0x55,
 0x68,
 0x66,
 0x6D,
 0x8E,
 0x84,
 0x82,
 0x75,
 0x73,
 0x89,
 0xEF,
 0x81,
 0x65,
 0x4D,
 0x4A,
 0x40,
 0x3E,
 0x45,
 0x62,
 0x5D,
 0x7E,
 0x74,
 0x72,
 0x79,
 0xA9,
 0xDD,
 0xF2,
 0xF7,
 0x31,
 0x2E,
 0x29,
 0x77,
 0x43,
 0x9F,
 0xA2,
 0xAC,
 0xAE,
 0xBB,
 0xBE,
 0xD3,
 0xD6,
 0xE0,
 0xE2,
 0xDB,
 0xC3,
 0xA7,
 0x8F,
 0x5B,
 0x58,
 0x4F,
 0x3D,
 0x53,
 0x70,
 0x67,
 0x6B,
 0x8C,
 0x83,
 0x71,
 0x87,
 0xEB,
 0xF5,
 0xA5,
 0xC1,
 0xD9,
 0x9B,
 0xAA,
 0xB7,
 0xCF,
 0xDE,
 0xE4,
 0xED,
 0xE9,
 0x3C,
 0x33,
 0x37,
 0x85,
 0x69,
 0x51,
 0x42,
 0x76,
 0x35,
 0x7B,
 0x5F,
 0x47,
 0x2B,
 0x91,
 0x94,
 0x9D,
 0xAF,
 0xB0,
 0xB9,
 0xC5,
 0xC8,
 0xD1,
 0xE3,
 0xCD,
 0xB5,
 0x99,
 0xE5,
 0x8B,
 0x6F,
 0x57,
 0x3B,
 0x95,
 0xB1,
 0xC9,
 0xEA,
 0xF4,
 0xA4,
 0xC0,
 0xD8,
 0xF6,
 0xF8,
 0xF1,
 0x9A,
 0xA6,
 0xA8,
 0xB6,
 0xC2,
 0xC4,
 0xCE,
 0xDA,
 0xDC,
 0xD5,
 0xBD,
 0xA1,
 0xEC,
 0xEE,
 0xE7,
 0x34,
 0x32,
 0x39,
 0x86,
 0x6A,
 0x52,
 0x46,
 0x44,
 0x4B,
 0x5E,
 0x5C,
 0x63,
 0x7A,
 0x78,
 0x7F,
 0x36,
 0x2A,
 0x28,
 0x2F,
 0x7C,
 0x60,
 0x48,
 0x2C,
 0xFD,
 0xFF,
 0x23,
 0x21,
 0x32,
 0x92,
 0x9C,
 0x90,
 0x9E,
 0xAB,
 0xAD,
 0xB8,
 0xBA,
 0xC6,
 0xD0,
 0xD2,
 0xDF,
 0xE1,
 0xCB,
 0xB3,
 0x97,
 0x8D,
 0x59,
 0x56,
 0x4C,
 0x49,
 0x6E,
 0x64,
 0x61,
 0x8A,
 0x80,
 0x7D,
 0xE6,
 0xF0,
 0xF3,
 0x3A,
 0x30,
 0x93,
 0x96,
 0xA0,
 0x2D,
 0xA3,
 0xB2,
 0xBC,
 0xBF,
 0xC7,
 0xCA,
 0xD4,
 0xD7,
 0xE8,
 0x88,
 0x6C,
 0x54,
 0x38,
 0x98,
 0xB4,
 0xCC,
};


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
  return getfraction(scn,7);
}

float abcNoteCents(abcScanner *scn)
{
  float cents;
  cents = abcNoteBending(scn);
  if (cents == abcNatural)
      cents = 0.0; 
  return cents ; 
}

static int abc_sagittalnum(char *sag)
{
  int n,j,k;
  int cmp;
  
  n = 0;
  j = 0; k = (sizeof(sagittal) / sizeof(sagittal[0]))-1;
  while (j<=k) {
    n = (j+k)/2;
    cmp = strcmp(sag,sagittal[n]) ;
    if ( cmp == 0) break;
    if ( cmp < 0) k = n-1;
    else j = n+1;
  }
  if (j<=k) return n;
  return -1;
}


static float abc_sagittalcents(char *sag,int len)
{
  float  bend = 0.0;
  char sagit[8];
  int n;
  
  if (len > 7) len = 7;
  switch (*sag) {
    case '\'': bend = 1.954 * 2;
    case '`' : bend -= 1.954;
               sag++;  len--;
               break;
  }
  if (len == 0) return bend;
  
  switch(sag[len-1]) {
    case '\'': if (len>1 && sag[len-2] == '\'') {
                 bend += 0.833 - 0.423; len--;
               }
               bend += 0.423; len--;
               break;  
     
    case '`':  if (len>1 && sag[len-2] == '\'') {
                 bend += -(0.833 - 0.423); len--;
               }
               bend += -0.423; len--;
               break;  
  }
  if (len == 0) return bend;
  
  strncpy(sagit,sag,len);
  sagit[len] = '\0';
  _dbgmsg("SAGIT: %s\n",sagit);
  n = abc_sagittalnum(sagit);
  if (n>=0) bend += sagittalbend[n];
  return bend;  
}

float abcNoteBending(abcScanner *scn)
{
  int len;
  static float bend;
  
  if (abcToken(scn) != T_NOTE) return 0.0;
  
  if (abcTestFlag(scn, ABC_F_BENDCOMPUTED))  
    return bend;
    
  abcSetFlag(scn,ABC_F_BENDCOMPUTED);
  
  len = abcTokenLen(scn,1) ;
  if (len > 1 && *abcTokenStart(scn,1) == '=') 
    bend = abc_sagittalcents(abcTokenStart(scn,1)+1,len-1);
  else 
    bend = abc_notebending(abcTokenStart(scn,1), abcTokenLen(scn,1),
                         abcTokenStart(scn,2), abcTokenLen(scn,2),
                         abcTokenStart(scn,3), abcTokenLen(scn,3),
                         abcTokenStart(scn,4), abcTokenLen(scn,4) );
                         
  return bend;
}

float abcNoteMicrotoneNum(abcScanner *scn)
{
  int k;
  float num = 1.0;
  
  if (abcToken(scn) != T_NOTE) return 0.0;
  if (abcTokenLen(scn,2) > 0) 
    num = atof(abcTokenStart(scn,2));
  k = abcTokenLen(scn,1);
  if (k>0 && abcTokenStart(scn,1)[k-1] == '_')
    num = -num;
  return num;
}

float abcNoteMicrotoneDen(abcScanner *scn)
{
  int k;
  float den = -1.0;
  
  if (abcToken(scn) != T_NOTE) return 1.0;
  
  if (abcTokenLen(scn,4) > 0) 
    den = atof(abcTokenStart(scn,4));
    
  k = abcTokenLen(scn,3);
  if (k > 0) {
    if (den <= 0.0) den = 2.0;
    den = den * ((float)(1 << (k-1)));
  }
    
  if (den <= 0.0) {
    if (abcTokenLen(scn,2) > 0) den = 100.0;
    else den = 1.0;
  }
  return den;
}


int abcNoteOctave(abcScanner *scn)
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
  if (oct <  -1) oct = -1;
  else if (oct > 9) oct = 9;
  return oct;
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
    if (bend == abcNatural)
      bend = 0.0;
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

unsigned short abcNote2Midi(char pitch, int octave, int acc)
{
  int n;
  n = abcSemitones[abcNote2Num(pitch)] - 1 + acc;
  n += 12 * octave;
  while (n>127) n -= 12;
  while (n<0) n += 12;
  return n;
}

unsigned short abcNoteMidi(abcScanner *scn)
{
  if (abcToken(scn) != T_NOTE) return 0;
  return abcNote2Midi(*abcTokenStart(scn,5),
                       abcNoteOctave(scn)+1,
                 (int)(abcNoteCents(scn)/100.0));
 }

unsigned short abcNoteMidiPitchBend(abcScanner *scn)
{
  float bend;
  if (abcToken(scn) != T_NOTE) return 0;
  
  bend =  abcNoteCents(scn)/100;
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
  return getfraction(scn,2);
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

int abcBarEnding(abcScanner *scn)
{
  if (abcToken(scn) != T_ENDING) return 0;
  return atoi(abcTokenStart(scn,2));
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
  return getfraction(scn,1);
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


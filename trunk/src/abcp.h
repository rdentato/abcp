/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This sofwtare is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
**   http://opensource.org/licenses/bsd-license.php 
*/

#ifndef ABCP_H
#define ABCP_H

/* .% Scanner states
*/
#define S_NONE                0x0000
#define S_ERROR               0x0001
#define S_LIMBO               0x0002
#define S_EOF                 0x0003
#define S_EATSOL              0x0004
#define S_HEADER              0x0005
#define S_INCLUDE             0x0010
#define S_TEXT                0x0011
#define S_FIELD               0x1000  
#define S_EXTFIELD            0x2000
#define S_INFIELD             0x4000
#define S_CFIELD              0x5000  /* A "colon" field like A: or [A:]*/
#define S_ANYFIELD            0x7000
#define S_TUNE                0x8000
#define S_GRACE               0x8001  /* in grace */
#define S_CHORD               0x8002  /* in a chord */
#define S_LYRICS              0x8004
#define S_SYMBOLS             0x8008
#define S_CLEARIN             0xBFFF  /* */

/* -- BEGIN TOKENS  */

#define T_NONE                0x0000 
#define T_UNKNOWN             0x0001 
#define T_UNUSED              0x00FF 
#define T_EOF                 0x0003 
#define T_COMMENT             0x0005 
#define T_EMPTYLINE           0x0006 
#define T_EXTFIELD            0x0007 
#define T_FIELD               0x0008 
#define T_NOTE                0x0009 
#define T_SPACER              0x000A 
#define T_BAR                 0x000B 
#define T_INFIELD             0x000C 
#define T_TEXT                0x000D 
#define T_ENDLINE             0x000E 
#define T_CONTINUE            0x000F 
#define T_DECORATION          0x0010 
#define T_REST                0x0011 
#define T_INVREST             0x0012 
#define T_MULTIREST           0x0013 
#define T_WHITESPACE          0x0014 
#define T_TUPLET              0x0015 
#define T_SLURSTART           0x0016 
#define T_SLUREND             0x0017 
#define T_BROKENRIGHT         0x0018 
#define T_BROKENLEFT          0x0019 
#define T_CHORD               0x001A 
#define T_CHORDEND            0x001B 
#define T_GCHORD              0x001C 
#define T_REPEAT              0x001D 
#define T_ACCIACATURA         0x001E 
#define T_GRACE               0x001F 
#define T_GRACEEND            0x0020 
#define T_TIE                 0x0021 
#define T_STRING              0x0022 
#define T_STRINGUNF           0x0023 
#define T_ANNOTATION          0x0024 
#define T_BREAKLINE           0x0025 
#define T_STARTLINE           0x0026 
#define T_FIELDB              0x0027 
#define T_PRAGMA              0x0028 
#define T_INCLUDE             0x0029 
#define T_INCLUDE_EOF         0x002A 
#define T_ENDING              0x002B 
#define T_USERSYMBOL          0x002C

#define T_SYMBOLS        0x002D
#define T_LYRICS         0x002E
#define T_SYLLABLE            0x002F 
#define T_VERSE               0x0034 
                                     
#define T_TIE_DOT             0x0037 
#define T_DUPMESURE           0x0038 
                                     
#define T_SYM_BLANK           0x003A 
#define T_SYM_BAR             0x003B 
#define T_SYM_VERSE           0x003C 
#define T_SYM_CONTINUE        0x003D 
#define T_SYM_GCHORD          0x003E 
#define T_SYM_DECORATION      0x003F 
#define T_SYM_ANNOTATION      0x0040 
                                     
#define T_IGNORE              0x0041 
#define T_DOTLEFT2            0x0042 
#define T_DOTRIGHT2           0x0043 
#define T_LAST                0x0044 
                                     
#define T_OVLRESET            0x0046 
#define T_OVLEND              0x0047 
#define T_OVLSTART            0x0048 
#define T_DOVERLAY            0x0049 
#define T_DOVLEND             0x004A 
#define T_DOVLSTART           0x004B 

#define T_BEGINTEXT           0x0050
#define T_ENDTEXT             0x0051   

#define T_MIDI                0x0052
#define T_DEFINE              0x0053
#define T_SETDRUM             0x0054
                       
                       
#define T_INTERNALERR         0xF000
   
/* -- END TOKENS  */

/* -- FLAGS */

#define F_LINESTART           0x0001
#define F_AFTERBAR            0x0002

#define abcSetFlag(s,f)       ((s)->flags |= (f))
#define abcClrFlag(s,f)       ((s)->flags &= ~(f))
#define abcTestFlag(s,f)      ((s)->flags & (f))

/* -- END FLAGS */


typedef  unsigned short abcToken; 

#define abc_MAXTOKSTR 10

typedef struct {
  char  *abc;
  char  *pos;
  FILE  *file;
  char  *line;
  char  *cur;
  unsigned short state;
  unsigned short nextstate;
  unsigned short lnumber;
  unsigned short ln_logical;
  unsigned short flags;
  abcToken tok;
  char *tok_str[abc_MAXTOKSTR][2]; /* begin, end */
} abcScanner;

abcScanner *abc_newscanner(char *text,char how);

#define abcNew_file(s) abc_newscanner(s,'F')
#define abcNew_str(s)  abc_newscanner(s,'S')
#define abcNew_chs(s)  abc_newscanner(s,'C')

#define abcNewScanner(x,y) abcNew_##x(y)
abcScanner *abcFreeScanner(abcScanner *scn);

abcToken abcNextToken(abcScanner *scn);

#define abcTokenStart(s,k) ((s)->tok_str[k][0])
#define abcTokenEnd(s,k)   ((s)->tok_str[k][1])
#define abcTokenLen(s,k)   (abcTokenEnd(s,k) - abcTokenStart(s,k))

#define abcToken(s) ((s)->tok)

#define abcCurState(s) ((s)->state)
#define abcNextState(s) ((s)->nextstate)
#define abcLineNumber(s) ((s)->ln_logical)

#endif

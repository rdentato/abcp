/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This sofwtare is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
**   http://opensource.org/licenses/bsd-license.php 
*/

#ifndef ABCP_H
#define ABCP_H

/*  
** xxxx xxxx xxxx xxxx
** \__/
**   \__________________ 0 : STATE
**                       1 : TOKEN
**                       2 : EXTENDED FIELD
**                       3 : INSTRUMENT
**                       4 : DECORATION
*/

/* .% Scanner states
*/
#define S_NONE                0x0000
#define S_EOF                 0x0001
#define S_LIMBO               0x0002
#define S_HEADER              0x0003
#define S_LYRICS              0x0004
#define S_SYMBOLS             0x0005
#define S_TUNE                0x0010
#define S_GRACE               0x0011  /* in grace */
#define S_CHORD               0x0012  /* in a chord */


/* -- BEGIN TOKENS  */

#define T_NONE                0x1000 
#define T_UNKNOWN             0x1001 
#define T_EOF                 0x1002 
#define T_COMMENT             0x1003 
#define T_FIELD               0x1004 
#define T_EXTFIELD            0x1005 
#define T_INFIELD             0x1006 
#define T_PRAGMA              0x1007 
#define T_NOTE                0x1008 
#define T_REST                0x1009 
#define T_SPACER              0x100A 
#define T_BAR                 0x100B 
#define T_TEXT                0x100C 
#define T_EMPTYLINE           0x100D 
#define T_ENDLINE             0x100E 
#define T_CONTINUE            0x100F 
#define T_BREAKLINE           0x1010 
#define T_DECORATION          0x1011 
#define T_ANNOTATION          0x1012 
#define T_GCHORD              0x1013 
#define T_WHITESPACE          0x1014 
#define T_IGNORE              0x1015
#define T_TIE                 0x1016 
#define T_TUPLET              0x1017 
#define T_SLURSTART           0x1018 
#define T_SLUREND             0x1019 
#define T_OVLRESET            0x101A 
#define T_OVLEND              0x101B 
#define T_OVLSTART            0x101C
#define T_BROKENRIGHT         0x101D 
#define T_BROKENLEFT          0x101E 
#define T_CHORD               0x101F 
#define T_CHORDEND            0x1020 
#define T_GRACE               0x1021 
#define T_GRACEEND            0x1022 
#define T_ENDING              0x1023 
#define T_USERSYMBOL          0x1024
#define T_SYMBOLS             0x1025
#define T_SYLLABLE            0x1026 
#define T_LYRICS              0x1027
#define T_VERSE               0x1028 
                                   
#if 0                              
                                     
#define T_INVREST             0x1012 
#define T_REPEAT              0x101D 
#define T_MULTIREST           0x1013 
#define T_DUPMESURE           0x0038 
                                     
#define T_TIE_DOT             0x0037 
#define T_SYM_BLANK           0x003A 
#define T_SYM_BAR             0x003B 
#define T_SYM_VERSE           0x003C 
#define T_SYM_CONTINUE        0x003D 
#define T_SYM_GCHORD          0x003E 
#define T_SYM_DECORATION      0x003F 
#define T_SYM_ANNOTATION      0x0040 
#define T_ACCIACATURA         0x001E 
#define T_STRING              0x0022 
#define T_STRINGUNF           0x0023 
#define T_STARTLINE           0x0026 
#define T_FIELDB              0x0027 
#define T_INCLUDE             0x0029 
#define T_INCLUDE_EOF         0x002A 
                                     
#define T_DOTLEFT2            0x0042 
#define T_DOTRIGHT2           0x0043 
#define T_LAST                0x0044 
                                     
 
#define T_DOVERLAY            0x0049 
#define T_DOVLEND             0x004A 
#define T_DOVLSTART           0x004B 

#define T_BEGINTEXT           0x0050
#define T_ENDTEXT             0x0051   

#define T_MIDI                0x0052
#define T_DEFINE              0x0053
#define T_SETDRUM             0x0054
                       
#define T_INTERNALERR         0xF000
#endif
   
/* -- END TOKENS  */



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

#define abcToken(s) ((s)->tok)
char *abcTokenStart(abcScanner *scn, int strnum);
char *abcTokenEnd(abcScanner *scn, int strnum);
int abcTokenLen(abcScanner *scn, int strnum);


#define abcCurState(s) ((s)->state)
#define abcNextState(s) ((s)->nextstate)
#define abcLineNumber(s) ((s)->ln_logical)

#endif

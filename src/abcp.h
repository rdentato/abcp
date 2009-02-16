/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This sofwtare is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
**   http://opensource.org/licenses/bsd-license.php 
*/

#ifndef ABCP_H
#define ABCP_H

#include <stdio.h>
#include <assert.h>
#include <math.h>

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
#define S_TEXT                0x0006
#define S_HISTORY             0x0007
#define S_TUNE                0x0008
#define S_GRACE               0x0009
#define S_CHORD               0x000A
#define S_TEXT_PS             0x000B
#define S_MAXSTATE            0x000C


/* -- BEGIN TOKENS  */

#define T_NONE                0x1000 
#define T_UNKNOWN             0x1001 
#define T_EOF                 0x1002 
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
#define T_MEASUREREPEAT       0x1029
#define T_BEGINTEXT           0x102A
#define T_ENDTEXT             0x102B
#define T_BEGINHISTORY        0x102C
#define T_ENDHISTORY          0x102D
#define T_INCLUDE             0x102E
#define T_INCLUDEEND          0x102F
#define T_INCLUDEPATH         0x1030


/* -- END TOKENS  */

#define ABC_F_CLEFSCANNED   0x0001
#define ABC_F_CHORDSCANNED  0x0001
#define ABC_F_ONETIMERS     0x00FF

#define abcSetFlag(s,f)  ((s)->flags |= (f))
#define abcClrFlag(s,f)  ((s)->flags &= ~(f))
#define abcTestFlag(s,f) ((s)->flags & (f))

typedef  unsigned short abcToken; 

/* must be abc_MAXTOKSTR >= abcPMXCAPT */
#define abc_PMXCAPT     10
#define abc_MAXTOKSTR   abc_PMXCAPT

typedef struct abc_scanner {
  struct abc_scanner *saved;
  char  *abc;
  char  *pos;
  FILE  *file;
  char  *line;
  char  *cur;
  char  *tmpbuf;
  char  *incpath;
  unsigned short state;
  unsigned short nextstate;
  unsigned short prevstate;
  unsigned short lnumber;
  unsigned short ln_logical;
  unsigned short flags;
  abcToken lasttok;
  abcToken tok;
  char *tok_str[abc_MAXTOKSTR][2]; /* begin, end */
} abcScanner;

abcScanner *abc_newscanner(char *text,char how);

#define abcNew_file(s) abc_newscanner(s,'F')
#define abcNew_str(s)  abc_newscanner(s,'S')
#define abcNew_chs(s)  abc_newscanner(s,'C')

#define abcScannerNew(x,y) abcNew_##x(y)

abcScanner *abcScannerFree(abcScanner *scn);

char *abcScannerAddToIncludePath(abcScanner *scn,char *path);
char *abcScannerSetIncludePath(abcScanner *scn,char *path);
char *abcScannerGetIncludePath(abcScanner *scn);

abcToken abcNextToken(abcScanner *scn);

#define abcToken(s) ((s)->tok)
char *abcTokenStart(abcScanner *scn, int strnum);
char *abcTokenEnd(abcScanner *scn, int strnum);
int abcTokenLen(abcScanner *scn, int strnum);

#define abcScannerCurState(s) ((s)->state)
#define abcScannerNextState(s) ((s)->nextstate)
#define abcScannerLineNumber(s) ((s)->ln_logical)

unsigned short abcScannerSetState(abcScanner *scn, unsigned short state);

int abcInclude(abcScanner *scn);


/********/

typedef unsigned short abcFraction;

#define abcNumerator(x)   (((x) & 0xFF00) >> 8)
#define abcDenominator(x)  ((x) & 0x00FF)
abcFraction abc_getfraction(abcScanner *scn,int ndx);

char abcField(abcScanner *scn);

/****** */

/* abcpnote.c */

#define abcNoteFlats(scn)     (abcNoteAccidentals(scn) & 0x000F)
#define abcNoteSharps(scn)   ((abcNoteAccidentals(scn) & 0x00F0) >> 4)
#define abcNoteNaturals(scn) ((abcNoteAccidentals(scn) & 0x0F00) >> 8)

#define abcNoteCourtesyAccidentals(scn) (abcNoteAccidentals(scn) & 0x1000)

unsigned short abcNoteAccidentals(abcScanner *scn);

abcFraction abcNoteDuration(abcScanner *scn);
abcFraction abcNoteMicrotone(abcScanner *scn);
unsigned short abcNoteOctave(abcScanner *scn);
unsigned char abcNotePitch(abcScanner *scn);
unsigned short abcNoteCents(abcScanner *scn);

unsigned short abcNoteMidi(abcScanner *scn);

unsigned short abcRestInvisible(abcScanner *scn);
unsigned short abcRestMultimeasure(abcScanner *scn);
abcFraction abcRestDuration(abcScanner *scn);

char *abcBarStart(abcScanner *scn);
int abcBarLen(abcScanner *scn);
int abcBarDotted(abcScanner *scn);
int abcBarInvisible(abcScanner *scn);
int abcBarRepeatBefore(abcScanner *scn);
int abcBarRepeatAfter(abcScanner *scn);
char *abcBarEndingStart(abcScanner *scn);
int abcBarEndingLen(abcScanner *scn);

int abcBroken(abcScanner *scn);
abcFraction abcChordDuration(abcScanner *scn);

int abcOverlay(abcScanner *scn);
char abcSlurDirection(abcScanner *scn);
char abcTieDirection(abcScanner *scn);
int abcSlurDotted(abcScanner *scn);
int abcTieDotted(abcScanner *scn);

int abcTupletNext(abcScanner *scn);
int abcTupletTime(abcScanner *scn);
int abcTuplet(abcScanner *scn);
            
/* abcptext.c */

char *abcAnnotationTextStart(abcScanner *scn);
int abcAnnotationTextLen(abcScanner *scn);
float abcAnnotationPosX(abcScanner *scn);
float abcAnnotationPosY(abcScanner *scn);
char abcAnnotationPos(abcScanner *scn);

char abcTextType(abcScanner *scn);
int abcTextLen(abcScanner *scn);
char *abcTextStart(abcScanner *scn);
char abcTextNL(abcScanner *scn);

char *abcDecorationStart(abcScanner *scn);
int abcDecorationLen(abcScanner *scn);
int abcDecorationSpan(abcScanner *scn);

char *abcPragmaStart(abcScanner *scn);
int abcPragmaLen(abcScanner *scn);
char *abcPragmaArgsStart(abcScanner *scn);
int abcPragmaArgsLen(abcScanner *scn);

float abcSpacer(abcScanner *scn);

char *abcSyllableStart(abcScanner *scn);
int abcSyllableLen(abcScanner *scn);
int abcSyllableContinue(abcScanner *scn);
int abcSyllableHold(abcScanner *scn);
int abcSyllableBlank(abcScanner *scn);

int abcVerse(abcScanner *scn);
char abcVerseSeparator(abcScanner *scn);
char abcUserSymbol(abcScanner *scn);
char *abcUserSymbolDefault(abcScanner *scn);
char *abcUserSymbolDefineStart(abcScanner *scn);
int abcUserSymbolDefineLen(abcScanner *scn);


/* abcpchord.pmx */

char *abcChordRootStart(abcScanner *scn);
char *abcChordTypeStart(abcScanner *scn);
char *abcChordBassStart(abcScanner *scn);
char *abcChordAltRootStart(abcScanner *scn);
char *abcChordAltTypeStart(abcScanner *scn);
char *abcChordAltBassStart(abcScanner *scn);

int abcChordRootLen(abcScanner *scn);
int abcChordTypeLen(abcScanner *scn);
int abcChordBassLen(abcScanner *scn);
int abcChordAltRootLen(abcScanner *scn);
int abcChordAltTypeLen(abcScanner *scn);
int abcChordAltBassLen(abcScanner *scn);


/* abcpkey.pmx */

#define ABC_Tonic                    3
#define ABC_VoiceId                  4
#define ABC_Name                     5 
#define ABC_ShortName                6 
#define ABC_Clef                     7
 
#define ABC_Transpose               0 
#define ABC_Octave                  1
#define ABC_Stafflines              2
#define ABC_Middle                  3
#define ABC_Stems                   4
#define ABC_Gstems                  5
#define ABC_Space                   6
#define ABC_Staves                  7
#define ABC_Brace                   8
#define ABC_Bracket                 9 
#define ABC_Program                10
#define ABC_ProgramChannel         11
#define ABC_Merge                  12
#define ABC_Tune                   13
#define ABC_Mute                   14
#define ABC_Pan                    15
#define ABC_Volume                 16
#define ABC_Longbar                17
#define ABC_Gchord                 18
#define ABC_Mode                   19 


char *abcKVStringStart(abcScanner *scn,int str, char kv);
int abcKVStringLen(abcScanner *scn,int str, char kv);
short abcKVParm(abcScanner *scn,int p,char kv);
short abcKVTranspose(abcScanner *scn, char kv);
char *abcKVExp(abcScanner *scn,char kv);

#define abcKeyTonicStart(s)        abcKVStringStart(s,ABC_Tonic,'K')        
#define abcKeyTonicLen(s)          abcKVStringLen(s,ABC_Tonic,'K')                                          
#define abcKeyClefStart(s)         abcKVStringStart(s,ABC_Clef,'K')           
#define abcKeyClefLen(s)           abcKVStringLen(s,ABC_Clef,'K')
                                   
#define abcKeyMode(s)              abcKVParm(s,ABC_Mode,'K')
#define abcKeyStafflines(s)        abcKVParm(s,ABC_Stafflines,'K')
#define abcKeyMiddle(s)            abcKVParm(s,ABC_Middle,'K')
#define abcKeyStems(s)             abcKVParm(s,ABC_Stems,'K')
#define abcKeyGstems(s)            abcKVParm(s,ABC_Gstems,'K')
#define abcKeySpace(s)             abcKVParm(s,ABC_Space,'K')
#define abcKeyStaves(s)            abcKVParm(s,ABC_Staves,'K')            
#define abcKeyBrace(s)             abcKVParm(s,ABC_Brace,'K')             
#define abcKeyBracket(s)           abcKVParm(s,ABC_Bracket,'K')           
#define abcKeyProgram(s)           abcKVParm(s,ABC_Program,'K')           
#define abcKeyProgramChannel(s)    abcKVParm(s,ABC_ProgramChannel,'K')    
#define abcKeyMerge(s)             abcKVParm(s,ABC_Merge,'K')             
#define abcKeyTune(s)              abcKVParm(s,ABC_Tune,'K')              
#define abcKeyMute(s)              abcKVParm(s,ABC_Mute,'K')              
#define abcKeyPan(s)               abcKVParm(s,ABC_Pan,'K')               
#define abcKeyVolume(s)            abcKVParm(s,ABC_Volume,'K')            
#define abcKeyLongbar(s)           abcKVParm(s,ABC_Longbar,'K')           
#define abcKeyGchord(s)            abcKVParm(s,ABC_Gchord,'K')
#define abcKeyAccidentals(s)       abcKVExp(s,'K')
            
#define abcKeyTranspose(s)         abcKVTranspose(s,'K')

#define abcVoiceIdStart(s)           abcKVStringStart(s,ABC_VoiceId,'V')           
#define abcVoiceIdLen(s)             abcKVStringLen(s,ABC_VoiceId,'V')
#define abcVoiceNameStart(s)         abcKVStringStart(s,ABC_Name,'V')           
#define abcVoiceNameLen(s)           abcKVStringLen(s,ABC_Name,'V')
#define abcVoiceShortNameStart(s)    abcKVStringStart(s,ABC_ShortName,'V')           
#define abcVoiceShortNameLen(s)      abcKVStringLen(s,ABC_ShortName,'V')                                   
#define abcVoiceClefStart(s)         abcKVStringStart(s,ABC_Clef,'V')           
#define abcVoiceClefLen(s)           abcKVStringLen(s,ABC_Clef,'V')
#define abcVoiceMode(s)              abcKVParm(s,ABC_Mode,'V')
#define abcVoiceStafflines(s)        abcKVParm(s,ABC_Stafflines,'V')
#define abcVoiceMiddle(s)            abcKVParm(s,ABC_Middle,'V')
#define abcVoiceStems(s)             abcKVParm(s,ABC_Stems,'V')
#define abcVoiceGstems(s)            abcKVParm(s,ABC_Gstems,'V')
#define abcVoiceSpace(s)             abcKVParm(s,ABC_Space,'V')
#define abcVoiceStaves(s)            abcKVParm(s,ABC_Staves,'V')            
#define abcVoiceBrace(s)             abcKVParm(s,ABC_Brace,'V')             
#define abcVoiceBracket(s)           abcKVParm(s,ABC_Bracket,'V')           
#define abcVoiceProgram(s)           abcKVParm(s,ABC_Program,'V')           
#define abcVoiceProgramChannel(s)    abcKVParm(s,ABC_ProgramChannel,'V')    
#define abcVoiceMerge(s)             abcKVParm(s,ABC_Merge,'V')             
#define abcVoiceTune(s)              abcKVParm(s,ABC_Tune,'V')              
#define abcVoiceMute(s)              abcKVParm(s,ABC_Mute,'V')              
#define abcVoicePan(s)               abcKVParm(s,ABC_Pan,'V')               
#define abcVoiceVolume(s)            abcKVParm(s,ABC_Volume,'V')            
#define abcVoiceLongbar(s)           abcKVParm(s,ABC_Longbar,'V')           
#define abcVoiceGchord(s)            abcKVParm(s,ABC_Gchord,'V')
#define abcVoiceAccidentals(s)       abcKVExp(s,'V')
            
#define abcVoiceTranspose(s)         abcKVTranspose(s,'V')
 
#endif

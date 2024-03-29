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
/*
#define T_EXTFIELD            0x1005 
#define T_INFIELD             0x1006 

*/
#define T_NONE                0x1000 
#define T_UNKNOWN             0x1001 
#define T_EOF                 0x1002 
#define T_FIELD               0x1004 
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
#define T_LYRBAR              0x1031
#define T_SYMBAR              0x1032


/* -- END TOKENS  */

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

#define     abcScannerNew(x,y) abcNew_##x(y)
abcScanner *abcScannerFree(abcScanner *scn);

#define     abcScannerLineNumber(s) ((s)->ln_logical)

char *abcScannerGetIncludePath(abcScanner *scn);
char *abcScannerSetIncludePath(abcScanner *scn,char *path);
char *abcScannerAddToIncludePath(abcScanner *scn,char *path);

int   abcInclude(abcScanner *scn);
char *abcIncludePathStart(abcScanner *scn);
int   abcIncludePathLen(abcScanner *scn);
char *abcIncludePathSet(abcScanner *scn);
int   abcIncludePathAdding(abcScanner *scn);

abcToken abcNextToken(abcScanner *scn);

#define abcToken(s) ((s)->tok)
char   *abcTokenStart(abcScanner *scn, int strnum);
char   *abcTokenEnd(abcScanner *scn, int strnum);
int     abcTokenLen(abcScanner *scn, int strnum);

#define abcStateCurrent(s) ((s)->state)
#define abcStateNext(s) ((s)->nextstate)
#define abcStateSet(s,t) ((s)->nextstate = (t))



/********/
float abc_getfracion(char *a, int al, char *b, int bl, char *c, int cl);


/* abcpfield.pmx */

char  abcField(abcScanner *scn); 
int   abcFieldLen(abcScanner *scn);
char *abcFieldStart(abcScanner *scn);

/****** */

/* abcpnote.c */

extern char abcSemitones[7];

#define abcNote2Num(c) ((tolower(c)-'c'+7) % 7)
unsigned short abcNote2Midi(char pitch, int octave, int acc);

char abcNote(abcScanner *scn);

int            abcNoteAccidentalCourtesy(abcScanner *scn);
int            abcNoteAccidentalLen(abcScanner *scn);
char          *abcNoteAccidentalStart(abcScanner *scn);

float          abcNoteDuration(abcScanner *scn);
int            abcNoteOctave(abcScanner *scn);
unsigned char *abcNotePitch(abcScanner *scn);

float          abcNoteCents(abcScanner *scn);
float          abcNoteBending(abcScanner *scn);

float abcNoteMicrotoneDen(abcScanner *scn);
float abcNoteMicrotoneNum(abcScanner *scn);

unsigned short abcNoteMidi(abcScanner *scn);
unsigned short abcNoteMidiPitchBend(abcScanner *scn);

unsigned short abcRestInvisible(abcScanner *scn);
unsigned short abcRestMultimeasure(abcScanner *scn);
float          abcRestDuration(abcScanner *scn);


char *abcBarStart(abcScanner *scn);
int   abcBarLen(abcScanner *scn);
int   abcBarDotted(abcScanner *scn);
int   abcBarInvisible(abcScanner *scn);
int   abcBarRepeatBefore(abcScanner *scn);
int   abcBarRepeatAfter(abcScanner *scn);
char *abcBarEndingStart(abcScanner *scn);
int   abcBarEndingLen(abcScanner *scn);
int   abcBarEnding(abcScanner *scn);

int   abcBroken(abcScanner *scn);
float abcChordDuration(abcScanner *scn);
int   abcChordOctave(abcScanner *scn);
char *abcChordFormula(abcScanner *scn);

int  abcOverlay(abcScanner *scn);

char abcSlurDirection(abcScanner *scn);
char abcTieDirection(abcScanner *scn);

int  abcSlurDotted(abcScanner *scn);
int  abcTieDotted(abcScanner *scn);

int  abcTupletNext(abcScanner *scn);
int  abcTupletTime(abcScanner *scn);
int  abcTuplet(abcScanner *scn);
            
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
extern int abc_tmpi;

#define ABC_Tonic                    4
#define ABC_VoiceId                  5
#define ABC_Name                     6 
#define ABC_ShortName                7 
#define ABC_Clef                     8
 
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

#define ABC_Mode_Ion 1
#define ABC_Mode_Dor 2
#define ABC_Mode_Phr 3
#define ABC_Mode_Lyd 4
#define ABC_Mode_Mix 5
#define ABC_Mode_Aeo 6
#define ABC_Mode_Loc 7

extern char *abc_modes;

char *abcKVStringStart(abcScanner *scn,int str);
int abcKVStringLen(abcScanner *scn,int str);
short abcKVParm(abcScanner *scn,int p);
short abcVoiceTranspose(abcScanner *scn);
float *abcKeySignature(abcScanner *scn);
float *abcKeyExpSignature(abcScanner *scn);
                            
#define abcNatural 9999.00
int abcKeyExpNatural(abcScanner *scn,char pitch);
float abcKeyExpBending(abcScanner *scn,char pitch);
                                   
#define abcKeyTonicStart(s)        abcKVStringStart(s,ABC_Tonic)      
#define abcKeyTonicLen(s)          abcKVStringLen(s,ABC_Tonic)                                          
#define abcKeyMode(s)              abc_modes[abcKVParm(s,ABC_Mode)&0x0F]
  
#define abcClefStart(s)             abcKVStringStart(s,ABC_Clef)           
#define abcClefLen(s)               abcKVStringLen(s,ABC_Clef)
#define abcVoiceMiddle(s)           abcKVParm(s,ABC_Middle)

#define abcVoiceIdStart(s)           abcKVStringStart(s,ABC_VoiceId)           
#define abcVoiceIdLen(s)             abcKVStringLen(s,ABC_VoiceId)
#define abcVoiceNameStart(s)         abcKVStringStart(s,ABC_Name)           
#define abcVoiceNameLen(s)           abcKVStringLen(s,ABC_Name)
#define abcVoiceShortNameStart(s)    abcKVStringStart(s,ABC_ShortName)           
#define abcVoiceShortNameLen(s)      abcKVStringLen(s,ABC_ShortName)

#define abcVoiceMute(s)              abcKVParm(s,ABC_Mute)                  
#define abcVoiceProgram(s)           abcKVParm(s,ABC_Program)           
#define abcVoiceProgramChannel(s)    abcKVParm(s,ABC_ProgramChannel)    
#define abcVoicePan(s)               abcKVParm(s,ABC_Pan)               
#define abcVoiceVolume(s)            abcKVParm(s,ABC_Volume)            
#define abcVoiceStems(s)             abcKVParm(s,ABC_Stems)
#define abcVoiceGstems(s)            abcKVParm(s,ABC_Gstems)
#define abcVoiceStafflines(s)        abcKVParm(s,ABC_Stafflines)

#define abcVoiceSpace(s)             abcKVParm(s,ABC_Space)
#define abcVoiceStaves(s)            abcKVParm(s,ABC_Staves)            
#define abcVoiceBrace(s)             abcKVParm(s,ABC_Brace)             
#define abcVoiceBracket(s)           abcKVParm(s,ABC_Bracket)           
#define abcVoiceMerge(s)             abcKVParm(s,ABC_Merge)             
#define abcVoiceLongbar(s)           abcKVParm(s,ABC_Longbar)          
#define abcKeyTune(s)                abcKVParm(s,ABC_Tune)              
#define abcVoiceGchord(s)            abcKVParm(s,ABC_Gchord)
            

/********* */

int abcMeterBeatsNth(abcScanner *scn,int k);
int abcMeterBeats(abcScanner *scn);
int abcMeterUnits(abcScanner *scn);

char *abcMeterBeatsStart(abcScanner *scn);
int   abcMeterBeatsLen(abcScanner *scn);
char *abcMeterUnitsStart(abcScanner *scn);
int   abcMeterUnitsLen(abcScanner *scn);
char *abcMeterExplicitBeatsStart(abcScanner *scn);
int   abcMeterExplicitBeatsLen(abcScanner *scn);
char *abcMeterExplicitUnitsStart(abcScanner *scn);
int   abcMeterExplicitUnitsLen(abcScanner *scn);

/* in abcpfield.pmx */

long abcTempoDenNth(abcScanner *scn,int n);
long abcTempoNumNth(abcScanner *scn,int n);
int abcTempoDen(abcScanner *scn);
int abcTempoNum(abcScanner *scn);
int abcTempoBeats(abcScanner *scn);
int abcTempoLen(abcScanner *scn);
char *abcTempoStart(abcScanner *scn);


#endif

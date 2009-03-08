/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This sofwtare is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
*/

#include "libutl.h"
#include "abcp.h"

int main(int argc, char *argv[])
{
  abcScanner *scn;
  abcToken tok;
  chs_t abctext = NULL;
  int k = S_NONE; 
  int j;
  FILE *f;
  int argn;
  
  argn = 1;

  if (argn < argc && argv[1][0]=='-') {
    k = atoi(argv[1]+1);
    argn++;
  }
   
  f = stdin;
  if (argn < argc) {
    f = fopen(argv[argn],"rb");
    if (!f) {
      fprintf(stderr,"Unable to open input file\n");
      exit(1);
    }
  }
  
  abctext = chsRead(abctext,f,'w');
  if (f!=stdin) fclose(f);
  if (abctext && *abctext) {
    scn = abcScannerNew(str,abctext);
    abcStateSet(scn,k);
    /*printf("<<\n%s\n>>\n",scn->abc);*/
    while ((tok = abcNextToken(scn)) != T_EOF) {
      printf("%04d %04X %04X ",abcScannerLineNumber(scn), abcStateCurrent(scn), abcToken(scn));
      switch (tok) {
        case T_ANNOTATION:
             printf("ANNOTATION: ");
             printf("pos: %c ",abcAnnotationPos(scn));
             printf("x: %.2f y: %.2f ",abcAnnotationPosX(scn), abcAnnotationPosY(scn));
             printf("\n");
             printf("               text: %.*s",abcAnnotationTextLen(scn),
                                                  abcAnnotationTextStart(scn));
             printf("\n");
             break;

        case T_UNKNOWN    : printf("UNKNOWN: "); 
             printf("[%.*s]\n", abcTokenLen(scn,0),abcTokenStart(scn,0));
             break;

        case T_TEXT       : printf("TEXT: "); goto thetext;
        case T_ENDTEXT    : printf("ENDTEXT: "); goto thetext;
        case T_BEGINTEXT  : printf("BEGINTEXT: "); goto thetext;
        case T_EMPTYLINE  : printf("EMPTYLINE: "); goto thetext;
        case T_WHITESPACE : printf("WHITESPACE: "); goto thetext;
        case T_IGNORE     : printf("IGNORE: "); goto thetext;
        thetext:    
             printf("type: '%c' [%.*s]",abcTextType(scn),abcTextLen(scn),
                                                  abcTextStart(scn));
             if (abcTextNL(scn)) printf(" \\n");
             printf("\n");
             break;
             
        case T_SYMBAR     : printf("SYMBAR:\n"); break;
        case T_LYRBAR     : printf("LYRBAR:\n"); break;
        
        case T_BAR:
             printf("BAR: ");
             if (abcBarInvisible(scn)) printf("invisible ");
             if (abcBarDotted(scn))   printf("dotted ");
             printf("%d '%.*s' %d", abcBarRepeatBefore(scn),
                                             abcBarLen(scn), abcBarStart(scn),
                                             abcBarRepeatAfter(scn));
             printf("\n");
             break;
 
        case T_SYMBOLS:
             printf("SYMBOLS:\n");
             break;
             
        case T_LYRICS:
             printf("LYRICS:\n");
             break;
             
        case T_CHORD:
             printf("CHORD:\n");
             break;
             
        case T_CHORDEND:
             printf("CHORDEND: ");
             printf(" %.2f\n",abcChordDuration(scn));
             break;
             
        case T_SPACER:
             printf("SPACER: %f\n",abcSpacer(scn));
             break;
             
        case T_CONTINUE:
             printf("CONTINUE:\n");
             break;
             
        case T_DECORATION:
             printf("DECORATION: "); 
             printf("[%.*s]",abcDecorationLen(scn),abcDecorationStart(scn));
             switch (abcDecorationSpan(scn)) {
               case '(' : printf(" START"); break;
               case ')' : printf(" END"); break;
             }
             printf("\n");
             break;
             
        case T_PRAGMA:
             printf("PRAGMA: "); 
             printf("[%.*s]",abcPragmaLen(scn),abcPragmaStart(scn));
             printf("[%.*s]",abcPragmaArgsLen(scn),abcPragmaArgsStart(scn));
             printf("\n");
             break;
             
        case T_SLUREND:
             printf("SLUREND:\n");
             break;
             
        case T_TIE:
             printf("TIE: %c", abcTieDirection(scn));
             printf(" %sdotted\n", abcTieDotted(scn)?"":"un");
             break;
             
        case T_SLURSTART:
             printf("SLURSTART: %c", abcSlurDirection(scn));
             printf(" %sdotted\n", abcSlurDotted(scn)?"":"un");
             break;
             
        case T_BREAKLINE:
             printf("BREAKLINE:\n");
             break;
             
        case T_ENDING :
             printf("ENDING: %d %.*s\n", abcBarEnding(scn), abcBarEndingLen(scn), abcBarEndingStart(scn));
             break;
             
        case T_OVLRESET:
             printf("T_OVLRESET: %d\n", abcOverlay(scn));
             break;
             
        case T_OVLSTART:
             printf("T_OVLSTART: %d\n", abcOverlay(scn));
             break;
             
        case T_OVLEND:
             printf("T_OVLEND: %d\n", abcOverlay(scn));
             break;
        
        case T_BROKENLEFT :
             printf("BROKENLEFT: %d\n", abcBroken(scn));
             break;
             
        case T_BROKENRIGHT :
             printf("BROKENRIGHT: %d\n", abcBroken(scn));
             break;
             
        case T_USERSYMBOL :
             printf("USERSYMBOL: %c %s\n", abcUserSymbol(scn),abcUserSymbolDefault(scn));
             break;
             
        case T_VERSE:
             printf("VERSE: [%d][%c]\n", abcVerse(scn),abcVerseSeparator(scn));
             break;
             
        case T_SYLLABLE:
             printf("SYLLABLE: [%.*s] %d%s%s",abcSyllableLen(scn),
                  abcSyllableStart(scn), abcSyllableHold(scn), 
                  (abcSyllableContinue(scn)?" CONTINUE":""),
                  (abcSyllableBlank(scn)?" BLANK":""));
             printf("\n");
             break;
             
        case T_TUPLET:
             printf("TUPLET: %d:%d:%d\n",abcTuplet(scn),abcTupletTime(scn),abcTupletNext(scn));
             break;
             
        case T_ENDLINE:
             printf("ENDLINE:\n");
             break;
             
        case T_GCHORD:
             printf("GCHORD: ");
             printf("[%.*s]",abcChordRootLen(scn),abcChordRootStart(scn));
             printf("[%.*s]",abcChordTypeLen(scn),abcChordTypeStart(scn));
             printf("[%.*s]",abcChordBassLen(scn),abcChordBassStart(scn));
             printf("[%.*s]",abcChordAltRootLen(scn),abcChordAltRootStart(scn));
             printf("[%.*s]",abcChordAltTypeLen(scn),abcChordAltTypeStart(scn));
             printf("[%.*s]",abcChordAltBassLen(scn),abcChordAltBassStart(scn));
             printf(" (%d)",abcChordOctave(scn));
             printf(" %d",*abcChordFormula(scn));
             printf("\n");
             break; 
             
        case T_NOTE:
             printf("NOTE: ");
             printf("%s%s ", abcNotePitch(scn),abcNoteBending(scn) == abcNatural?" (natural)":"");
             printf("bend: %.2f (%.2f/%.2f) ",abcNoteBending(scn),abcNoteMicrotoneNum(scn),abcNoteMicrotoneDen(scn));
             printf("court: %d\n",abcNoteCourtesyAccidentals(scn));
             printf("               duration: %.2f ",abcNoteDuration(scn));
             printf("octave: %d ",abcNoteOctave(scn));
             printf("midi: (%d,%d)\n",abcNoteMidi(scn),abcNoteMidiPitchBend(scn));
             break;
              
        case T_REST:
             printf("REST: %.2f%s\n", abcRestDuration(scn),
                                       abcRestMultimeasure(scn)?" measures":"");
             break;
             
        case T_INCLUDE:
             printf("INCLUDE: %.*s%.*s", abcTokenLen(scn,1),abcTokenStart(scn,1),
                                         abcTokenLen(scn,2),abcTokenStart(scn,2));                                             
             printf(" %s\n", abcInclude(scn)? "OK":"FAILED");
             break;
        
        case T_INCLUDEEND:
             printf("INCLUDEEND\n");
             break;
        
        case T_INCLUDEPATH:
             printf("INCLUDEPATH: %s%.*s\n",abcIncludePathAdding(scn)?"+ ":"",
                               abcIncludePathLen(scn),abcIncludePathStart(scn));
             printf("               current: %s\n",abcIncludePathSet(scn));
             break;
        
        case T_FIELD:
             switch (abcField(scn)) { 
               case 'V' : 
               case 'K' : 
                 printf("VOICE: %.*s ", abcVoiceIdLen(scn), abcVoiceIdStart(scn));
                 printf("KEY: %.*s ", abcKeyTonicLen(scn), abcKeyTonicStart(scn));
                 printf("mode: %c\n", abcKeyMode(scn));
                 printf("               sig: ");
                 printf("%6.2f", abcKeySignature(scn)[0]);
                 for (k=1; k<7; k++) {
                   printf(", %6.2f", abcKeySignature(scn)[k]);
                 }
                 printf("\n"); 
                 printf("               exp: ");
                 printf("%6.2f", abcKeyExpSignature(scn)[0]);
                 for (k=1; k<7; k++) {
                   printf(", %6.2f", abcKeyExpSignature(scn)[k]);
                 }
                 printf("\n");
               /*  printf(" accidentals: <%s>", abcKeyAccidentals(scn));*/
    
                 printf("               transpose: %d", abcVoiceTranspose(scn));
                 printf(" clef: %.*s", abcClefLen(scn), abcClefStart(scn));
                 printf(" stafflines: %d\n",abcVoiceStafflines(scn));       
                 printf("               middle: %c",abcVoiceMiddle(scn));           
                 printf(" stems: %d",abcVoiceStems(scn));            
                 printf(" gstems: %d",abcVoiceGstems(scn));           
                 printf(" spaces: %d",abcVoiceSpace(scn));            
                 printf(" staves: %d",abcVoiceStaves(scn));           
                 printf(" brace: %d\n",abcVoiceBrace(scn));            
                 printf("               bracket: %d",abcVoiceBracket(scn));
                 printf(" program: %d",abcVoiceProgram(scn));          
                 printf(" prog ch.: %d",abcVoiceProgramChannel(scn));   
                 printf(" merge: %d",abcVoiceMerge(scn));            
                 printf(" mute: %d\n",abcVoiceMute(scn));             
                 printf("               pan: %d",abcVoicePan(scn));              
                 printf(" volume: %d",abcVoiceVolume(scn));           
                 printf(" longbar: %d",abcVoiceLongbar(scn));          
                 printf(" gchord: %d\n",abcVoiceGchord(scn));           
                 break;
                 
                 
               case 'U' :
                 printf("USRSYMDEF: [%c][%.*s]\n", abcUserSymbol(scn),
                          abcUserSymbolDefineLen(scn),
                          abcUserSymbolDefineStart(scn));
                 break;
                 
               case 'M' :
                 printf("METER: %d/%d ", abcMeterBeats(scn), abcMeterUnits(scn));
                 printf("(%d",abcMeterBeatsNth(scn,1));
                 k=2;

                 while ((j = abcMeterBeatsNth(scn,k++)) != 0) 
                   printf("+%d",j);
                   
                 printf(")\n");
                 break;
                 
               default:
                 printf("FIELD: [%c][%.*s]", abcField(scn), abcFieldLen(scn),abcFieldStart(scn));
                 printf("\n");
             }
             break; 
        
        default:
             printf("%.*s\n", abcTokenLen(scn,0),abcTokenStart(scn,0));
      }
    }
    scn = abcScannerFree(scn);
  }
  abctext = chsFree(abctext);
  return(0);
}

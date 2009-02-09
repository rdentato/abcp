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
  int k; 
  
  abctext = chsRead(abctext,stdin,'w');
  if (abctext && *abctext) {
    scn = abcNewScanner(str,abctext);
    /*printf("<<\n%s\n>>\n",scn->abc);*/
    while ((tok = abcNextToken(scn)) != T_EOF) {
      printf("%04d %04X %04X ",abcLineNumber(scn), abcCurState(scn), abcToken(scn));
      switch (tok) {
        case T_ANNOTATION:
             printf("ANNOTATION: ");
             printf("pos: %c ",abcAnnotationPos(scn));
             printf("x: %f y: %f ",abcAnnotationPosX(scn), abcAnnotationPosY(scn));
             printf("\n");
             printf("               text: %.*s",abcAnnotationTextLen(scn),
                                                  abcAnnotationTextStart(scn));
             printf("\n");
             break;

        case T_COMMENT:
             printf("COMMENT: ");
             printf("Inline: %c ",abcCommentInline(scn)? 'Y' : 'N');
             printf("\n");
             printf("               text: %.*s",abcCommentLen(scn),
                                                  abcCommentStart(scn));
             printf("\n");
             break;
             
        case T_BAR:
             printf("BAR: (");
             if (abcBarInvisible(scn)) 
               printf("in");
             printf("visible");
             if (abcBarDotted(scn))
               printf(", dotted");
             printf(") %d '%.*s' %d", abcBarRepeatBefore(scn),
                                             abcBarLen(scn), abcBarStart(scn),
                                             abcBarRepeatAfter(scn));
             printf("\n");
             break;
             
        case T_NOTE:
             printf("NOTE: ");
             printf("%c", abcNotePitch(scn));
             printf("%.*s", abcNoteSharps(scn), "####");
             printf("%.*s", abcNoteFlats(scn), "bbbb");
             printf("\n");
             printf("               flats: %d\n",abcNoteFlats(scn));
             printf("               sharp: %d\n",abcNoteSharps(scn));
             printf("               naturals: %d\n",abcNoteNaturals(scn));
             printf("               courtesy: %d\n",abcNoteCourtesyAccidentals(scn));
             k = abcNoteDuration(scn);
             printf("               duration: %d/%d\n",abcNumerator(k),abcDenominator(k));
             printf("               octave: %d\n",abcNoteOctave(scn));
             printf("               midi: %d\n",abcNoteMidi(scn));
             k = abcNoteMicrotone(scn);
             printf("               microtones: %d/%d\n",abcNumerator(k),abcDenominator(k));
             printf("               cents: %d\n",abcNoteCents(scn));
             break;
             
        case T_INFIELD:
        case T_FIELD:
             switch (*abcTokenStart(scn,1)) {
               case 'K' : 
                 printf("KEY: %.*s\n", abcKeyTonicLen(scn), abcKeyTonicStart(scn));
                 printf("               mode: %c\n", abcKeyMode(scn));
                 printf("               accidentals: <%s>\n", abcKeyAccidentals(scn));
                 printf("               transpose: %d\n", abcKeyTranspose(scn));
                 printf("               clef: %.*s\n", abcKeyClefLen(scn), abcKeyClefStart(scn));
                 printf("               stafflines: %d\n",abcKeyStafflines(scn));       
                 printf("               middle: %c\n",abcKeyMiddle(scn));           
                 printf("               stems: %c\n",abcKeyStems(scn));            
                 printf("               gstems: %c\n",abcKeyGstems(scn));           
                 printf("               spaces: %d\n",abcKeySpace(scn));            
                 printf("               staves: %d\n",abcKeyStaves(scn));           
                 printf("               brace: %d\n",abcKeyBrace(scn));            
                 printf("               bracket: %d\n",abcKeyBracket(scn));
                 printf("               program: %d\n",abcKeyProgram(scn));          
                 printf("               prog ch.: %d\n",abcKeyProgramChannel(scn));   
                 printf("               merge: %c\n",abcKeyMerge(scn));            
                 printf("               tune: %d\n",abcKeyTune(scn));             
                 printf("               mute: %c\n",abcKeyMute(scn));             
                 printf("               pan: %d\n",abcKeyPan(scn));              
                 printf("               volume: %d\n",abcKeyVolume(scn));           
                 printf("               longbar: %c\n",abcKeyLongbar(scn));          
                 printf("               gchord: %c\n",abcKeyGchord(scn));           
                 break;
                 
               case 'V' : 
                 printf("VOICE: %.*s\n", abcVoiceIdLen(scn), abcVoiceIdStart(scn));
                 printf("               name: %.*s\n", abcVoiceNameLen(scn), abcVoiceNameStart(scn));
                 printf("               shortname: %.*s\n", abcVoiceShortNameLen(scn), abcVoiceShortNameStart(scn));
                 printf("               mode: %c\n", abcVoiceMode(scn));
                 printf("               accidentals: <%s>\n", abcVoiceAccidentals(scn));
                 printf("               transpose: %d\n", abcVoiceTranspose(scn));
                 printf("               clef: %.*s\n", abcVoiceClefLen(scn), abcVoiceClefStart(scn));
                 printf("               stafflines: %d\n",abcVoiceStafflines(scn));       
                 printf("               middle: %c\n",abcVoiceMiddle(scn));           
                 printf("               stems: %c\n",abcVoiceStems(scn));            
                 printf("               gstems: %c\n",abcVoiceGstems(scn));           
                 printf("               spaces: %d\n",abcVoiceSpace(scn));            
                 printf("               staves: %d\n",abcVoiceStaves(scn));           
                 printf("               brace: %d\n",abcVoiceBrace(scn));            
                 printf("               bracket: %d\n",abcVoiceBracket(scn));
                 printf("               program: %d\n",abcVoiceProgram(scn));          
                 printf("               prog ch.: %d\n",abcVoiceProgramChannel(scn));   
                 printf("               merge: %c\n",abcVoiceMerge(scn));            
                 printf("               tune: %d\n",abcVoiceTune(scn));             
                 printf("               mute: %c\n",abcVoiceMute(scn));             
                 printf("               pan: %d\n",abcVoicePan(scn));              
                 printf("               volume: %d\n",abcVoiceVolume(scn));           
                 printf("               longbar: %c\n",abcVoiceLongbar(scn));          
                 printf("               gchord: %c\n",abcVoiceGchord(scn));           
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

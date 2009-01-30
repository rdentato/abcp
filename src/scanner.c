#include "abcp_.h"

void ABCAPI abcClose(abcScanner *s)
{
  YYSTREAM *ys;
  if (abcGoodScanner(s)) {
    while (s->stream_ndx > 0) {
      ys=s->streams[--s->stream_ndx];
      if (ys) YYCLOSE(ys);
    }
    s->stream=NULL;
    s->_guard=0xBAAD;
    free(s);
  }
}

static YYSTREAM *abc_addstream(abcScanner *s, unsigned char *string, unsigned char *fname, unsigned long bufsize)
{
  YYSTREAM *ys=NULL;
  
  if (s->stream_ndx < MAX_NESTED_INC) {
    if (string != NULL) 
      ys=YYSTRING(string);
    else
      ys=YYFILE(filename,bufsize);
      
    if (ys) {
      s->states[s->stream_ndx]=s->state;
      s->streams[s->stream_ndx++]=ys;
      s->stream = ys;
    }
  }
  return ys;
}

static YYSTREAM *abc_delstream(abcScanner *s)
{
  YYSTREAM *ys=NULL;
  if (s) {
    if (s->stream_ndx > 1) {
      s->stream_ndx--;
      s->state=s->states[s->stream_ndx];
      ys=s->streams[s->stream_ndx];
      if (ys) YYCLOSE(ys);
      s->streams[s->stream_ndx] = NULL;
      s->stream =s->streams[s->stream_ndx-1];
      s->nextstate=S_NONE;
      s->token=T_INCLUDE_EOF;
      ys=s->stream;
    }
  }
  
  return ys;
}


static abcScanner *newscanner(char *string,char *fname,bufsize)
{
  abcScanner *s=NULL;
  
  s=malloc(sizeof(abcScanner));
  if (s) {  
    s->_guard        = 0xF00D;
    s->state         = S_LIMBO;
    s->nextstate     = S_NONE;
    s->oldstate      = S_NONE;
    s->token->code   = T_UNKNOWN;
    s->token->start  = NULL;
    s->token->len    = 0;
    
    abc_addstream(s,string,filename,bufsize);
      
    if (s->stream == NULL) {
      free(s);
      s=NULL;
    }
  }                  
  return s;
}

abcScanner * ABCAPI abcFileScanner(char *fname, unsigned long bufsize)
{ return newscanner(NULL,fname,bufsize); } 

abcScanner * ABCAPI abcStringScanner(char *string)
{ return newscanner(string,NULL,0); }


int abcInclude(abcScanner *s,unsigned char *str,unsigned short len)
{
  char fname[512];
  char *t=fname;
  
  if (len > 511) len=511;
  while (len>0) { *t++=*str++; len--;}
  *t='\0';
  abc_addstream(s, NULL,fname,s->stream->size);
  return 0;
}

void ABCAPI abcStopInclude(abcScanner *s)
{ if (s->nextstate == S_INCLUDE) s->nextstate=S_NONE; }

const char * ABCAPI abcFileName(abcScanner *s)
{ return YYFILENAME(s->stream); }

unsigned int ABCAPI abcLineNum(abcScanner *s)
{ return YYLINENUMBER(s->stream); }

abcToken * ABCAPI abcCurToken(abcScanner *s)
{ return s->token; }


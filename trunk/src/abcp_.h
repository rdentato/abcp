
#include <stdio.h>
#include <stdlib.h>
#include "yrx.h


#define MAX_NESTED_INC 8

typedef struct {
  unsigned short   _guard;
  unsigned short   state;
  unsigned short   nextstate;
  
  unsigned short   token;
  unsigned char   *tokstart;
  unsigned short   toklen;
  
  unsigned short   stream_ndx;
  unsigned short   states[MAX_NESTED_INC];
  YYSTREAM        *streams[MAX_NESTED_INC];
  YYSTREAM        *stream;
  
} abcScanner;

#define abcGoodScanner(s) ((s) && ((s)->_guard == 0xF00D))

#define abc_START(_s,_n) YYSTART((_s)->stream,_n)
#define abc_END(_s,_n)   YYEND((_s)->stream,_n)
#define abc_LEN(_s,_n)   YYLEN((_s)->stream,_n)


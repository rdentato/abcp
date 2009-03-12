/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
** 
** This sofwtare is distributed under the terms of the BSD license:
**   http://creativecommons.org/licenses/BSD/
**   http://opensource.org/licenses/bsd-license.php 
*/

#ifndef ABCP_PRIV_H
#define ABCP_PRIV_H

#include "libutl.h"
#include "abcp.h"
#include <ctype.h>


#define ABC_F_CLEFSCANNED   0x0001
#define ABC_F_CHORDSCANNED  0x0001
#define ABC_F_FIELDSCANNED  0x0001
#define ABC_F_BENDCOMPUTED  0x0001
#define ABC_F_ONETIMERS     0x00FF

#define abcSetFlag(s,f)  ((s)->flags |= (f))
#define abcClrFlag(s,f)  ((s)->flags &= ~(f))
#define abcTestFlag(s,f) ((s)->flags & (f))



float abc_getfracion(char *a, int al,
                     char *b, int bl,
                     char *c, int cl);
                     
float abc_notebending(char *e, int el, char *a, int al,
                      char *b, int bl, char *c, int cl);

#endif

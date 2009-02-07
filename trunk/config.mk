# 
#  (C) by Remo Dentato (rdentato@gmail.com)
# 
# This sofwtare is distributed under the terms of the BSD license:
#   http://creativecommons.org/licenses/BSD/
#   http://opensource.org/licenses/bsd-license.php
#


SYS=LINUX

ifeq ($(OSTYPE),msys)
 SYS=MINGW
endif

#SYS=MINGW

################

_LIB=.a
_OBJ=.o
_EXE=

ifeq ($(SYS),MINGW)
_EXE=.exe
endif

AR=ar -ru
CAT=cat
RM=rm -f
CP=cp -f
LN=gcc $(LNFLAGS) -o

##################

S=/

DIST=$(TOP)dist
LIBNAME=libabcp$(_LIB)
CHKLIB=$(DIST)$(S)$(LIBNAME)$(_LIB)
UTL=$(TOP)tools$(S)libutl
PMX2C=$(UTL)$(S)pmx2c

CFLAGS=-I. -I$(DIST) -I$(UTL) -Wall $(CCOPTS)
LNFLAGS=-L. -L$(DIST) -L$(UTL) $(LNOPTS)

.SUFFIXES: .c .h .o .pmx

.c.o:
	$(CC) $(CFLAGS) -c -o $*.o $*.c
	
.pmx.o:
	$(PMX2C) $*.pmx $*.c
	$(CC) $(CFLAGS) -c -o $*.o $*.c
	$(RM) $*.c
	
###########
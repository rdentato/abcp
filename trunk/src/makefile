# 
#  (C) by Remo Dentato (rdentato@gmail.com)
# 
# This sofwtare is distributed under the terms of the BSD license:
#   http://creativecommons.org/licenses/BSD/
#   http://opensource.org/licenses/bsd-license.php
#

TOP=../

include $(TOP)config.mk

LIBSRC = abcpscan.pmx abcpnote.pmx abcpkey.pmx abcpcommon.c \
         abcptext.pmx abcpchords.pmx abcpfield.pmx
LIBHDR = abcp.h  abcp_priv.h 
LIBOBJ = abcpscan$(_OBJ) abcpnote$(_OBJ) abcpkey$(_OBJ) abcpcommon$(_OBJ) \
         abcptext$(_OBJ) abcpchords$(_OBJ)  abcpfield$(_OBJ) 

$(LIBNAME) : $(DIST)$(S)abcp.h $(LIBOBJ) 
	$(AR) $@ $(LIBOBJ) $(UTL)$(S)*$(_OBJ)
	$(CP) $@ $(DIST)

$(DIST)$(S)abcp.h: abcp.h
	$(CP) abcp.h $(DIST)

$(LIBSRC) : abcp.h

clean:
	$(RM) $(DIST)$(S)abcp.h
	$(RM) $(LIBNAME)
	$(RM) $(DIST)$(S)$(LIBNAME)
	$(RM) $(LIBOBJ)
	$(RM) gmon.out

include $(TOP)targets.mk

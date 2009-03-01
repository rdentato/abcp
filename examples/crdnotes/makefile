# 
#  (C) by Remo Dentato (rdentato@gmail.com)
# 
# This software is distributed under the terms of the BSD license:
#   http://creativecommons.org/licenses/BSD/
#   http://opensource.org/licenses/bsd-license.php
#

TOP=../../
PROG=crdnotes

include $(TOP)config.mk

$(PROG)$(_EXE): $(CHKLIB) $(PROG)$(_OBJ)
	$(LN)$@ $(PROG)$(_OBJ) -labcp $(MLIB)

clean:
	$(RM) $(PROG)$(_EXE)
	$(RM) $(PROG)$(_OBJ)
	$(RM) gmon.out

include $(TOP)targets.mk

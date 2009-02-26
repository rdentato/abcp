# 
#  (C) by Remo Dentato (rdentato@gmail.com)
# 
# This sofwtare is distributed under the terms of the BSD license:
#   http://creativecommons.org/licenses/BSD/
#   http://opensource.org/licenses/bsd-license.php
#

TOP=../../

include $(TOP)config.mk

explicit$(_EXE): $(CHKLIB) explicit$(_OBJ)
	$(LN)$@ explicit$(_OBJ) -labcp $(MLIB)

clean:
	$(RM) explicit$(_EXE)
	$(RM) explicit$(_OBJ)
	$(RM) gmon.out

include $(TOP)targets.mk

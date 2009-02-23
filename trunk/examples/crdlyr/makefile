# 
#  (C) by Remo Dentato (rdentato@gmail.com)
# 
# This sofwtare is distributed under the terms of the BSD license:
#   http://creativecommons.org/licenses/BSD/
#   http://opensource.org/licenses/bsd-license.php
#

TOP=../../

include $(TOP)config.mk

crdlyr$(_EXE): $(CHKLIB) crdlyr$(_OBJ)
	$(LN)$@ crdlyr$(_OBJ) -labcp $(MLIB)

clean:
	$(RM) crdlyr$(_EXE)
	$(RM) crdlyr$(_OBJ)
	$(RM) gmon.out

include $(TOP)targets.mk

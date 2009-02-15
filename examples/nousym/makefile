# 
#  (C) by Remo Dentato (rdentato@gmail.com)
# 
# This sofwtare is distributed under the terms of the BSD license:
#   http://creativecommons.org/licenses/BSD/
#   http://opensource.org/licenses/bsd-license.php
#

TOP=../../

include $(TOP)config.mk

nousym$(_EXE): $(CHKLIB) nousym$(_OBJ)
	$(LN)$@ nousym$(_OBJ) -labcp $(MLIB)

clean:
	$(RM) nousym$(_EXE)
	$(RM) nousym$(_OBJ)
	$(RM) gmon.out

include $(TOP)targets.mk

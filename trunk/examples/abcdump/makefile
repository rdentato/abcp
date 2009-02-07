# 
#  (C) by Remo Dentato (rdentato@gmail.com)
# 
# This sofwtare is distributed under the terms of the BSD license:
#   http://creativecommons.org/licenses/BSD/
#   http://opensource.org/licenses/bsd-license.php
#

TOP=../../

include $(TOP)config.mk

abcdump$(_EXE): $(CHKLIB) abcdump$(_OBJ)
	$(LN)$@ abcdump$(_OBJ) -labcp 

clean:
	$(RM) abcdump$(_EXE)
	$(RM) abcdump$(_OBJ)
	$(RM) gmon.out

include $(TOP)targets.mk

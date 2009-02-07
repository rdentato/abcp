# 
#  (C) by Remo Dentato (rdentato@gmail.com)
# 
# This sofwtare is distributed under the terms of the BSD license:
#   http://creativecommons.org/licenses/BSD/
#   http://opensource.org/licenses/bsd-license.php
#

TOP=./

include config.mk

all: FORCE
	cd src ; make CCOPTS="$(CCOPTS)" LNOPTS="$(LNOPTS)"
        
release: FORCE
	cd src ; make release
	
debug: FORCE
	cd src ; make debug
	
profile: FORCE
	cd src ; make profile

clean: FORCE
	cd src ; make clean
	
allclean: clean	
	$(RM) $(UTL)$(S)*

tools: FORCE
	$(CP) ..$(S)c-libutl$(S)dist$(S)* $(UTL)
 
FORCE: 

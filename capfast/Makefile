# Makefile,v 1.2 1995/09/02 01:45:00 epics Exp
#

include .applTop
-include $(APPLIC_TOP)/config/CONFIG

APPLIC_DIR_TYPE = capfast

-include $(APPLIC_TOP)/config/RULES.Dirs

# Define cp if we don't have it yet
ifndef CP
CP = cp
endif

.applTop : ../.applTop
	$(CP) $< $@

clean ::
	$(RM) .applTop


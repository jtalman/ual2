include $(UAL)/gui/ROOT/src/Makefile.config

MY_DIR  := $(UAL)/gui/ROOT/

OBJS     = 

include  ./Objects

HHS      = Def.hh

UICDECLS = Svd1DViewUI.hh

UICIMPLS = Svd1DViewUI.cc

SRCMOC   = moc_BasicViewer.cc \
	   moc_Svd1DViewUI.cc \
	   moc_BpmSvd1DViewer.cc \
	   moc_BunchCtDeViewer.cc \
	   moc_BunchTBTViewer.cc \
	   moc_BunchXPxViewer.cc \
	   moc_BunchYPyViewer.cc \
	   moc_MonitorFFTViewer.cc \
	   moc_MountainRangeViewer.cc \
	   moc_RFEditor.cc \
	   moc_TwissViewer.cc

compile: $(OBJS)
	if [ ! -d $(MY_DIR)/include/UAL/ROOT/Viewers ]; \
		then mkdir $(MY_DIR)/include/UAL/ROOT/Viewers; fi;
	cp $(HHS) $(MY_DIR)/include/UAL/ROOT/Viewers/.

#qt: $(SRCMOC)
 qt: $(UICDECLS) $(UICIMPLS) $(SRCMOC)

clean:
	rm -rf $(OBJS)
	rm -rf $(MY_DIR)/include/UAL/ROOT/Viewers
	rm -rf $(MY_DIR)/src/UAL/ROOT/Viewers/Svd1DViewUI.cc
	rm -rf $(MY_DIR)/src/UAL/ROOT/Viewers/moc_*
	rm -rf $(MY_DIR)/src/UAL/ROOT/Viewers/native_Svd1DViewUI.cc

Svd1DViewUI.cc: Svd1DViewUI.ui 
	@echo JDTSvd1DViewUIJDT
	$(UIC) -impl Svd1DViewUI.hh Svd1DViewUI.ui -o native_Svd1DViewUI.cc
	cp XXXSvd1DViewUI.ccXXX Svd1DViewUI.cc

include $(UAL)/gui/QT/src/Makefile.config

MY_DIR   = $(UAL)/gui/QT/

OBJS     =  
include  ./Objects

HHS      = Def.hh \
	   PlayerShell.hh

UICDECLS = MainPlayerUI.hh \
	   PlayerUI.hh \
	   TablePageUI.hh 

#UICIMPLS = MainPlayerUI.cc \
UICIMPLS =  PlayerUI.cc \
	   TablePageUI.cc 

SRCMOC   = moc_MainPlayerUI.cc \
	   moc_PlayerUI.cc \
	   moc_BasicPlayer.cc \
           moc_BasicViewer.cc \
           moc_BasicEditor.cc \
	   moc_TablePageUI.cc \
	   moc_BeamEditor.cc

compile: $(OBJS)
	if [ ! -d $(MY_DIR)/include/UAL/QT/Player ] ; \
		then mkdir $(MY_DIR)/include/UAL/QT/Player ; fi;
	cp $(HHS) $(MY_DIR)/include/UAL/QT/Player/.

qt: $(UICDECLS) $(UICIMPLS) $(SRCMOC)

clean:
	rm -rf $(OBJS)
	rm -rf $(MY_DIR)/include/UAL/QT/Player
	rm -rf $(MY_DIR)/src/UAL/QT/Player/moc_*
	rm -rf $(MY_DIR)/src/UAL/QT/Player/TablePageUI.cc

#MainPlayerUI.cc: MainPlayerUI.ui 
#	$(UIC) -impl MainPlayerUI.hh MainPlayerUI.ui   -o $@
#	cp ../../../../../XXXMainPlayerUI.ccXXX .

#PlayerUI.cc: PlayerUI.ui 
#	$(UIC) -impl PlayerUI.hh PlayerUI.ui   -o $@

#TablePageUI.cc: TablePageUI.ui 
#	$(UIC) -impl TablePageUI.hh TablePageUI.ui   -o $@

TablePageUI.cc: TablePageUI.ui 
	@echo JDTTablePageUIJDT
	$(UIC) -impl TablePageUI.hh TablePageUI.ui -o native_TablePageUI.cc
	cp XXXTablePageUI.ccXXX TablePageUI.cc

$(UAL)/gui/QT/lib/linux/obj/%.o : %.cc
	$(CC) $(CCFLAGS) -I. -I../../../../src/ -I../../../../../../ext/UI/src/ -I../../../../../../codes/UAL/src/ -I../../../../../../codes/PAC/src/ -I../../../../../../codes/ZLIB/src/ -I../../../../../../codes/TIBETAN/src/ -I../../../../../../ext/SXF/src/ -I../../../../../../codes/ACCSIM/src/ -I../../../../../../codes/AIM/src/ -c $< -o $@
#	$(CC) $(CCFLAGS) -I. -c $< -o $@;

#############################################################################
# Makefile for building: imageformats
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:48 2025
# Project:  imageformats.pro
# Template: subdirs
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile imageformats.pro
#############################################################################

MAKEFILE =	Makefile
QMAKE    =	qmake
DEL_FILE =    rm -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p
INSTALL_FILE= 
INSTALL_DIR = 
SUBTARGETS =	 \
		sub-jpeg \
		sub-mng

first: all

all: Makefile $(SUBTARGETS)

jpeg/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) "jpeg" || $(MKDIR) "jpeg"
	cd jpeg && $(QMAKE) jpeg.pro -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o $(MAKEFILE)
sub-jpeg: jpeg/$(MAKEFILE) FORCE
	cd jpeg && $(MAKE) -f $(MAKEFILE)

mng/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) "mng" || $(MKDIR) "mng"
	cd mng && $(QMAKE) mng.pro -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o $(MAKEFILE)
sub-mng: mng/$(MAKEFILE) FORCE
	cd mng && $(MAKE) -f $(MAKEFILE)

Makefile: imageformats.pro ../../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf 
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile imageformats.pro
qmake: qmake_all
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile imageformats.pro

all: $(SUBTARGETS)
qmake_all: jpeg/$(MAKEFILE) mng/$(MAKEFILE)
	 ( [ -d jpeg ] && cd jpeg ; grep "^qmake_all:" $(MAKEFILE) && $(MAKE) -f $(MAKEFILE) qmake_all; ) || true
	 ( [ -d mng ] && cd mng ; grep "^qmake_all:" $(MAKEFILE) && $(MAKE) -f $(MAKEFILE) qmake_all; ) || true
clean uicables mocables uiclean mocclean lexclean yaccclean : qmake_all FORCE
	 ( [ -d jpeg ] && cd jpeg ; $(MAKE) -f $(MAKEFILE) $@; ) || true
	 ( [ -d mng ] && cd mng ; $(MAKE) -f $(MAKEFILE) $@; ) || true
uninstall_subdirs: qmake_all FORCE
	 ( [ -d jpeg ] && cd jpeg ; $(MAKE) -f $(MAKEFILE) uninstall; ) || true
	 ( [ -d mng ] && cd mng ; $(MAKE) -f $(MAKEFILE) uninstall; ) || true
install_subdirs: qmake_all FORCE
	 ( [ -d jpeg ] && cd jpeg ; $(MAKE) -f $(MAKEFILE) install; ) || true
	 ( [ -d mng ] && cd mng ; $(MAKE) -f $(MAKEFILE) install; ) || true
distclean: qmake_all FORCE
	 ( [ -d jpeg ] && cd jpeg ; $(MAKE) -f $(MAKEFILE) $@; $(DEL_FILE) $(MAKEFILE); ) || true
	 ( [ -d mng ] && cd mng ; $(MAKE) -f $(MAKEFILE) $@; $(DEL_FILE) $(MAKEFILE); ) || true

install:  install_subdirs

uninstall:  uninstall_subdirs

FORCE:


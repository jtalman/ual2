#############################################################################
# Makefile for building: libqassistantclient.a
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:47 2025
# Project:  lib.pro
# Template: lib
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile lib.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -fno-exceptions -fPIC -Wall -W -O2 -D_REENTRANT  -DQT_INTERNAL_NETWORK -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_SHARED
CXXFLAGS = -pipe -fno-exceptions -fPIC -Wall -W -O2 -D_REENTRANT  -DQT_INTERNAL_NETWORK -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_SHARED
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -I. -I../../../include -I.moc/release-shared-mt/
AR       = ar cqs
RANLIB   = 
MOC      = /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/bin/moc
UIC      = /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/bin/uic -L /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/plugins
QMAKE    = /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/bin/qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = .obj/release-shared-mt/

####### Files

HEADERS = qassistantclient.h
SOURCES = qassistantclient.cpp
OBJECTS = .obj/release-shared-mt/qassistantclient.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = .moc/release-shared-mt/moc_qassistantclient.cpp
OBJMOC = .obj/release-shared-mt/moc_qassistantclient.o
DIST	   = ../../../src/qt_professional.pri \
		lib.pro
QMAKE_TARGET = qassistantclient
DESTDIR  = ../../../lib/
TARGET   = libqassistantclient.a

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile ../../../lib/libqassistantclient.prl ../../../lib/$(TARGET) 

staticlib: ../../../lib/$(TARGET)

../../../lib/$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC) $(OBJCOMP) 
	test -d ../../../lib/ || mkdir -p ../../../lib/
	-$(DEL_FILE) $(TARGET)
	$(AR) $(TARGET) $(OBJECTS) $(OBJMOC)
	-$(DEL_FILE) ../../../lib/$(TARGET)
	-$(MOVE) $(TARGET) ../../../lib/


mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/src/moc && $(MAKE) )

../../../lib/libqassistantclient.prl: 
	@$(QMAKE) -prl  -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ lib.pro
Makefile: lib.pro ../../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../../src/qt_professional.pri \
		../../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile lib.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile lib.pro

dist: 
	@mkdir -p .obj/release-shared-mt/qassistantclient && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/release-shared-mt/qassistantclient/ && ( cd `dirname .obj/release-shared-mt/qassistantclient` && $(TAR) qassistantclient.tar qassistantclient && $(GZIP) qassistantclient.tar ) && $(MOVE) `dirname .obj/release-shared-mt/qassistantclient`/qassistantclient.tar.gz . && $(DEL_FILE) -r .obj/release-shared-mt/qassistantclient

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:

yaccclean:
lexclean:
clean: mocclean
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) ../../../lib/$(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/release-shared-mt/qassistantclient.o: qassistantclient.cpp qassistantclient.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/qassistantclient.o qassistantclient.cpp

.obj/release-shared-mt/moc_qassistantclient.o: .moc/release-shared-mt/moc_qassistantclient.cpp  qassistantclient.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_qassistantclient.o .moc/release-shared-mt/moc_qassistantclient.cpp

.moc/release-shared-mt/moc_qassistantclient.cpp: $(MOC) qassistantclient.h
	$(MOC) qassistantclient.h -o .moc/release-shared-mt/moc_qassistantclient.cpp

####### Install

install_target: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)../../../lib/" || $(MKDIR) "$(INSTALL_ROOT)../../../lib/"
	-$(INSTALL_FILE) "../../../lib/libqassistantclient.prl" "$(INSTALL_ROOT)../../../lib/libqassistantclient.prl"
	
	-$(INSTALL_FILE) "../../../lib/$(TARGET)" "$(INSTALL_ROOT)../../../lib/$(TARGET)"

uninstall_target: 
	-$(DEL_FILE) "$(INSTALL_ROOT)../../../lib/libqassistantclient.prl"
	-$(DEL_FILE) "$(INSTALL_ROOT)../../../lib/$(TARGET)"
	-$(DEL_DIR) "$(INSTALL_ROOT)../../../lib/"


install: install_target  

uninstall: uninstall_target  


#############################################################################
# Makefile for building: simple_dd
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:52 2025
# Project:  simple_dd.pro
# Template: app
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile simple_dd.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -fno-exceptions -Wall -W -O2 -D_REENTRANT  -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_SHARED
CXXFLAGS = -pipe -fno-exceptions -Wall -W -O2 -D_REENTRANT  -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_SHARED
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -I. -I../../../include -I.moc/release-shared-mt/
LINK     = g++
LFLAGS   = -fno-exceptions  -Wl,-rpath,/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/lib
LIBS     = $(SUBLIBS) -L/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/lib -L/usr/X11R6/lib -lqt-mt -lXext -lX11 -lm -ldl -lpthread
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

HEADERS = main.h
SOURCES = main.cpp
OBJECTS = .obj/release-shared-mt/main.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = .moc/release-shared-mt/moc_main.cpp
OBJMOC = .obj/release-shared-mt/moc_main.o
DIST	   = simple_dd.pro
QMAKE_TARGET = simple_dd
DESTDIR  = 
TARGET   = simple_dd

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

all: Makefile $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(OBJCOMP) $(LIBS)

mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/src/moc && $(MAKE) )

Makefile: simple_dd.pro ../../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile simple_dd.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile simple_dd.pro

dist: 
	@mkdir -p .obj/release-shared-mt/simple_dd && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/release-shared-mt/simple_dd/ && ( cd `dirname .obj/release-shared-mt/simple_dd` && $(TAR) simple_dd.tar simple_dd && $(GZIP) simple_dd.tar ) && $(MOVE) `dirname .obj/release-shared-mt/simple_dd`/simple_dd.tar.gz . && $(DEL_FILE) -r .obj/release-shared-mt/simple_dd

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
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/release-shared-mt/main.o: main.cpp main.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/main.o main.cpp

.obj/release-shared-mt/moc_main.o: .moc/release-shared-mt/moc_main.cpp  main.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_main.o .moc/release-shared-mt/moc_main.cpp

.moc/release-shared-mt/moc_main.cpp: $(MOC) main.h
	$(MOC) main.h -o .moc/release-shared-mt/moc_main.cpp

####### Install

install:  

uninstall:  


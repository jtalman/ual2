#############################################################################
# Makefile for building: extension
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:58 2025
# Project:  extension.pro
# Template: app
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile extension.pro
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
INCPATH  = -I/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -I. -I../../include -I.moc/release-shared-mt/
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

HEADERS = 
SOURCES = main.cpp
OBJECTS = .obj/release-shared-mt/main.o \
		.obj/release-shared-mt/mainform.o \
		.obj/release-shared-mt/dialogform.o \
		.obj/release-shared-mt/extension.o
FORMS = mainform.ui \
		dialogform.ui \
		extension.ui
UICDECLS = mainform.h \
		dialogform.h \
		extension.h
UICIMPLS = mainform.cpp \
		dialogform.cpp \
		extension.cpp
SRCMOC   = .moc/release-shared-mt/moc_mainform.cpp \
		.moc/release-shared-mt/moc_dialogform.cpp \
		.moc/release-shared-mt/moc_extension.cpp
OBJMOC = .obj/release-shared-mt/moc_mainform.o \
		.obj/release-shared-mt/moc_dialogform.o \
		.obj/release-shared-mt/moc_extension.o
DIST	   = extension.pro
QMAKE_TARGET = extension
DESTDIR  = 
TARGET   = extension

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

Makefile: extension.pro ../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile extension.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile extension.pro

dist: 
	@mkdir -p .obj/release-shared-mt/extension && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/release-shared-mt/extension/ && $(COPY_FILE) --parents mainform.ui.h dialogform.ui.h extension.ui.h .obj/release-shared-mt/extension/ && ( cd `dirname .obj/release-shared-mt/extension` && $(TAR) extension.tar extension && $(GZIP) extension.tar ) && $(MOVE) `dirname .obj/release-shared-mt/extension`/extension.tar.gz . && $(DEL_FILE) -r .obj/release-shared-mt/extension

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:
	-$(DEL_FILE) $(UICIMPLS) $(UICDECLS)

yaccclean:
lexclean:
clean: mocclean uiclean
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/release-shared-mt/main.o: main.cpp mainform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/main.o main.cpp

mainform.h: mainform.ui 
	$(UIC) mainform.ui -o mainform.h

mainform.cpp: mainform.h mainform.ui mainform.ui.h 
	$(UIC) mainform.ui -i mainform.h -o mainform.cpp

dialogform.h: dialogform.ui 
	$(UIC) dialogform.ui -o dialogform.h

dialogform.cpp: dialogform.h dialogform.ui dialogform.ui.h 
	$(UIC) dialogform.ui -i dialogform.h -o dialogform.cpp

extension.h: extension.ui 
	$(UIC) extension.ui -o extension.h

extension.cpp: extension.h extension.ui extension.ui.h 
	$(UIC) extension.ui -i extension.h -o extension.cpp

.obj/release-shared-mt/mainform.o: mainform.cpp mainform.ui.h \
		mainform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/mainform.o mainform.cpp

.obj/release-shared-mt/dialogform.o: dialogform.cpp extension.h \
		dialogform.ui.h \
		dialogform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/dialogform.o dialogform.cpp

.obj/release-shared-mt/extension.o: extension.cpp extension.ui.h \
		extension.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/extension.o extension.cpp

.obj/release-shared-mt/moc_mainform.o: .moc/release-shared-mt/moc_mainform.cpp  mainform.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_mainform.o .moc/release-shared-mt/moc_mainform.cpp

.obj/release-shared-mt/moc_dialogform.o: .moc/release-shared-mt/moc_dialogform.cpp  dialogform.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_dialogform.o .moc/release-shared-mt/moc_dialogform.cpp

.obj/release-shared-mt/moc_extension.o: .moc/release-shared-mt/moc_extension.cpp  extension.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_extension.o .moc/release-shared-mt/moc_extension.cpp

.moc/release-shared-mt/moc_mainform.cpp: $(MOC) mainform.h
	$(MOC) mainform.h -o .moc/release-shared-mt/moc_mainform.cpp

.moc/release-shared-mt/moc_dialogform.cpp: $(MOC) dialogform.h
	$(MOC) dialogform.h -o .moc/release-shared-mt/moc_dialogform.cpp

.moc/release-shared-mt/moc_extension.cpp: $(MOC) extension.h
	$(MOC) extension.h -o .moc/release-shared-mt/moc_extension.cpp

####### Install

install:  

uninstall:  


#############################################################################
# Makefile for building: credit
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:50 2025
# Project:  credit.pro
# Template: app
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile credit.pro
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
INCPATH  = -I/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -I. -I../../../../include -I.moc/release-shared-mt/
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

HEADERS = creditform.h
SOURCES = main.cpp \
		creditform.cpp
OBJECTS = .obj/release-shared-mt/main.o \
		.obj/release-shared-mt/creditform.o \
		.obj/release-shared-mt/creditformbase.o
FORMS = creditformbase.ui
UICDECLS = creditformbase.h
UICIMPLS = creditformbase.cpp
SRCMOC   = .moc/release-shared-mt/moc_creditform.cpp \
		.moc/release-shared-mt/moc_creditformbase.cpp
OBJMOC = .obj/release-shared-mt/moc_creditform.o \
		.obj/release-shared-mt/moc_creditformbase.o
DIST	   = credit.pro
QMAKE_TARGET = credit
DESTDIR  = 
TARGET   = credit

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

Makefile: credit.pro ../../../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile credit.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile credit.pro

dist: 
	@mkdir -p .obj/release-shared-mt/credit && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/release-shared-mt/credit/ && $(COPY_FILE) --parents creditformbase.ui.h .obj/release-shared-mt/credit/ && ( cd `dirname .obj/release-shared-mt/credit` && $(TAR) credit.tar credit && $(GZIP) credit.tar ) && $(MOVE) `dirname .obj/release-shared-mt/credit`/credit.tar.gz . && $(DEL_FILE) -r .obj/release-shared-mt/credit

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

.obj/release-shared-mt/main.o: main.cpp creditform.h \
		creditformbase.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/main.o main.cpp

.obj/release-shared-mt/creditform.o: creditform.cpp creditform.h \
		creditformbase.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/creditform.o creditform.cpp

creditformbase.h: creditformbase.ui creditformbase.ui.h
	$(UIC) creditformbase.ui -o creditformbase.h

creditformbase.cpp: creditformbase.h creditformbase.ui creditformbase.ui.h creditformbase.ui.h
	$(UIC) creditformbase.ui -i creditformbase.h -o creditformbase.cpp

.obj/release-shared-mt/creditformbase.o: creditformbase.cpp creditformbase.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/creditformbase.o creditformbase.cpp

.obj/release-shared-mt/moc_creditform.o: .moc/release-shared-mt/moc_creditform.cpp  creditform.h creditformbase.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_creditform.o .moc/release-shared-mt/moc_creditform.cpp

.obj/release-shared-mt/moc_creditformbase.o: .moc/release-shared-mt/moc_creditformbase.cpp  creditformbase.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_creditformbase.o .moc/release-shared-mt/moc_creditformbase.cpp

.moc/release-shared-mt/moc_creditform.cpp: $(MOC) creditform.h
	$(MOC) creditform.h -o .moc/release-shared-mt/moc_creditform.cpp

.moc/release-shared-mt/moc_creditformbase.cpp: $(MOC) creditformbase.h
	$(MOC) creditformbase.h -o .moc/release-shared-mt/moc_creditformbase.cpp

####### Install

install:  

uninstall:  


#############################################################################
# Makefile for building: opentype
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:50 2025
# Project:  opentype.pro
# Template: app
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile opentype.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -fno-exceptions -Wall -W -g -D_REENTRANT  -DQT_THREAD_SUPPORT -DQT_SHARED -DQT_NO_DEBUG
CXXFLAGS = -pipe -fno-exceptions -Wall -W -g -D_REENTRANT  -DQT_THREAD_SUPPORT -DQT_SHARED -DQT_NO_DEBUG
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -I. -I. -I../../../include -I.moc/debug-shared-mt/
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

OBJECTS_DIR = .obj/debug-shared-mt/

####### Files

HEADERS = disasm.h \
		fterrcompat.h \
		ftxgdef.h \
		ftxgpos.h \
		ftxgsub.h \
		ftxopen.h \
		ftxopenf.h
SOURCES = disasm.c \
		ftxopentype.c \
		ottest.c
OBJECTS = .obj/debug-shared-mt/disasm.o \
		.obj/debug-shared-mt/ftxopentype.o \
		.obj/debug-shared-mt/ottest.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = 
OBJMOC = 
DIST	   = opentype.pro
QMAKE_TARGET = opentype
DESTDIR  = 
TARGET   = opentype

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

Makefile: opentype.pro ../../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile opentype.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile opentype.pro

dist: 
	@mkdir -p .obj/debug-shared-mt/opentype && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/debug-shared-mt/opentype/ && ( cd `dirname .obj/debug-shared-mt/opentype` && $(TAR) opentype.tar opentype && $(GZIP) opentype.tar ) && $(MOVE) `dirname .obj/debug-shared-mt/opentype`/opentype.tar.gz . && $(DEL_FILE) -r .obj/debug-shared-mt/opentype

mocclean:

uiclean:

yaccclean:
lexclean:
clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/debug-shared-mt/disasm.o: disasm.c disasm.h \
		ftxopen.h \
		ftxgdef.h \
		ftxgsub.h \
		ftxgpos.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o .obj/debug-shared-mt/disasm.o disasm.c

.obj/debug-shared-mt/ftxopentype.o: ftxopentype.c ftxopen.c \
		ftxgdef.c \
		ftxgpos.c \
		ftxgsub.c \
		fterrcompat.h \
		ftxopen.h \
		ftxopenf.h \
		ftxgdef.h \
		ftxgsub.h \
		ftxgpos.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o .obj/debug-shared-mt/ftxopentype.o ftxopentype.c

.obj/debug-shared-mt/ottest.o: ottest.c ftxopen.h \
		disasm.h \
		ftxgdef.h \
		ftxgsub.h \
		ftxgpos.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o .obj/debug-shared-mt/ottest.o ottest.c

####### Install

install:  

uninstall:  


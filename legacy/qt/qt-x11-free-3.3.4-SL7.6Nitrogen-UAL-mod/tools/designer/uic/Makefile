#############################################################################
# Makefile for building: ../../../bin/uic
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:49 2025
# Project:  uic.pro
# Template: app
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile uic.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -fno-exceptions -Wall -W -O2 -D_REENTRANT  -DQT_INTERNAL_XML -DUIC -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_SHARED
CXXFLAGS = -pipe -fno-exceptions -Wall -W -O2 -D_REENTRANT  -DQT_INTERNAL_XML -DUIC -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_SHARED
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -I. -I../shared -I../../../include -I.moc/release-shared-mt/
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

HEADERS = uic.h \
		../shared/widgetdatabase.h \
		../shared/domtool.h \
		../shared/parser.h \
		../interfaces/widgetinterface.h
SOURCES = main.cpp \
		uic.cpp \
		form.cpp \
		object.cpp \
		subclassing.cpp \
		embed.cpp \
		../shared/widgetdatabase.cpp \
		../shared/domtool.cpp \
		../shared/parser.cpp
OBJECTS = .obj/release-shared-mt/main.o \
		.obj/release-shared-mt/uic.o \
		.obj/release-shared-mt/form.o \
		.obj/release-shared-mt/object.o \
		.obj/release-shared-mt/subclassing.o \
		.obj/release-shared-mt/embed.o \
		.obj/release-shared-mt/widgetdatabase.o \
		.obj/release-shared-mt/domtool.o \
		.obj/release-shared-mt/parser.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = 
OBJMOC = 
DIST	   = ../../../src/qt_professional.pri \
		uic.pro
QMAKE_TARGET = uic
DESTDIR  = ../../../bin/
TARGET   = ../../../bin/uic

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
	test -d ../../../bin/ || mkdir -p ../../../bin/
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(OBJCOMP) $(LIBS)

mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/src/moc && $(MAKE) )

Makefile: uic.pro ../../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../../src/qt_professional.pri \
		../../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile uic.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile uic.pro

dist: 
	@mkdir -p .obj/release-shared-mt/uic && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/release-shared-mt/uic/ && ( cd `dirname .obj/release-shared-mt/uic` && $(TAR) uic.tar uic && $(GZIP) uic.tar ) && $(MOVE) `dirname .obj/release-shared-mt/uic`/uic.tar.gz . && $(DEL_FILE) -r .obj/release-shared-mt/uic

mocclean:

uiclean:

yaccclean:
lexclean:
clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) ../../../bin/$(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/release-shared-mt/main.o: main.cpp uic.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/main.o main.cpp

.obj/release-shared-mt/uic.o: uic.cpp uic.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/uic.o uic.cpp

.obj/release-shared-mt/form.o: form.cpp uic.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/form.o form.cpp

.obj/release-shared-mt/object.o: object.cpp uic.h \
		../interfaces/widgetinterface.h \
		../shared/widgetdatabase.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/object.o object.cpp

.obj/release-shared-mt/subclassing.o: subclassing.cpp uic.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/subclassing.o subclassing.cpp

.obj/release-shared-mt/embed.o: embed.cpp uic.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/embed.o embed.cpp

.obj/release-shared-mt/widgetdatabase.o: ../shared/widgetdatabase.cpp ../shared/widgetdatabase.h \
		../interfaces/widgetinterface.h \
		../shared/globaldefs.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/widgetdatabase.o ../shared/widgetdatabase.cpp

.obj/release-shared-mt/domtool.o: ../shared/domtool.cpp ../shared/domtool.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/domtool.o ../shared/domtool.cpp

.obj/release-shared-mt/parser.o: ../shared/parser.cpp ../shared/parser.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/parser.o ../shared/parser.cpp

####### Install

install_target: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)../../../bin/" || $(MKDIR) "$(INSTALL_ROOT)../../../bin/"
	-$(INSTALL_FILE) "../../../bin/$(QMAKE_TARGET)" "$(INSTALL_ROOT)../../../bin/$(QMAKE_TARGET)"
	-strip "$(INSTALL_ROOT)../../../bin/$(QMAKE_TARGET)"

uninstall_target: 
	-$(DEL_FILE) "$(INSTALL_ROOT)../../../bin/$(QMAKE_TARGET)"
	-$(DEL_DIR) "$(INSTALL_ROOT)../../../bin/"


install: install_target  

uninstall: uninstall_target  


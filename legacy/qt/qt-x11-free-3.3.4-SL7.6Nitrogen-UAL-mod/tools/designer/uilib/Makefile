#############################################################################
# Makefile for building: libqui.so.1.0.0
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:47 2025
# Project:  uilib.pro
# Template: lib
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile uilib.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -fno-exceptions -Wall -W -O2 -D_REENTRANT -fPIC  -DQT_INTERNAL_XML -DQ_TEMPLATE_EXTERN=extern -DRESOURCE -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_SHARED
CXXFLAGS = -pipe -fno-exceptions -Wall -W -O2 -D_REENTRANT -fPIC  -DQT_INTERNAL_XML -DQ_TEMPLATE_EXTERN=extern -DRESOURCE -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_SHARED
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -I. -I../shared -I../../../include -I.moc/release-shared-mt/
LINK     = g++
LFLAGS   = -fno-exceptions  -Wl,-rpath,/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/lib -shared -Wl,-soname,libqui.so.1 -Wl,-rpath,/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/lib
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

HEADERS = ../shared/domtool.h \
		../shared/uib.h \
		../designer/database2.h
SOURCES = qwidgetfactory.cpp \
		../shared/domtool.cpp \
		../shared/uib.cpp \
		../designer/database.cpp
OBJECTS = .obj/release-shared-mt/qwidgetfactory.o \
		.obj/release-shared-mt/domtool.o \
		.obj/release-shared-mt/uib.o \
		.obj/release-shared-mt/database.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = .moc/release-shared-mt/moc_database2.cpp
OBJMOC = .obj/release-shared-mt/moc_database2.o
DIST	   = ../../../src/qt_professional.pri \
		uilib.pro
QMAKE_TARGET = qui
DESTDIR  = ../../../lib/
TARGET   = libqui.so.1.0.0
TARGETA	= ../../../lib/libqui.a
TARGETD	= libqui.so.1.0.0
TARGET0	= libqui.so
TARGET1	= libqui.so.1
TARGET2	= libqui.so.1.0

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

all: Makefile ../../../lib/libqui.prl ../../../lib/$(TARGET)

../../../lib/$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC) $(SUBLIBS) $(OBJCOMP)  
	test -d ../../../lib/ || mkdir -p ../../../lib/
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS) $(OBJCOMP)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)
	-$(DEL_FILE) ../../../lib/$(TARGET)
	-$(DEL_FILE) ../../../lib/$(TARGET0)
	-$(DEL_FILE) ../../../lib/$(TARGET1)
	-$(DEL_FILE) ../../../lib/$(TARGET2)
	-$(MOVE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) ../../../lib/



staticlib: $(TARGETA)

$(TARGETA):  $(UICDECLS) $(OBJECTS) $(OBJMOC) $(OBJCOMP)
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(TARGETA) $(OBJECTS) $(OBJMOC)

mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/src/moc && $(MAKE) )

../../../lib/libqui.prl: 
	@$(QMAKE) -prl  -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ uilib.pro
Makefile: uilib.pro ../../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../../src/qt_professional.pri \
		../../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile uilib.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile uilib.pro

dist: 
	@mkdir -p .obj/release-shared-mt/qui && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/release-shared-mt/qui/ && ( cd `dirname .obj/release-shared-mt/qui` && $(TAR) qui.tar qui && $(GZIP) qui.tar ) && $(MOVE) `dirname .obj/release-shared-mt/qui`/qui.tar.gz . && $(DEL_FILE) -r .obj/release-shared-mt/qui

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
	-$(DEL_FILE) ../../../lib/$(TARGET0) ../../../lib/$(TARGET1) ../../../lib/$(TARGET2) $(TARGETA)


FORCE:

####### Compile

.obj/release-shared-mt/qwidgetfactory.o: qwidgetfactory.cpp qwidgetfactory.h \
		../interfaces/languageinterface.h \
		../interfaces/widgetinterface.h \
		../designer/database2.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/qwidgetfactory.o qwidgetfactory.cpp

.obj/release-shared-mt/domtool.o: ../shared/domtool.cpp ../shared/domtool.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/domtool.o ../shared/domtool.cpp

.obj/release-shared-mt/uib.o: ../shared/uib.cpp ../shared/uib.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/uib.o ../shared/uib.cpp

.obj/release-shared-mt/database.o: ../designer/database.cpp ../designer/database.h \
		../designer/database2.h \
		../designer/formwindow.h \
		../designer/mainwindow.h \
		../designer/command.h \
		../designer/metadatabase.h \
		../designer/sizehandle.h \
		../designer/actiondnd.h \
		../designer/orderindicator.h \
		../designer/layout.h \
		../designer/pixmapchooser.h \
		../interfaces/languageinterface.h \
		../shared/widgetdatabase.h \
		../interfaces/widgetinterface.h \
		../interfaces/actioninterface.h \
		../interfaces/editorinterface.h \
		../interfaces/templatewizardiface.h \
		../interfaces/filterinterface.h \
		../interfaces/interpreterinterface.h \
		../interfaces/preferenceinterface.h \
		../interfaces/projectsettingsiface.h \
		../interfaces/sourcetemplateiface.h \
		../designer/sourceeditor.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/database.o ../designer/database.cpp

.obj/release-shared-mt/moc_database2.o: .moc/release-shared-mt/moc_database2.cpp  ../designer/database2.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_database2.o .moc/release-shared-mt/moc_database2.cpp

.moc/release-shared-mt/moc_database2.cpp: $(MOC) ../designer/database2.h
	$(MOC) ../designer/database2.h -o .moc/release-shared-mt/moc_database2.cpp

####### Install

install_target: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)../../../lib/" || $(MKDIR) "$(INSTALL_ROOT)../../../lib/"
	-$(INSTALL_FILE) "../../../lib/libqui.prl" "$(INSTALL_ROOT)../../../lib/libqui.prl"
	
	-$(INSTALL_FILE) "../../../lib/$(TARGET)" "$(INSTALL_ROOT)../../../lib/$(TARGET)"
	-strip --strip-unneeded "$(INSTALL_ROOT)../../../lib/$(TARGET)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)../../../lib/$(TARGET0)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)../../../lib/$(TARGET1)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)../../../lib/$(TARGET2)"

uninstall_target: 
	-$(DEL_FILE) "$(INSTALL_ROOT)../../../lib/libqui.prl"
	-$(DEL_FILE) "$(INSTALL_ROOT)../../../lib/$(TARGET)"
	-$(DEL_FILE) "$(INSTALL_ROOT)../../../lib/$(TARGET0)"
	-$(DEL_FILE) "$(INSTALL_ROOT)../../../lib/$(TARGET1)"
	-$(DEL_FILE) "$(INSTALL_ROOT)../../../lib/$(TARGET2)"
	-$(DEL_DIR) "$(INSTALL_ROOT)../../../lib/"


install: install_target  

uninstall: uninstall_target  


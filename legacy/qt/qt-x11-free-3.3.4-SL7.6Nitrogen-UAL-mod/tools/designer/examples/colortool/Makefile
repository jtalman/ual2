#############################################################################
# Makefile for building: colortool
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:50 2025
# Project:  colortool.pro
# Template: app
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile colortool.pro
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
INCPATH  = -I/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -I. -I../../../../include -I.ui/ -I. -I.moc/
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

OBJECTS_DIR = .obj/

####### Files

HEADERS = 
SOURCES = main.cpp
OBJECTS = .obj/main.o \
		.obj/mainform.o \
		.obj/colornameform.o \
		.obj/findform.o \
		.obj/optionsform.o \
		.obj/qmake_image_collection.o
FORMS = mainform.ui \
		colornameform.ui \
		findform.ui \
		optionsform.ui
UICDECLS = .ui/mainform.h \
		.ui/colornameform.h \
		.ui/findform.h \
		.ui/optionsform.h
UICIMPLS = .ui/mainform.cpp \
		.ui/colornameform.cpp \
		.ui/findform.cpp \
		.ui/optionsform.cpp
SRCMOC   = .moc/moc_mainform.cpp \
		.moc/moc_colornameform.cpp \
		.moc/moc_findform.cpp \
		.moc/moc_optionsform.cpp
OBJMOC = .obj/moc_mainform.o \
		.obj/moc_colornameform.o \
		.obj/moc_findform.o \
		.obj/moc_optionsform.o
DIST	   = colortool.pro
QMAKE_TARGET = colortool
DESTDIR  = 
TARGET   = colortool

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

Makefile: colortool.pro ../../../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile colortool.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile colortool.pro

dist: 
	@mkdir -p .obj/colortool && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/colortool/ && $(COPY_FILE) --parents images/filenew.png images/fileopen.png images/filesave.png images/editcut.png images/editcopy.png images/searchfind.png images/tabwidget.png images/table.png images/iconview.png images/richtextedit.png images/widgetstack.png images/editraise.png .obj/colortool/ && $(COPY_FILE) --parents mainform.ui.h colornameform.ui.h findform.ui.h optionsform.ui.h .obj/colortool/ && ( cd `dirname .obj/colortool` && $(TAR) colortool.tar colortool && $(GZIP) colortool.tar ) && $(MOVE) `dirname .obj/colortool`/colortool.tar.gz . && $(DEL_FILE) -r .obj/colortool

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:
	-$(DEL_FILE) $(UICIMPLS) $(UICDECLS)

yaccclean:
lexclean:
clean: mocclean uiclean
	-$(DEL_FILE) $(OBJECTS)
		-$(DEL_FILE) .ui/qmake_image_collection.cpp
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/main.o: main.cpp .ui/mainform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/main.o main.cpp

.ui/mainform.h: mainform.ui .ui/findform.h
	$(UIC) mainform.ui -o .ui/mainform.h

.ui/mainform.cpp: .ui/mainform.h mainform.ui mainform.ui.h .ui/findform.h
	$(UIC) mainform.ui -i mainform.h -o .ui/mainform.cpp

.ui/colornameform.h: colornameform.ui 
	$(UIC) colornameform.ui -o .ui/colornameform.h

.ui/colornameform.cpp: .ui/colornameform.h colornameform.ui colornameform.ui.h 
	$(UIC) colornameform.ui -i colornameform.h -o .ui/colornameform.cpp

.ui/findform.h: findform.ui 
	$(UIC) findform.ui -o .ui/findform.h

.ui/findform.cpp: .ui/findform.h findform.ui findform.ui.h 
	$(UIC) findform.ui -i findform.h -o .ui/findform.cpp

.ui/optionsform.h: optionsform.ui 
	$(UIC) optionsform.ui -o .ui/optionsform.h

.ui/optionsform.cpp: .ui/optionsform.h optionsform.ui optionsform.ui.h 
	$(UIC) optionsform.ui -i optionsform.h -o .ui/optionsform.cpp

.obj/mainform.o: .ui/mainform.cpp .ui/colornameform.h \
		.ui/optionsform.h \
		mainform.ui.h \
		.ui/mainform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/mainform.o .ui/mainform.cpp

.obj/colornameform.o: .ui/colornameform.cpp colornameform.ui.h \
		.ui/colornameform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/colornameform.o .ui/colornameform.cpp

.obj/findform.o: .ui/findform.cpp findform.ui.h \
		.ui/findform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/findform.o .ui/findform.cpp

.obj/optionsform.o: .ui/optionsform.cpp optionsform.ui.h \
		.ui/optionsform.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/optionsform.o .ui/optionsform.cpp

.obj/moc_mainform.o: .moc/moc_mainform.cpp  .ui/mainform.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/moc_mainform.o .moc/moc_mainform.cpp

.obj/moc_colornameform.o: .moc/moc_colornameform.cpp  .ui/colornameform.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/moc_colornameform.o .moc/moc_colornameform.cpp

.obj/moc_findform.o: .moc/moc_findform.cpp  .ui/findform.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/moc_findform.o .moc/moc_findform.cpp

.obj/moc_optionsform.o: .moc/moc_optionsform.cpp  .ui/optionsform.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/moc_optionsform.o .moc/moc_optionsform.cpp

.moc/moc_mainform.cpp: $(MOC) .ui/mainform.h
	$(MOC) .ui/mainform.h -o .moc/moc_mainform.cpp

.moc/moc_colornameform.cpp: $(MOC) .ui/colornameform.h
	$(MOC) .ui/colornameform.h -o .moc/moc_colornameform.cpp

.moc/moc_findform.cpp: $(MOC) .ui/findform.h
	$(MOC) .ui/findform.h -o .moc/moc_findform.cpp

.moc/moc_optionsform.cpp: $(MOC) .ui/optionsform.h
	$(MOC) .ui/optionsform.h -o .moc/moc_optionsform.cpp

.obj/qmake_image_collection.o: .ui/qmake_image_collection.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/qmake_image_collection.o .ui/qmake_image_collection.cpp

.ui/qmake_image_collection.cpp: images/filenew.png \
		images/fileopen.png \
		images/filesave.png \
		images/editcut.png \
		images/editcopy.png \
		images/searchfind.png \
		images/tabwidget.png \
		images/table.png \
		images/iconview.png \
		images/richtextedit.png \
		images/widgetstack.png \
		images/editraise.png
	$(UIC)  -embed colortool images/filenew.png images/fileopen.png images/filesave.png images/editcut.png images/editcopy.png images/searchfind.png images/tabwidget.png images/table.png images/iconview.png images/richtextedit.png images/widgetstack.png images/editraise.png -o .ui/qmake_image_collection.cpp

####### Install

install:  

uninstall:  


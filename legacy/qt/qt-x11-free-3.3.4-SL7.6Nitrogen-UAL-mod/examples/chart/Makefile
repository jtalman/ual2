#############################################################################
# Makefile for building: chart
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:59 2025
# Project:  chart.pro
# Template: app
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile chart.pro
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

HEADERS = element.h \
		canvastext.h \
		canvasview.h \
		chartform.h \
		optionsform.h \
		setdataform.h
SOURCES = element.cpp \
		canvasview.cpp \
		chartform.cpp \
		chartform_canvas.cpp \
		chartform_files.cpp \
		optionsform.cpp \
		setdataform.cpp \
		main.cpp
OBJECTS = .obj/release-shared-mt/element.o \
		.obj/release-shared-mt/canvasview.o \
		.obj/release-shared-mt/chartform.o \
		.obj/release-shared-mt/chartform_canvas.o \
		.obj/release-shared-mt/chartform_files.o \
		.obj/release-shared-mt/optionsform.o \
		.obj/release-shared-mt/setdataform.o \
		.obj/release-shared-mt/main.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = .moc/release-shared-mt/moc_canvasview.cpp \
		.moc/release-shared-mt/moc_chartform.cpp \
		.moc/release-shared-mt/moc_optionsform.cpp \
		.moc/release-shared-mt/moc_setdataform.cpp
OBJMOC = .obj/release-shared-mt/moc_canvasview.o \
		.obj/release-shared-mt/moc_chartform.o \
		.obj/release-shared-mt/moc_optionsform.o \
		.obj/release-shared-mt/moc_setdataform.o
DIST	   = chart.pro
QMAKE_TARGET = chart
DESTDIR  = 
TARGET   = chart

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

Makefile: chart.pro ../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile chart.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile chart.pro

dist: 
	@mkdir -p .obj/release-shared-mt/chart && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/release-shared-mt/chart/ && ( cd `dirname .obj/release-shared-mt/chart` && $(TAR) chart.tar chart && $(GZIP) chart.tar ) && $(MOVE) `dirname .obj/release-shared-mt/chart`/chart.tar.gz . && $(DEL_FILE) -r .obj/release-shared-mt/chart

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

.obj/release-shared-mt/element.o: element.cpp element.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/element.o element.cpp

.obj/release-shared-mt/canvasview.o: canvasview.cpp canvasview.h \
		chartform.h \
		element.h \
		canvastext.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/canvasview.o canvasview.cpp

.obj/release-shared-mt/chartform.o: chartform.cpp canvasview.h \
		chartform.h \
		optionsform.h \
		setdataform.h \
		images/file_new.xpm \
		images/file_open.xpm \
		images/file_save.xpm \
		images/file_print.xpm \
		images/options_setdata.xpm \
		images/options_setfont.xpm \
		images/options_setoptions.xpm \
		images/options_horizontalbarchart.xpm \
		images/options_piechart.xpm \
		images/options_verticalbarchart.xpm \
		element.h \
		canvastext.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/chartform.o chartform.cpp

.obj/release-shared-mt/chartform_canvas.o: chartform_canvas.cpp canvastext.h \
		chartform.h \
		element.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/chartform_canvas.o chartform_canvas.cpp

.obj/release-shared-mt/chartform_files.o: chartform_files.cpp canvasview.h \
		chartform.h \
		element.h \
		canvastext.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/chartform_files.o chartform_files.cpp

.obj/release-shared-mt/optionsform.o: optionsform.cpp optionsform.h \
		images/options_horizontalbarchart.xpm \
		images/options_piechart.xpm \
		images/options_verticalbarchart.xpm
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/optionsform.o optionsform.cpp

.obj/release-shared-mt/setdataform.o: setdataform.cpp setdataform.h \
		chartform.h \
		images/pattern01.xpm \
		images/pattern02.xpm \
		images/pattern03.xpm \
		images/pattern04.xpm \
		images/pattern05.xpm \
		images/pattern06.xpm \
		images/pattern07.xpm \
		images/pattern08.xpm \
		images/pattern09.xpm \
		images/pattern10.xpm \
		images/pattern11.xpm \
		images/pattern12.xpm \
		images/pattern13.xpm \
		images/pattern14.xpm \
		element.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/setdataform.o setdataform.cpp

.obj/release-shared-mt/main.o: main.cpp chartform.h \
		element.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/main.o main.cpp

.obj/release-shared-mt/moc_canvasview.o: .moc/release-shared-mt/moc_canvasview.cpp  canvasview.h element.h \
		canvastext.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_canvasview.o .moc/release-shared-mt/moc_canvasview.cpp

.obj/release-shared-mt/moc_chartform.o: .moc/release-shared-mt/moc_chartform.cpp  chartform.h element.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_chartform.o .moc/release-shared-mt/moc_chartform.cpp

.obj/release-shared-mt/moc_optionsform.o: .moc/release-shared-mt/moc_optionsform.cpp  optionsform.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_optionsform.o .moc/release-shared-mt/moc_optionsform.cpp

.obj/release-shared-mt/moc_setdataform.o: .moc/release-shared-mt/moc_setdataform.cpp  setdataform.h element.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_setdataform.o .moc/release-shared-mt/moc_setdataform.cpp

.moc/release-shared-mt/moc_canvasview.cpp: $(MOC) canvasview.h
	$(MOC) canvasview.h -o .moc/release-shared-mt/moc_canvasview.cpp

.moc/release-shared-mt/moc_chartform.cpp: $(MOC) chartform.h
	$(MOC) chartform.h -o .moc/release-shared-mt/moc_chartform.cpp

.moc/release-shared-mt/moc_optionsform.cpp: $(MOC) optionsform.h
	$(MOC) optionsform.h -o .moc/release-shared-mt/moc_optionsform.cpp

.moc/release-shared-mt/moc_setdataform.cpp: $(MOC) setdataform.h
	$(MOC) setdataform.h -o .moc/release-shared-mt/moc_setdataform.cpp

####### Install

install:  

uninstall:  


#############################################################################
# Makefile for building: book7
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:49 2025
# Project:  book7.pro
# Template: app
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile book7.pro
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
INCPATH  = -I/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -I. -I../../../../../include -I.moc/release-shared-mt/
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
SOURCES = main.cpp \
		../connection.cpp
OBJECTS = .obj/release-shared-mt/main.o \
		.obj/release-shared-mt/connection.o \
		.obj/release-shared-mt/book.o \
		.obj/release-shared-mt/editbook.o
FORMS = book.ui \
		editbook.ui
UICDECLS = book.h \
		editbook.h
UICIMPLS = book.cpp \
		editbook.cpp
SRCMOC   = .moc/release-shared-mt/moc_book.cpp \
		.moc/release-shared-mt/moc_editbook.cpp
OBJMOC = .obj/release-shared-mt/moc_book.o \
		.obj/release-shared-mt/moc_editbook.o
DIST	   = book7.pro
QMAKE_TARGET = book7
DESTDIR  = 
TARGET   = book7

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

Makefile: book7.pro ../../../../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../../../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile book7.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile book7.pro

dist: 
	@mkdir -p .obj/release-shared-mt/book7 && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/release-shared-mt/book7/ && $(COPY_FILE) --parents book.ui.h editbook.ui.h .obj/release-shared-mt/book7/ && ( cd `dirname .obj/release-shared-mt/book7` && $(TAR) book7.tar book7 && $(GZIP) book7.tar ) && $(MOVE) `dirname .obj/release-shared-mt/book7`/book7.tar.gz . && $(DEL_FILE) -r .obj/release-shared-mt/book7

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

.obj/release-shared-mt/main.o: main.cpp book.h \
		../connection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/main.o main.cpp

.obj/release-shared-mt/connection.o: ../connection.cpp ../connection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/connection.o ../connection.cpp

book.h: book.ui editbook.h \
		book.ui.h
	$(UIC) book.ui -o book.h

book.cpp: book.h book.ui book.ui.h editbook.h \
		book.ui.h
	$(UIC) book.ui -i book.h -o book.cpp

editbook.h: editbook.ui editbook.ui.h
	$(UIC) editbook.ui -o editbook.h

editbook.cpp: editbook.h editbook.ui editbook.ui.h editbook.ui.h
	$(UIC) editbook.ui -i editbook.h -o editbook.cpp

.obj/release-shared-mt/book.o: book.cpp book.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/book.o book.cpp

.obj/release-shared-mt/editbook.o: editbook.cpp editbook.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/editbook.o editbook.cpp

.obj/release-shared-mt/moc_book.o: .moc/release-shared-mt/moc_book.cpp  book.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_book.o .moc/release-shared-mt/moc_book.cpp

.obj/release-shared-mt/moc_editbook.o: .moc/release-shared-mt/moc_editbook.cpp  editbook.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/release-shared-mt/moc_editbook.o .moc/release-shared-mt/moc_editbook.cpp

.moc/release-shared-mt/moc_book.cpp: $(MOC) book.h
	$(MOC) book.h -o .moc/release-shared-mt/moc_book.cpp

.moc/release-shared-mt/moc_editbook.cpp: $(MOC) editbook.h
	$(MOC) editbook.h -o .moc/release-shared-mt/moc_editbook.cpp

####### Install

install:  

uninstall:  


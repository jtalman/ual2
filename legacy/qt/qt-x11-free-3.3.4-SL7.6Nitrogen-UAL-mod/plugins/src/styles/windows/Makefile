#############################################################################
# Makefile for building: libqwindowsstyle.so
# Generated by qmake (1.07a) (Qt 3.3.4) on: Sun May 25 09:14:51 2025
# Project:  windows.pro
# Template: lib
# Command: $(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile windows.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -fno-exceptions -w -O2 -D_REENTRANT -fPIC  -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED
CXXFLAGS = -pipe -fno-exceptions -w -O2 -D_REENTRANT -fPIC  -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -I. -I../../../../include -I.moc/release-shared-mt/
LINK     = g++
LFLAGS   = -fno-exceptions  -Wl,-rpath,/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/lib -shared -Wl,-rpath,/home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/plugins/styles
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

HEADERS = ../../../../include/qwindowsstyle.h
SOURCES = main.cpp \
		../../../../src/styles/qwindowsstyle.cpp
OBJECTS = .obj/main.o \
		.obj/qwindowsstyle.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = .moc/release-shared-mt/moc_qwindowsstyle.cpp
OBJMOC = .obj/moc_qwindowsstyle.o
DIST	   = windows.pro
QMAKE_TARGET = qwindowsstyle
DESTDIR  = ../../../styles/
TARGET   = libqwindowsstyle.so
TARGETD   = libqwindowsstyle.so

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

all: Makefile  ../../../styles/$(TARGET)

../../../styles/$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC) $(SUBLIBS) $(OBJCOMP)  
	test -d ../../../styles/ || mkdir -p ../../../styles/
	-$(DEL_FILE) $(TARGET)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS) $(OBJCOMP)
	-$(MOVE) $(TARGET) ../../../styles/



mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/src/moc && $(MAKE) )

Makefile: windows.pro ../../../../.qmake.cache /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++/qmake.conf ../../../../lib/libqt-mt.prl
	$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile windows.pro
qmake: 
	@$(QMAKE) -spec /home/ualusr2011/git-gui-2/ual2/legacy/qt/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/mkspecs/linux-g++ -o Makefile windows.pro

dist: 
	@mkdir -p .obj/qwindowsstyle && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/qwindowsstyle/ && ( cd `dirname .obj/qwindowsstyle` && $(TAR) qwindowsstyle.tar qwindowsstyle && $(GZIP) qwindowsstyle.tar ) && $(MOVE) `dirname .obj/qwindowsstyle`/qwindowsstyle.tar.gz . && $(DEL_FILE) -r .obj/qwindowsstyle

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
	-$(DEL_FILE) ../../../styles/$(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/main.o: main.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/main.o main.cpp

.obj/qwindowsstyle.o: ../../../../src/styles/qwindowsstyle.cpp ../../../../src/styles/qwindowsstyle.h \
		../../../../include/qcommonstyle.h \
		../../../../include/qstyle.h \
		../../../../include/qobject.h \
		../../../../include/qobjectdefs.h \
		../../../../include/qwindowdefs.h \
		../../../../include/qstring.h \
		../../../../include/qevent.h \
		../../../../include/qnamespace.h \
		../../../../include/qwinexport.h \
		../../../../include/qglobal.h \
		../../../../include/qconfig-minimal.h \
		../../../../include/qconfig-small.h \
		../../../../include/qconfig-medium.h \
		../../../../include/qconfig-large.h \
		../../../../include/qconfig.h \
		../../../../include/qmodules.h \
		../../../../include/qfeatures.h \
		../../../../include/qcstring.h \
		../../../../include/qmemarray.h \
		../../../../include/qgarray.h \
		../../../../include/qshared.h \
		../../../../include/qregion.h \
		../../../../include/qmime.h \
		../../../../include/qpair.h \
		../../../../include/qrect.h \
		../../../../include/qsize.h \
		../../../../include/qpoint.h \
		../../../../include/qmap.h \
		../../../../include/qptrlist.h \
		../../../../include/qdatastream.h \
		../../../../include/qvaluelist.h \
		../../../../include/qiodevice.h \
		../../../../include/qtl.h \
		../../../../include/qtextstream.h \
		../../../../include/qglist.h \
		../../../../include/qptrcollection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/qwindowsstyle.o ../../../../src/styles/qwindowsstyle.cpp

.obj/moc_qwindowsstyle.o: .moc/release-shared-mt/moc_qwindowsstyle.cpp  ../../../../include/qwindowsstyle.h ../../../../include/qcommonstyle.h \
		../../../../include/qstyle.h \
		../../../../include/qobject.h \
		../../../../include/qobjectdefs.h \
		../../../../include/qwindowdefs.h \
		../../../../include/qstring.h \
		../../../../include/qevent.h \
		../../../../include/qnamespace.h \
		../../../../include/qwinexport.h \
		../../../../include/qglobal.h \
		../../../../include/qconfig-minimal.h \
		../../../../include/qconfig-small.h \
		../../../../include/qconfig-medium.h \
		../../../../include/qconfig-large.h \
		../../../../include/qconfig.h \
		../../../../include/qmodules.h \
		../../../../include/qfeatures.h \
		../../../../include/qcstring.h \
		../../../../include/qmemarray.h \
		../../../../include/qgarray.h \
		../../../../include/qshared.h \
		../../../../include/qregion.h \
		../../../../include/qmime.h \
		../../../../include/qpair.h \
		../../../../include/qrect.h \
		../../../../include/qsize.h \
		../../../../include/qpoint.h \
		../../../../include/qmap.h \
		../../../../include/qptrlist.h \
		../../../../include/qdatastream.h \
		../../../../include/qvaluelist.h \
		../../../../include/qiodevice.h \
		../../../../include/qtl.h \
		../../../../include/qtextstream.h \
		../../../../include/qglist.h \
		../../../../include/qptrcollection.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/moc_qwindowsstyle.o .moc/release-shared-mt/moc_qwindowsstyle.cpp

.moc/release-shared-mt/moc_qwindowsstyle.cpp: $(MOC) ../../../../include/qwindowsstyle.h
	$(MOC) ../../../../include/qwindowsstyle.h -o .moc/release-shared-mt/moc_qwindowsstyle.cpp

####### Install

install_target: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)../../../styles/" || $(MKDIR) "$(INSTALL_ROOT)../../../styles/"
	-$(INSTALL_FILE) "../../../styles/$(TARGET)" "$(INSTALL_ROOT)../../../styles/$(TARGET)"
	-strip --strip-unneeded "$(INSTALL_ROOT)../../../styles/$(TARGET)"

uninstall_target: 
	-$(DEL_FILE) "$(INSTALL_ROOT)../../../styles/$(TARGET)"
	-$(DEL_DIR) "$(INSTALL_ROOT)../../../styles/"


install: install_target  

uninstall: uninstall_target  


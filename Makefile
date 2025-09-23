CC       = g++                                    # Compiler
CCFLAGS  =
CCFLAGS  = -g -fpic  -O0 -fno-inline -Wall  -Wno-deprecated
LD       = g++
LDFLAGS  =
LDFLAGS += -Wl,-rpath=$(UAL)/gui/libxml2-2.9.7/.libs/libxml2.so.2.9.7:$(UAL)/gui/run-scripts/USPAS-legacy/examples/RT-transition/lib/linux/:$(UAL)/gui/root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/

all: run

compile: run

clean:
	rm -f run
	rm -f run.o
	rm -f twiss
	rm -f echo.sxf
	rm -f map

run: run.o
	$(CC) -o run run.o $(UAL)/gui/libxml2-2.9.7/.libs/libxml2.so.2.9.7 ./player/lib/linux/libUspasPlayer.so $(LDFLAGS) $(UAL)/gui/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/lib/libqt-mt.so.3.3.4 $(UAL)/gui/root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/libRint.so.4.04 -L$(UAL)/gui/root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/ -lCore -lCint -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lGui -pthread -lm -ldl -rdynamic -lThread $(UAL)/codes/ZLIB/lib/linux/libZTps.so $(UAL)/gui/QT/lib/linux/libUalQt.so $(UAL)/ext/UI/lib/linux/libUalUI.so $(UAL)/tools/lib/linux/libxerces-c.so.27.0 $(UAL)/gui/ROOT/lib/linux/libUalRootViewers.so $(UAL)/codes/AIM/lib/linux/libAim.so $(UAL)/gui/root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/libGQt.so $(UAL)/codes/UAL/lib/linux/libUal.so $(UAL)/codes/ETEAPOT/lib/linux/libETeapot.so $(UAL)/codes/TIBETAN/lib/linux/libTibetan.so $(UAL)/codes/PAC/lib/linux/libPacSMF.so $(UAL)/codes/PAC/lib/linux/libPacOptics.so $(UAL)/codes/PAC/lib/linux/libPac.so

run.o: run.cc
	$(CC) -Wl,-rpath=. -pipe -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT -I$(UAL)/gui/libxml2-2.9.7/include/libxml/ $(CCFLAGS) -c $< -o $@ -I$(UAL)/gui/qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/include/ -I$(UAL)/ext/UI/src/ -I$(UAL)/codes/PAC/src/ -I$(UAL)/codes/UAL/src/ -I$(UAL)/codes/ZLIB/src/ -I$(UAL)/gui/QT/src/ -I$(UAL)/codes/TIBETAN/src/ -I$(UAL)/gui/ROOT/src/ -I$(UAL)/gui/root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/include/ -I$(UAL)/gui/root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/qt/inc/ -I$(UAL)/ext/ADXF/src/ -I$(UAL)/tools/include/ -I./player/src/ -I$(UAL)/codes/ETEAPOT/src/ -I$(UAL)/codes/TIBETAN/src/

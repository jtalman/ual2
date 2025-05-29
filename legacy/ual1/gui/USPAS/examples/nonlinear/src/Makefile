#include $(UAL_USPAS)/Makefile.config
 include $(UAL)/env/linux/Makefile.config

LDFLAGS  =
LDFLAGS += -Wl,-rpath=$(UAL)/gui/USPAS/examples/nonlinear/lib/linux/:$(UAL)/gui/root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/

APP_DIR  = $(UAL)/gui/USPAS/examples/player/
SRC_DIR  = .
LIB_DIR  = $(APP_DIR)/lib/linux/
OBJ_DIR  = $(LIB_DIR)/obj

OBJS     = $(OBJ_DIR)/MapTracker.o \
	$(OBJ_DIR)/TuneRamper.o

all:  $(APP_DIR)/$(UAL_ARCH)/run

compile:  $(APP_DIR)/$(UAL_ARCH)/run 

clean:
	rm  -f  $(OBJ_DIR)/*


$(APP_DIR)/linux/run: $(OBJ_DIR)/run.o
	$(LD) -o ../lib/linux/run $(OBJ_DIR)/run.o $(LIB_DIR)/libUspasPlayer.so $(LDFLAGS) ../../../../qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/lib/libqt-mt.so.3.3.4 ../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/libRint.so.4.04 -L../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/ -lCore -lCint -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lGui -pthread -lm -ldl -rdynamic -lThread ../../../../../codes/ZLIB/lib/linux/libZTps.so ../../../../QT/lib/linux/libUalQt.so ../../../../../ext/UI/lib/linux/libUalUI.so ../../../../../tools/lib/linux/libxerces-c.so.27.0 ../../../../ROOT/lib/linux/libUalRootViewers.so ../../../../../codes/AIM/lib/linux/libAim.so ../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/libGQt.so

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) -Wl,-rpath=. -pipe -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT $(CCFLAGS) -c $< -o $@ -I../../../../qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/include/ -I../../../../../ext/UI/src/ -I../../../../../codes/PAC/src/ -I../../../../../codes/UAL/src/ -I../../../../../codes/ZLIB/src/ -I../../../../QT/src/ -I../../../../../codes/TIBETAN/src/ -I../../../../ROOT/src/ -I../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/include/ -I../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/qt/inc/ -I../../../../../ext/ADXF/src/ -I../../../../../tools/include/ -I../../player/src/

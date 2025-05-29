CC       = g++                                    # Compiler
CCFLAGS  =
CCFLAGS  = -g -fpic  -O0 -fno-inline -Wall  -Wno-deprecated
LD       = g++
LDFLAGS  =
LDFLAGS += -Wl,-rpath=$(UAL)/gui/libxml2-2.9.7/.libs/libxml2.so.2.9.7:$(UAL)/gui/USPAS/examples/decoherence/lib/linux/:$(UAL)/gui/root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/

APP_DIR  = $(UAL)/gui/USPAS/examples/decoherence/
SRC_DIR  = .
LIB_DIR  = $(APP_DIR)/lib/linux/
OBJ_DIR  = $(LIB_DIR)/obj

HEADERS  =   

OBJS     =  $(OBJ_DIR)/OneTurnKicker.o

all: $(OBJS) $(APP_DIR)/linux/run

compile: $(OBJS) $(APP_DIR)/linux/run

clean:
	rm -f $(OBJ_DIR)/*
	rm -f $(APP_DIR)/lib/linux/run
	rm -f $(APP_DIR)/lib/linux/libDecoherence.so
	rm -f $(APP_DIR)/lib/linux/libGt.so
	rm -f $(APP_DIR)/lib/twiss
	rm -f $(APP_DIR)/lib/echo.sxf
	rm -f $(APP_DIR)/lib/map

$(LIB_DIR)/%.so: $(OBJMOC) $(OBJS) # $(DICT)
	$(DLD) $(DLDFLAGS) -Wl,-soname,$@ $(LDFLAGS) $(OBJMOC) $(OBJS)  -o $@

$(APP_DIR)/linux/run: $(OBJ_DIR)/run.o
	@echo $(LDFLAGS)
	$(LD) -o ../lib/linux/run $(OBJS) $(OBJ_DIR)/run.o $(UAL)/gui/libxml2-2.9.7/.libs/libxml2.so.2.9.7 ../../player/lib/linux/libUspasPlayer.so $(LDFLAGS) ../../../../qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/lib/libqt-mt.so.3.3.4 ../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/libRint.so.4.04 -L../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/ -lCore -lCint -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lGui -pthread -lm -ldl -rdynamic -lThread ../../../../../codes/ZLIB/lib/linux/libZTps.so ../../../../QT/lib/linux/libUalQt.so ../../../../../ext/UI/lib/linux/libUalUI.so ../../../../../tools/lib/linux/libxerces-c.so.27.0 ../../../../ROOT/lib/linux/libUalRootViewers.so ../../../../../codes/AIM/lib/linux/libAim.so ../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/libGQt.so ../../../../../codes/UAL/lib/linux/libUal.so ../../../../../codes/ETEAPOT/lib/linux/libETeapot.so ../../../../../codes/TIBETAN/lib/linux/libTibetan.so ../../../../../codes/PAC/lib/linux/libPacSMF.so ../../../../../codes/PAC/lib/linux/libPacOptics.so ../../../../../codes/PAC/lib/linux/libPac.so
#	$(LD) -o ../lib/linux/run $(OBJ_DIR)/run.o $(UAL)/gui/libxml2-2.9.7/.libs/libxml2.so.2.9.7 ../../player/lib/linux/libUspasPlayer.so $(LDFLAGS) ../../../../qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/lib/libqt-mt.so.3.3.4 ../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/libRint.so.4.04 -L../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/ -lCore -lCint -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lGui -pthread -lm -ldl -rdynamic -lThread -lTable ../../../../../codes/ZLIB/lib/linux/libZTps.so ../../../../QT/lib/linux/libUalQt.so ../../../../../ext/UI/lib/linux/libUalUI.so ../../../../../tools/lib/linux/libxerces-c.so.27.0 ../../../../ROOT/lib/linux/libUalRootViewers.so ../../../../../codes/AIM/lib/linux/libAim.so ../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/lib/libGQt.so ../../../../../codes/UAL/lib/linux/libUal.so ../../../../../codes/ETEAPOT/lib/linux/libETeapot.so

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) -Wl,-rpath=. -pipe -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT -I$(UAL)/gui/libxml2-2.9.7/include/libxml/ $(CCFLAGS) -c $< -o $@ -I../../../../qt-x11-free-3.3.4-SL7.6Nitrogen-UAL-mod/include/ -I../../../../../ext/UI/src/ -I../../../../../codes/PAC/src/ -I../../../../../codes/UAL/src/ -I../../../../../codes/ZLIB/src/ -I../../../../QT/src/ -I../../../../../codes/TIBETAN/src/ -I../../../../ROOT/src/ -I../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/include/ -I../../../../root_v4.04.02.source-SL7.6Nitrogen-UAL-mod/qt/inc/ -I../../../../../ext/ADXF/src/ -I../../../../../tools/include/ -I../../player/src/ -I../../../../../codes/ETEAPOT/src/ -I../../../../../codes/TIBETAN/src/

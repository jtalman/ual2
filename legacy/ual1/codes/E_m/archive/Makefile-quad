INC  = 
INC += -I$(UAL)/codes/PAC/src/
INC += -I$(UAL)/codes/UAL/src/
INC += -I$(UAL)/codes/ZLIB/src/
INC += -I$(UAL)/codes/ETEAPOT/src/
INC += -I$(UAL)/codes/ETEAPOT2/src/

LIBS  =
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)   -lPacSMF
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)   -lPacOptics 
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)   -lPac 
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)   -lPacSurvey
LIBS += -L$(UAL_ZLIB)/lib/$(UAL_ARCH)  -lZTps
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH) -lgsl -lgslcblas

compile: quad.o TrackerFactory.o
	cp TrackerFactory.hh include
	g++ -shared -o lib/linux/libE_m.so quad.o TrackerFactory.o

%.o: %.cc
	gcc -c $< $(INC) -g -fpic  -O0 -fno-inline -Wall  -Wno-deprecated -o $@

clean:
	rm -rf echo.sxf quad.o TrackerFactory.o lib/linux/libE_m.so include/TrackerFactory.hh

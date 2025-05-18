CC       = g++                       # Compiler
CCFLAGS  = -g -fpic -ansi -Wall -Wextra -fpermissive     # Compilation flags

DLD      = g++                       # Dynamic linker
DLDFLAGS = -shared                   # Dynamic linker flags
LD       = g++                       # Linker

INC += -I. -I./src
INC += -I$(UAL)/ext/UI/include
INC += -I$(UAL_SXF)/src
INC += -I$(UAL_ACCSIM)/src
INC += -I$(UAL_TEAPOT)/src
INC += -I$(UAL_ETEAPOT)/src
INC += -I$(UAL_ETEAPOT_MltTurn)/src
INC += -I$(UAL_PAC)/src
INC += -I$(UAL)/codes/UAL/include
INC += -I$(UAL_ZLIB)/src
INC += -I$(UAL)/tools/include
INC += -I$(SXF)/src
INC += -I/home/ual/tools/gsl/include
INC += -I$(UAL)/codes/ETEAPOT/src/ETEAPOT/Integrator

LIBS += -L$(UAL)/ext/UI/lib/$(UAL_ARCH)  	 -lUalUI
LIBS += -L$(UAL_TIBETAN)/lib/$(UAL_ARCH)  	 -lTibetan
LIBS += -L$(UAL_SXF)/lib/$(UAL_ARCH) 		 -lUalSXF2
LIBS += -L$(UAL_ACCSIM)/lib/$(UAL_ARCH) 	 -lAccsim
LIBS += -L$(UAL_TEAPOT)/lib/$(UAL_ARCH) 	 -lTeapot
#LIBS += -L$(UAL_ETEAPOT)/lib/$(UAL_ARCH) 	 -lETeapot
LIBS += -L$(UAL_ETEAPOT_MltTurn)/lib/$(UAL_ARCH) -lETEAPOT_MltTurn
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)  		 -lPacSMF -lPacOptics -lPac
LIBS += -L$(UAL_ZLIB)/lib/$(UAL_ARCH) 		 -lZTps
LIBS += -L$(UAL)/codes/UAL/lib/$(UAL_ARCH) 	 -lUal
LIBS += -L$(SXF)/lib/$(UAL_ARCH) 		 -lSXF
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH) 		 -lpcre -lxml2 -lrfftw -lfftw
LIBS += 					 -lm

LIBS2 = $(UAL)/codes/ETEAPOT/lib/linux/obj/ElemSlice.o $(UAL)/codes/ETEAPOT/lib/linux/obj/DipoleData.o $(UAL)/codes/ETEAPOT/lib/linux/obj/MltData.o $(UAL)/codes/ETEAPOT/lib/linux/obj/BasicPropagator.o $(UAL)/codes/ETEAPOT/lib/linux/obj/BasicTracker.o $(UAL)/codes/ETEAPOT/lib/linux/obj/DriftTracker.o $(UAL)/codes/ETEAPOT/lib/linux/obj/MarkerTracker.o $(UAL)/codes/ETEAPOT/lib/linux/obj/MltTracker.o $(UAL)/codes/ETEAPOT/lib/linux/obj/TrackerFactory.o $(UAL)/codes/ETEAPOT/lib/linux/obj/RFCavityTracker.o $(UAL)/codes/ETEAPOT/lib/linux/obj/LossCollector.o $(UAL)/codes/ETEAPOT/lib/linux/obj/Eteapot.o $(UAL)/codes/ETEAPOT/lib/linux/obj/positionPrinter.o -L$(UAL)/codes/ETEAPOT/lib/linux -L$(UAL)/codes/PAC/lib/linux -L$(UAL)/codes/ZLIB/lib/linux -L$(UAL)/codes/UAL/lib/linux -lUal -ldl -L$(UAL)/codes/TEAPOT/lib/linux -lTeapot -L$(UAL)/codes/UAL/lib/linux -lUal -L$(UAL)/tools/lib/linux -lgsl -lgslcblas -L$(UAL)/codes/PAC/lib/linux -lPacSMF -L$(UAL)/codes/PAC/lib/linux -lPacOptics -L$(UAL)/codes/PAC/lib/linux -lPac  -L$(UAL)/codes/PAC/lib/linux -lPacSurvey -L$(UAL)/codes/ZLIB/lib/linux -lZTps -L$(UAL)/tools/lib/linux -lgsl -lgslcblas

OBJ_DIR  = ./lib/$(UAL_ARCH)/obj

#compile : ./tracker
#compile : ./libETeapot.so
 compile : ./libETeapot.so ./tracker

clean :
	rm -f $(OBJ_DIR)/* ./libETeapot.so ./tracker out/cpp/* trtrin OUT E* echo.sxf m_m Nu*

$(OBJ_DIR)/DipoleTracker.o :
	$(CC) -c DipoleTracker.cc -fPIC $(INC) -o $(OBJ_DIR)/DipoleTracker.o

$(OBJ_DIR)/tracker.o : 
	$(CC) -c tracker.cc $(INC) -o $(OBJ_DIR)/tracker.o

./libETeapot.so  : $(OBJ_DIR)/DipoleTracker.o
	$(DLD) $(DLDFLAGS) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/DipoleTracker.o $(LIBS2)

./tracker  : $(OBJ_DIR)/tracker.o
	$(CC) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/tracker.o $(LIBS) ./libETeapot.so

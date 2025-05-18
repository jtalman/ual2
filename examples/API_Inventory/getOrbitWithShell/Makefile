CC       = g++                       # Compiler
CCFLAGS  = -g -fpic -ansi -DsxfCheck -Wall -Wextra -fpermissive     # Compilation flags

DLD      = g++                       # Dynamic linker
DLDFLAGS = -shared                   # Dynamic linker flags
LD       = g++                       # Linker
LDFLAGS  =                           # Linker flags

INC += -I. -I./src
INC += -I$(UAL)/ext/UI/include
INC += -I$(UAL_SXF)/src
INC += -I$(UAL_ACCSIM)/src
INC += -I$(UAL_TEAPOT)/src
INC += -I$(UAL_ETEAPOT)/src
INC += -I$(UAL_ETEAPOT_MltTurn)/src
INC += -I$(UAL_ETEAPOT2)/src
INC += -I$(UAL_PAC)/src
INC += -I$(UAL)/codes/UAL/include
INC += -I$(UAL_ZLIB)/src
INC += -I$(UAL)/tools/include
INC += -I$(SXF)/src
INC += -I/home/ual/tools/gsl/include

LIBS += -L$(UAL)/ext/UI/lib/$(UAL_ARCH)  	 -lUalUI
LIBS += -L$(UAL_TIBETAN)/lib/$(UAL_ARCH)  	 -lTibetan
LIBS += -L$(UAL_SXF)/lib/$(UAL_ARCH) 		 -lUalSXF2
LIBS += -L$(UAL_ACCSIM)/lib/$(UAL_ARCH) 	 -lAccsim
LIBS += -L$(UAL_TEAPOT)/lib/$(UAL_ARCH) 	 -lTeapot
LIBS += -L$(UAL_ETEAPOT)/lib/$(UAL_ARCH) 	 -lETeapot
LIBS += -L$(UAL_ETEAPOT_MltTurn)/lib/$(UAL_ARCH) -lETEAPOT_MltTurn
LIBS += -L$(UAL_ETEAPOT2)/lib/$(UAL_ARCH)        -lETeapot2
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)  		 -lPacSMF -lPacOptics -lPac
LIBS += -L$(UAL_ZLIB)/lib/$(UAL_ARCH) 		 -lZTps
LIBS += -L$(UAL)/codes/UAL/lib/$(UAL_ARCH) 	 -lUal
LIBS += -L$(SXF)/lib/$(UAL_ARCH) 		 -lSXF
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH) 		 -lpcre -lxml2 -lrfftw -lfftw
LIBS += 					 -lm
LIBS += 					 -lgsl

SRC_DIR  = .
OBJ_DIR  = ./lib/linux/obj

compile: ./getOrbitWithShell

clean:
	rm -f ../working.orbit
	rm -f $(OBJ_DIR)/getOrbitWithShell.o ./getOrbitWithShell
	rm -rf m_m echo.sxf NikolayOut* out/*
	rm -f $(UAL)/codes/ETEAPOT2/lib/linux/libETeapot2.so

./getOrbitWithShell: $(UAL)/codes/ETEAPOT2/lib/linux/libETeapot2.so $(OBJ_DIR)/getOrbitWithShell.o
	$(LD) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/getOrbitWithShell.o $(OBJS) $(LIBS)

$(OBJ_DIR)/getOrbitWithShell.o : $(SRC_DIR)/getOrbitWithShell.cc
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@

$(UAL)/codes/ETEAPOT2/lib/linux/libETeapot2.so:
	\cp -f ./perSplitBendOutput.method $(UAL)/codes/ETEAPOT2/src/ETEAPOT2/Integrator/bendMethods
	( cd $(UAL)/codes/ETEAPOT2; make clean; make; echo "######################"; ls -l lib/linux/libETeapot2.so; echo "######################"; )

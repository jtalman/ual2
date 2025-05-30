CC       = g++                       # Compiler
CCFLAGS  = -g -fpic -ansi -Wall -Wextra -fpermissive     # Compilation flags

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
INC += -I$(UAL_PAC)/src
INC += -I$(UAL)/codes/UAL/include
INC += -I$(UAL_ZLIB)/src
INC += -I$(UAL)/tools/include
INC += -I$(SXF)/src
INC += -I/home/ual/tools/gsl/include

LIBS  =
LIBS += 					 -lgsl
LIBS += -L$(UAL)/ext/UI/lib/$(UAL_ARCH)  	 -lUalUI
LIBS += -L$(UAL_TIBETAN)/lib/$(UAL_ARCH)  	 -lTibetan
LIBS += -L$(UAL_SXF)/lib/$(UAL_ARCH) 		 -lUalSXF2
LIBS += -L$(UAL_ACCSIM)/lib/$(UAL_ARCH) 	 -lAccsim
LIBS += -L$(UAL_TEAPOT)/lib/$(UAL_ARCH) 	 -lTeapot
LIBS += -L$(UAL_ETEAPOT)/lib/$(UAL_ARCH) 	 -lETeapot
LIBS += -L$(UAL_ETEAPOT_MltTurn)/lib/$(UAL_ARCH) -lETEAPOT_MltTurn
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)  		 -lPacSMF -lPacOptics -lPac -lPacSurvey
LIBS += -L$(UAL_ZLIB)/lib/$(UAL_ARCH) 		 -lZTps
LIBS += -L$(UAL)/codes/UAL/lib/$(UAL_ARCH) 	 -lUal
LIBS += -L$(SXF)/lib/$(UAL_ARCH) 		 -lSXF
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH) 		 -lpcre -lxml2 -lrfftw -lfftw
LIBS += 					 -lm

SRC_DIR  = .
OBJ_DIR  = ./lib/$(UAL_ARCH)/obj

OBJS = $(OBJ_DIR)/positionPrinter.o \
	$(OBJ_DIR)/xmgracePrint.o

 compile : $(OBJS) ./evolver ./transferMatrix ./determineTwiss ./transferMatrices ./orbitsWithSpin ./ETEAPOT_MltTurn 
#compile : $(OBJS) ./evolver ./transferMatrix ./determineTwiss ./transferMatrices ./orbitsWithSpin ./ETEAPOT_MltTurn ./spinOrbits
#compile : $(OBJS) ./evolver ./transferMatrix ./determineTwiss ./transferMatrices ./orbitsWithSpin ./ETEAPOT_MltTurn ./manualTune ./spinOrbits

clean :
	rm -f $(OBJS) $(OBJ_DIR)/evolver.o ./evolver $(OBJ_DIR)/transferMatrix.o ./transferMatrix $(OBJ_DIR)/determineTwiss.o ./determineTwiss $(OBJ_DIR)/transferMatrices.o ./transferMatrices $(OBJ_DIR)/orbitsWithSpin.o ./orbitsWithSpin $(OBJ_DIR)/ETEAPOT_MltTurn.o ./ETEAPOT_MltTurn $(OBJ_DIR)/manualTune.o ./manualTune $(OBJ_DIR)/spinOrbits.o ./spinOrbits

outputReady :
	rm -rf betaFunctions* OUT* initialSpin* NikolayOut* IG* out/cpp/* out/STT/* out/TWISS/* out/VERIF/*

./evolver  : $(OBJ_DIR)/evolver.o $(OBJS)
	$(LD) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/evolver.o $(OBJS) $(LIBS)

./transferMatrix  : $(OBJ_DIR)/transferMatrix.o $(OBJS)
	$(LD) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/transferMatrix.o $(OBJS) $(LIBS)

./determineTwiss  : $(OBJ_DIR)/determineTwiss.o $(OBJS)
	$(LD) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/determineTwiss.o $(OBJS) $(LIBS)

./transferMatrices  : $(OBJ_DIR)/transferMatrices.o $(OBJS)
	$(LD) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/transferMatrices.o $(OBJS) $(LIBS)

./orbitsWithSpin  : $(OBJ_DIR)/orbitsWithSpin.o $(OBJS)
	$(LD) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/orbitsWithSpin.o $(OBJS) $(LIBS)

./ETEAPOT_MltTurn  : $(OBJ_DIR)/ETEAPOT_MltTurn.o $(OBJS)
	$(LD) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/ETEAPOT_MltTurn.o $(OBJS) $(LIBS)

./sxfUtility: $(OBJ_DIR)/sxfUtility.o
	$(LD) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/sxfUtility.o $(LIBS)

./manualTune: $(OBJ_DIR)/manualTune.o
	$(LD) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/manualTune.o $(OBJS) $(LIBS)

./spinOrbits: $(OBJ_DIR)/spinOrbits.o
	$(LD) -o $@  $(LDFLAGS) $(INC) $(OBJ_DIR)/spinOrbits.o $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@


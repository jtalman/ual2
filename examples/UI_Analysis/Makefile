CC       = g++                       # Compiler
CCFLAGS  = -g -fpic -ansi -Wall      # Compilation flags
LD       = g++                       # Linker
LDFLAGS  =                           # Linker flags

INC += -I. -I./src
INC += -I$(UAL)/ext/UI/include
INC += -I$(UAL_SXF)/src 
INC += -I$(UAL_SIMBAD)/src 
INC += -I$(UAL_ACCSIM)/src 
INC += -I$(UAL_TEAPOT)/src 
INC += -I$(UAL_PAC)/src 
INC += -I$(UAL)/codes/UAL/include
INC += -I$(UAL_ZLIB)/src
INC += -I$(UAL)/tools/include 
INC += -I$(SXF)/src 
INC += -I/home/ual/tools/gsl/include
 
LIBS += -L$(UAL)/ext/UI/lib/$(UAL_ARCH)  	-lUalUI
LIBS += -L$(UAL_SIMBAD)/lib/$(UAL_ARCH)  	-lSimbad
LIBS += -L$(UAL_TIBETAN)/lib/$(UAL_ARCH)  	-lTibetan
LIBS += -L$(UAL_SXF)/lib/$(UAL_ARCH) 		-lUalSXF2
LIBS += -L$(UAL_ACCSIM)/lib/$(UAL_ARCH) 	-lAccsim 
LIBS += -L$(UAL_TEAPOT)/lib/$(UAL_ARCH) 	-lTeapot 
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)  		-lPacSMF -lPacOptics -lPac
LIBS += -L$(UAL_ZLIB)/lib/$(UAL_ARCH) 		-lZTps
LIBS += -L$(UAL)/codes/UAL/lib/$(UAL_ARCH) 	-lUal
LIBS += -L$(SXF)/lib/$(UAL_ARCH) 		-lSXF
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH) 		-lpcre -lxml2 -lrfftw -lfftw
LIBS += 					-lm

compile: ./shell_sns

clean:
	rm -f ./lib/linux/obj/shell_sns.o ./shell_sns

./shell_sns: ./lib/linux/obj/shell_sns.o
	$(LD) -o $@  $(LDFLAGS) $(INC) ./lib/linux/obj/shell_sns.o $(LIBS)

./lib/linux/obj/shell_sns.o: ./shell_sns.cc
	$(CC) -c $< -o $@ $(CCFLAGS) $(INC)

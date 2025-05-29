CC      = g++
CCFLAGS = -g -ansi -fpic -fpermissive -Wall

                                             # PAC, Shell, sxf all prerequisites!
INC     = -I$(UAL_PAC)/src                   # PAC
INC    += -I$(UAL)/ext/UI/src                #      ShellImpl
INC    += -I$(SXF)/src                       #             sxf 
INC    += -I$(UAL_SXF)/src                   #             sxf

INC    += -I$(UAL_TEAPOT)/src
INC    += -I$(UAL)/codes/UAL/src
INC    += -I$(UAL)/tools/include

LIBS   += -L$(UAL_PAC)/lib/$(UAL_ARCH)        -lPacSMF -lPacOptics -lPac
LIBS   += -L$(UAL)/ext/UI/lib/$(UAL_ARCH)     -lUalUI
LIBS   += -L$(UAL)/codes/ZLIB/lib/$(UAL_ARCH) -lZTps

LIBS   += -L$(SXF)/lib/$(UAL_ARCH)            -lSXF
LIBS   += -L$(UAL_SXF)/lib/$(UAL_ARCH)        -lUalSXF2

LIBS   += -L$(UAL_TEAPOT)/lib/$(UAL_ARCH)     -lTeapot
LIBS   += -L$(UAL)/codes/UAL/lib/$(UAL_ARCH)  -lUal
LIBS   += -L$(UAL)/tools/lib/$(UAL_ARCH)      -lpcre

#LIBS   += -lm

compile: ./tunefitWithTeapot

clean:
	rm -f ./tunefitWithTeapot.o ./tunefitWithTeapot
	rm -f $(UAL)/codes/TEAPOT/lib/linux/libTeapot.so

./tunefitWithTeapot: $(UAL)/codes/TEAPOT/lib/linux/libTeapot.so ./tunefitWithTeapot.o
	$(CC) -o $@  $(INC) ./tunefitWithTeapot.o $(LIBS)

./tunefitWithTeapot.o: ./tunefitWithTeapot.cc
	$(CC)  $(CCFLAGS) $(INC) -c $< -o $@ -I$(UAL)/codes/ZLIB/src

$(UAL)/codes/TEAPOT/lib/linux/libTeapot.so:
	\cp -f ./TeapotElement.cc $(UAL)/codes/TEAPOT/src/Integrator
	( cd $(UAL)/codes/TEAPOT; make clean; make; echo "######################"; ls -l lib/linux/libTeapot.so; echo "######################"; )

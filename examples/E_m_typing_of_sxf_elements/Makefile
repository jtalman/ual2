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
INC += -I$(UAL_E4_interim)/src
INC += -I$(UAL_E_m)/include
INC += -I$(UAL_PAC)/src
INC += -I$(UAL_PAC)/include
INC += -I$(UAL)/codes/UAL/include
INC += -I$(UAL_ZLIB)/src
INC += -I$(UAL)/tools/include
INC += -I$(SXF)/src
INC += -I/home/ual/tools/gsl/include

LIBS  =
LIBS += -L$(UAL)/ext/UI/lib/$(UAL_ARCH)  	 -lUalUI
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH)  	 -lgsl
LIBS += -L$(UAL_TIBETAN)/lib/$(UAL_ARCH)  	 -lTibetan
LIBS += -L$(UAL_SXF)/lib/$(UAL_ARCH) 		 -lUalSXF2
LIBS += -L$(UAL_ACCSIM)/lib/$(UAL_ARCH) 	 -lAccsim
LIBS += -L$(UAL_TEAPOT)/lib/$(UAL_ARCH) 	 -lTeapot
LIBS += -L$(UAL_ETEAPOT)/lib/$(UAL_ARCH) 	 -lETeapot
LIBS += -L$(UAL_ETEAPOT_MltTurn)/lib/$(UAL_ARCH) -lETEAPOT_MltTurn
LIBS += -L$(UAL_ETEAPOT2)/lib/$(UAL_ARCH)        -lETeapot2
LIBS += -L$(UAL_E4_interim)/lib/$(UAL_ARCH)      -lE4_interim
LIBS += -L$(UAL_E_m)/lib/$(UAL_ARCH)             -lE_m
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)  		 -lPac
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)  		 -lPacOptics
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH)  		 -lPacSMF
LIBS += -L$(UAL_ZLIB)/lib/$(UAL_ARCH) 		 -lZTps
LIBS += -L$(UAL)/codes/UAL/lib/$(UAL_ARCH) 	 -lUal
LIBS += -L$(SXF)/lib/$(UAL_ARCH) 		 -lSXF
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH) 		 -lpcre
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH) 		 -lxml2
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH) 		 -lrfftw
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH) 		 -lfftw
LIBS += 					 -lm

compile: ./typing_of_sxf_elements ./E_m_Survey++

clean:
	rm -f typing_of_sxf_elements.o ./typing_of_sxf_elements ./E_m_Survey++.o ./E_m_Survey++
	rm -f echo.sxf

typing_of_sxf_elements: typing_of_sxf_elements.o
	$(LD) -o $@  $(LDFLAGS) $(INC) typing_of_sxf_elements.o $(LIBS)

typing_of_sxf_elements.o: typing_of_sxf_elements.cc
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@

E_m_Survey++: E_m_Survey++.o
	$(LD) -o $@  $(LDFLAGS) $(INC) E_m_Survey++.o $(LIBS)

E_m_Survey++.o: E_m_Survey++.cc
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@

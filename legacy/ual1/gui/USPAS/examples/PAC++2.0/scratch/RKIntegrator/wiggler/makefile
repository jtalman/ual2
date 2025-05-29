CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC)

CFLAGS     = $(INCLUDES)
COMPILE    = gcc -g -c  

LIBRARIES  = -L$(LIB) -lField -lZlib -lMonitor -lBeam -lNumLib -lm
LFLAGS     = $(LIBRARIES)
LOAD       = g++  

OBJ   = wiggler24.o

wiggler24   : $(OBJ) 
	$(LOAD) -o wiggler24 $(OBJ) $(LFLAGS) 

wiggler24.o : wiggler24.cc
	$(COMPILE) $(CFLAGS)  wiggler24.cc 


CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC)

CFLAGS     = $(INCLUDES)
COMPILE    = gcc -c 

LIBRARIES  = -L$(LIB) -lTeapot -lMonitor -lBeam -lNumLib -lm
LFLAGS     = $(LIBRARIES)
LOAD       = g++  

OBJ   = main.o

main : $(OBJ) 
	$(LOAD) -o main $(OBJ) $(LFLAGS) 

main.o : main.cc
	$(COMPILE) $(CFLAGS)  main.cc 


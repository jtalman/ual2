CORE       = /home/cfsa/malitsky/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC)

CFLAGS     = $(INCLUDES)
COMPILE    = g++ -c 

LIBRARIES  = -L$(LIB) -lVectorA -lField -lLieMap -lZlib -lMonitor -lBeam \
                      -lNumLib -lm
LFLAGS     = $(LIBRARIES)
LOAD       = g++  

OBJ   = main.o

main : $(OBJ) 
	$(LOAD) -o main $(OBJ) $(LFLAGS) 

main.o : main.cc
	$(COMPILE) $(CFLAGS)  main.cc 


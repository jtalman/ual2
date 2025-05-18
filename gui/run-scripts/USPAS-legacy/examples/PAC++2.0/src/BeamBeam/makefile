CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC)

CFLAGS     = $(INCLUDES)
COMPILE    = gcc -g 

LIBRARIES  = -L$(LIB) -lMonitor -lBeam -lNumLib -lm
LFLAGS     = $(INCLUDES)  $(LIBRARIES)
LOAD       = g++

SRCS        = main.cc \
              Bunch.cc 

OBJS        = main.o \
              Bunch.o 

all  : main

main : $(OBJS) 
	$(LOAD) -o $@ $(OBJS) $(LFLAGS)

%.o : %.cc 
	$(COMPILE) $(CFLAGS) -c -o  $@ $<

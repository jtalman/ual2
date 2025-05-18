CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = -L$(CORE)/lib 

INCLUDES   = -I$(SRC) 
CFLAGS     = $(INCLUDES)
COMPILE    = gcc -c 

LIBRARIES  = $(LIB) -lMonitor -lBeam -lNumLib -lm
LFLAGS     = $(LIBRARIES)
LOAD       = g++  

TEST_OBJ   = test.o

test : $(TEST_OBJ) 
	$(LOAD) -o test $(TEST_OBJ) $(LFLAGS) 

test.o : test.cc 
	$(COMPILE) $(CFLAGS)  test.cc 

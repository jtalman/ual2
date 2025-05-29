CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC) 
CFLAGS     = $(INCLUDES)
COMPILE    = gcc -g -c 

LIBRARIES  = -L$(LIB) -lZlib -lMonitor -lBeam -lNumLib -lm 
LFLAGS     = $(LIBRARIES)
LOAD       = g++ -g  

TEST_OBJ   = zlib.o

zlib : $(TEST_OBJ) 
	$(LOAD) -o zlib $(TEST_OBJ) $(LFLAGS) 

zlib.o : zlib.cc 
	$(COMPILE) $(CFLAGS)  zlib.cc 
Zbessi.o : Zbessi.cc 
	$(COMPILE) $(CFLAGS)  Zbessi.cc 
zlib_inv.o : zlib_inv.cc 
	$(COMPILE) $(CFLAGS)  zlib_inv.cc 
zlib_sqrt.o : zlib_sqrt.cc 
	$(COMPILE) $(CFLAGS)  zlib_sqrt.cc 

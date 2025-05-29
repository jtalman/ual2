CORE       = /home/owl/guest/ssc/PAC++
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC)

CFLAGS     = $(INCLUDES)
COMPILE    = CC -c 

LIBRARIES  = -L$(LIB)  -lZmap -lTeapot -lPac -lElement -lZlib -lMonitor -lBeam -lNumLib
LFLAGS     = $(LIBRARIES)
LOAD       = CC  

OBJ   = leb++.o

leb++ : $(OBJ) 
	$(LOAD) -o leb++ $(OBJ) $(LFLAGS) 

leb++.o : leb++.cc 
	$(COMPILE) $(CFLAGS)  leb++.cc 


CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC) 
CFLAGS     = $(INCLUDES)
COMPILE    = gcc -c 


OBJS =  LieMap.o     

$(LIB)/libLieMap.a : $(OBJS)
	rm -f   $(LIB)/libLieMap.a
	ar cru  $(LIB)/libLieMap.a $(OBJS)
	ranlib  $(LIB)/libLieMap.a

LieMap.o : LieMap.cc LieMap.hh  
	$(COMPILE) $(CFLAGS)  LieMap.cc 

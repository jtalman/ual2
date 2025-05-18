CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC) 
CFLAGS     = $(INCLUDES)
COMPILE    = gcc -c 


OBJS =  VectorA.o     

$(LIB)/libVectorA.a : $(OBJS)
	rm -f   $(LIB)/libVectorA.a
	ar cru  $(LIB)/libVectorA.a $(OBJS)
	ranlib  $(LIB)/libVectorA.a

VectorA.o : VectorA.cc VectorA.hh  
	$(COMPILE) $(CFLAGS)  VectorA.cc 

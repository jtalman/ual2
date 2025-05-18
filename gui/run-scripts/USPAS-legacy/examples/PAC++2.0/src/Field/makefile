CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC) 
CFLAGS     = $(INCLUDES)
COMPILE    = gcc -c 


OBJS =  Field.o     

$(LIB)/libField.a : $(OBJS)
	rm -f   $(LIB)/libField.a
	ar cru  $(LIB)/libField.a $(OBJS)
	ranlib  $(LIB)/libField.a

Field.o : Field.cc Field.hh  
	$(COMPILE) $(CFLAGS)  Field.cc 

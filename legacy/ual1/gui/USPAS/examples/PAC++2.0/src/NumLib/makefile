CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC)
CFLAGS     = $(INCLUDES)
COMPILE    = gcc -c 

OBJS       = NumLib.o      

$(LIB)/libNumLib.a : $(OBJS)
	rm -f   $(LIB)/libNumLib.a
	ar cru  $(LIB)/libNumLib.a $(OBJS)
	ranlib  $(LIB)/libNumLib.a

NumLib.o : NumLib.cc NumLib.hh
	$(COMPILE) $(CFLAGS)  NumLib.cc 

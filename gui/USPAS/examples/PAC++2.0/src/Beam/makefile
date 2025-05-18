CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC) 
CFLAGS     = $(INCLUDES)
COMPILE    = gcc -c 


OBJS =  Beam.o     

$(LIB)/libBeam.a : $(OBJS)
	rm -f   $(LIB)/libBeam.a
	ar cru  $(LIB)/libBeam.a $(OBJS)
	ranlib  $(LIB)/libBeam.a

Beam.o : Beam.cc Beam.hh BeamDef.hh 
	$(COMPILE) $(CFLAGS)  Beam.cc 

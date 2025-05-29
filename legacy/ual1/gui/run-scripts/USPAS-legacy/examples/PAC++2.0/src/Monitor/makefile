CORE       = /A/lns101/nfs/homes/cesr/malitsky/tools/codes/PAC++2.0
SRC        = $(CORE)/src
LIB        = $(CORE)/lib

INCLUDES   = -I$(SRC)
CFLAGS     = $(INCLUDES)
COMPILE    = gcc -c 

OBJS       =  Particle.o    

$(LIB)/libMonitor.a : $(OBJS)
	rm -f   $(LIB)/libMonitor.a
	ar cru  $(LIB)/libMonitor.a $(OBJS)
	ranlib  $(LIB)/libMonitor.a

Monitor.o : Monitor.cc Monitor.hh 
	$(COMPILE) $(CFLAGS)  Monitor.cc
Particle.o : Particle.cc Particle.hh 
	$(COMPILE) $(CFLAGS)  Particle.cc
Twiss.o : Twiss.cc Twiss.hh 
	$(COMPILE) $(CFLAGS)  Twiss.cc
TwissParameters.o : TwissParameters.cc TwissParameters.hh 
	$(COMPILE) $(CFLAGS)  TwissParameters.cc
Rmatrix.o : Rmatrix.cc Rmatrix.hh 
	$(COMPILE) $(CFLAGS)  Rmatrix.cc


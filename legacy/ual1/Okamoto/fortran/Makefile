SUBR_FILES = cmplxerf.f
SUBR_OBJS = cmplxerf.o

MAIN_OBJS = gafeld.o fnctnw.o testbmbm.o

all: gafeld fnctnw testbmbm
	rm -f $(SUBR_OBJS) $(MAIN_OBJS)

testbmbm: testbmbm.f $(SUBR_OBJS)
	gfortran $(SUBR_OBJS) -o testbmbm testbmbm.f

gafeld: gafeld.f $(SUBR_OBJS)
	gfortran $(SUBR_OBJS) -o gafeld gafeld.f

fnctnw: fnctnw.f $(SUBR_OBJS)
	gfortran $(SUBR_OBJS) -o fnctnw fnctnw.f

cmplxerf.o: cmplxerf.f
	gfortran -c cmplxerf.f

clean:
	rm -f $(SUBR_OBJS) $(MAIN_OBJS) fort.* a.out testbmbm fnctnw gafeld

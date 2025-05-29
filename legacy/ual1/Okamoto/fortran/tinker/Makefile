SUBR_OBJS = cmplxerf.o

MAIN_OBJS = fnctnw-slightly-modified.o

all: fnctnw-slightly-modified
	rm -f $(SUBR_OBJS) $(MAIN_OBJS)

fnctnw-slightly-modified: fnctnw-slightly-modified.f $(SUBR_OBJS)
	gfortran $(SUBR_OBJS) -o fnctnw-slightly-modified fnctnw-slightly-modified.f

cmplxerf.o: ../cmplxerf.f
	gfortran -c ../cmplxerf.f

clean:
	rm -f $(SUBR_OBJS) $(MAIN_OBJS) wexct fnctnw-slightly-modified

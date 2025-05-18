include $(UAL_E4_interim)/src/Makefile.config

INC += -I$(UAL_PAC)/include/
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH) -lPac

include ./Objects

compile : $(OBJS)
	if [ ! -d $(UAL_E4_interim)/include/E4_interim/Integrator ] ; \
		then mkdir $(UAL_E4_interim)/include/E4_interim/Integrator ; fi;
	cp *.hh $(UAL_E4_interim)/include/E4_interim/Integrator/.

clean:	
	rm -f $(OBJS) 
	rm -rf $(UAL_E4_interim)/include/E4_interim/Integrator

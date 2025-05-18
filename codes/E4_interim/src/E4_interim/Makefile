include $(UAL_E4_interim)/src/Makefile.config

INC  += -I$(UAL_PAC)/src
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH) -lPac

DIRS = ./Integrator

OBJS = 

include ./Integrator/Objects

compile : 
	if [ ! -d $(UAL_E4_interim)/include/E4_interim ] ; \
		then (mkdir $(UAL_E4_interim)/include/E4_interim;); fi;
	@for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then $(MAKE) compile; fi;); \
	done

clean:
	@for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then $(MAKE) clean; fi;); \
	done 	

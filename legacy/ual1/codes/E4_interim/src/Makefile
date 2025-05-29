include $(UAL_E4_interim)/src/Makefile.config

DIRS =  ./E4_interim

LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH) -lPacSMF
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH) -lPacOptics 
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH) -lPac 
LIBS += -L$(UAL_PAC)/lib/$(UAL_ARCH) -lPacSurvey
LIBS += -L$(UAL_ZLIB)/lib/$(UAL_ARCH) -lZTps
LIBS += -L$(UAL)/tools/lib/$(UAL_ARCH)		-lgsl -lgslcblas

include ./E4_interim/Integrator/Objects

compile : 
	if [ ! -d $(UAL_E4_interim)/include ] ; \
		then (mkdir $(UAL_E4_interim)/include;); fi;
	(if [ !  -d $(UAL_E4_interim)/lib ]; \
		then mkdir  $(UAL_E4_interim)/lib; fi;)
	(if [ !  -d $(UAL_E4_interim)/lib/$(UAL_ARCH) ]; \
		then mkdir  $(UAL_E4_interim)/lib/$(UAL_ARCH); fi;)
	(if [ !  -d $(UAL_E4_interim)/lib/$(UAL_ARCH)/obj ]; \
		then mkdir  $(UAL_E4_interim)/lib/$(UAL_ARCH)/obj; fi;)
	@for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then $(MAKE) compile; fi;); \
	done
	$(LD) $(LDFLAGS) $(INC) -o $(UAL_E4_interim)/lib/$(UAL_ARCH)/obj/dummy   \
		./dummy.cc $(OBJS) $(LIBS)
	cp /dev/null $(UAL_E4_interim)/lib/$(UAL_ARCH)/obj/dummy
	$(DLD) $(DLDFLAGS) -o $(UAL_E4_interim)/lib/$(UAL_ARCH)/libE4_interim.so $(OBJS) $(LIBS)

clean :
	@for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then $(MAKE) clean; fi;); \
	done 
	rm -rf $(UAL_E4_interim)/include
	rm -rf $(UAL_E4_interim)/lib/$(UAL_ARCH)

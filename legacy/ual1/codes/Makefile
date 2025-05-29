DIRS = ./UAL ./ZLIB ./PAC ./TEAPOT ./ETEAPOT ./ETEAPOT_MltTurn ./ETEAPOT2 ./E4_interim ./ACCSIM ./SPINK ./TIBETAN ./SIMBAD ./AIM # ./E_m ##./ICE ./AIM 

#ifdef BETACOOL
#DIRS += ./BETACOOL
#endif

compile:  
	@for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then $(MAKE) compile; fi;); \
	done

clean: 
	@for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then $(MAKE) clean; fi;); \
	done

include $(UAL)/gui/ROOT/src/Makefile.config

MY_DIR  := $(UAL)/gui/ROOT/
DIRS = ./Viewers

OBJS = 

include ./Viewers/Objects

compile : 
	(if [ !  -d $(MY_DIR)/include/UAL/ROOT ]; \
		then mkdir  $(MY_DIR)/include/UAL/ROOT; fi;)
	@for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then $(MAKE) compile; fi;); \
	done
	$(DLD) $(DLDFLAGS) -o $(MY_DIR)/lib/linux/libUalRootViewers.so $(OBJS) $(LIBS)

clean :
	@for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then $(MAKE) clean; fi;); \
	done 
	rm -rf $(MY_DIR)/include/UAL/ROOT
	rm -f $(MY_DIR)/lib/linux/libUalRootViewers.so

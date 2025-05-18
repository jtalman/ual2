include $(UAL)/gui/QT/src/Makefile.config

MY_DIR  := $(UAL)/gui/QT/

DIRS    := ./UAL

compile :
	(if [ !  -d $(MY_DIR)/include ]; then mkdir  $(MY_DIR)/include; fi;)
	(if [ !  -d $(MY_DIR)/lib ]; \
		then mkdir  $(MY_DIR)/lib; fi;)	
	(if [ !  -d $(MY_DIR)/lib/linux ]; \
		then mkdir  $(MY_DIR)/lib/linux; fi;)
	(if [ !  -d $(MY_DIR)/lib/linux/obj ]; \
		then mkdir  $(MY_DIR)/lib/linux/obj; fi;)
	@for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then $(MAKE) compile; fi;); \
	done

clean :
	@for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then $(MAKE) clean; fi;); \
	done 
	rm -rf $(MY_DIR)/include
	rm -rf $(MY_DIR)/lib/linux

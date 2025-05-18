MFILES = working.map*
TFILES = working.twiss*
OFILES = working.orbit*
DIRS = ./getMapWithOpticsCalculator ./getTwissWithShell ./getOrbitWithShell

compile:  
	for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then make compile; fi;); \
	done

clean: 
	rm -f $(MFILES)
	rm -f $(TFILES)
	rm -f $(OFILES)
	rm -f echo.sxf
	rm -f NikolayOut
	rm -f m_m
	rm -f map*
	for dir in $(DIRS); do \
		(cd $$dir; if [ -f ./Makefile ]; then make clean; fi;); \
	done

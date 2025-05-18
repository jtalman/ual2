INC  = 
INC += -I$(UAL)/codes/PAC/src/
INC += -I$(UAL)/codes/UAL/src/
INC += -I$(UAL)/codes/ZLIB/src/
INC += -I$(UAL)/codes/ETEAPOT/src/
INC += -I$(UAL)/codes/ETEAPOT2/src/

compile: quad.o TrackerFactory.o

%.o: %.cc
	gcc -c $< $(INC) -o $@

clean:
	rm -rf quad.o

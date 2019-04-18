CXX=g++
CXXFLAGS=-g -Wall -std=c++11
# Uncomment for parser DEBUG
#DEFS=-DDEBUG

OBJS= bin/floorplan.o

all: floorplan

bin:
	mkdir bin
floorplan: bin $(OBJS)
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ $(OBJS)
bin/floorplan.o: floorplan.cpp avlbst.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c floorplan.cpp
 

clean:
	rm -f *.o flooplan
	rm bin/*.o
	rmdir bin
	
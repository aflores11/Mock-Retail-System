CXX=g++
CXXFLAGS=-g -Wall -std=c++11
# Uncomment for parser DEBUG
#DEFS=-DDEBUG

OBJS = puzzle.o board.o puzzle_heur.o

all: puzzle

bin:
	mkdir bin
puzzle: bin $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@
bin/board.o: board.h board.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c board.cpp
bin/puzzle_heur.o: puzzle_heur.h puzzle_heur.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c puzzle_heur.cpp
bin/puzzle_move.o: puzzle_move.h puzzle_move.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c puzzle_move.cpp	

vgnd:
	valgrind --tool=memcheck -v --leak-check=yes ./amazon 

clean:
	rm -f *.o puzzle
	rm bin/*.o
	rmdir bin
	
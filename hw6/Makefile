CXX=g++
CXXFLAGS=-g -Wall -std=c++11
# Uncomment for parser DEBUG
#DEFS=-DDEBUG

OBJS = bin/puzzle.o bin/board.o bin/puzzle_heur.o bin/puzzle_move.o bin/puzzle_solver.o 

all: puzzle

bin:
	mkdir bin
puzzle: bin $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@
bin/board.o: board.h board.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c board.cpp
bin/puzzle_heur.o: puzzle_heur.h puzzle_heur.cpp board.h
	$(CXX) $(CXXFLAGS) -o $@ -c puzzle_heur.cpp
bin/puzzle_move.o: puzzle_move.h puzzle_move.cpp board.h
	$(CXX) $(CXXFLAGS) -o $@ -c puzzle_move.cpp	
bin/puzzle_solver.o: puzzle_solver.h puzzle_solver.cpp board.cpp puzzle_heur.cpp puzzle_move.cpp
	$(CXX) $(CXXFLAGS) -c puzzle_solver.cpp -o $@
bin/puzzle.o: puzzle.cpp board.h puzzle_solver.h
	$(CXX) $(CXXFLAGS) -o $@ -c puzzle.cpp	

vgnd:
	valgrind --tool=memcheck --leak-check=yes -v  ./puzzle 4 30 1537 0

clean:
	rm -f *.o puzzle
	rm bin/*.o
	rmdir bin
	
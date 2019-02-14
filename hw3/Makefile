all: stringparser

stringparser: stringparser.cpp bin/ulliststr.o bin/stackstring.o
	g++ -g -Wall -std=c++11 stringparser.cpp bin/ulliststr.o bin/stackstring.o -o stringparser

bin/ulliststr.o: ulliststr.h ulliststr.cpp
	g++ -g -Wall -std=c++11 -c ulliststr.cpp -o bin/ulliststr.o	

sparser_vg:
	valgrind --tool=memcheck --leak-check=yes ./stringparser input.txt output.txt	

bin/stackstring.o: stackstring.h stackstring.cpp
	 g++ -g -Wall -std=c++11 -c stackstring.cpp -o bin/stackstring.o
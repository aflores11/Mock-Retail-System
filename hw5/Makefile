CXX=g++
CXXFLAGS=-g -Wall -std=c++11
# Uncomment for parser DEBUG
#DEFS=-DDEBUG

OBJS= bin/user.o bin/db_parser.o bin/product.o bin/product_parser.o bin/util.o bin/book.o bin/movie.o bin/clothing.o bin/mydatastore.o bin/review.o bin/amazon.o


all: amazon

bin:
	mkdir bin
amazon: bin $(OBJS)
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ $(OBJS)
bin/user.o: user.cpp user.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c user.cpp
bin/db_parser.o: db_parser.cpp db_parser.h product.h product_parser.h user.h datastore.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c db_parser.cpp
bin/product.o: product.cpp product.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c product.cpp
bin/util.o: util.cpp util.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c util.cpp
bin/book.o: book.h book.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c book.cpp
bin/movie.o: movie.h movie.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c movie.cpp	
bin/clothing.o: clothing.h clothing.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c clothing.cpp	
bin/product_parser.o: product_parser.cpp product_parser.h product.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c product_parser.cpp
bin/review.o: review.h review.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c review.cpp	
bin/mydatastore.o: mydatastore.h mydatastore.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c mydatastore.cpp	
bin/amazon.o: amazon.cpp db_parser.h datastore.h product_parser.h 
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c amazon.cpp	
vgnd:
	valgrind --tool=memcheck -v --leak-check=yes ./amazon database.txt 

clean:
	rm -f *.o amazon
	rm bin/*.o
	rmdir bin

Name: Angel Flores
ID: 3518 767 555
Email: angelflo@usc.edu


Problems 4 and 5 are uploaded through pdf files on github.

g++ -g -Wall -std=c++11 split_test.cpp split.cpp -o split_test

g++ -g -Wall -std=c++11 ulliststr_test.cpp ulliststr.cpp -o ulliststr_test

valgrind --tool=memcheck --leak-check=yes ./split_test

valgrind --tool=memcheck --leak-check=yes ./ulliststr_test




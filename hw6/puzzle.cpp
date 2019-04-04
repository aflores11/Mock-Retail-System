#include <iostream>
#include <cstdlib>
#include <deque>
#include "board.h"
#include <sstream>
#include <string>
#include "puzzle_heur.h"
#include "puzzle_solver.h"
#include <algorithm>
#include <exception>

using namespace std;

int main(int argc, char *argv[])
{
  if(argc < 5){
    cerr << "Usage: ./puzzle size initMoves seed heur" << endl;
    return 1;
  }
  int size = std::stoi(argv[1]);
  int initMoves = std::stoi(argv[2]);
  int seed = std::stoi(argv[3]);
  int heur = std::stoi(argv[4]);

  cout << size << " " << initMoves << " " << seed << " " << heur<< endl;
















  return 0;
}

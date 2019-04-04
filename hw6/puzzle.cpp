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
#include <deque>
struct notanumber{};

void givememyheur(PuzzleHeuristic* &in, int &num);

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

  
  
   Board* game = new Board(size, initMoves, seed);

  cout << *game << endl; 

  bool endgame = false;

  while(!endgame)
  {
    std::cout<< "Enter tile number to move or -1 for a cheat:";
    int playermove;

    cin >> playermove;

    try
    {
      if(cin.fail()) throw notanumber();
    }
    catch(notanumber &e)
    {
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;

    }

    if(playermove == 0) endgame =true;

    else if(playermove == -1)
    {
      PuzzleHeuristic* myheur;
      givememyheur(myheur, heur);
      PuzzleSolver* cheat = new PuzzleSolver(*game, myheur);
      cheat->run();
      deque<int> sol = cheat->getSolution();
      cout << "Try this sequence:\n";
      while(!sol.empty())
      {
        cout << sol.back() << " ";
        sol.pop_back();
      }
      cout << endl;
      int exp = cheat->getNumExpansions();
      cout << "(Expansions = " << exp << ")" << endl << endl;
      cout << *game << endl;
      delete myheur;
      delete cheat;
    }

    else if(playermove>0 && playermove<game->size())
    {
      game->move(playermove);
      cout << *game << endl;
      if(game->solved()) endgame = true;
    }

    else
    {
      continue;
    }


  }

  delete game;

  return 0;
}

void givememyheur(PuzzleHeuristic* &in, int &num)
{
  if(num ==0) in = new PuzzleBFSHeuristic;
  else if(num == 1) in = new PuzzleOutOfPlaceHeuristic;
  else in = new PuzzleManhattanHeuristic;
}
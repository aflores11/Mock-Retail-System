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

  /*set up the board */
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

    /*checks if input is a valid number*/
    try
    {
      if(cin.fail()) throw notanumber();
    }
    catch(notanumber &e)
    {
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // resets cin to be able to use again
      continue;

    }

    //player wants to exit game
    if(playermove == 0) endgame =true;

    //player wants a cheat
    else if(playermove == -1)
    {
      PuzzleHeuristic* myheur;
      givememyheur(myheur, heur); //helper function to get appropriate heuristic
      PuzzleSolver* cheat = new PuzzleSolver(*game, myheur); //allocates A*
      cheat->run();
      deque<int> sol = cheat->getSolution(); 
      cout << "Try this sequence:\n";
      while(!sol.empty()) //displays solution from back to front as they are push from last to start
      {
        cout << sol.back() << " ";
        sol.pop_back();
      }
      cout << endl;
      int exp = cheat->getNumExpansions();
      cout << "(Expansions = " << exp << ")" << endl << endl;
      cout << *game << endl;
      /*cleans up my allocations*/
      delete myheur; 
      delete cheat;
    }

    /*makes sure that the integer inputed is in the range of tile values available*/
    else if(playermove>0 && playermove<game->size())
    {
      /* trys to make a valid move next to blank space */
      try{
      game->move(playermove);
      cout <<endl;
      cout << *game << endl;
      if(game->solved()) endgame = true;
      }
      catch(BoardMoveError &m)
      {
        cout << m.what();
        continue;
      }
    }

    else
    {
      continue;
    }


  }
  //clean after my memory
  delete game;

  return 0;
}

void givememyheur(PuzzleHeuristic* &in, int &num)
{
  if(num ==0) in = new PuzzleBFSHeuristic;
  else if(num == 1) in = new PuzzleOutOfPlaceHeuristic;
  else in = new PuzzleManhattanHeuristic;
}
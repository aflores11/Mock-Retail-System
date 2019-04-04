#ifndef PUZZLEMOVE_H
#define PUZZLEMOVE_H
#include "board.h"
#include "puzzle_heur.h"
#include <map>

using namespace std;

struct PuzzleMove
{
  // Data members can be public
  int tileMove;  // tile moved to reach the Board b
  Board *b;      // Pointer to a board representing the updated state
  int g;         // distance from the start board
  int h;         // heuristic distance to the goal
  int f_score;   //calculates f score of board;
  PuzzleMove *prev;  // Pointer to parent PuzzleMove
  //std::map<int, Board*> nextBoards; 


  // Constructor for starting Board (i.e. initial move)
  PuzzleMove(Board *board /*, PuzzleHeuristic* ph*/);

  // Constructor for subsequent search boards 
  // (i.e. those returned by Board::potentialMoves() )
  PuzzleMove(int tile, Board *board, PuzzleMove *parent);

  // Destructor
  ~PuzzleMove();

};


struct PuzzleMoveScoreComp
{
  bool operator()(const PuzzleMove *m1, const PuzzleMove *m2) const
  {
    // Add code to check if m1's f-score is less than m2's
    // If both have the same f-score, break ties by 
    // checking if m1's h-score is less than m2's.
    // If both have the same f and same h score, break ties
    // by returning true when m1's tileMove is less than m2's.

    if(m1->f_score < m2->f_score) return true;
    else if( (m1->f_score == m2->f_score) && ((m1->h) < (m2->h) ) ) return true; 
    else
    {
      if( (m1->f_score == m2->f_score) && (m1-> h == m2->h) ) return m1->tileMove < m2->tileMove;
      else return false;
    }

  }
};

struct PuzzleMoveBoardComp
{
  bool operator()(const PuzzleMove *m1, const PuzzleMove *m2) const
  {
    // Add code to check if m1's board is "less-than" m2's board by 
    // using the Board class' operator<()

  return *( m1->b) < *(m2->b) ;


  }
};

#endif

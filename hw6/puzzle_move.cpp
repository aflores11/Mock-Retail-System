#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include "board.h"
#include "puzzle_move.h"
//#include "puzzle_heur.h"

using namespace std;


PuzzleMove::PuzzleMove(Board* board /*, PuzzleHeuristic* ph*/)
{
    this->b = new Board(*board);
    this->prev = nullptr;
    this->g = 0;

    //this->h = ph->compute(*board);
    //this->f_score = g+h;
}

PuzzleMove:: PuzzleMove(int tile, Board* board, PuzzleMove *parent)
{
    this->b = new Board(*board);
    this->prev = parent;
    this->tileMove = tile;
    this->nextBoards = board->potentialMoves();
    this->g = this->prev-> g + 1;

}

PuzzleMove::~PuzzleMove()
{
    delete this->b;
    std::map<int, Board*>::iterator it;
    for(it = this-> nextBoards.begin(); it!=this->nextBoards.end(); ++it)
    {
        delete it->second;
    }
    this->nextBoards.clear();
}
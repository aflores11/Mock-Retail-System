#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include "board.h"
#include "puzzle_move.h"

using namespace std;


PuzzleMove::PuzzleMove(Board* board )
{
    this->b = new Board(*board);
    this->prev = nullptr;
    this->g = 0;
    this->f_score=0;
    this->h=0;
}

PuzzleMove:: PuzzleMove(int tile, Board* board, PuzzleMove *parent)
{
    this->b = board;
    this->prev = parent;
    this->tileMove = tile;
    this->g = this->prev-> g + 1;
    this->f_score=0;
    this->h=0;

}

PuzzleMove::~PuzzleMove()
{
    delete this->b;
}
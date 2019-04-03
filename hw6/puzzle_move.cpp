#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include "board.h"
#include "puzzle_move.h"
#include "puzzle_heur.h"

using namespace std;


PuzzleMove::PuzzleMove(Board* board, PuzzleHeuristic* ph)
{
    this->b = new Board(*board);
    this->prev = nullptr;
    this->g = 0;
    this->h = ph->compute(*board);
    this->f_score = g+h;
}


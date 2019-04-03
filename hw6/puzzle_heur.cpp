#include <iostream>
#include "board.h"
#include "puzzle_heur.h"
#include <cstdlib>

using namespace std;

int PuzzleManhattanHeuristic::compute(const Board& b)
{
    int m_score = 0;
    for(int i = 0; i<b.size() ; i++)
    {
        if( (b[i] != 0)  && (b[i] != i) )
        {
            //each value belongs in their same number location ex: value 5 should be in board[5];
            int val = b[i]; //using it to find where it belongs
            int val_row = val/b.dim(); //finds the row it belongs in
            int val_col = val%b.dim(); //finds column it belongs in
            int val_currow = i/b.dim(); //finds its current row
            int val_curcol = i%b.dim(); //find its current column

            m_score+= abs(val_row-val_currow) + abs(val_col - val_curcol); //finds distance away from proper spot



        }
    }

    return m_score;
}

int PuzzleOutOfPlaceHeuristic::compute(const Board& b)
{
    int displaced = 0;
    for(int i =0 ; i<b.size(); i++)
    {
        if( (b[i] != 0)  && (b[i] != i) ) displaced++;
    }

    return displaced;
}

int PuzzleBFSHeuristic::compute(const Board& b)
{
    return 0;
}
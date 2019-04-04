#include "puzzle_solver.h"
#include "puzzle_heur.h"
#include "puzzle_move.h"
#include <iostream>
#include <set>
#include "heap.h"

using namespace std;

PuzzleSolver::PuzzleSolver(const Board &b, PuzzleHeuristic* ph)
{
    this->b_= new Board(b);
    this->ph_ = ph;


}

PuzzleSolver::~PuzzleSolver()
{
  PuzzleMoveSet::iterator it;
  for(it= this->closed_list.begin(); it != this->closed_list.end(); ++it )
  {
      delete *it;
  }
  
    delete this->b_;

}

void PuzzleSolver::run()
{
    Heap< PuzzleMove*, PuzzleMoveScoreComp > open_list(2,PuzzleMoveScoreComp());

    /*Creates the starting puzzlemove and pushes it into both lists*/
    PuzzleMove* start = new PuzzleMove(this->b_);
    start->h = this->ph_->compute(*start->b);
    start->f_score = start->g + start->h;
    this->closed_list.insert(start);
    open_list.push(start);

    /*actual A* algorithm doing work*/
    while(!open_list.empty())
    {
        PuzzleMove* temp = open_list.top();
        open_list.pop();
        if(temp->b->solved())
        {
            PuzzleMove* tracker = temp;
            while(tracker->prev != nullptr)
            {
                this->solution_.push_back(tracker->tileMove); 
                tracker = tracker->prev;  
            }
            return;
        }

        std::map<int,Board*> newBaords = temp->b->potentialMoves();
        std::map<int,Board*>::iterator it;
        for(it  = newBaords.begin(); it != newBaords.end(); ++it) //iterates though new baords to make them into puzzlemoves
        {
            PuzzleMove* newpm = new PuzzleMove(it->first, it->second, temp); //makes new puzzlemove
            /*updates the scores of the new puzzlemoves*/
            newpm->h = this->ph_->compute(*it->second);
            newpm->f_score = newpm->h + newpm->g;

            if(closed_list.find(newpm) == closed_list.end()) //checks if it is already in the set and adds if not
            {
                open_list.push(newpm);
                closed_list.insert(newpm);
                this->expansions_++;
                continue;
            }
            
            delete newpm; //already exists in the set therefore do not need it again

        }



        
        
    }

return;



}

std::deque<int> PuzzleSolver::getSolution()
{
    return this->solution_;
}

int PuzzleSolver::getNumExpansions()
{
    return this->expansions_;
}
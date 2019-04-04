#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include "board.h"
using namespace std;

//helper function to find boundaries to get potential boards
void find_boundaries(int* pot, int loc, int dim)
{
  if(loc/dim == 0) //edge case for top row
  {
    if(loc%dim== dim-1) //top right corner
    {
      pot[0] = loc-1; //gets left tile
      pot[1] = -5; //gets right tile // out of bounds
      pot[2] = -5; //gets tile above
      pot[3] = loc+dim; // gets tile below 
      return;
    }
    else if(loc%dim== 0) //top left  corner
    {
      pot[0] = -5; // left tile out of bounds
      pot[1] = loc+1; //gets right tile 
      pot[2] = -5; //gets tile above
      pot[3] = loc+dim; // gets tile below 
      return;
    }
    else
    {
      pot[0] = loc-1; // left tile
      pot[1] = loc+1; //gets right tile 
      pot[2] = -5; // tile above out of bunds
      pot[3] = loc+dim; // gets tile below 
    }  
  }

  else if(loc/dim == dim-1) //bottom row
  {
    if(loc%dim== dim-1) //bottom right corner
    {
      pot[0] = loc-1; //gets left tile
      pot[1] = -5; //gets right tile // out of bounds
      pot[2] = loc - dim; //gets tile above
      pot[3] = -5; // gets tile below 
      return;
    }
    else if(loc%dim== 0) //top left  corner
    {
      pot[0] = -5; // left tile out of bounds
      pot[1] = loc+1; //gets right tile 
      pot[2] = loc-dim; //gets tile above
      pot[3] = -5; // gets tile below 
      return;
    }
    else
    {
      pot[0] = loc-1; // left tile
      pot[1] = loc+1; //gets right tile 
      pot[2] = loc-dim; // tile above out of bunds
      pot[3] = -5; // gets tile below 
    }  
  }

  else if(loc%dim == 0) //left column , but no need to worry about corners anymore
  {
      pot[0] = -5; // left tile out of bounds
      pot[1] = loc+1; //gets right tile 
      pot[2] = loc-dim; // tile above out of bunds
      pot[3] = loc+dim; // gets tile below 
      return;
  }
  else if(loc%dim == dim-1) // right column, no need to worry about corners
  {
      pot[0] = loc-1; // left tile out of bounds
      pot[1] = -5; // right tile  out of bounds
      pot[2] = loc-dim; // tile above out of bunds
      pot[3] = loc+dim; // gets tile below
      return; 
  }
  else // anywhere that's not on the edges
  {
      pot[0] = loc-1; // left tile out of bounds
      pot[1] = loc+1; // right tile  out of bounds
      pot[2] = loc-dim; // tile above out of bunds
      pot[3] = loc+dim; // gets tile below
      return;
  }
  
}


Board::Board(int dim, int numInitMoves, int seed )
{
  size_ = dim*dim;
  tiles_ = new int[size_];
  srand(seed);
  for(int i=0; i < size_; i++){
    tiles_[i] = i;
  }
  int blankLoc = 0;
  while(numInitMoves > 0){
    int r = rand()%4;
    int randNeighbor = -1;
    if(r == 0){
      int n = blankLoc - dim;
      if(n >= 0){
	randNeighbor = n;
      }
    }
    else if(r == 1){
      int w = blankLoc - 1;
      if(blankLoc % dim != 0){
	randNeighbor = w;
      }
    }
    else if(r == 2){
      int s = blankLoc + dim;
      if(s  < size_){
	randNeighbor = s;
      }
    }
    else {
      int e = blankLoc + 1;
      if(blankLoc % dim != dim-1){
	randNeighbor = e;
      }
    }
    if(randNeighbor > -1){
      tiles_[blankLoc] = tiles_[randNeighbor];
      tiles_[randNeighbor] = 0;
      blankLoc = randNeighbor;
      numInitMoves--;
    }
  }
}


void Board::move(int tile)
{
  int side_dim = dim();
  int tr, tc, br, bc;

  // find tile row and column
  int i=-1;
  while(tiles_[++i] != tile);

  tr = i / side_dim; 
  tc = i % side_dim;

  // find blank row and column
  int j=-1;
  while(tiles_[++j] != 0);

  br = j / side_dim;
  bc = j % side_dim;

  if( abs(static_cast<double>(tr-br)) + abs(static_cast<double>(tc-bc)) != 1){
    stringstream ss;
    ss << "Invalid move of tile " << tile << " at ";
    ss << tr << "," << tc << " and blank spot at ";
    ss << br << "," << bc << endl;
    throw BoardMoveError(ss.str());
  }
  // Swap tile and blank spot
  tiles_[j] = tile;
  tiles_[i] = 0;
}

// Generate new boards representing all the potential moves of tiles into 
// the current blank tile location. The returned map should have
// the key as the tile moved and the value as a new Board object with the
// configuration reflecting the move of that tile into the blank spot
map<int, Board*> Board::potentialMoves() const
{
  int loc;

  //finds the location of where the empty space is
  for(int i=0 ; i<this->size(); i++)
  {
    if(this->tiles_[i] == 0)
    {
      loc = i;
      break;
    }
  }

  int pottiles[4]; //holds potential tiles that can be moved, there are a max of 4
  find_boundaries(pottiles, loc, this->dim() );

  map<int, Board*> results;

  for(int i= 0; i<4; i++)
  {
    if((pottiles[i] >=0 && pottiles[i] < (int)this->size()) ) //only creates new boards for valid tiles that are in bounds
    {
      Board* temp = new Board(*this); //makes a copy of current board
      temp->move(this->tiles_[pottiles[i]]); // moves the curr potential tile and swaps with empty space
      results.insert(std::pair<int, Board*>(this->tiles_[pottiles[i]], temp)); // push the new board to map
    }
  }


  return results;
}

// Complete this function
bool Board::solved() const
{
  for(int i =0 ; i<this->size(); i++)
  {
    if(this->tiles_[i] != i) return false;
  }

  return true;

}


const int& Board::operator[](int loc) const 
{ 
  return tiles_[loc]; 
}

int Board::size() const 
{ 
  return size_; 
}

int Board::dim() const
{
  return static_cast<int>(sqrt(size_));
}

void Board::printRowBanner(ostream& os) const
{
  int side_dim = dim();
  if(side_dim == 0) return;
  os << '+';
  for(int i=0; i < side_dim; i++){
    os << "--+";
  }
  os << endl;
}

Board::Board(const Board& b)
{
  this->tiles_ = new int[b.size()];
  this->size_ = b.size();
  for(int i = 0; i<b.size(); i++)
  {
    this->tiles_[i] = b.tiles_[i];
  }

}

Board::~Board()
{
  delete[] this->tiles_;
}

bool Board::operator<(const Board& rhs) const
{
  for(int i =0 ; i<this->size(); i++)
  {
    if(this->tiles_[i] == rhs.tiles_[i]) continue; // if equal, goes to next one
    if(this->tiles_[i] > rhs.tiles_[i]) return false; //if lhs is >, then it is not less than board 2 
    if(this->tiles_[i] < rhs.tiles_[i]) return true; // if this board tile is greater return false
  } 

 return false;
}

std::ostream& operator<<(std::ostream &os, const Board &b)
{
  b.printRowBanner(os);
  for (int rows = 0; rows<b.dim(); rows++)
  {
    os << "|";
    for(int cols = rows*b.dim(); cols<rows*b.dim()+b.dim(); cols++)
    {
      if(b.tiles_[cols] == 0) 
      {
        os << "  |";
        continue;
      }
      
      os << " " << b.tiles_[cols] << "|"; 
    }
    os <<"\n";
    b.printRowBanner(os);

  }

  return os;
}


#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include "board.h"
using namespace std;


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
  
  pottiles[0] = loc-1; //gets left tile
  pottiles[1] = loc+1; //gets right tile
  pottiles[2] = loc - (int)this->dim(); //gets tile above
  pottiles[3] = loc+(int)this->dim(); // gets tile below

  map<int, Board*> results;

  for(int i= 0; i<4; i++)
  {
    if(pottiles[i] >=0 || pottiles[i] < (int)this->size()) //only creates new boards for valid tiles that are in bounds
    {
      Board* temp = new Board(*this); //makes a copy of current board
      temp->move(pottiles[i]); // moves the curr potential tile and swaps with empty space
      results.insert(std::pair<int, Board*>(pottiles[i], temp)); // push the new board to map
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
    for(int cols = rows*b.dim(); cols<b.dim(); cols++)
    {
      if(b.tiles_[cols] == 0) os << "  |";
      
      os << " " << b.tiles_[cols] << "|"; 
    }
    os <<"\n";
    b.printRowBanner(os);

  }
  b.printRowBanner(os);
  return os;
}
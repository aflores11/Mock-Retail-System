#ifndef HEAP_H
#define HEAP_H
#include <functional>
#include <stdexcept>
#include <vector>
#include <utility>
#include <algorithm>

template <typename T, typename PComparator = std::less<T> >
class Heap
{
 public:
  /// Constructs an m-ary heap for any m >= 2
  Heap(int m, PComparator c = PComparator())
  {
    this->m_ary = m;
    this->comp = c;
    //myheap = new std::vector<T>;
  }

  /// Destructor as needed
  ~Heap()
  {

  }

  /// Adds an item
  void push(const T& item);

  /// returns the element at the top of the heap 
  ///  max (if max-heap) or min (if min-heap)
  T const & top() const;

  /// Removes the top element
  void pop();

  /// returns true if the heap is empty
  bool empty() const
  {
    return myheap.size() == 0;
  }
  
 private:
  /// Add whatever helper functions and data members you need below
  int m_ary;
  std::vector<T> myheap; 
  PComparator comp;
  void trickleup(int pos);
  void heapify(int pos);


};

// Add implementation of member functions here
template <typename T, typename PComparator>
void Heap<T,PComparator>::trickleup(int pos)
{
  if(pos == 0) return;

//same thing as (pos/m-ary) -1  but if you do it that way, you run the risk of parent = a number < 0 
  int parent = (pos-1)/this->m_ary;
  if(pos > 0 && this->comp(this->myheap[pos], this->myheap[parent])) // base case since we cannot work on empty list
  {
    std::swap(this->myheap[pos] , this->myheap[parent]); //based on comparison, if child  needs to be before the parent, swap
    trickleup(parent); // calls trickle up again on the newly moved parent
  }
}

template <typename T, typename PComparator>
void Heap<T,PComparator>::heapify(int pos)
{
  if((pos*this->m_ary) + 1  < (int)this->myheap.size()) //makes sure curr position is not a leaf node
  {
    int child = pos*(this->m_ary) +1; //one child is gauranteed, also sets it to first child
    for(int i = child+1; i< child + this->m_ary; i++ )
    {
      if(i >= (int)this->myheap.size()) break; // makes sure we only look at children that exist
      //next line checks if two chldren are tied, if so child stays the left most child
      if(this->comp(this->myheap[child], this->myheap[i]) == this->comp(this->myheap[i], this->myheap[child])) continue;
      else if(!this->comp(this->myheap[child], this->myheap[i]))
      {
        child = i;
      }
    }

    if(!this->comp(this->myheap[pos], this->myheap[child]))
    {
        std::swap(this->myheap[pos], this->myheap[child]);
        heapify(child);
    }

  }
}


// We will start top() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
T const & Heap<T,PComparator>::top() const
{
  // Here we use exceptions to handle the case of trying
  // to access the top element of an empty heap
  if(this->empty()){
    throw std::logic_error("can't top an empty heap");
  }
  // If we get here we know the heap has at least 1 item
  // Add code to return the top element
  return this->myheap[0];


}

template<typename T, typename PComparator>
void Heap<T,PComparator>::push(const T& item)
{
  this->myheap.push_back(item);
  trickleup(this->myheap.size() - 1);
}


// We will start pop() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
void Heap<T,PComparator>::pop()
{
  if(this->empty()){
    throw std::logic_error("can't pop an empty heap");
  }

  this->myheap[0] = this->myheap[this->myheap.size()-1];
  this->myheap.pop_back();
  heapify(0);
}

#endif


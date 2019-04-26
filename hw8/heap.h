#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <iostream>
template <
         typename T,
         typename TComparator = std::equal_to<T>,
         typename PComparator = std::less<double>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const TComparator& tcomp = TComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item with the provided priority
    void push(double pri, const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey reduces the current priority of
    /// item to newpri, moving it up in the heap
    /// as appropriate.
    void decreaseKey(double newpri, const T& item);

private:
    /// Add whatever helper functions you need below
    void trickleup(int pos);
    void heapify(int pos);




    // These should be all the data members you need.
    std::vector< std::pair<double, T> > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, TComparator> keyToLocation_;

};

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::heapify(int pos)
{
    if((pos*this->m_) + 1  < (int)this->store_.size()) //makes sure curr position is not a leaf node,+1 or 0 based indexing
    {
        int child = pos*(this->m_) +1; //one child is gauranteed, also sets it to first child
        for(int i = child+1; i< child + this->m_; i++ )
        {
            if(i >= (int)this->store_.size()) break; // makes sure we only look at children that exist
            //next line checks if two chldren are tied, if so child stays the left most child
            if(this->c_(this->store_[child].first, this->store_[i].first) == this->c_(this->store_[i].first, this->store_[child].first)) continue;
            
            if(!this->c_(this->store_[child].first, this->store_[i].first))
            {
                child = i;
            }
        }

        if(!this->c_(this->store_[pos].first, this->store_[child].first))
        {
            std::swap(this->store_[pos], this->store_[child]);
            keyToLocation_[store_[pos].second] = pos; 
            keyToLocation_[store_[child].second] = child; 
            heapify(child);
        }

    }

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleup(int pos)
{
  if(pos == 0) return;

//same thing as (pos/m-ary) -1  but if you do it that way, you run the risk of parent = a number < 0 
  int parent = (pos-1)/this->m_;
  if(pos > 0 && this->c_(this->store_[pos].first, this->store_[parent].first)) // base case since we cannot work on empty list
  {
    std::swap(this->store_[pos] , this->store_[parent]); //based on comparison, if child  needs to be before the parent, swap
    //after swap update the index of both objects that were swapped
    keyToLocation_[store_[pos].second] = pos; 
    keyToLocation_[store_[parent].second] = parent; 
    trickleup(parent); // calls trickle up again on the newly moved parent
  }
}


// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const TComparator& tcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, tcomp)

{

}

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::~Heap()
{
    this->store_.clear();
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::push(double priority, const T& item)
{
    // You complete.
    this->store_.push_back(make_pair(priority,item));
    this->keyToLocation_.insert(make_pair(item, store_.size() -1));
    trickleup(this->store_.size() - 1);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::decreaseKey(double priority, const T& item)
{
    auto index = keyToLocation_[item]; //returns some number size_t
    this->store_[index].first = priority;
    trickleup(index); //only making the priority smaller

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T const & Heap<T,TComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }

    return this->store_[0].second;


}

/// Removes the top element
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    this->store_[0] = this->store_[this->store_.size()-1];
    this->store_.pop_back();
    heapify(0);

}

/// returns true if the heap is empty
template <typename T, typename TComparator, typename PComparator, typename Hasher >
bool Heap<T,TComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}

#endif


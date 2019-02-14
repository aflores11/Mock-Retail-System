#include <cstddef>
#include <stdexcept>
#include "ulliststr.h"

ULListStr::ULListStr()
{
  head_ = NULL;
  tail_ = NULL;
  size_ = 0;
}

ULListStr::~ULListStr()
{
  clear();
}

bool ULListStr::empty() const
{
  return size_ == 0;
}

size_t ULListStr::size() const
{
  return size_;
}

void ULListStr::set(size_t loc, const std::string& val)
{
  std::string* ptr = getValAtLoc(loc);
  if(NULL == ptr){
    throw std::invalid_argument("Bad location");
  }
  *ptr = val;
}

std::string& ULListStr::get(size_t loc)
{
  std::string* ptr = getValAtLoc(loc);
  if(NULL == ptr){
    throw std::invalid_argument("Bad location");
  }
  return *ptr;
}

std::string const & ULListStr::get(size_t loc) const
{
  std::string* ptr = getValAtLoc(loc);
  if(NULL == ptr){
    throw std::invalid_argument("Bad location");
  }
  return *ptr;
}

void ULListStr::clear()
{
  while(head_ != NULL){
    Item *temp = head_->next;
    delete head_;
    head_ = temp;
  }
  tail_ = NULL;
  size_ = 0;
}

void ULListStr::push_back(const std::string& val){
   //case for empty list
     if(head_ == nullptr && tail_ == nullptr){
      Item* newtail= new Item();
      newtail->prev = nullptr;
      newtail->next=nullptr;
      newtail->first=0;
      newtail->last=0;
      newtail->val[newtail->first] = val;
      newtail->last= newtail->last+1;
      head_= newtail;
      tail_= head_;
      size_= 1;
    }
    
    else{
      
      if(tail_->last == ARRSIZE){
        Item* newtail= new Item();
        tail_->next = newtail;
        newtail->next = nullptr;
        newtail->prev= tail_;
        newtail->first=0;
        newtail->last=0;
        newtail->val[newtail->first] = val;
        newtail->last= newtail->last+1;
        tail_ = newtail;
        size_++;
      }

      else{
        tail_->val[tail_->last] = val;
        tail_->last = tail_->last +1;
        size_++;    
      }
    }
}

void ULListStr::pop_back(){
  if(tail_ == nullptr) return; //case for a list with no items
  else{
 // case for there is only one val and only one item: deletes item because we do not want empty items   
        if(tail_->last == 1 && tail_== head_){ 
          delete tail_;
          head_=nullptr;
          tail_=nullptr;
          size_--;
        }

        else{ //there are more than one items
              if(tail_->last == 1){  // case where there is only one value in the array and need to delete node 
                Item* temp = tail_->prev;  // so that we do not have an empty node
                delete tail_;  
                tail_=temp;
                tail_->next = nullptr;
                size_--; // need to reduce size since we are betting rid of a value
                
              }
              else{
                // will set last to the one before so if push _back is called again, it will overide that previos value that was there
                tail_->last = tail_->last-1; 
                size_--; // need to reduce size since we are betting rid of a value
              }
        }
  }
}

void ULListStr::push_front(const std::string& val){
  if(head_ == nullptr && tail_ == nullptr){ //checks case if there is nothing on the List
      Item* newtail= new Item();
      newtail->prev = nullptr;
      newtail->next=nullptr;
      newtail->first=0;
     //edited right now
      newtail->last=0;
      newtail->val[newtail->first] = val;
      newtail->last= newtail->last+1;
      head_= newtail;
      tail_=head_;
      size_= 1;
    }
   else{
    
    if(head_->first == 0){
      Item* newhead= new Item();
      newhead->next = head_;
      newhead->prev= nullptr;
      head_->prev = newhead;
      head_ = newhead;
      head_->val[ARRSIZE-1] = val;
      size_++;
      head_->first = ARRSIZE-1;
      head_->last = ARRSIZE;
    }
    else{
      head_->first= head_->first -1;
      head_->val[head_->first] = val;
      size_++;
    }
  }  
  
}

void ULListStr::pop_front(){
 if(head_ == nullptr) return; //does nothing if list empty

  if(head_->first == ARRSIZE-1 && tail_== head_){
      delete head_;
      head_ = nullptr;
      tail_=nullptr;
      size_--;
      return;
  }
  if(head_->first == ARRSIZE-1){  // case where there is only one value in the array and need to delete node 
        Item* temp = head_->next;  // so that we do not have an empty node
        delete head_;
        head_ = temp;
        head_->prev = nullptr;
        head_->first=0;
        size_--; // need to reduce size since we are betting rid of a value
      }
  else{
        head_->first = head_->first+1; 
        size_--; // need to reduce size since we are betting rid of a value
      }
}

std::string const & ULListStr::back() const{
    return tail_->val[tail_->last-1];
}

std::string const & ULListStr::front() const{
  return head_->val[head_->first];
}

std::string* ULListStr::getValAtLoc(size_t loc) const{
  
  if(empty()){  // cannot access top value on stack if list is empty
    return nullptr;
  }
  else{
    if(loc <0 || loc > size_-1 ) return nullptr; //checks if the location if between the indexes that we actually have
    else{
      Item* temp = head_; //sets a temporary item to not modify the original
      int tempfirst = temp->first;
      int location = 0; // diff from size_t loc; keeps track of long list index
      
      while(location != (int)loc){
        
        if(tempfirst == (int)temp->last -1){ //checks if there is no more 
          temp = temp->next;
          tempfirst=temp->first;
          location++;

        }
        else{
          tempfirst++;
          location++;
        }
      }
    
      std::string* ptr = &temp->val[tempfirst];
      return ptr;
    }
  } 
}

ULListStr::ULListStr(const ULListStr& other){
  size_=0;
  head_= nullptr;
  tail_=nullptr;
  this->appendContents(other);
}

ULListStr& ULListStr::operator= (const ULListStr& other){
  if(this == &other) return *this;
  
  else{
    if(! this->empty()){
      
      this->clear();
      this->appendContents(other);
      return *this;
    }
    
    else{
      this->appendContents(other);
      return *this;
    }
  }  
}

ULListStr ULListStr::operator+ (const ULListStr& other) const{

  ULListStr temp(*this);
  temp.appendContents(other);
  return temp;

}

ULListStr& ULListStr::operator-= (size_t num){
  if(num > this->size_){
    this->clear();
    return *this;
  }
  else{
    for(int i=0; i<(int)num; i++){
      this->pop_back();
    }
    return *this;
  }
  
}

std::string const & ULListStr::operator[] (size_t loc) const{
  return this->get(loc);
}

std::string & ULListStr::operator[] (size_t loc){
  return this->get(loc);
}

void ULListStr::appendContents(const ULListStr& other){
  
  if(other.size_ == 0) return;
  
  Item* temphead = other.head_;  // sets temporary head to not modufy original
  int tempfirst = temphead->first; // gives me first index 
  int size_count = 0; // counter to know when to stop appending  
 

  while(size_count != (int)other.size_ ){  //iterates through the nodes 
      
    if(tempfirst == (int)temphead->last){ //checks if there is no more 
      temphead = temphead->next; //goes to next node
      if(temphead == nullptr) break;
      tempfirst=temphead->first; // resets index 

    }
      this->push_back(temphead->val[tempfirst]);
      tempfirst++;
      size_count++;

  } 
  
 
}
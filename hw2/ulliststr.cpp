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
      //case if we are at the last index and there is no space left; makes new item and adds value to first index
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
        //adds the value at unfilled last (since exlusive) and then update the last to make it exclusive again
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
          Item* temp = tail_->prev;
          delete head_;
          head_=temp;
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
      tail_=head_;
      size_= 1;
    }
   //if not empty
   else{
    
    if(head_->first == 0){
      Item* newhead= new Item(); //case for if there is no more space in front of the current item
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
   //makes the first value be one before and adds the new pushed value there
      head_->first= head_->first -1;
      head_->val[head_->first] = val;
      size_++;
    }
  }  
  
}

void ULListStr::pop_front(){
  if(head_ == nullptr) return; // if empty list do nothing

  else{

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
}

std::string const & ULListStr::back() const{
    if(tail_ == nullptr) return NULL;
    else{
      return tail_->val[tail_->last-1]; // we want the last value in the last item
    }
}

std::string const & ULListStr::front() const{
  if(head_ == nullptr) return NULL;
  else{
    return head_->val[head_->first]; //we want the most front value
  }
}

std::string* ULListStr::getValAtLoc(size_t loc) const{
  if(loc <0 || loc > size_-1 ) return nullptr; //checks if the location if between the indexes that we actually have
  else{
    Item* temp = head_; //sets a temporary item to not modify the original
    int tempfirst =(int)temp->first;
    int location = 0; // diff from size_t loc; keeps track of long list index
    
    while(location != (int)loc){
      
      if(tempfirst == (int)temp->last -1){ //checks if there is no more 
        temp = temp->next; //goes to the next item
        tempfirst=(int)temp->first;  //sets the first pointer to the first pointer of the new item we are in
        location++;

      }
      else{
      tempfirst++; // iterates through the current index in the current item setting it to the next one
      location++; // updates the overall location we are at
      }
    }
   
    std::string* ptr = &temp->val[tempfirst];
    return ptr;
  }
}
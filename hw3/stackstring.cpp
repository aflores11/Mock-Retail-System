#include "stackstring.h"
#include "ulliststr.h"


    StackString::StackString(){
        //this->list_; 
    }

    StackString::~StackString(){
        list_.clear();
    }

    bool StackString::empty() const{
        return list_.empty();
    }

    size_t StackString::size() const{
        return list_.size();
    }

    void StackString::push(const std::string& val){
        list_.push_front(val);
    }

    const std::string& StackString::top() const{
       return list_.get(0);
    }

    void StackString::pop(){
        list_.pop_front();
        
    }
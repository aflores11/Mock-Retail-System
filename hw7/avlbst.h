#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insert_fix(AVLNode<Key,Value>* child, AVLNode<Key,Value>* parent);
    void left_rotate(AVLNode<Key,Value>* currnode_);
    void right_rotate(AVLNode<Key,Value>* currnode_);

    

};

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if(this->empty())
    {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    if(this->internalFind(new_item.first) != nullptr) //if already exists , simply override value 
    {
        Node<Key,Value>* temp = this->internalFind(new_item.first);
        temp->setValue(new_item.second);
        return;
            
    } 


    AVLNode<Key, Value>* temp = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr );
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key,Value>*>(this->root_); //keeps track of what noode we are looking at

    while(curr != nullptr)
    {
        if(curr->getKey() < new_item.first) // new pair is greater than current node 
        {
            if(curr->getRight() == nullptr) 
            {
                temp->setParent(curr);
                curr->setRight(temp);
                this->insert_fix(temp, curr);
                break; // already inserted , no need to go further
            }
            
            curr = curr->getRight();

        }
        else // new pair is less than current node
        {
            if(curr->getLeft() == nullptr) 
            {
                temp->setParent(curr);
                curr->setLeft(temp);
                this->insert_fix(temp, curr);

                break; // already inserted , no need to go further
            }
            curr = curr->getLeft();
        }
    }



}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* child, AVLNode<Key,Value>* parent)
{
    if(child == nullptr || parent == nullptr) return;

    if(child->getItem().first < parent->getItem().first) // inserted to the left of parent 
    {
        parent->updateBalance(-1);
        if(parent->getBalance() == 0) return; //graph is balanced 
        else if(parent->getBalance() == -1) // left subtree has higher height of 1
        {
            insert_fix(parent, parent->getParent());
        }
        else // left subtree is unbalanced  
        {
            if(parent->getLeft() == child) // zig-zig
            {
                right_rotate(parent);
            }
            else //zig zag
            {
                left_rotate(child);
                right_rotate(parent);
            }
        }
        
    }
    else //inserted to the right of parent 
    {
        parent->updateBalance(1);
        if(parent->getBalance() == 0) return; //graph is balanced 
        else if(parent->getBalance() == 1) // right subtree has higher height of 1
        {
            insert_fix(parent, parent->getParent());
        }
        else // right subtree is unbalanced  
        {
            if(parent->getRight() == child) // zig-zig
            {
                left_rotate(parent);
                
            }
            else //zig zag
            {
                left_rotate(child);
                right_rotate(parent);
            }
        }
        
    }
    

}

template<class Key, class Value>
void AVLTree<Key,Value>::left_rotate(AVLNode<Key,Value>* currnode_)
{
    AVLNode<Key,Value>* temp = currnode_->getParent();
    AVLNode<Key,Value>* child = currnode_->getRight();
    
    if(temp == nullptr) // rotating the root
    {
        this->root_ = child;
        currnode_->setParent(child);
        currnode_->setRight(nullptr);
        child->setParent(temp);
        child->setLeft(currnode_);
        child->setBalance(0);
        currnode_->setBalance(0);
        return;
    } 
    
    
    if(temp->getLeft() == currnode_) temp->setLeft(child);
    else temp->setRight(child);   

    currnode_->setParent(child);
    currnode_->setRight(nullptr);
    child->setParent(temp);
    child->setLeft(currnode_);
    child->setBalance(0);
    currnode_->setBalance(0);
    return;
    

}

template<class Key, class Value>
void AVLTree<Key,Value>::right_rotate(AVLNode<Key,Value>* currnode_)
{
    AVLNode<Key,Value>* temp = currnode_->getParent();
    AVLNode<Key,Value>* child = currnode_->getLeft();
    
    if(temp == nullptr) // rotating the root
    {
        this->root_ = child;
        currnode_->setParent(child);
        currnode_->setLeft(nullptr);
        child->setParent(temp);
        child->setRight(currnode_);
        child->setBalance(0);
        currnode_->setBalance(0);
        return;
    } 
    
    
    if(temp->getLeft() == currnode_) temp->setLeft(child);
    else temp->setRight(child);   

    currnode_->setParent(child);
    currnode_->setLeft(nullptr);
    child->setParent(temp);
    

    child->setRight(currnode_);
    child->setBalance(0);
    currnode_->setBalance(0);
    return;
}




#endif

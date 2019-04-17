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
    void remove_fix(AVLNode<Key,Value>*node, char diff);
    

    

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
                curr->updateBalance(1);
                if(curr->getBalance() != 0) this->insert_fix(temp, curr); // checks if insert balances before
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
                curr->updateBalance(-1);
                if(curr->getBalance() != 0) this->insert_fix(temp, curr); // checks if insert balances before

                break; // already inserted , no need to go further
            }
            curr = curr->getLeft();
        }
    }



}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    if(this->empty()) return;

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if(curr == nullptr) return;
    AVLNode<Key, Value>* parent = curr->getParent();
    char diff;
    if(parent != nullptr)
    {
        if(parent->getLeft() == curr) diff = 1;
        if(parent->getRight() == curr) diff = -1;
    
    }
    if(curr->getLeft() == nullptr && curr->getRight() == nullptr) // leaf node
    {
       if(curr->getParent() == nullptr) //root case
       {
           this->root_ = nullptr;
           delete curr;
           return;
       } 
       
       /***** Case for not the root ********/
        if(diff == 1) parent->setLeft(nullptr);
        if(diff == -1)parent->setRight(nullptr);
        delete curr;
        remove_fix(parent, diff);
        return;
    }
    else if(curr->getLeft() != nullptr && curr->getRight() != nullptr) // has two children
    {
        AVLNode<Key,Value>* mypred = static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key,Value>::predecessor(curr));
        if(mypred->getLeft() == nullptr) //is a leaf since pred would be most right 
        {
            if(parent == nullptr)//root case
            {
                nodeSwap(curr, mypred);
                this->root_ = mypred;

                if(curr->getParent() == mypred) // nxt to each other
                {
                    mypred->setLeft(nullptr);
                    delete curr;
                    remove_fix(mypred, +1);//lost one in left subtree
                    return; 
                }   
                else //not next to each other
                {
                    AVLNode<Key,Value>* tempparent = curr->getParent();
                    tempparent->setRight(nullptr);
                    delete curr;
                    remove_fix(tempparent, -1); // lost a node in right of tempparent since it was predecessor
                    return;
                } // pred is most right of left sub tree from node being removed
            }

            /***** Case for not the root ********/
            nodeSwap(curr, mypred);
            if(curr->getParent() == mypred) //right next to each other
            {
                mypred->setLeft(nullptr);
                delete curr;
                remove_fix(mypred, +1);
                return;
            }
            else 
                {
                    AVLNode<Key,Value>* tempparent = curr->getParent();
                    tempparent->setRight(nullptr);
                    delete curr;
                    remove_fix(tempparent, -1); // lost a node in right of tempparent since it was predecessor
                    return;
                } // pred is most right of left sub tree from node being removed
        }
        if(mypred->getLeft() != nullptr)
        {
            if(parent == nullptr)//root case
            {
                nodeSwap(curr, mypred);
                this->root_ = mypred;
                if(curr->getParent() == mypred) //next to each other 
                {
                    mypred->setLeft(curr->getLeft());
                    curr->getLeft()->setParent(mypred);
                    delete curr;
                    remove_fix(mypred, +1);//lost one in left subtree
                    return; 
                }   
                else 
                {
                    AVLNode<Key,Value>* tempparent = curr->getParent();
                    tempparent->setRight(curr->getLeft());
                    tempparent->getRight()->setParent(tempparent);
                    delete curr;
                    remove_fix(tempparent, -1); // lost a node in right of tempparent since it was predecessor
                    return;
                } // pred is most right of left sub tree from node being removed
            }
            /***** Case for not the root ********/
            nodeSwap(curr, mypred);
            if(curr->getParent() == mypred) //next to each other 
            {
                mypred->setLeft(curr->getLeft());
                curr->getLeft()->setParent(mypred);
                delete curr;
                remove_fix(mypred, +1);//lost one in left subtree
                return; 
            }   
            else 
            {
                AVLNode<Key,Value>* tempparent = curr->getParent();
                tempparent->setRight(curr->getLeft());
                tempparent->getRight()->setParent(tempparent);
                delete curr;
                remove_fix(tempparent, -1); // lost a node in right of tempparent since it was predecessor
                return;
            }   
        }    
    }

    else if(curr->getLeft() != nullptr && curr->getRight() == nullptr) //only has left child
    {
        if(parent == nullptr) // curr is root
        {
            AVLNode<Key,Value>* child = curr->getLeft();
            nodeSwap(curr, child );
            this->root_ =child;
            child->setLeft(curr->getLeft());
            child->setParent(parent);
            delete curr;
            remove_fix(child, +1);//lost a left child
            return;
        }
        /***** Case for not the root ********/
        
        AVLNode<Key,Value>* child = curr->getLeft();
        nodeSwap(curr, child );
        child->setLeft(curr->getLeft());
        child->setParent(parent);
        if(diff == 1) parent->setLeft(child);
        if(diff== -1) parent->setRight(child);
        delete curr;
        remove_fix(child, +1);//lost a left child
        return;

    }
    else if(curr->getLeft() == nullptr && curr->getRight() != nullptr) // only has right child
    {
        if(parent == nullptr) // curr is root
        {
            AVLNode<Key,Value>* child = curr->getRight();
            nodeSwap(curr, child );
            this->root_ =child;
            child->setRight(curr->getRight());
            child->setParent(parent);
            delete curr;
            remove_fix(child, -1);//lost a right child
            return;
        }
        /***** Case for not the root ********/
        
        AVLNode<Key,Value>* child = curr->getRight();
        nodeSwap(curr, child );
        child->setRight(curr->getRight());
        child->setParent(parent);
        if(diff == 1) parent->setLeft(child);
        if(diff== -1) parent->setRight(child);
        delete curr;
        remove_fix(child, -1);//lost a right child
        return;


    }
    else{}//dummy



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
    if(parent == nullptr || parent->getParent() == nullptr) return;

    AVLNode<Key, Value>* grandparent = parent->getParent();

    if(grandparent->getLeft() == parent)  // grandparent-left(p)
    {
        grandparent->updateBalance(-1);
        if(grandparent->getBalance() == 0) return; //graph is balanced 
        else if(grandparent->getBalance() == -1) // left subtree has higher height of 1
        {
            insert_fix(parent, grandparent);
        }
        else if(grandparent->getBalance() == -2) // left subtree is unbalanced  
        {
            if( parent->getBalance() == -1 ) // zig-zig   g-left(p)-left(c)
            {
                right_rotate(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            }
            else if(parent->getBalance() == 1)//zig zag  g-left(p)-right(c)
            {
                left_rotate(parent);
                right_rotate(grandparent);
                if(child->getBalance() == -1) 
                {
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                    child->setBalance(0);
                }
                else if(child->getBalance() == 0) 
                {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    child->setBalance(0);
                }
                else if(child->getBalance()==1)
                {
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                    child->setBalance(0);
                }
                else
                {
                
                }            

            }
            else{}
        }
        else{}
    }
    else if(grandparent->getRight() == parent) // g-right(p)
    {
        grandparent->updateBalance(+1);
        if(grandparent->getBalance() == 0) return; //graph is balanced 
        else if(grandparent->getBalance() == 1) // left subtree has higher height of 1
        {
            insert_fix(parent, grandparent);
        }
        else if(grandparent->getBalance() == 2)
        {
            if(parent->getBalance() == 1) // zig-zig g-right(p)-right(c)
            {
                left_rotate(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            }
            else if (parent->getBalance() < 0)//zig zag g-right(p)-left(c)
            {
                right_rotate(parent);
                left_rotate(grandparent);
                if(child->getBalance() == -1) 
                {
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                    child->setBalance(0);
                }
                else if(child->getBalance() == 0) 
                {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    child->setBalance(0);
                    
                }
                else if(child->getBalance() == 1)
                {
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                    child->setBalance(0);
                }
                else
                {
                
                }
            }
            else{}
        } 
        else{}  
    
    }
    else{}

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
        if(child->getLeft() != nullptr)
        {
            currnode_->setRight(child->getLeft());
            child->getLeft()->setParent(currnode_);
        }
        child->setParent(temp);
        child->setLeft(currnode_);
        return;
    } 
    
    
    if(temp->getLeft() == currnode_) temp->setLeft(child);
    else temp->setRight(child);   

    currnode_->setParent(child);
    if(child->getLeft() != nullptr)
    {
        currnode_->setRight(child->getLeft());
        child->getLeft()->setParent(currnode_);
    }
    else
    {
        currnode_->setRight(nullptr);
    }

    child->setParent(temp);
    child->setLeft(currnode_);
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
        if(child->getRight() != nullptr)
        {
            currnode_->setLeft(child->getRight());
            child->getRight()->setParent(currnode_);
        }
        child->setParent(temp);
        child->setRight(currnode_);
        return;
    } 
    
    
    if(temp->getLeft() == currnode_) temp->setLeft(child);
    else temp->setRight(child);   

    currnode_->setParent(child);
    if(child->getRight() != nullptr)
    {
        currnode_->setLeft(child->getRight());
        child->getRight()->setParent(currnode_);
    }
    else currnode_->setLeft(nullptr);
  
    child->setParent(temp);
    child->setRight(currnode_);
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key,Value>*node, char diff)
{
    if(node == nullptr) return;
    AVLNode<Key, Value>* parent = node->getParent();
    char nextdiff =0;
    if(parent != nullptr)
    {
        if(parent->getLeft() == node) nextdiff = 1;
        else nextdiff = -1;
    }
    
    if(diff == -1) //node removed was to right of parent
    {
        if(node->getBalance() + diff == -2)
        {
            AVLNode<Key, Value>* child = node->getLeft();
            if(child->getBalance() == -1 )
            {
                right_rotate(node); 
                node->setBalance(0);
                child->setBalance(0);
                remove_fix(parent, nextdiff);
                return;
            }
            else if(child->getBalance() == 0)
            {
                right_rotate(node);
                node->setBalance(-1);
                child->setBalance(+1);
                return;
            }
            else if(child->getBalance() == 1)
            {
                AVLNode<Key, Value>* rightofchild = child->getRight();
                left_rotate(child);
                right_rotate(node);
                if(rightofchild->getBalance() == 1)
                {
                    node->setBalance(0);
                    child->setBalance(-1);
                    rightofchild->setBalance(0);
                    remove_fix(parent, nextdiff);
                    return;
                }
                else if(rightofchild->getBalance() == 0)
                {
                    node->setBalance(0);
                    child->setBalance(0);
                    rightofchild->setBalance(0);
                    remove_fix(parent, nextdiff);
                    return;
                }
                else if(rightofchild->getBalance() == -1)
                {
                    node->setBalance(+1);
                    child->setBalance(0);
                    rightofchild->setBalance(0);
                    remove_fix(parent, nextdiff);
                    return;
                }
                else{} // dummy
            }
            else{}
        }
        else if(node->getBalance() + diff == -1)
        {
            node->setBalance(-1);
            return;
        }
        else if(node->getBalance() + diff == 0) 
        {
            node->setBalance(0); 
            remove_fix(parent, nextdiff);
            return;
        }
        else{}

    }
    else if(diff == 1) //node removed was to the left of parent
    {
        if(node->getBalance() + diff == 2)
        {
            AVLNode<Key, Value>* child = node->getRight();
            if(child->getBalance() == 1 )
            {
                left_rotate(node); 
                node->setBalance(0);
                child->setBalance(0);
                remove_fix(parent, nextdiff);
                return;
            }
            else if(child->getBalance() == 0)
            {
                left_rotate(node);
                node->setBalance(1);
                child->setBalance(-1);
                return;
            }
            else if(child->getBalance() == -1)
            {
                AVLNode<Key, Value>* leftofchild = child->getLeft();
                right_rotate(child);
                left_rotate(node);
                if(leftofchild->getBalance() == -1)
                {
                    node->setBalance(0);
                    child->setBalance(1);
                    leftofchild->setBalance(0);
                    remove_fix(parent, nextdiff);
                    return;
                }
                else if(leftofchild->getBalance() == 0)
                {
                    node->setBalance(0);
                    child->setBalance(0);
                    leftofchild->setBalance(0);
                    remove_fix(parent, nextdiff);
                    return;
                }
                else if(leftofchild->getBalance() == 1)
                {
                    node->setBalance(-1);
                    child->setBalance(0);
                    leftofchild->setBalance(0);
                    remove_fix(parent, nextdiff);
                    return;
                }
                else{} // dummy
            }
            else{}
        }
        else if(node->getBalance() + diff == 1)
        {
            node->setBalance(1);
            return;
        }
        else if(node->getBalance() + diff == 0) 
        {
            node->setBalance(0); 
            remove_fix(parent, nextdiff);
            return;
        }
        else{}
    }
    else{} //dummy 
}


#endif
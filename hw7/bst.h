#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <cstdlib>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key &key, const Value &value, Node<Key, Value> *parent);
    virtual ~Node();

    const std::pair<const Key, Value> &getItem() const;
    std::pair<const Key, Value> &getItem();
    const Key &getKey() const;
    const Value &getValue() const;
    Value &getValue();

    virtual Node<Key, Value> *getParent() const;
    virtual Node<Key, Value> *getLeft() const;
    virtual Node<Key, Value> *getRight() const;

    void setParent(Node<Key, Value> *parent);
    void setLeft(Node<Key, Value> *left);
    void setRight(Node<Key, Value> *right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value> *parent_;
    Node<Key, Value> *left_;
    Node<Key, Value> *right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template <typename Key, typename Value>
Node<Key, Value>::Node(const Key &key, const Value &value, Node<Key, Value> *parent) : item_(key, value),
                                                                                       parent_(parent),
                                                                                       left_(NULL),
                                                                                       right_(NULL)
{
}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed within the deleteAll() helper method in the BinarySearchTree.
*/
template <typename Key, typename Value>
Node<Key, Value>::~Node()
{
}

/**
* A const getter for the item.
*/
template <typename Key, typename Value>
const std::pair<const Key, Value> &Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template <typename Key, typename Value>
std::pair<const Key, Value> &Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template <typename Key, typename Value>
const Key &Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template <typename Key, typename Value>
const Value &Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template <typename Key, typename Value>
Value &Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template <typename Key, typename Value>
Node<Key, Value> *Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template <typename Key, typename Value>
Node<Key, Value> *Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template <typename Key, typename Value>
Node<Key, Value> *Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template <typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value> *parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template <typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value> *left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template <typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value> *right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template <typename Key, typename Value>
void Node<Key, Value>::setValue(const Value &value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree();                                                   //TODO
    virtual ~BinarySearchTree();                                          //TODO
    virtual void insert(const std::pair<const Key, Value> &keyValuePair); //TODO
    virtual void remove(const Key &key);                                  //TODO
    void clear();                                                         //TODO
    bool isBalanced() const;                                              //TODO
    void print() const;
    bool empty() const;

    template <typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> &tree);

public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator // TODO
    {
    public:
        iterator();

        std::pair<const Key, Value> &operator*() const;
        std::pair<const Key, Value> *operator->() const;

        bool operator==(const iterator &rhs) const;
        bool operator!=(const iterator &rhs) const;

        iterator &operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key, Value> *ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key &key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value> *internalFind(const Key &k) const;              // TODO
    Node<Key, Value> *getSmallestNode() const;                       // TODO
    static Node<Key, Value> *predecessor(Node<Key, Value> *current); // TODO
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot(Node<Key, Value> *r) const;
    virtual void nodeSwap(Node<Key, Value> *n1, Node<Key, Value> *n2);

    // Add helper functions here
    void PostOrderDelete(Node<Key, Value>* curr);
    static Node<Key, Value> *successor(Node<Key, Value> *current);
    bool subtreebalance(Node<Key,Value>* rootnode) const;

protected:
    Node<Key, Value> *root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template <class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key, Value> *ptr)
{
    this->current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template <class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() 
{
    this->current_ = nullptr;
}

/**
* Provides access to the item.
*/
template <class Key, class Value>
std::pair<const Key, Value> &
    BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template <class Key, class Value>
std::pair<const Key, Value> *
    BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template <class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator &rhs) const
{
    return this->current_ == rhs.current_;   
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template <class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator &rhs) const
{
    return this->current_ != rhs.current_;
}

/**
* Advances the iterator's location using an in-order sequencing
*/
template <class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator &
BinarySearchTree<Key, Value>::iterator::operator++()
{
    /* IFollowing finds the successor of th current node*/
     /* Case 1, there is a right child   */
    Node<Key, Value>* temp = this->current_;
    if(this->current_->getRight() != nullptr)
    {
        temp = this->current_->getRight();
        while(temp->getLeft() != nullptr)
        {
            temp = temp->getLeft();
        }
        
        this->current_ = temp;
        return *this;
    }
    
    /*Case 2: there is no right child to current */
    else
    {
        Node<Key,Value>* tempParent = this->current_->getParent();
        while(temp != nullptr && tempParent != nullptr)
        {
            if(tempParent->getLeft() == temp) // want to return the first node where current is the left child of the parent 
            {
                this->current_ = tempParent;
                return *this; 
            }
            else
            {
                if(tempParent->getRight() == temp)
                {
                    temp = temp->getParent();
                    tempParent = temp->getParent();
                }
            }
        }
        this->current_ = tempParent;
        return *this;
    
    }
    
}

/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template <class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{
    
    this->root_ = nullptr;
}

template <typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    this->clear();
}

/**
 * Returns true if tree is empty
*/
template <class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template <class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template <class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template <class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key &k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
*/
template <class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    if(this->empty()) // checks if its the first node
    {
        this->root_= new Node<Key, Value>(keyValuePair.first, keyValuePair.second, nullptr);
        return;
    }
    
    if(this->internalFind(keyValuePair.first) != nullptr) //if already exists , simply override value 
    {
        Node<Key,Value>* temp = this->internalFind(keyValuePair.first);
        temp->setValue(keyValuePair.second);
        return;
            
    } 

    Node<Key, Value>* temp = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, nullptr );
    Node<Key, Value>* curr = this->root_; //keeps track of what noode we are looking at

    while(curr != nullptr)
    {
        if(curr->getKey() < keyValuePair.first) // new pair is greater than current node 
        {
            if(curr->getRight() == nullptr) 
            {
                temp->setParent(curr);
                curr->setRight(temp);
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
                break; // already inserted , no need to go further
            }
            curr = curr->getLeft();
        }
    }
}

/**
* A remove method to remove a specific key from a Binary Search Tree.
* The tree may not remain balanced after removal.
*/
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key &key)
{
    if(this->empty()) return; //cannot remove on an empty list
    Node<Key, Value>* curr = internalFind(key);
    if(curr == nullptr) return; // not found
    bool flush = true;
    
    while(flush)
    {
        if(curr->getLeft() != nullptr && curr->getRight() == nullptr) //has left child only, promotion of child
        {
            Node<Key, Value>* tempParent = curr->getParent();
            if(tempParent == nullptr) // curr is the root 
            {
                this->root_ = curr->getLeft();
                this->root_->setParent(nullptr);
                delete curr;
                return;
            }
            if(tempParent -> getRight() == curr) 
            {
                tempParent->setRight(curr->getLeft());
                tempParent->getRight()->setParent(tempParent);
                delete curr;
                return;
            }
            if(tempParent -> getLeft() == curr)
            { 
                tempParent -> setLeft(curr->getLeft());
                tempParent->getLeft()->setParent(tempParent);
                delete curr;
                return;
            }

        }
        else if(curr->getLeft() == nullptr && curr->getRight() != nullptr) // has right child only, promotion of child
        {
            Node<Key, Value>* tempParent = curr->getParent();
            if(tempParent == nullptr) // curr is the root 
            {
                this->root_ = curr->getRight();
                this->root_->setParent(nullptr);
                delete curr;
                return;
            }
            if(tempParent -> getRight() == curr) 
            {
                tempParent->setRight(curr->getRight());
                tempParent->getRight()->setParent(tempParent);
                delete curr;
                return;
            }
            if(tempParent -> getLeft() == curr)
            { 
                tempParent -> setLeft(curr->getRight());
                tempParent->getLeft()->setParent(tempParent);
                delete curr;
                return;
            }
        }
        else if(curr->getLeft() != nullptr && curr->getRight() != nullptr) // has two children
        {  
            Node<Key, Value>* temp = predecessor(curr);
            this->nodeSwap(temp, curr);
            /* 
            what this does is that it uses the while loop to flush out the node we want to remove to be either one of the
            following cases: has only one child, or becomes a leaf node; 
            */
        }
        else
        {
            if(curr->getLeft() == nullptr && curr->getRight() == nullptr) // has no children
            {
                Node<Key, Value>* tempParent = curr->getParent();
                if(tempParent == nullptr) // case for single node
                {
                    this->clear(); 
                    return;
                } 
                
                if(tempParent -> getRight() == curr) tempParent->setRight(nullptr);
                if(tempParent -> getLeft() == curr) tempParent -> setLeft(nullptr);
                delete curr;
                return;
            } 
        
        }
    }
}

template <class Key, class Value>
Node<Key, Value> *
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value> *current)
{
    // TODO
    Node<Key, Value>* temp = current;
    if(temp->getLeft() != nullptr) // left child exists, therefor go to left, then most right 
    {
        temp = temp->getLeft();
        while(temp->getRight() != nullptr)
        {
            temp = temp->getRight();
        }
        return temp; 
    }
    else // left child does not exist, go up the chain until first right child is found 
    {
       Node<Key, Value>* tempParent = temp->getParent();
       while(tempParent != nullptr)
       {
           if(tempParent->getRight() == temp)
           {
               return temp;
           }
           else
           {
               temp = tempParent;
               tempParent = tempParent->getParent();
           }
       } 
       return tempParent;
    }
    
}

template <class Key, class Value>
Node<Key, Value> *
BinarySearchTree<Key, Value>::successor(Node<Key, Value> *current)
{
    /* Case 1, there is a right child   */
    Node<Key, Value>* temp = current;
    if(current->getRight() != nullptr)
    {
        temp = current->getRight();
        while(temp->getLeft() != nullptr)
        {
            temp = temp->getLeft();
        }
        
        return temp;
    }
    
    /*Case 2: there is no right child to current */
    else
    {
        Node<Key,Value>* tempParent = current->getParent();
        while(temp != nullptr && tempParent != nullptr)
        {
            if(tempParent->getLeft() == temp) // want to return the first node where current is the left child of the parent 
            {
                return tempParent; // 
            }
            else
            {
                if(tempParent->getRight() == temp)
                {
                    temp = temp->getParent();
                    tempParent = temp->getParent();
                }
            }
        }
        return tempParent;
    
    }
    


}

/* helper function to delete a tree erasing children first from left side to right, until reaches root */
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::PostOrderDelete(Node<Key,Value>* curr)
{
    if(curr->getLeft() != nullptr) {
        PostOrderDelete(curr->getLeft());
    }
    if(curr->getRight() != nullptr) {
        PostOrderDelete(curr->getRight());
    }
    delete curr;
}

/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    // TODO
    if(this->empty()) return;
    PostOrderDelete(this->root_);
    this->root_ = nullptr;
    return;
}

/**
* A helper function to find the smallest node in the tree.
*/
template <typename Key, typename Value>
Node<Key, Value> *
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    Node<Key,Value>* temp = this->root_; 
    if(temp->getLeft() == nullptr) return temp;
   
    while(temp->getLeft() != nullptr)
    {
        temp = temp->getLeft();
    }
    return temp;

}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template <typename Key, typename Value>
Node<Key, Value> *BinarySearchTree<Key, Value>::internalFind(const Key &key) const
{
    Node<Key,Value>* temp = this->root_; 
    while(temp != nullptr)
    {
        if(temp->getKey() == key) return temp;
        
        if(temp->getKey() < key)
        {
            temp = temp->getRight();   
        }
        else
        {
            temp = temp->getLeft();
        }
        
    }
    return temp;
}

/**
 * Return true iff the BST is balanced.
 */
template <typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    if(this->empty()) return true;
    
    BinarySearchTree<Key,Value>::iterator it;
    for(it = this->begin(); it != this->end(); ++it)
    {
        if(subtreebalance(it.current_) )
        {
            return false;
        }

    }
    

    return true;
}
template <typename Key, typename Value>
bool BinarySearchTree<Key, Value>::subtreebalance(Node<Key,Value>* rootnode) const
{

    int leftside =0;
    int rightside = 0;
    if(rootnode->getLeft() != nullptr )
    {
        leftside = getSubtreeHeight(rootnode->getLeft());
    }
    if(rootnode->getRight() != nullptr )
    {
        rightside = getSubtreeHeight(rootnode->getRight());
    }

    return abs(leftside - rightside) > 1; 
}


template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap(Node<Key, Value> *n1, Node<Key, Value> *n2)
{
    if ((n1 == n2) || (n1 == NULL) || (n2 == NULL))
    {
        return;
    }
    Node<Key, Value> *n1p = n1->getParent();
    Node<Key, Value> *n1r = n1->getRight();
    Node<Key, Value> *n1lt = n1->getLeft();
    bool n1isLeft = false;
    if (n1p != NULL && (n1 == n1p->getLeft()))
        n1isLeft = true;
    Node<Key, Value> *n2p = n2->getParent();
    Node<Key, Value> *n2r = n2->getRight();
    Node<Key, Value> *n2lt = n2->getLeft();
    bool n2isLeft = false;
    if (n2p != NULL && (n2 == n2p->getLeft()))
        n2isLeft = true;

    Node<Key, Value> *temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if ((n1r != NULL && n1r == n2))
    {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if (n2r != NULL && n2r == n1)
    {
        n1->setRight(n2);
        n2->setParent(n1);
    }
    else if (n1lt != NULL && n1lt == n2)
    {
        n2->setLeft(n1);
        n1->setParent(n2);
    }
    else if (n2lt != NULL && n2lt == n1)
    {
        n1->setLeft(n2);
        n2->setParent(n1);
    }

    if (n1p != NULL && n1p != n2)
    {
        if (n1isLeft)
            n1p->setLeft(n2);
        else
            n1p->setRight(n2);
    }
    if (n1r != NULL && n1r != n2)
    {
        n1r->setParent(n2);
    }
    if (n1lt != NULL && n1lt != n2)
    {
        n1lt->setParent(n2);
    }

    if (n2p != NULL && n2p != n1)
    {
        if (n2isLeft)
            n2p->setLeft(n1);
        else
            n2p->setRight(n1);
    }
    if (n2r != NULL && n2r != n1)
    {
        n2r->setParent(n1);
    }
    if (n2lt != NULL && n2lt != n1)
    {
        n2lt->setParent(n1);
    }

    if (this->root_ == n1)
    {
        this->root_ = n2;
    }
    else if (this->root_ == n2)
    {
        this->root_ = n1;
    }
}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif

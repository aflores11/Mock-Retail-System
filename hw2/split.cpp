/*
CSCI 104: Homework 2 Problem 6

Write a recursive function to split a sorted singly-linked
list into two sorted linked lists, where one has the even 
numbers and the other contains the odd numbers. Students 
will receive no credit for non-recursive solutions. 
To test your program write a separate .cpp file and #include
split.h.  **Do NOT add main() to this file**.  When you submit
the function below should be the only one in this file.
*/

#include "split.h"
#include <cstddef>


/* Add a prototype for a helper function here if you need */
void put_next(Node* &in, Node*& head);



void split(Node*& in, Node*& odds, Node*& evens)
{
  /* Add code here */
if(in == nullptr) return;

	Node* hold = in; // holds the current node
	in = in->next; // sets in to the next node getting rid of current node in Original List
	if(hold->value%2 ==0){ // checks if the current node's value is even
		
		if(evens == nullptr){
			evens = hold;
			evens->next=nullptr;
			split(in, odds, evens);
		}
		else{
			put_next(hold, evens);
			
			split(in, odds, evens);
		}
	}
	else{ // if the current node's value is not even, then it is odd
		if(odds == nullptr){
			odds = hold;
			odds->next=nullptr;
			split(in, odds, evens); //function adds the current node the end of the list
		}
		else{
			put_next(hold, odds);
			
			split(in, odds, evens);
		}
	}

}

/* If you needed a helper function, write it here */
//head = Even or Odd list
void put_next(Node*& in, Node*& head){
	Node* list = head;
	while(list != nullptr){
		if(list->next == nullptr) break;
		list = list->next;
	
	}	
	list->next=in;
	in->next=nullptr;
	
}
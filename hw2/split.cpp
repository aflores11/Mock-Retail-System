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
if(in == nullptr) return;

	Node* hold = in; // holds the current node
	in = in->next; // sets in to the next node getting rid of current node in Original List
	if(hold->value%2 ==0){ // checks if the current node's value is even
		
		if(evens == nullptr){ //empty list case
			evens = hold; // sets evens to be the current node
			evens->next=nullptr; // we only care about the current node so get rid of the rest
			split(in, odds, evens);
		}
		else{
			put_next(hold, evens);
			
			split(in, odds, evens);
		}
	}
	else{ // if the current node's value is not even, then it is odd
		if(odds == nullptr){ //empty list case
			odds = hold;
			odds->next=nullptr;
			split(in, odds, evens); 
		}
		else{ //not empty
			put_next(hold, odds); //function adds the current node the end of the list
			
			split(in, odds, evens);
		}
	}

}
//head = Even or Odd list
//purpose of this function is to set the node given to the end of the list to keep the order of the original list
void put_next(Node*& in, Node*& head){
	Node* list = head; // makes temporary node so it does not modify the original
	while(list != nullptr){ //goes through the list until it reaches the final node
		if(list->next == nullptr) break;
		list = list->next;
	
	}	
	list->next=in; // sets the current node to be the last node in the even/odd's list 
	in->next=nullptr;
	
}
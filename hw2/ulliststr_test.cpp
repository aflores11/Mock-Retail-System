/* Write your test code for the ULListStr in this file */

#include <iostream>
#include "ulliststr.h"
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
 

//test1 for popfront in empty List
 ULListStr List;
 List.pop_front();
 //if does not segfault
 cout << "Test1: passed" << endl;
 List.clear();

 //test2 for popback in empty List
 List.pop_back();
 //if does not segfault
 cout << "Test2: passed" << endl;
 List.clear();

 //Test3 for pushback on empty list
  List.push_back("1");
  if(List.size() == 1 && List.get(0) == "1"){
    cout << "Test3: passed"  << endl; 
  }
  else{
    cout << "Test3: failed"  << endl; 
  }
  List.clear();

//Test4: pushfront on empty list
  List.push_front("1");
  if(List.size() == 1 && List.get(0) == "1"){
    cout << "Test4: passed"  << endl; 
  }
  else{
    cout << "Test4: failed"  << endl; 
  }
  List.clear();

//Test5: multiple pushbacks on one item
    
  for(int i=0; i<7;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }
  if(List.size() == 7){
    cout << "Test5: passed"  << endl;
  }
   else{
    cout << "Test5: failed"  << endl; 
  }
  List.clear();

//Test6: multiple push_front on two items
    for(int i=0; i<7;i++){
    string temp = to_string(i);
    List.push_front(temp);
  }
  if(List.size() == 7){
    cout << "Test6: passed"  << endl;
  }
   else{
    cout << "Test6: failed"  << endl; 
  }
  List.clear();

//Test7: multiple pushbacks, making two items
    
  for(int i=0; i<13;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }
  if(List.size() == 13){
    cout << "Test7: passed"  << endl;
  }
   else{
    cout << "Test7: failed"  << endl; 
  }
  List.clear();

//test8 for popfront on List of 1 val
 List.push_back("0");
 List.pop_front();
 if(List.size() == 0){
  cout << "Test8: passed" << endl;
 }
 else{
    cout << "Test8: failed" << endl;
  }
List.clear();

//test9 for popback on list of 1 val
 
 List.push_back("0");
 List.pop_back();
 if(List.size() == 0){
  cout << "Test9: passed" << endl;
 }
 else{
    cout << "Test9: failed" << endl;
  }
List.clear();

//Test10 successive pop backs on one item
 for(int i= 0; i<9 ;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }

  for(int i=0;i<5;i++){
    List.pop_back();
  }

  if(List.size()==4){
    cout << "Test10: passed" << endl;
  }
  else{
    cout << "Test10: failed" << endl;
  }
  List.clear();

//Test11 successive pop front on one item
 for(int i= 0; i<9 ;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }

  for(int i=0;i<5;i++){
    List.pop_front();
  }

  if(List.size()==4){
    cout << "Test11: passed" << endl;
  }
  else{
    cout << "Test11: failed" << endl;
  }
  List.clear();
  
//Test12 successive pop front over multiple items
 
 for(int i= 0; i<25 ;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }

  for(int i=0;i<15;i++){
    List.pop_front();
  }

  if(List.size()==10){
    cout << "Test12: passed" << endl;
  }
  else{
    cout << "Test12: failed" << endl;
  }
  List.clear();
  
//Test13 successive pop back over multiple items
 
 for(int i= 0; i<25 ;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }

  for(int i=0;i<15;i++){
    List.pop_back();
  }

  if(List.size()==10){
    cout << "Test13: passed" << endl;
  }
  else{
    cout << "Test13: failed" << endl;
  }
  List.clear();
  
//Test 14 push back and front silmultaneously

  for(int i= 0; i<12 ;i++){
    string temp = to_string(i);
    List.push_back(temp);
    List.push_front(temp);
  }

  if(List.size()==24){
    cout << "Test14: passed" << endl;
  }
  else{
    cout << "Test14: failed" << endl;
  }
  List.clear();

//Test15 front function on one item

  for(int i=0;i<5;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }

  if(List.front() == "0"){
    cout << "Test15: passed" << endl;
  }
  else{
    cout << "Test15: failed" << endl;
  }
  List.clear();

//Test16 back function on one item

  for(int i=0;i<5;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }

  if(List.back() == "4"){
    cout << "Test16: passed" << endl;
  }
  else{
    cout << "Test16: failed" << endl;
  }
  List.clear();

//Test17 front function multiple items

  for(int i=0;i<24;i++){
    string temp = to_string(i);
    List.push_front(temp);
  }

  if(List.front() == "23"){
    cout << "Test17: passed" << endl;
  }
  else{
    cout << "Test17: failed" << endl;
  }
  List.clear();

//Test18 back function multiple items

  for(int i=0;i<25;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }

  if(List.back() == "24"){
    cout << "Test18: passed" << endl;
  }
  else{
    cout << "Test18: failed" << endl;
  }
  List.clear();

//Test19 back/front function on mixed puh_back and push_front
  for(int i=0;i<23;i++){
    string temp = to_string(i);
    List.push_back(temp);
    List.push_front(temp);
  }

  if(List.back() == List.front()){
    cout << "Test19: passed" << endl;
  }
  else{
    cout << "Test19: failed" << endl;
  }
  List.clear();

//Test20 get at one location on single item
  for(int i=0;i<9;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }

  if(List.get(5) == "5"){
    cout << "Test20: passed" << endl;
  }
  else{
    cout << "Test20: failed" << endl;
  }
  List.clear();




//Test21 get one location on multiple items
  for(int i=0;i<25;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }

  if(List.get(14) == "14"){
    cout << "Test21: passed" << endl;
  }
  else{
    cout << "Test21: failed" << endl;
  }
  List.clear();

//Test22 successive gets on one item
  for(int i=0;i<9;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }
  bool final = true;
  for(int i=0;i<9;i++){
    string temp = to_string(i);
    if(List.get(i) == temp){
      final = true;
    }
    else{
      final = false;
      break;
    }

  }

  if(final == true){
    cout << "Test22: passed" << endl;
  }
  else{
    cout << "Test22: failed" << endl;
  }
  List.clear();

//Test23 successive gets on multiple items
  for(int i=0;i<30;i++){
    string temp = to_string(i);
    List.push_back(temp);
  }
  
  final = true;
  
  for(int i=7;i<20;i++){
    string temp = to_string(i);
    if(List.get(i) == temp){
      final = true;
    }
    else{
      final = false;
      break;
    }

  }

  if(final == true){
    cout << "Test23: passed" << endl;
  }
  else{
    cout << "Test23: failed" << endl;
  }
  List.clear();

  return 0;
}

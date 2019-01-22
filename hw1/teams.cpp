#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// *You* are not allowed to use global variables
//  but for just the output portion *we* will. ;>
int combo = 1;

// @brief Prints a single combination of teams
//
// @param[in] team1 Array containing the names of team 1
// @param[in] team2 Array containing the names of team 2
// @param[in] len Size of each array
void printSolution(const string* team1, 
		   const string* team2,
		   int len)
{
  cout << "\nCombination " << combo++ << endl;
  cout << "T1: ";
  for(int i=0; i < len; i++){
    cout << team1[i] << " ";
  }
  cout << endl;
  cout << "T2: ";
  for(int i=0; i < len; i++){
    cout << team2[i] << " ";
  }
  cout << endl;
}

// You may add additional functions here
void make_teams(string* names, string* team1, string* team2, int array_size, int len, int iteration);

void get_first_thing(string& hold); // Removes whitespace from first line in order to check
//if first thing given is a number;


int main(int argc, char* argv[])
{
  if(argc < 2){
    cerr << "Please provide a file of names" << endl;
    return 1;
  }
  // Complete the rest of main

 
  ifstream ifile(argv[1]);  
  if(ifile.fail()){ // check if file exists.
    cout << "Error" << endl;
    return 1;
  }
  string input, temp;
  int array_size;
  getline(ifile, input);
  if(ifile.fail()){
    cout << "Error" << endl;
    return 1;
  } 

  char* numberholder = new char[input.size()];
  string cheker = input;
  get_first_thing(cheker);
  if(cheker.empty()){ // checks if the first line where the  number is supposed to be is empty
    cout << "Error" << endl;
    return 1;
  }

  for(int i=0; i<(int)cheker.size();i++){  // checks if the first thing given is a number
    numberholder[i] = cheker[i] ;
    
    if(!isdigit(numberholder[i])){
    cout << "Error" << endl;
    return 1;
    }
  
  }




  stringstream buf(input);
  buf >> array_size;
  
  
//allocates array for all names included
  string* names = new string[array_size];
  input.clear();

//line 93 to 123 add names to the names array
  getline(ifile, input);
  int array_locator = 0;
  int name_size_check=0;   
  
  while(!ifile.fail()){
    

    string holder;
    stringstream tempstream(input);
    tempstream >> holder;

    while(!tempstream.fail()){
      
      if(name_size_check >= array_size){ // checks if more names are provided than told
        cout <<"Error" << endl;
        return 1;
      }
      
      names[array_locator] = holder;

      array_locator++;
      name_size_check++;
      tempstream >> holder;
      
    }
    
    getline(ifile, input);

  
  }
//checks if the number inputed matches the number of names given
  if(name_size_check != array_size){  
    cout << "Error" << endl;
    return 1;
  }

//allocates space to hold two teams
  string* team1 = new string[array_size/2];
  string* team2 = new string[array_size/2];
  int iteration =0;
  int len = 0;
  make_teams(names, team1, team2, array_size, len, iteration);

cout << endl;



delete[] names;
delete[] team1;
delete[] team2;
delete[] numberholder;

  return 0;
}


void make_teams(string* names, string* team1, string* team2, int array_size, int len, int iteration){
 //makes all elements in team2 blank since they were allocated with nothing inside 
  if(len == array_size/2){
    for(int i=0; i<array_size/2;i++){
      team2[i] = ""; 
    }

    //used to keep track of where we are in team2 array
    int index=0;
    
    for(int i=0; i<array_size;i++){ //index for team1 array
      for(int j=0; j < (array_size/2);j++){ //index for the names array

        if(names[i] == team1[j]){  //if the name on t1 matches a name break the loop and go
         break;                    //go to the next index in team1 array
        }
        else{
          if(j == array_size/2 -1 ){ //checks if its the last instance of the loop to enusre there are no more names to check
            if(names[i] != team1[j]){ //makes sure the last name does not match the index of team1
              team2[index] = names[i]; //sets team2[index] to the name not found in team 1
              index+=1; //goes to the next index of team2 to fill name

             }
          }
        }
      }    
    }
    
    printSolution(team1,team2,array_size/2);
  }
  else{ 

    //len keeps track of index of team1; iteration keeps track of the name on the names array
    for(int i=iteration; i<array_size; i++){
      team1[len]=names[i];
      make_teams(names,team1, team2, array_size,len+1, i+1);      
    }

  }

}



//gets rid of whitespace and tabs on the first getline to check if first item is a valid number
void get_first_thing(string& hold){
  if( hold.empty()) return;
  int interation =0;

  for(int i=0; i<(int)hold.size();i++){

    if(interation== (int)hold.size()-1 && hold[interation] != ' '){ //base case: if string only contains numbers or letters return
      return;
    }

  }

  if(hold[0] == ' ' || hold[0] == ' '){ //checks if first character is space or tab
      hold.erase(0,1);
      get_first_thing(hold);
   }
   
    for(int j=0; j<(int)hold.size(); j++){  //check if current char is a space or tab
      if(hold[j] == ' ' || hold[j] == ' '){
      hold.erase(j,hold.size());  // creates a new string with everything before the space or tab
      get_first_thing(hold);
      }
     }
     
 } 
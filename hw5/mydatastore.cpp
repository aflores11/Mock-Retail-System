#include "mydatastore.h"
#include <iostream>
#include <set>
#include <map>
#include "util.h"
#include "user.h"
#include "review.h"
#include <iomanip>
#include <sstream>

using namespace std;

MyDataStore::MyDataStore(){

}

MyDataStore::~MyDataStore(){

    KeytoProd.clear(); //clears map of keywords to products
    ProdReviews.clear(); //clears map of string to Reviews
    pNametoP.clear();
    
    for(map<string,User*>::iterator it = allusers.begin();it != allusers.end(); ++it)
    {    
        delete it->second; //deletes all the users that were allocated
    }
    allusers.clear(); //clears to username to user map

    for(set<Product*>::iterator it= allproducts_.begin(); it != allproducts_.end(); ++it)
    {
        delete *it; //deletes all allocated products
    }
    allproducts_.clear(); //erases everything in the products set

    for(set<Review*>::iterator it = allReviews.begin();it != allReviews.end(); ++it)
    {    
        delete *it; //deletes all the Reviews that were allocated
    }
    ProdReviews.clear(); //erases everything in the reviews set


}

void MyDataStore::addProduct(Product* p){

    allproducts_.insert(p); //we want to add it in the product set to later check if it exists
    pNametoP.insert(make_pair(p->getName(), p));
    std::set<string> keywrd =p->keywords();   
    for(set<string>::iterator it = keywrd.begin(); it!= keywrd.end(); ++it)
    {
        string hold = *it;
        if(KeytoProd.find(hold) == KeytoProd.end()){  //checks if the index does not already exist. If not, create and start product set
            set<Product*> newprod;
            newprod.insert(p);
            KeytoProd.insert(std::pair<string,set<Product*> >(hold, newprod));
        }
        else{ //key already exists. Copy the prod set in the key, add the new product, override update set back in
            std::set<Product*> temp = KeytoProd[hold];
            temp.insert(p);
            KeytoProd[hold] = temp;
        }

    } 
}

void MyDataStore::addUser(User* u){
    allusers.insert(pair<string, User*>(u->getName(), u)); //adds to the map a new username to user object pointer pair
    vector<Product*> emptyvec;
    userCart.insert(pair<std::string, std::vector<Product*> >(u->getName(), emptyvec)); //initializes this users cart
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type){
       vector<Product*> results;
       set<Product*> preresult;

       if(type == 0) // AND search
       {
            
            if(KeytoProd.find(terms[0]) != KeytoProd.end())
            {
                preresult = KeytoProd[terms[0]]; 
            }
            else return results; //means first term not found

            if(terms.size() > 1) //if there are more than one term
            {
                for(int i=1; i<(int)terms.size(); i++)
                {
                    if(KeytoProd.find(terms[i]) == KeytoProd.end())
                    {
                        return results; //if one keyword out of all others are not found, return an empty vector
                    }
                    else
                    {
                        set<Product*> tempset = setIntersection(preresult, KeytoProd[terms[i]]); //singles out products that match every keyword
                        preresult = tempset;
                    }
                }
            }

            //pushes all the products found with the same keywords into ouput vector
            for(set<Product*>::iterator it = preresult.begin(); it != preresult.end(); ++it)
            {
                results.push_back(*it);
            }  
            
            return results;
       }

       else //OR search
       {    
            
            //searches through all terms to see if any are found
            for(int i=0; i<(int)terms.size(); i++)
            {
                set<Product*> specificset = KeytoProd[terms[i]];// copy current keyword's set of products
                set<Product*> tempset = setUnion(preresult, specificset); //makes union of output set and current keyword's set
                preresult = tempset; //updates ouput set
            }
        
            //pushes every match in the set into a ouput vector
            for(set<Product*>::iterator it = preresult.begin(); it != preresult.end(); ++it)
            {
                results.push_back(*it);
            }  

            return results;
       }
}

void MyDataStore::dump(std::ostream& ofile){
    ofile << "<products>\n";
    //iterates through all products and prints them out
    for(set<Product*>::iterator it = allproducts_.begin(); it != allproducts_.end(); ++it)
        {
            Product* prod = *it; 
            prod->dump(ofile);
        } 
    ofile << "</products>\n";
    ofile <<"<users>\n";
    //iterates through all users to print
    for(map<string,User*>::iterator it = allusers.begin(); it != allusers.end(); ++it)
        {
            User* temp = it->second;
            temp->dump(ofile);
        } 
    ofile << "</users>\n";
    ofile << "<reviews>\n";
    //iterates through all products and prints them out
    for(map<string, vector<Review*> >::iterator it = ProdReviews.begin(); it != ProdReviews.end(); ++it)
        {
            vector<Review*> prod = it->second; 
            for(int i =0; i<(int)prod.size(); i++)
            {
                prod[i]->dump(ofile, it->first);
            }
        } 
    ofile << "</reviews>\n";
}

//used in amazon for checking if a user inputed exists
const bool MyDataStore::find_user(const string& u)
{
    if(allusers.find(u) == allusers.end())
    {
        return false;
    }
    
    return true;
}


void MyDataStore::addReview(const std::string& prodName, int rating, const std::string& username,
			                const std::string& date, const std::string& review_text)
{
    
    if(pNametoP.find(prodName) == pNametoP.end()) return;

    Review* temp = new Review(rating, username, date, review_text);
    allReviews.insert(temp);

    if(ProdReviews.find(prodName) == ProdReviews.end() )
    {
        vector<Review*> NewVec;
        NewVec.push_back(temp);
        ProdReviews.insert(make_pair(prodName, NewVec));
        pNametoP[prodName]->Rating_= AvgRating(prodName);

    }
    else
    {
        vector<Review*> NewVec = ProdReviews[prodName];
        NewVec.push_back(temp);
        ProdReviews[prodName] = NewVec;
        pNametoP[prodName]->Rating_= AvgRating(prodName);
    }    
   

}

string MyDataStore::AvgRating(std::string productName)
    {
        vector<Review*> temp = ProdReviews[productName];
        double rting =0;

        for(int i=0 ; i<(int)temp.size(); i++)
        {
            rting += (double)temp[i]->rating;
        }

        rting= rting/temp.size();
        stringstream dummy;
        dummy << fixed << setprecision(2) << rting;

        return dummy.str();
    }
        
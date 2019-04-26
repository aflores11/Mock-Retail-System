#include "mydatastore.h"
#include <iostream>
#include <set>
#include <map>
#include "util.h"
#include "user.h"
#include "review.h"
#include <iomanip>
#include <sstream>
#include <cmath>
#include <numeric>
#include "heap.h"
#include <fstream>

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
    
    if(pNametoP.find(prodName) == pNametoP.end()) return; // checks if product exists 

    Review* temp = new Review(rating, username, date, review_text);
    allusers[username]->insertProdRev(pNametoP[prodName], rating); //lets us track every product a user has reviewed
    allReviews.insert(temp);
    pNametoP[prodName]->haveRated_.insert(username);//keeps track of all the users that have rated this specific product

    if(ProdReviews.find(prodName) == ProdReviews.end() ) //makes a new review vector if a product has no reviews
    {
        vector<Review*> NewVec;
        NewVec.push_back(temp);
        ProdReviews.insert(make_pair(prodName, NewVec));
        pNametoP[prodName]->Rating_= AvgRating(prodName);

    }
    else //updates the vector of reviews with newest addition
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

bool MyDataStore::checkdate(std::string &in)
{
    if(in.size() != 10) return false;
    stringstream ss(in);
    char temp;
    for(int i =0; i<4;i++) //checks year
    {
        if(ss>>temp)
        {
            if(temp<=47 ||temp>=58) return false;
        }  
        else return false; 
    }
    if(ss>>temp) //make sure there is dash
    {
        if(temp != '-') return false;
    }
    char m1dig = 126; //set to dummy char '~'
    char m2dig =126;// holds second digit of the month

    if(ss>>temp) //checks month first digit
    {
        if(temp <= 47 || temp >= 50) return false; // 0 to 1 inclusive
        m1dig = temp;
    }
    if(ss>>temp) //checks month second digit
    {
        if(temp<=47 ||temp>=58) return false;
        if(m1dig == '0' && temp == '0') return false;
        else if(m1dig == '1')
        {
            if(temp <=47 || temp > 51) return false; //has to be between 0 and 2 inclusive
            m2dig=temp;
        }
        else m2dig = temp;
    }  
    if(ss>>temp) //make sure there is dash
    {
        if(temp != '-') return false;
    }
    char d1dig; // holds first digit of date
    if(ss>>temp)//check date first digit
    {
        if(temp <=47 || temp >= 52) return false; // 0 to 3 inclusive
        if(m1dig == '0'&& m2dig == '2')
        {
            if(temp == '3') return false;
            d1dig = temp;
        }
        else
        {
            d1dig = temp;
        } 
    }  
    if(ss>>temp)//check date second digit
    {
        if(temp <= 47 || temp >= 58) return false; // 0 to 9 inclusive
        
        if(m1dig == '0'&& m2dig == '2' && d1dig == '2') //case for February
        {
            if(temp<=47 || temp >= 57) return false; // can't pass 28
        }
        else if ((m1dig == '0' && m2dig == '4') || (m1dig == '0' && m2dig == '6') || (m1dig == '0' && m2dig == '9') || (m1dig == '1' && m2dig == '1')) 
        {
            if(d1dig == '3' && temp != '0') return false; //months that only go up to 30
        } 
         else if ((m1dig == '0' && m2dig == '1') || (m1dig == '0' && m2dig == '3') || (m1dig == '0' && m2dig == '5') || (m1dig == '0' && m2dig == '7') || (m1dig == '0' && m2dig == '8') || (m1dig == '1' && m2dig == '0')  || (m1dig == '1' && m2dig == '2') )
         {
            if(d1dig == '3' && ( temp <= 47 || temp >= 50)) return false;  // months that go up to 31
         }
    } 


    return true;
}

 double MyDataStore::calc_Basic_Similarities(std::string &currUser, std::string &otherUser)
 {
     std::set<Product*> currUserset = allusers[currUser]->getRevProds();
     std::set<Product*> otherUserset = allusers[otherUser]->getRevProds();
     std::set<Product*> sameProdsReviewed = setIntersection(currUserset, otherUserset);
     if(sameProdsReviewed.empty())
     {
         return 1.0;
     }
    
    vector<double> sim; 
    /*adds up all the common products they reviewed and gets a score relating their ratings*/
    for(auto it = sameProdsReviewed.begin(); it != sameProdsReviewed.end(); ++it)
    {
        double basicSimilarity = abs( (double)allusers[currUser]->specificProdRating(*it) - (double)allusers[otherUser]->specificProdRating(*it))/4.0;
        sim.push_back(basicSimilarity);
    }


    return    (double)(std::accumulate(sim.begin(), sim.end(), 0.00))/( sim.size() ) ;

 }

std::vector<std::pair<std::string, double> > MyDataStore::makeSuggestion(std::string currentUser)
{
    std::map<std::string, double> refined;
    Heap<std::string> pq;
  
    auto curr = make_pair(allusers[currentUser], 0.0);
    refined.insert(make_pair(currentUser, curr.second));
    pq.push(curr.second, currentUser);

    for(auto it = allusers.begin(); it != allusers.end(); ++it) //creating list 
    {
        if(it->first != currentUser)
        {
            string temp = it->first; 
            refined.insert(make_pair(temp, 2.0)); //2.0 is the equivalent of infinity sine max is 1.0
            pq.push(2.0, temp);

        }
    }
    /**** Dijktra's Implementation */
    while(!pq.empty())
    {
        string currUSR = pq.top();
        pq.pop();
        std::map<User*,double> neighbors = getBasicSimGraph_(currUSR);
        for(auto iter = neighbors.begin(); iter != neighbors.end(); ++iter)
        {
            double weight = iter->second;
            if(refined[currUSR] + weight < refined[iter->first->getName()])
            {
                refined[iter->first->getName()] = refined[currUSR] + weight;
                pq.decreaseKey(refined[currUSR] + weight, currUSR);
            }
        }
    }


    allusers[currentUser]->updateRefineSim(refined);
     
    vector<std::pair<string, double> > results;
  
    set<Product*> userprodrev = allusers[currentUser]->getRevProds();
    set<Product*> prodNotRevewed = setDifference(allproducts_, userprodrev);//gets the products current user has not reviewed

    //calculates the ratings for potential recommendations
    for(auto it = prodNotRevewed.begin(); it != prodNotRevewed.end() ; ++it)
    {
        double sum = 0.00;
        double w = 0.00;
        set<string> usersWhoRated = (*it)->haveRated_ ;
        if(usersWhoRated.empty()) continue;
        for(auto iter = usersWhoRated.begin(); iter != usersWhoRated.end(); ++iter)
        {
            int r = allusers[*iter]->specificProdRating(*it) ;
            double s = refined[*iter];
            sum += (1-s)*(double)r;
            w += 1-s;
        }
        double newrating = 0.00;
        if(w == 0.00) newrating = 0.00;
        else
        {
            newrating = sum/w;
        }

        results.push_back(make_pair((*it)->getName(), newrating));
         
    }
    return results;  
}

std::map<User*, double> MyDataStore::getBasicSimGraph_(std::string &usR)
{
    std::map<User*,double > result;
    for(auto it = allusers.begin(); it != allusers.end(); ++it)
    {
        if(it->first != usR)
        {
            string temp = it->first;
            result.insert(make_pair(it->second, calc_Basic_Similarities(usR, temp)));//builds main graph inserting 
        }
    }

    return result;
}



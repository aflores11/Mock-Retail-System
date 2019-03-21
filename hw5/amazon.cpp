#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "book.h"
#include "movie.h"
#include "clothing.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"
#include "msort.h"
#include <string>

using namespace std;
// Functors
struct ByName {
  bool operator()(Product* a, Product* b)
  {
    string name1 = a->getName();
    string name2 = b->getName();

    return a<b;
    
  }
};


struct ByRating {
  bool operator()(Product* a, Product* b)
  {
    double left = stod(a->Rating_);
    double right = stod(b->Rating_);

    return left > right;
    
  }
};

struct ByDate {
  bool operator()(Review* a, Review* b)
  {

    return a->date > b->date;
    
  }
};
//////////////////////

struct ProdNameSorter
{
    bool operator()(Product *p1, Product *p2)
    {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product *> &hits, char in); // used for searching AND/OR r/n
void displayProducts(vector<Product *> &hits); // used for displaying cart when checking out
void displayCart(vector<Product *> &hits);
void revDump(std::ostream &os, std::string &name, const vector<Review*> temp);


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;

    // Instantiate the individual section and product parsers we want
    ProductSectionParser *productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser *userSectionParser = new UserSectionParser;
    ReviewSectionParser* reviewSectionParser = new ReviewSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);
    parser.addSectionParser("reviews", reviewSectionParser);

    // Now parse the database to populate the DataStore
    if (parser.parse(argv[1], ds))
    {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  LOGIN username                     " << endl;
    cout << "  LOGOUT                          "    << endl;
    cout << "  AND r/n term term ...                  " << endl;
    cout << "  OR r/n term term ...                   " << endl;
    cout << "  ADD search_hit_number     "              << endl;
    cout << "  VIEWCART                   "           << endl;
    cout << "  BUYCART                    "            << endl;
    cout << "  ADDREV seach_hit_number rating date review_text  " << endl;
    cout << "  VIEWREV seach_hit_number           " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product *> hits;
    bool done = false;
    while (!done)
    {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin, line);
        stringstream ss(line);
        string cmd;
        if ((ss >> cmd))
        {
            
            if(cmd == "LOGIN")
            {
                string usr_;
                if (ss >> usr_)
                {
                    if(ds.find_user(usr_))
                    {
                        ds.loggedUser = usr_;
                    }
                    else{
                        cout << "Invalid user\n";
                    }
                }
            }
            else if(cmd == "LOGOUT")
            {
                if(! ds.loggedUser.empty())
                {
                    ds.loggedUser.clear();
                }
            }
            else if (cmd == "AND")
            {
                
                char option;
                
                if(ss>>option)
                {
                    if(option == 'r' || option == 'n')
                    {

                    } 
                    else continue;
                }
                else continue;
                
                string term;
                vector<string> terms;
                while (ss >> term)
                {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);

                displayProducts(hits,option);
            }
            else if (cmd == "OR")
            {
                char option;
                
                if(ss>>option)
                {
                    if(option == 'r' || option == 'n')
                    {

                    } 
                    else continue;
                }
                else continue;
                
                string term;
                vector<string> terms;
                while (ss >> term)
                {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                
                displayProducts(hits, option);
                
                
            }
            else if (cmd == "QUIT")
            {
                string filename;
                if (ss >> filename)
                {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
            else if(cmd == "ADD"){
                
                if(ds.loggedUser.empty())
                {
                    cout << "No current user\n";
                    continue;
                }
                        
                        int search_hit_num;
                        if(ss >> search_hit_num)
                        {
                            if(search_hit_num > (int)hits.size()) //checks if user choosing a valid found item
                            {
                                cout << "Invalid request\n";
                            }
                            else
                            {
                                vector<Product*> temp = ds.userCart[ds.loggedUser]; //copy our current cart to update it 
                                temp.push_back(hits[search_hit_num - 1]); //subtract 1 to search hit because we are 0 indexed
                                ds.userCart[ds.loggedUser] = temp; //overrides old cart with new cart
                            }

                        } 
                        else 
                        {
                            cout << "Invalid request\n";
                        }  
                    
                
            }
            else if(cmd == "VIEWCART")
            {
                if(ds.loggedUser.empty())
                {
                    cout << "No current user\n";
                    continue;
                }
 
                displayCart(ds.userCart[ds.loggedUser]); //implemented on bottom
                cout << "\n\n";
                    
                
            }
            else if(cmd == "BUYCART")
            {
                if(ds.loggedUser.empty())
                {
                    cout << "No current user\n";
                    continue;
                }

                        string userName = ds.loggedUser;
                        vector<int> indexes; //used to track index of products that are being bought
                        vector<Product*> cart = ds.userCart[userName]; 
                        
                        for(int i=0; i<(int)cart.size(); i++)
                        {
                            if(cart[i]->getQty() == 0) continue; //checks if in stock

                            double price = cart[i]->getPrice();
                            if(price > ds.allusers[userName]->getBalance()) continue; //checks if user can afford it
                            ds.allusers[userName]->deductAmount(price);
                            cart[i]->subtractQty(1);
                            indexes.push_back(i);
                        }
                        
                        for(int i = indexes.size() -1 ; i>=0; i--)
                        {
                            cart.erase(cart.begin() + indexes[i]); //erases products from cart from back to front so vector indexes do not mess up
                        }
                        ds.userCart[userName] = cart; //overrides old cart with updated cart
                   
                    
            }
            else if(cmd == "ADDREV")
            {
                if(ds.loggedUser.empty())
                {
                    cout << "No current user\n";
                    continue;
                }
                int hitnum;
                if(ss >> hitnum)
                {
                    if(hitnum > (int)hits.size()) //checks if user choosing a valid found item
                        {
                            cout << "Invalid request\n";
                            continue;
                        }
                    else
                    {
                        int rating;
                        string text, date;

                        string pname = hits[hitnum-1]->getName();
                        vector<Review*> temp = ds.ProdReviews[pname];
                        ss>>rating;
                        if(ss.fail() || (rating <0 || rating >5)) continue; 
                        ss>>date;
                        if(ss.fail() || !ds.checkdate(date)) continue;
                        
                        if(getline(ss, text))
                        {
                            text = trim(text);
                        }
                        Review* rev = new Review(rating, ds.loggedUser, date, text);
                        ds.allReviews.insert(rev);
                        temp.push_back(rev);
                        ds.ProdReviews[pname] = temp;
                    }    
                }
                else
                {
                    continue;
                }
            }
            else if(cmd == "VIEWREV")
            {
                if(ds.loggedUser.empty())
                {
                    cout << "No current user\n";
                    continue;
                }
                int hitnum;
                if(ss >> hitnum)
                {
                    if(hitnum > (int)hits.size()) //checks if user choosing a valid found item
                        {
                            cout << "Invalid request\n";
                            continue;
                        }
                    else
                    {
                        string _name = hits[hitnum-1]->getName();
                        revDump(cout, _name, ds.ProdReviews[_name]);
                    }    
                }


            }
            else
            {
                cout << "Unknown command" << endl;
            }
        }
    }
    return 0;
}

void displayProducts(vector<Product *> &hits, char in)
{
    int resultNo = 1;

    if(in == 'r')
    {
        mergeSort(hits, ByRating());
    }
    if(in == 'n')
    {
        mergeSort(hits, ByName());
    }
    for (vector<Product *>::iterator it = hits.begin(); it != hits.end(); ++it)
    {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}

void displayProducts(vector<Product *> &hits)
{
    int resultNo = 1;
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for (vector<Product *>::iterator it = hits.begin(); it != hits.end(); ++it)
    {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}




//offshoot of display products where only "hit" is changed to "Item" and is not sorted to keep original order of products added
void displayCart(vector<Product *> &cart)
{
    int resultNo = 1;
    for (vector<Product *>::iterator it = cart.begin(); it != cart.end(); ++it)
    {
        cout << "Item " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}

void revDump(std::ostream &os, std::string &name, const vector<Review*> temp)
{
    vector<Review*> srted = temp;
    mergeSort(srted, ByDate());
    for(int i=0; i<(int)srted.size(); i++)
    {
        os << srted[i]->rating << " " << srted[i]->username << " " << srted[i]->date << " "
         << srted[i]->reviewText << "\n";
    }

}
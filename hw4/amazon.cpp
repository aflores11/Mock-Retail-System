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

using namespace std;
struct ProdNameSorter
{
    bool operator()(Product *p1, Product *p2)
    {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product *> &hits);
void displayCart(vector<Product *> &hits);

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

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if (parser.parse(argv[1], ds))
    {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
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
            if (cmd == "AND")
            {
                string term;
                vector<string> terms;
                while (ss >> term)
                {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                displayProducts(hits);
            }
            else if (cmd == "OR")
            {
                string term;
                vector<string> terms;
                while (ss >> term)
                {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                displayProducts(hits);
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
                string userName;
                if(ss >> userName)
                {
                    if(!ds.find_user(userName)) //checks if username exists
                    {
                        cout << "Invalid request\n";
                    }
                    else
                    {
                        int search_hit_num;
                        if(ss >> search_hit_num)
                        {
                            if(search_hit_num > (int)hits.size()) //checks if user choosing a valid found item
                            {
                                cout << "Invalid request\n";
                            }
                            else
                            {
                                vector<Product*> temp = ds.userCart[userName]; //copy our current cart to update it 
                                temp.push_back(hits[search_hit_num - 1]); //subtract 1 to search hit because we are 0 indexed
                                ds.userCart[userName] = temp; //overrides old cart with new cart
                            }

                        } 
                        else 
                        {
                            cout << "Invalid request\n";
                        }  
                    }
                }
            }
            else if(cmd == "VIEWCART")
            {
                string userName;
                if(ss >> userName){
                    if(!ds.find_user(userName)) //verifies user exists
                    {
                        cout << "Invalid username\n";
                    }
                    else
                    {
                        displayCart(ds.userCart[userName]); //implemented on bottom
                        cout << "\n\n";
                    }
                }
            }
            else if(cmd == "BUYCART")
            {
                string userName;
                if(ss >> userName)
                {
                    if(!ds.find_user(userName)) //verifies user exists
                    {
                        cout << "Invalid username\n";
                    }
                    else
                    {
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

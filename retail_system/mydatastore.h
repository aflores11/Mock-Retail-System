#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "product.h"
#include "user.h"
#include <map>
#include <set>
#include "review.h"
#include <iostream>
#include <vector>
#include <fstream>

class MyDataStore : public DataStore{
    public:

        MyDataStore();

        ~MyDataStore();

        /**
         * Adds a product to the data store
         */
        void addProduct(Product* p);

        /**
         * Adds a user to the data store
         */
        void addUser(User* u);

        /**
         * Performs a search of products whose keywords match the given "terms"
         *  type 0 = AND search (intersection of results for each term) while
         *  type 1 = OR search (union of results for each term)
         */
        std::vector<Product*> search(std::vector<std::string>& terms, int type);

        /**
         * Reproduce the database file from the current Products and User values
         */
        void dump(std::ostream& ofile);

        const bool find_user(const std::string& u);

        double calc_Basic_Similarities(std::string &currUser, std::string &otherUser);

        std::set<Product*> allproducts_; 
        std::map<std::string, User*> allusers;
        std::map<std::string, std::set<Product*> > KeytoProd;
        std::map<std::string, std::vector<Product*> > userCart;
        std::map<std::string, std::vector<Review*> > ProdReviews;
        std::set<Review*> allReviews;
        std::map<std::string, Product*> pNametoP; //makes a map of the products full name to its object pointer
        std::string loggedUser;
        std::vector<std::pair<std::string, double> > makeSuggestion(std::string currentUser);
        std::map<User*, double> getBasicSimGraph_(std::string &usR);
                


        /*** Creates and adds a review with the given information   ***/
        virtual void addReview(const std::string& prodName,
			   int rating,
			   const std::string& username,
			   const std::string& date,
			   const std::string& review_text);

        std::string AvgRating(std::string productName);

        bool checkdate(std::string &in); // used to check if date provided is valid

};
#endif
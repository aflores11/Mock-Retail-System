#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>
#include <set>
#include "product.h"
#include <map>

/**
 * Implements User functionality and information storage
 *  You should not need to derive anything from User at this time
 */
class User {
public:
    User();
    User(std::string name, double balance, int type, unsigned long long password);
    virtual ~User();

    double getBalance() const;
    bool verifypass(unsigned long long &in);
    std::string getName() const;
    void deductAmount(double amt);
    std::set<Product*> getRevProds() const; // used to calculate intersection of products two users have reviewed
    int specificProdRating(Product* in); //returns the rating of a specefic product curr user has rated
    void insertProdRev(Product* newreview, int rating);//inserts new product into set of all products user has
    virtual void dump(std::ostream& os);
    void updateRefineSim(std::map<std::string, double> &in);
    std::map<std::string, double> getRefSim();
    


private:
    std::string name_;
    double balance_;
    int type_;
    unsigned long long password_; 
    std::set<Product*> reviewedItems; //contains all the products current user has rated
    std::map<Product*, int> prodratings;//gives you the rating the user gave to a specific product 
    std::map<std::string, double> refinedSim_; //stores refined similarities to other users
};
#endif

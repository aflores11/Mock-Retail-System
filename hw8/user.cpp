#include "user.h"
#include "myhash.h"
using namespace std;

User::User() : name_("unknown"), balance_(0.0), type_(1), password_(0)
{

}
User::User(std::string name, double balance, int type, unsigned long long password) :
    name_(name), balance_(balance), type_(type), password_(password)
{
}

User::~User()
{

}

bool User::verifypass(unsigned long long &in)
{
    if(in == this->password_) return true;
    return false;
}

std::string User::getName() const
{
    return name_;
}

double User::getBalance() const
{
    return balance_;
}

void User::deductAmount(double amt)
{
    balance_ -= amt;
}

void User::dump(std::ostream& os)
{
    os << name_ << " "  << balance_ << " " << type_ << " " << password_ << endl;
}

std::set<Product*> User::getRevProds() const
{
    return this->reviewedItems;
}

void User::insertProdRev(Product* newreview, int rating)
{
    this->reviewedItems.insert(newreview);
    this->prodratings[newreview] = rating;
}

int User::specificProdRating(Product* in)
{
    return prodratings[in];
}

void User::updateRefineSim(std::map<std::string, double> &in)
{
    this->refinedSim_ = in;
}

std::map<std::string, double> User::getRefSim()
{
    return this->refinedSim_;
}
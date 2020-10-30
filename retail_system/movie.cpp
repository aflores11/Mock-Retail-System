#include "product.h"
#include "movie.h"
#include <iostream>
#include <string>
#include "util.h"

using namespace std;

/* this function cuts off the extra zeros in a double to string conversion ex: 12.0000000 = 12.00 */
void tostring2(double num, string &in)
{
    in = to_string(num);
    int i = 0;
    while (in[i] != '.')
    {
        if (in[i + 1] == '.')
        {
            in.erase(i + 4, in.length());
            break;
        }
        i++;
    }
}

Movie::Movie(const std::string category, const std::string name, double price, int qty, std::string genre, std::string rating) : Product(category, name, price, qty)
{
    this->Rating_ = "0.00"; // this one is how good the movie is
    this->Rating = rating; // this one is the discretion ex: G, R, PG-13
    this->Genre = genre;
    this->moviekeywordset = parseStringToWords(name);
    this->moviekeywordset.insert(convToLower(genre)); //its genre needs to be its own keyword 
    this->moviekeywordset.insert(convToLower(rating)); // rating its own keyword
}

Movie::~Movie()
{
}

std::set<std::string> Movie::keywords() const
{
    return this->moviekeywordset;
}

bool Movie::isMatch(vector<string> &searchTerms) const
{
    return true;
}

std::string Movie::displayString() const
{
    string Movieinfo;

    string prc;
    tostring2(getPrice(), prc);

    Movieinfo = getName() + "\n" + "Genre: " + Genre + " " +
                "Rating: " +
                Rating + "\n" + prc + " " + to_string(getQty()) + " " + "left.\n"
                + "Rating: " + this->Rating_ + "\n" ;

    return Movieinfo;
}

void Movie::dump(ostream &os) const
{
    os << category_ + "\n";
    os << getName()+ "\n";
    string prc;
    tostring2(getPrice(), prc); //gets rid of extra trailing zeros in to_string(double)
    os << prc + "\n";
    os << to_string(getQty()) + "\n";
    os << this->Genre + "\n";
    os << this->Rating + "\n"; 
}

double Movie::getPrice() const
{
    return price_;
}
std::string Movie::getName() const
{
    return name_;
}
int Movie::getQty() const
{
    return qty_;
}
void Movie::subtractQty(int num)
{
    qty_ = qty_ - num;
}

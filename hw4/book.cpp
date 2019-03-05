#include "product.h"
#include "book.h"
#include <iostream>
#include <string>
#include "util.h"

using namespace std;

/* this function cuts off the extra zeros in a double to string conversion ex: 12.0000000 = 12.00 */
void tostring3(double num, string &in)
{
    in = to_string(num);
    int i = 0;
    //finds the first period;
    while (in[i] != '.')
    {
        if (in[i + 1] == '.')
        {
            in.erase(i + 4, in.length()); //makes the string be up two two decimal points only
            break;
        }
        i++;
    }
}

Book::Book(const std::string category, const std::string name, double price, int qty, std::string isbn, std::string author) : Product(category, name, price, qty)
{
    this->isbn = isbn;
    this->author = author;
    set<string> temp = parseStringToWords(name); //attaches name to keywords of this specific book
    this->bookkeywordset = setUnion(this->bookkeywordset, temp);
    temp = parseStringToWords(author); //attaches author to keywords of this specific book
    this->bookkeywordset = setUnion(this->bookkeywordset, temp) ;
    this->bookkeywordset.insert(isbn); //attaches isbn# to keywords of this specific book

}

Book::~Book()
{
}

std::set<std::string> Book::keywords() const
{
    return this->bookkeywordset;
}

bool Book::isMatch(vector<string> &searchTerms) const
{
    return true;
}

std::string Book::displayString() const
{
    string bookinfo;
    string prc; //used to cut off extra decimals
    tostring3(getPrice(), prc);

    bookinfo = getName() + "\n" + "Author: " + author + " "
                                                        " ISBN: " +
               isbn + "\n" + prc + " " + to_string(getQty()) + " " + "left\n";

    return bookinfo;
}

void Book::dump(ostream &os) const
{
    os << category_ + "\n";
    os << getName()+ "\n";
    string prc;
    tostring3(getPrice(), prc); //gets rid of extra trailing zeros in to_string(double)
    os << prc + "\n";
    os << to_string(getQty()) + "\n";
    os << this->isbn + "\n";
    os << this->author + "\n"; 
}

double Book::getPrice() const
{
    return price_;
}
std::string Book::getName() const
{
    return name_;
}
int Book::getQty() const
{
    return qty_;
}
void Book::subtractQty(int num)
{
    qty_ = qty_ - num;
}

#include "product.h"
#include "clothing.h"
#include <iostream>
#include <string>
#include "util.h"

using namespace std;

/* this function cuts off the extra zeros in a double to string conversion ex: 12.0000000 = 12.00 */
void tostring1(double num, string &in)
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

Clothing::Clothing(const std::string category, const std::string name, double price, int qty, std::string size, std::string brand) : Product(category, name, price, qty)
{
    this->Brand = brand;
    this->Size = size;
    this->clothingkeywordset = parseStringToWords(name); //adds name to keywords
    set<string> temp = parseStringToWords(brand); //adds brand to keywords
    this->clothingkeywordset = setUnion(this->clothingkeywordset, temp) ;
    this->clothingkeywordset.insert(convToLower(size));
}

Clothing::~Clothing()
{
}

std::set<std::string> Clothing::keywords() const
{
    return this->clothingkeywordset;
}

bool Clothing::isMatch(vector<string> &searchTerms) const
{
    return true;
}

std::string Clothing::displayString() const
{
    string Clothinginfo;

    string prc; //used to trim string from double to string conversion
    tostring1(getPrice(), prc);

    Clothinginfo = getName() + "\n" + "Size: " + Size + " " +
                   "Brand: " +
                   Brand + "\n" + prc + " " + to_string(getQty()) + " " + "left.\n";

    return Clothinginfo;
}

void Clothing::dump(ostream &os) const
{
    os << category_ + "\n";
    os << getName()+ "\n";
    string prc;
    tostring1(getPrice(), prc); //gets rid of extra trailing zeros in to_string(double)
    os << prc + "\n";
    os << to_string(getQty()) + "\n";
    os << this->Size + "\n";
    os << this->Brand + "\n"; 
}

double Clothing::getPrice() const
{
    return price_;
}
std::string Clothing::getName() const
{
    return name_;
}
int Clothing::getQty() const
{
    return qty_;
}
void Clothing::subtractQty(int num)
{
    qty_ = qty_ - num;
}

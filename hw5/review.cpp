#include <iostream>
#include "review.h"

using namespace std;

void Review::dump(ostream &os, const string &in)
{
    os << in + "\n";
    os << this->rating << " " << this->username << " " << this->date << " " << this->reviewText << "\n";
}
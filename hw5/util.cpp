#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"
#include <string>

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** splits the word at the punctuation and inserts a valid keyword into the set **/
void punctuation_remover(const std::string &in, std::set<std::string> &myset)
{
    int count = 0;
    string temp = in;
    string side1, side2;

    int i = 0;

    //iterators through string to find first punctuation mark
    while (!ispunct(temp[i]) && i < (int)temp.length())
    {
        i++;
    }

    //means no punctuation was found
    if (i >= (int)temp.length())
    {
        side1 = temp.substr(0, i);
    }
    else // splits the string into two from the punctuation sign
    {
        side1 = temp.substr(0, i);
        side2 = temp.substr(i + 1, temp.length() - 1 - i);
    }

    //check if the new substrings are valid
    if (side1.length() > 1)
    {
        myset.insert(side1);
        count++;
    }

    if (side2.length() > 1)
    {
        myset.insert(side2);
        count++;
    }

    return;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWords)
{

    set<string> words;
    string tmpkey = trim(rawWords); //incase there are any leading line spaces
    while (!tmpkey.empty())
    {
        stringstream mystream(tmpkey);
        string keyword;
        getline(mystream, keyword, ' '); //gets the first word in the string separated by space
        keyword = convToLower(keyword); 
        punctuation_remover(keyword, words); //checks to see if it can be split into sub strings and inserts into set
        tmpkey.erase(0, keyword.size()); 
        tmpkey = trim(tmpkey);//gets rid of trailing whitespace left from erasing previous word
    }

    return words;
}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s)
{
    s.erase(s.begin(),
            std::find_if(s.begin(),
                         s.end(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s)
{
    s.erase(
        std::find_if(s.rbegin(),
                     s.rend(),
                     std::not1(std::ptr_fun<int, int>(std::isspace)))
            .base(),
        s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s)
{
    return ltrim(rtrim(s));
}

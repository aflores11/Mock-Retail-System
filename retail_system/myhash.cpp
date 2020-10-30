#include "myhash.h"
#include <iostream>


typedef unsigned long long myvalue;
#define A 65521;

myvalue hashpass(std::string passwrd, int index)
{   
    if(index == 0) return (int)passwrd[0];
    return (int)passwrd[index] + 128*hashpass(passwrd, index-1);
}

unsigned long long encryptpassword(std::string& in)
{
    unsigned int w[4];
    myvalue data = hashpass(in, in.length() -1 );
    w[3] = data%A;
    data = data/A;
    w[2] = data%A;
    data = data/A;
    w[1] =data%A;
    data = data/A;
    w[0] = data%A;
    unsigned long long myhash = (45912*w[0] + 35511*w[1] + 65169*w[2]+ 4625*w[3]) % 65521;
    return myhash;
}

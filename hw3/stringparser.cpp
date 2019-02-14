#include <iostream>
#include "stackstring.h"
#include <string>
#include <fstream>
#include <sstream>
#include "ulliststr.h"

using namespace std;

void simplify(string &in, const string &c);
void simplify(const string &in1, const string &in2, string &out, const string &c);
bool check_if_word(const string &in);
void check_if_empty(StackString &in, ofstream &putout);

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        cout << "ERROR" << endl;
        return 0;
    }

    ifstream infile(argv[1]);
    if (infile.fail())
    {
        cout << "ERROR" << endl;
        return 0;
    }

    string expression;
    getline(infile, expression);
    if (infile.fail())
    {
        cout << "ERROR" << endl;
        return 0;
    }

    ofstream products;
    products.open(argv[2]);

    if (expression.empty())
    {
        products << "MALFORMED\n";
        getline(infile, expression);
    }

    while (!infile.fail())
    {
        StackString main_stack;
        if (expression.empty())
        {
            products << "MALFORMED\n";
            getline(infile, expression);
            continue;
        }
        stringstream ParseStr(expression);

        char tempchar;
        string finalword;
        ParseStr >> tempchar;

        while (!ParseStr.fail())
        {
            if (!islower(tempchar))
            {
                if (tempchar == '<' || tempchar == '>' || tempchar == '+' || tempchar == '(' || tempchar == '-')
                {
                    main_stack.push(string(1, tempchar));
                    ParseStr >> tempchar;
                    continue;
                }
                if (tempchar == ')')
                {
                    int count = 0;
                    string prevoperator;
                    main_stack.push(string(1, tempchar));
                    main_stack.pop();
                    while (main_stack.top() != "(")
                    {
                        //checks for if list is empty and closing parenthesis is not found
                        check_if_empty(main_stack, products);

                        string word2 = main_stack.top();
                        main_stack.pop();
                        if (!check_if_word(word2)) //want the first string to be a word
                        {
                            products << "MALFORMED" << endl;
                            break;
                        }

                        check_if_empty(main_stack, products);
                        string operator_ = main_stack.top();
                        if (count == 0)
                        {
                            prevoperator = operator_;
                        }
                        // take care of (<> word ) cases
                        if (operator_ == "(")
                        {
                            main_stack.pop();
                            main_stack.push(word2);
                            break;
                        }

                        if (operator_ == "<" || operator_ == ">")
                        {
                            while (main_stack.top() == "<" || main_stack.top() == ">")
                            {

                                simplify(word2, main_stack.top());
                                main_stack.pop();

                                //so that it wont throw exception if <> are the last strings on stack and won't try to access an empty list when calling top()
                                if (main_stack.empty())
                                {
                                    break;
                                }
                            }
                            operator_ = main_stack.top();
                            if (count == 0)
                            {
                                prevoperator = operator_;
                            }
                        }
                        main_stack.pop();
                        if (check_if_word(operator_)) //we want the operator to actually be a + or -
                        {
                            products << "MALFORMED\n";
                            break;
                        }

                        if (operator_ == "+")
                        {
                            check_if_empty(main_stack, products);
                            string word1 = main_stack.top();
                            main_stack.pop();
                            if (!check_if_word(word2)) //want string after operator to be a word
                            {
                                products << "MALFORMED\n";
                                break;
                            }
                            simplify(word1, word2, finalword, operator_);
                            check_if_empty(main_stack, products);
                            if (main_stack.top() == "(")
                            {
                                main_stack.pop();
                                main_stack.push(finalword);
                                break;
                            }
                            else
                            {
                                main_stack.push(finalword);
                                count++;
                                continue;
                            }
                        }

                        if (operator_ == "-")
                        {
                            if (prevoperator == "+") // checks if it is mixed with different operator
                            {
                                products << "MALFORMED\n";
                                break;
                            }
                            check_if_empty(main_stack, products);
                            string word1 = main_stack.top();
                            main_stack.pop();
                            if (!check_if_word(word2)) //want string after operator to be a word
                            {
                                products << "MALFORMED\n";
                                break;
                            }
                            check_if_empty(main_stack, products);
                            if (main_stack.top() == "<" || main_stack.top() == ">")
                            {
                                while (main_stack.top() == "<" || main_stack.top() == ">")
                                {

                                    simplify(finalword, main_stack.top());
                                    main_stack.pop();

                                    //so that it wont throw exception if <> are the last strings on stack and won't try to access an empty list when calling top()
                                    if (main_stack.empty())
                                    {
                                        break;
                                    }
                                }
                            }
                            if (main_stack.top() == "(")
                            {
                                simplify(word1, word2, finalword, operator_);
                                main_stack.pop();
                                main_stack.push(finalword);
                                break;
                            }
                            else
                            {
                                products << "MALFORMED\n";
                                break;
                            }
                        }
                    }
                    ParseStr >> tempchar;
                    continue;
                }

                else
                {
                    products << "MALFORMED\n";
                    break;
                }
            }

            else
            {
                finalword.clear();
                while (islower(tempchar) && !ParseStr.fail())
                {
                    finalword += tempchar;
                    ParseStr >> tempchar;
                }

                while (main_stack.top() == "<" || main_stack.top() == ">")
                {

                    simplify(finalword, main_stack.top());
                    main_stack.pop();

                    //so that it wont throw exception if <> are the last strings on stack and won't try to access an empty list when calling top()
                    if (main_stack.empty())
                    {
                        break;
                    }
                }
                main_stack.push(finalword);
            }
        }

        if (main_stack.size() != 1)
        {
            finalword = main_stack.top();
            main_stack.pop();
            if (!check_if_word(finalword))
            {
                products << "MALFORMED\n";
                break;
            }
            else
            {
                if (main_stack.top() == "<" || main_stack.top() == ">")
                {
                    while (main_stack.top() == "<" || main_stack.top() == ">")
                    {

                        simplify(finalword, main_stack.top());
                        main_stack.pop();

                        //so that it wont throw exception if <> are the last strings on stack and won't try to access an empty list when calling top()
                        if (main_stack.empty())
                        {
                            break;
                        }
                    }
                    main_stack.push(finalword);
                }
                else
                {
                    products << "MALFORMED\n";
                    break;
                }
            }
        }

        if (main_stack.size() != 1)
        {
            products << "MALFORMED\n";
            break;
        }
        else
        {
            products << main_stack.top() << "\n";
            main_stack.pop();
        }
        expression.clear();
        getline(infile, expression);
    }

    products.close();
}

/*
based on the input of the character, it does the arithmetic for the < or > characters
*/
void simplify(string &in, const string &c)
{
    if (c == "<")
    {
        in.erase(in.size() - 1, 1);
        return;
    }
    if (c == ">")
    {
        in.erase(0, 1);
        return;
    }
}

/*
overloaded simplify for the cases of + or -
*/
void simplify(const string &in1, const string &in2, string &out, const string &c)
{
    if (c == "+")
    {
        out = in1 + in2;
        return;
    }
    if (c == "-")
    {
        int temp = in1.find(in2, 0);
        if (temp != (int)string::npos)
        {
            out = in1;
            out.erase(temp, in2.size());
        }
        else
        {
            out = in1;
        }

        return;
    }
}

/*
using this function to check if the string passed in is actually a string of words
using it for when I am simplifying expressions
*/
bool check_if_word(const string &in)
{
    if (in == "+" || in == "-" || in == "<" || in == ">")
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*
made a function to check if a stack is empty before calling top so I do not have to constantly type it
*/
void check_if_empty(StackString &in, ofstream &putout)
{
    if (in.empty())
    {
        putout << "MALFORMED\n";
        std::exit(0);
    }
}
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
bool evaluate(StackString &main_stack, stringstream &ParseStr, char &tempchar, string &finalword);
void carrot_simplify(StackString &in, string &finalword);

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

    ofstream ofile;
    ofile.open(argv[2]);

    if (expression.empty())
    {
        ofile << "\n";
        getline(infile, expression);
    }

    while (!infile.fail())
    {
        StackString main_stack;
        if (expression.empty())
        {
            ofile << "\n";
            getline(infile, expression);
            continue;
        }
        stringstream ParseStr(expression);

        char tempchar;
        string finalword;
        //initial parsing; if does not work, the next while loop will not run
        ParseStr >> tempchar;
        bool already_Malformed = false;

//this while continues the parsing
        while (!ParseStr.fail())
        {
            //makes sure that the character is a valid input, if not , malform 
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
                    //if we find a closed paren, we want to start evaluating until we find the first open paren
                    if (!evaluate(main_stack, ParseStr, tempchar, finalword))
                    {
                        ofile << "Malformed\n";
                        already_Malformed = true;
                        break;
                    }
                }

                else
                {
                    ofile << "Malformed\n";
                    already_Malformed = true;
                    break;
                }
            }

            else
            {
                //this while loop groups successive characters that are all letters and makes them into a string to push onto stack
                finalword.clear();
                while (islower(tempchar) && !ParseStr.fail())
                {
                    finalword += tempchar;
                    ParseStr >> tempchar;
                }
                if (!main_stack.empty() && (main_stack.top() == "<" || main_stack.top() == ">"))
                {
                    //evaluates the word if there are <  or > signs before it
                    carrot_simplify(main_stack, finalword);
                }
                else
                {
                    main_stack.push(finalword);
                }
            }
        }

        //serves as a check if rogram through malformed earlier for current expression so that it does not throw more
        if (already_Malformed)
        {
            expression.clear();
            getline(infile, expression);
            continue;
        }

        //this checks for the case where you have < > outside parenthesis e.g. <>(sjd+ashdk)
        if (!main_stack.empty() && main_stack.size() != 1)
        {
            finalword = main_stack.top();
            main_stack.pop();
            if (!check_if_word(finalword))
            {
                ofile << "Malformed\n";
                already_Malformed = true;
            }
            else
            {
                if (!main_stack.empty() && (main_stack.top() == "<" || main_stack.top() == ">"))
                {
                    carrot_simplify(main_stack, finalword);
                }
                else
                {
                    ofile << "Malformed\n";
                    already_Malformed = true;
                }
            }
        }
     //this checks for the case where you have < > outside parenthesis e.g. <>(sjd+ashdk)
        if (already_Malformed)
        {
            expression.clear();
            getline(infile, expression);
            continue;
        }
        //makes sure there is nothing but the final word after evaluating the < > if there were any in the front
        if (main_stack.size() != 1)
        {
            ofile << "Malformed\n";
        }
        else
        {
            ofile << main_stack.top() << "\n";
            main_stack.pop();
        }
        expression.clear();
        getline(infile, expression);
    }

    ofile.close();
}

/*
point of this function is to continue evaluating a word until there are no carrots left
*/
void carrot_simplify(StackString &in, string &finalword)
{
    while (in.top() == "<" || in.top() == ">")
    {

        simplify(finalword, in.top());
        in.pop();
        //so that it wont throw exception if <> are the last strings on stack and won't try to access an empty list when calling top()
        if (in.empty())
            break;
    }
    in.push(finalword);
    return;
}

/*
based on the input of the character, it does the arithmetic for the < or > characters
*/
void simplify(string &in, const string &c)
{
    if (in.empty())
        return;
    if (c == "<")
    {
        in.erase(in.length() - 1, 1);
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
            out.erase(temp, in2.length());
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
starts evaluating after finding ')' 
*/
bool evaluate(StackString &main_stack, stringstream &ParseStr, char &tempchar, string &finalword)
{
    int count = 0;
    string prevoperator;
    main_stack.push(string(1, tempchar)); //pushes ) to stack to then pop it
    main_stack.pop();
    if (main_stack.empty())
        return false;

    while (main_stack.top() != "(")
    {
        //checks for if list is empty and closing parenthesis is not found
        if (main_stack.empty())
            return false;

        string word2 = main_stack.top();
        main_stack.pop();
        if (!check_if_word(word2)) //want the first string to be a word
        {
            return false;
        }

        if (main_stack.empty())
            return false;
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
            carrot_simplify(main_stack, finalword);
            if (main_stack.empty())
                return false;
            operator_ = main_stack.top();
            if (count == 0)
            {
                prevoperator = operator_;
            }
        }
        main_stack.pop();
        if (check_if_word(operator_)) //we want the operator to actually be a + or -
        {
            return false;
        }

        if (operator_ == "+")
        {
            if (main_stack.empty())
                return false;
            string word1 = main_stack.top();
            main_stack.pop();
            if (!check_if_word(word2)) //want string after operator to be a word
            {
                return false;
            }
            simplify(word1, word2, finalword, operator_);
            if (!main_stack.empty() && main_stack.top() == "(")
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
                return false;
            }
            if (main_stack.empty())
                return false;
            string word1 = main_stack.top();
            main_stack.pop();
            if (!check_if_word(word1)) //want string after operator to be a word
            {
                return false;
            }

            //checks the cases for when we have carrots in between to open parenthesis
            if (!main_stack.empty() && (main_stack.top() == "<" || main_stack.top() == ">"))
            {
                while (main_stack.top() == "<" || main_stack.top() == ">")
                {

                    simplify(word1, main_stack.top());
                    main_stack.pop();
                    //so that it wont throw exception if <> are the last strings on stack and won't try to access an empty list when calling top()
                    if (main_stack.empty())
                        break;
                }
            }

            if (!main_stack.empty() && main_stack.top() == "(")
            {
                simplify(word1, word2, finalword, operator_);
                main_stack.pop();
                main_stack.push(finalword);
                break;
            }
            else
            {
                return false;
            }
        }
    }
    ParseStr >> tempchar;
    return true;
}
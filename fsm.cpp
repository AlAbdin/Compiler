#include <iostream>
#include <cctype>
#include <string.h>
#include <vector>
#include <algorithm>
#include <fstream>


bool isOp(char c){
    return c == '*' or c == '+' or c == '-' or c == '/' or c == '<' or c == '>' or c == '=' or c == '%';
}


bool isSpecial(char c){
    return c == '$' or c == '_';
}

// Keywords to compare the strings to
bool isKeyword(std::string buffer){
    std::vector<std::string> keywords = {"int", "float", "bool", "true", "false",
                               "if", "else", "then", "endif", "while",
                               "whileend", "do", "doend", "for", "forend",
                               "input", "output", "and", "or", "not"};

    return find(keywords.begin(), keywords.end(), buffer) != keywords.end();
}

bool isdecimal(char c){
    return c == '.';
}

bool isSeparator(char c){
    return  c == '(' or c == ')' or c == '{' or c == '}' or c == '[' or c == ']' or c == ',' or c == ':' or c == ';';
}

bool iscomt(char c){
    return c == '!';
}

// start of fsm function
void fsm(std::string s, std::ofstream &outfile)
{
                        //0,L  1,d   2,op   3,dec  4,sep  5,spl  6,sp  7,!
    int table[11][8] = { {1,    3,     8,    7,     7,    2,     0,   9}, //0, Initial state.
                         {1,    1,     2,    2,     2,    1,     2,   2}, //1, Indentifier writing
                         {0,    0,     0,    0,     0,    0,     0,   0}, //2, End of Identifier
                         {4,    3,     4,    5,     4,    4,     4,   4}, //3, Number
                         {0,    0,     0,    0,     0,    0,     0,   0}, //4, End of Real number
                         {6,    5,     6,    6,     6,    6,     6,   6}, //5, Float Number
                         {0,    0,     0,    0,     0,    0,     0,   0}, //6, End Float Number
                         {0,    0,     0,    0,     0,    0,     0,   0}, //7, End Separator
                         {0,    0,     0,    0,     0,    0,     0,   0}, //8, End Op
                         {9,    9,     9,    9,     9,    9,     9,  10}, //9, Begin Comment
                         {0,    0,     0,    0,     0,    0,     0,   0}  //10,End of Comment
                        };

    int state = 0, col;
    std::string buffer;
    s.push_back(' ');

    for (size_t i = 0; i < s.size(); ++i)
    {
        char c = s.at(i);
        
        if (isalpha(c))
            col = 0;
        else if (isdigit(c))
            col = 1;
        else if (isOp(c))
            col = 2;
        else if (isdecimal(c))
            col = 3;
        else if (isSeparator(c))
            col = 4;
        else if (isSpecial(c))
            col = 5;
        else if (iscomt(c))
            col = 7;
        else if (isspace(c))
            col = 6;

        state = table[state][col];
        buffer.push_back(c);
        if(state == 0)
            buffer.clear();

        if (state == 2){ // State = varaible
                buffer.pop_back();
                if (isKeyword(buffer))
                    outfile << "Keyword\t\t\t";
                else
                    outfile << "Identifier\t\t";

                outfile << buffer << std::endl;
                buffer.clear();
                state = 0;
                --i;
            }
        else if (state == 4){ // State = real number
            buffer.pop_back();
            outfile << "Real\t\t\t" << buffer << std::endl;
            buffer.clear();
            state = 0;
            --i;
        }
        else if (state == 6){ // State = float
            buffer.pop_back();
            outfile << "Float\t\t\t" << buffer << std::endl;
            buffer.clear();
            state = 0;
            --i;

        }
        else if ( state == 7){ //State = Separator
            
            outfile << "Separator\t\t" << buffer << std::endl;
            buffer.clear();
            state = 0;
        
        }
        else if( state == 8){ //State = Operator
            
            outfile << "Operator\t\t" << buffer << std::endl;
            buffer.clear();
            state = 0;
            
        }
        else if (state == 10){ // State = Comment
            buffer.clear();
            state = 0;
        }
    }
}

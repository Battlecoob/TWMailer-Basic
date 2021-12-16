#include "functions.hpp"

bool SecureInput(std::string str)
{
    for (int c = 0; c < (int)str.size()-1; c++)
    {
        if(!isdigit(str[c]) && !isalpha(str[c]))
            return false;
    }
    return true;
}
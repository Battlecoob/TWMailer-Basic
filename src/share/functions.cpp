#include "functions.hpp"

bool secureInput(std::string str)
{
    for (int c = 0; c < (int)str.size(); c++)
    {
        if(!isdigit(str[c]) && !isalpha(str[c]))
            return false;
    }
    return true;
}
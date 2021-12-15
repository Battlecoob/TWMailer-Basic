#include "include/database_class.hpp"

const User Database::GetUserByName()
{
    return _users[0]; // FALSCH, nur dass die Warnings wegsind
}
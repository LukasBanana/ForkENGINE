/*
 * FSC scope file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/FSCInterpreter/FSCScope.h"


namespace Fork
{

namespace Lang
{


FSCScope::FSCScope()
{
}
FSCScope::~FSCScope()
{
}

bool FSCScope::Add(const std::string& varName, const VarObjectPtr& varObject)
{
    /* Check if variable was already declared */
    auto it = nameTable_.find(varName);

    if (it != nameTable_.end())
        return false;

    /* Add new variable */
    nameTable_[varName] = varObject;

    return true;
}

bool FSCScope::Fetch(const std::string& varName, VarObjectPtr& varObject) const
{
    /* Try to find variable in this scope */
    auto it = nameTable_.find(varName);

    if (it != nameTable_.end())
    {
        varObject = it->second;
        return true;
    }

    return false;
}


} // /namespace Lang

} // /namespace Fork



// ========================
/*
 * FSC scope manager file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/FSCInterpreter/FSCScopeManager.h"


namespace Fork
{

namespace Lang
{


FSCScopeManager::FSCScopeManager()
{
}
FSCScopeManager::~FSCScopeManager()
{
}

void FSCScopeManager::PushScope()
{
    scopes_.push_back(std::make_shared<FSCScope>());
}

void FSCScopeManager::PopScope()
{
    scopes_.pop_back();
}

FSCScopePtr FSCScopeManager::TopScope() const
{
    return scopes_.empty() ? nullptr : scopes_.back();
}

void FSCScopeManager::Add(const std::string& varName, const VarObjectPtr& varObject)
{
    if (scopes_.empty())
        throw std::string("There is no scope to add variable");
    if (!scopes_.back()->Add(varName, varObject))
        throw std::string("Variable \"" + varName + "\" was already declared in this scope");
}

VarObjectPtr FSCScopeManager::Fetch(const std::string& varName) const
{
    if (scopes_.empty())
        throw std::string("There is no scope to fetch variable");
        
    /* Search for variable in all scopes in bottom-up order */
    VarObjectPtr varObject;

    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it)
    {
        if ((*it)->Fetch(varName, varObject))
            return varObject;
    }

    /* No variable found in any scope -> throw an error message */
    throw std::string("Variable \"" + varName + "\" was not declared in this scope");

    return nullptr;
}


} // /namespace Lang

} // /namespace Fork



// ========================
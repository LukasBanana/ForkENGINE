/*
 * FSC scope header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LANG_FSC_SCOPE_H__
#define __FORK_LANG_FSC_SCOPE_H__


#include "Lang/FSCInterpreter/VarObject.h"

#include <string>
#include <map>


namespace Fork
{

namespace Lang
{


class FORK_EXPORT FSCScope
{
    
    public:
        
        FSCScope();
        ~FSCScope();

        bool Add(const std::string& varName, const VarObjectPtr& varObject);

        bool Fetch(const std::string& varName, VarObjectPtr& varObject) const;

    private:
        
        std::map<std::string, VarObjectPtr> nameTable_;

};

typedef std::shared_ptr<FSCScope> FSCScopePtr;


} // /namespace Lang

} // /namespace Fork


#endif



// ========================
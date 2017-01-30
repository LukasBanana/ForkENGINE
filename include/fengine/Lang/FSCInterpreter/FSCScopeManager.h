/*
 * FSC scope manager header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LANG_FSC_SCOPE_MANAGER_H__
#define __FORK_LANG_FSC_SCOPE_MANAGER_H__


#include "Lang/FSCInterpreter/FSCScope.h"

#include <vector>


namespace Fork
{

namespace Lang
{


/**
The FSC scope manager allowed only simple scopes but not any kind of namespaces.
\ingroup lang_forkscript
*/
class FORK_EXPORT FSCScopeManager
{
    
    public:
        
        FSCScopeManager();
        ~FSCScopeManager();

        void PushScope();
        void PopScope();

        FSCScopePtr TopScope() const;

        /**
        Adds the specified variable object to the top level scope.
        \throws std::string if the variable is already declared in the top level scope.
        */
        void Add(const std::string& varName, const VarObjectPtr& varObject);

        /**
        Fetches the specified variable object.
        \return Shared pointer to the variable object or a null pointer if the object has currently no specific type.
        \throws std::string if the variable is not declared in the current (and upper) scopes.
        */
        VarObjectPtr Fetch(const std::string& varName) const;

        //! Returns the current depth of the scope levels.
        inline size_t ScopeDepth() const
        {
            return scopes_.size();
        }

    private:
        
        std::vector<FSCScopePtr> scopes_;

};


} // /namespace Lang

} // /namespace Fork


#endif



// ========================
/*
 * Script language variable object header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LANG_VAROBJECT_H__
#define __FORK_LANG_VAROBJECT_H__


#include "Core/Export.h"

#include <memory>


namespace Fork
{

namespace Lang
{


/**
Base class for all ForkSCript variable objects.
\ingroup lang_forkscript
*/
class FORK_EXPORT VarObject
{
    
    public:
        
        //! Variable object types.
        enum class Types
        {
            Integer,
            Float,
            String,
            List,
            Resource,
        };

        virtual ~VarObject()
        {
        }

        //! Returns the variable object's type.
        inline Types Type() const
        {
            return type_;
        }

    protected:
        
        VarObject(const Types type) :
            type_(type)
        {
        }

    private:
        
        Types type_;

};

typedef std::shared_ptr<VarObject> VarObjectPtr;


} // /namespace Lang

} // /namespace Fork


#endif



// ========================
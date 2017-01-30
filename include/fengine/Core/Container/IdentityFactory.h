/*
 * Identity facotr header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IDENTITY_FACTORY_H__
#define __FORK_IDENTITY_FACTORY_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Core/Container/Identifiable.h"

#include <vector>


namespace Fork
{


DECL_SHR_PTR(IdentityFactory);

/**
This class will generate uniquely identity numbers.
The minimal ID number will be 1 and 0 is the only invalid ID number.
*/
class FORK_EXPORT IdentityFactory
{
    
    public:
        
        IdentityFactory() = default;

        IdentityFactory(const IdentityFactory&) = delete;
        IdentityFactory& operator = (const IdentityFactory&) = delete;

    protected:
        
        friend class Identifiable;

        //! Generates a new ID.
        Identifiable::IDType GenerateID();
        //! Releases the specified ID.
        void ReleaseID(const Identifiable::IDType& id);

    private:
        
        //! Container for all ID numbers.
        std::vector<Identifiable::IDType> idList_;

};


} // /namespace Fork


#endif



// ========================
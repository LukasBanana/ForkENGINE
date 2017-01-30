/*
 * Identifiable file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Core/Container/Identifiable.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Container/IdentityFactory.h"


namespace Fork
{


Identifiable::Identifiable(const std::shared_ptr<IdentityFactory>& idFactory) :
    idFactory_{ idFactory }
{
    /* Generate and store new ID */
    ASSERT_POINTER(idFactory);
    id_ = idFactory->GenerateID();
}
Identifiable::~Identifiable()
{
    /* Release ID if factory still exists */
    auto idFactory = idFactory_.lock();
    if (idFactory)
        idFactory->ReleaseID(GetID());
}


} // /namespace Fork



// ========================
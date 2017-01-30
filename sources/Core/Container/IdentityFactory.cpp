/*
 * Identity facotr file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Core/Container/IdentityFactory.h"
#include "Core/STLHelper.h"


namespace Fork
{


Identifiable::IDType IdentityFactory::GenerateID()
{
    /* Search unused ID number in the list */
    Identifiable::IDType id = 1;

    if (!idList_.empty())
    {
        for (auto it = idList_.begin(); it != idList_.end(); ++it)
        {
            /* Find first free entry */
            if (id != *it)
            {
                /*
                Insert new ID exactly at this point to
                keep the list sorted in ascending order
                */
                idList_.insert(it, id);
                return id;
            }

            /* Try next ID */
            ++id;
        }
    }

    /*
    Add entry to the back of the list if:
    - this is the first entry or,
    - all other ID numbers are already used
    */
    idList_.push_back(id);

    return id;
}

void IdentityFactory::ReleaseID(const Identifiable::IDType& id)
{
    RemoveFromList(idList_, id);
}


} // /namespace Fork



// ========================
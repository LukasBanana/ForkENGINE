/*
 * Identifiable header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IDENTIFIABLE_H__
#define __FORK_IDENTIFIABLE_H__


#include "Core/Export.h"

#include <memory>


namespace Fork
{


class IdentityFactory;

//! Base class for all uniquely identifiable objects.
class FORK_EXPORT Identifiable
{
    
    public:
        
        //! Data type for the IDs: 32-bit unsigned integer.
        typedef unsigned int IDType;

        //! Constant for invalid IDs. This is zero (0).
        static const IDType invalidID = 0;

        Identifiable(const Identifiable&) = delete;
        Identifiable& operator = (const Identifiable&) = delete;

        /**
        Constructs the identifiable object and initializes the ID number.
        \param[in] idFactory Shared pointer to the ID factory.
        Only a weak pointer will be stored for this identifiable object to
        release the ID from the factory when this object is destroyed.
        \throws NullPointerException If 'idFactory' is null.
        */
        Identifiable(const std::shared_ptr<IdentityFactory>& idFactory);
        virtual ~Identifiable();

        //! Returns the ID number.
        inline const Identifiable::IDType& GetID() const
        {
            return id_;
        }

        /**
        Returns true if this is a valid ID number.
        This is the case when the ID is unequal to IdentityFactory::invalidID.
        \see GetID
        \see IdentityFactory::invalidID
        */
        inline bool IsValid() const
        {
            return id_ != Identifiable::invalidID;
        }

    private:
        
        //! Uniqueley identification number. By default Identifiable::invalidID.
        Identifiable::IDType id_ = Identifiable::invalidID;

        /**
        Weak pointer to the identity factory.
        Only required to release the ID when this identifiable object is destroyed.
        */
        std::weak_ptr<IdentityFactory> idFactory_;

};


} // /namespace Fork


#endif



// ========================
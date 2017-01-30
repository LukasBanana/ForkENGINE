/*
 * Component exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COMPONENT_EXCEPTION_H__
#define __FORK_COMPONENT_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Component exception.
\ingroup fork_exception
*/
class FORK_EXPORT ComponentException : public DefaultException
{
    
    public:
        
        //! Component failures enumeration.
        enum class Failures
        {
            __Unknown__,            //!< Unknown component failure.
            ComponentAlreadyExists, //!< A new component type already exists for a game object.
            ComponentIncompatible,  //!< A new component is incompatible with all previously created components.
        };

        ComponentException(const std::string& procName, const Failures failure);

        static std::string FailureToString(const Failures failure);

        //! Returns the component failure type.
        inline Failures GetFailure() const
        {
            return failure_;
        }

    private:
        
        Failures failure_ = Failures::__Unknown__;

};


} // /namespace Fork


#endif



// ========================
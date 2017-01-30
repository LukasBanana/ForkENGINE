/*
 * Encryption exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENCRYPTION_EXCEPTION_H__
#define __FORK_ENCRYPTION_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Encryption exception.
\ingroup fork_exception
*/
class FORK_EXPORT EncryptionException : public DefaultException
{
    
    public:
        
        EncryptionException(
            const std::string& encryptionMethod, const std::string& errorDesc) :
                DefaultException
                {
                    "EncryptionException",
                    "Encryption: " + encryptionMethod + ", Description: " + errorDesc
                }
        {
        }

};


} // /namespace Fork


#endif



// ========================
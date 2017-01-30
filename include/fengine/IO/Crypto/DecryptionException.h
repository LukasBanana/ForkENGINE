/*
 * Decryption exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DECRYPTION_EXCEPTION_H__
#define __FORK_DECRYPTION_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Decryption exception.
\ingroup fork_exception
*/
class FORK_EXPORT DecryptionException : public DefaultException
{
    
    public:
        
        DecryptionException(
            const std::string& decryptionMethod, const std::string& errorDesc) :
                DefaultException
                {
                    "DecryptionException",
                    "Encryption: " + decryptionMethod + ", Description: " + errorDesc
                }
        {
        }

};


} // /namespace Fork


#endif



// ========================
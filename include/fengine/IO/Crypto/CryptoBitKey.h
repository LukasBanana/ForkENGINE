/*
 * Crpyto bit key header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_CRYPTO_BIT_KEY_H__
#define __FORK_IO_CRYPTO_BIT_KEY_H__


#include "IO/Crypto/CryptoKey.h"

#include <vector>
#include <string>


namespace Fork
{

namespace IO
{


/**
Cryptographic bit key implementation.
This cryptographic key implements a simple bit wise XOR encryption and descryption,
similiar to the "one-time-pad".
\note This has a low security encryption!
*/
class FORK_EXPORT CryptoBitKey : public CryptoKey
{
    
    public:
        
        CryptoBitKey() = default;
        //! \see code
        CryptoBitKey(const std::string& keyCode);

        //! Encodes the data block by XOR encryption. Similiar to the "one-time-pad".
        void Encode(void* data, size_t size) const;
        //! Decodes the data block by XOR encryption. Similiar to the "one-time-pad".
        void Decode(void* data, size_t size) const;
        
        /**
        Generates a random key code.
        \param[in] size Specifies the key code size (in bytes). By default 32.
        */
        void GenerateRandomKey(size_t size = 32);

        //! Key code (used as bit set).
        std::string code;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================
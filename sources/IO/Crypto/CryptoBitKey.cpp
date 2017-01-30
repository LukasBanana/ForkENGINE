/*
 * Crpyto bit key file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Crypto/CryptoBitKey.h"
#include "Core/StringModifier.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "IO/Crypto/DecryptionException.h"
#include "IO/Crypto/EncryptionException.h"

#include <cstdlib>


namespace Fork
{

namespace IO
{


static void BitwiseXOREncryption(char* data, size_t size, const std::string& code)
{
    ASSERT_POINTER(data);

    auto codeBegin  = code.begin();
    auto codeEnd    = code.end();
    auto codeIt     = codeBegin;

    while (size-- > 0)
    {
        /* Encrypt current data fragment (with XOR) */
        *data ^= *codeIt;
        ++data;

        /* Get next code fragment */
        ++codeIt;
        if (codeIt == codeEnd)
            codeIt = codeBegin;
    }
}

CryptoBitKey::CryptoBitKey(const std::string& keyCode) :
    code(keyCode)
{
}

void CryptoBitKey::Encode(void* data, size_t size) const
{
    if (code.empty())
        throw EncryptionException("CryptoBitKey", "Key code is empty");
    BitwiseXOREncryption(reinterpret_cast<char*>(data), size, code);
}

void CryptoBitKey::Decode(void* data, size_t size) const
{
    if (code.empty())
        throw DecryptionException("CryptoBitKey", "Key code is empty");
    BitwiseXOREncryption(reinterpret_cast<char*>(data), size, code);
}

void CryptoBitKey::GenerateRandomKey(size_t size)
{
    /* Simple random function to fill the key code */
    code.resize(size);
    for (auto& frag : code)
        frag = rand() % 256;
}


} // /namespace IO

} // /namespace Fork



// ========================
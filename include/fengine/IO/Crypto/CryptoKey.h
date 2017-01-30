/*
 * Crpyto key interface header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_CRYPTO_KEY_H__
#define __FORK_IO_CRYPTO_KEY_H__


#include "Core/Export.h"


namespace Fork
{

namespace IO
{


//! Cryptographic key interface.
class FORK_EXPORT CryptoKey
{
    
    public:
        
        virtual ~CryptoKey()
        {
        }

        /**
        Encodes the specified data block.
        \param[in,out] data Raw pointer to the data which is to be encoded.
        \param[in] size Specifies the data block size (in bytes).
        \throws NullPointerException If 'data' is empty.
        \throws EncryptionException If the data encryption failed with the given key.
        The data may be corrupted after failed encryption!
        */
        virtual void Encode(void* data, size_t size) const = 0;
        /**
        Decodes the specified data block.
        \param[in,out] data Raw pointer to the data which is to be decoded.
        \param[in] size Specifies the data block size (in bytes).
        \throws NullPointerException If 'data' is empty.
        \throws DecryptionException If the data decryption failed with the given key.
        The data may be corrupted after failed decryption!
        */
        virtual void Decode(void* data, size_t size) const = 0;
        
        /**
        Encodes the specified data block.
        \tparam Data Specifies the data block structure.
        This should be a 'struct', 'class' or any primitive data type. Pointers are not allowed!
        \param[in,out] data Specifies the data block which is to be encoded.
        \see Encode(void*, size_t)
        */
        template <class Data> void Encode(Data& data)
        {
            static_assert(!std::is_pointer<Data>::value, "\"" __FUNCTION__ "\" does not allow pointer types");
            Encode(&data, sizeof(Data));
        }
        /**
        Decodes the specified data block.
        \tparam Data Specifies the data block structure.
        This should be a 'struct', 'class' or any primitive data type. Pointers are not allowed!
        \param[in,out] data Specifies the data block which is to be decoded.
        \see Decode(void*, size_t)
        */
        template <class Data> void Decode(Data& data)
        {
            static_assert(!std::is_pointer<Data>::value, "\"" __FUNCTION__ "\" does not allow pointer types");
            Decode(&data, sizeof(Data));
        }

        /**
        Encodes the specified STL container.
        \tparam Container Specifies the container type. This type must implement the following interface:
        \code
        reference operator [] (size_t index);
        size_t size() const;
        \endcode
        This could be std::vector or std::string.
        \param[in,out] data Specifies the container which is to be encoded.
        */
        template <class Container> void EncodeContainer(Container& data)
        {
            Encode(&data[0], data.size());
        }
        /**
        Decodes the specified STL container.
        \tparam Container Specifies the container type. This type must implement the following interface:
        \code
        reference operator [] (size_t index);
        size_t size() const;
        \endcode
        This could be std::vector or std::string.
        \param[in,out] data Specifies the container which is to be decoded.
        */
        template <class Container> void DecodeContainer(Container& data)
        {
            Decode(&data[0], data.size());
        }

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================
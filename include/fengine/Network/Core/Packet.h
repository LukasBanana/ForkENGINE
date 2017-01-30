/*
 * Network packet header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_PACKET_H__
#define __FORK_NETWORK_PACKET_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"

#include <vector>


namespace Fork
{

namespace Network
{


DECL_SHR_PTR(Packet);

/**
Network packet class. This can be used to implement simple custom protocols.
The format of the packet buffer is as follows:
\code
16 Bit (Unsigned Short) for the type ID.
16 Bit (Unsigned Short) for the flags.
N Bit for the packet data.
\endcode
\ingroup network
*/
class FORK_EXPORT Packet
{
    
    public:
        
        static const size_t defaultBufferMaxSize = 1024;

        Packet(Packet&& other);
        /**
        This constructor creates an empty raw-buffer with the specified size (in bytes).
        \throws InvalidArgumentException If size < Packet::BufferHeaderSize().
        */
        Packet(size_t rawSize);
        /**
        This constructor creates an initialized raw-buffer with the specified size (in bytes) and content.
        \throws InvalidArgumentException If size < Packet::BufferHeaderSize().
        */
        Packet(size_t rawSize, const char* rawBuffer);
        /**
        This constructor creates an empty buffer with the specified size (in bytes).
        The raw buffer is larger (see BufferHeaderSize).
        \see BufferHeaderSize
        \see WriteTypeID
        \see WriteFlags
        */
        Packet(size_t size, unsigned short typeID, unsigned short flags);
        /**
        This constructor creates an initialized buffer with the specified size (in bytes) and content.
        \see BufferHeaderSize
        \see WriteTypeID
        \see WriteFlags
        */
        Packet(size_t size, const void* buffer, unsigned short typeID, unsigned short flags);
        /**
        This constructor creates an initialized buffer with the specified size (in bytes) and content.
        \see BufferHeaderSize
        \see Packet(size_t, const void*, unsigned short, unsigned short)
        */
        Packet(const std::vector<char>& buffer, unsigned short typeID, unsigned short flags);

        //! Returns the buffer header size (in bytes).
        static size_t BufferHeaderSize();

        //! Writes the buffer data.
        void WriteBuffer(const void* buffer, size_t size, size_t offset = 0);

        //! Returns a pointer to the packet buffer.
        void* Buffer();
        //! Returns a constant pointer to the packet buffer.
        const void* Buffer() const;

        /**
        Writes the type ID for this packet. By default 0.
        \remarks Because the default value for the type ID is 0,
        one should use a value larger than 0 in a custom protocol.
        */
        void WriteTypeID(unsigned short typeID);
        //! Returns the type ID of this packet.
        unsigned short TypeID() const;

        //! Writes the flags of this packet. By default 0.
        void WriteFlags(unsigned short flags);
        //! Returns the flags of this packet.
        unsigned short Flags() const;

        //! Resizes the packet buffer.
        void Resize(size_t size);
        //! Returns the size of the packet buffer.
        size_t Size() const;

        /**
        Resizes the packet raw-buffer.
        \throws InvalidArgumentException If rawSize < Packet::BufferHeaderSize().
        */
        void RawResize(size_t rawSize);
        
        //! Returns the size of the raw buffer.
        inline size_t RawSize() const
        {
            return buffer_.size();
        }

        /**
        Returns a pointer to the raw buffer. The first 4 bytes are reserved for the packet header:
        2 unsigned bytes for the type ID and 2 unsigned bytes for the flags.
        */
        inline char* RawBuffer()
        {
            return buffer_.data();
        }
        //! \see char* RawBuffer()
        inline const char* RawBuffer() const
        {
            return buffer_.data();
        }

    private:
        
        void CheckRawSize(size_t rawSize);

        /**
        Packet buffer with format: 16 bit for the type ID,
        16 bit for flags and the rest is for the buffer data.
        */
        std::vector<char> buffer_;

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================
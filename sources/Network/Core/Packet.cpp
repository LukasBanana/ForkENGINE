/*
 * Network packet file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/Core/Packet.h"
#include "Core/Exception/InvalidArgumentException.h"

#include <algorithm>


namespace Fork
{

namespace Network
{


Packet::Packet(Packet&& other) :
    buffer_{ std::move(other.buffer_) }
{
}
Packet::Packet(size_t rawSize) :
    buffer_( rawSize, '\0' )
{
    CheckRawSize(rawSize);
    WriteTypeID(0);
    WriteFlags(0);
}
Packet::Packet(size_t rawSize, const char* rawBuffer) :
    Packet{ rawSize }
{
    std::copy(rawBuffer, rawBuffer + rawSize, RawBuffer());
}
Packet::Packet(size_t size, unsigned short typeID, unsigned short flags) :
    buffer_( size + Packet::BufferHeaderSize(), '\0' )
{
    WriteTypeID(typeID);
    WriteFlags(flags);
}
Packet::Packet(size_t size, const void* buffer, unsigned short typeID, unsigned short flags) :
    Packet{ size, typeID, flags }
{
    WriteBuffer(buffer, size);
}
Packet::Packet(const std::vector<char>& buffer, unsigned short typeID, unsigned short flags) :
    Packet{ buffer.size(), buffer.data(), typeID, flags }
{
}

size_t Packet::BufferHeaderSize()
{
    return sizeof(unsigned short)*2;
}

void Packet::WriteBuffer(const void* buffer, size_t size, size_t offset)
{
    auto byteAlignedSrcBuffer = reinterpret_cast<const char*>(buffer);
    auto byteAlignedDestBuffer = reinterpret_cast<char*>(Buffer());
    
    std::copy(
        byteAlignedSrcBuffer,
        byteAlignedSrcBuffer + size,
        byteAlignedDestBuffer + offset
    );
}

void* Packet::Buffer()
{
    return RawBuffer() + Packet::BufferHeaderSize();
}

const void* Packet::Buffer() const
{
    return RawBuffer() + Packet::BufferHeaderSize();
}

void Packet::WriteTypeID(unsigned short typeID)
{
    *reinterpret_cast<unsigned short*>(RawBuffer()) = typeID;
}

unsigned short Packet::TypeID() const
{
    return *reinterpret_cast<const unsigned short*>(RawBuffer());
}

void Packet::WriteFlags(unsigned short flags)
{
    *reinterpret_cast<unsigned short*>(RawBuffer() + sizeof(unsigned short)) = flags;
}

unsigned short Packet::Flags() const
{
    return *reinterpret_cast<const unsigned short*>(RawBuffer() + sizeof(unsigned short));
}

void Packet::Resize(size_t size)
{
    buffer_.resize(Packet::BufferHeaderSize() + size);
}

size_t Packet::Size() const
{
    return buffer_.size() - Packet::BufferHeaderSize();
}

void Packet::RawResize(size_t rawSize)
{
    CheckRawSize(rawSize);
    buffer_.resize(rawSize);
}


/*
 * ======= Private: =======
 */

void Packet::CheckRawSize(size_t rawSize)
{
    auto headerSize = Packet::BufferHeaderSize();
    
    if (rawSize < headerSize)
    {
        throw InvalidArgumentException(
            __FUNCTION__, "rawSize",
            "Size of the raw network packet buffer must not be less than " + ToStr(headerSize)
        );
    }
}


} // /namespace Network

} // /namespace Fork



// ========================
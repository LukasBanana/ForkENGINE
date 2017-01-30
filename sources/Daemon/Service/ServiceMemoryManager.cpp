/*
 * Daemon service memory manager file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ServiceMemoryManager.h"

#include <vector>
#include <map>
#include <algorithm>
#include <fstream>


namespace Fork
{

namespace Daemon
{

namespace Service
{

namespace MemoryManager
{


/*
 * Internals
 */

static std::shared_ptr<IdentityFactory> chunkIdFactory = std::make_shared<IdentityFactory>();

class MemoryChunk : public Identifiable
{
    
    public:
        
        MemoryChunk(unsigned int size, const Client& owner) :
            Identifiable( chunkIdFactory     ),
            buffer      ( size_t(size), '\0' ),
            ownerAddress( owner.address      )
        {
        }

        MemoryChunk(const MemoryChunk&) = delete;
        MemoryChunk& operator = (const MemoryChunk&) = delete;

        inline unsigned int Size() const
        {
            return static_cast<unsigned int>(buffer.size());
        }

        inline char* Buffer(unsigned int offset = 0)
        {
            return buffer.data() + offset;
        }
        inline const char* Buffer(unsigned int offset = 0) const
        {
            return buffer.data() + offset;
        }

        inline bool IsOwner(const Client& owner) const
        {
            return ownerAddress == owner.address;
        }

        std::vector<char> buffer;
        Network::AddressPtr ownerAddress;

};

typedef std::unique_ptr<MemoryChunk> MemoryChunkPtr;

static std::map<ChunkID, MemoryChunkPtr> memoryChunks;

static MemoryChunk* FindChunk(ChunkID chunkID, const Client& owner)
{
    auto it = memoryChunks.find(chunkID);
    return (it != memoryChunks.end() && it->second->IsOwner(owner)) ? it->second.get() : nullptr;
}


/*
 * Global functions
 */

ChunkID AllocChunk(unsigned int size, const Client& owner)
{
    /* Allocate new memory chunk */
    auto chunk = std::make_unique<MemoryChunk>(size, owner);
    
    const auto id = chunk->GetID();
    memoryChunks[id] = std::move(chunk);

    return id;
}

bool FreeChunk(ChunkID id, const Client& owner)
{
    if (id == Identifiable::invalidID)
    {
        /* Remove all owern's memory chunks */
        for (auto it = memoryChunks.begin(); it != memoryChunks.end();)
        {
            if (it->second->IsOwner(owner))
                it = memoryChunks.erase(it);
            else
                ++it;
        }
        return true;
    }
    else
    {
        /* Remove chunk from the map (unique_ptr will delete the memory) */
        auto it = memoryChunks.find(id);
        if (it != memoryChunks.end() && it->second->IsOwner(owner))
        {
            memoryChunks.erase(it);
            return true;
        }
    }
    return false;
}

bool PushSegment(ChunkID id, const Client& owner, unsigned int offset, unsigned int size, const void* data)
{
    auto chunk = FindChunk(id, owner);

    if (chunk && (offset + size) <= chunk->Size())
    {
        /* Copy memory from source 'data' to chunk */
        const auto dataByteAlign = reinterpret_cast<const char*>(data);
        std::copy(dataByteAlign, dataByteAlign + size, chunk->Buffer(offset));
        return true;
    }

    return false;
}

bool PullSegment(ChunkID id, const Client& owner, unsigned int offset, unsigned int size, void* data)
{
    auto chunk = FindChunk(id, owner);

    if (chunk && (offset + size) <= chunk->Size())
    {
        /* Copy memory from chunk to destination 'data' */
        auto dataByteAlign = reinterpret_cast<char*>(data);
        std::copy(chunk->Buffer(offset), chunk->Buffer(offset + size), dataByteAlign);
        return true;
    }

    return false;
}

bool StoreChunkToFile(ChunkID id, const Client& owner, const std::string& filename)
{
    auto chunk = FindChunk(id, owner);

    if (chunk)
    {
        /* Open output file */
        std::ofstream file(filename, std::ofstream::binary);

        if (file.good())
        {
            file.write(chunk->Buffer(), chunk->Size());
            return true;
        }
    }

    return false;
}

unsigned int QueryChunkSize(ChunkID id, const Client& owner)
{
    auto chunk = FindChunk(id, owner);
    return chunk != nullptr ? chunk->Size() : 0u;
}

unsigned int NumChunks(const Client& owner)
{
    if (owner)
    {
        unsigned int num = 0;

        for (const auto& chunk : memoryChunks)
        {
            if (chunk.second->IsOwner(owner))
                ++num;
        }

        return num;
    }
    return static_cast<unsigned int>(memoryChunks.size());
}

std::vector<ChunkID> EnumChunkIDs(const Client& owner)
{
    std::vector<ChunkID> chunkIDs;

    for (const auto& chunk : memoryChunks)
    {
        if (chunk.second->IsOwner(owner))
            chunkIDs.push_back(chunk.second->GetID());
    }

    return chunkIDs;
}

/*void AdoptOwnership(const Client& owner)
{
}*/


} // /namespace MemoryManager

} // /namespace Service

} // /namespace Daemon

} // /namespace Fork



// ========================
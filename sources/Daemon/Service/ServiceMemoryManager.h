/*
 * Daemon service memory manager header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DAEMON_SERVICE_MEMORY_MANAGER_H__
#define __FORK_DAEMON_SERVICE_MEMORY_MANAGER_H__


#include "Core/Container/IdentityFactory.h"
#include "../ServerSideClient.h"

#include <string>


namespace Fork
{

namespace Daemon
{

namespace Service
{

namespace MemoryManager
{


//! Memory chunk ID type.
typedef Identifiable::IDType ChunkID;

//! Allocates a new memory chunk and returns its ID.
ChunkID AllocChunk(unsigned int size, const Client& owner);
//! Frees the specified memory chunk or frees all chunks which belong to the specified owner (if id is 0).
bool FreeChunk(ChunkID id, const Client& owner);

//! Pushs a data segment to the specified memory chunk.
bool PushSegment(ChunkID id, const Client& owner, unsigned int offset, unsigned int size, const void* data);
//! Pulls a data segment from the specified memory chunk.
bool PullSegment(ChunkID id, const Client& owner, unsigned int offset, unsigned int size, void* data);

//! Stores the specified memory chunk to a file.
bool StoreChunkToFile(ChunkID id, const Client& owner, const std::string& filename);

//! Returns the size of the specified memory chunk.
unsigned int QueryChunkSize(ChunkID id, const Client& owner);

//! Returns the number of active memory chunks.
unsigned int NumChunks(const Client& owner);
//! Enumerates the chunk IDs which belong to the specified owner.
std::vector<ChunkID> EnumChunkIDs(const Client& owner);

//! Adopts the ownership of all ownerless memory chunks to 'owner'.
//void AdoptOwnership(const Client& owner);


} // /namespace MemoryManager

} // /namespace Service

} // /namespace Daemon

} // /namespace Fork


#endif



// ========================
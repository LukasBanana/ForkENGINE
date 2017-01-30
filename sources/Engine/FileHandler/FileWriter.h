/*
 * Engine file writer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_FILE_WRITER_H__
#define __FORK_ENGINE_FILE_WRITER_H__


#include "FileHandler.h"
#include "BaseStructs.h"
#include "Core/Exception/InvalidArgumentException.h"

#include <vector>
#include <stack>
#include <map>
#include <limits>


namespace Fork
{

namespace Engine
{

namespace Format
{


//! Engine format file writer.
class FileWriter : public FileHandler
{
    
    public:
        
        FileWriter(IO::File& file);

        FileWriter(const FileWriter&) = delete;
        FileWriter& operator = (const FileWriter&) = delete;

        /* --- Base writing --- */

        /**
        Writes the file header with the specified attributes.
        \param[in] magic Specifies the magic 'number'. This must be a string of four characters.
        \param[in] version Specifies the format version.
        \param[in] compatibleVersion Specifies the last compatible format version.
        \param[in] flags Optional format flags.
        \throws InvalidArguments If 'magic' is not a string of four characters.
        */
        void WriteHeader(const std::string& magic, UInt16 version, UInt16 compatibleVersion, UInt32 flags = 0);

        //! Writes the specified 32-bit unsigned integer.
        void WriteUInt32(UInt32 value);

        //! Writes a UTF-8 string with a maximal length of 2^8 and throws "InvalidArgumentException" if the string is too long.
        void WriteString8(const std::string& str);
        //! Writes a UTF-16 string with a maximal length of 2^8 and throws "InvalidArgumentException" if the string is too long.
        void WriteWString8(const std::wstring& str);

        //! Writes a UTF-8 string with a maximal length of 2^16 and throws "InvalidArgumentException" if the string is too long.
        void WriteString16(const std::string& str);
        //! Writes a UTF-16 string with a maximal length of 2^16 and throws "InvalidArgumentException" if the string is too long.
        void WriteWString16(const std::wstring& str);

        //! Writes a UTF-8 string with a maximal length of 2^32 and throws "InvalidArgumentException" if the string is too long.
        void WriteString32(const std::string& str);
        //! Writes a UTF-16 string with a maximal length of 2^32 and throws "InvalidArgumentException" if the string is too long.
        void WriteWString32(const std::wstring& str);

        //! Writes the specified buffer.
        template <typename T> void Write(const T& buffer)
        {
            GetFile().Write(buffer);
        }

        /* --- Stacks --- */

        /**
        Reserves memory in the file for a file offset.
        This will be pushed onto a stack, which can be resolved later with "ResolveOffset".
        */
        void ReserveOffset();
        /**
        Pops the top most reserved offset from a stack and
        resolves this offset with the current file position.
        \throws InvalidStateException If the internal stack is empty.
        */
        void ResolveOffset();

        /**
        Reserves memory in the file for a chunk description.
        \param[in] id Specifies the chunk ID. This must be unique!
        \throws InvalidArgumentException If 'id' is already occupied.
        */
        void ReserveChunkDesc(UInt32 id);
        /**
        Stores the file offset in the chunk description with the specified ID.
        \throws InvalidArgumentException If 'id' has not been reserved with "ReserveChunkDesc".
        */
        void BeginChunk(UInt32 id);
        /**
        Stores the chunk size in the chunk description with the specified ID.
        \throws InvalidArgumentException If 'id' has not been reserved with "ReserveChunkDesc".
        */
        void EndChunk(UInt32 id);

        /* --- Lists --- */

        /**
        Writes the chunk descriptions array list.
        \see ReserveChunkDesc
        */
        void WriteChunkDescList(const std::vector<UInt32>& idList);

        /* --- Misc --- */

        /**
        Returns the 32-bit size of the specified container.
        \throws InvalidArgumentException If the container is too large for this file format.
        */
        template <typename Container> UInt32 ListSize(const Container& container) const
        {
            auto size = container.size();
            if (size > std::numeric_limits<UInt32>::max())
                throw InvalidArgumentException(__FUNCTION__, "container.size()", "Container exceeded maximal size for the engine file format");
            return static_cast<UInt32>(size);
        }

    private:
        
        struct StreamChunkDesc
        {
            std::streampos  reservedPos;
            ChunkDesc       chunkDesc;
        };

        //! Returns the stream position for the specified chunk ID.
        StreamChunkDesc* FindChunkDesc(UInt32 id);

        template <typename SizeType, typename CharType> void WriteString(const std::basic_string<CharType>& str);

        /* === Members === */

        std::stack<std::streampos>          offsetStack_;
        std::map<UInt32, StreamChunkDesc>   chunkDescMap_;

};


} // /namespace Format

} // /namespace Engine

} // /namespace Fork


#endif



// ========================
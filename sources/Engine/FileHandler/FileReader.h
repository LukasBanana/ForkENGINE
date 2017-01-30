/*
 * Engine file reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_FILE_READER_H__
#define __FORK_ENGINE_FILE_READER_H__


#include "FileHandler.h"
#include "BaseStructs.h"

#include <vector>
#include <map>


namespace Fork
{

namespace Engine
{

namespace Format
{


//! Engine format file reader.
class FileReader : public FileHandler
{
    
    public:
        
        FileReader(IO::File& file);

        FileReader(const FileReader&) = delete;
        FileReader& operator = (const FileReader&) = delete;

        /* --- Base reading --- */

        /**
        Reads the file header with the specified attributes.
        \param[in] magic Specifies the required magic 'number'. This must be a string of four characters.
        \param[in] version Specifies the version which must be compatible with the file format version (see "FileHeader::compatibleVersion").
        \throws EngineFormatException If reading the header failed.
        */
        void ReadHeader(const std::string& magic, UInt16 version);

        //! Reads a 32-bit unsigned integer.
        UInt32 ReadUInt32();

        //! Reads a UTF-8 string with a maximal length of 2^8.
        std::string ReadString8();
        //! Reads a UTF-16 string with a maximal length of 2^8.
        std::wstring ReadWString8();

        //! Reads a UTF-8 string with a maximal length of 2^16.
        std::string ReadString16();
        //! Reads a UTF-16 string with a maximal length of 2^16.
        std::wstring ReadWString16();

        //! Reads a UTF-8 string with a maximal length of 2^32.
        std::string ReadString32();
        //! Reads a UTF-16 string with a maximal length of 2^32.
        std::wstring ReadWString32();

        //! Reads a buffer.
        template <typename T> T Read()
        {
            return GetFile().Read<T>();
        }

        /* --- Lists --- */

        //! Reads an array list of chunk descriptions, stores them in a hash-map and returns their IDs.
        std::vector<UInt32> ReadChunkDescList();

        /**
        Seeks the stream to the chunk with the specified ID.
        \param[in] id Specifies the chunk ID.
        \return Pointer to the chunk description of null if the chunk ID does not exist.
        */
        const ChunkDesc* LocateChunk(UInt32 id);

    private:
        
        template <typename SizeType, typename CharType> std::basic_string<CharType> ReadString();

        /* === Members === */

        std::map<UInt32, ChunkDesc> chunkDescMap_;

};


} // /namespace Format

} // /namespace Engine

} // /namespace Fork


#endif



// ========================
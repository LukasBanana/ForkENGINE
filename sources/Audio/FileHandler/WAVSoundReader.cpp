/*
 * WAV sound reader file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "WAVSoundReader.h"
#include "IO/Core/Log.h"
#include "IO/FileSystem/FileStreamHelper.h"
#include "Core/AuxiliaryMacros.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Audio
{


/*
 * Internal structures
 */

#include "Core/PackPush.h"

struct RIFFWAVEChunk
{
    unsigned long id;   //!< Chunk ID (either 'fmt ' or 'data').
    unsigned long size; //!< Chunk size (in bytes).
}
PACK_STRUCT;

#include "Core/PackPop.h"


/*
 * Internal functions
 */

static void WAVReadRIFFWAVEHeader(std::istream& file, unsigned long& fileSize)
{
    /* Read magic number 'RIFF' */
    unsigned long magicNumber = 0;
    IO::ReadFromStream(file, magicNumber, 4);

    if (magicNumber != HEX_FROM_STRING("RIFF"))
        throw std::runtime_error("Invalid magic number in RIFF WAVE file");

    /* Read file size */
    fileSize = 0;
    IO::ReadFromStream(file, fileSize, 4);

    if (fileSize <= 16)
        throw std::runtime_error("Invalid size data field in RIFF WAVE file (size = " + ToStr(fileSize) + ")");

    /* Read file format */
    unsigned long formatType = 0;
    IO::ReadFromStream(file, formatType, 4);

    if (formatType != HEX_FROM_STRING("WAVE"))
        throw std::runtime_error("Invalid format type in RIFF WAVE file");
}

static RIFFWAVEChunk WAVFindChunk(std::istream& file, unsigned long fileSize, const char* chunkID)
{
    RIFFWAVEChunk chunk;

    unsigned long offset = 12;

    while (offset < fileSize)
    {
        /* Read next chunk header */
        file.seekg(offset, std::ios::beg);
        IO::ReadFromStream(file, chunk.id  , 4);
        IO::ReadFromStream(file, chunk.size, 4);

        if (chunk.id == HEX_FROM_STRING(chunkID))
            return chunk;

        /*
        Add offset to find next chunk (guarantee WORD padding alignment)
        -> see http://www.win32developer.com/tutorial/xaudio/xaudio_tutorial_1.shtm
        */
        chunk.size += 9;
        chunk.size &= 0xfffffffe;

        offset += chunk.size;
    }

    throw std::runtime_error("Missing RIFF WAVE chunk '" + ToStr(chunkID) + "'");

    return chunk;
}

/*
RIFF WAVE format chunk (for RIFF tags see details).
\see http://de.wikipedia.org/wiki/RIFF_WAVE
\see http://www.sno.phy.queensu.ca/~phil/exiftool/TagNames/RIFF.html
*/
static void WAVReadChunks(std::istream& file, unsigned long fileSize, WaveBuffer& waveBuffer)
{
    static const unsigned short tagRIFF_MS_PCM = 0x0001;

    /* Read 'fmt ' chunk */
    auto chunkFMT = WAVFindChunk(file, fileSize, "fmt ");

    IO::ReadFromStream(file, waveBuffer.format);
    
    /* Validate format chunk */
    if (chunkFMT.size < 16)
        throw std::runtime_error("Invalid length in RIFF WAVE format chunk");
    if (waveBuffer.format.formatTag != tagRIFF_MS_PCM)
        throw std::runtime_error("Unsupported RIFF WAVE format tag (0x" + NumToHex(waveBuffer.format.formatTag) + ")");

    /* Read 'data' chunk */
    auto chunkDATA = WAVFindChunk(file, fileSize, "data");

    /* Read PCM data */
    waveBuffer.buffer.resize(chunkDATA.size);
    file.read(waveBuffer.buffer.data(), chunkDATA.size);
}


/*
 * WAVSoundReader class
 */

WaveBufferPtr WAVSoundReader::ReadSound(const std::string& filename)
{
    /* Open file for reading */
    std::ifstream file(filename, std::ios::binary);

    if (!file.good())
    {
        IO::Log::Error("Loading sound file \"" + filename + "\" failed");
        return nullptr;
    }

    try
    {
        /* Read RIFF WAVE header */
        unsigned long fileSize = 0;
        WAVReadRIFFWAVEHeader(file, fileSize);

        /* Create wave buffer and read chunks 'fmt ' and 'data' */
        auto waveBuffer = std::make_shared<WaveBuffer>();
        WAVReadChunks(file, fileSize, *waveBuffer);

        return waveBuffer;
    }
    catch (const std::exception& err)
    {
        IO::Log::Error(err.what());
    }

    return nullptr;
}


} // /namespace Audio

} // /namespace Fork



// ========================
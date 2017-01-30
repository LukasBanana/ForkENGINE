/*
 * OGG sound stream reader file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "OGGSoundStreamReader.h"


namespace Fork
{

namespace Audio
{


/*
 * Internal functions
 */

static std::string OGGErrorToString(int err)
{
    switch (err)
    {
        case OV_EREAD:
            return "A read from media returned an error";
        case OV_ENOTVORBIS:
            return "Bitstream does not contain any Vorbis data";
        case OV_EVERSION:
            return "Vorbis version mismatch";
        case OV_EBADHEADER:
            return "Invalid Vorbis bitstream header";
        case OV_EFAULT:
            return "Internal logic fault; indicates a bug or heap/stack corruption";
    }
    return "Unknown error";
}

#define FSTREAM_DATASRC reinterpret_cast<std::ifstream*>(datasource)

static size_t OGGReadProc(void* ptr, size_t size, size_t nmemb, void* datasource)
{
    auto file = FSTREAM_DATASRC;
    file->read(reinterpret_cast<char*>(ptr), size*nmemb);
    return static_cast<size_t>(file->gcount());
}

static int OGGSeekProc(void* datasource, ogg_int64_t offset, int whence)
{
    auto file = FSTREAM_DATASRC;
    switch (whence)
    {
        case SEEK_SET:
            file->seekg(offset, std::ios_base::beg);
            break;
        case SEEK_CUR:
            file->seekg(offset, std::ios_base::cur);
            break;
        case SEEK_END:
            file->seekg(offset, std::ios_base::end);
            break;
    }
    return file->fail() ? 1 : 0;
}

static int OGGCloseProc(void* datasource)
{
    auto file = FSTREAM_DATASRC;
    file->close();
    return file->fail() ? 1 : 0;
}

static long OGGTellProc(void* datasource)
{
    return static_cast<long>(FSTREAM_DATASRC->tellg());
}

#undef FSTREAM_DATASRC


/*
 * OGGSoundStreamReader class
 */

OGGSoundStreamReader::OGGSoundStreamReader(const std::string& filename)
{
    #ifndef _FORKAUDIO_USE_ANSI_C_FILES_
    /* Initialize function callbacks */
    ov_callbacks callbacks;
    
    callbacks.read_func = OGGReadProc;
    callbacks.seek_func = OGGSeekProc;
    callbacks.close_func = OGGCloseProc;
    callbacks.tell_func = OGGTellProc;

    /* Open file stream */
    file_.open(filename);
    if (!file_.good())
        throw std::runtime_error("Opening ogg vorbis stream failed");

    /* Open ogg-vorbis stream */
    auto result = ov_open_callbacks(&file_, &oggFile_, nullptr, 0, callbacks);
    if (result != 0)
        throw std::runtime_error("(Ogg Vorbis Stream) " + OGGErrorToString(result));
    #else
    /* Open file stream */
    file_ = fopen(filename.c_str(), "rb");
    if (!file_)
        throw std::runtime_error("Opening ogg vorbis stream failed");

    /* Open ogg-vorbis stream */
    auto result = ov_open(file_, &oggFile_, nullptr, 0);
    if (result != 0)
        throw std::runtime_error("(Ogg Vorbis Stream) " + OGGErrorToString(result));
    #endif

    /* Extract file information */
    info_ = ov_info(&oggFile_, -1);
    if (!info_)
        throw std::runtime_error("Retrieving information from ogg vorbis stream failed");
    
    /* Extract file comments */
    auto oggComment = ov_comment(&oggFile_, -1);
    if (oggComment)
    {
        for (int i = 0; i < oggComment->comments; ++i)
            comments.push_back(oggComment->user_comments[i]);
    }

    /* Extract stream total time */
    totalTime = ov_time_total(&oggFile_, -1);

    /* Setup wave format (ogg vorbis is always 16-bit coded) */
    waveFormat = MakeWaveFormatPCM(
        static_cast<unsigned short>(info_->channels),
        static_cast<unsigned long>(info_->rate),
        16
    );
}
OGGSoundStreamReader::~OGGSoundStreamReader()
{
    /* Close ogg-vorbis file stream */
    ov_clear(&oggFile_);
}

size_t OGGSoundStreamReader::ReadStream(PCMBuffer& buffer)
{
    int bitStream = 0;

    size_t pos = 0;
    size_t size = buffer.size();

    while (pos < size)
    {
        /* Read next ogg vorbis chunk */
        auto result = ov_read(
            &oggFile_,      // Ogg vorbis stream handle
            &(buffer[pos]), // Byte aligned buffer
            size,           // Buffer size
            0,              // Little endian
            2,              // 2 byte (16 bit) samples
            1,              // Signed samples
            &bitStream      // Current bit stream section
        );

        /* Copy array into output buffer */
        if (result > 0)
        {
            auto chunkSize = static_cast<size_t>(result);
            pos += chunkSize;
            size -= chunkSize;
        }
        else
            break;
    }

    return pos;
}

bool OGGSoundStreamReader::TimeSeek(double time)
{
    /* Seek ogg vorbis stream to specified time point */
    auto result = ov_time_seek(&oggFile_, time);

    if (result)
    {
        //!TODO! error handling ... !!!
        return false;
    }

    return true;
}


} // /namespace Audio

} // /namespace Fork



// ========================
/*
 * Sound stream reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUND_STREAM_READER_H__
#define __FORK_SOUND_STREAM_READER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Audio/Core/WaveBuffer.h"

#include <vector>


namespace Fork
{

namespace Audio
{


DECL_SHR_PTR(SoundStreamReader);

//! Sound stream reader interface.
class FORK_EXPORT SoundStreamReader
{
    
    public:
        
        virtual ~SoundStreamReader()
        {
        }

        /**
        Reads the sound stream.
        \param[in,out] buffer Buffer where the stream output is stored.
        This must already be an allocated buffer, i.e. if the buffer is empty, this function has no effect.
        \return The number of bytes read from the stream. If possible, the entire buffer will be filled.
        \remarks The stream is opened in the stream reader's constructor and closed in its destructor.
        */
        virtual size_t ReadStream(PCMBuffer& buffer) = 0;

        /**
        Seeks the stream to the specified time point. This can be used to fast-forward the sound stream.
        \param[in] time Specifies the time point (in seconds).
        \return True on success. Otherwise an error occured.
        \see GetTotalTime
        */
        virtual bool TimeSeek(double time) = 0;

        /**
        Seeks the stream to the specified position. This can be used to fast-forward the sound stream.
        \param[in] position Specifies the seek position. Must be in the range [0.0 .. 1.0].
        \see TimeSeek
        */
        inline void Seek(double position)
        {
            if (GetTotalTime() > 0.0)
                TimeSeek(position / GetTotalTime());
        }

        //! Returns the wave format of this sound stream.
        inline const WaveFormat& GetWaveFormat() const
        {
            return waveFormat;
        }

        //! Returns the comments string list.
        inline const std::vector<std::string>& GetComments() const
        {
            return comments;
        }

        //! Returns the total time (in seconds) of the stream.
        inline double GetTotalTime() const
        {
            return totalTime;
        }

    protected:

        SoundStreamReader() = default;

        //! Specifies the wave format.
        WaveFormat waveFormat;

        //! Comments string list.
        std::vector<std::string> comments;

        //! Total time (in seconds) of the stream.
        double totalTime = 0.0;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================
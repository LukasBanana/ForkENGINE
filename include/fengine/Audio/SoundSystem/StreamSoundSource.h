/*
 * Stream sound source header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STREAM_SOUND_SOURCE_H__
#define __FORK_STREAM_SOUND_SOURCE_H__


#include "Audio/SoundSystem/SoundSource.h"
#include "Audio/Core/WaveBuffer.h"


namespace Fork
{

namespace Audio
{


DECL_SHR_PTR(StreamSoundSource);

//! Stream sound source interface.
class FORK_EXPORT StreamSoundSource : public virtual SoundSource
{
    
    public:
        
        StreamSoundSource(const WaveFormat& waveFormat);

        //! Returns Types::Stream.
        Types Type() const override;

        /**
        Sumbits a new PCM wave buffer to this stream sound source.
        It will be appended to the end of the wave buffer queue.
        \param[in] pcmBuffer Specifies the PCM audio buffer.
        \param[in] bufferLimit Optional buffer limit. This can be used to submit only the
        first 'limit' bytes of the buffer. By default the entire buffer will be submitted.
        \param[in] loopCount Specifies the number of loop repetitions. By default 0.
        \remarks To determine the number of buffers currenlty queued, use "QueueSize" function.
        \return True if the buffer could be submitted successfully.
        Otherwise the maximum numbers of buffers in the queue has been reached.
        \see QueueSize
        \see MaxQueueSize
        */
        virtual bool SubmitBuffer(
            const PCMBuffer& pcmBuffer, size_t bufferLimit = size_t(-1), unsigned int loopCount = 0
        ) = 0;

        /**
        Returns the number of buffers currently queued.
        \see MaxQueueSize
        */
        virtual unsigned int QueueSize() const = 0;
        //! Returns the maxmimum number of buffers in the queue.
        virtual unsigned int MaxQueueSize() const = 0;

        //! Returns the wave format of this sound source.
        inline const WaveFormat& GetWaveFormat() const
        {
            return waveFormat_;
        }

    private:
        
        WaveFormat waveFormat_;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================
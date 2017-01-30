/*
 * Buffer sound source header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BUFFER_SOUND_SOURCE_H__
#define __FORK_BUFFER_SOUND_SOURCE_H__


#include "Audio/SoundSystem/SoundSource.h"
#include "Audio/Core/WaveBuffer.h"


namespace Fork
{

namespace Audio
{


DECL_SHR_PTR(BufferSoundSource);

//! Buffer sound source interface.
class FORK_EXPORT BufferSoundSource : public virtual SoundSource
{
    
    public:
        
        //! \throws NullPointerException If 'waveBuffer' is null.
        BufferSoundSource(const WaveBufferPtr& waveBuffer);

        //! Returns Types::Buffer.
        Types Type() const override;

        //! Returns the wave buffer of this sound source.
        inline const WaveBuffer* GetWaveBuffer() const
        {
            return waveBuffer_.get();
        }

    private:
        
        WaveBufferPtr waveBuffer_;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================
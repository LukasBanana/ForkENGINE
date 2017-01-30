/*
 * Sound emitter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUND_EMITTER_H__
#define __FORK_SOUND_EMITTER_H__


#include "Core/Export.h"


namespace Fork
{

namespace Audio
{


/**
Sound emitter interface. Used as base class for SoundEmitter2D and SoundEmitter3D.
\see SoundEmitter2D
\see SoundEmitter3D
\todo IN PROGRESS!
*/
class FORK_EXPORT SoundEmitter
{
    
    public:
        
        virtual ~SoundEmitter()
        {
        }

        virtual void Play() = 0;
        virtual void Pause(bool isPaused = true) = 0;
        virtual void Stop() = 0;

        virtual void SetupVolume(float volume) = 0;

        inline float GetVolume() const
        {
            return volume;
        }

    protected:
        
        float volume = 1.0f;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================
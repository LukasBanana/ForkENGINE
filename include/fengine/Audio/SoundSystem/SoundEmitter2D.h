/*
 * Sound emitter 2D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUND_EMITTER_2D_H__
#define __FORK_SOUND_EMITTER_2D_H__


#include "Audio/SoundSystem/SoundEmitter.h"


namespace Fork
{

namespace Audio
{


//! Sound emitter 2D interface.
class FORK_EXPORT SoundEmitter2D : public SoundEmitter
{
    
    public:
        
        virtual void SetupBalance(float balance) = 0;

        inline float GetBalance() const
        {
            return balance;
        }

    protected:
        
        float balance = 1.0f;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================
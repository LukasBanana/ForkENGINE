/*
 * Sound emitter 3D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUND_EMITTER_3D_H__
#define __FORK_SOUND_EMITTER_3D_H__


#include "Audio/SoundSystem/SoundEmitter.h"
#include "Math/Core/Vector3.h"


namespace Fork
{

namespace Audio
{


//! Sound emitter 3D interface.
class FORK_EXPORT SoundEmitter3D : public SoundEmitter
{
    
    public:
        
        virtual void SetupPosition(const Math::Point3f& position) = 0;

        inline const Math::Point3f& GetPosition() const
        {
            return position;
        }

        virtual void SetupVelovity(const Math::Vector3f& velovity) = 0;

        inline const Math::Vector3f& GetVelovity() const
        {
            return velovity;
        }

        virtual void SetupAttenuation(float attenuation) = 0;

        inline float GetAttenuation() const
        {
            return attenuation;
        }

    protected:
        
        Math::Point3f   position;
        Math::Vector3f  velovity;
        float           attenuation = 0.0f;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================
/*
 * Animation header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ANIMATION_H__
#define __FORK_ANIMATION_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Animation/Core/Playback.h"


namespace Fork
{

namespace Anim
{


DECL_SHR_PTR(Animation);

/**
Animation interface.
\ingroup animation
*/
class FORK_EXPORT Animation
{

    public:
        
        //! Animation class types.
        enum class Types
        {
            Node,           //!< Scene node animation
            Skeletal,       //!< Skeletal animation.
            MorphTarget,    //!< Morph-target animation.
        };

        virtual ~Animation()
        {
        }

        //! Returns the animation class type.
        virtual Types Type() const = 0;

        /**
        Updates the animation.
        \param[in] deltaTime Specifies the time derivation. For more information see 'Playback::Update'.
        \see Playback::Update
        */
        virtual void Update(double deltaTime = 1.0/60.0) = 0;

        //! Animation playback state object.
        Playback playback;

    protected:
        
        Animation() = default;

};


} // /namespace Anim

} // /namespace Fork


#endif



// ========================
/*
 * Default playback event handlers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DEFAULT_PLAYBACK_EVENT_HANDLERS_H__
#define __FORK_DEFAULT_PLAYBACK_EVENT_HANDLERS_H__


#include "Core/Export.h"
#include "Animation/Core/Playback.h"

#include <vector>


namespace Fork
{

namespace Anim
{

//! Namespace for all default playback event handlers.
namespace DefaultPlayback
{


//! One shot playback event handler.
class FORK_EXPORT OneShot : public Playback::EventHandler
{

    public:

        void OnNextFrame(Playback& playback) override;

};

//! Infinite loop playback event handler.
class FORK_EXPORT Loop : public Playback::EventHandler
{

    public:

        void OnNextFrame(Playback& playback) override;

};

/**
Infinite ping-pong loop playback event handler.
The playback will switch between forward and backward.
*/
class FORK_EXPORT PingPongLoop : public Playback::EventHandler
{

    public:

        void OnNextFrame(Playback& playback) override;

};

//! Infinite loop playback event handler with a list of frame indices.
class FORK_EXPORT ListLoop : public Playback::EventHandler
{
    
    public:
        
        void OnNextFrame(Playback& playback) override;

        //! List of frame indices.
        std::vector<size_t> frames;

        //! Frame list iterator.
        std::vector<size_t>::size_type iterator = 0;

};


} // /namespace DefaultPlayback

} // /namespace Anim

} // /namespace Fork


#endif



// ========================
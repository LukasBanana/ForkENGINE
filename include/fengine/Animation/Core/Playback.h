/*
 * Playback header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLAYBACK_H__
#define __FORK_PLAYBACK_H__


#include "Core/Export.h"

#include <memory>


namespace Fork
{

namespace Anim
{


/**
Animation playback class. This class does not store any data about the keyframes or transformations.
Only the process of playing an animation is managed by this class.
\remarks Although this class has an event handler, there are no functions called "Post..."
because the events are posted from the internal functions (such as "Update", "Play" etc.).
\remarks No matter what direction the playback moves, an interpolation between frames is allways computed as follows:
\code
auto from   = playback.frame;           // Frame index 'from' which we interpolate.
auto to     = playback.nextFrame;       // Frame index 'to' which we interpolate.
auto time   = playback.interpolator;    // Interpolation time (in the range [0.0 .. 1.0]).
auto result = Math::Lerp(MyTransformations[from], MyTransformations[to], time);
\endcode
\see Playback::EventHandler
\ingroup animation
*/
class FORK_EXPORT Playback
{

    public:
        
        //! Playback states.
        enum class States
        {
            Playing,    //!< Animation is currently playing.
            Paused,     //!< Animation has been paused.
            Stopped,    //!< Animation has been stopped.
        };

        /* === Classes === */

        //! Playback event handler interface.
        class FORK_EXPORT EventHandler
        {

            public:

                virtual ~EventHandler()
                {
                }

                /**
                Receives the 'playback start' event. All playback configurations are done when this event is posted.
                \see Playback::Play
                */
                virtual void OnPlay(Playback& playback)
                {
                }
                /**
                Receives the 'playback paused' event. All playback configurations are done when this event is posted.
                \see Playback::Pause
                */
                virtual void OnPause(Playback& playback)
                {
                }
                /**
                Receives the 'playback stopped' event. All playback configurations are done when this event is posted.
                \remarks This will only be posted if the playback was previously playing or paused.
                \see Playback::Stop
                */
                virtual void OnStop(Playback& playback)
                {
                }
                /**
                Receives the 'next frame' event. This event will be posted in the "Playback::Update" function.
                Depending on the playback speed sometimes this event will be posted several times for
                a single call to the "Playback::Update" function. This happens when several frames will be skipped,
                i.e. every single frame can be examined with this event handler interface, no matter how fast the playback speed is.
                All playback configurations for the next frame are done when this event is posted.
                \remarks This function should set the next frame (see 'nextFrame' field).
                \see Playback::Update
                \see nextFrame
                */
                virtual void OnNextFrame(Playback& playback)
                {
                }
                
        };

        typedef std::shared_ptr<EventHandler> EventHandlerPtr;

        /* === Functions === */

        /**
        Playback constructor.
        \param[in] initialEventHandler Optional shared pointer to the
        initial event handler. This may also be null. By default null.
        */
        Playback(const EventHandlerPtr& initialEventHandler = nullptr);

        /**
        Starts the playback process.
        \param[in] firstFrameIndex Specifies the index of the first frame. This may also be greater than the index of the last frame.
        \param[in] lastFrameIndex Specifies the index of the last frame. This may also be less than the index of the first frame.
        \param[in] newSpeed Specifies the animation speed factor. This may also be negative (to animate backward). By default 1.0.
        \param[in] newEventHandler Shared pointer to the new event handler. If this is null, the previous event handler will be used. By default null.
        \see DefaultEventHandler
        */
        void Play(size_t firstFrameIndex, size_t lastFrameIndex, double newSpeed, const EventHandlerPtr& newEventHandler = nullptr);
        /**
        Starts the playback process with the previous speed.
        \see Play(size_t, size_t, double, const EventHandlerPtr&)
        */
        void Play(size_t firstFrameIndex, size_t lastFrameIndex, const EventHandlerPtr& newEventHandler = nullptr);
        /**
        Starts the playback process with the previous frame indicies and speed.
        \see Play(size_t, size_t, double, const EventHandlerPtr&)
        */
        void Play(const EventHandlerPtr& newEventHandler = nullptr);

        /**
        Pauses or resumes the animation playback.
        \param[in] isPaused Specifies whether the playback is to be paused or resumed.
        If true the playback will be paused otherwise it will be resumed.
        An animation playback can only be resumed if it was paused previously (state must be 'States::Paused').
        An animation playback can only be paused if it was playing previously (state must be 'States::Playing').
        \see GetState
        \see States
        */
        void Pause(bool isPaused = true);

        /**
        Stops the animation playback. After this call the state will be States::Stopped.
        \see GetState
        \see States
        */
        void Stop();

        /**
        Updates the playback process. This is increase (or decrease if speed is negative) the frame interpolator.
        \param[in] deltaTime Specifies the time derivation between the previous and current rendered frame.
        If the application runs with 60 Hz this value should be 1.0/60.0, if the application runs with 200 Hz
        it should be 1.0/200.0 etc. This value will be added (and always added, also when the playback is backward)
        to the 'interpolator'. By default '1.0/60.0'.
        \remarks If the frame interpolator is greater than or equal to 1.0 the next frame will be set.
        Also the "OnNextFrame" function will be called from the event handler (see 'EventHandler' class).
        \remarks If the animation is currently not playing (state must be 'States::Playing') or
        'deltaTime' is less than or equal to 0.0, this function call has no effect.
        \see EventHandler
        \see Play
        */
        void Update(double deltaTime = 1.0/60.0);

        /**
        Sets the next frame depending on the playback direction.
        \param[in] nextFrameIndex Specifies the next frame index.
        \remarks If the playback direction is forward the field
        'nextFrame' is set, otherwise the field 'frame' is set.
        \see IsForward
        */
        void SetupNextFrame(size_t nextFrameIndex);

        /**
        Sets the next frame depending on the current frame, the playback direction and the frame chronlogy.
        \note The frame index range (first- and last frame indices) is ignored here!
        \see IsForward
        \see AreFramesChrono
        */
        void SetupNextFrame();

        /**
        Returns true if the end of animation playback has been reached.
        This depends on the playback direction and chronology.
        \see IsForward
        \see AreFramesChrono
        */
        bool HasEndReached() const;

        //! Returns the playback state.
        inline States GetState() const
        {
            return state_;
        }

        /**
        Returns true if the first- and last frames are chronologic.
        This is the case when the first frame is less than or equal to the last frame (firstFrame <= lastFrame).
        \see firstFrame
        \see lastFrame
        */
        inline bool AreFramesChrono() const
        {
            return firstFrame <= lastFrame;
        }

        /**
        Returns true if the playback moves froward.
        This is the case when the speed is non-negative (speed >= 0.0).
        \see speed
        */
        inline bool IsForward() const
        {
            return speed >= 0.0;
        }

        /**
        Returns true if this playback is currently being played (also true when it's paused).
        \remarks This is equivalent to 'GetState() != States::Stopped'.
        \see GetState
        */
        inline bool IsPlaying() const
        {
            return GetState() != States::Stopped;
        }

        /* === Members === */

        //! First frame is in the range [0 .. +inf). By default 0.
        size_t firstFrame = 0;

        //! Last frame is in the range [0 .. +inf). By default 0.
        size_t lastFrame = 0;

        //! Current frame is in the range [firstFrame .. lastFrame]. By default 0.
        size_t frame = 0;

        //! Next frame is in the range [firstFrame .. lastFrame]. By default 0.
        size_t nextFrame = 0;

        //! Frame interpolator is in the range [0.0 .. 1.0]. By default 0.0.
        double interpolator = 0.0;

        //! Animation speed is in the range (-inf .. +inf). By default 1.0.
        double speed = 1.0;

    private:
        
        void ChangePauseState(const States state);

        States state_ = States::Stopped;

        EventHandlerPtr eventHandler_;

};


} // /namespace Anim

} // /namespace Fork


#endif



// ========================
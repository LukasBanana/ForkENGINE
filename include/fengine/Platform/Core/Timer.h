/*
 * Timer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TIMER_H__
#define __FORK_TIMER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"


namespace Fork
{

namespace Platform
{


DECL_SHR_PTR(Timer);

class FORK_EXPORT Timer
{
    
    public:
        
        //! Type for frame counting. This is a 64-bit unsigned integer.
        typedef unsigned long long FrameCounterType;

        virtual ~Timer();

        //! Creates a platform specific timer object.
        static TimerPtr Create();

        /**
        Starts the time measurement. The result of the time measurement can be retrieved by the "GetElapsedTime" function.
        \see GetElapsedTime
        \see Stop
        */
        virtual void Start() = 0;
        /**
        Stop the time measurement. The result of the time measurement can be retrieved by the "GetElapsedTime" function.
        \see GetElapsedTime
        \see Start
        */
        virtual void Stop() = 0;

        /**
        Measures the current frame time.
        \note This must be called once per frame, before 'GetDeltaTime' or 'GetFrameRame' can be used.
        \see GetDeltaTime
        \see GetFrameRame
        */
        void MeasureFrame();

        /**
        Resets the frame count.
        \see GetFrameCount
        */
        void ResetFrameCount();

        /**
        Returns the elapsed time (in microseconds) between the previous calls of the "Start" and "Stop" functions.
        \remarks Example usage:
        \code
        timer->Start();
        // Time measurement is here ...
        timer->Stop();
        auto elaspedTime = timer->GetElapsedTime();
        \endcode
        \see Start
        \see Stop
        */
        inline double GetElapsedTime() const
        {
            return elapsedTime_;
        }

        /**
        Returns the total number of frames since the game has started.
        \remarks 'MeasureFrame' must be called once per frame before this function can be used.
        \see MeasureFrame
        \see ResetFrameCount
        */
        inline FrameCounterType GetFrameCount() const
        {
            return frameCount_;
        }

        /**
        Returns the time derivation between the previous and current frame.
        \remarks 'MeasureFrame' must be called once per frame before this function can be used.
        \see MeasureFrame
        */
        inline double GetDeltaTime() const
        {
            return deltaTime_;
        }

        /**
        Returns the scaled delta time.
        \remarks This can be used for slow-motion effects.
        \see GetDeltaTime
        \see timeScale
        */
        inline double ScaledDeltaTime() const
        {
            return GetDeltaTime() * timeScale;
        }

        /**
        Returns the number of frames per second (statisic value) or rather the frame rate (in Hz).
        \remarks 'MeasureFrame' must be called once per frame before this function can be used.
        \see MeasureFrame
        */
        inline double GetFrameRate() const
        {
            return frameRate_;
        }

        /**
        Specifies the time detla scale factor. By default 1.0.
        \remarks This can be used for slow-motion effects.
        \see ScaledDeltaTime
        */
        double timeScale = 1.0;

    protected:
        
        Timer() = default;

        /**
        Sets the elapsed time.
        \remarks The elapsed time can only be accessed by its getter and setter
        functions to avoid using pointers or references of this value.
        \see GetElapsedTime
        */
        inline void SetElapsedTime(double time)
        {
            elapsedTime_ = time;
        }

    private:

        double              elapsedTime_    = 0.0;
        double              deltaTime_      = 0.0;
        double              frameRate_      = 0.0;
        FrameCounterType    frameCount_     = 0;


};


} // /namespace Platform

} // /namespace Fork


#endif



// ========================
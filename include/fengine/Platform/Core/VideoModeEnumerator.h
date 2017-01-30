/*
 * Video mode enumerator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VIDEO_MODE_ENUMERATOR_H__
#define __FORK_VIDEO_MODE_ENUMERATOR_H__


#include "Video/Core/VideoMode.h"
#include "Core/Export.h"

#include <vector>
#include <memory>


namespace Fork
{

namespace Platform
{


/**
Enumerates all supported video modes and all available display devices.
\see Video::VideoMode
\see Video::DisplayDevice
*/
class FORK_EXPORT VideoModeEnumerator
{
    
    public:
        
        //! When this constructor is called, all video modes and display devices will be enumerated.
        VideoModeEnumerator();

        //! Returns the smallest video mode (smallest in resolution and color-bit-depth).
        Video::VideoMode FindSmallestVideoMode() const;
        //! Returns the largest video mode (largest in resolution and color-bit-depth).
        Video::VideoMode FindLargestVideoMode() const;

        //! Returns the video mode list.
        inline const std::vector<Video::VideoMode>& GetVideoModes() const
        {
            return videoModes_;
        }
        //! Returns the display device list.
        inline const std::vector<Video::DisplayDevice>& GetDisplayDevices() const
        {
            return displayDevices_;
        }

    private:
        
        std::vector<Video::VideoMode>       videoModes_;
        std::vector<Video::DisplayDevice>   displayDevices_;

};

typedef std::shared_ptr<VideoModeEnumerator> VideoModeEnumeratorPtr;


} // /namespace Platform

} // /namespace Fork


#endif



// ========================
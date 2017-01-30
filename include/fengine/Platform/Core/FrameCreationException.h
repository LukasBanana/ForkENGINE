/*
 * Frame creation exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FRAME_CREATION_EXCEPTION_H__
#define __FORK_FRAME_CREATION_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"
#include "Platform/Core/Frame.h"


namespace Fork
{


/**
Frame (also "Window") creation exception.
\see Platform::Frame
\ingroup fork_exception
*/
class FORK_EXPORT FrameCreationException : public DefaultException
{
    
    public:
        
        FrameCreationException(const Platform::FrameDescription& frameDesc, const std::string& errorDesc) :
            DefaultException
            {
                "FrameCreationException",
                "Description: " + errorDesc
            },
            frameDesc_(frameDesc)
        {
        }

        //! Returns the frame description.
        inline const Platform::FrameDescription& GetFrameDesc() const
        {
            return frameDesc_;
        }

    private:
        
        Platform::FrameDescription frameDesc_;

};


} // /namespace Fork


#endif



// ========================
/*
 * Render system flags header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDER_SYSTEM_FLAGS_H__
#define __FORK_RENDER_SYSTEM_FLAGS_H__


namespace Fork
{

namespace Video
{


/**
Flags enumeration for clear buffers.
\see RenderSystem::ClearBuffers
*/
struct ClearBuffersFlags
{
    typedef unsigned char DataType;
    enum : DataType
    {
        Color       = (1 << 0),         //!< Clear color buffer.
        Depth       = (1 << 1),         //!< Clear depth buffer.
        Stencil     = (1 << 2),         //!< Clear stencil buffer.

        FrameBuffer = (Color | Depth),  //!< Clear color- and depth buffer.
        All         = 0x07,             //!< Clear all buffers.
    };
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================
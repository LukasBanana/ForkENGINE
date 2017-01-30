/*
 * Render attributes enumeration header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDER_ATTRIBUTES_H__
#define __FORK_RENDER_ATTRIBUTES_H__


namespace Fork
{

namespace Video
{


/**
Render state attributes enumeration.
All these render states can be stored temporarily in a render context.
\see RenderContext::PushRenderAttribute
*/
enum class RenderAttributes
{
    Viewport,   //!< Viewport state. \see RenderContext::SetupViewport
    Scissor,    //!< Scissor state. \see RenderContext::SetupScissor
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================
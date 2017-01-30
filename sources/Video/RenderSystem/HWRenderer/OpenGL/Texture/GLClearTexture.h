/*
 * OpenGL clear texture header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_CLEAR_TEXTURE_H__
#define __FORK_GL_CLEAR_TEXTURE_H__


#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{


class GLTexture1D;
class GLTexture2D;
class GLTexture3D;
class GLTextureCube;

/*
These functions are used to clear (or initialize) the image data of OpenGL textures.
If the "GL_ARB_clear_texture" GL extension is available, native texture clearing will be performed.
Otherwise manual initialization (using "glTexImage") is done.
*/

void GLClearTexture1D(GLTexture1D* textureGL, const Video::ColorRGBAub& clearColor); //!< \todo Not yet implemented
void GLClearTexture2D(GLTexture2D* textureGL, const Video::ColorRGBAub& clearColor);
void GLClearTexture3D(GLTexture3D* textureGL, const Video::ColorRGBAub& clearColor); //!< \todo Not yet implemented
void GLClearTextureCube(GLTextureCube* textureGL, const Video::ColorRGBAub& clearColor); //!< \todo Not yet implemented


} // /namespace Video

} // /namespace Fork


#endif



// ========================
/*
 * OpenGL clear texture file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLClearTexture.h"
#include "GLTexture1D.h"
#include "GLTexture2D.h"
#include "GLTexture3D.h"
#include "GLTextureCube.h"
#include "../GLExtensions.h"
#include "../GLCore.h"
#include "Core/Exception/NotYetImplementedException.h"

#include <vector>


namespace Fork
{

namespace Video
{


static bool GLClearTextureNative(GLuint texID, const Video::ColorRGBAub& clearColor)
{
    if (glClearTexImage)
    {
        /* "glClearTexImage" GL extension supported -> clear texture with native function */
        glClearTexImage(texID, 0, GL_RGBA, GL_UNSIGNED_BYTE, clearColor.Ptr());
        return true;
    }
    return false;
}

void GLClearTexture1D(GLTexture1D* textureGL, const Video::ColorRGBAub& clearColor)
{
    if (GLClearTextureNative(textureGL->GetTextureID(), clearColor))
        return;
    
    #ifdef FORK_ENABLE_EXCEPTION_NOTYETIMPLEMENTED
    throw NotYetImplementedException(__FUNCTION__, "Only native GL texture clear has been implemented yet");
    #endif
    //...
}

void GLClearTexture2D(GLTexture2D* textureGL, const Video::ColorRGBAub& clearColor)
{
    if (GLClearTextureNative(textureGL->GetTextureID(), clearColor))
        return;
    
    /* Setup initial image data */
    auto size = textureGL->GetSize();

    const size_t imageDataSize = size.Area();
    std::vector<Video::ColorRGBAub> imageData(imageDataSize);

    for (auto& color : imageData)
        color = clearColor;

    /* Initialize texture data */
    if (textureGL->HasArray())
    {
        FLUSH_GL_ERROR;

        for (int z = 0, maxZ = static_cast<int>(textureGL->GetArraySize()); z < maxZ; ++z)
        {
            glTexSubImage3D(
                textureGL->GetTarget(),
                0,
                0, 0, z,
                size.width, size.height, 1,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                imageData.data()
            );
        }

        SHOW_GL_ERROR("ClearTexture2D/glTexSubImage3D");
    }
    else
    {
        FLUSH_GL_ERROR;

        glTexSubImage2D(
            textureGL->GetTarget(),
            0,
            0, 0,
            size.width, size.height,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            imageData.data()
        );

        SHOW_GL_ERROR("ClearTexture2D/glTexSubImage2D");
    }
}

void GLClearTexture3D(GLTexture3D* textureGL, const Video::ColorRGBAub& clearColor)
{
    if (GLClearTextureNative(textureGL->GetTextureID(), clearColor))
        return;
    
    #ifdef FORK_ENABLE_EXCEPTION_NOTYETIMPLEMENTED
    throw NotYetImplementedException(__FUNCTION__, "Only native GL texture clear has been implemented yet");
    #endif
    //...
}

void GLClearTextureCube(GLTextureCube* textureGL, const Video::ColorRGBAub& clearColor)
{
    if (GLClearTextureNative(textureGL->GetTextureID(), clearColor))
        return;
    
    #ifdef FORK_ENABLE_EXCEPTION_NOTYETIMPLEMENTED
    throw NotYetImplementedException(__FUNCTION__, "Only native GL texture clear has been implemented yet");
    #endif
    //...
}


} // /namespace Video

} // /namespace Fork



// ========================
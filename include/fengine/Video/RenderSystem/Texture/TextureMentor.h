/*
 * Texture mentor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TEXTURE_MENTOR_H__
#define __FORK_TEXTURE_MENTOR_H__


#include "Core/Export.h"
#include "Video/RenderSystem/Texture/Texture.h"


namespace Fork
{

namespace Video
{

namespace TextureMentor
{


/**
Queries the description from the specified texture object.
\param[in] texture Constant raw-pointer to the texture object whose description is to be queried.
\return Texture description denpending on its texture sub class type (e.g. Texture2D or TextureCube).
\throws NullPointerException If 'texture' is null.
*/
FORK_EXPORT Texture::Description QueryTextureDesc(const Texture* texture);


} // /namespace TextureMentor

} // /namespace Video

} // /namespace Fork


#endif



// ========================
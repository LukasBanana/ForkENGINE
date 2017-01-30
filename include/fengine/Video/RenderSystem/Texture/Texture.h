/*
 * Texture header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TEXTURE_H__
#define __FORK_TEXTURE_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "Core/TreeHierarchy/NodeMetaData.h"
#include "Video/RenderSystem/Texture/TextureAttributes.h"
#include "Math/Core/Size3.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(Texture);

/**
Texture base interface.
\ingroup texture_classes
*/
class FORK_EXPORT Texture
{
    
    public:
        
        /* === Structures === */

        /**
        Texture description structure.
        \remarks This is only used for creating a texture. The base class "Texture"
        does not has any information about the texture size for instance.
        To query a texture description from an existing texture object use "TextureMentor::QueryTextureDesc".
        \see TextureMentor::QueryTextureDesc
        */
        struct Description
        {
            TextureFormats  format      = TextureFormats::RGBA; //!< Texture format. By default RGBA.
            Math::Size3i    size;                               //!< Texture size.
            unsigned int    arraySize   = 0;                    //!< Texture array size. By default 0.
            bool            hasMIPMaps  = true;                 //!< Specifies whether MIP-maps are used or not. By default true.
        };

        /* === Enumerations === */

        //! Texture types enumeration.
        enum class Types
        {
            Tex1D,      //!< 1-dimensional texture and 1-dimensional texture array.
            Tex2D,      //!< 2-dimensional texture and 2-dimensional texture array.
            Tex3D,      //!< 3-dimensional texture.
            TexCube,    //!< cube texture and cube texture array.
        };

        /* === Functions === */

        virtual ~Texture()
        {
        }

        Texture(const Texture&) = delete;
        Texture& operator = (const Texture&) = delete;
        
        //! Returns the type of this texture object.
        virtual Types Type() const = 0;

        //! Returns the hardware texture format.
        inline TextureFormats GetFormat() const
        {
            return format;
        }

        //! Returns true if this texture has MIP-maps. By default false.
        inline bool HasMIPMaps() const
        {
            return hasMIPMaps;
        }

        //! Texture meta data.
        NodeMetaData metaData;

    protected:
        
        friend class RenderSystem;
        friend class RenderContext;

        Texture() = default;

        //! Internal hardware texture format.
        TextureFormats  format      = TextureFormats::RGBA;
        bool            hasMIPMaps  = false;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================
/*
 * Texture font header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TEXTURE_FONT_H__
#define __FORK_TEXTURE_FONT_H__


#include "Video/Font/Font.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/HardwareBuffer/VertexBuffer.h"
#include "Core/Container/PartitionContainer.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(TextureFont);

//! Texture font class.
class FORK_EXPORT TextureFont : public Font
{
    
    public:
        
        typedef PartitionContainer<const Font*> PartitionContianerType;

        TextureFont(const Font::Description& desc, const FontGlyphSet& glyphSet, const Texture2DPtr& texture);

        //! Returns Types::Texture.
        Types Type() const override;

        //! Returns a constant raw-pointer to the texture object.
        inline const Texture2D* GetTexture() const
        {
            return texture_.get();
        }

        //! Returns the partition to be used inside an buffer atlas.
        inline const PartitionContianerType::Partition& GetPartition() const
        {
            return partition_;
        }

    private:
        
        friend class PrimitiveRenderer;

        Texture2DPtr texture_;
        PartitionContianerType::Partition partition_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================
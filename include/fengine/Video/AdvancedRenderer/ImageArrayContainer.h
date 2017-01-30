/*
 * Image array container header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_ARRAY_CONTAINER_H__
#define __FORK_IMAGE_ARRAY_CONTAINER_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(ImageArrayContainer);

/**
The image array container can be used as base class for image array managers.
The image array entries are used to draw many 2D images efficiently with the 'PrimitiveRenderer'.
\see PrimitiveRenderer
\see PrimitiveRenderer::ImageArrayEntry
\see PrimitiveRenderer::DrawImageArray
*/
class FORK_EXPORT ImageArrayContainer
{
    public:

        virtual ~ImageArrayContainer();

        /**
        Sets the image array entries and draw the image array with the specified primitive renderer.
        \see PrimitiveRenderer::SetupImageArray
        \see PrimitiveRenderer::DrawImageArray
        */
        virtual void SetupAndDrawImageArray(
            PrimitiveRenderer& renderer, const Math::Point2f& offsetPosition,
            const Math::Size2f& baseSize, const Video::ColorRGBAf& baseColor = {}
        );

        /**
        Returns a constant raw-pointer to the image array entry at the
        specified index or null if there is no entry at this index.
        */
        const PrimitiveRenderer::ImageArrayEntry* ImageEntry(size_t index) const;

        //! Returns a raw-pointer to the 2D texture object.
        inline Texture2D* GetTexture() const
        {
            return texture_.get();
        }

        //! Returns the image entry list.
        inline const std::vector<PrimitiveRenderer::ImageArrayEntry>& GetImageEntries() const
        {
            return imageEntries_;
        }

    protected:

        ImageArrayContainer() = default;

        Video::Texture2DPtr                             texture_;
        std::vector<PrimitiveRenderer::ImageArrayEntry> imageEntries_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================
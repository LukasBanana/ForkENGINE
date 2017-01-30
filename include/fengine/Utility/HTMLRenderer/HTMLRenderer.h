/*
 * HTML renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_HTML_RENDERER_H__
#define __FORK_UTILITY_HTML_RENDERER_H__


#include "Core/Export.h"
#include "Utility/HTMLRenderer/HTMLTags.h"
#include "Utility/HTMLRenderer/HTMLDocument.h"
#include "Video/AdvancedRenderer/RichTextRenderer.h"
#include "Math/Core/Size2.h"
#include "Math/Core/Vector2.h"

#include <memory>


namespace Fork
{

namespace Utility
{


//! HTML document renderer.
class FORK_EXPORT HTMLRenderer
{
    
    public:
        
        HTMLRenderer(const Video::PrimitiveRendererPtr& renderer);
        virtual ~HTMLRenderer();

        /**
        Draws the entire HTML document to the current frame buffer.
        \param[in] document Specifies the HTML document which is to be drawn.
        \param[in] viewArea Specifies the area where the document is to be drawn. This is similiar to the renderer viewport.
        \param[in] viewOffset Specifies an optional offset for the document. This is similiar to the document scroll offset. By default { 0, 0 }.
        \param[in] border Specifies an optional document border. By default { 0, 0 }.
        */
        void DrawDocument(
            const HTMLDocument& document, const Math::Rectf& viewArea,
            const Math::Point2f& viewOffset = {}, const Math::Size2f& border = {}
        );

    private:
        
        std::unique_ptr<Video::RichTextRenderer> textRenderer_;

        const HTMLDocument* document_ = nullptr;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================
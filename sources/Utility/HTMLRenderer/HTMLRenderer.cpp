/*
 * HTML renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/HTMLRenderer/HTMLRenderer.h"
#include "XMLVisitor.h"


namespace Fork
{

namespace Utility
{


HTMLRenderer::HTMLRenderer(const Video::PrimitiveRendererPtr& renderer) :
    textRenderer_{ std::make_unique<Video::RichTextRenderer>(renderer) }
{
}
HTMLRenderer::~HTMLRenderer()
{
}

void HTMLRenderer::DrawDocument(
    const HTMLDocument& document, const Math::Rectf& viewArea,
    const Math::Point2f& viewOffset, const Math::Size2f& border)
{
    /* Store constant reference to the current document */
    document_ = &document;

    /* Initialize illustrator */
    textRenderer_->viewOffset   = viewArea.LTPoint() + viewOffset;
    textRenderer_->viewSize     = viewArea.Size();
    textRenderer_->textPosition = { 0, 0 };
    textRenderer_->textBorder   = border;

    /* Visit the entire XML tree */
    HTML::XMLVisitor xmlVisitor(textRenderer_.get());

    for (auto tag : document.tags)
        tag->Visit(&xmlVisitor);
}


} // /namespace Utility

} // /namespace Fork



// ========================
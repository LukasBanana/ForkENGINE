/*
 * HTML renderer XML visitor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_HTML_RENDERER_XML_VISITOR_H__
#define __FORK_UTILITY_HTML_RENDERER_XML_VISITOR_H__


#include "Lang/AbstractSyntaxTrees/Visitor.h"
#include "Video/AdvancedRenderer/RichTextRenderer.h"


namespace Fork
{

namespace Utility
{

namespace HTML
{


//! XML visitor class for the HTML renderer.
class XMLVisitor : public Lang::AbstractSyntaxTrees::Visitor
{
        
    public:
        
        //! \throws NullPointerException If 'textRenderer' is null.
        XMLVisitor(Video::RichTextRenderer* textRenderer);

        typedef Lang::AbstractSyntaxTrees::XMLTag       XMLTag;
        typedef Lang::AbstractSyntaxTrees::XMLAttribute XMLAttribute;
        typedef Lang::AbstractSyntaxTrees::XMLContent   XMLContent;

        void VisitXMLTag        (XMLTag*        node, void* args) override;
        void VisitXMLAttribute  (XMLAttribute*  node, void* args) override;
        void VisitXMLContent    (XMLContent*    node, void* args) override;

    private:
        
        Video::RichTextRenderer* textRenderer_ = nullptr;

};


} // /namespace HTML

} // /namespace Utility

} // /namespace Fork


#endif



// ========================
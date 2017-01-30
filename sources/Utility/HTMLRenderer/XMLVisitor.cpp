/*
 * HTML renderer XML visitor file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XMLVisitor.h"
#include "Lang/XMLParser/XMLTag.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/StringModifier.h"

#include "Core/Exception/IndexOutOfBoundsException.h"


namespace Fork
{

namespace Utility
{

namespace HTML
{


XMLVisitor::XMLVisitor(Video::RichTextRenderer* textRenderer) :
    textRenderer_{ textRenderer }
{
    ASSERT_POINTER(textRenderer);
}

void XMLVisitor::VisitXMLTag(XMLTag* node, void* args)
{
    /* Push attributes */
    auto ident = ToLower(node->ident);

    bool hasPushedFont = false;
    bool hasPushedColor = false;
    unsigned int newLines = 0;

    if (ident == "i")
    {
        textRenderer_->PushFontItalic();
        hasPushedFont = true;
    }
    else if (ident == "b")
    {
        textRenderer_->PushFontBold();
        hasPushedFont = true;
    }
    else if (ident == "br")
    {
        newLines = 1;
    }
    else if (ident == "head")
    {
        /* Ignore the HTML head */
        return;
    }
    else if (ident == "font")
    {
        auto fontDesc = textRenderer_->CurrentFont()->GetDesc();

        for (auto attrib : node->attributes)
        {
            auto attrIdent = ToLower(attrib->ident);

            if (attrIdent == "face")
                fontDesc.name = UTF8toUTF16(attrib->value);
            else if (attrIdent == "size")
                fontDesc.size.height = StrToNum<int>(attrib->value)*6;
        }

        textRenderer_->PushFont(fontDesc);
        hasPushedFont = true;
    }
    else
    {
        static const char* captionTags[] = { "h1", "h2", "h3", "h4", "h5", "h6" };
        static const int captionSizes[] = { 50, 43, 36, 29, 22, 15 };

        for (int i = 0; i < 6; ++i)
        {
            if (ident == captionTags[i])
            {
                textRenderer_->PushFontSize(captionSizes[i]);
                hasPushedFont = true;
                newLines = 2;
                break;
            }
        }
    }

    /* Visit child nodes */
    for (auto node : node->nodes)
        node->Visit(this);

    for (; newLines > 0; --newLines)
        textRenderer_->NewLine();

    /* Pop attributes */
    if (hasPushedFont)
        textRenderer_->PopFont();
    if (hasPushedColor)
        textRenderer_->PopColor();
}

void XMLVisitor::VisitXMLAttribute(XMLAttribute* node, void* args)
{
    //...
}

void XMLVisitor::VisitXMLContent(XMLContent* node, void* args)
{
    try
    {
        textRenderer_->Text(UTF8toUTF16(node->value + " "));
    }
    catch (const IndexOutOfBoundsException&)// err)
    {
        //int x = 0;
    }
}


} // /namespace HTML

} // /namespace Utility

} // /namespace Fork



// ========================
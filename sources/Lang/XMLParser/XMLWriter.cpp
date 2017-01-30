/*
 * XML writer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Lang/XMLParser/XMLWriter.h"


namespace Fork
{

namespace Lang
{

namespace XMLWriter
{


using namespace AbstractSyntaxTrees;

/* --- Internal functions --- */

static std::string indent;

static void WriteTag(std::ostream& stream, const XMLTag* tag);

static void WriteAttribute(std::ostream& stream, const XMLAttribute* attrib)
{
    stream << attrib->ident << "=\"" << attrib->value << "\"";
}

static void WriteContent(std::ostream& stream, const XMLContent* content)
{
    auto value = content->value;

    /* Replace special characters with keywords */
    auto Replace = [&value](size_t& pos, const std::string& replace)
    {
        value.erase(value.begin() + pos);
        value.insert(pos, replace);
        pos += replace.size();
    };

    for (size_t i = 0; i < value.size(); ++i)
    {
        switch (value[i])
        {
            case  '<': Replace(i, "&lt;"  ); break;
            case  '>': Replace(i, "&gt;"  ); break;
            case  '&': Replace(i, "&amp;" ); break;
            case '\"': Replace(i, "&quot;"); break;
            case '\'': Replace(i, "&apos;"); break;
            case  ' ': Replace(i, "&nbsp;"); break;
        }
    }

    /* Write content to stream */
    stream << value << ' ';
}

static void WriteAttributeList(std::ostream& stream, const std::vector<XMLAttributePtr>& attributes)
{
    for (const auto& attrib : attributes)
    {
        stream << " ";
        WriteAttribute(stream, attrib.get());
    }
}

static void WriteSingleLineTag(std::ostream& stream, const XMLTag* tag)
{
    stream << indent << "<" << tag->ident;
    WriteAttributeList(stream, tag->attributes);
    stream << "/>";
}

static void WriteNode(std::ostream& stream, const XMLNode* node)
{
    switch (node->Type())
    {
        case XMLNode::Types::Content:
            WriteContent(stream, dynamic_cast<const XMLContent*>(node));
            break;
        case XMLNode::Types::Tag:
            WriteTag(stream, dynamic_cast<const XMLTag*>(node));
            break;
    }
}

static void WriteMultiLineTag(std::ostream& stream, const XMLTag* tag)
{
    /* Write tag header */
    stream << indent << "<" << tag->ident;
    WriteAttributeList(stream, tag->attributes);
    stream << ">" << std::endl;

    /* Push indentation */
    indent += std::string(4, ' ');

    /* Write tag body */
    for (const auto& node : tag->nodes)
        WriteNode(stream, node.get());

    stream << std::endl;

    /* Pop indentation */
    if (indent.size() >= 4)
        indent.resize(indent.size() - 4);
    else
        indent.clear();

    /* Write tag footprint */
    stream << indent << "</" << tag->ident << ">";
}

static void WriteTag(std::ostream& stream, const XMLTag* tag)
{
    if (tag->nodes.empty())
        WriteSingleLineTag(stream, tag);
    else
        WriteMultiLineTag(stream, tag);
}


/* --- Global functions --- */

FORK_EXPORT void WriteXMLTagsToFile(
    std::ostream& stream, const std::vector<XMLTagPtr>& xmlTags)
{
    for (const auto& tag : xmlTags)
    {
        WriteTag(stream, tag.get());
        stream << std::endl << std::endl;
    }
}


} // /namepsace XMLWriter

} // /namespace Lang

} // /namespace Fork



// ========================
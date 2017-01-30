/*
 * Vertex format file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/BufferFormat/VertexFormat.h"
#include "Video/BufferFormat/InvalidVertexFormatException.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Video
{


/*
 * Attribute class
 */

VertexFormat::Attribute::Attribute(
    const std::string& name, const RendererDataTypes dataType, unsigned int numComponents) :
        name            { name          },
        dataType_       { dataType      },
        numComponents_  { numComponents }
{
    if (numComponents < 1 || numComponents > 4)
    {
        throw InvalidVertexFormatException(
            nullptr,
            "Invalid number of components for vertex attribute (Must be 1, 2, 3 or 4 but given " + ToStr(numComponents) + ")"
        );
    }
}

size_t VertexFormat::Attribute::Size() const
{
    return GetDataTypeSize(GetDataType()) * GetNumComponents();
}


/*
 * VertexFormat class
 */

void VertexFormat::SetupAttributes(const std::vector<Attribute>& attributes)
{
    /* Store new attribute list */
    attributes_ = attributes;

    /* Compute new offsets and track increasing format size */
    formatSize_ = 0;

    for (auto& attr : attributes_)
    {
        attr.offset_ = formatSize_;
        formatSize_ += attr.Size();
    }
}

void VertexFormat::AddAttribute(const Attribute& attribute)
{
    /* Add new attribute entry */
    attributes_.push_back(attribute);

    /* Setup attribute's offset and track increasing format size */
    auto& attr = attributes_.back();

    attr.offset_ = formatSize_;
    formatSize_ += attr.Size();
}

void VertexFormat::ClearAttributes()
{
    /* Clear attribute list and format size information */
    attributes_.clear();
    formatSize_ = 0;
}


} // /namespace Video

} // /namespace Fork



// ========================
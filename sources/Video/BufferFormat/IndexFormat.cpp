/*
 * Index format file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/BufferFormat/IndexFormat.h"


namespace Fork
{

namespace Video
{


IndexFormat::IndexFormat(const RendererDataTypes dataType)
{
    ChangeDataType(dataType);
}

void IndexFormat::ChangeDataType(const RendererDataTypes dataType)
{
    switch (dataType)
    {
        case RendererDataTypes::UByte:
        case RendererDataTypes::UShort:
        case RendererDataTypes::UInt:
            dataType_ = dataType;
            break;

        default:
            throw InvalidRendererDataTypeException(
                dataType,
                "Valid hardware buffer index formats are UByte, UShort and UInt"
            );
            break;
    }
}


} // /namespace Video

} // /namespace Fork



// ========================
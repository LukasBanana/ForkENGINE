/*
 * Renderer data types file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/BufferFormat/RendererDataTypes.h"


namespace Fork
{

namespace Video
{


FORK_EXPORT size_t GetDataTypeSize(const RendererDataTypes dataType)
{
    switch (dataType)
    {
        case RendererDataTypes::Float:
            return 4;
        case RendererDataTypes::Double:
            return 8;
    
        case RendererDataTypes::Byte:
        case RendererDataTypes::UByte:
            return 1;

        case RendererDataTypes::Short:
        case RendererDataTypes::UShort:
            return 2;

        case RendererDataTypes::Int:
        case RendererDataTypes::UInt:
            return 4;
    }
    return 0;
}

FORK_EXPORT bool IsIntegerType(const RendererDataTypes dataType)
{
    return dataType >= RendererDataTypes::Byte;
}


} // /namespace Video

} // /namespace Fork



// ========================
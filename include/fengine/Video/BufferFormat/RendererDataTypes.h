/*
 * Renderer data types header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDERER_DATA_TYPES_H__
#define __FORK_RENDERER_DATA_TYPES_H__


#include "Core/Exception/DefaultException.h"
#include "Core/Export.h"


namespace Fork
{

namespace Video
{


//! Renderer data types enumeration.
enum class RendererDataTypes
{
    Float,  //!< 32-bit floating-point.
    Double, //!< 64-bit floating-point.
    
    Byte,   //!< 8-bit signed integer.
    UByte,  //!< 8-bit unsigned integer.

    Short,  //!< 16-bit signed integer.
    UShort, //!< 16-bit unsigned integer.

    Int,    //!< 32-bit signed integer.
    UInt,   //!< 32-bit unsigned integer.
};


/* --- Global "GetDataTypeSize" function --- */

/**
Returns the data type size (in bytes) of the specified renderer data type.
\see RendererDataTypes
*/
FORK_EXPORT size_t GetDataTypeSize(const RendererDataTypes dataType);

/**
Returns true if the specified type is an integer type.
\see RendererDataTypes
*/
FORK_EXPORT bool IsIntegerType(const RendererDataTypes dataType);


/* --- "GetDataType" template specializations --- */

/**
Returns the renderer data type for the specified template argument.
\tparam T Specifies the data type. This can only be:
- float
- double
- char
- unsigned char
- short
- unsigned short
- int
- unsigned int
For any other type a static assertion will occur.
*/
template <typename T> inline RendererDataTypes GetDataType()
{
    static_assert(false, "Unknown renderer data type");
    return RendererDataTypes::Byte;
}

//! Returns RendererDataTypes::Float.
template <> inline RendererDataTypes GetDataType<float>()
{
    return RendererDataTypes::Float;
}
//! Returns RendererDataTypes::Double.
template <> inline RendererDataTypes GetDataType<double>()
{
    return RendererDataTypes::Double;
}

//! Returns RendererDataTypes::Byte.
template <> inline RendererDataTypes GetDataType<char>()
{
    return RendererDataTypes::Byte;
}
//! Returns RendererDataTypes::UByte.
template <> inline RendererDataTypes GetDataType<unsigned char>()
{
    return RendererDataTypes::UByte;
}

//! Returns RendererDataTypes::Short.
template <> inline RendererDataTypes GetDataType<short>()
{
    return RendererDataTypes::Short;
}
//! Returns RendererDataTypes::UShort.
template <> inline RendererDataTypes GetDataType<unsigned short>()
{
    return RendererDataTypes::UShort;
}

//! Returns RendererDataTypes::Int.
template <> inline RendererDataTypes GetDataType<int>()
{
    return RendererDataTypes::Int;
}
//! Returns RendererDataTypes::UInt.
template <> inline RendererDataTypes GetDataType<unsigned int>()
{
    return RendererDataTypes::UInt;
}


} // /namespace Video


/**
Invalid renderer data type exception.
\see RendererDataTypes
\ingroup fork_exception
*/
class FORK_EXPORT InvalidRendererDataTypeException : public DefaultException
{
    
    public:
        
        InvalidRendererDataTypeException(const Video::RendererDataTypes dataType, const std::string& errorDesc) :
            DefaultException
            {
                "InvalidRendererDataTypeException",
                "Description: " + errorDesc
            },
            dataType_(dataType)
        {
        }

        inline Video::RendererDataTypes GetDataType() const
        {
            return dataType_;
        }

    private:
        
        Video::RendererDataTypes dataType_ = Video::RendererDataTypes::Byte;

};


} // /namespace Fork


#endif



// ========================
/*
 * Texture attributes header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TEXTURE_ATTRIBUTES_H__
#define __FORK_TEXTURE_ATTRIBUTES_H__


#include "Core/Export.h"
#include "Video/Image/ImageAttributes.h"
#include "Video/BufferFormat/RendererDataTypes.h"


namespace Fork
{

namespace Video
{


//! Hardware texture formats.
enum class TextureFormats
{
    /* --- Base internal formats --- */
    DepthComponent, //!< Base internal format: depth component.
    DepthStencil,   //!< Base internal format: depth- and stencil components.
    R,              //!< Base internal format: red component.
    RG,             //!< Base internal format: red and green components.
    RGB,            //!< Base internal format: red, green and blue components.
    RGBA,           //!< Base internal format: red, green, blue and alpha components.

    /* --- Sized internal formats --- */
    R8UInt,         //!< Sized internal format: red 8-bit unsigned integer component.
    R8SInt,         //!< Sized internal format: red 8-bit signed integer component.

    R16UInt,        //!< Sized internal format: red 16-bit unsigned interger component.
    R16SInt,        //!< Sized internal format: red 16-bit signed interger component.
    R16Float,       //!< Sized internal format: red 16-bit floating point component.

    R32UInt,        //!< Sized internal format: red 32-bit unsigned interger component.
    R32SInt,        //!< Sized internal format: red 32-bit signed interger component.
    R32Float,       //!< Sized internal format: red 32-bit floating point component.

    RG8UInt,        //!< Sized internal format: red, green 8-bit unsigned integer components.
    RG8SInt,        //!< Sized internal format: red, green 8-bit signed integer components.

    RG16UInt,       //!< Sized internal format: red, green 16-bit unsigned interger components.
    RG16SInt,       //!< Sized internal format: red, green 16-bit signed interger components.
    RG16Float,      //!< Sized internal format: red, green 16-bit floating point components.

    RG32UInt,       //!< Sized internal format: red, green 32-bit unsigned interger components.
    RG32SInt,       //!< Sized internal format: red, green 32-bit signed interger components.
    RG32Float,      //!< Sized internal format: red, green 32-bit floating point components.

    RGB8UInt,       //!< Sized internal format: red, green, blue 8-bit unsigned integer components.
    RGB8SInt,       //!< Sized internal format: red, green, blue 8-bit signed integer components.

    RGB16UInt,      //!< Sized internal format: red, green, blue 16-bit unsigned interger components.
    RGB16SInt,      //!< Sized internal format: red, green, blue 16-bit signed interger components.
    RGB16Float,     //!< Sized internal format: red, green, blue 16-bit floating point components.

    RGB32UInt,      //!< Sized internal format: red, green, blue 32-bit unsigned interger components.
    RGB32SInt,      //!< Sized internal format: red, green, blue 32-bit signed interger components.
    RGB32Float,     //!< Sized internal format: red, green, blue 32-bit floating point components.

    RGBA8UInt,      //!< Sized internal format: red, green, blue, alpha 8-bit unsigned integer components.
    RGBA8SInt,      //!< Sized internal format: red, green, blue, alpha 8-bit signed integer components.

    RGBA16UInt,     //!< Sized internal format: red, green, blue, alpha 16-bit unsigned interger components.
    RGBA16SInt,     //!< Sized internal format: red, green, blue, alpha 16-bit signed interger components.
    RGBA16Float,    //!< Sized internal format: red, green, blue, alpha 16-bit floating point components.

    RGBA32UInt,     //!< Sized internal format: red, green, blue, alpha 32-bit unsigned interger components.
    RGBA32SInt,     //!< Sized internal format: red, green, blue, alpha 32-bit signed interger components.
    RGBA32Float,    //!< Sized internal format: red, green, blue, alpha 32-bit floating point components.
};


/**
Returns the number of color components of the specified texture format.
\see TextureFormats
*/
FORK_EXPORT int NumColorComponents(const TextureFormats format);

/**
Returns the number of bits per color component of the specified texture format.
\code
// Examples:
NumColorComponentBits(TextureFormats::RG8SInt) = 8;
NumColorComponentBits(TextureFormats::RGBA16UInt) = 16;
NumColorComponentBits(TextureFormats::R32Float) = 32;
\endcode
*/
FORK_EXPORT int NumColorComponentBits(const TextureFormats format);

/**
Returns the base hardware texture format of the specified format.
\param[in] format Specifies the texture format from which the base internal format is to be determined.
\code
// Examples:
BaseTextureFormat(TextureFormats::R32UInt) == TextureFormats::R;
BaseTextureFormat(TextureFormats::RGB16SInt) == TextureFormats::RGB;
BaseTextureFormat(TextureFormats::RGBA32Float) == TextureFormats::RGBA;
\endcode
\see TextureFormats
*/
FORK_EXPORT TextureFormats BaseTextureFormat(const TextureFormats format);

/**
Returns a suitable texture format for the specified image color format and data type.
\return The choosen texture format. If no suitable texture format was found, the return value is TextureFormats::RGBA as default format.
\see TextureFormats
\see ImageColorFormats
\see RendererDataTypes
*/
FORK_EXPORT TextureFormats ChooseTextureFormat(const ImageColorFormats imageFormat, const RendererDataTypes dataType);

/**
Reduces the size to the specified MIP level size.
This value is always in the range [1 .. size].
\param[in] size Specifies the size of the first MIP level.
\param[in] mipLevel Specifies the MIP level whose size is to be computed.
\return Respective MIP level size. If 'mipLevel' is 0,
the return value is equal to 'size'. The minimal return value is 1.
*/
FORK_EXPORT int MIPLevelSize(int size, unsigned int mipLevel);


} // /namespace Video

} // /namespace Fork


#endif



// ========================
/*
 * Variant header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_VARIANT_H__
#define __FORK_IO_VARIANT_H__


#include "Core/Export.h"
#include "Math/Core/Vector4.h"
#include "Video/Core/ColorRGBA.h"

#include <string>


namespace Fork
{

namespace IO
{


/**
Value variant class.
\remarks This is mainly used in the editor but can also be used at other spots.
It provides an easy way for conversion between several base types, such as boolean, integer, floating-point, string.
But extended data types such as Math::Vector4 and Video::ColorRGBA are also provided.
\remarks Most parts in the engine use floating-point colors ('Video::ColorRGBf' and 'Video::ColorRGBAf'),
but this variant class uses unsigned byte colors ('Video::ColorRGBub' and 'Video::ColorRGBAub').
This is because the editor only supports 8-bit unsigned byte components for colors.
If you need 32-bit floating-point components for colors, use a 4D vector ('Math::Vector4f').
\note This class has many implicit conversion constructors.
\see Math::Vector3
\see Math::Vector4
\see Video::ColorRGB
\see Video::ColorRGBA
*/
class FORK_EXPORT Variant
{

    public:
        
        /* --- Constructors --- */

        Variant() = default;
        Variant(bool value);
        Variant(int value);
        Variant(unsigned int value);
        Variant(float value);
        Variant(const std::string& value);
        Variant(const Math::Vector3f& value);
        Variant(const Math::Vector4f& value);
        Variant(const Video::ColorRGBub& value);
        Variant(const Video::ColorRGBAub& value);
        virtual ~Variant();

        /* --- Write --- */

        //! Overwrites the variant with the specified boolean value.
        void Write(bool value);
        //! Overwrites the variant with the specified integer value.
        void Write(int value);
        //! Overwrites the variant with the specified unsigned integer value.
        void Write(unsigned int value);
        //! Overwrites the variant with the specified floating-point value.
        void Write(float value);
        //! Overwrites the variant with the specified string.
        void Write(const std::string& value);
        //! Overwrites the variant with the specified 3D vector.
        void Write(const Math::Vector3f& value);
        //! Overwrites the variant with the specified 4D vector.
        void Write(const Math::Vector4f& value);
        //! Overwrites the variant with the specified RGB color.
        void Write(const Video::ColorRGBub& value);
        //! Overwrites the variant with the specified RGBA color.
        void Write(const Video::ColorRGBAub& value);

        /* --- Value access functions --- */

        //! Returns the internal boolean value.
        inline bool GetBool() const
        {
            return valueBool;
        }
        //! Returns the internal integer value.
        inline int GetInt() const
        {
            return valueInt;
        }
        //! Returns the internal unsigned integer value.
        inline int GetUInt() const
        {
            return valueUInt;
        }
        //! Returns the internal floating-point value.
        inline float GetFloat() const
        {
            return valueFloat;
        }
        //! Returns the internal string.
        inline const std::string& GetString() const
        {
            return valueString;
        }
        //! Returns the internal 3D vector.
        inline Math::Vector3f GetVector3() const
        {
            return valueVector.Vec3();
        }
        //! Returns the internal 4D vector.
        inline const Math::Vector4f& GetVector4() const
        {
            return valueVector;
        }
        //! Returns the internal RGB color.
        inline Video::ColorRGBub GetColorRGB() const
        {
            return valueColor.ToRGB();
        }
        //! Returns the internal RGBA color.
        inline const Video::ColorRGBAub& GetColorRGBA() const
        {
            return valueColor;
        }

        /* --- Conversion operators --- */

        inline operator bool () const
        {
            return GetBool();
        }
        inline operator int () const
        {
            return GetInt();
        }
        inline operator unsigned int () const
        {
            return GetUInt();
        }
        inline operator float () const
        {
            return GetFloat();
        }
        inline operator std::string () const
        {
            return GetString();
        }
        inline operator Math::Vector4f () const
        {
            return GetVector4();
        }
        inline operator Video::ColorRGBAub () const
        {
            return GetColorRGBA();
        }

    protected:
        
        bool                valueBool   = false;
        int                 valueInt    = 0;
        unsigned int        valueUInt   = 0u;
        float               valueFloat  = 0.0f;
        std::string         valueString;
        Math::Vector4f      valueVector;
        Video::ColorRGBAub  valueColor;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================
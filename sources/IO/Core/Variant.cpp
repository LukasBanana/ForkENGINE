/*
 * Variant file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Core/Variant.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace IO
{


static inline std::string FltToConcatStr(float value)
{
    return ToStr(value) + ";";
}

Variant::Variant(bool value)
{
    Write(value);
}
Variant::Variant(int value)
{
    Write(value);
}
Variant::Variant(unsigned int value)
{
    Write(value);
}
Variant::Variant(float value)
{
    Write(value);
}
Variant::Variant(const std::string& value)
{
    Write(value);
}
Variant::Variant(const Math::Vector3f& value)
{
    Write(value);
}
Variant::Variant(const Math::Vector4f& value)
{
    Write(value);
}
Variant::Variant(const Video::ColorRGBub& value)
{
    Write(value);
}
Variant::Variant(const Video::ColorRGBAub& value)
{
    Write(value);
}
Variant::~Variant()
{
}

void Variant::Write(bool value)
{
    valueBool   = value;
    valueInt    = (value ? 1 : 0);
    valueUInt   = (value ? 1u : 0u);
    valueFloat  = (value ? 1.0f : 0.0f);
    valueString = (value ? "1" : "");
}

void Variant::Write(int value)
{
    valueBool   = (value != 0);
    valueInt    = value;
    valueUInt   = (value >= 0 ? static_cast<unsigned int>(value) : 0);
    valueFloat  = static_cast<float>(value);
    valueString = ToStr(value);
}

void Variant::Write(unsigned int value)
{
    valueBool   = (value != 0);
    valueUInt   = value;
    valueFloat  = static_cast<float>(value);
    valueString = ToStr(value);
}

void Variant::Write(float value)
{
    valueBool   = (value != 0.0f);
    valueInt    = static_cast<int>(value);
    valueFloat  = value;
    valueString = ToStr(value);
}

void Variant::Write(const std::string& value)
{
    valueBool   = !value.empty();
    valueInt    = StrToNum<int>(value);
    valueFloat  = StrToNum<float>(value);
    valueString = value;
}

void Variant::Write(const Math::Vector3f& value)
{
    valueVector = Math::Vector4f(value);
    valueString = FltToConcatStr(value.x);
    valueString += FltToConcatStr(value.y);
    valueString += FltToConcatStr(value.z);
    auto color = Video::ColorRGBf(value.x, value.y, value.z);
    valueColor = color.Cast<unsigned char>();
}

void Variant::Write(const Math::Vector4f& value)
{
    valueVector = value;
    valueString = FltToConcatStr(value.x);
    valueString += FltToConcatStr(value.y);
    valueString += FltToConcatStr(value.z);
    valueString += FltToConcatStr(value.w);
    auto color = Video::ColorRGBAf(value.x, value.y, value.z, value.w);
    valueColor = color.Cast<unsigned char>();
}

void Variant::Write(const Video::ColorRGBub& value)
{
    valueColor = value;
    valueString = FltToConcatStr(value.r);
    valueString += FltToConcatStr(value.g);
    valueString += FltToConcatStr(value.b);
    auto color = value.Cast<float>();
    valueVector = { color.r, color.b, color.b };
}

void Variant::Write(const Video::ColorRGBAub& value)
{
    valueColor = value;
    valueString = FltToConcatStr(value.r);
    valueString += FltToConcatStr(value.g);
    valueString += FltToConcatStr(value.b);
    valueString += FltToConcatStr(value.a);
    auto color = value.Cast<float>();
    valueVector = { color.r, color.b, color.b, color.a };
}


} // /namespace IO

} // /namespace Fork



// ========================
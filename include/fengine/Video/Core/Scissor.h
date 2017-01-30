/*
 * Scissor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCISSOR_H__
#define __FORK_SCISSOR_H__


#include "Math/Core/Vector2.h"
#include "Math/Core/Size2.h"
#include "Core/Export.h"


namespace Fork
{

namespace Video
{


/**
Scissor settings structure. This structure only stores the information about a scissor (-rectangle).
To update a graphics scissor, use the respective rendering class.
*/
struct FORK_EXPORT Scissor
{
    Scissor() = default;
    Scissor(const Math::Point2i& scPosition, const Math::Size2i& scSize);

    Math::Point2i   position;   //!< Left-top position of the scissor (in screen space).
    Math::Size2i    size;       //!< Size of the scissor (in screen space).
};

//! Compares the two scissors on equality.
FORK_EXPORT bool operator == (const Scissor& a, const Scissor& b);
//! Compares the two scissors on unequality. This is equivalent to: !(a == b).
FORK_EXPORT bool operator != (const Scissor& a, const Scissor& b);


} // /namespace Video

} // /namespace Fork


#endif



// ========================
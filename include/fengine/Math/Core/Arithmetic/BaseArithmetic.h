/*
 * Base arithmetic header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BASE_ARITHMETIC_H__
#define __FORK_BASE_ARITHMETIC_H__


#include "Core/Export.h"


namespace Fork
{

namespace Math
{


//! Rounds the given size to the nearest power of two value (e.g. 34 to 32 and 120 to 128).
FORK_EXPORT int RoundPow2(int size);


} // /namespace Math

} // /namespace Fork


#endif



// ========================
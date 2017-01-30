/*
 * Base arithmetic file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Math/Core/Arithmetic/BaseArithmetic.h"


namespace Fork
{

namespace Math
{


FORK_EXPORT int RoundPow2(int size)
{
    int result = 1;
    
    while (result < size)
        result <<= 1;
    
    if (result - size <= size - result/2)
        return result;
    
    return result/2;
}


} // /namespace Math

} // /namespace Fork



// ========================
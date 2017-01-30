/*
 * Bit arithmetic header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BIT_ARITHMETIC_H__
#define __FORK_BIT_ARITHMETIC_H__


#include "Core/Export.h"


namespace Fork
{

namespace Math
{


//! \todo Not yet implemented!
FORK_EXPORT bool GetBitR2L(unsigned int bitMask, int pos);
//! \todo Not yet implemented!
FORK_EXPORT void SetBitR2L(unsigned int& bitMask, int pos, bool enable);

//! \todo Not yet implemented!
FORK_EXPORT bool GetBitL2R(unsigned int bitMask, int pos);
//! \todo Not yet implemented!
FORK_EXPORT void SetBitL2R(unsigned int& bitMask, int pos, bool enable);

//! \todo Not yet implemented!
FORK_EXPORT bool GetBitR2L(unsigned char bitMask, int pos);
//! \todo Not yet implemented!
FORK_EXPORT void SetBitR2L(unsigned char& bitMask, int pos, bool enable);

//! \todo Not yet implemented!
FORK_EXPORT bool GetBitL2R(unsigned char bitMask, int pos);
//! \todo Not yet implemented!
FORK_EXPORT void SetBitL2R(unsigned char& bitMask, int pos, bool enable);


} // /namespace Math

} // /namespace Fork


#endif



// ========================
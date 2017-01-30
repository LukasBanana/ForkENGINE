/*
 * Renderer compare functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDERER_COMPARE_FUNCTIONS_H__
#define __FORK_RENDERER_COMPARE_FUNCTIONS_H__


namespace Fork
{

namespace Video
{


/**
Renderer compare functions. These functions are used to compare
depth values from the frame buffer, alpha values from texture samples and others.
*/
enum class CompareFunctions
{
    Never,          //!< Comparision succeeds never.
    Equal,          //!< Comparision succeeds when the two values are equal (New == Old).
    NotEqual,       //!< Comparision succeeds when the two values are not equal (New != Old).
    Less,           //!< Comparision succeeds when the new value is less than the old value (New < Old).
    LessEqual,      //!< Comparision succeeds when the new value is less than or equal to the old value (New <= Old).
    Greater,        //!< Comparision succeeds when the new value is greater than the old value (New > Old).
    GreaterEqual,   //!< Comparision succeeds when the new value is greater than or equal to the old value (New >= Old).
    Always,         //!< Comparision succeeds always.
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================
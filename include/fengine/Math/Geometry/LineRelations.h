/*
 * Line relation enumerations header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LINE_RELATIONS_H__
#define __FORK_LINE_RELATIONS_H__


namespace Fork
{

namespace Math
{


//! Relations between line and point.
enum class LinePointRelations
{
    Start,  //!< Point is at the line's start corner.
    End,    //!< Point is in the line's end corner.
    Onto,   //!< Point is onto the line.
};


} // /namespace Math

} // /namespace Fork


#endif



// ========================
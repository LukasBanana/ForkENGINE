/*
 * FKS string helper header
 * 
 * This file is part of the "FKS-FileHandler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FKS_STRING_HELPER_H__
#define __FKS_STRING_HELPER_H__


#include <string>
#include <sstream>


namespace FKS
{


template <typename T> inline std::string ToStr(const T &val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}


} // /namespace FKS


#endif



// ========================
/*
 * Node meta data header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NODE_META_DATA_H__
#define __FORK_NODE_META_DATA_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{


//! Meta data structure for any kind of nodes.
struct FORK_EXPORT NodeMetaData
{
    std::string     name;                       //!< Name of the node.
    void*           userData        = nullptr;  //!< User data. Can be used to link this ndoe to any kind of data. By default null.
    unsigned int    userDataFlags   = 0;        //!< General purpose flags to describe what the user data is used for. By default 0.
};


} // /namespace Fork


#endif



// ========================
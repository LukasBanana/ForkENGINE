/*
 * Vendor IDs header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VIDEO_VENDOR_IDS_H__
#define __FORK_VIDEO_VENDOR_IDS_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace Video
{


//! Hardware vendor IDs.
struct VendorIDs
{
    typedef unsigned int DataType;
    enum : DataType
    {
        AMD     = 0x1002, //!< AMD Inc.
        NVIDIA  = 0x10de, //!< NVIDIA Corporation.
        Matrox  = 0x102b, //!< Matrox Electronic Systems Ltd.
        S3GC    = 0x5333, //!< S3 Graphics Co., Ltd.
        Intel   = 0x8086, //!< Intel Corporation.
        Oracle  = 0x80ee, //!< Oracle VirtualBox Graphics Adapter.
    };
};


/**
Returns the name of the graphics hardware vendor by the specified ID.
\see VendorIDs
*/
FORK_EXPORT std::string GetVendorNameByID(const VendorIDs::DataType id);


} // /namespace Video

} // /namespace Fork


#endif



// ========================
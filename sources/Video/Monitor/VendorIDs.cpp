/*
 * Vendor IDs file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Monitor/VendorIDs.h"


namespace Fork
{

namespace Video
{


FORK_EXPORT std::string GetVendorNameByID(const VendorIDs::DataType id)
{
    switch (id)
    {
        case VendorIDs::AMD:
            return "Advanced Micro Devices, Inc.";
        case VendorIDs::NVIDIA:
            return "NVIDIA Corporation";
        case VendorIDs::Matrox:
            return "Matrox Electronic Systems Ltd.";
        case VendorIDs::S3GC:
            return "S3 Graphics Co., Ltd.";
        case VendorIDs::Intel:
            return "Intel Corporation";
        case VendorIDs::Oracle:
            return "Oracle";
        default:
            break;
    }
    return "";
}


} // /namespace Video

} // /namespace Fork



// ========================
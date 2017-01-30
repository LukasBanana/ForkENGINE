/*
 * Network adapter file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/Core/Adapter.h"
#include "Network/Core/NetworkImport.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Network
{


Adapter::Adapter(
    const Types type, const std::string& desc,
    const std::string& address, const std::string& mask, bool isEnabled) :
        type_       { type      },
        desc_       { desc      },
        address_    { address   },
        mask_       { mask      },
        isEnabled_  { isEnabled }
{
}

std::string Adapter::BroadcastAddress() const
{
    /* Get address and mask from strings */
    auto addr = inet_addr(GetAddress().c_str());
    auto mask = inet_addr(GetMask().c_str());

    auto addrSub = reinterpret_cast<unsigned char*>(&addr);
    auto maskSub = reinterpret_cast<unsigned char*>(&mask);

    /* Get broad cast IP address */
    std::string broadcastAddr;

    broadcastAddr += (maskSub[0] > 0 ? Fork::ToStr(static_cast<int>(addrSub[0])) : "255");
    broadcastAddr += ".";
    broadcastAddr += (maskSub[1] > 0 ? Fork::ToStr(static_cast<int>(addrSub[1])) : "255");
    broadcastAddr += ".";
    broadcastAddr += (maskSub[2] > 0 ? Fork::ToStr(static_cast<int>(addrSub[2])) : "255");
    broadcastAddr += ".";
    broadcastAddr += (maskSub[3] > 0 ? Fork::ToStr(static_cast<int>(addrSub[3])) : "255");

    return broadcastAddr;
}

std::string Adapter::ToStr(Types type)
{
    switch (type)
    {
        case Types::__Unknown__:    return "Unknown";
        case Types::Ethernet:       return "Ethernet";
        case Types::TokenRing:      return "Token Ring (IEEE 802.5)";
        case Types::FDDI:           return "Fiber Distributed Data Interface (FDDI)";
        case Types::PPP:            return "Point-to-Point Protocol (PPP)";
        case Types::LoopBack:       return "Loop Back Protocol";
        case Types::SLIP:           return "Serial Line Internet Protocol (SLIP)";
    }
    return "";
}


} // /namespace Network

} // /namespace Fork



// ========================
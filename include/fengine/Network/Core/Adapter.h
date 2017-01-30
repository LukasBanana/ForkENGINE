/*
 * Network adapter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_ADAPTER_H__
#define __FORK_NETWORK_ADAPTER_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace Network
{


//! Network adapter information.
class FORK_EXPORT Adapter
{
    
    public:

        //! Network adapter types.
        enum class Types
        {
            __Unknown__,
            Ethernet,   //!< Ethernet (e.g. for LAN connections).
            TokenRing,  //!< Token Ring (IEEE 802.5).
            FDDI,       //!< Fiber Distributed Data Interface.
            PPP,        //!< Point-to-Point Protocol.
            LoopBack,   //!< Loop Back Protocol.
            SLIP,       //!< Serial Line Internet Protocol.
        };

        Adapter(
            const Types type, const std::string& desc,
            const std::string& address, const std::string& mask, bool isEnabled
        );

        /**
        Returns a broadcast address for the adapter's address,
        e.g. when 'GetAddress' returns "192.168.2.103" and 'GetMask' returns "255.255.255.0"
        the return value of this function is "192.168.2.255".
        */
        std::string BroadcastAddress() const;

        //! Returns the specified adapter type a string.
        static std::string ToStr(Types type);

        //! Returns the adapter type.
        inline Types GetType() const
        {
            return type_;
        }

        //! Returns the adapter description.
        inline const std::string& GetDesc() const
        {
            return desc_;
        }
        //! Returns the adapter address.
        inline const std::string& GetAddress() const
        {
            return address_;
        }
        //! Returns the adapter sub-net mask.
        inline const std::string& GetMask() const
        {
            return mask_;
        }

        //! Returns true if this adapter is enabled.
        inline bool IsEnabled() const
        {
            return isEnabled_;
        }

    private:
        
        Types       type_       = Types::__Unknown__;   //!< Network adapter type.
        std::string desc_;                              //!< Description of the hardware/software network adapter.
        std::string address_;                           //!< IP address (e.g. "192.168.0.1").
        std::string mask_;                              //!< IP address mask (e.g. "255.255.255.0").
        bool        isEnabled_  = false;                //!< Specifies whether it's enabled or disbaled.

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================
///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2016 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <aware/bonjour/error.hpp>
#include "detail/dns_sd.hpp"

namespace aware
{
namespace bonjour
{
namespace error
{

class bonjour_category : public boost::system::error_category
{
public:
    const char *name() const BOOST_SYSTEM_NOEXCEPT
    {
        return "bonjour";
    }
    std::string message(int value) const
    {
        switch (DNSServiceErrorType(value))
        {
        case kDNSServiceErr_NoError:
            return "success";
        case kDNSServiceErr_Unknown:
            return "unknown";
        case kDNSServiceErr_NoSuchName:
            return "no such name";
        case kDNSServiceErr_NoMemory:
            return "no memory";
        case kDNSServiceErr_BadParam:
            return "bad parameter";
        case kDNSServiceErr_BadReference:
            return "bad reference";
        case kDNSServiceErr_BadState:
            return "bad state";
        case kDNSServiceErr_BadFlags:
            return "bad flags";
        case kDNSServiceErr_Unsupported:
            return "unsupported";
        case kDNSServiceErr_NotInitialized:
            return "not initialized";
        case kDNSServiceErr_AlreadyRegistered:
            return "already registered";
        case kDNSServiceErr_NameConflict:
            return "name conflict";
        case kDNSServiceErr_Invalid:
            return "invalid";
        case kDNSServiceErr_Incompatible:
            return "incompatible";
        case kDNSServiceErr_BadInterfaceIndex:
            return "bad interface index";
        case kDNSServiceErr_NoSuchRecord:
            return "no such record";
        case kDNSServiceErr_NoAuth:
            return "no authentication";
        case kDNSServiceErr_NoSuchKey:
            return "no such key";
        case kDNSServiceErr_BadTime:
            return "bad time";
        case kDNSServiceErr_BadSig:
            return "bad signature";
        case kDNSServiceErr_BadKey:
            return "bad key";
        case kDNSServiceErr_Transient:
            return "transient";
        case kDNSServiceErr_ServiceNotRunning:
            return "service not running";
        case kDNSServiceErr_NoRouter:
            return "no router";
        case kDNSServiceErr_Firewall:
            return "firewall";
        case kDNSServiceErr_Refused:
            return "refused";
        case kDNSServiceErr_NATTraversal:
            return "NAT traversal";
        case kDNSServiceErr_DoubleNAT:
            return "double NAT";
        case kDNSServiceErr_NATPortMappingUnsupported:
            return "NAT port mapping unsupported";
        case kDNSServiceErr_NATPortMappingDisabled:
            return "NAT port mapping disabled";
        case kDNSServiceErr_PollingMode:
            return "polling mode";
        }
        return "unknown";
    }
};

const boost::system::error_category& category()
{
    static bonjour_category instance;
    return instance;
}

} // namespace error
} // namespace bonjour
} // namespace aware

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

#include <boost/asio/ip/address.hpp>
#include <boost/asio/detail/socket_ops.hpp> // network_to_host_short
#include <aware/detail/utility.hpp>
#include <aware/bonjour/error.hpp>
#include <aware/bonjour/detail/throw_on_error.hpp>
#include <aware/bonjour/detail/resolver.hpp>
#include "dns_sd.hpp"

namespace aware
{
namespace bonjour
{
namespace detail
{

boost::asio::ip::address to_address(const sockaddr& addr)
{
    using namespace boost::asio;
    switch (addr.sa_family)
    {
    case AF_INET:
        return ip::address_v4(reinterpret_cast<const ip::address_v4::bytes_type&>(reinterpret_cast<const sockaddr_in&>(addr).sin_addr));

    case AF_INET6:
        return ip::address_v6(reinterpret_cast<const ip::address_v6::bytes_type&>(reinterpret_cast<const sockaddr_in6&>(addr).sin6_addr));

    default:
        assert(false);
        break;
    }
}

//-----------------------------------------------------------------------------
// Callback
//-----------------------------------------------------------------------------

struct resolver::callback
{
    static void on_resolved(::DNSServiceRef ref,
                            ::DNSServiceFlags flags,
                            uint32_t interface_index,
                            ::DNSServiceErrorType error,
                            const char *fullname, // name._type._tcp.local
                            const char *host, // host.local
                            uint16_t port,
                            uint16_t txt_length,
                            const unsigned char *txt_record,
                            void *context)
    {
        resolver *self = static_cast<resolver *>(context);
        assert(self);

        try
        {
            if (error == kDNSServiceErr_NoError)
            {
                // Properties
                aware::contact::property_map_type properties;
                const uint16_t keyLength = 256; // See dns_sd.h
                char key[keyLength];
                uint8_t valueLength;
                const void *value;
                uint16_t count = ::TXTRecordGetCount(txt_length, txt_record);
                for (uint16_t current = 0; current < count; ++current)
                {
                    if (::TXTRecordGetItemAtIndex(txt_length, txt_record,
                                                  current,
                                                  keyLength, key,
                                                  &valueLength, &value) == kDNSServiceErr_NoError)
                    {
                        properties[key] = std::string(static_cast<const char *>(value),
                                                      valueLength);
                    }
                }

                self->contact.index(to_index(interface_index));
                self->contact.properties(properties);
                self->port = boost::asio::detail::socket_ops::network_to_host_short(port);
                self->on_resolved(host);
            }
            else
            {
                self->listener.on_resolver_failure(bonjour::error::make_error_code(error));
            }
        }
        catch (const boost::system::system_error& ex)
        {
            self->listener.on_resolver_failure(ex.code());
        }
        catch (...)
        {
            // Ignore unknown exceptions
        }
    }

    static void on_addrinfo(::DNSServiceRef ref,
                            ::DNSServiceFlags flags,
                            uint32_t interface_index,
                            ::DNSServiceErrorType error,
                            const char *host,
                            const struct sockaddr *address,
                            uint32_t ttl,
                            void *context)
    {
        resolver *self = static_cast<resolver *>(context);
        assert(self);

        try
        {
            if (error == kDNSServiceErr_NoError)
            {
                const bool more = flags & kDNSServiceFlagsMoreComing;
                boost::asio::ip::tcp::endpoint endpoint(bonjour::detail::to_address(*address),
                                                        self->port);
                self->contact.endpoint(endpoint);
                self->on_addrinfo(more);
            }
            else
            {
                self->listener.on_resolver_failure(bonjour::error::make_error_code(error));
            }
        }
        catch (...)
        {
            // Ignore unknown exceptions
        }
    }
};

//-----------------------------------------------------------------------------
// resolver
//-----------------------------------------------------------------------------

resolver::resolver(detail::handle& connection,
                   const aware::contact& contact,
                   typename resolver::listener& listener)
    : connection(connection),
      listener(listener),
      contact(contact),
      port(0)
{
    const ::DNSServiceFlags flags = kDNSServiceFlagsShareConnection;
    std::string regtype = aware::detail::type_encode(contact.type());

    ::DNSServiceRef ref = connection.get<DNSServiceRef>();
    ::DNSServiceErrorType error = ::DNSServiceResolve(&ref,
                                                      flags,
                                                      from_index(contact.index()),
                                                      contact.name().c_str(),
                                                      regtype.c_str(),
                                                      contact.domain().c_str(),
                                                      &resolver::callback::on_resolved,
                                                      this);
    throw_on_error(error);

    handle.reset(ref);
}

void resolver::on_resolved(const char *host)
{
    const ::DNSServiceFlags flags = kDNSServiceFlagsShareConnection;
    const ::DNSServiceProtocol protocol =
        (contact.endpoint().protocol() == boost::asio::ip::tcp::v6())
         ? kDNSServiceProtocol_IPv6
         : kDNSServiceProtocol_IPv4;

    ::DNSServiceRef ref = connection.get<DNSServiceRef>();
    ::DNSServiceErrorType error = ::DNSServiceGetAddrInfo(&ref,
                                                          flags,
                                                          from_index(contact.index()),
                                                          protocol,
                                                          host,
                                                          &resolver::callback::on_addrinfo,
                                                          this);
    throw_on_error(error);

    handle.reset(ref);
}

void resolver::on_addrinfo(bool more)
{
    if (!more)
    {
        handle.reset();
    }
    listener.on_resolver_done(contact);
}

} // namespace detail
} // namespace bonjour
} // namespace aware

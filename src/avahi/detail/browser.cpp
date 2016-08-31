///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <boost/asio.hpp>
#include <avahi-common/error.h>
#include <avahi-common/malloc.h>
#include <avahi-client/lookup.h>
#include <boost/system/system_error.hpp>
#include <aware/detail/utility.hpp>
#include <aware/avahi/error.hpp>
#include <aware/avahi/detail/client.hpp>
#include <aware/avahi/detail/browser.hpp>

namespace aware
{
namespace avahi
{
namespace detail
{

boost::asio::ip::address to_address(const AvahiAddress& addr)
{
    using namespace boost::asio;
    switch (addr.proto)
    {
    case AVAHI_PROTO_INET:
        return ip::address_v4(reinterpret_cast<const ip::address_v4::bytes_type&>(addr.data.ipv4.address));

    case AVAHI_PROTO_INET6:
        return ip::address_v6(reinterpret_cast<const ip::address_v6::bytes_type&>(addr.data.ipv6.address));

    default:
        assert(false);
        break;
    }
}

struct browser::wrapper
{
    static void resolver_callback(
        AvahiServiceResolver *resolver,
        AvahiIfIndex interface,
        AvahiProtocol protocol,
        AvahiResolverEvent event,
        const char *name,
        const char *full_type,
        const char *domain,
        const char *host_name,
        const AvahiAddress *address,
        unsigned short port,
        AvahiStringList *txt,
        AvahiLookupResultFlags,
        void *userdata)
    {
        aware::avahi::detail::browser *self = static_cast<aware::avahi::detail::browser *>(userdata);

        if (event == AVAHI_RESOLVER_FOUND)
        {
            // Convert txt record to property map
            aware::contact::property_map_type properties;
            for (; txt != 0; txt = avahi_string_list_get_next(txt))
            {
                char *key;
                char *value;
                if (avahi_string_list_get_pair(txt, &key, &value, 0) == AVAHI_OK)
                {
                    properties[key] = value;
                    avahi_free(key);
                    avahi_free(value);
                }
            }
            // Notify requester
            boost::asio::ip::tcp::endpoint endpoint(aware::avahi::detail::to_address(*address),
                                                    port);
            std::string type = aware::detail::type_decode(full_type);
            aware::contact contact = aware::contact(type).name(name).endpoint(endpoint).properties(properties);
            self->listener.on_appear(contact);
        }
        else
        {
            self->listener.on_failure(avahi::error::make_error_code(avahi_client_errno(avahi_service_resolver_get_client(resolver))));
        }
        avahi_service_resolver_free(resolver);
    }

    static void browser_callback(
        AvahiServiceBrowser *browser,
        AvahiIfIndex interface,
        AvahiProtocol protocol,
        AvahiBrowserEvent event,
        const char *name,
        const char *full_type,
        const char *domain,
        AvahiLookupResultFlags,
        void *userdata)
    {
        aware::avahi::detail::browser *self = static_cast<aware::avahi::detail::browser *>(userdata);

        switch (event)
        {
        case AVAHI_BROWSER_FAILURE:
            {
                self->listener.on_failure(avahi::error::make_error_code(avahi_client_errno(avahi_service_browser_get_client(browser))));
            }
            break;

        case AVAHI_BROWSER_NEW:
            {
                // Use resolver to obtain port and properties
                AvahiServiceResolver *resolver = avahi_service_resolver_new(
                    avahi_service_browser_get_client(browser),
                    interface,
                    protocol,
                    name,
                    full_type,
                    domain,
                    AVAHI_PROTO_UNSPEC,
                    AvahiLookupFlags(0),
                    wrapper::resolver_callback,
                    userdata);
                if (!resolver)
                {
                    self->listener.on_failure(avahi::error::make_error_code(avahi_client_errno(avahi_service_browser_get_client(browser))));
                }
                // resolver is freed in the callback
            }
            break;

        case AVAHI_BROWSER_REMOVE:
            {
                std::string type = aware::detail::type_decode(full_type);
                aware::contact contact = aware::contact(type).name(name);
                self->listener.on_disappear(contact);
            }
            break;

        case AVAHI_BROWSER_CACHE_EXHAUSTED:
            break;

        case AVAHI_BROWSER_ALL_FOR_NOW:
            break;

        default:
            break;
        }
    }
};

browser::browser(const aware::avahi::detail::client& client,
                 const aware::contact& contact,
                 typename browser::listener& listener)
    : listener(listener)
{
    const AvahiProtocol protocol =
        contact.endpoint().protocol() == boost::asio::ip::tcp::v6()
        ? AVAHI_PROTO_INET6
        : AVAHI_PROTO_INET;
    std::string type = aware::detail::type_encode(contact.type());
    ptr = avahi_service_browser_new(client,
                                    AVAHI_IF_UNSPEC,
                                    protocol,
                                    type.c_str(),
                                    NULL,
                                    AvahiLookupFlags(0),
                                    wrapper::browser_callback,
                                    this);
    if (ptr == 0)
        throw boost::system::system_error(avahi::error::make_error_code(AVAHI_ERR_DISCONNECTED));
}

browser::~browser()
{
    if (ptr)
    {
        avahi_service_browser_free(ptr);
    }
}

} // namespace detail
} // namespace avahi
} // namespace aware

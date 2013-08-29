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
#include <new> // std::bad_alloc
#include <boost/asio.hpp>
#include <avahi-common/error.h>
#include <avahi-common/malloc.h>
#include <avahi-client/lookup.h>
#include <aware/detail/avahi/client.hpp>
#include <aware/detail/avahi/browser.hpp>

namespace aware
{
namespace detail
{
namespace avahi
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

// Extract "type" from a "_type._tcp" string.
// Returns empty string if a parse error occurred.
std::string extract_type(const char *type)
{
  const char *beginning = type;
  const char *ending = type;
  if (*beginning == '_')
  {
    ++beginning;
    ending = std::strchr(beginning, '.');
    if (ending == 0)
    {
      ending = beginning;
    }
  }
  return std::string(beginning, ending);
}

} // namespace avahi
} // namespace detail
} // namespace 

extern "C"
void aware_avahi_resolver_callback(AvahiServiceResolver *resolver,
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
    aware::detail::avahi::browser *self = static_cast<aware::detail::avahi::browser *>(userdata);

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
        boost::asio::ip::tcp::endpoint endpoint(aware::detail::avahi::to_address(*address),
                                                port);
        std::string type = aware::detail::avahi::extract_type(full_type);
        aware::contact contact(name, type, endpoint, properties);
        self->on_join(contact);
    }
    else
    {
        boost::system::error_code error(avahi_client_errno(avahi_service_resolver_get_client(resolver)),
                                        boost::system::system_category());
        self->on_failure(error);
    }
    avahi_service_resolver_free(resolver);
}

extern "C"
void aware_avahi_browser_callback(AvahiServiceBrowser *browser,
                                  AvahiIfIndex interface,
                                  AvahiProtocol protocol,
                                  AvahiBrowserEvent event,
                                  const char *name,
                                  const char *full_type,
                                  const char *domain,
                                  AvahiLookupResultFlags,
                                  void *userdata)
{
    aware::detail::avahi::browser *self = static_cast<aware::detail::avahi::browser *>(userdata);

    switch (event)
    {
    case AVAHI_BROWSER_FAILURE:
        {
            boost::system::error_code error(avahi_client_errno(avahi_service_browser_get_client(browser)),
                                            boost::system::system_category());
            self->on_failure(error);
        }
        break;

    case AVAHI_BROWSER_NEW:
        {
            // Use resolver to obtain port and properties
            AvahiServiceResolver *resolver = avahi_service_resolver_new(avahi_service_browser_get_client(browser),
                                                                        interface,
                                                                        protocol,
                                                                        name,
                                                                        full_type,
                                                                        domain,
                                                                        AVAHI_PROTO_UNSPEC,
                                                                        AvahiLookupFlags(0),
                                                                        aware_avahi_resolver_callback,
                                                                        userdata);
            if (!resolver)
            {
                boost::system::error_code error(avahi_client_errno(avahi_service_browser_get_client(browser)),
                                                boost::system::system_category());
                self->on_failure(error);
            }
            // resolver is freed in the callback
        }
        break;

    case AVAHI_BROWSER_REMOVE:
        {
            std::string type = aware::detail::avahi::extract_type(full_type);
            aware::contact contact(name, type);
            self->on_leave(contact);
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

namespace aware
{
namespace detail
{
namespace avahi
{

browser::browser(const aware::detail::avahi::client& client,
                 const aware::contact& contact,
                 join_type on_join,
                 leave_type on_leave,
                 failure_type on_failure)
    : on_join(on_join),
      on_leave(on_leave),
      on_failure(on_failure)
{
    const AvahiProtocol protocol =
        contact.get_endpoint().protocol() == boost::asio::ip::tcp::v6()
        ? AVAHI_PROTO_INET6
        : AVAHI_PROTO_INET;
    std::string type = "_" + contact.get_type() + "._tcp";
    ptr = avahi_service_browser_new(client,
                                    AVAHI_IF_UNSPEC,
                                    protocol,
                                    type.c_str(),
                                    NULL,
                                    AvahiLookupFlags(0),
                                    aware_avahi_browser_callback,
                                    this);
    if (ptr == 0)
        throw std::bad_alloc();
}

browser::~browser()
{
    if (ptr)
    {
        avahi_service_browser_free(ptr);
    }
}

} // namespace avahi
} // namespace detail
} // namespace 

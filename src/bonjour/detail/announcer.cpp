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

#include <cassert>
#include <string>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <aware/detail/utility.hpp>
#include <aware/bonjour/detail/announcer.hpp>
#include <aware/bonjour/detail/properties.hpp>
#include <aware/bonjour/detail/throw_on_error.hpp>
#include <aware/bonjour/error.hpp>
#include "dns_sd.hpp"

namespace aware
{
namespace bonjour
{
namespace detail
{

struct announcer::callback
{
    static void on_registered(::DNSServiceRef,
                              ::DNSServiceFlags flags,
                              ::DNSServiceErrorType error,
                              const char *name,
                              const char *regtype,
                              const char * /* domain */,
                              void *context)
    {
        using namespace aware::bonjour::detail;
        announcer *self = static_cast<announcer *>(context);
        assert(self);

        try
        {
            if (error == kDNSServiceErr_NoError)
            {
                if (flags & kDNSServiceFlagsAdd)
                {
                    assert(self->current);
                    // Endpoint and properties have to be obtained via the monitor
                    self->current->contact = aware::contact(aware::detail::type_decode(regtype))
                        .name(name);
                    self->current->handler(bonjour::error::make_error_code(kDNSServiceErr_NoError));
                    self->current = boost::none;
                }
                else
                {
                    // Name conflicts not implemented yet
                    self->current->handler(bonjour::error::make_error_code(kDNSServiceErr_NameConflict));
                    self->current = boost::none;
                }
            }
            else
            {
                assert(self->current);
                self->current->handler(bonjour::error::make_error_code(error));
                self->current = boost::none;
            }
        }
        catch (const boost::system::system_error& ex)
        {
            self->current->handler(ex.code());
            self->current = boost::none;
        }
        catch (...)
        {
            // Ignore unknown exceptions
        }
    }
};

announcer::announcer(detail::handle& connection)
    : connection(connection)
{
}

void announcer::announce(aware::contact& contact,
                         async_announce_handler handler)
{
    if (current || !handle.empty())
    {
        throw_on_error(kDNSServiceErr_AlreadyRegistered);
    }

    // This operation will be completed when data is ready and process() is called
    current = boost::in_place(boost::ref(contact), handler);

    const ::DNSServiceFlags flags = kDNSServiceFlagsShareConnection | kDNSServiceFlagsNoAutoRename;
    const uint32_t interface_index = from_index(contact.index());
    std::string name = contact.name();
    std::string type = aware::detail::type_encode(contact.type());
    const char *domain = 0; // .local
    const boost::asio::ip::address& address = contact.endpoint().address();
    std::string host = address.is_unspecified()
        ? "" // Use default host name
        : address.to_string();
    const uint16_t port = boost::asio::detail::socket_ops::host_to_network_short(contact.endpoint().port());

    current->properties = boost::make_shared<detail::properties>(contact.properties());

    ::DNSServiceRef ref = connection.get<DNSServiceRef>();
    // May block for seconds if service not running
    ::DNSServiceErrorType error = ::DNSServiceRegister(
        &ref,
        flags,
        interface_index,
        name.c_str(),
        type.c_str(),
        domain,
        host.empty() ? 0 : host.c_str(),
        port,
        current->properties->size(),
        current->properties->data(),
        &announcer::callback::on_registered,
        this);
    throw_on_error(error);

    handle.reset(ref);
}

announcer::current_type::current_type(aware::contact& contact,
                                      async_announce_handler handler)
    : contact(contact),
      handler(handler)
{
}

announcer::current_type::current_type(const current_type& other)
    : contact(other.contact),
      properties(other.properties),
      handler(other.handler)
{
}

} // namespace detail
} // namespace bonjour
} // namespace aware

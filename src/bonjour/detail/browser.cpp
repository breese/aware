///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <boost/make_shared.hpp>
#include <aware/contact.hpp>
#include <aware/detail/utility.hpp>
#include <aware/bonjour/error.hpp>
#include <aware/bonjour/detail/throw_on_error.hpp>
#include <aware/bonjour/detail/browser.hpp>
#include "dns_sd.hpp"

namespace aware
{
namespace bonjour
{
namespace detail
{

//-----------------------------------------------------------------------------
// Callback
//-----------------------------------------------------------------------------

struct browser::callback
{
    static void on_browsed(::DNSServiceRef,
                           ::DNSServiceFlags flags,
                           uint32_t interface_index,
                           ::DNSServiceErrorType error,
                           const char *name,
                           const char *regtype,
                           const char *domain,
                           void *context)
    {
        try
        {
            browser *self = static_cast<browser *>(context);
            assert(self);

            if (error == kDNSServiceErr_NoError)
            {
                aware::contact contact = aware::contact(aware::detail::type_decode(regtype))
                    .name(name)
                    .domain(domain)
                    .index(to_index(interface_index));
                const bool commit = !(flags & kDNSServiceFlagsMoreComing);
                if (flags & kDNSServiceFlagsAdd)
                {
                    self->listener.on_browser_appear(contact, commit);
                }
                else
                {
                    self->listener.on_browser_disappear(contact, commit);
                }
            }
            else
            {
                self->listener.on_browser_failure(bonjour::error::make_error_code(error));
            }
        }
        catch (...)
        {
            // Ignore unknown exceptions
        }
    }
};

//-----------------------------------------------------------------------------
// browser
//-----------------------------------------------------------------------------

browser::browser(const std::string& type,
                 detail::handle& connection,
                 typename browser::listener& listener)
    : connection(connection),
      listener(listener)
{
    const ::DNSServiceFlags flags = kDNSServiceFlagsShareConnection;
    std::string regtype = aware::detail::type_encode(type);
    const uint32_t interfaceIndex = kDNSServiceInterfaceIndexAny;
    const char *domain = 0; // Use .local

    ::DNSServiceRef ref = connection.get<DNSServiceRef>();
    ::DNSServiceErrorType error = ::DNSServiceBrowse(
        &ref,
        flags,
        interfaceIndex,
        regtype.c_str(),
        domain,
        &browser::callback::on_browsed,
        this);
    throw_on_error(error);

    handle.reset(ref);
}

} // namespace detail
} // namespace bonjour
} // namespace aware

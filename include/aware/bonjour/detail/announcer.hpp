#ifndef AWARE_BONJOUR_DETAIL_ANNOUNCER_HPP
#define AWARE_BONJOUR_DETAIL_ANNOUNCER_HPP

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

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <aware/contact.hpp>
#include <aware/announce_socket.hpp>
#include <aware/bonjour/detail/handle.hpp>
#include <aware/bonjour/detail/properties.hpp>

namespace aware
{
namespace bonjour
{
namespace detail
{

// Wrapper for DNSServiceRegister
class announcer
{
public:
    typedef int error_type;
    typedef typename aware::announce_socket::async_announce_handler async_announce_handler;

    // Instantiated per announce type
    announcer(detail::handle&);

    // Called per announce request
    void announce(aware::contact&, async_announce_handler);

private:
    struct callback;

    detail::handle& connection;
    detail::handle handle;
    struct current_type
    {
        current_type(aware::contact&, async_announce_handler);
        current_type(const current_type&);

        aware::contact& contact;
        boost::shared_ptr<detail::properties> properties;
        async_announce_handler handler;
    };
    boost::optional<current_type> current;
};

} // namespace detail
} // namespace bonjour
} // namespace aware

#endif // AWARE_BONJOUR_DETAIL_ANNOUNCER_HPP

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

#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <aware/avahi/announce_socket.hpp>

namespace aware
{
namespace avahi
{

announce_socket::announce_socket(aware::avahi::io_service& io)
    : announcer(io.get_client())
{
}

void announce_socket::async_announce(const aware::contact& contact,
                                     async_announce_handler handler)
{
    announcer.async_announce(contact,
                             boost::bind(&announce_socket::process_announce,
                                         this,
                                         boost::asio::placeholders::error,
                                         handler));
}

void announce_socket::process_announce(const boost::system::error_code& error,
                                       async_announce_handler handler)
{
    handler(error);
}

} // namespace avahi
} // namespace aware

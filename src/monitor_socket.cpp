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
#include <boost/make_shared.hpp>
#include <boost/asio/placeholders.hpp>
#include <aware/monitor_socket.hpp>

namespace aware
{

monitor_socket::monitor_socket(aware::io_service& io)
    : io(io)
{
    // Browser must be created by an io_service thread because it may trigger
    // callbacks
    io.post(boost::bind(&monitor_socket::do_initialize,
                        this));
}

void monitor_socket::async_listen(async_listen_handler handler)
{
    // Browser may not have been initialized yet
    io.post(boost::bind(&monitor_socket::do_async_listen,
                        this,
                        handler));
}

void monitor_socket::do_async_listen(async_listen_handler handler)
{
    browser->async_listen(boost::bind(&monitor_socket::process_listen,
                                      this,
                                      boost::asio::placeholders::error,
                                      _2,
                                      handler));
}

void monitor_socket::do_initialize()
{
    browser = boost::make_shared<aware::detail::avahi::browser>(io.get_client());
}

void monitor_socket::process_listen(const boost::system::error_code& error,
                                    const aware::contact& contact,
                                    async_listen_handler handler)
{
    handler(error, contact);
}

} // namespace aware

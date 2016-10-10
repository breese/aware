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
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/make_shared.hpp>
#include <aware/bonjour/detail/monitor.hpp>
#include <aware/bonjour/error.hpp>
#include <aware/bonjour/monitor_socket.hpp>
#include "detail/dns_sd.hpp"

namespace aware
{
namespace bonjour
{

monitor_socket::monitor_socket(boost::asio::io_service& io)
    : io(io),
      connection(detail::handle::with_connection),
      socket(io, connection.native_handle()),
      waiting(false)
{
}

monitor_socket::~monitor_socket()
{
    monitors.clear();
}

void monitor_socket::async_listen(aware::contact& contact,
                                  async_listen_handler handler)
{
    if (connection.empty())
    {
        permanent_error = bonjour::error::make_error_code(kDNSServiceErr_BadState);
    }
    if (permanent_error)
    {
        io.post(boost::bind(&monitor_socket::invoke,
                            this,
                            permanent_error,
                            handler));
        return;
    }

    try
    {
        const std::string& key = contact.type();
        monitor_map::iterator where = monitors.lower_bound(key);
        if ((where == monitors.end()) || (monitors.key_comp()(key, where->first)))
        {
            where = monitors.insert(
                where,
                monitor_map::value_type(
                    key,
                    boost::make_shared<bonjour::detail::monitor>(boost::ref(io),
                                                                 boost::ref(connection))));
        }
        assert(where != monitors.end());
        where->second->listen(contact, handler);

        if (!waiting)
        {
            socket.async_read_event(boost::bind(&monitor_socket::process_read_event,
                                                this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }
    }
    catch (const boost::system::system_error& ex)
    {
        io.post(boost::bind(&monitor_socket::invoke,
                            this,
                            ex.code(),
                            handler));
    }
    // Other exceptions are propagated outwards
}

void monitor_socket::process_read_event(const boost::system::error_code& error,
                                        std::size_t)
{
    waiting = false;
    if (!error)
    {
        // Execute the register callbacks
        int status = ::DNSServiceProcessResult(connection.get<DNSServiceRef>());
        if (status == kDNSServiceErr_NoError)
        {
            socket.async_read_event(boost::bind(&monitor_socket::process_read_event,
                                                this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
            waiting = true;
        }
        else
        {
            permanent_error = bonjour::error::make_error_code(status);
        }
    }
    else if (error == boost::asio::error::operation_aborted)
    {
        // Ignore
    }
    else
    {
        permanent_error = error;
    }
}

void monitor_socket::invoke(const boost::system::error_code& error,
                            async_listen_handler handler)
{
    handler(error);
}

} // namespace bonjour
} // namespace aware

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
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <aware/bonjour/detail/throw_on_error.hpp>
#include <aware/bonjour/detail/announcer.hpp>
#include <aware/bonjour/announce_socket.hpp>
#include "detail/dns_sd.hpp"

namespace aware
{
namespace bonjour
{

announce_socket::announce_socket(boost::asio::io_service& io)
    : io(io),
      connection(detail::handle::with_connection),
      socket(io, connection.native_handle()),
      waiting(false)
{
}

void announce_socket::async_announce(aware::contact& contact,
                                     async_announce_handler handler)
{
    if (connection.empty())
    {
        using namespace boost::system;
        error_code irrecoverable(errc::state_not_recoverable, system_category());
        io.post(boost::bind(&announce_socket::invoke,
                            this,
                            irrecoverable,
                            handler));
        return;
    }

    try
    {
        const std::string& key = contact.type();
        announcer_map::iterator where = announcers.lower_bound(key);
        if ((where == announcers.end()) || (announcers.key_comp()(key, where->first)))
        {
            where = announcers.insert(
                where,
                announcer_map::value_type(
                    key,
                    boost::make_shared<bonjour::detail::announcer>(boost::ref(connection))));
        }
        assert(where != announcers.end());
        where->second->announce(contact, handler);

        if (!waiting)
        {
            socket.async_read_event(boost::bind(&announce_socket::process_read_event,
                                                this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
            waiting = true;
        }
    }
    catch (const boost::system::system_error& ex)
    {
        io.post(boost::bind(&announce_socket::invoke,
                            this,
                            ex.code(),
                            handler));
    }
    // Other exceptions are propagated outwards
}

void announce_socket::process_read_event(const boost::system::error_code& error,
                                         std::size_t)
{
    waiting = false;
    if (!error)
    {
        // Execute the register callbacks
        int status = ::DNSServiceProcessResult(connection.get<DNSServiceRef>());
        if (status == kDNSServiceErr_NoError)
        {
            socket.async_read_event(boost::bind(&announce_socket::process_read_event,
                                                this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
            waiting = true;
        }
        else
        {
            // FIXME: How to report error?
        }
    }
    else if (error == boost::asio::error::operation_aborted)
    {
        // Ignore
    }
    else
    {
        // FIXME: How to report error?
    }
}

void announce_socket::invoke(const boost::system::error_code& error,
                             async_announce_handler handler)
{
    handler(error);
}

} // namespace bonjour
} // namespace aware

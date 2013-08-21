#ifndef AWARE_IO_SERVICE_HPP
#define AWARE_IO_SERVICE_HPP

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

#include <boost/asio/io_service.hpp>

// FIXME: Remove these from public API
#include <aware/detail/avahi/poller.hpp>
#include <aware/detail/avahi/client.hpp>

namespace aware
{

class io_service
{
public:
    io_service(boost::asio::io_service& io)
        : io(io),
          poller(io),
          client(poller)
    {}

    const detail::avahi::client& get_client() const { return client; }

    template <typename CompletionHandler>
    void post(CompletionHandler handler) { io.post(handler); }

private:
    boost::asio::io_service& io;
    detail::avahi::poller poller;
    detail::avahi::client client;
};

} // namespace aware

#endif // AWARE_IO_SERVICE_HPP

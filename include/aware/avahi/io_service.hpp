#ifndef AWARE_AVAHI_IO_SERVICE_HPP
#define AWARE_AVAHI_IO_SERVICE_HPP

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

#include <aware/io_service.hpp>
#include <aware/avahi/detail/poller.hpp>
#include <aware/avahi/detail/client.hpp>

namespace aware
{

namespace avahi
{

class io_service : public aware::io_service
{
public:
    io_service(boost::asio::io_service& io)
        : aware::io_service(io),
          poller(io),
          client(poller)
    {}

    const detail::client& get_client() const { return client; }

    /*template <typename CompletionHandler>
    void post(CompletionHandler handler) { io.post(handler); }*/

private:
    detail::poller poller;
    detail::client client;
};

} // namespace avahi
} // namespace aware

#endif // AWARE_IO_SERVICE_HPP

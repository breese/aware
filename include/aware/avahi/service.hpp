#ifndef AWARE_AVAHI_SERVICE_HPP
#define AWARE_AVAHI_SERVICE_HPP

///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2014 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/asio/io_service.hpp>
#include <aware/avahi/detail/poller.hpp>
#include <aware/avahi/detail/client.hpp>

namespace aware
{
namespace avahi
{

class service : public boost::asio::io_service::service
{
public:
    static boost::asio::io_service::id id;

    struct implementation_type
    {
    };

    explicit service(boost::asio::io_service& io);

    void construct(implementation_type&) {}
    void destroy(implementation_type&) {}

    detail::client& get_client() { return client; }

private:
    virtual void shutdown_service() {}

private:
    detail::poller poller;
    detail::client client;
};

} // namespace avahi
} // namespace aware

#endif // AWARE_AVAHI_SERVICE_HPP

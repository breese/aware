#ifndef AWARE_AVAHI_FACTORY_HPP
#define AWARE_AVAHI_FACTORY_HPP

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

#include <aware/factory.hpp>
#include <aware/avahi/announce_socket.hpp>
#include <aware/avahi/monitor_socket.hpp>

#include <boost/make_shared.hpp>

namespace aware
{
namespace avahi
{

class factory : public aware::factory
{
public:
    boost::shared_ptr<aware::announce_socket> make_announce(boost::asio::io_service& io)
    {
        return boost::make_shared<aware::avahi::announce_socket>(boost::ref(io));
    }

    boost::shared_ptr<aware::monitor_socket> make_monitor(boost::asio::io_service& io)
    {
        return boost::make_shared<aware::avahi::monitor_socket>(boost::ref(io));
    }
};

} // namespace avahi
} // namespace aware

#endif // AWARE_AVAHI_FACTORY_HPP

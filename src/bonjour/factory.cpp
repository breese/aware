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

#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <aware/bonjour/factory.hpp>

namespace aware
{
namespace bonjour
{

boost::shared_ptr<aware::announce_socket> factory::make_announce(boost::asio::io_service& io)
{
    return boost::make_shared<aware::bonjour::announce_socket>(boost::ref(io));
}

boost::shared_ptr<aware::monitor_socket> factory::make_monitor(boost::asio::io_service& io)
{
    return boost::make_shared<aware::bonjour::monitor_socket>(boost::ref(io));
}

} // namespace bonjour
} // namespace aware

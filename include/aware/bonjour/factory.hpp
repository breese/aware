#ifndef AWARE_BONJOUR_FACTORY_HPP
#define AWARE_BONJOUR_FACTORY_HPP

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

#include <aware/factory.hpp>
#include <aware/bonjour/announce_socket.hpp>
#include <aware/bonjour/monitor_socket.hpp>

namespace aware
{
namespace bonjour
{

class factory : public aware::factory
{
public:
    virtual boost::shared_ptr<aware::announce_socket> make_announce(boost::asio::io_service&);
    virtual boost::shared_ptr<aware::monitor_socket> make_monitor(boost::asio::io_service&);
};

} // namespace bonjour
} // namespace aware

#endif // AWARE_BONJOUR_FACTORY_HPP

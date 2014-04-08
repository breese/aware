#ifndef AWARE_FACTORY_HPP
#define AWARE_FACTORY_HPP

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
#include <boost/shared_ptr.hpp>

namespace aware
{

class announce_socket;
class monitor_socket;
class io_service;

class factory
{
public:
    virtual boost::shared_ptr<aware::io_service> make_service(boost::asio::io_service&) =0;
    virtual boost::shared_ptr<announce_socket> make_announce(aware::io_service&) = 0;
    virtual boost::shared_ptr<monitor_socket> make_monitor(aware::io_service&) = 0;

};

} // namespace aware

#endif // AWARE_FACTORY_HPP

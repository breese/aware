#ifndef AWARE_MONITOR_SOCKET_HPP
#define AWARE_MONITOR_SOCKET_HPP

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

#include <boost/function.hpp>
#include <boost/system/error_code.hpp>
#include <aware/contact.hpp>

namespace aware
{

class monitor_socket
{
public:
    typedef boost::function<void (const boost::system::error_code&)> async_listen_handler;

    virtual ~monitor_socket() {}

    virtual void async_listen(aware::contact& contact,
                              async_listen_handler handler) = 0;
};

} // namespace aware

#endif // AWARE_MONITOR_SOCKET_HPP

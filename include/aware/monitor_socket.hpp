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
#include <boost/shared_ptr.hpp>
#include <aware/io_service.hpp>
#include <aware/contact.hpp>

// FIXME: Remove these from public API
#include <aware/detail/avahi/browser.hpp>

namespace aware
{

class monitor_socket
{
public:
    typedef boost::function<void (const boost::system::error_code&,
                                  const aware::contact&)> async_listen_handler;

    monitor_socket(aware::io_service& io);

    // FIXME: Listen on domain or for specific service?
    void async_listen(async_listen_handler handler);

private:
    void do_initialize();
    void do_async_listen(async_listen_handler);
    void process_listen(const boost::system::error_code&,
                        const aware::contact&,
                        async_listen_handler);

private:
    aware::io_service& io;
    boost::shared_ptr<aware::detail::avahi::browser> browser;
};

} // namespace aware

#endif // AWARE_MONITOR_SOCKET_HPP

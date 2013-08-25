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

#include <string>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <aware/io_service.hpp>
#include <aware/contact.hpp>

namespace aware
{
namespace detail { class monitor; }

class monitor_socket
{
    typedef boost::shared_ptr<aware::detail::monitor> monitor_ptr;
    typedef std::map<std::string, monitor_ptr> monitor_map;

public:
    typedef boost::function<void (const boost::system::error_code&,
                                  const aware::contact&)> async_listen_handler;

    monitor_socket(aware::io_service& io);

    void async_listen(const aware::contact& contact,
                      async_listen_handler handler);

private:
    void do_async_listen(const aware::contact&, async_listen_handler);

private:
    aware::io_service& io;
    monitor_map monitors; // FIXME: thread-safety
};

} // namespace aware

#endif // AWARE_MONITOR_SOCKET_HPP

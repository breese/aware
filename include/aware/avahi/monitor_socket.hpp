#ifndef AWARE_AVAHI_MONITOR_SOCKET_HPP
#define AWARE_AVAHI_MONITOR_SOCKET_HPP

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
#include <boost/asio/io_service.hpp>
#include <boost/asio/basic_io_object.hpp>

#include <aware/contact.hpp>
#include <aware/monitor_socket.hpp>
#include <aware/avahi/service.hpp>

namespace aware
{

namespace avahi
{

namespace detail { class monitor; }

class monitor_socket
    : public aware::monitor_socket,
      public boost::asio::basic_io_object<avahi::service>
{
    typedef boost::shared_ptr<aware::avahi::detail::monitor> monitor_ptr;
    typedef std::map<std::string, monitor_ptr> monitor_map;

public:
    monitor_socket(boost::asio::io_service& io);

    void async_listen(aware::contact& contact,
                      async_listen_handler handler);

private:
    void do_async_listen(aware::contact&, async_listen_handler);

private:
    monitor_map monitors; // FIXME: thread-safety
};

} // namespace avahi
} // namespace aware

#endif // AWARE_AVAHI_MONITOR_SOCKET_HPP

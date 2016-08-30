#ifndef AWARE_AVAHI_ANNOUNCE_SOCKET_HPP
#define AWARE_AVAHI_ANNOUNCE_SOCKET_HPP

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
#include <boost/asio/io_service.hpp>
#include <boost/asio/basic_io_object.hpp>

#include <aware/contact.hpp>
#include <aware/announce_socket.hpp>
#include <aware/avahi/service.hpp>
#include <aware/avahi/detail/announcer.hpp>

namespace aware
{
namespace avahi
{

class announce_socket
    : public aware::announce_socket,
      public boost::asio::basic_io_object<avahi::service>
{
public:
    announce_socket(boost::asio::io_service&);

    virtual void async_announce(aware::contact& contact,
                                async_announce_handler);

private:
    void process_announce(const boost::system::error_code&,
                          async_announce_handler);

private:
    // FIXME: Defer intialization until inside io.run()
    aware::avahi::detail::announcer announcer; // FIXME: This may not be created before client has entered one of the AVAHI_CLIENT_S_* states [defs.h]
};

} // namespace avahi
} // namespace aware

#endif // AWARE_ANNOUNCE_SOCKET_HPP

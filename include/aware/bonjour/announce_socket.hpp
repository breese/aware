#ifndef AWARE_BONJOUR_ANNOUNCE_SOCKET_HPP
#define AWARE_BONJOUR_ANNOUNCE_SOCKET_HPP

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

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/asio/io_service.hpp>
#include <aware/announce_socket.hpp>
#include <aware/detail/native_socket.hpp>
#include <aware/bonjour/detail/handle.hpp>

namespace aware
{
namespace bonjour
{

namespace detail { class announcer; }

class announce_socket
    : public aware::announce_socket
{
public:
    announce_socket(boost::asio::io_service&);

    virtual void async_announce(aware::contact& contact,
                                async_announce_handler);

private:
    void process_read_event(const boost::system::error_code&,
                            std::size_t);

    void invoke(const boost::system::error_code&,
                async_announce_handler);

private:
    boost::asio::io_service& io;
    detail::handle connection;
    aware::detail::native_socket socket;
    bool waiting;
    typedef std::map< std::string, boost::shared_ptr<detail::announcer> > announcer_map;
    announcer_map announcers;
};

} // namespace bonjour
} // namespace aware

#endif // AWARE_BONJOUR_ANNOUNCE_SOCKET_HPP

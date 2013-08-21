#ifndef AWARE_DETAIL_NATIVE_SOCKET_HPP
#define AWARE_DETAIL_NATIVE_SOCKET_HPP

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

#include <boost/asio.hpp>
#include <boost/function.hpp>

namespace aware
{
namespace detail
{

template <typename Service>
class non_closing_service : public Service
{
public:
    non_closing_service(boost::asio::io_service& io)
        : Service(io)
    {}

    void destroy(typename Service::implementation_type&)
    {
        // Do not close the file descriptor as we have no ownership of it
    }
};

//! @brief Boost.Asio wrapper for a native socket
class native_socket
{
    typedef non_closing_service<boost::asio::posix::stream_descriptor_service> service_type;
    typedef boost::asio::posix::basic_stream_descriptor<service_type> socket_type;

public:
    typedef boost::function<void (const boost::system::error_code&, std::size_t)> async_read_event_handler;
    typedef boost::function<void (const boost::system::error_code&, std::size_t)> async_write_event_handler;
    typedef socket_type::native_handle_type native_handle_type;

    native_socket(boost::asio::io_service&, native_handle_type);
    ~native_socket();

    void async_read_event(async_read_event_handler);
    void async_write_event(async_write_event_handler);

    native_handle_type native_handle();

private:
    socket_type socket;
};

} // namespace detail
} // namespace aware

#endif // AWARE_DETAIL_NATIVE_SOCKET_HPP

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

#include <aware/detail/native_socket.hpp>

namespace aware
{
namespace detail
{

native_socket::native_socket(boost::asio::io_service& io,
                             native_handle_type handle)
    : socket(io, handle)
{
}

native_socket::~native_socket()
{
  // Close the socket to make sure all asynchronous requests are cancelled.
  if (socket.is_open())
  {
    boost::system::error_code dummy; // Ignore errors
    socket.close(dummy);
  }
}

native_socket::native_handle_type native_socket::native_handle()
{
    return socket.native_handle();
}

} // namespace detail
} // namespace aware

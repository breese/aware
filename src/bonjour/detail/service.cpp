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

#include <boost/bind.hpp>
#include <aware/bonjour/detail/throw_on_error.hpp>
#include <aware/bonjour/detail/service.hpp>
#include "dns_sd.hpp"

namespace aware
{
namespace bonjour
{
namespace detail
{

boost::asio::io_service::id service::id;

service::service(boost::asio::io_service& io)
    : boost::asio::io_service::service(io),
      connection(detail::handle::with_connection)
{
}

detail::handle& service::get_connection()
{
    return connection;
}

} // namespace detail
} // namespace bonjour
} // namespace aware

#ifndef AWARE_BONJOUR_DETAIL_SERVICE_HPP
#define AWARE_BONJOUR_DETAIL_SERVICE_HPP

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

#include <boost/scoped_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <aware/detail/native_socket.hpp>
#include <aware/bonjour/detail/handle.hpp>

namespace aware
{
namespace bonjour
{
namespace detail
{

class service : public boost::asio::io_service::service
{
public:
    static boost::asio::io_service::id id;

    struct implementation_type
    {
    };

    explicit service(boost::asio::io_service& io);

    void construct(implementation_type&) {}
    void destroy(implementation_type&) {}

    detail::handle& get_connection();

private:
    virtual void shutdown_service() {}

private:
    detail::handle connection;
    boost::scoped_ptr<aware::detail::native_socket> socket;
};

} // namespace detail
} // namespace bonjour
} // namespace aware

#endif // AWARE_BONJOUR_DETAIL_SERVICE_HPP

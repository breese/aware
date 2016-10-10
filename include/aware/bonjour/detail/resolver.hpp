#ifndef AWARE_BONJOUR_DETAIL_RESOLVER_HPP
#define AWARE_BONJOUR_DETAIL_RESOLVER_HPP

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

#include <aware/contact.hpp>
#include <aware/bonjour/detail/handle.hpp>

namespace aware
{
namespace bonjour
{
namespace detail
{

class resolver
{
public:
    class listener
    {
    public:
        virtual ~listener() {};

        virtual void on_resolver_done(const aware::contact& contact) = 0;
        virtual void on_resolver_failure(const boost::system::error_code&) = 0;
    };

    resolver(detail::handle&,
             const aware::contact& contact,
             resolver::listener&);

private:
    void on_resolved(const char *host);
    void on_addrinfo(bool more);

private:
    struct callback;

    detail::handle& connection;
    resolver::listener& listener;
    detail::handle handle;
    aware::contact contact;
    short port;
};

} // namespace detail
} // namespace bonjour
} // namespace aware

#endif // AWARE_BONJOUR_DETAIL_RESOLVER_HPP

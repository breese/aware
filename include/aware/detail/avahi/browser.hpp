#ifndef AWARE_DETAIL_AVAHI_BROWSER_HPP
#define AWARE_DETAIL_AVAHI_BROWSER_HPP

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
#include <boost/system/error_code.hpp>
#include <aware/contact.hpp>

class AvahiServiceBrowser;

namespace aware
{
namespace detail
{
namespace avahi
{
class client;

class browser
{
public:
    typedef boost::function<void (const aware::contact&)> join_type;
    typedef boost::function<void (const aware::contact&)> leave_type;
    typedef boost::function<void (const boost::system::error_code&)> failure_type;

    browser(const aware::detail::avahi::client&,
            const aware::contact& contact,
            join_type,
            leave_type,
            failure_type);
    ~browser();

private:
    AvahiServiceBrowser *ptr;
public:
    join_type on_join;
    leave_type on_leave;
    failure_type on_failure;
};

} // namespace avahi
} // namespace detail
} // namespace aware

#endif // AWARE_DETAIL_AVAHI_BROWSER_HPP

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
    typedef boost::function<void (const boost::system::error_code&,
                                  const aware::contact&)> async_listen_handler;

    browser(const aware::detail::avahi::client&);
    ~browser();

    void async_listen(async_listen_handler);

    // These are internal functions
    void on_join(const aware::contact&);
    void on_leave(const aware::contact&);
    void on_failure(const boost::system::error_code&);

private:
    AvahiServiceBrowser *ptr;
    async_listen_handler handler;
};

} // namespace avahi
} // namespace detail
} // namespace aware

#endif // AWARE_DETAIL_AVAHI_BROWSER_HPP

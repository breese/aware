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

#include <boost/system/error_code.hpp>
#include <aware/contact.hpp>
#include <aware/avahi/detail/client.hpp>

struct AvahiServiceBrowser;

namespace aware
{
namespace avahi
{
namespace detail
{
class client;

class browser
{
public:
    class listener
    {
    public:
        virtual ~listener() {}

        virtual void on_appear(const aware::contact&) = 0;
        virtual void on_disappear(const aware::contact&) = 0;
        virtual void on_failure(const boost::system::error_code&) = 0;
    };

    browser(const aware::avahi::detail::client&,
            const aware::contact& contact,
            browser::listener&);
    ~browser();

private:
    typedef browser::listener listener_type;
    struct wrapper;

    browser::listener& listener;
    AvahiServiceBrowser *ptr;
};

} // namespace detail
} // namespace avahi
} // namespace aware

#endif // AWARE_DETAIL_AVAHI_BROWSER_HPP

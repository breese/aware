#ifndef AWARE_BONJOUR_DETAIL_BROWSER_HPP
#define AWARE_BONJOUR_DETAIL_BROWSER_HPP

///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <boost/system/error_code.hpp>
#include <aware/contact.hpp>
#include <aware/bonjour/detail/handle.hpp>

namespace aware
{
namespace bonjour
{
namespace detail
{

class browser
{
    typedef void * identifier_type;

public:
    class listener
    {
    public:
        virtual ~listener() {}

        virtual void on_browser_appear(const aware::contact& contact,
                                       bool more) = 0;
        virtual void on_browser_disappear(const aware::contact& contact,
                                          bool more) = 0;
        virtual void on_browser_failure(const boost::system::error_code&) = 0;
    };

    browser(const std::string& type,
            detail::handle&,
            browser::listener&);

private:
    struct callback;

    detail::handle& connection;
    browser::listener& listener;
    detail::handle handle;
};

} // namespace detail
} // namespace bonjour
} // namespace aware

#endif // AWARE_BONJOUR_DETAIL_BROWSER_HPP

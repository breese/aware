#ifndef AWARE_DETAIL_AVAHI_ANNOUNCER_HPP
#define AWARE_DETAIL_AVAHI_ANNOUNCER_HPP

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

#include <string>
#include <map>
#include <boost/function.hpp>
#include <boost/system/error_code.hpp>
#include <aware/contact.hpp>

struct AvahiEntryGroup;

namespace aware
{
namespace avahi
{
namespace detail
{
class client;

// RAII wrapper for avahi_entry_group functionality
class announcer
{
public:
    typedef std::map<std::string, std::string> property_map;
    typedef boost::function<void (const boost::system::error_code&)> async_announce_handler;

    announcer(const aware::avahi::detail::client&);
    ~announcer();

    void async_announce(const aware::contact&,
                        async_announce_handler);

    // These are internal functions
    void on_established(AvahiEntryGroup *);
    void on_collision(AvahiEntryGroup *);
    void on_failure(AvahiEntryGroup *);
    void commit(AvahiEntryGroup *);

private:
    AvahiEntryGroup *ptr;
    async_announce_handler handler;
};

} // namespace detail
} // namespace avahi
} // namespace aware

#endif // AWARE_DETAIL_AVAHI_ANNOUNCER_HPP

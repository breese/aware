#ifndef AWARE_DETAIL_AVAHI_CLIENT_HPP
#define AWARE_DETAIL_AVAHI_CLIENT_HPP

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

struct AvahiClient;

namespace aware
{
namespace detail
{
namespace avahi
{
class poller;

// RAII wrapper of avahi_client functionality
class client
{
public:
    client(aware::detail::avahi::poller&);
    virtual ~client();

    operator AvahiClient *() const { return ptr; }

    // These are internal functions
    void registering(AvahiClient *);
    void connecting(AvahiClient *);
    void running(AvahiClient *);
    void collision(AvahiClient *);
    void failure(AvahiClient *);

private:
    AvahiClient *ptr;
};

} // namespace avahi
} // namespace detail
} // namespace aware

#endif // AWARE_DETAIL_AVAHI_CLIENT_HPP

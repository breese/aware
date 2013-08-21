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

#include <iostream> // FIXME
#include <cassert>
#include <new> // std::bad_alloc
#include <avahi-client/client.h>
#include <aware/detail/avahi/poller.hpp>
#include <aware/detail/avahi/client.hpp>

extern "C"
void avahi_client_callback(AvahiClient *client,
                           AvahiClientState state,
                           void *userdata)
{
    aware::detail::avahi::client *self = static_cast<aware::detail::avahi::client *>(userdata);

    switch (state)
    {
    case AVAHI_CLIENT_S_REGISTERING:
        self->registering(client);
        break;
    case AVAHI_CLIENT_S_RUNNING:
        self->running(client);
        break;
    case AVAHI_CLIENT_S_COLLISION:
        self->collision(client);
        break;
    case AVAHI_CLIENT_FAILURE:
        self->failure(client);
        break;
    case AVAHI_CLIENT_CONNECTING:
        self->connecting(client);
        break;
    default:
        break;
    }
}

namespace aware
{
namespace detail
{
namespace avahi
{

client::client(aware::detail::avahi::poller& poller)
    : ptr(0)
{
    const AvahiClientFlags flags = AvahiClientFlags(0);
    // This will trigger the running() callback before ptr is set here
    ptr = avahi_client_new(reinterpret_cast<AvahiPoll *>(&poller),
                           flags,
                           avahi_client_callback,
                           this,
                           0);
    if (ptr == 0)
        throw std::bad_alloc();

    assert(avahi_client_get_state(ptr) == AVAHI_CLIENT_S_RUNNING);
}

client::~client()
{
    if (ptr)
    {
        avahi_client_free(ptr);
    }
}

void client::registering(AvahiClient *client)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    assert(ptr == client);
    // FIXME
}

void client::connecting(AvahiClient *client)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    assert(ptr == client);
    // FIXME
}

void client::running(AvahiClient *client)
{
    // std::cout << __PRETTY_FUNCTION__ << " ptr=" << ptr << std::endl;
    if (ptr == 0)
    {
        // Ignore because we have been called from the constructor
    }
    else
    {
        assert(ptr == client);
    }
}

void client::collision(AvahiClient *client)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    assert(ptr == client);
    // FIXME
}

void client::failure(AvahiClient *client)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    assert(ptr == client);
    // FIXME
}

} // namespace avahi
} // namespace detail
} // namespace aware

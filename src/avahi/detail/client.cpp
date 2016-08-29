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
#include <avahi-common/error.h>
#include <avahi-client/client.h>
#include <boost/system/system_error.hpp>
#include <aware/avahi/error.hpp>
#include <aware/avahi/detail/poller.hpp>
#include <aware/avahi/detail/client.hpp>

namespace aware
{
namespace avahi
{
namespace detail
{

struct client::wrapper
{

    static void client_callback(AvahiClient *client,
                                AvahiClientState state,
                                void *userdata)
    {
        aware::avahi::detail::client *self = static_cast<aware::avahi::detail::client *>(userdata);

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
};

client::client(aware::avahi::detail::poller& poller)
    : ptr(0)
{
    const AvahiClientFlags flags = AvahiClientFlags(0);
    // This will trigger the running() callback before ptr is set here
    ptr = avahi_client_new(reinterpret_cast<AvahiPoll *>(&poller),
                           flags,
                           wrapper::client_callback,
                           this,
                           0);
    if (ptr == 0)
        throw boost::system::system_error(avahi::error::make_error_code(AVAHI_ERR_DISCONNECTED));

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

} // namespace detail
} // namespace avahi
} // namespace aware

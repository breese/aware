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

#include <cassert>
#include <new> // std::bad_alloc
#include <avahi-common/error.h>
#include <avahi-common/strlst.h>
#include <avahi-common/alternative.h>
#include <avahi-common/malloc.h>
#include <avahi-client/publish.h>
#include <avahi-client/client.h>
#include <aware/detail/avahi/error.hpp>
#include <aware/detail/avahi/client.hpp>
#include <aware/detail/avahi/announcer.hpp>

extern "C"
void avahi_entry_group_callback(AvahiEntryGroup *group,
                                AvahiEntryGroupState state,
                                void *userdata)
{
    aware::detail::avahi::announcer *self = static_cast<aware::detail::avahi::announcer *>(userdata);

    switch (state)
    {
    case AVAHI_ENTRY_GROUP_ESTABLISHED:
        self->on_established(group);
        break;

    case AVAHI_ENTRY_GROUP_COLLISION:
        self->on_collision(group);
        break;

    case AVAHI_ENTRY_GROUP_FAILURE:
        self->on_failure(group);
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

//-----------------------------------------------------------------------------
// property_list
//-----------------------------------------------------------------------------

// RAII wrapper for AvahiStringList
class property_list
{
public:
    property_list()
        : data(0)
    {
    }

    ~property_list()
    {
        avahi_string_list_free(data);
    }

    property_list& operator = (const announcer::property_map& properties)
    {
        for (announcer::property_map::const_iterator it = properties.begin();
             it != properties.end();
             ++it)
        {
            data = avahi_string_list_add_pair(data, it->first.c_str(), it->second.c_str());
            if (data == 0)
            {
                break;
            }
        }
        return *this;
    }

    operator AvahiStringList *()
    {
        return data;
    }

private:
    AvahiStringList *data;
};

//-----------------------------------------------------------------------------
// announcer
//-----------------------------------------------------------------------------

announcer::announcer(const aware::detail::avahi::client& client)
    : ptr(0)
{
    ptr = avahi_entry_group_new(client,
                                avahi_entry_group_callback,
                                this);
    if (ptr == 0)
        throw std::bad_alloc();

    assert(avahi_entry_group_get_state(ptr) == AVAHI_ENTRY_GROUP_UNCOMMITED);
}

announcer::~announcer()
{
    if (ptr)
    {
        avahi_entry_group_free(ptr);
    }
}

void announcer::async_announce(const aware::contact& contact,
                               async_announce_handler h)
{
    assert(ptr != 0);

    handler = h;

    const AvahiPublishFlags flags = AvahiPublishFlags(0);
    const aware::contact::endpoint_type endpoint = contact.get_endpoint();
    // Use all network interfaces
    const AvahiIfIndex interface_index = AVAHI_IF_UNSPEC;
    // Use only a specific protocol
    const AvahiProtocol protocol =
        contact.get_endpoint().protocol() == boost::asio::ip::tcp::v6()
        ? AVAHI_PROTO_INET6
        : AVAHI_PROTO_INET;
    std::string name = contact.get_name();
    // FIXME: from contact (endpoint.protocol())
    std::string type = "_" + contact.get_type() + "._tcp";
    // Use .local
    const char *domain = 0;
    // Host name
    boost::asio::ip::address address = endpoint.address();
    std::string host = address.is_unspecified()
        ? "" // Use default host name
        : address.to_string();

    property_list properties;
    if (!contact.get_properties().empty())
    {
        properties = contact.get_properties();
        if (properties == 0)
        {
            boost::system::error_code error(boost::system::errc::not_enough_memory,
                                            boost::system::system_category());
            handler(error);
            return;
        }
    }

    while (true)
    {
        int rc = avahi_entry_group_add_service_strlst(ptr,
                                                      interface_index,
                                                      protocol,
                                                      flags,
                                                      name.c_str(),
                                                      type.c_str(),
                                                      domain,
                                                      host.empty() ? 0 : host.c_str(),
                                                      endpoint.port(),
                                                      properties);
        if (rc == AVAHI_ERR_COLLISION)
        {
            char *alternative = avahi_alternative_service_name(name.c_str());
            name = alternative;
            avahi_free(alternative);
        }
        else if (rc != AVAHI_OK)
        {
            handler(convert_error(rc));
            return;
        }
        else
        {
            break;
        }
    }
    commit(ptr);
}

void announcer::commit(AvahiEntryGroup *group)
{
    assert(group != 0);

    if (!avahi_entry_group_is_empty(group))
    {
        int rc = avahi_entry_group_commit(group);
        if (rc != AVAHI_OK)
        {
            handler(convert_error(rc));
        }
    }
}

void announcer::on_established(AvahiEntryGroup *group)
{
    boost::system::error_code success;
    handler(success);
}

void announcer::on_collision(AvahiEntryGroup *group)
{
    // FIXME
}

void announcer::on_failure(AvahiEntryGroup *group)
{
    boost::system::error_code error(avahi_client_errno(avahi_entry_group_get_client(group)),
                                    boost::system::system_category());
    handler(error);
}

} // namespace avahi
} // namespace detail
} // namespace 

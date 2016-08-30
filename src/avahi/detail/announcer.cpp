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
#include <avahi-common/error.h>
#include <avahi-common/strlst.h>
#include <avahi-common/alternative.h>
#include <avahi-common/malloc.h>
#include <avahi-client/publish.h>
#include <avahi-client/client.h>
#include <boost/system/system_error.hpp>
#include <aware/detail/utility.hpp>
#include <aware/avahi/error.hpp>
#include <aware/avahi/detail/client.hpp>
#include <aware/avahi/detail/announcer.hpp>

namespace aware
{
namespace avahi
{
namespace detail
{

//-----------------------------------------------------------------------------
// announcer::wrapper
//-----------------------------------------------------------------------------

struct announcer::wrapper
{
    static void entry_group_callback(AvahiEntryGroup *group,
                                     AvahiEntryGroupState state,
                                     void *userdata)
    {
        aware::avahi::detail::announcer *self = static_cast<aware::avahi::detail::announcer *>(userdata);

        switch (state)
        {
        case AVAHI_ENTRY_GROUP_ESTABLISHED:
            {
                boost::system::error_code success;
                self->handler(success);
            }
            break;

        case AVAHI_ENTRY_GROUP_COLLISION:
        case AVAHI_ENTRY_GROUP_FAILURE:
            {
                self->handler(avahi::error::make_error_code(avahi_client_errno(avahi_entry_group_get_client(group))));
            }
            break;

        default:
            break;
        }
    }
};

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

announcer::announcer(const aware::avahi::detail::client& client)
    : ptr(0)
{
    ptr = avahi_entry_group_new(client,
                                wrapper::entry_group_callback,
                                this);
    if (ptr == 0)
        throw boost::system::system_error(avahi::error::make_error_code(AVAHI_ERR_DISCONNECTED));

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
    const aware::contact::endpoint_type endpoint = contact.endpoint();
    // Use all network interfaces
    const AvahiIfIndex interface_index = AVAHI_IF_UNSPEC;
    // Use only a specific protocol
    const AvahiProtocol protocol =
        contact.endpoint().protocol() == boost::asio::ip::tcp::v6()
        ? AVAHI_PROTO_INET6
        : AVAHI_PROTO_INET;
    std::string name = contact.name();
    std::string type = aware::detail::type_encode(contact.type());
    // Use .local
    const char *domain = 0;
    // Host name
    boost::asio::ip::address address = endpoint.address();
    std::string host = address.is_unspecified()
        ? "" // Use default host name
        : address.to_string();

    property_list properties;
    if (!contact.properties().empty())
    {
        properties = contact.properties();
        if (properties == 0)
        {
            handler(avahi::error::make_error_code(AVAHI_ERR_NO_MEMORY));
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
            handler(avahi::error::make_error_code(rc));
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
            handler(avahi::error::make_error_code(rc));
        }
    }
}

} // namespace avahi
} // namespace detail
} // namespace 

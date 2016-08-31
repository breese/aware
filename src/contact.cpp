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

#include <aware/contact.hpp>

namespace aware
{

// FIXME: Verify parameters against RFC 6762

contact::contact(const std::string& type)
    : contact_type(type),
      contact_index(-1)
{
}

contact::contact(const contact& other)
    : contact_name(other.contact_name),
      contact_type(other.contact_type),
      contact_index(other.contact_index),
      contact_endpoint(other.contact_endpoint),
      contact_properties(other.contact_properties)
{
}

contact& contact::operator= (const contact& other)
{
    if (&other != this)
    {
        contact_name = other.contact_name;
        contact_type = other.contact_type;
        contact_index = other.contact_index;
        contact_endpoint = other.contact_endpoint;
        contact_properties = other.contact_properties;
    }
    return *this;
}

contact& contact::name(const std::string& n)
{
    contact_name = n;
    return *this;
}

contact& contact::endpoint(const endpoint_type& e)
{
    contact_endpoint = e;
    return *this;
}

contact& contact::index(int i)
{
    contact_index = i;
    return *this;
}

contact& contact::properties(const property_map_type& p)
{
    contact_properties = p;
    return *this;
}

const std::string& contact::name() const
{
    return contact_name;
}

bool contact::empty() const
{
    return contact_endpoint.address().is_unspecified();
}

const std::string& contact::type() const
{
    return contact_type;
}

const contact::endpoint_type& contact::endpoint() const
{
    return contact_endpoint;
}

int contact::index() const
{
    return contact_index;
}

const contact::property_map_type& contact::properties() const
{
    return contact_properties;
}

} // namespace aware

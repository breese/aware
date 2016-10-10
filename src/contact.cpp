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
      contact_index(wildcard)
{
}

contact::contact(const contact& other)
    : contact_type(other.contact_type),
      contact_name(other.contact_name),
      contact_domain(other.contact_domain),
      contact_index(other.contact_index),
      contact_endpoint(other.contact_endpoint),
      contact_properties(other.contact_properties)
{
}

contact& contact::operator= (const contact& other)
{
    if (&other != this)
    {
        contact_type = other.contact_type;
        contact_name = other.contact_name;
        contact_domain = other.contact_domain;
        contact_index = other.contact_index;
        contact_endpoint = other.contact_endpoint;
        contact_properties = other.contact_properties;
    }
    return *this;
}

contact& contact::name(const std::string& value)
{
    contact_name = value;
    return *this;
}

contact& contact::domain(const std::string& value)
{
    contact_domain = value;
    return *this;
}

contact& contact::endpoint(const endpoint_type& value)
{
    contact_endpoint = value;
    return *this;
}

contact& contact::index(int value)
{
    contact_index = value;
    return *this;
}

contact& contact::properties(const property_map_type& value)
{
    contact_properties = value;
    return *this;
}

bool contact::empty() const
{
    return contact_endpoint.address().is_unspecified();
}

const std::string& contact::type() const
{
    return contact_type;
}

const std::string& contact::name() const
{
    return contact_name;
}

const std::string& contact::domain() const
{
    return contact_domain;
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

bool contact::operator== (const contact& other) const
{
    return (((contact_index == other.contact_index) ||
             (contact_index == wildcard) ||
             (other.contact_index == wildcard)) &&
            (contact_type == other.contact_type) &&
            (contact_name == other.contact_name) &&
            (contact_domain == other.contact_domain));
}

bool contact::operator< (const contact& other) const
{
    return ((contact_index < other.contact_index) ||
            ((contact_index == other.contact_index) &&
             ((contact_type < other.contact_type) ||
              ((contact_type == other.contact_type) &&
               ((contact_name < other.contact_name) ||
                ((contact_name == other.contact_name) &&
                 (contact_domain < other.contact_domain)))))));
}

} // namespace aware

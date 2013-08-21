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

contact::contact()
{
}

contact::contact(const std::string& name,
                 const std::string& type)
    : name(name),
      type(type)
{
}

contact::contact(const std::string& name,
                 const std::string& type,
                 const endpoint_type& endpoint)
    : name(name),
      type(type),
      endpoint(endpoint)
{
}

contact::contact(const std::string& name,
                 const std::string& type,
                 const endpoint_type& endpoint,
                 const property_map_type& properties)
    : name(name),
      type(type),
      endpoint(endpoint),
      properties(properties)
{
}

} // namespace aware

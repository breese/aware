#ifndef AWARE_CONTACT_HPP
#define AWARE_CONTACT_HPP

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
#include <boost/asio/ip/tcp.hpp>

namespace aware
{

class contact
{
public:
    typedef boost::asio::ip::tcp::endpoint endpoint_type;
    typedef std::map<std::string, std::string> property_map_type;

    contact();
    contact(const std::string& name,
            const std::string& type);
    contact(const std::string& name,
            const std::string& type,
            const endpoint_type& endpoint);
    contact(const std::string& name,
            const std::string& type,
            const endpoint_type& endpoint,
            const property_map_type& properties);

    const std::string& get_name() const { return name; }
    const std::string& get_type() const { return type; }
    const endpoint_type& get_endpoint() const { return endpoint; }
    const property_map_type& get_properties() const { return properties; }

private:
    std::string name;
    std::string type;
    endpoint_type endpoint;
    property_map_type properties;
};

} // namespace aware

#endif // AWARE_CONTACT_HPP

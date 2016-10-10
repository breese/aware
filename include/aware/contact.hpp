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

    contact(const std::string&);
    contact(const contact&);
    contact& operator= (const contact&);

    contact& name(const std::string&);
    contact& domain(const std::string&);
    contact& endpoint(const endpoint_type&);
    contact& index(int);
    contact& properties(const property_map_type&);

    bool empty() const;
    const std::string& type() const;
    const std::string& name() const;
    const std::string& domain() const;
    const endpoint_type& endpoint() const;
    int index() const;
    const property_map_type& properties() const;

    bool operator== (const contact&) const;
    bool operator< (const contact&) const;

public:
    static const int wildcard = -1;

private:
    std::string contact_type;
    std::string contact_name;
    std::string contact_domain;
    int contact_index;
    endpoint_type contact_endpoint;
    property_map_type contact_properties;
};

} // namespace aware

#endif // AWARE_CONTACT_HPP

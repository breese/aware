///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2015 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <aware/contact.hpp>
#include <aware/avahi/announce_socket.hpp>

void announced(const boost::system::error_code& error,
               aware::contact& contact)
{
    std::cout << "Announced: " << error << std::endl;
    if (!error)
    {
        std::cout << contact.type() << " = " << contact.name() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <type> <name>" << std::endl;
        return 1;
    }

    aware::contact::property_map_type properties;
    properties["key"] = "value";
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 3834);
    aware::contact contact = aware::contact(argv[1])
        .name(argv[2])
        .endpoint(endpoint)
        .properties(properties);

    boost::asio::io_service io;
    aware::avahi::announce_socket announcer(io);
    announcer.async_announce(contact,
                             boost::bind(&announced,
                                         _1,
                                         boost::ref(contact)));
    io.run();
    return 0;
}

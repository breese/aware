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

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/io_service.hpp>
#include <aware/contact.hpp>
#include <aware/avahi/monitor_socket.hpp>

class my_monitor
{
public:
    my_monitor(boost::asio::io_service& io)
        : socket(io)
    {}

    void async_listen(aware::contact& contact)
    {
        socket.async_listen(contact,
                            boost::bind(&my_monitor::process_listen,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::ref(contact)));
    }

private:
    void process_listen(const boost::system::error_code& error,
                        aware::contact& contact)
    {
        if (!error)
        {
            std::cout << "Entry:" << std::endl;
            std::cout << "  name = " << contact.name() << std::endl;
            std::cout << "  type = " << contact.type() << std::endl;
            std::cout << "  endpoint = " << contact.endpoint() << std::endl;
            aware::contact::property_map_type properties = contact.properties();
            for (aware::contact::property_map_type::const_iterator it = properties.begin();
                 it != properties.end();
                 ++it)
            {
                std::cout << "  " << it->first << " = " << it->second << std::endl;
            }
            // Launch the next listen
            async_listen(contact);
        }
        else if (error == boost::asio::error::operation_aborted)
        {
            // Ignore
        }
        else
        {
            std::cout << "Error = " << error << std::endl;
        }
    }

private:
    aware::avahi::monitor_socket socket;
};

int main(int argc, char *argv[])
{
    aware::contact contact = aware::contact().type("announce");
    boost::asio::io_service io;
    my_monitor monitor(io);
    monitor.async_listen(contact);
    io.run();
    return 0;
}

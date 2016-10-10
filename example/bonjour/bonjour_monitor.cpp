///////////////////////////////////////////////////////////////////////////////
//
// http://github.com/breese/aware
//
// Copyright (C) 2016 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <aware/contact.hpp>
#include <aware/bonjour/monitor_socket.hpp>

class my_monitor
{
public:
    my_monitor(boost::asio::io_service& io)
        : socket(io)
    {
    }

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
            if (contact.empty())
            {
                std::cout << "Removed:" << std::endl;
                std::cout << "  type     = " << contact.type() << std::endl;
                std::cout << "  name     = " << contact.name() << std::endl;
                std::cout << "  index    = " << contact.index() << std::endl;
            }
            else
            {
                std::cout << "Added:" << std::endl;
                std::cout << "  type     = " << contact.type() << std::endl;
                std::cout << "  name     = " << contact.name() << std::endl;
                std::cout << "  index    = " << contact.index() << std::endl;
                std::cout << "  endpoint = " << contact.endpoint() << std::endl;
                aware::contact::property_map_type properties = contact.properties();
                for (aware::contact::property_map_type::const_iterator it = properties.begin();
                     it != properties.end();
                     ++it)
                {
                    std::cout << "  " << it->first << " = " << it->second << std::endl;
                }
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
            std::cout << "Error: " << error.message() << std::endl;
        }
    }

private:
    aware::bonjour::monitor_socket socket;
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <type>" << std::endl;
        return 1;
    }
    aware::contact contact(argv[1]);
    boost::asio::io_service io;
    my_monitor monitor(io);
    monitor.async_listen(contact);
    io.run();
    return 0;
}

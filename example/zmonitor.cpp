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

#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <aware/io_service.hpp>
#include <aware/contact.hpp>
#include <aware/monitor_socket.hpp>

class my_monitor
{
public:
    my_monitor(aware::io_service& io)
        : io(io),
          socket(io)
    {}

    void async_listen(const aware::contact& contact)
    {
        socket.async_listen(contact,
                            boost::bind(&my_monitor::process_listen,
                                        this,
                                        boost::asio::placeholders::error,
                                        _2));
    }

private:
    void process_listen(const boost::system::error_code& error,
                        const aware::contact& contact)
    {
        switch (error.value())
        {
        case 0:
            {
                std::cout << "Entry:" << std::endl;
                std::cout << "  name = " << contact.get_name() << std::endl;
                std::cout << "  type = " << contact.get_type() << std::endl;
                std::cout << "  endpoint = " << contact.get_endpoint() << std::endl;
                aware::contact::property_map_type properties = contact.get_properties();
                for (aware::contact::property_map_type::const_iterator it = properties.begin();
                     it != properties.end();
                     ++it)
                {
                    std::cout << "  " << it->first << " = " << it->second << std::endl;
                }
                // Launch the next listen
                async_listen(contact);
            }
            break;

        case boost::asio::error::operation_aborted:
            break;

        default:
            std::cout << "Error = " << error << std::endl;
            break;
        }
    }

private:
    aware::io_service& io;
    aware::monitor_socket socket;
};

int main(int argc, char *argv[])
{
    aware::contact contact("", "announce");
    boost::asio::io_service io;
    aware::io_service aio(io);
    my_monitor monitor(aio);
    monitor.async_listen(contact);
    io.run();
    return 0;
}

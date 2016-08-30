#include <iostream>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/asio/io_service.hpp>
#include <aware/contact.hpp>
#include <aware/avahi/monitor_socket.hpp>

void AwareHandler(const boost::system::error_code& error,
                  aware::contact& contact,
                  aware::monitor_socket& socket)
{
    // The device described by contact has joined or left the network
    std::cout << contact.type() << " = " << contact.endpoint() << std::endl;
    // Listen for next event
    socket.async_listen(contact,
                        boost::bind(AwareHandler,
                                    _1,
                                    boost::ref(contact),
                                    boost::ref(socket)));
}

int main()
{
    boost::asio::io_service io;
    aware::avahi::monitor_socket socket(io);
    // Listen for "_announce._tcp" announcements
    aware::contact contact = aware::contact().type("announce");
    socket.async_listen(contact,
                        boost::bind(AwareHandler,
                                    _1,
                                    boost::ref(contact),
                                    boost::ref(socket)));
    io.run();
    return 0;
}

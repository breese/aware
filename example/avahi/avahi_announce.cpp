#include <string>
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <aware/contact.hpp>
#include <aware/avahi/announce_socket.hpp>

void announced(const boost::system::error_code& error)
{
    std::cout << __PRETTY_FUNCTION__ << " error=" << error << std::endl;
}

int main(int argc, char *argv[])
{
    aware::contact::property_map_type properties;
    properties["mac"] = "11:22:33:44:55:66";
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 3834);
    aware::contact contact = aware::contact("announce").name("This is an announcement").endpoint(endpoint).properties(properties);
    boost::asio::io_service io;
    aware::avahi::announce_socket announcer(io);
    announcer.async_announce(contact, announced);
    io.run();
    return 0;
}

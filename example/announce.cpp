#include <string>
#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <aware/io_service.hpp>
#include <aware/contact.hpp>
#include <aware/announce_socket.hpp>

void announced(const boost::system::error_code& error)
{
    std::cout << __PRETTY_FUNCTION__ << " error=" << error << std::endl;
}

int main(int argc, char *argv[])
{
    aware::contact::property_map_type properties;
    properties["mac"] = "11:22:33:44:55:66";
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 3834);
    aware::contact contact("This is an announcement", "announce", endpoint, properties);
    boost::asio::io_service io;
    aware::io_service aio(io);
    aware::announce_socket announcer(aio);
    announcer.async_announce(contact, announced);
    io.run();
    return 0;
}

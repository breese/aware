aware
=====

Device discovery with [Boost.Asio](http://www.boost.org/doc/libs/release/libs/asio/).

Notice that this project is continued in [trial.aware](https://github.com/breese/trial.aware).

Introduction
------------

The aware module discovers devices using [ZeroConf](http://en.wikipedia.org/wiki/Zeroconf).


An example of the use of aware:

```c++
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
```

Building aware with [Avahi](http://avahi.org/) backend
--------------

1. Make sure [CMake](http://cmake.org/) is installed.

2. Make sure that the libavahi-client-dev package is installed.

3. Checkout the repository.

4. Build the example

   ```bash
   cmake .
   make
   ```
5. Run the examples

   ```bash
   bin/avahi_monitor
   ```
   and from another shell:

   ```bash
   bin/avahi_announce
   ```

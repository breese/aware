aware
=====

Device discovery with [Boost.Asio](http://www.boost.org/doc/libs/release/libs/asio/).

Introduction
------------

The aware module discovers devices using [ZeroConf](http://en.wikipedia.org/wiki/Zeroconf).


An example of the use of aware:

```c++
#include <iostream>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <aware/io_service.hpp>
#include <aware/contact.hpp>
#include <aware/monitor_socket.hpp>

void AwareHandler(const boost::system::error_code& error,
                  const aware::contact& contact,
                  boost::shared_ptr<aware::monitor_socket> socket)
{
    // The device described by contact has joined or left the network
    std::cout << contact.get_type() << " = " << contact.get_endpoint() << std::endl;
    // Listen for next event
    socket->async_listen(contact,
                         boost::bind(AwareHandler,
                                     _1,
                                     _2,
                                     socket));
}

int main()
{
    boost::asio::io_service io;
    aware::io_service aio(io);
    boost::shared_ptr<aware::monitor_socket> socket = boost::make_shared<aware::monitor_socket>(boost::ref(aio));
    // Listen for "_announce._tcp" announcements
    aware::contact contact("", "announce");
    socket->async_listen(contact,
                         boost::bind(AwareHandler,
                                     _1,
                                     _2,
                                     socket));
    io.run();
    return 0;
}
```

Building aware
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
   bin/zmonitor
   ```
   and from another shell:

   ```bash
   bin/zannounce
   ```

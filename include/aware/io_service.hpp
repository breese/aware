#ifndef AWARE_IO_SERVICE_HPP
#define AWARE_IO_SERVICE_HPP

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

#include <boost/asio/io_service.hpp>

namespace aware
{

class io_service
{
public:
    io_service(boost::asio::io_service& io)
        : io(io)
    {}

    template <typename CompletionHandler>
    void post(CompletionHandler handler) { io.post(handler); }

    virtual ~io_service() {}

private:
    boost::asio::io_service& io;
};

} // namespace aware

#endif // AWARE_IO_SERVICE_HPP

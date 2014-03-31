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

#include <avahi-common/error.h>
#include <aware/avahi/detail/error.hpp>

namespace aware
{
namespace avahi
{
namespace detail
{

// FIXME: We may need to define our own error codes
boost::system::error_code convert_error(int error)
{
    using namespace boost::asio::error;
    switch (error)
    {
    case AVAHI_OK:
        return boost::system::error_code();

    case AVAHI_ERR_INVALID_HOST_NAME:
        return make_error_code(name_too_long); // FIXME

    case AVAHI_ERR_NO_NETWORK:
        return make_error_code(network_unreachable);

    // FIXME: Add the rest

    default:
        return make_error_code(bad_descriptor); // FIXME
    }
}

} // namespace detail
} // namespace avahi
} // namespace aware

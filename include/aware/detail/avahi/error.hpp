#ifndef AWARE_DETAIL_AVAHI_ERROR_HPP
#define AWARE_DETAIL_AVAHI_ERROR_HPP

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

#include <boost/asio/error.hpp>

namespace aware
{
namespace detail
{
namespace avahi
{

boost::system::error_code convert_error(int error);

} // namespace avahi
} // namespace detail
} // namespace aware

#endif // AWARE_DETAIL_AVAHI_ERROR_HPP

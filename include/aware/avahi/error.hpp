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

#include <boost/system/error_code.hpp>

namespace aware
{
namespace avahi
{
namespace error
{

const boost::system::error_category& category();

inline boost::system::error_code make_error_code(int e)
{
    return boost::system::error_code(e, avahi::error::category());
}

} // namespace error
} // namespace avahi
} // namespace aware

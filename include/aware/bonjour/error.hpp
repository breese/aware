#ifndef AWARE_BONJOUR_ERROR_HPP
#define AWARE_BONJOUR_ERROR_HPP

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
namespace bonjour
{
namespace error
{

const boost::system::error_category& category();

inline boost::system::error_code make_error_code(int e)
{
    return boost::system::error_code(e, bonjour::error::category());
}

} // namespace error
} // namespace bonjour
} // namespace aware

#endif // AWARE_BONJOUR_ERROR_HPP

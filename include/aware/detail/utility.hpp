#ifndef AWARE_DETAIL_UTILITY_HPP
#define AWARE_DETAIL_UTILITY_HPP

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

#include <string>

namespace aware
{
namespace detail
{

std::string type_encode(const std::string&);
std::string type_decode(const char *);

} // namespace detail
} // namespace aware

#endif // AWARE_DETAIL_UTILITY_HPP

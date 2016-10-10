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

#include <boost/system/system_error.hpp>
#include <aware/bonjour/error.hpp>
#include <aware/bonjour/detail/throw_on_error.hpp>

namespace aware
{
namespace bonjour
{
namespace detail
{

void throw_on_error(int error)
{
    if (error)
        throw boost::system::system_error(bonjour::error::make_error_code(error));
}

} // namespace detail
} // namespace bonjour
} // namespace aware

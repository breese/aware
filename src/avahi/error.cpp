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

#include <avahi-common/error.h>
#include <aware/avahi/error.hpp>

namespace aware
{
namespace avahi
{
namespace error
{

class avahi_category : public boost::system::error_category
{
public:
    const char *name() const BOOST_SYSTEM_NOEXCEPT
    {
        return "avahi";
    }
    std::string message(int value) const
    {
        return avahi_strerror(value);
    }
};

const boost::system::error_category& category()
{
    static avahi_category instance;
    return instance;
}

} // namespace error
} // namespace avahi
} // namespace aware

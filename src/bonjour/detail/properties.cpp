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

#include <boost/static_assert.hpp>
#include <aware/bonjour/detail/properties.hpp>
#include <aware/bonjour/detail/throw_on_error.hpp>
#include "dns_sd.hpp"

namespace aware
{
namespace bonjour
{
namespace detail
{

properties::properties(const map_type& input)
{
    BOOST_STATIC_ASSERT_MSG(properties::aligned_record::size >= sizeof(::TXTRecordRef),
                            "Increase properties::record_size");

    ::TXTRecordCreate(reinterpret_cast<TXTRecordRef*>(record.address()), 0, 0);
    for (map_type::const_iterator it = input.begin();
         it != input.end();
         ++it)
    {
        const std::string& key = it->first;
        const std::string& value = it->second;
        ::DNSServiceErrorType error = ::TXTRecordSetValue(reinterpret_cast<TXTRecordRef*>(record.address()),
                                                          key.data(),
                                                          value.size(),
                                                          value.data());
        throw_on_error(error);
    }
}

properties::~properties()
{
    ::TXTRecordDeallocate(reinterpret_cast<TXTRecordRef*>(record.address()));
}

std::size_t properties::size() const
{
    return ::TXTRecordGetLength(reinterpret_cast<const TXTRecordRef*>(record.address()));
}

const void *properties::data() const
{
    return ::TXTRecordGetBytesPtr(reinterpret_cast<const TXTRecordRef*>(record.address()));
}

} // namespace detail
} // namespace bonjour
} // namespace aware

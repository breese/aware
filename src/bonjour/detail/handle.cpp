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

#include "dns_sd.hpp"
#include <cassert>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <aware/bonjour/detail/throw_on_error.hpp>
#include <aware/bonjour/detail/handle.hpp>

namespace aware
{
namespace bonjour
{
namespace detail
{

struct handle::opaque_type
{
    opaque_type()
        : ref(0)
    {
    }

    opaque_type(DNSServiceRef ref)
        : ref(ref)
    {
    }

    ~opaque_type()
    {
        if (ref)
        {
            ::DNSServiceRefDeallocate(ref);
        }
    }

    BOOST_STATIC_ASSERT(boost::is_pointer<DNSServiceRef>::value);
    DNSServiceRef ref;
};

handle::handle()
    : opaque(new opaque_type)
{
}

handle::handle(with_connection_tag)
    : opaque(0)
{
    ::DNSServiceRef ref = 0;
    ::DNSServiceErrorType error = ::DNSServiceCreateConnection(&ref);
    throw_on_error(error);
    opaque = new opaque_type(ref);
}

handle::~handle()
{
    assert(opaque);
    delete opaque;
}

template <>
::DNSServiceRef handle::get<DNSServiceRef>()
{
    if (empty())
        throw_on_error(kDNSServiceErr_NotInitialized);
    return opaque->ref;
}

template <>
const ::DNSServiceRef& handle::get<const DNSServiceRef>() const
{
    if (empty())
        throw_on_error(kDNSServiceErr_NotInitialized);
    return opaque->ref;
}

void handle::reset()
{
    if (!empty())
    {
        delete opaque;
        opaque = new opaque_type;
    }
}

template <>
void handle::reset(const DNSServiceRef& ref)
{
    if (empty())
    {
        opaque->ref = ref;
    }
    else
    {
        delete opaque;
        opaque = new opaque_type(ref);
    }
}

bool handle::empty() const
{
    assert(opaque);
    return (opaque->ref == 0);
}

handle::native_handle_type handle::native_handle()
{
    if (empty())
        throw_on_error(kDNSServiceErr_NotInitialized);
    int result = ::DNSServiceRefSockFD(get<DNSServiceRef>());
    if (result == -1)
        throw_on_error(kDNSServiceErr_NotInitialized);
    return result;
}

} // namespace detail
} // namespace bonjour
} // namespace aware

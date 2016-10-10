#ifndef AWARE_BONJOUR_DETAIL_HANDLE_HPP
#define AWARE_BONJOUR_DETAIL_HANDLE_HPP

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

#include <boost/noncopyable.hpp>

namespace aware
{
namespace bonjour
{
namespace detail
{

// Type-erased RAII wrapper for DNSServiceRef
class handle : boost::noncopyable
{
public:
    typedef int native_handle_type;
    struct with_connection_tag {};
    static with_connection_tag with_connection;

    handle();
    handle(with_connection_tag);
    ~handle();

    template <typename T>
    T get();

    template <typename T>
    const T& get() const;

    void reset();

    template <typename T>
    void reset(const T&);

    bool empty() const;

    native_handle_type native_handle();

private:
    struct opaque_type;
    opaque_type *opaque;
};

} // namespace detail
} // namespace bonjour
} // namespace aware

#endif // AWARE_BONJOUR_DETAIL_HANDLE_HPP

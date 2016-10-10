#ifndef AWARE_BONJOUR_DETAIL_PROPERTIES_HPP
#define AWARE_BONJOUR_DETAIL_PROPERTIES_HPP

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
#include <map>
#include <boost/type_traits/aligned_storage.hpp>

namespace aware
{
namespace bonjour
{
namespace detail
{

class properties
{
public:
    typedef std::map<std::string, std::string> map_type;

    properties(const map_type& input);
    ~properties();

    std::size_t size() const;
    const void *data() const;

private:
    static const std::size_t record_size = 16;
    typedef boost::aligned_storage<record_size, sizeof(void *)> aligned_record;
    aligned_record::type record;
};

} // namespace detail
} // namespace bonjour
} // namespace aware

#endif // AWARE_BONJOUR_DETAIL_PROPERTIES_HPP

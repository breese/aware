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

#include <cstring>
#include <aware/detail/utility.hpp>

namespace aware
{
namespace detail
{

std::string type_encode(const std::string& type)
{
    return "_" + type + "._tcp";
}

// Extract "type" from a "_type._tcp" string.
// Returns empty string if a parse error occurred.
std::string type_decode(const char *type)
{
  const char *beginning = type;
  const char *ending = type;
  if (*beginning == '_')
  {
    ++beginning;
    ending = std::strchr(beginning, '.');
    if (ending == 0)
    {
      ending = beginning;
    }
  }
  return std::string(beginning, ending);
}

} // namespace detail
} // namespace aware

// Boost.Signals2 library

// Copyright Frank Mori Hess 2007-2010.
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org
#pragma once

#ifndef BOIST_SIGNALS2_EXPIRED_SLOT_HPP
#define BOIST_SIGNALS2_EXPIRED_SLOT_HPP

#include <memory>

namespace boist
{
  namespace signals2
  {
    class expired_slot: public std::bad_weak_ptr
    {
    public:
      virtual char const * what() const throw()
      {
        return "boist::signals2::expired_slot";
      }
    };
  }
} // end namespace boist

#endif // BOOST_SIGNALS2_EXPIRED_SLOT_HPP

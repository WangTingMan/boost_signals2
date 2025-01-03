// last_value function object (documented as part of Boost.Signals)

// Copyright Frank Mori Hess 2007.
// Copyright Douglas Gregor 2001-2003. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org
#pragma once

#ifndef BOIST_SIGNALS2_LAST_VALUE_HPP
#define BOIST_SIGNALS2_LAST_VALUE_HPP

#include <boost_signals2/expired_slot.hpp>

#include <optional>
#include <stdexcept>

namespace boist {
  namespace signals2 {

    // no_slots_error is thrown when we are unable to generate a return value
    // due to no slots being connected to the signal.
    class no_slots_error: public std::exception
    {
    public:
      virtual const char* what() const throw() {return "boist::signals2::no_slots_error";}
    };

    template<typename T>
    class last_value {
    public:
      typedef T result_type;

      template<typename InputIterator>
      T operator()(InputIterator first, InputIterator last) const
      {
        if(first == last)
        {
          throw (no_slots_error());
        }
        std::optional<T> value;
        while (first != last)
        {
          try
          {
            value = *first;
          }
          catch(const expired_slot &) {}
          ++first;
        }
        if(value) return value.value();
        throw (no_slots_error());
      }
    };

    template<>
    class last_value<void> {
    public:
      typedef void result_type;
      template<typename InputIterator>
        result_type operator()(InputIterator first, InputIterator last) const
      {
        while (first != last)
        {
          try
          {
            *first;
          }
          catch(const expired_slot &) {}
          ++first;
        }
        return;
      }
    };
  } // namespace signals2
} // namespace boist
#endif // BOOST_SIGNALS2_LAST_VALUE_HPP

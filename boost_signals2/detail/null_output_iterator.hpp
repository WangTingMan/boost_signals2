/*
  An output iterator which simply discards output.
*/
// Copyright Frank Mori Hess 2008.
// Distributed under the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/signals2 for library home page.
#pragma once

#ifndef BOIST_SIGNALS2_NULL_OUTPUT_ITERATOR_HPP
#define BOIST_SIGNALS2_NULL_OUTPUT_ITERATOR_HPP

namespace boist
{
  namespace signals2
  {
    namespace detail
    {
      class does_nothing
      {
      public:
        template<typename T>
          void operator()(const T&) const
          {}
      };

      class null_output_proxy
      {

      public:

          template <class T>
          null_output_proxy& operator=( T&& value )
          {
              return *this;
          }
      };

      class null_output_iterator
      {

      public:

          null_output_proxy operator*() { return null_output_proxy(); }

          null_output_iterator& operator++() { return *this; }
          null_output_iterator& operator++( int ) { return *this; }
      };

    } // namespace detail
  } // namespace signals2
} // namespace boist

#endif  // BOOST_SIGNALS2_NULL_OUTPUT_ITERATOR_HPP

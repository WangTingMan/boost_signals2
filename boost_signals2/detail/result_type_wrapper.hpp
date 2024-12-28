// Boost.Signals2 library

// Copyright Douglas Gregor 2001-2004.
// Copyright Frank Mori Hess 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org
#pragma once

#ifndef BOIST_SIGNALS2_RESULT_TYPE_WRAPPER_HPP
#define BOIST_SIGNALS2_RESULT_TYPE_WRAPPER_HPP

namespace boist {
  namespace signals2 {
    namespace detail {
      // A placeholder for void on compilers that don't support void returns
      struct void_type {};

      // Replaces void with void_type
      template<typename R>
      struct nonvoid {
        typedef R type;
      };
      template<>
      struct nonvoid<void> {
        typedef void_type type;
      };

      // Replaces void with void_type only if compiler doesn't support void returns
      template<typename R>
      struct result_type_wrapper {
        typedef R type;
      };

      // specialization deals with possible void return from combiners
      template<typename R> class combiner_invoker
      {
      public:
        typedef R result_type;
        template<typename Combiner, typename InputIterator>
          result_type operator()(Combiner &combiner,
          InputIterator first, InputIterator last) const
        {
          return combiner(first, last);
        }
      };
      template<> class combiner_invoker<void>
      {
      public:
        typedef result_type_wrapper<void>::type result_type;
        template<typename Combiner, typename InputIterator>
          result_type operator()(Combiner &combiner,
          InputIterator first, InputIterator last) const
        {
          combiner(first, last);
          return result_type();
        }
      };
    } // end namespace detail
  } // end namespace signals2
} // end namespace boist

#endif // BOOST_SIGNALS2_RESULT_TYPE_WRAPPER_HPP

// Boost.Signals library

// Copyright Douglas Gregor 2001-2004.
// Copyright Frank Mori Hess 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org
#pragma once

#ifndef BOIST_SIGNALS2_SIGNALS_COMMON_HPP
#define BOIST_SIGNALS2_SIGNALS_COMMON_HPP

#include <boost_signals2/signal_base.hpp>

#include <type_traits>

namespace boist {
  namespace signals2 {
    namespace detail {


    template<typename T> struct is_std_reference_wrapper
    {
        using type = std::false_type;
    };

    template<typename T> struct is_std_reference_wrapper<std::reference_wrapper<T>>
    {
        using type = std::true_type;
    };

    template<typename T> struct is_std_reference_wrapper<std::reference_wrapper<T> const>
    {
        using type = std::true_type;
    };

    template<typename T> struct is_std_reference_wrapper<std::reference_wrapper<T> volatile>
    {
        using type = std::true_type;
    };

    template<typename T> struct is_std_reference_wrapper<std::reference_wrapper<T> const volatile>
    {
        using type = std::true_type;
    };

      // A slot can be a signal, a reference to a function object, or a
      // function object.
      struct signal_tag {};
      struct reference_tag {};
      struct value_tag {};

      // Classify the given slot as a signal, a reference-to-slot, or a
      // standard slot
      template<typename S>
      class get_slot_tag {
      public:
          typedef typename std::conditional<std::is_base_of_v<signal_base,std::decay_t<S>>,
              signal_tag, reference_tag>::type type;
      };

      // Get the slot so that it can be copied
      template<typename F>
      typename F::weak_signal_type
      get_invocable_slot(const F &signal, signal_tag)
      { return typename F::weak_signal_type(signal); }

      template<typename F>
      const F&
      get_invocable_slot(const F& f, reference_tag)
      { return f; }

      template<typename F>
      const F&
      get_invocable_slot(const F& f, value_tag)
      { return f; }

      // Determines the type of the slot - is it a signal, a reference to a
      // slot or just a normal slot.
      template<typename F>
      typename get_slot_tag<F>::type
      tag_type(const F&)
      {
        typedef typename get_slot_tag<F>::type
          the_tag_type;
        the_tag_type tag = the_tag_type();
        return tag;
      }
    } // end namespace detail
  } // end namespace signals2
} // end namespace boist

#endif // BOOST_SIGNALS2_SIGNALS_COMMON_HPP

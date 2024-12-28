/*
  A thread-safe version of Boost.Signals.

  Author: Frank Mori Hess <fmhess@users.sourceforge.net>
  Begin: 2007-01-23
*/
// Copyright Frank Mori Hess 2007-2008
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org
#pragma once

#ifndef BOIT_SIGNALS2_PREPROCESSED_SIGNAL_HPP
#define BOIST_SIGNALS2_PREPROCESSED_SIGNAL_HPP

#include <boost_signals2/detail/preprocessed_arg_type.hpp> 
#include <boost_signals2/function_traits.hpp>
#include <boost_signals2/optional_last_value.hpp>
#include <boost_signals2/detail/signal_template.hpp>

#include <functional>
#include <mutex>
#include <type_traits>

namespace boist
{
  namespace signals2
  {
    template<typename Signature,
      typename Combiner = optional_last_value<typename boist::signals2::function_traits<Signature>::result_type>,
      typename Group = int,
      typename GroupCompare = std::less<Group>,
      typename SlotFunction = std::function<Signature>,
      typename ExtendedSlotFunction = typename detail::extended_signature<function_traits<Signature>::arity, Signature>::function_type,
      typename Mutex = std::mutex >
    class signal: public detail::signalN<function_traits<Signature>::arity,
      Signature, Combiner, Group, GroupCompare, SlotFunction, ExtendedSlotFunction, Mutex>::type
    {
    private:
      typedef typename detail::signalN<boist::function_traits<Signature>::arity,
        Signature, Combiner, Group, GroupCompare, SlotFunction, ExtendedSlotFunction, Mutex>::type base_type;
    public:
      signal(const Combiner &combiner_arg = Combiner(), const GroupCompare &group_compare = GroupCompare()):
        base_type(combiner_arg, group_compare)
      {}
    };
  }
}

#endif // BOOST_SIGNALS2_PREPROCESSED_SIGNAL_HPP

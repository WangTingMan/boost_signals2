//  A thread-safe version of Boost.Signals.

// Copyright Frank Mori Hess 2007-2009
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org
#pragma once

#ifndef BOIST_SIGNALS2_SIGNAL_HPP
#define BOIST_SIGNALS2_SIGNAL_HPP

#include <algorithm>

#include <boost_signals2/connection.hpp>
#include <boost_signals2/detail/replace_slot_function.hpp>
#include <boost_signals2/detail/result_type_wrapper.hpp>
#include <boost_signals2/detail/signals_common.hpp>
#include <boost_signals2/detail/signal_template.hpp>
#include <boost_signals2/detail/slot_groups.hpp>
#include <boost_signals2/detail/slot_call_iterator.hpp>
#include <boost_signals2/optional_last_value.hpp>
#include <boost_signals2/slot.hpp>

#include <functional>
#include <memory>
#include <boost_signals2/variadic_signal.hpp>

namespace boist
{
  namespace signals2
  {
    // free swap function, findable by ADL
    template<typename Signature,
      typename Combiner,
      typename Group,
      typename GroupCompare,
      typename SlotFunction,
      typename ExtendedSlotFunction,
      typename Mutex>
      void swap(
        signal<Signature, Combiner, Group, GroupCompare, SlotFunction, ExtendedSlotFunction, Mutex> &sig1,
        signal<Signature, Combiner, Group, GroupCompare, SlotFunction, ExtendedSlotFunction, Mutex> &sig2) noexcept
    {
      sig1.swap(sig2);
    }
  }
}

#endif // BOOST_SIGNALS2_SIGNAL_HPP

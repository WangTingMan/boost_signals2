// DEPRECATED in favor of adl_predestruct with deconstruct<T>().
// A simple framework for creating objects with predestructors.
// The objects must inherit from boist::signals2::predestructible, and
// have their lifetimes managed by
// boist::shared_ptr created with the boist::signals2::deconstruct_ptr()
// function.
//
// Copyright Frank Mori Hess 2007-2008.
//
//Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#ifndef BOIST_SIGNALS2_PREDESTRUCTIBLE_HPP
#define BOIST_SIGNALS2_PREDESTRUCTIBLE_HPP

#include <memory>

namespace boist
{
  namespace signals2
  {
    template<typename T> class predestructing_deleter;

    namespace predestructible_adl_barrier
    {
      class predestructible
      {
      protected:
        predestructible() {}
      public:
        template<typename T>
          friend void adl_postconstruct(const std::shared_ptr<T> &, ...)
        {}
        friend void adl_predestruct(predestructible *p)
        {
          p->predestruct();
        }
        virtual ~predestructible() {}
        virtual void predestruct() = 0;
      };
    } // namespace predestructible_adl_barrier
    using predestructible_adl_barrier::predestructible;
  }
}

#endif // BOOST_SIGNALS2_PREDESTRUCTIBLE_HPP

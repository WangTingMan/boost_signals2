// Boost.Signals2 library

// Copyright Frank Mori Hess 2007-2008.
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org
#pragma once

#ifndef BOIST_SIGNALS2_SIGNAL_BASE_HPP
#define BOIST_SIGNALS2_SIGNAL_BASE_HPP

#include <memory>

namespace boist {
  namespace signals2 {
    class slot_base;

    class signal_base
    {
    public:
      friend class slot_base;

      signal_base() = default;

      signal_base( const signal_base& ) = delete;
      signal_base& operator=( const signal_base& ) = delete;

      virtual ~signal_base() {}
    protected:
      virtual std::shared_ptr<void> lock_pimpl() const = 0;
    };
  } // end namespace signals2
} // end namespace boist

#endif // BOOST_SIGNALS2_SIGNAL_BASE_HPP

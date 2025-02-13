// DEPRECATED in favor of adl_postconstruct and adl_predestruct with
// deconstruct<T>().
// A factory function for creating a shared_ptr that enhances the plain
// shared_ptr constructors by adding support for postconstructors
// and predestructors through the boist::signals2::postconstructible and
// boist::signals2::predestructible base classes.
//
// Copyright Frank Mori Hess 2007-2008.
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

#ifndef BOIST_SIGNALS2_DECONSTRUCT_PTR_HPP
#define BOIST_SIGNALS2_DECONSTRUCT_PTR_HPP

#include <boost_signals2/postconstructible.hpp>
#include <boost_signals2/predestructible.hpp>

#include <cassert>
#include <memory>

namespace boist
{
  namespace signals2
  {
    namespace detail
    {
      inline void do_postconstruct(const postconstructible *ptr)
      {
        postconstructible *nonconst_ptr = const_cast<postconstructible*>(ptr);
        nonconst_ptr->postconstruct();
      }
      inline void do_postconstruct(...)
      {
      }
      inline void do_predestruct(...)
      {
      }
      inline void do_predestruct(const predestructible *ptr)
      {
        try
        {
          predestructible *nonconst_ptr = const_cast<predestructible*>(ptr);
          nonconst_ptr->predestruct();
        }
        catch(...)
        {
          assert(false);
        }
      }
    }

    template<typename T>
    class predestructing_deleter
    {
    public:
      void operator()(const T *ptr) const
      {
        detail::do_predestruct(ptr);
        delete ptr;
      }
    };

    template<typename T>
    std::shared_ptr<T> deconstruct_ptr(T *ptr)
    {
      if(ptr == 0) return std::shared_ptr<T>(ptr);
      std::shared_ptr<T> shared(ptr, boist::signals2::predestructing_deleter<T>());
      detail::do_postconstruct(ptr);
      return shared;
    }
    template<typename T, typename D>
    std::shared_ptr<T> deconstruct_ptr(T *ptr, D deleter)
    {
        std::shared_ptr<T> shared(ptr, deleter);
      if(ptr == 0) return shared;
      detail::do_postconstruct(ptr);
      return shared;
    }
  }
}

#endif // BOOST_SIGNALS2_DECONSTRUCT_PTR_HPP

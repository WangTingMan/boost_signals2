// Boost.Signals2 library

// Copyright Frank Mori Hess 2007-2008.
// Copyright Timmo Stange 2007.
// Copyright Douglas Gregor 2001-2004. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org
#pragma once

#ifndef BOIST_SIGNALS2_TRACKED_OBJECTS_VISITOR_HPP
#define BOIST_SIGNALS2_TRACKED_OBJECTS_VISITOR_HPP

#include <boost_signals2/detail/signals_common.hpp>
#include <boost_signals2/function_traits.hpp>
#include <boost_signals2/slot_base.hpp>
#include <boost_signals2/trackable.hpp>

#include <functional> 
#include <type_traits>

namespace boist
{
  namespace signals2
  {
    namespace detail
    {

      // Visitor to collect tracked objects from a bound function.
      class tracked_objects_visitor
      {
      public:

        tracked_objects_visitor(slot_base *slot) : slot_(slot)
        {}

        template<typename T>
        void operator()(const T& t) const
        {
            using check_type = is_std_reference_wrapper<T>::type;
            m_visit_reference_wrapper( t, check_type() );
        }

      private:

        template<typename T>
        void m_visit_reference_wrapper(const std::reference_wrapper<T> &t, std::true_type) const
        {
            m_visit_reference_wrapper(t.get(), std::false_type());
        }

        template<typename T>
        void m_visit_reference_wrapper(const T &t, std::false_type ) const
        {
            using check_result = std::conditional_t<std::is_pointer_v<T>, std::true_type, std::false_type>;
            if constexpr( function_traits<T>::is_normal_function || function_traits<T>::is_function_pointer )
            {
                // we do not track normal function. hmm...just want to avoid a build error with clang.
                return;
            }
            else
            {
                m_visit_pointer( t, check_result() );
            }
        }

        template<typename T>
        void m_visit_pointer(const T *t, std::true_type) const
        {
            using check_type = std::is_function<typename std::remove_pointer<T>::type>;
            m_visit_not_function_pointer(t, check_type());
        }

        template<typename T>
        void m_visit_pointer(const T &t, std::false_type) const
        {
            m_visit_pointer(std::addressof(t), std::true_type());
        }

        template<typename T>
        void m_visit_not_function_pointer(const T *t, std::true_type) const
        {
            using check_type = std::is_base_of<signal_base, std::decay_t<T>>::type;
            m_visit_signal(t, check_type());
        }

        template<typename T>
        void m_visit_not_function_pointer(const T *t, std::false_type) const
        {
            using check_type = std::is_base_of<signal_base, std::decay_t<T>>::type;
            m_visit_signal(t, check_type());
        }

        template<typename T>
        void m_visit_signal(const T *signal, std::true_type) const
        {
          if(signal)
            slot_->track_signal(*signal);
        }

        template<typename T>
        void m_visit_signal(const T &t, std::false_type) const
        {
            add_if_trackable(t);
        }

        void add_if_trackable(const trackable *trackable) const
        {
          if(trackable)
            slot_->_tracked_objects.push_back(trackable->get_weak_ptr());
        }

        void add_if_trackable(const void *) const {}

        mutable slot_base * slot_;
      };


    } // end namespace detail
  } // end namespace signals2
} // end namespace boist

#endif // BOOST_SIGNALS2_TRACKED_OBJECTS_VISITOR_HPP



//  helper code for dealing with tracking non-boost shared_ptr/weak_ptr

// Copyright Frank Mori Hess 2009.
// Distributed under the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/signals2 for library home page.
#pragma once

#ifndef BOIST_SIGNALS2_FOREIGN_PTR_HPP
#define BOIST_SIGNALS2_FOREIGN_PTR_HPP

#include <algorithm>
#include <memory>

namespace boist
{
  namespace signals2
  {
    template<typename WeakPtr>
    struct weak_ptr_traits
    {};

    template<typename T> struct weak_ptr_traits<std::weak_ptr<T> >
    {
      typedef std::shared_ptr<T> shared_type;
    };

    template<typename SharedPtr> struct shared_ptr_traits
    {};

    template<typename T> struct shared_ptr_traits<std::shared_ptr<T> >
    {
      typedef std::weak_ptr<T> weak_type;
    };

    namespace detail
    {
      struct foreign_shared_ptr_impl_base
      {
        virtual ~foreign_shared_ptr_impl_base() {}
        virtual foreign_shared_ptr_impl_base * clone() const = 0;
      };

      template<typename FSP>
      class foreign_shared_ptr_impl: public foreign_shared_ptr_impl_base
      {
      public:
        foreign_shared_ptr_impl(const FSP &p): _p(p)
        {}
        virtual foreign_shared_ptr_impl * clone() const
        {
          return new foreign_shared_ptr_impl(*this);
        }
      private:
        FSP _p;
      };

      class foreign_void_shared_ptr
      {
      public:
        foreign_void_shared_ptr():
          _p(0)
        {}
        foreign_void_shared_ptr(const foreign_void_shared_ptr &other):
          _p(other._p->clone())
        {}
        template<typename FSP>
        explicit foreign_void_shared_ptr(const FSP &fsp):
          _p(new foreign_shared_ptr_impl<FSP>(fsp))
        {}
        ~foreign_void_shared_ptr()
        {
          delete _p;
        }
        foreign_void_shared_ptr & operator=(const foreign_void_shared_ptr &other)
        {
          if(&other == this) return *this;
          foreign_void_shared_ptr(other).swap(*this);
          return *this;
        }
        void swap(foreign_void_shared_ptr &other)
        {
          std::swap(_p, other._p);
        }
      private:
        foreign_shared_ptr_impl_base *_p;
      };

      struct foreign_weak_ptr_impl_base
      {
        virtual ~foreign_weak_ptr_impl_base() {}
        virtual foreign_void_shared_ptr lock() const = 0;
        virtual bool expired() const = 0;
        virtual foreign_weak_ptr_impl_base * clone() const = 0;
      };

      template<typename FWP>
      class foreign_weak_ptr_impl: public foreign_weak_ptr_impl_base
      {
      public:
        foreign_weak_ptr_impl(const FWP &p): _p(p)
        {}
        virtual foreign_void_shared_ptr lock() const
        {
          return foreign_void_shared_ptr(_p.lock());
        }
        virtual bool expired() const
        {
          return _p.expired();
        }
        virtual foreign_weak_ptr_impl * clone() const
        {
          return new foreign_weak_ptr_impl(*this);
        }
      private:
        FWP _p;
      };

      class foreign_void_weak_ptr
      {
      public:
        foreign_void_weak_ptr()
        {}
        foreign_void_weak_ptr(const foreign_void_weak_ptr &other):
          _p(other._p->clone())
        {}
        template<typename FWP>
        explicit foreign_void_weak_ptr(const FWP &fwp):
          _p(new foreign_weak_ptr_impl<FWP>(fwp))
        {}
        foreign_void_weak_ptr & operator=(const foreign_void_weak_ptr &other)
        {
          if(&other == this) return *this;
          foreign_void_weak_ptr(other).swap(*this);
          return *this;
        }
        void swap(foreign_void_weak_ptr &other)
        {
          std::swap(_p, other._p);
        }
        foreign_void_shared_ptr lock() const
        {
          return _p->lock();
        }
        bool expired() const
        {
          return _p->expired();
        }
      private:
        std::shared_ptr<foreign_weak_ptr_impl_base> _p;
      };
    } // namespace detail

  } // namespace signals2
} // namespace boist

#endif  // BOOST_SIGNALS2_FOREIGN_PTR_HPP

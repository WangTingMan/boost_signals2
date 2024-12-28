#ifndef BOIST_SIGNALS2_DECONSTRUCT_HPP
#define BOIST_SIGNALS2_DECONSTRUCT_HPP
//  deconstruct.hpp
//
// A factory function for creating a shared_ptr which creates
// an object and its owning shared_ptr with one allocation, similar
// to make_shared<T>().  It also supports postconstructors
// and predestructors through unqualified calls of adl_postconstruct() and
// adl_predestruct, relying on argument-dependent
// lookup to find the appropriate postconstructor or predestructor.
// Passing arguments to postconstructors is also supported.
//
//  based on make_shared.hpp and make_shared_access patch from Michael Marcin
//
//  Copyright (c) 2007, 2008 Peter Dimov
//  Copyright (c) 2008 Michael Marcin
//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  See http://www.boost.org
//  for more information
#pragma once

#include <cstddef>
#include <memory>
#include <new>
#include <type_traits>

namespace boist
{
  template<typename T> class enable_shared_from_this;

namespace signals2
{
  class deconstruct_access;

namespace detail
{
  inline void adl_predestruct(...) {}
} // namespace detail

template<typename T>
class postconstructor_invoker
{
public:
    operator const std::shared_ptr<T> & () const
    {
        return postconstruct();
    }
    const std::shared_ptr<T>& postconstruct() const
    {
        if(!_postconstructed)
        {
            adl_postconstruct(_sp, const_cast<typename std::remove_const<T>::type *>(_sp.get()));
            _postconstructed = true;
        }
        return _sp;
    }

    template<class... Args>
      const std::shared_ptr<T>& postconstruct(Args && ... args) const
    {
        if(!_postconstructed)
        {
            adl_postconstruct(_sp, const_cast<typename std::remove_const<T>::type *>(_sp.get()),
                std::forward<Args>(args)...);
            _postconstructed = true;
        }
        return _sp;
    }
private:
    friend class boist::signals2::deconstruct_access;
    postconstructor_invoker(const std::shared_ptr<T> & sp):
        _sp(sp), _postconstructed(false)
    {}
    std::shared_ptr<T> _sp;
    mutable bool _postconstructed;
};

namespace detail
{

template< std::size_t N, std::size_t A > struct sp_aligned_storage
{
    union type
    {
        char data_[ N ];
        //typename boist::type_with_alignment< A >::type align_;
    };
};

template< class T > class deconstruct_deleter
{
private:

    typedef typename sp_aligned_storage< sizeof( T ), std::alignment_of< T >::value >::type storage_type;

    bool initialized_;
    storage_type storage_;

private:

    void destroy()
    {
        if( initialized_ )
        {
            T* p = reinterpret_cast< T* >( storage_.data_ );
            using boist::signals2::detail::adl_predestruct;
            adl_predestruct(const_cast<typename std::remove_const<T>::type *>(p));
            p->~T();
            initialized_ = false;
        }
    }

public:

    deconstruct_deleter(): initialized_( false )
    {
    }

    // this copy constructor is an optimization: we don't need to copy the storage_ member,
    // and shouldn't be copying anyways after initialized_ becomes true
    deconstruct_deleter(const deconstruct_deleter &): initialized_( false )
    {
    }

    ~deconstruct_deleter()
    {
        destroy();
    }

    void operator()( T * )
    {
        destroy();
    }

    void * address()
    {
        return storage_.data_;
    }

    void set_initialized()
    {
        initialized_ = true;
    }
};
}  // namespace detail

class deconstruct_access
{
public:

    template< class T >
    static postconstructor_invoker<T> deconstruct()
    {
        std::shared_ptr<T> pt( static_cast<T*>(0), detail::deconstruct_deleter<T>() );

        detail::deconstruct_deleter<T> * pd = std::get_deleter< detail::deconstruct_deleter< T > >( pt );

        void * pv = pd->address();

        new(pv) T();
        pd->set_initialized();

        std::shared_ptr<T> retval( pt, static_cast< T* >( pv ) );
        return retval;

    }

    // Variadic templates, rvalue reference

    template< class T, class... Args >
    static postconstructor_invoker<T> deconstruct( Args && ... args )
    {
        std::shared_ptr< T > pt( static_cast< T* >( 0 ), detail::deconstruct_deleter< T >() );

        detail::deconstruct_deleter< T > * pd = std::get_deleter< detail::deconstruct_deleter< T > >( pt );

        void * pv = pd->address();

        new( pv ) T( std::forward<Args>( args )... );
        pd->set_initialized();

        std::shared_ptr< T > retval( pt, static_cast< T* >( pv ) );
        return retval;
    }
};

// Zero-argument versions
//
// Used even when variadic templates are available because of the new T() vs new T issue

template< class T > postconstructor_invoker<T> deconstruct()
{
    return deconstruct_access::deconstruct<T>();
}

// Variadic templates, rvalue reference
template< class T, class... Args > postconstructor_invoker< T > deconstruct( Args && ... args )
{
    return deconstruct_access::deconstruct<T>( std::forward<Args>( args )... );
}

} // namespace signals2
} // namespace boist

#endif // #ifndef BOOST_SIGNALS2_DECONSTRUCT_HPP

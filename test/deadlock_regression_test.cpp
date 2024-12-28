// Signals2 library
//
// Regression test based on bug report from Arian Alin Radu.
// The problem was that tracked objects could be released 
// while holding the signal mutex during signal invocation.  
// This could result in a recursive
// lock attempt if the tracked object manipulates the signal
// in its destructor.

// Copyright Frank Mori Hess 2019
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE signals2 deadlock regression test
#include <boost/test/included/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost_signals2/signal.hpp>
#include <boost_signals2/signal_type.hpp>

namespace bs2 = boist::signals2;


// dummy mutex that detects attempts to recursively lock
class test_mutex
{
public:
    test_mutex() : m_locked( false ) {}
    void lock()
    {
        BOOST_CHECK( m_locked == false );
        m_locked = true;
    }
    bool try_lock()
    {
        if( m_locked ) return false;
        lock();
        return true;
    }
    void unlock()
    {
        m_locked = false;
    }
private:
    bool m_locked;
};

using default_last_value = bs2::optional_last_value<typename bs2::function_traits<void()>::result_type>;
using extend_function_tupe = bs2::detail::variadic_extended_signature<void()>::function_type;
using Signal = bs2::signal<void(), default_last_value, int, std::less<int>, std::function<void()>, extend_function_tupe, test_mutex>;

class SelfReference
{
public:
    std::shared_ptr<SelfReference> m_self;
    std::shared_ptr<Signal> m_signal;

    boist::signals2::connection m_conReleaseSelf;
    boist::signals2::connection m_conDoNothing;

    SelfReference()
    {
        m_signal = std::make_shared<Signal>();
    }

    ~SelfReference()
    {
        // the first slot (ReleaseSelf) has been called; now the trackable object (this)
        // was released, while the second slot is locked
        BOOST_CHECK( !m_conReleaseSelf.connected() );
        // the second slot is locked, and we enter a recursive (pthread: dead) lock
        BOOST_CHECK( m_conDoNothing.connected() );
        m_conReleaseSelf.disconnect();
        m_conDoNothing.disconnect();
        // enter recursive (pthread: dead) lock again:
        BOOST_CHECK( m_signal->empty() );
    }

    void ReleaseSelf()
    {
        m_self.reset();
    }

    static void DoNothing()
    {
    }

    static void Run()
    {
        std::shared_ptr<Signal> signal;
        {
            std::shared_ptr<SelfReference> obj = std::make_shared<SelfReference>();
            obj->m_self = obj;
            signal = obj->m_signal;

            obj->m_conReleaseSelf = signal->connect( Signal::slot_type( &SelfReference::ReleaseSelf, obj.get() ).track( obj ) );
            obj->m_conDoNothing = signal->connect( Signal::slot_type( &SelfReference::DoNothing ) );
        }
        ( *signal )( );
    }
};

BOOST_AUTO_TEST_CASE( test_main )
{
    SelfReference::Run();
}

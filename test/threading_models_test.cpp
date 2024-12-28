// thread_safe_signals library
// basic test for alternate threading models

// Copyright Frank Mori Hess 2008
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <boost_signals2/signals2.hpp>
#define BOOST_TEST_MODULE threading_models_test
#include <boost/test/included/unit_test.hpp>
#include <mutex>

// combiner that returns the number of slots invoked
struct slot_counter {
  typedef unsigned result_type;
  template<typename InputIterator>
  unsigned operator()(InputIterator first, InputIterator last) const
  {
    unsigned count = 0;
    for (; first != last; ++first)
    {
      try
      {
        *first;
        ++count;
      }
      catch(const std::bad_weak_ptr &)
      {}
    }
    return count;
  }
};

void myslot()
{
    int ui = 90;
    ui = 100;
}

template<typename signal_type>
void simple_test()
{
  signal_type sig;
  sig.connect(typename signal_type::slot_type(&myslot));
  BOOST_CHECK(sig() == 1);
  sig.disconnect(&myslot);
  BOOST_CHECK(sig() == 0);
}

class recursion_checking_dummy_mutex
{
  int recursion_count;
public:
  recursion_checking_dummy_mutex(): recursion_count(0)
  {}
  void lock() 
  { 
    BOOST_REQUIRE(recursion_count == 0);
    ++recursion_count;
  }
  bool try_lock() 
  { 
    lock(); 
    return true;
  }
  void unlock() 
  { 
    --recursion_count;
    BOOST_REQUIRE(recursion_count == 0);
  }
};

BOOST_AUTO_TEST_CASE(test_main)
{
  typedef boist::signals2::signal<void (), slot_counter, int, std::less<int>, std::function<void ()>,
    boost::function<void (const boist::signals2::connection &)>, recursion_checking_dummy_mutex> sig0_rc_type;
  simple_test<sig0_rc_type>();
   typedef boist::signals2::signal<void (), slot_counter, int, std::less<int>, std::function<void ()>,
     boost::function<void (const boist::signals2::connection &)>, std::mutex> sig0_mt_type;
   simple_test<sig0_mt_type>();
  typedef boist::signals2::signal<void (), slot_counter, int, std::less<int>, std::function<void ()>,
    boost::function<void (const boist::signals2::connection &)>, boist::signals2::dummy_mutex> sig0_st_type;
  simple_test<sig0_st_type>();
}

// Boost.Signals2 library

// Copyright Douglas Gregor 2001-2006.
// Copyright Frank Mori Hess 2009.
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <boost_signals2/signal.hpp>
#include <boost_signals2/trackable.hpp>
#define BOOST_TEST_MODULE trackable_test
#include <boost/test/included/unit_test.hpp>

using namespace std::placeholders;

struct short_lived : public boist::signals2::trackable {
  ~short_lived() {}
};

struct swallow {
  typedef int result_type;

  template<typename T> int operator()(const T*, int i) {
      return i;
  }
  template<typename T> int operator()(T &, int i) {
      return i;
  }
  template<typename T> int operator()(boost::weak_ptr<T>, int i) {
      return i;
  }
};

template<typename T>
struct max_or_default {
  typedef T result_type;

  template<typename InputIterator>
  T operator()(InputIterator first, InputIterator last) const
  {
    if (first == last)
      return T();

    T max = *first++;
    for (; first != last; ++first)
      max = (*first > max)? *first : max;

    return max;
  }
};

struct self_deleting : public boist::signals2::trackable {
    void delete_myself( boist::signals2::connection connection)
    {
      BOOST_CHECK(connection.connected());
      delete this;
      BOOST_CHECK(connection.connected() == false);
    }
    boist::signals2::scoped_connection connection;
};

// test that slot assocated with signals2::trackable
// gets disconnected immediately upon deletion of the
// signals2::trackable, even when a signal invocation
// is in progress.
void test_immediate_disconnect_on_delete()
{
  boist::signals2::signal<void () > sig;
  self_deleting *obj = new self_deleting();
  obj->connection = sig.connect_extended(std::bind(&self_deleting::delete_myself, obj, _1));
  sig();
}

BOOST_AUTO_TEST_CASE(test_main)
{
  typedef boist::signals2::signal<int (int), max_or_default<int> > sig_type;
  sig_type s1;

  // Test auto-disconnection
  BOOST_CHECK(s1(5) == 0);
  {
    short_lived shorty;
    boist::signals2::scoped_connection connection;
    connection = s1.connect(std::bind<int>(swallow(), &shorty, _1));
    BOOST_CHECK(s1(5) == 5);
  }
  int s_r = s1( 5 );
  BOOST_CHECK(s1(5) == 0);
  // Test auto-disconnection of trackable inside reference_wrapper
  {
    short_lived shorty;
    boist::signals2::scoped_connection connection;
    connection = s1.connect(std::bind<int>(swallow(), std::ref(shorty), _1));
    BOOST_CHECK(s1(5) == 5);
  }
  BOOST_CHECK(s1(5) == 0);

  // Test multiple arg slot constructor
  {
    short_lived shorty;
    boist::signals2::scoped_connection connection;
    connection = s1.connect(sig_type::slot_type(swallow(), &shorty, _1));
    BOOST_CHECK(s1(5) == 5);
  }
  BOOST_CHECK(s1(5) == 0);

  // Test auto-disconnection of slot before signal connection
  {
    short_lived* shorty = new short_lived();

    sig_type::slot_type slot(std::bind<int>(swallow(), shorty, _1));
    delete shorty;

    BOOST_CHECK(s1(5) == 0);
  }

  test_immediate_disconnect_on_delete();
}

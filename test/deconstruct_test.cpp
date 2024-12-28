// Tests for boost::signals2::deconstruct_ptr and friends

// Copyright Frank Mori Hess 2007-2008.
// Distributed under the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/signals2 for library home page.

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost_signals2/deconstruct.hpp>
#include <boost_signals2/deconstruct_ptr.hpp>
#define BOOST_TEST_MODULE deconstruct_test
#include <boost/test/included/unit_test.hpp>

#include <memory>

class X: public boist::signals2::postconstructible {
public:
  X(): _postconstructed(false)
  {}
  ~X()
  {
    BOOST_CHECK(_postconstructed);
  }
protected:
  virtual void postconstruct()
  {
    BOOST_CHECK(!_postconstructed);
    _postconstructed = true;
  }
  bool _postconstructed;
};

class Y: public boist::signals2::predestructible {
public:
  Y(): _predestructed(false)
  {}
  ~Y()
  {
    BOOST_CHECK(_predestructed);
  }
protected:
  virtual void predestruct()
  {
    _predestructed = true;
  }
  bool _predestructed;
};

class Z: public X, public Y
{};

class by_deconstruct_only: public boist::signals2::postconstructible {
public:
  ~by_deconstruct_only()
  {
    BOOST_CHECK(_postconstructed);
  }
  int value;
protected:
  virtual void postconstruct()
  {
    BOOST_CHECK(!_postconstructed);
    _postconstructed = true;
  }
  bool _postconstructed;
private:
  friend class boist::signals2::deconstruct_access;
  by_deconstruct_only(int value_in):
    value(value_in), _postconstructed(false)
  {}
};

namespace mytest
{
  class A
  {
  public:
    template<typename T> friend
      void adl_postconstruct(const std::shared_ptr<T> &sp, A *p)
    {
      BOOST_CHECK(!p->_postconstructed);
      p->_postconstructed = true;
    }
    template<typename T> friend
      void adl_postconstruct(const std::shared_ptr<T> &sp, A *p, int val)
    {
      p->value = val;
      BOOST_CHECK(!p->_postconstructed);
      p->_postconstructed = true;
    }
    friend void adl_predestruct(A *p)
    {
      p->_predestructed = true;
    }
    ~A()
    {
      BOOST_CHECK(_postconstructed);
      BOOST_CHECK(_predestructed);
    }
    int value;
  private:
    friend class boist::signals2::deconstruct_access;
    A(int value_in = 0):
      value(value_in),
      _postconstructed(false),
      _predestructed(false)
    {}
    bool _postconstructed;
    bool _predestructed;
  };
}

void deconstruct_ptr_test()
{
  {
    std::shared_ptr<X> x = boist::signals2::deconstruct_ptr(new X);
  }
  {
    std::shared_ptr<Y> x = boist::signals2::deconstruct_ptr(new Y);
  }
  {
    std::shared_ptr<Z> z = boist::signals2::deconstruct_ptr(new Z);
  }
}

class deconstructed_esft : public std::enable_shared_from_this<deconstructed_esft>
{
public:
  deconstructed_esft() : x(0) {}

private:
  friend void adl_postconstruct(std::shared_ptr<void>, deconstructed_esft *) {}
  int x;
};

void deconstruct_test()
{
  {
      std::shared_ptr<X> x = boist::signals2::deconstruct<X>();
  }
  {
      std::shared_ptr<Y> x = boist::signals2::deconstruct<Y>();
  }
  {
      std::shared_ptr<Z> z = boist::signals2::deconstruct<Z>();
  }
  {
      std::shared_ptr<by_deconstruct_only> a = boist::signals2::deconstruct<by_deconstruct_only>(1);
    BOOST_CHECK(a->value == 1);
  }
  {
      std::shared_ptr<mytest::A> a = boist::signals2::deconstruct<mytest::A>(1);
    BOOST_CHECK(a->value == 1);
  }
  {// deconstruct const type
      std::shared_ptr<const mytest::A> a = boist::signals2::deconstruct<const mytest::A>(3);
    BOOST_CHECK(a->value == 3);
  }
  {// passing arguments to postconstructor
      std::shared_ptr<mytest::A> a = boist::signals2::deconstruct<mytest::A>().postconstruct(2);
    BOOST_CHECK(a->value == 2);
  }
  {// enable_shared_from_this with deconstruct
      std::shared_ptr<deconstructed_esft> a = boist::signals2::deconstruct<deconstructed_esft>();
      //BOOST_CHECK(!(a->shared_from_this() < a || a < a->shared_from_this()));
  }
}

BOOST_AUTO_TEST_CASE(test_main)
{
  deconstruct_ptr_test();
  deconstruct_test();
}

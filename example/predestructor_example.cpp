// Minimal example of defining a predestructor for a class which
// uses boist::signals2::deconstruct as its factory function.
//
// Copyright Frank Mori Hess 2009.

// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// For more information, see http://www.boost.org

#include <boost_signals2/deconstruct.hpp>
#include <iostream>
#include <memory>

namespace bs2 = boist::signals2;

namespace mynamespace
{
  class X
  {
  public:
    ~X()
    {
      std::cout << "cruel world!" << std::endl;
    }
    /* This adl_predestruct friend function will be found by
    via argument-dependent lookup when using boist::signals2::deconstruct. */
    friend void adl_predestruct(X *)
    {
      std::cout << "Goodbye, ";
    }
    /* boist::signals2::deconstruct always requires an adl_postconstruct function
    which can be found via argument-dependent, so we define one which does nothing. */
    template<typename T> friend
      void adl_postconstruct(const std::shared_ptr<T> &, X *)
    {}
  private:
    friend class bs2::deconstruct_access;  // give boist::signals2::deconstruct access to private constructor
    // private constructor forces use of boist::signals2::deconstruct to create objects.
    X()
    {}
  };
}

int main()
{
  {
     std::shared_ptr<mynamespace::X> x = bs2::deconstruct<mynamespace::X>();
  }
  return 0;
}

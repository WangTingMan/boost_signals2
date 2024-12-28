// Example program showing signals with custom combiners.
//
// Copyright Douglas Gregor 2001-2004.
// Copyright Frank Mori Hess 2009.
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// For more information, see http://www.boost.org

#include <iostream>
#include <boost_signals2/signal.hpp>
#include <vector>

float product(float x, float y)
{
    std::cout << "product invoking...\n";
    return x * y;
}

float quotient(float x, float y)
{
    std::cout << "quotient invoking...\n";
    return x / y;
}

float sum(float x, float y)
{
    std::cout << "sum invoking...\n";
    return x + y;
}

float difference(float x, float y)
{
    std::cout << "difference invoking...\n";
    return x - y;
}

template<typename T>
struct DistributeRequest
{
    typedef T result_type;

    template<typename InputIterator>
    result_type operator()( InputIterator first, InputIterator last ) const
    {
        result_type fulfilled;
        while( first != last )
        {
            fulfilled = *first;
            if( fulfilled > 10 )
            {
                std::cout << "since we got result which bigger than 10, then ignore any other slots!\n";
                return fulfilled;
            }
            ++first;
        }
        return 0;
    }
};

void distribute_request_example()
{
  // signal which uses our custom "maximum" combiner
  boist::signals2::signal<float (float x, float y), DistributeRequest<float> > sig;

  //[ custom_combiners_maximum_usage_code_snippet
  sig.connect(&difference );
  sig.connect(&product);
  sig.connect(&quotient);
  sig.connect(&sum);

  sig.num_slots();
  // Outputs the maximum value returned by the connected slots, in this case
  // 15 from the product function.
  std::cout << "emit signal.\n" << sig(5, 3) << std::endl;
//]
}

int main()
{
  distribute_request_example();
}

#include <iostream>
#include <type_traits>
#include <boost_signals2/signals2.hpp>
#include <boost_signals2/function_traits.hpp>
#include <optional>
#include <functional>

template<typename T>
struct max_or_default {
  typedef T result_type;
  template<typename InputIterator>
  T operator()(InputIterator first, InputIterator last) const
  {
    std::optional<T> max;
    for (; first != last; ++first)
    {
      try
      {
        auto v = *first;
        if( max.has_value() )
        {
            max = ( v > max.value() ) ? v : max.value();
        }
        else
        {
            max = v;
        }
      }
      catch(const std::bad_weak_ptr &)
      {}
    }

    if( max.has_value() )
    {
        return max.value();
    }
    else
    {
        return T();
    }
  }
};

struct make_int
{
    make_int( int n, int cn ) : N( n ), CN( cn ) {}

    int operator()() { return N; }
    int operator()() const { return CN; }

    int N;
    int CN;
};

int main()
{
    make_int i42( 42, 41 );
    make_int i2( 2, 1 );
    make_int i72( 72, 71 );
    make_int i63( 63, 63 );
    make_int i62( 62, 61 );

    int x = 0;

    {
        boist::signals2::signal<int(), max_or_default<int> > s0;

        std::cout << "sizeof(signal) = " << sizeof( s0 ) << std::endl;
        boist::signals2::connection c2 = s0.connect( i2 );
        boist::signals2::connection c72 = s0.connect( 72, i72 );
        boist::signals2::connection c62 = s0.connect( 60, i62 );
        boist::signals2::connection c42 = s0.connect( i42 );

        ( s0() == 72 );

        s0.disconnect( 72 );
        ( s0() == 62 );

        c72.disconnect(); // Double-disconnect should be safe
        ( s0() == 62 );

        s0.disconnect( 72 ); // Triple-disconect should be safe
        ( s0() == 62 );

        // Also connect 63 in the same group as 62
        s0.connect( 60, i63 );
        ( s0() == 63 );

        // Disconnect all of the 60's
        s0.disconnect( 60 );
        ( s0() == 42 );

        c42.disconnect();
        ( s0() == 2 );

        c2.disconnect();
        x = s0();
    }

        std::cout << "Hello World!\n";
}


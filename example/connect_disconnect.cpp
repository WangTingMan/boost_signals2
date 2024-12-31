#include <algorithm>
#include <iostream>
#include <type_traits>
#include <boost_signals2/signals2.hpp>
#include <boost_signals2/function_traits.hpp>
#include <optional>
#include <functional>
#include <vector>

struct max_or_default
{
    typedef int result_type;
    template<typename InputIterator>
    result_type operator()( InputIterator first, InputIterator last ) const
    {
        std::vector<int> all_return;
        for( ; first != last; ++first )
        {
            try
            {
                auto v = *first;
                all_return.push_back( v );
            }
            catch( const std::bad_weak_ptr& )
            {
            }
        }

        auto it = std::max_element( all_return.begin(), all_return.end() );
        if( std::max_element( all_return.begin(), all_return.end() ) == all_return.end() )
        {
            return 0;
        }
        return *it;
    }
};

int negative( int x )
{
    return -x;
}

int multiply( int x, int y )
{
    return x * y;
}

int plus_one( int x )
{
    return x + 1;
}

void output( int value, int expected_values, std::size_t line_number )
{
    if( value == expected_values )
    {
        std::cout << "No error.\n";
        return;
    }

    std::cerr << "Not expect! line:" << line_number << "\n";
}

#define EXPECT_RESULT(A,B) output((A),B, __LINE__)

int main()
{
    std::vector<int> returned_values;
    using signal_type = boist::signals2::signal<int( int value ), max_or_default >;
    signal_type s1;
    s1.connect( &negative );
    EXPECT_RESULT( s1( 89 ), -89 );
    s1.disconnect( negative );
    EXPECT_RESULT( s1( 89 ), 0 );
    EXPECT_RESULT( s1.num_slots(), 0 );

    s1.connect( negative );
    EXPECT_RESULT( s1( 89 ), -89 );
    s1.disconnect( &negative );
    EXPECT_RESULT( s1( 89 ), 0 );
    EXPECT_RESULT( s1.num_slots(), 0 );

    signal_type s2;
    s2.connect( std::bind( multiply, 2, std::placeholders::_1 ) );
    s2.connect( plus_one );
    s1.connect( s2 );
    s1.connect( negative );
    EXPECT_RESULT( s1( 21 ), 42 );
    EXPECT_RESULT( s1.num_slots(), 2 );

    s1.disconnect( s2 );
    EXPECT_RESULT( s1( 25 ), -25 );
    EXPECT_RESULT( s1.num_slots(), 1 );

    s1.connect( s2 );
    s1.disconnect( &s2 );
    EXPECT_RESULT( s1( 21 ), -21 );
    EXPECT_RESULT( s1.num_slots(), 1 );

    {
        signal_type s3;
        s1.connect( s3 );
        s3.connect( plus_one );
        EXPECT_RESULT( s1( 21 ), 22 );
        EXPECT_RESULT( s1.num_slots(), 2 );
    }
    EXPECT_RESULT( s1( 2 ), -2 );
    EXPECT_RESULT( s1.num_slots(), 1 );

    return 0;
}


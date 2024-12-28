/*
  A meta function which supports using named template type parameters
  via Boost.Parameter to specify the template type parameters for
  the boist::signals2::signal class.

  Author: Frank Mori Hess <fmhess@users.sourceforge.net>
  Begin: 2009-01-22
*/
// Copyright Frank Mori Hess 2009
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org
#pragma once

#ifndef BOIST_SIGNALS2_SIGNAL_TYPE_HPP
#define BOIST_SIGNALS2_SIGNAL_TYPE_HPP

#include <boost_signals2/signal.hpp>

#include <functional>
#include <optional>
#include <type_traits>

namespace boost
{
  namespace signals2
  {
    namespace keywords
    {
#if 0
        namespace tag {
            struct signature_type;
        }

        template <typename T>
        struct signature_type : ::boost::parameter::template_keyword<tag::signature_type, T> {};

        namespace tag {
            struct combiner_type;
        } template <typename T> struct combiner_type : ::boost::parameter::template_keyword<tag::combiner_type, T> {};

        namespace tag {
            struct group_type;
        } template <typename T> struct group_type : ::boost::parameter::template_keyword<tag::group_type, T> {};

        namespace tag {
            struct group_compare_type;
        } template <typename T> struct group_compare_type : ::boost::parameter::template_keyword<tag::group_compare_type, T> {};

        namespace tag {
            struct slot_function_type;
        } template <typename T> struct slot_function_type : ::boost::parameter::template_keyword<tag::slot_function_type, T> {};

        namespace tag {
            struct extended_slot_function_type;
        } template <typename T> struct extended_slot_function_type : ::boost::parameter::template_keyword<tag::extended_slot_function_type, T> {};

        namespace tag {
            struct mutex_type;
        } template <typename T> struct mutex_type : ::boost::parameter::template_keyword<tag::mutex_type, T> {};
#endif
    } // namespace keywords

#if 0
    template <
        typename A0,
        typename A1 = parameter::void_,
        typename A2 = parameter::void_,
        typename A3 = parameter::void_,
        typename A4 = parameter::void_,
        typename A5 = parameter::void_,
        typename A6 = parameter::void_
      >
    class signal_type
    {
      typedef parameter::parameters<
          parameter::required<keywords::tag::signature_type, is_function<boost::mpl::_> >,
          parameter::optional<keywords::tag::combiner_type>,
          parameter::optional<keywords::tag::group_type>,
          parameter::optional<keywords::tag::group_compare_type>,
          parameter::optional<keywords::tag::slot_function_type>,
          parameter::optional<keywords::tag::extended_slot_function_type>,
          parameter::optional<keywords::tag::mutex_type>
        > parameter_spec;

    public:
      // ArgumentPack
      typedef typename parameter_spec::bind<A0, A1, A2, A3, A4, A5, A6>::type
        args;

      typedef typename parameter::value_type<args, keywords::tag::signature_type>::type
        signature_type;

      typedef typename parameter::value_type
        <
          args,
          keywords::tag::combiner_type,
          optional_last_value
            <
              typename boost::function_traits<signature_type>::result_type
            >
        >::type combiner_type;

      typedef typename
        parameter::value_type<args, keywords::tag::group_type, int>::type group_type;

      typedef typename
        parameter::value_type<args, keywords::tag::group_compare_type, std::less<group_type> >::type
        group_compare_type;

      typedef typename
        parameter::value_type<args, keywords::tag::slot_function_type, function<signature_type> >::type
        slot_function_type;

      typedef typename
        parameter::value_type
          <
            args,
            keywords::tag::extended_slot_function_type,
            typename detail::extended_signature<function_traits<signature_type>::arity, signature_type>::function_type
          >::type
          extended_slot_function_type;

      typedef typename
        parameter::value_type<args, keywords::tag::mutex_type, mutex>::type mutex_type;

      typedef signal
        <
          signature_type,
          combiner_type,
          group_type,
          group_compare_type,
          slot_function_type,
          extended_slot_function_type,
          mutex_type
        > type;
    };

#endif
  } // namespace signals2
} // namespace boost

#endif // BOOST_SIGNALS2_SIGNAL_TYPE_HPP

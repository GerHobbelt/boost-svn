//  (C) Copyright Gennadiy Rozental 2001-2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 74640 $
//
//  Description : defines test_case_counter
// ***************************************************************************

#ifndef BOOST_TEST_TREE_TEST_CASE_COUNTER_HPP_100211GER
#define BOOST_TEST_TREE_TEST_CASE_COUNTER_HPP_100211GER

// Boost.Test
#include <boost/test/detail/config.hpp>
#include <boost/test/utils/class_properties.hpp>

#include <boost/test/tree/test_unit.hpp>
#include <boost/test/tree/visitor.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {

// ************************************************************************** //
// **************                test_case_counter             ************** //
// ************************************************************************** //

class test_case_counter : public test_tree_visitor {
public:
    // Constructor
    test_case_counter() : p_count( 0 ) {}

    BOOST_READONLY_PROPERTY( counter_t, (test_case_counter)) p_count;
private:
    // test tree visitor interface
    virtual void    visit( test_case const& tc )                { if( tc.p_enabled ) ++p_count.value; }
    virtual bool    test_suite_start( test_suite const& ts )    { return ts.p_enabled; }
};

} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_TREE_TEST_CASE_COUNTER_HPP_100211GER


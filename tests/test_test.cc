
#define BOOST_TEST_MODULE trivial_test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( just_a_trivial_test_case )
{
    BOOST_CHECK_EQUAL( true, true );
}


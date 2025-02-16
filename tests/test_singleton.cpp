#define BOOST_TEST_MODULE "Singleton Utility Tests"

#include "test_singleton.h"

BOOST_AUTO_TEST_SUITE(singleton_tests)

    BOOST_FIXTURE_TEST_CASE(initialized, fixture)
    {
        BOOST_TEST_REQUIRE(the_singleton.references() == 1);

        test_singleton another_singleton;
        {
            BOOST_TEST_REQUIRE(the_singleton.references() == 2);
            BOOST_TEST_REQUIRE(another_singleton.references() == 2);
        }

        BOOST_TEST_REQUIRE(initialized_counter == 1);
    }

    BOOST_FIXTURE_TEST_CASE(finalized, fixture)
    {
        test_singleton* singleton1 = new test_singleton();
        {
            delete singleton1;
        }

        BOOST_TEST_REQUIRE(finalized_counter == 1);
    }

    BOOST_FIXTURE_TEST_CASE(rollover, fixture)
    {
        std::size_t limit = std::numeric_limits<std::size_t>::max();
        
        the_singleton.force_to(limit);
        BOOST_TEST_REQUIRE(the_singleton.references() == limit);

        test_singleton another_singleton;
        BOOST_TEST_REQUIRE(the_singleton.references() == 0);
        BOOST_TEST_REQUIRE(finalized_counter == 1);
    }

    BOOST_FIXTURE_TEST_CASE(forcing, fixture)
    {
        the_singleton.force_to(0);
        BOOST_TEST_REQUIRE(the_singleton.references() == 0);
        BOOST_TEST_REQUIRE(finalized_counter == 1);
        BOOST_TEST_REQUIRE(initialized_counter == 1);

        the_singleton.force_to(1);
        BOOST_TEST_REQUIRE(the_singleton.references() == 1);
        BOOST_TEST_REQUIRE(initialized_counter == 2);
    }

BOOST_AUTO_TEST_SUITE_END()
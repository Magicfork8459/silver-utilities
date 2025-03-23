#define BOOST_TEST_MODULE "Registry Utility Tests"

#include "test_registry.h"

BOOST_AUTO_TEST_SUITE(registry_tests)

BOOST_FIXTURE_TEST_CASE(name_collision, fixture)
{
    std::string name("test");
    auto first_result = the_registry.generate(name);
    auto second_result = the_registry.generate(name);

    BOOST_TEST_REQUIRE(first_result.has_value());
    BOOST_TEST_REQUIRE(not second_result.has_value());
    BOOST_TEST_REQUIRE(the_registry.size() == 1);
}

BOOST_FIXTURE_TEST_CASE(random_generation, fixture)
{
    unsigned short limit = std::numeric_limits<unsigned short>::max();

    for(unsigned short i = 0; i < limit; ++i)
    {
        auto result = the_registry.generate();

        BOOST_TEST_REQUIRE(result.has_value());
    }
}

BOOST_AUTO_TEST_SUITE_END()
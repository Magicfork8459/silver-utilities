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

BOOST_FIXTURE_TEST_CASE(test_revoke_by_name, fixture)
{
    std::string name("test");
    auto result = the_registry.generate(name);

    BOOST_TEST_REQUIRE(result.has_value());
    BOOST_TEST_REQUIRE(the_registry.size() == 1);

    auto revoke_result = the_registry.revoke(name);

    BOOST_TEST_REQUIRE(revoke_result);
    BOOST_TEST_REQUIRE(the_registry.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(test_revoke_by_uuid, fixture)
{
    boost::uuids::uuid uuid = the_registry.generate().value();

    BOOST_TEST_REQUIRE(the_registry.size() == 1);
    
    auto revoke_result = the_registry.revoke(uuid);

    BOOST_TEST_REQUIRE(revoke_result);
    BOOST_TEST_REQUIRE(the_registry.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(negative_revoke, fixture)
{
    boost::uuids::uuid uuid = the_registry.generate().value();

    auto revoke_result = the_registry.revoke(uuid);
    BOOST_TEST_REQUIRE(revoke_result);
    revoke_result = the_registry.revoke(uuid);
    BOOST_TEST_REQUIRE(not revoke_result);
}

BOOST_FIXTURE_TEST_CASE(revoke_and_reregister, fixture)
{
    std::string name("test");
    auto result = the_registry.generate(name);

    BOOST_TEST_REQUIRE(result.has_value());
    BOOST_TEST_REQUIRE(the_registry.size() == 1);

    auto revoke_result = the_registry.revoke(name);

    BOOST_TEST_REQUIRE(revoke_result);
    BOOST_TEST_REQUIRE(the_registry.size() == 0);

    result = the_registry.generate(name);

    BOOST_TEST_REQUIRE(result.has_value());
    BOOST_TEST_REQUIRE(the_registry.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
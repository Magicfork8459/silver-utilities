#define BOOST_TEST_MODULE "Program Utility Tests"

#include "test_program.h"

fixture::fixture()
{
    program.begin();
} 

fixture::~fixture()
{
    program.stop();
}

a_program::a_program()
    : beginning_flag(0)
    , begun_flag(0)
    , stopping_flag(0)
    , stopped_flag(0)
{

}

void a_program::_beginning()
{
    ++beginning_flag;
}

void a_program::_begun()
{
    ++begun_flag;
}

void a_program::_stopping()
{
    ++stopping_flag;
}

void a_program::_stopped()
{
    ++stopped_flag;
}

BOOST_AUTO_TEST_SUITE(program_tests)

    BOOST_AUTO_TEST_CASE(test_state_callbacks_sync, * boost::unit_test_framework::timeout(2))
    {
        a_program program;
        boost::asio::steady_timer timer = program.create_with_context<boost::asio::steady_timer>();
        {
            timer.expires_after(150ms);
            timer.async_wait([&](const boost::system::error_code& error_code)
            {
                program.stop();
            });
        }

        BOOST_TEST_REQUIRE(program.beginning_flag == 0);
        BOOST_TEST_REQUIRE(program.begun_flag == 0);
        BOOST_TEST_REQUIRE(program.stopping_flag == 0);
        BOOST_TEST_REQUIRE(program.stopped_flag == 0);
        program.begin();
        BOOST_TEST_REQUIRE(program.beginning_flag == 1);
        BOOST_TEST_REQUIRE(program.begun_flag == 1);
        BOOST_TEST_REQUIRE(program.stopping_flag == 1);
        BOOST_TEST_REQUIRE(program.stopped_flag == 1);
    }

    BOOST_AUTO_TEST_CASE(test_state_callbacks_async, * boost::unit_test_framework::timeout(5))
    {
        a_program program;
        std::chrono::milliseconds wait_time(150ms);
        bool context_did_something = false;
        boost::asio::steady_timer timer = program.create_with_context<boost::asio::steady_timer>();
        {
            timer.expires_after(wait_time);
            timer.async_wait([&](const boost::system::error_code& error_code)
            {
                context_did_something = true;
            });
        }

        BOOST_TEST_REQUIRE(program.beginning_flag == 0);
        BOOST_TEST_REQUIRE(program.begun_flag == 0);
        BOOST_TEST_REQUIRE(program.stopping_flag == 0);
        BOOST_TEST_REQUIRE(program.stopped_flag == 0);

        program.async_begin();

        BOOST_TEST_REQUIRE(program.beginning_flag == 1);
        BOOST_TEST_REQUIRE(program.begun_flag == 1);
        BOOST_TEST_REQUIRE(program.stopping_flag == 0);
        BOOST_TEST_REQUIRE(program.stopped_flag == 0);

        std::this_thread::sleep_for(wait_time * 2);
        program.stop();

        BOOST_TEST_REQUIRE(program.stopping_flag == 1);
        BOOST_TEST_REQUIRE(program.stopped_flag == 1);
        
        BOOST_TEST_REQUIRE(context_did_something);
    }

    BOOST_AUTO_TEST_CASE(test_state_callbacks_sync_for)
    {
        a_program program;
        
        BOOST_TEST_REQUIRE(program.beginning_flag == 0);
        BOOST_TEST_REQUIRE(program.begun_flag == 0);
        BOOST_TEST_REQUIRE(program.stopping_flag == 0);
        BOOST_TEST_REQUIRE(program.stopped_flag == 0);

        auto before = std::chrono::steady_clock::now();
        program.begin_for(1s);
        auto diff = std::chrono::steady_clock::now() - before;

        BOOST_TEST_REQUIRE(diff >= 1s);

        BOOST_TEST_REQUIRE(program.beginning_flag == 1);
        BOOST_TEST_REQUIRE(program.begun_flag == 1);
        BOOST_TEST_REQUIRE(program.stopping_flag == 1);
        BOOST_TEST_REQUIRE(program.stopped_flag == 1);
    }

    BOOST_AUTO_TEST_CASE(test_state_callbacks_async_for, * boost::unit_test_framework::timeout(5))
    {
        a_program program;

        BOOST_TEST_REQUIRE(program.beginning_flag == 0);
        BOOST_TEST_REQUIRE(program.begun_flag == 0);
        BOOST_TEST_REQUIRE(program.stopping_flag == 0);
        BOOST_TEST_REQUIRE(program.stopped_flag == 0);

        auto before = std::chrono::steady_clock::now();
        program.async_begin_for(1s);
        
        do 
        {
            std::this_thread::yield();
        }while(program.running());

        auto diff = std::chrono::steady_clock::now() - before;

        BOOST_TEST_REQUIRE(diff >= 1s);

        BOOST_TEST_REQUIRE(program.beginning_flag == 1);
        BOOST_TEST_REQUIRE(program.begun_flag == 1);
        BOOST_TEST_REQUIRE(program.stopping_flag == 1);
        BOOST_TEST_REQUIRE(program.stopped_flag == 1);
    }

    //TODO test for whether the signals actually work or not
    //TODO need to launch a subprocess and send that subprocess the signals in order to properly test this
    //TODO furthermore, can't seem to do this on Windows specifically... if we add Linux support then we can add this test in since it actually uses the signals correctly
    

BOOST_AUTO_TEST_SUITE_END()
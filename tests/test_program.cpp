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

    // BOOST_AUTO_TEST_CASE(test_begin_stop, * boost::unit_test_framework::timeout(2))
    // {       
    //     a_program program; 
    //     boost::asio::steady_timer timer = program.create_with_context<boost::asio::steady_timer>();
    //     bool timer_worked = false;

    //     timer.expires_after(1s);        
    //     timer.async_wait([&](const boost::system::error_code& error_code)
    //     {
    //         timer_worked = true;
    //         program.stop();
    //     });
    //     program.begin();

    //     BOOST_TEST_REQUIRE(timer_worked);
    // }

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

    BOOST_AUTO_TEST_CASE(test_state_callbacks_async, * boost::unit_test_framework::timeout(2))
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
        
    }

    //TODO this isn't possible to do like this, need to launch a subprocess and send that subprocess the signals in order to properly test this
    // BOOST_AUTO_TEST_CASE(test_sigterm, * boost::unit_test_framework::timeout(2))
    // {
    //     a_program program;
    //     program.async_begin();
    //     std::raise(SIGTERM);

    //     while(program.running())
    //     {
    //         std::this_thread::yield();
    //     }
    //     std::this_thread::sleep_for(250ms);
    //     //BOOST_TEST_REQUIRE(program.stopping_flag == 1);
    //     //BOOST_TEST_REQUIRE(program.stopped_flag == 1);
    // }
    
    // BOOST_FIXTURE_TEST_CASE(test_async_begin, fixture)
    // {
    //     BOOST_TEST_FAIL("unimplemented");
    // }

BOOST_AUTO_TEST_SUITE_END()
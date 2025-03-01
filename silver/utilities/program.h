#pragma once

#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

namespace silver
{
    namespace utilities
    {
        class program
        {
            public:
                program();
                ~program();

                boost::asio::io_context& context();

                template<typename T, typename... A>
                T create_with_context(A&... args)                
                {
                    return std::move(T(__context, args...));
                }

                //NOTE this blocking function will allow the program to execute until it has run out of anything to do or receives SIG_TERM/SIG_INT
                void begin();
                //NOTE this blocking function will only allow the program to execute for the specified amount of time or until it has run out of anything to do or receives SIG_TERM/SIG_INT
                void begin_for(const std::chrono::duration<double>& time);
                //NOTE this non-blocking function will allow the program to execute until it has run out of anything to do or receives SIG_TERM/SIG_INT
                void async_begin();
                //NOTE this blocking function will end program execution
                void stop();

                bool running();
                
            protected:
                void _stop();
                virtual void _beginning() = 0;
                virtual void _begun() = 0;
                virtual void _stopping() = 0;
                virtual void _stopped() = 0;
            private:
                
                void __do_wait();
                
                boost::asio::io_context __context;
                boost::asio::signal_set __signals;
                std::jthread __async_thread;
                bool __user_triggered_stop;
        };
    }
}
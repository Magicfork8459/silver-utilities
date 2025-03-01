#include <silver/utilities/program.h>

namespace silver 
{
    namespace utilities
    {
        program::program()
            : __signals(__context, SIGINT, SIGTERM)
            , __user_triggered_stop(false)
        {
            
        }
        
        program::~program()
        {
            stop();
        }

        boost::asio::io_context& program::context()
        {
            return __context;
        }

        void program::begin()
        {
            __user_triggered_stop = false;

            _beginning();
            __do_wait();
            _begun();
            __context.run();

            if(not __user_triggered_stop)
            {
                _stopping();
                _stopped();
            }
        }

        void program::begin_for(const std::chrono::duration<double>& time)
        {
            __user_triggered_stop = false;

            _beginning();
            __do_wait();
            _begun();
            __context.run_for(std::chrono::duration_cast<std::chrono::nanoseconds>(time));

            if(not __user_triggered_stop)
            {
                _stopping();
                _stopped();
            }
        }

        void program::async_begin()
        {
            __user_triggered_stop = false;

            _beginning();
            __do_wait();
            
            __async_thread = std::jthread([&](std::stop_token token)
            {
                do
                {                    
                    __context.poll_one();
                    std::this_thread::yield();
                } while (not token.stop_requested() and not __context.stopped());
                
            });

            _begun();
        }

        void program::stop()
        {
            if(running())
            {
                __user_triggered_stop = true;
                _stop();            
            }            
        }

        void program::_stop()
        {
            if(running())
            {
                _stopping();

                __context.stop();

                if(__async_thread.joinable())
                {
                    __async_thread.request_stop();
                    __async_thread.join();
                }

                _stopped();
            }
        }

        bool program::running()
        {
            return not __context.stopped();
        }

        void program::__do_wait()
        {
            __signals.async_wait([&](const boost::system::error_code error_code, const int& signal_number)
            {
                switch(signal_number)
                {
                    case SIGINT:
                    case SIGTERM:
                        _stop();
                    default:
                        return;
                }
            });
        }
    }
}
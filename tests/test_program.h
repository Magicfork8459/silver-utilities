#pragma once
#include <csignal>

#include <boost/asio/steady_timer.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/system/error_code.hpp>

#include <silver/utilities/program.h>

using namespace silver::utilities;
using namespace std::chrono_literals;

class a_program: public program
{
    public:
        a_program();
        unsigned short beginning_flag;
        unsigned short begun_flag;
        unsigned short stopping_flag;
        unsigned short stopped_flag;
    protected:
        void _beginning() override;
        void _begun() override;
        void _stopping() override;
        void _stopped() override;

        
    private:

};

struct fixture
{
    fixture();    
    ~fixture();

    a_program program;
    
};
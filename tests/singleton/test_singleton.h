#pragma once
#include <boost/test/unit_test.hpp>
#include <silver/utilities/singleton.h>

using namespace silver::utilities;

inline static std::size_t finalized_counter { 0 }; 
inline static std::size_t initialized_counter { 0 }; 

class test_singleton: public singleton 
{
    public:
        test_singleton()
        {
            _referenced();
        }

        ~test_singleton()
        {
            _dereferenced();
        }

        void force_to(const std::size_t& value)
        {
            _force_references_to(value);
        }      

    protected:
        void _initialize() override 
        {
            ++initialized_counter;
        }

        void _finalize() override 
        {
            ++finalized_counter;
        }
};

struct fixture
{
    fixture()
    {
        finalized_counter = 0;
        initialized_counter = 1;   
    }

    ~fixture()
    {             
        the_singleton.force_to(0);
        finalized_counter = 0;
        initialized_counter = 0;   
    }    

    test_singleton the_singleton;    
};
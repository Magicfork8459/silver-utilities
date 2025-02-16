#pragma once
#include <numeric>

namespace silver
{
    namespace utilities
    {
        class singleton
        {
            public:
                std::size_t references();

            protected:
                //NOTE this function must be called in your constructor
                virtual void referenced() final;
                //NOTE this function must be called in your destructor
                virtual void dereferenced() final;          
                //NOTE usage of this function is not recommended, it is for testing purposes                      
                virtual void _force_references_to(const std::size_t& value) final;
                //NOTE this function will be called by the base class whenever references go from 0 -> 1
                virtual void initialize() = 0;
                //NOTE this function will be called by the base class whenever references go to 0
                virtual void finalize() = 0;

            private:
                static std::size_t __references;
        };        
    }
}
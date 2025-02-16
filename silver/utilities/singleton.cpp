#include <silver/utilities/singleton.h>

namespace silver
{
    namespace utilities
    {
        std::size_t singleton::__references = 0;

        std::size_t singleton::references()
        {
            return __references;
        }

        void singleton::referenced()
        {
            std::size_t last_reference = __references;
            std::size_t next_reference = ++__references;

            if(last_reference != 0 and next_reference == 0)
            {
                finalize();
            }
            else if(last_reference == 0)
            {
                initialize();
            }
        }

        void singleton::dereferenced()
        {
            if(--__references <= 0)
            {
                finalize();                
            }
        }

        void singleton::_force_references_to(const std::size_t& value)
        {
            std::size_t last_reference = __references;
            
            __references = value;

            if(last_reference == 0 and value > 0)
            {
                initialize();
            }
            else if(value == 0)
            {
                finalize();
            }
        }
    }
}
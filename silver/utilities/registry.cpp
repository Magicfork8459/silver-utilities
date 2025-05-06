#include <silver/utilities/registry.h>

namespace silver
{
    namespace utilities
    {
        const std::size_t registry::DEFAULT_MAX_ATTEMPTS = 5;
        boost::uuids::random_generator registry::__random_generator;
        boost::uuids::name_generator registry::__name_generator(boost::uuids::ns::oid());
        std::set<boost::uuids::uuid> registry::__uuids;
        std::size_t registry::__max_attempts = registry::DEFAULT_MAX_ATTEMPTS;

        registry::registry()
        {
            _referenced();
        }

        registry::~registry()
        {
            _dereferenced();
        }

        std::optional<boost::uuids::uuid> registry::generate()
        {
            std::optional<boost::uuids::uuid> ret_val = {};
            std::size_t current_attempts = 0;

            do
            {
                boost::uuids::uuid generated = __random_generator();

                if(not contains(generated))
                {
                    ret_val = generated;
                    __uuids.insert(generated);
                }
            } while (++current_attempts < __max_attempts and not ret_val.has_value());

            return ret_val;
        }

        std::optional<boost::uuids::uuid> registry::generate(const std::string& name)
        {
            std::optional<boost::uuids::uuid> ret_val = {};            
            boost::uuids::uuid generated = __name_generator(name);

            if(not contains(generated))
            {
                ret_val = generated;
                __uuids.insert(generated);
            }
            
            return ret_val;
        }

        bool registry::revoke(const boost::uuids::uuid& by_uuid)
        {
            std::size_t before = __uuids.size();

            if(__uuids.contains(by_uuid))
            {
                __uuids.erase(by_uuid);
            }

            return before > __uuids.size();
        }
        
        bool registry::revoke(const std::string& by_name)
        {
            std::size_t before = __uuids.size();

            if(__uuids.contains(__name_generator(by_name)))
            {
                __uuids.erase(__name_generator(by_name));                
            }

            return before > __uuids.size();
        }

        std::size_t registry::size() const
        {
            return __uuids.size();
        }

        std::optional<boost::uuids::uuid> registry::get(const std::string& by_name) const
        {
            std::optional<boost::uuids::uuid> ret_val = {};
            boost::uuids::uuid temp = __name_generator(by_name);

            if(contains(temp))
            {
                ret_val = std::move(temp);
            }

            return ret_val;
        }

        bool registry::contains(const boost::uuids::uuid& uuid) const
        {
            return __uuids.contains(uuid);
        }

        void registry::_initialize()
        {
            __random_generator = boost::uuids::random_generator();
            __name_generator = boost::uuids::name_generator(boost::uuids::ns::oid());
        }

        void registry::_finalize()
        {
            __uuids.clear();
        }
    }
}
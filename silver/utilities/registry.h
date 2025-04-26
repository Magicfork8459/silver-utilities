#pragma once

#include <set>
#include <optional>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <silver/utilities/singleton.h>

namespace silver
{
    namespace utilities
    {
        class registry: public singleton
        {
            public:
                static const std::size_t DEFAULT_MAX_ATTEMPTS;

                registry();
                ~registry();

                std::optional<boost::uuids::uuid> generate();
                std::optional<boost::uuids::uuid> generate(const std::string& name);
                bool revoke(const boost::uuids::uuid& by_uuid);
                bool revoke(const std::string& by_name);
                std::size_t size() const;
                std::optional<boost::uuids::uuid> get(const std::string& by_name) const;
                bool contains(const boost::uuids::uuid& uuid) const;

            protected:
                void _initialize() override;
                void _finalize() override;
            private:
                static boost::uuids::random_generator __random_generator;
                static boost::uuids::name_generator __name_generator;
                static std::set<boost::uuids::uuid> __uuids;
                static std::size_t __max_attempts;

        };
    }
}
#pragma once

#include <set>
#include <string>
#include <ostream>

namespace gpugraph
{

    class StyleHash
    {
    public:
        // this is any value of the hash or std::string() if the hash is empty
        std::string const& key() const;

        bool contains(std::string const&) const;

        void insert_type(std::string);
        void insert_id(std::string const&);
        void insert_class(std::string const&);

        void erase_type(std::string const&);
        void erase_id(std::string const&);
        void erase_class(std::string const&);

        bool is_subset_of(StyleHash const&) const;

        std::set<std::string> const& values() const;

    private:
        std::set<std::string> _values;
    };

    std::ostream& operator<<(std::ostream& stream, StyleHash const& hash);

}
#include "StyleHash.h"

#include <algorithm>

namespace gpugraph
{
    
    namespace
    {
        std::string const empty_string;
    }

    std::string const& StyleHash::key() const
    {
        return _values.empty() ? empty_string : *_values.begin();
    }

    bool StyleHash::contains(std::string const &item) const
    {
        return _values.count(item) > 0;
    }
    
    void StyleHash::insert_type(std::string type)
    {
        _values.insert(type);
    }
    
    void StyleHash::insert_class(std::string const& class_)
    {
        _values.insert("." + class_);
    }

    void StyleHash::erase_type(std::string const& type)
    {
        _values.erase(type);
    }

    void StyleHash::erase_id(std::string const& id)
    {
        _values.erase("#" + id);
    }

    void StyleHash::erase_class(std::string const& class_)
    {
        _values.erase("." + class_);
    }

    bool StyleHash::is_subset_of(StyleHash const& other) const
    {
        return (std::all_of(_values.begin(), _values.end(), [&](auto& value) {
            return other._values.count(value) > 0;
        }));
    }

    std::set<std::string> const& StyleHash::values() const
    {
        return _values;
    }

    void StyleHash::insert_id(std::string const& id)
    {
        _values.insert("#" + id);
    }

    std::ostream& operator<<(std::ostream& stream, StyleHash const& hash)
    {
        for (auto& value : hash.values())
            stream << value;
        return stream;
    }

}
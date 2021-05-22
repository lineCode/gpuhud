#pragma once

#include <functional>
#include <string>
#include <memory>
#include <unordered_map>

namespace gpugraph
{

    class Attributed
    {
    public:
        using AttributeGetter = std::function<std::string(void)>;
        using AttributeSetter = std::function<void(std::string)>;

        class Attribute;

        void add_attribute(std::string name, AttributeGetter, AttributeSetter);
        void remove_attribute(std::string const&);
        
        void set_attribute(std::string const& name, std::string value);
        std::string attribute(std::string const&) const;

    private:
        std::unordered_map<std::string, std::pair<AttributeGetter, AttributeSetter>> _attributes;
    };

}

#include "Attributed.h"

namespace gpugraph
{

    void Attributed::add_attribute(std::string name, AttributeGetter getter, AttributeSetter setter)
    {
        _attributes[std::move(name)] = { std::move(getter), std::move(setter) };
    }

    void Attributed::remove_attribute(std::string const& name)
    {
        _attributes.erase(name);
    }

    void Attributed::set_attribute(std::string const& name, std::string value)
    {
        auto it = _attributes.find(name);
        if (it == _attributes.end())
        {
            auto _value = std::make_shared<std::string>();
            _attributes[name] = { [_value]() {
                return *_value;
            }, [_value](std::string value) {
                *_value = std::move(value);
            } };
        }
        else
        {
            it->second.second(std::move(value));
        }
    }

    std::string Attributed::attribute(std::string const& name) const
    {
        auto it = _attributes.find(name);
        if (it == _attributes.end())
        {
            return std::string();
        }
        else
        {
            return it->second.first();
        }
    }

}

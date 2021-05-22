/***************************************************************************//*/

Copyright (c) 2021 Martin Rudoff

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/******************************************************************************/
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
            return std::string();
        else
            return it->second.first();
    }

}

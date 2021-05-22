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

    void Attributed::define_attribute(std::string name, AttributeGetter getter, AttributeSetter setter)
    {
        _attributes[std::move(name)] = { std::move(getter), std::move(setter)};
    }

    void Attributed::set_attribute(std::string const& name, std::optional<std::string> value)
    {
        auto it = _attributes.find(name);
        if (it == _attributes.end())
        {
            auto _value = std::make_shared<std::string>();
            _attributes[name] = { [=]() {
                return std::optional<std::string>(*_value);
            }, [=](std::optional<std::string> value) {
                if (value)
                    *_value = std::move(value.value());
                else
                    _attributes.erase(name);
            } };
            on_after_dynamic_attribute_changed();
        }
        else
        {
            it->second.setter(std::move(value));
        }
    }

    std::optional<std::string> Attributed::attribute(std::string const& name) const
    {
        auto it = _attributes.find(name);
        if (it == _attributes.end())
            return std::optional<std::string>();
        return it->second.getter();
    }

}

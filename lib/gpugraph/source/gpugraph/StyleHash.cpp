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
        std::string separator;
        for (auto& value : hash.values())
        {
            stream << separator << value;
            separator = " ";
        }
        return stream;
    }

}
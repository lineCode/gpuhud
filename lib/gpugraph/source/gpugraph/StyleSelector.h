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
#pragma once

#include <set>
#include <functional>
#include <css/selector.hpp>

#include "StyleHash.h"

namespace gpugraph
{

    class Node;

    class StyleSelector
    {
    public:
        struct PathItem 
        {
            using PseudoSelector = std::function<bool(Node const&)>;
            css::selector::combinator combinator;
            StyleHash style_hash;
            std::vector<PseudoSelector> pseudo_selectors;
        };
        using Path = std::vector <PathItem>;

        StyleSelector(css::selector const&);
        css::selector const& definition() const;

        // https://www.w3.org/TR/2011/REC-CSS2-20110607/cascade.html#specificity
        std::uint64_t specificity() const;
        Path const& path() const;
        bool is_applicable_to(Node const&) const;

    private:
        css::selector _definition;
        Path _path;
        std::uint64_t _specificity = 0;
    };

}

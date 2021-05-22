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

#include <css/parser.hpp>

#include "StyleRule.h"

namespace gpugraph
{

    class StyleSelector;
    class StyleBlock;

    class StyleCompiler : public css::parser::handler
    {
    public:
        using Sink = std::function<void(std::shared_ptr<StyleBlock>)>;
        StyleCompiler(Sink);

        void compile(std::string const&);

        void handle_begin_block(css::selector_list) override;
        void handle_end_block() override;

        void handle_left(css::distance) override;
        void handle_top(css::distance) override;
        void handle_right(css::distance) override;
        void handle_bottom(css::distance) override;

    private:
        Sink _sink;

        std::vector<std::shared_ptr<StyleSelector>> _selectors;
        std::vector<StyleRule> _rules;
    };


}

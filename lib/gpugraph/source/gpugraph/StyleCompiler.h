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

        void handle_position(css::cascadable<css::position>) override;
        void handle_display(css::cascadable<css::display>) override;

        void handle_width(css::cascadable<css::distance>) override;
        void handle_height(css::cascadable<css::distance>) override;

        void handle_left(css::cascadable<css::distance>) override;
        void handle_top(css::cascadable<css::distance>) override;
        void handle_right(css::cascadable<css::distance>) override;
        void handle_bottom(css::cascadable<css::distance>) override;

        void handle_font_style(css::cascadable<css::font_style>) override;
        void handle_font_size(css::cascadable<css::font_size>) override;
        void handle_font_family(css::cascadable<css::font_family>) override;

        void handle_color(css::cascadable<css::color>) override;
        void handle_background_color(css::cascadable<css::background_color>) override;

    private:
        Sink _sink;

        std::vector<std::shared_ptr<StyleSelector>> _selectors;
        std::vector<StyleRule> _rules;
    };


}

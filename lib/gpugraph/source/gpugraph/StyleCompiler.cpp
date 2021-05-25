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

#include "StyleCompiler.h"
#include "StyleSelector.h"
#include "StyleBlock.h"
#include "ComputedStyleSet.h"

#include <algorithm>

#include "log.h"

namespace gpugraph
{


    namespace
    {

        template<typename ComputedType, typename CssType>
        StyleRule make_cascade_rule(css::cascadable<CssType> cascadable)
        {
            if (std::holds_alternative<css::cascade>(cascadable))
            {
                switch (std::get<css::cascade>(cascadable))
                {
                case css::cascade::inherit:
                    return [=](auto const& parent_set, auto& set) {
                        set.insert(ComputedType::Identifier, parent_set.get<ComputedType>());
                    };
                case css::cascade::initial:
                    return [=](auto const&, auto& set) {
                        set.insert(ComputedType::Identifier, ComputedStyleSet::Initial.get<ComputedType>());
                    };
                case css::cascade::unset:
                    return [=](auto const& parent_set, auto& set) {
                        set.unset(ComputedType::Identifier);
                    };
                };
                throw ""; // make the compiler happy
            }
            else return[cascadable{std::move(cascadable)}](auto const& parent_set, auto& set)
            {
                set.insert(ComputedType::Identifier, std::make_shared<ComputedType>(std::get<CssType>(cascadable)));
            };
        }

    }

    StyleCompiler::StyleCompiler(Sink sink)
        : _sink(std::move(sink))
    {
    }

    void StyleCompiler::handle_begin_block(css::selector_list selectors)
    {
        _rules.clear();
        _selectors.clear();
        _selectors.resize(selectors.size());
        std::transform(selectors.begin(), selectors.end(), _selectors.begin(), [&](auto& source) {
            return std::make_shared<StyleSelector>(source);
        });
    }

    void StyleCompiler::handle_end_block()
    {
        for (auto& selector : _selectors)
        {
            //
            // compile selector and create block
            //
            // insert block, empty hash values are allowed
            _sink(std::make_shared<StyleBlock>(std::move(selector), _rules));
        }
        _rules.clear();
        _selectors.clear();
    }

    void StyleCompiler::handle_position(css::cascadable<css::position> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::Position>(std::move(cascadable)));
    }

    void StyleCompiler::handle_display(css::cascadable<css::display> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::Display>(std::move(cascadable)));
    }

    void StyleCompiler::handle_width(css::cascadable<css::distance> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::Width>(std::move(cascadable)));
    }

    void StyleCompiler::handle_height(css::cascadable<css::distance> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::Height>(std::move(cascadable)));
    }

    void StyleCompiler::handle_left(css::cascadable<css::distance> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::Left>(std::move(cascadable)));
    }

    void StyleCompiler::handle_top(css::cascadable<css::distance> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::Top>(std::move(cascadable)));
    }

    void StyleCompiler::handle_right(css::cascadable<css::distance> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::Right>(std::move(cascadable)));
    }

    void StyleCompiler::handle_bottom(css::cascadable<css::distance> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::Bottom>(std::move(cascadable)));
    }

    void StyleCompiler::handle_font_style(css::cascadable<css::font_style> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::FontStyle>(std::move(cascadable)));
    }

    void StyleCompiler::handle_font_size(css::cascadable<css::font_size> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::FontSize>(std::move(cascadable)));
    }

    void StyleCompiler::handle_font_family(css::cascadable<css::font_family> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::FontFamily>(std::move(cascadable)));
    }

    void StyleCompiler::handle_color(css::cascadable<css::color> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::Color>(std::move(cascadable)));
    }

    void StyleCompiler::handle_background_color(css::cascadable<css::background_color> cascadable)
    {
        _rules.push_back(make_cascade_rule<ComputedStyle::BackgroundColor>(std::move(cascadable)));
    }

    void StyleCompiler::compile(std::string const& source)
    {
        css::parser().parse(source, *this);
    }

}

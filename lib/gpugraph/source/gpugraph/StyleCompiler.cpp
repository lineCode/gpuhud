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

#include <algorithm>

#include "log.h"

namespace gpugraph
{

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

    void StyleCompiler::handle_left(css::distance distance)
    {
        if (std::holds_alternative<css::cascade>(distance))
        {
            _rules.push_back([=](Node& node) {
                // TODO: implement/apply cascade rule
            });
        }
    }

    void StyleCompiler::handle_top(css::distance)
    {
        // ...
    }

    void StyleCompiler::handle_right(css::distance)
    {
        // ...
    }

    void StyleCompiler::handle_bottom(css::distance)
    {
        // ...
    }

    void StyleCompiler::compile(std::string const& source)
    {
        css::parser().parse(source, *this);
    }

}

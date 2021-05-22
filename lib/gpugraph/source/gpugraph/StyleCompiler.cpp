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

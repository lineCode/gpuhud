#include "StyleCompiler.h"
#include "StyleSelector.h"

#include <algorithm>

#include "log.h"

namespace gpugraph
{

    Style::Compiler::Compiler(Style& style)
        : _style(style)
    {
    }

    void Style::Compiler::handle_begin_block(css::selector_list selectors)
    {
        _styling.clear();
        _selectors.clear();
        _selectors.resize(selectors.size());
        std::transform(selectors.begin(), selectors.end(), _selectors.begin(), [&](auto& source) {
            return std::make_shared<Selector>(source);
        });
    }

    void Style::Compiler::handle_end_block()
    {
        for (auto& selector : _selectors)
        {
            //
            // compile selector and create block
            auto block = std::make_shared<Block>(std::move(selector), _styling);
            //
            // insert block, empty hash values are allowed
            _style._hash[block->selector->path().back().style_hash.key()].insert(block);
        }
        _styling.clear();
        _selectors.clear();
    }

}

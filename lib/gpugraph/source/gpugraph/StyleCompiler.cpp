#include "StyleCompiler.h"
#include "StyleSelector.h"

namespace gpugraph
{

    Style::Compiler::Compiler(Style& style)
        : _style(style)
    {
    }

    void Style::Compiler::handle_begin_block(css::selector_list selectors)
    {
        _selectors.resize(selectors.size());
        std::transform(selectors.begin(), selectors.end(), _selectors.begin(), [](auto& source) {
            return std::make_shared<Selector>(std::move(source));
        });
    }

    void Style::Compiler::handle_end_block()
    {
        _selectors.clear();
    }

}

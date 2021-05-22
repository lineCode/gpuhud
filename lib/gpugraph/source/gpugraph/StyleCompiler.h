#pragma once

#include <css/parser.hpp>

#include "StyleRule.h"

namespace gpugraph
{

    struct StyleSelector;
    struct StyleBlock;

    class StyleCompiler : public css::parser::handler
    {
    public:
        using Sink = std::function<void(std::shared_ptr<StyleBlock>)>;
        StyleCompiler(Sink);

        void handle_begin_block(css::selector_list) override;
        void handle_end_block() override;

        void handle_left(css::distance) override;
        void handle_top(css::distance) override;
        void handle_right(css::distance) override;
        void handle_bottom(css::distance) override;

    private:
        Sink _sink;

        // 
        // compile state
        std::vector<std::shared_ptr<StyleSelector>> _selectors;
        std::vector<StyleRule> _rules;
    };


}

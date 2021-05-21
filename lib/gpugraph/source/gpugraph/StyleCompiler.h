#pragma once
#include "Style.h"
#include "StyleCompiler.h"

#include <css/parser.hpp>

namespace gpugraph
{

    class Style::Compiler : public css::parser::handler
    {
    public:
        Compiler(Style&);

        void handle_begin_block(css::selector_list) override;
        void handle_end_block() override;

        void handle_left(css::distance) override;
        void handle_top(css::distance) override;
        void handle_right(css::distance) override;
        void handle_bottom(css::distance) override;

    private:
        Style& _style;
        
        // 
        // compile state
        std::vector<std::shared_ptr<Selector>> _selectors;
        std::vector<Rule> _rules;
    };


}

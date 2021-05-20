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

    private:
        std::vector<std::shared_ptr<Selector>> _selectors;
        Styling _styling;
        Style& _style;
    };


}

#pragma once

#include "StyleRule.h"
#include "StyleSelector.h"

namespace gpugraph
{

    struct StyleBlock
    {
        StyleBlock(std::shared_ptr<StyleSelector>, std::vector<StyleRule>);

        bool can_be_linked_to(Node const&);
        bool is_applicable_to(Node const&);

        std::shared_ptr<StyleSelector> selector;
        std::vector<StyleRule> rules;
    };

}

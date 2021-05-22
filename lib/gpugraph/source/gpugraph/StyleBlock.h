#pragma once

#include "StyleRule.h"
#include "StyleSelector.h"

namespace gpugraph
{

    class StyleBlock
    {
    public:
        StyleBlock(std::shared_ptr<StyleSelector>, std::vector<StyleRule>);

        bool can_be_linked_to(Node const&);
        bool is_applicable_to(Node const&);

        std::shared_ptr<StyleSelector> const& selector() const;
        std::vector<StyleRule> const& rules() const;

    private:
        std::shared_ptr<StyleSelector> _selector;
        std::vector<StyleRule> _rules;
    };

}

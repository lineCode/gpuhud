#include "StyleBlock.h"
#include "StyleSelector.h"
#include "Node.h"

namespace gpugraph
{

    StyleBlock::StyleBlock(std::shared_ptr<StyleSelector> selector, std::vector<StyleRule> rules)
        : selector(std::move(selector))
        , rules(std::move(rules))
    {
    }

    bool StyleBlock::can_be_linked_to(Node const& node)
    {
        return selector->path().back().style_hash.is_subset_of(node.style_hash());
    }

    bool StyleBlock::is_applicable_to(Node const& node)
    {
        return selector->is_selecting(node);
    }

}

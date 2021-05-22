#include "StyleBlock.h"
#include "StyleSelector.h"
#include "Node.h"

namespace gpugraph
{

    StyleBlock::StyleBlock(std::shared_ptr<StyleSelector> selector, std::vector<StyleRule> rules)
        : _selector(std::move(selector))
        , _rules(std::move(rules))
    {
    }

    bool StyleBlock::can_be_linked_to(Node const& node)
    {
        return _selector->path().back().style_hash.is_subset_of(node.style_hash());
    }

    bool StyleBlock::is_applicable_to(Node const& node)
    {
        return _selector->is_selecting(node);
    }

    std::shared_ptr<StyleSelector> const& StyleBlock::selector() const
    {
        return _selector;
    }

    std::vector<StyleRule> const& StyleBlock::rules() const
    {
        return _rules;
    }

}

#include "StyleBlock.h"
#include "StyleSelector.h"
#include "Node.h"

namespace gpugraph
{

    Style::Block::Block(std::shared_ptr<Selector> selector, std::vector<Rule> rules)
        : selector(std::move(selector))
        , rules(std::move(rules))
    {
    }

    bool Style::Block::can_be_linked_to(Node const& node)
    {
        return selector->path().back().style_hash.is_subset_of(node.style_hash());
    }

    bool Style::Block::is_applicable_to(Node const& node)
    {
        return selector->is_selecting(node);
    }

}

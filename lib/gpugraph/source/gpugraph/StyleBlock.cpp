#include "StyleBlock.h"
#include "StyleSelector.h"
#include "Node.h"

namespace gpugraph
{

    Style::Block::Block(std::shared_ptr<Selector> selector, Styling styling)
        : selector(std::move(selector))
        , styling(std::move(styling))
    {
    }

    bool Style::Block::can_be_linked_to(Node const& node)
    {
        return selector->path().back().style_hash.is_subset_of(node.style_hash());
    }

}
